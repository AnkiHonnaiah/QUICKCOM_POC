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
/**       \file     json_parser.h
 *        \brief    Contains a parser for stateful JSON files.
 *
 *        \details  Provides methods to parse JSON elements and allows for chaining.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader_fwd.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {

/*!
 * \brief A parser that can be used to parse stateful JSON files.
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Reader-Method-Based-Parser
 */
class JsonParser final {
 public:
  /*!
   * \brief Constructs the parser.
   * \param data The JSON data.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit JsonParser(JsonData& data) noexcept;

  /*!
   * \brief Gets the current status of the parser.
   * \return The empty Result, or the error that occurred.
   *
   * \details Does not change the internal state of the parser and thus may be called multiple times.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if checking for a specific type but the next token is not
   *        of that type}
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if checking for a specific name of a key or string but the
   *        next key or string has a different name}
   * \error{A custom error domain and error code, if the Result has been customized using AddErrorInfo}
   * \error{The error of the callable, if the callable given to a function returned an error}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If no error occurred:
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return the error.
   * \endinternal
   */
  auto Validate() const noexcept -> Result<void> { return this->result_; }

  /*!
   * \brief Returns a reference to the JSON file.
   * \return A reference to the JSON file.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetJsonDocument() noexcept -> JsonData& { return *this->data_; }

  /*!
   * \brief Returns a reference to the JSON file.
   * \return A reference to the JSON file.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetJsonDocument() const noexcept -> JsonData const& { return *this->data_; }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  // VECTOR NC AutosarC++17_10-M5.2.12: MD_JSON_AutosarC++17_10-M5.2.12_array_to_pointer_decay
  /*!
   * \brief Checks if the next token is a key and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the name of the key as an ara::core::StringView and return void or Result<void>.
   *          The provided StringView is only valid until any other method or parser operating on the same document is
   *          called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the key could be parsed successfully:
   *   - Execute the given callable with the key.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto Key(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.Key(fn).Drop(); });
  }

  /*!
   * \brief Checks if the next token is a specific key.
   * \param key to compare with.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If no error occurred before and the key could be parsed successfully:
   *   - Compare the parsed key with the given one.
   * - If the keys do not match or an error occurred, store it as the state of the parser.
   * \endinternal
   */
  auto Key(ara::core::StringView key) noexcept -> JsonParser& {
    return this->IfValid([this, &key]() { return this->parser_.Key(key); });
  }

  /*!
   * \copydoc Key(ara::core::StringView)
   * \spec
   *   requires true;
   * \endspec
   */
  auto Key(CStringView key) noexcept -> JsonParser& { return this->Key(ara::core::StringView(key)); }

  /*!
   * \copydoc Key(ara::core::StringView)
   * \spec
   *   requires true;
   * \endspec
   */
  auto Key(ara::core::String const& key) noexcept -> JsonParser& { return this->Key(CStringView::FromString(key)); }

  /*!
   * \brief Checks if the next token is the start of an object.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto StartObject() noexcept -> JsonParser&;

  /*!
   * \brief Checks if the next token is the end of an object.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto EndObject() noexcept -> JsonParser&;

  /*!
   * \brief Checks if the next token is the start of an array.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto StartArray() noexcept -> JsonParser&;

  /*!
   * \brief Checks if the next token is the end of an array.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto EndArray() noexcept -> JsonParser&;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is a bool and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the bool and return void or Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the bool could be parsed successfully:
   *   - Execute the given callable with the bool.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto Bool(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.Bool(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is a string and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the string as an ara::core::StringView and return void or Result<void>. The
   *          provided StringView is only valid until any other method or parser operating on the same document is
   *          called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the string could be parsed successfully:
   *   - Execute the given callable with the string.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto String(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.String(fn).Drop(); });
  }

  /*!
   * \brief Checks if the next token is the expected string.
   * \param string to expect.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If no error occurred before and the string could be parsed successfully:
   *   - Compare the parsed string with the given one.
   * - If the strings do not match or an error occurred, store it as the state of the parser.
   * \endinternal
   */
  auto String(ara::core::StringView string) noexcept -> JsonParser& {
    return this->IfValid([this, &string]() { return this->parser_.String(string).Drop(); });
  }

  /*!
   * \copydoc String(ara::core::StringView)
   * \spec
   *   requires true;
   * \endspec
   */
  auto String(CStringView string) noexcept -> JsonParser& { return this->String(ara::core::StringView(string)); }

