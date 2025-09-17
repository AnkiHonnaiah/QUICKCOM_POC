/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file     depth_counter.h
 *        \brief    Keeps track of the nesting depth inside the JSON document.
 *
 *        \details  Provides methods to check and modify the current nesting depth.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_DEPTH_COUNTER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_DEPTH_COUNTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <stack>
#include <utility>

#include "amsr/core/string.h"

#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/json_error_domain.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief A counter to track the nesting depth of the JSON parser.
 *
 * \details -
 */
class DepthCounter final {
  /*!
   * \brief Stores the stack items.
   * \details Intentionally a string to make use of SSO.
   * \todo Replace with stack allocated object.
   */
  std::stack<char, amsr::core::String> stack_{};
  /*! \brief Stores the element count of arrays and objects on the stack. */
  std::stack<std::size_t> counter_{};
  /*! \brief Flag to indicate if a comma must appear before the next value. */
  bool comma_expected_{false};
  /*! \brief Flag to indicate if all elements are closed. */
  bool is_finished_{false};

 public:
  /*!
   * \brief CTOR.
   * \details Cannot be defaulted because then it gets deleted by the compiler.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  DepthCounter() noexcept {}

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Move constructor.
   * \param that The other DepthCounter to move from.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  DepthCounter(DepthCounter&& that) noexcept : stack_{std::move(that.stack_)}, counter_{std::move(that.counter_)} {}

  /*!
   * \brief Default copy constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  DepthCounter(DepthCounter const&) noexcept = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Move assignment.
   * \return Reference to the moved to object.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_MOLE_1298
  auto operator=(DepthCounter&&) & noexcept -> DepthCounter& = default;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_noexcept_DepthCounter
  /*!
   * \brief Default copy assignment.
   * \return Reference to the copied object.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_MOLE_1298
  auto operator=(DepthCounter const&) & noexcept(false) -> DepthCounter& = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default destructor.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_MOLE_1298
  ~DepthCounter() noexcept = default;

  /*!
   * \brief Checks if an EOF is correct.
   * \return The empty Result if the stack is empty, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no EOF was encountered.}
   *
   * \details An EOF is correct when there are no more elements on the stack, i.e. no arrays, objects, or keys are left
   *          open, or there is only a single value, in case the document consists of only a single value.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If there are no elements in the stack or there is only a single value:
   *   - Return an empty Result.
   * - Otherwise return kInvalidJson.
   * \endinternal
   */
  auto CheckEndOfFile() const noexcept -> ParserResult {
    return MakeResult(this->is_finished_,
                      [this]() {
                        ErrorCode error_code{JsonErrc::kInvalidJson, "DepthCounter::CheckEndOfFile: Empty document."};
                        if (not this->IsEmpty()) {
                          // VCA_VAJSON_EXTERNAL_CALL
                          error_code.SetUserMessage((this->stack_.top() == '[')
                                                        ? "DepthCounter::CheckEndOfFile: Expected Closing Brackets."
                                                        : "DepthCounter::CheckEndOfFile: Expected Closing Braces.");
                        }
                        return error_code;
                      })
        .Map([]() { return ParserState::kFinished; });
  }

  /*!
   * \brief Adds an Array to the stack.
   * \return The empty Result if an array can be added, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a key was expected}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddArray() noexcept -> Result<void> { return this->AddElement('['); }

  /*!
   * \brief Adds an Object to the stack.
   * \return The empty Result if an object can be added, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a key was expected}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddObject() noexcept -> Result<void> { return this->AddElement('{'); }

  /*!
   * \brief Adds a single key to the stack.
   * \return The empty Result if a key can be added, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a value was expected}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the last element is an object:
   *   - Add a key to the stack.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   */
  auto AddKey() noexcept -> Result<void> {
    Result<void> result{};
    if (this->is_finished_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddKey: Multiple top level elements.");
    } else if (this->comma_expected_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddKey: Missing comma.");
    } else if (this->CheckLastElement('{')) {
      // VCA_VAJSON_EXTERNAL_CALL
      this->stack_.push('k');
    } else {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddKey: Expected a value.");
    }
    return result;
  }

  /*!
   * \brief Adds a single value to the stack.
   * \return The empty Result if a value can be added, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a key was expected}
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a value is already present on the stack}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the stack contains elements:
   *   - If the last element is an array:
   *     - Increase the element count for the array.
   *   - Else if the last element is a key:
   *     - Pop the key, because the pair is complete.
   *     - Increase the count, because if a value follows a key, the enclosing element must be an object.
   *   - Otherwise:
   *     - Return an error.
   * - Otherwise:
   *   - Return an empty Result, because a single value is valid JSON.
   * \endinternal
   */
  auto AddValue() noexcept -> Result<void> {
    Result<void> result{};

    if (this->is_finished_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddValue: Multiple top level elements.");
    } else if (this->comma_expected_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddValue: Missing comma.");
    } else if (not this->IsEmpty()) {
      this->comma_expected_ = true;

      // We now know that the stack is not empty, so there's no need to use CheckLastElement.
      // VCA_VAJSON_EXTERNAL_CALL
      char const last{this->stack_.top()};
      switch (last) {
        case '[':
          // VCA_VAJSON_EXTERNAL_CALL
          ++this->counter_.top();
          break;
        case 'k':
          // VCA_VAJSON_EXTERNAL_CALL
          this->stack_.pop();
          // VCA_VAJSON_EXTERNAL_CALL
          ++this->counter_.top();
          break;
        default:
          // VCA_VAJSON_EXTERNAL_CALL
          result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddValue: Expected a key.");
          break;
      }
    } else {
      // VCA_VAJSON_THIS_DEREF
      this->is_finished_ = true;
    }

    return result;
  }

