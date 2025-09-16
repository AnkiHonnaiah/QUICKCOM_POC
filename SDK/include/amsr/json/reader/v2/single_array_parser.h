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
/**       \file
 *        \brief    A specialized parser to parse a single array of elements.
 *
 *        \details  Provides a callback for every element it encounters and keeps track of the nesting depth.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_ARRAY_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_ARRAY_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/json/reader/internal/level_validator.h"
#include "amsr/json/reader/v2/parser.h"

namespace amsr {
namespace json {
namespace v2 {
/*!
 * \brief A parser for a single array, i.e. an array that does not contain arrays as its elements.
 *
 * \details Handles the OnStartArray & OnEndArray callbacks by itself. Calls OnElement for every element it encounters
 *          and Finalize on array end.
 *
 * \vpublic
 */
class SingleArrayParser : public v2::Parser {
 private:
  /*! \brief Validates if the only one level of array has been entered. */
  internal::LevelValidator validator_{};
  /*! \brief Index for the array elements. */
  std::size_t index_{0};

 public:
  /*! \brief Inherited CTOR. */
  using v2::Parser::Parser;

  /*!
   * \brief Default event for the start of arrays.
   * \return kRunning if not in an array, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if already in an array}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   *
   * \internal
   * - If not inside an array:
   *   - Take a snapshot of the current state in case it has to be restored because this opening bracket is followed by
   *     a value.
   *   - Return kRunning to continue parsing.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto OnStartArray() noexcept -> ParserResult final {
    return this->validator_
        .Enter()
        // VCA_VAJSON_INTERNAL_CALL
        .AndThen([this](ParserState) { return this->GetJsonDocument().Snap(); })
        .Map([]() { return ParserState::kRunning; });
  }

  /*!
   * \brief Default event for the end of arrays.
   * \return kRunning if in an array, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if not in an array}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   *
   * \internal
   * - If inside an array:
   *   - Call the Finalize callback and return its Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto OnEndArray(std::size_t) noexcept -> ParserResult final {
    return this->validator_.Leave().Filter([this](ParserState) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      return this->Finalize();
    });
  }

  /*!
   * \brief Default event for unexpected elements that aborts the parsing.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if there is no callback registered for the event}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   *
   * \internal
   * - If not inside an array, return an error.
   * - Restore the last snapshot.
   * - If restoring the snapshot failed:
   *   - Return the error.
   * - Otherwise:
   *   - Call OnElement and return its Result.
   *   - On success, take a new snapshot, because the value could be followed by another value, and increase the index.
   * \endinternal
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult final {
    return MakeResult(this->validator_.IsInside(), JsonErrc::kUserValidationFailed,
                      "Expected to parse an array of elements.")
        // VCA_VAJSON_INTERNAL_CALL
        .AndThen([this]() { return this->GetJsonDocument().Restore(); })
        .AndThen([this]() {
          // VCA_VAJSON_THIS_DEREF
          return this->OnElement();
        })
        // VCA_VAJSON_INTERNAL_CALL
        .Filter([this](ParserState) { return this->GetJsonDocument().Snap(); })
        .Inspect([this](ParserState) { this->index_++; });
  }

  /*! \brief Returns the array index of the current element.
   * \return Array index of the current element.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetIndex() const noexcept -> std::size_t { return this->index_; }

 protected:
  /*!
   * \brief Event for array Elements to be implemented by the user.
   * \details This function is expected to consume all tokens representing the element, unless it returns
   *          an error Result or ParserState::kFinished.
   * \return -
   * \spec
   *   requires true;
   * \endspec
   */
  virtual auto OnElement() noexcept -> ParserResult = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_JSON_AutosarC++17_10-M9.3.3_non_const_method
  /*!
   * \brief Default event if the entire array has been successfully parsed.
   * \return The empty Result.
   *
   * \details Default implementation does nothing. User implementation could be used to validate parser results or
   fill
   *          in inout parameters etc.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  virtual auto Finalize() noexcept -> Result<void> { return Result<void>{}; }
};
}  // namespace v2
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_ARRAY_PARSER_H_