  /*!
   * \copydoc String(ara::core::StringView)
   * \spec
   *   requires true;
   * \endspec
   */
  auto String(ara::core::String const& string) noexcept -> JsonParser& {
    return this->String(CStringView::FromString(string));
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is a number and executes the given callable.
   * \tparam Num Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the number of template type Num and return void or Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the number could be parsed successfully:
   *   - Execute the given callable with the number.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Num, typename Fn>
  auto Number(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.Number<Num>(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is binary content and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the binary content as ara::core::Span<char> and return Result<void>. The provided
   *          Span is only valid until any other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the binary content could be parsed successfully:
   *   - Execute the given callable with the string.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto Binary(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.Binary(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is an array and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the current array index as a std::size_t and return void or Result<void>.
   *          The callable is expected to have consumed all tokens representing the element if it returns a successful
   *          Result or has return type void. If the callable is unable to consume all tokens it must return an
   *          error Result.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the array element could be parsed successfully:
   *   - Execute the given callable with the array index.
   *   - Repeat for all elements of the array.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto Array(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.Array(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is an array of strings and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the current array index as a std::size_t, the current string as an
   *          ara::core::StringView, and return void or Result<void>. The provided StringView is only valid until any
   *          other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the array element could be parsed successfully:
   *   - Execute the given callable with the array index and the parsed string.
   *   - Repeat for all elements of the array.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto StringArray(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.StringArray(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is an array of numbers and executes the given callable on every element.
   * \tparam Num Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the current array index as a std::size_t, the current number of template type Num,
   *          and return void or Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the array element could be parsed successfully:
   *   - Execute the given callable with the array index and the parsed number.
   *   - Repeat for all elements of the array.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Num, typename Fn>
  auto NumberArray(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.NumberArray<Num>(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is an array of bools and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return The reference to itself.
   *
   * \details The callable must take the current array index as a std::size_t, the current bool value, and return void
   *          or Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If no error occurred before and the array element could be parsed successfully:
   *   - Execute the given callable with the array index and the parsed bool.
   *   - Repeat for all elements of the array.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto BoolArray(Fn const& fn) noexcept -> JsonParser& {
    return this->IfValid([this, &fn]() { return this->parser_.BoolArray(fn).Drop(); });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Checks if the next token is an object and executes the given callable on every key.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \param object_already_open Specify if the object has already been opened. Defaults to false.
   * \return The reference to itself.
   *
   * \details The callable must take the name of the current key as an ara::core::StringView and return void or
   *          Result<void>. The provided StringView is only valid until any other method or parser operating on the same
   *          document is called.
   *          The callable is expected to have consumed all tokens representing the value related to the key if it
   *          returns a successful Result or has return type void. If the callable is unable to consume all tokens it
   *          must return an error Result.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If no error occurred before and the objects key could be parsed successfully:
   *   - Execute the given callable with the objects key.
   *   - Repeat for all keys of the object.
   * - If an error occurred, store it as the state of the parser.
   * \endinternal
   */
  template <typename Fn>
  auto Object(Fn const& fn, bool object_already_open = false) noexcept -> JsonParser& {
    return this->IfValid(
        [this, &fn, object_already_open]() { return this->parser_.Object(fn, object_already_open).Drop(); });
  }

  /*!
   * \brief Adds a custom error message to the ParserResult.
   * \param msg to add. Must live until the ParserResult is evaluated.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the error message has not been customized yet:
   *   - Replace the UserMessage of the erroneous parser result with the given one.
   * \endinternal
   */
  auto AddErrorInfo(CStr msg) noexcept -> JsonParser& {
    if (ContainsStandardError()) {
      ErrorCode const ec{this->result_.Error().WithUserMessage(msg)};
      this->result_.EmplaceError(ec);
      this->customized_ = true;
    }
    return *this;
  }

  /*!
   * \copydoc AddErrorInfo
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddErrorInfo(CStringView msg) noexcept -> JsonParser& { return AddErrorInfo(msg.c_str()); }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Adds a custom error message to the ParserResult.
   * \tparam Args Constructor arguments for the error code.
   * \param args to construct the error code.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the error message has not been customized yet:
   *   - Replace the erroneous parser result with the error constructed from the given arguments.
   * \endinternal
   */
  template <typename... Args>
  auto AddErrorInfo(Args... args) noexcept -> JsonParser& {
    if (ContainsStandardError()) {
      // VCA_VAJSON_WITHIN_SPEC
      this->result_.EmplaceError(ErrorCode{std::forward<Args>(args)...});
      this->customized_ = true;
    }
    return *this;
  }

 private:
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Executes the function and saves the Result if the previous actions were successful.
   * \tparam Fn Type of function.
   * \param fn Function to execute.
   * \return The reference to itself.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If no error occurred before:
   *   - Execute the function and set the state of the parser from its return value.
   * \endinternal
   */
  template <typename Fn>
  auto IfValid(Fn const& fn) noexcept -> JsonParser& {
    if (this->result_) {
      // If a callable that returns void changes this->result_ to an error, the return value of any pre-defined parser
      // call is still just a positive Result<void>. Thus, even if the parser call returned successfully, this->result_
      // must be checked again and set because it might contain an error.
      this->result_ = fn().And(this->result_);
    }
    return *this;
  }

  /*!
   * \brief Checks the current error state of the parser.
   * \return True if the state contains a standard error, otherwise false.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the parser state contains an error and it has not been customized yet:
   *   - Return true.
   * - Otherwise:
   *   - Return false.
   * \endinternal
   */
  auto ContainsStandardError() const noexcept -> bool { return !(this->customized_ || this->result_); }

  /*!
   * \brief Internal parser.
   */
  internal::CompositionParser<internal::VirtualParser> parser_;

  /*!
   * \brief JSON data.
   */
  internal::Ref<JsonData> data_;

  /*!
   * \brief State of the parser.
   * \details Contains the first error that occurred.
   */
  Result<void> result_{};

  /*!
   * \brief Flag if the error message has already been customized.
   */
  bool customized_{false};
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_PARSER_H_