  /*!
   * \brief Pops an Object from the stack.
   * \return The number of keys in the object if the end object call was valid, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, If the object cannot be closed.}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the last element is an object:
   *   - Pop the Object from the stack.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto PopObject() noexcept -> Result<std::size_t> {
    bool const expected_key{this->CheckLastElement('{')};
    return MakeResult(expected_key, JsonErrc::kInvalidJson, "DepthCounter::PopObject: Not in an object.").Map([this]() {
      this->comma_expected_ = true;
      return this->Pop();
    });
  }

  /*!
   * \brief Pops an Array from the stack.
   * \return The number of elements in the array if the end array call was valid, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if in an object or on toplevel.}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the last element is an array:
   *   - Pop the Array from the stack.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto PopArray() noexcept -> Result<std::size_t> {
    bool const expected_key{this->CheckLastElement('[')};
    return MakeResult(expected_key, JsonErrc::kInvalidJson, "DepthCounter::PopArray: Not in an array.").Map([this]() {
      this->comma_expected_ = true;
      return this->Pop();
    });
  }

  /*!
   * \brief Adds a comma.
   * \return True if a comma at the current position is valid, or false otherwise.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If a comma is expected:
   *   - Reset the flag.
   * - If the stack is empty:
   *   - Return false.
   * - If the previous element is not a key:
   *   - Return true, because in this case we must be inside an array or object and empty elements are fine.
   * - Otherwise:
   *   - Return false.
   * \endinternal
   */
  auto AddComma() noexcept -> bool {
    // Found at least the expected comma, ignore any succeeding commas (i.e. empty elements).
    if (this->comma_expected_) {
      this->comma_expected_ = false;
    }

    // VCA_VAJSON_EXTERNAL_CALL
    return ((not this->IsEmpty()) && (this->stack_.top() != 'k'));
  }

 private:
  /*!
   * \brief Checks if the stack is empty.
   * \return True if empty.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsEmpty() const noexcept -> bool { return this->stack_.empty(); }

  /*!
   * \brief Pops an element from the stack.
   * \return The value of the removed element.
   * \details Calling on empty counter invokes undefined behaviour.
   *
   * \context ANY
   * \pre The stack contains at least one element.
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Remove the element at the last position from the stack.
   * - Remove the counter at the last position and return its value.
   * - If the element was inside another element, increase the outer element's count.
   * \endinternal
   */
  auto Pop() noexcept -> std::size_t {
    // VCA_VAJSON_EXTERNAL_CALL
    std::size_t const count{this->counter_.top()};
    // VCA_VAJSON_EXTERNAL_CALL
    this->counter_.pop();
    // VCA_VAJSON_EXTERNAL_CALL
    this->stack_.pop();

    if (this->CheckLastElement('[') || this->CheckLastElement('{')) {
      // VCA_VAJSON_EXTERNAL_CALL
      ++this->counter_.top();
    }

    this->CheckIfFinished();

    return count;
  }

  /*!
   * \brief Adds an element to the stack.
   * \param element The element to add.
   * \return The empty Result if the element could be added, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a key was expected}
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if a value is already present on the stack}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the last element is not an object and not a value:
   *   - If the last element is a key, pop it from the stack, because the pair is complete.
   *   - Add the element to the stack.
   *   - Add a new counter to the stack.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto AddElement(char element) noexcept -> Result<void> {
    Result<void> result{};

    if (this->is_finished_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddElement: Multiple top level elements.");
    } else if (this->CheckLastElement('{')) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddElement: Expected a key.");
    } else if (this->comma_expected_) {
      // VCA_VAJSON_EXTERNAL_CALL
      result.EmplaceError(JsonErrc::kInvalidJson, "DepthCounter::AddElement: Expected a comma.");
    } else {
      if (this->CheckLastElement('k')) {
        // VCA_VAJSON_EXTERNAL_CALL
        this->stack_.pop();
      }
      // VCA_VAJSON_EXTERNAL_CALL
      this->stack_.push(element);
      // VCA_VAJSON_EXTERNAL_CALL
      this->counter_.push(0);
    }

    return result;
  }

  /*!
   * \brief Compares the given element to the last element on the stack if it is not empty.
   * \param item to check for.
   * \return True if the last element equals the given item.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckLastElement(char item) const noexcept -> bool {
    // VCA_VAJSON_EXTERNAL_CALL
    return (not this->IsEmpty()) && (this->stack_.top() == item);
  }

  /*! \brief Sets the finished flag if the stack is empty.
   * \spec
   *   requires true;
   * \endspec
   */
  void CheckIfFinished() noexcept {
    if (this->IsEmpty()) {
      this->is_finished_ = true;
    }
  }
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_DEPTH_COUNTER_H_
