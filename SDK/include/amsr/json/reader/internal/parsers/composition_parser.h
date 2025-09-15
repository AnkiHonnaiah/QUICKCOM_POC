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
/**       \file     composition_parser.h
 *        \brief    A parser that works through composition rather than inheritance.
 *
 *        \details  Provides pre-defined parsers for JSON elements.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader_fwd.h"
#include "amsr/json/util/json_error_domain.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief Returns bool if the function returns any Result.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ReturnsResult = amsr::core::is_result<std::result_of_t<F(T const&)>>;

/*!
 * \brief Returns bool if the function returns Result<void>.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ReturnsResultVoid = std::is_same<Result<void>, std::result_of_t<F(T const&)>>;

/*!
 * \brief Returns bool if the function returns void.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ReturnsVoid = std::is_void<std::result_of_t<F(T const&)>>;

/*!
 * \brief SFINAE for callable returning Result<...>.
 * \tparam F Type of callable.
 * \tparam T Type of argument.
 * \tparam Out Type of return value.
 */
template <typename F, typename T, typename Out>
using CallableReturnsResult = std::enable_if_t<ReturnsResult<F, T>::value, Out>;

/*!
 * \brief SFINAE for callable returning Result<...>.
 * \tparam F Type of callable.
 * \tparam T Type of argument.
 * \tparam Out Type of return value.
 */
template <typename F, typename T, typename Out>
using CallableReturnsNoResult = std::enable_if_t<!ReturnsResult<F, T>::value, Out>;

/*!
 * \brief Returns bool in case the function returns any Result type.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ArrayReturnsResult = amsr::core::is_result<std::result_of_t<F(std::size_t, T const&)>>;

/*!
 * \brief Returns bool in case the function returns a Result<void>.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ArrayReturnsResultVoid = std::is_same<Result<void>, std::result_of_t<F(std::size_t, T const&)>>;

/*!
 * \brief Returns bool in case the function returns void.
 * \tparam F Type of function.
 * \tparam T Type of argument.
 */
template <typename F, typename T>
using ArrayReturnsVoid = std::is_void<std::result_of_t<F(std::size_t, T const&)>>;

/*!
 * \brief  SFINAE for callable returning Result<...>.
 * \tparam F Type of callable.
 * \tparam T Type of argument.
 * \tparam Out Type of return value.
 */
template <typename F, typename T, typename Out>
using ArrayCallableReturnsResult = std::enable_if_t<ArrayReturnsResult<F, T>::value, Out>;

/*!
 * \brief  SFINAE for callable returning Result<...>.
 * \tparam F Type of callable.
 * \tparam T Type of argument.
 * \tparam Out Type of return value.
 */
template <typename F, typename T, typename Out>
using ArrayCallableReturnsNoResult = std::enable_if_t<!ArrayReturnsResult<F, T>::value, Out>;

/*!
 * \brief A parser that works through composition rather than inheritance.
 *
 * \details Provides pre-defined parsers for JSON elements.
 *
 * \vprivate Component private.
 *
 * \trace DSGN-JSON-Reader-Composition-Parser
 */
template <typename Mixin>
// VCA_VAJSON_MOLE_1298
class CompositionParser : public Mixin {
  /*!
   * \brief Shorthand for the ParserResult.
   */
  using R = ParserResult;

 public:
  /*!
   * \brief Constructs a CompositionParser.
   * \param doc JSON document to parse.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_INTERNAL_CALL
  explicit CompositionParser(JsonData& doc) noexcept : Mixin(doc), doc_(doc) {}

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following key value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no key comes next}
   *
   * \details The callable must take the name of the key as an ara::core::StringView and return void. The provided
   *          StringView is only valid until any other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the key could be parsed successfully:
   *   - Execute the callable with the key.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Key(Fn fn) noexcept -> CallableReturnsNoResult<Fn, ara::core::StringView, R> {
    static_assert(ReturnsVoid<Fn, ara::core::StringView>::value, "Must return void");
    return this->Key([&fn](ara::core::StringView s) {
      // VCA_VAJSON_WITHIN_SPEC
      std::forward<Fn>(fn)(s);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following key value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no key comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the name of the key as an ara::core::StringView and return Result<void>. The
   *          provided StringView is only valid until any other method or parser operating on the same document is
   *          called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the key could be parsed successfully:
   *   - Execute the callable with the key.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Key(Fn fn) noexcept -> CallableReturnsResult<Fn, ara::core::StringView, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following key value and checks if it is a specific key.
   * \param key to compare with.
   *
   * \return kRunning if the keys are the same.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if the names of the keys are not equal or no key comes
   *        next}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   *
   * \internal
   * - If the key could be parsed successfully:
   *   - Compare the parsed key with the given one.
   *   - If the keys match, return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto Key(ara::core::StringView key) noexcept -> R {
    return this->Key([&key](ara::core::StringView str_view) {
      return MakeResult(key == str_view, JsonErrc::kUserValidationFailed, "Incorrect key received");
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following bool value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no bool comes next}
   *
   * \details The callable must take the bool and return void.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the bool could be parsed successfully:
   *   - Execute the callable with the bool.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Bool(Fn fn) noexcept -> CallableReturnsNoResult<Fn, bool, R> {
    static_assert(ReturnsVoid<Fn, bool>::value, "Must return void");
    return this->Bool([&fn](bool b) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(b);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following bool value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no bool comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the bool and return Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the bool could be parsed successfully:
   *   - Execute the callable with the bool.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Bool(Fn fn) noexcept -> CallableReturnsResult<Fn, bool, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following number value and executes the given callable.
   * \tparam T Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no number comes next}
   *
   * \details The callable must take the number of type T and return void.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the number could be parsed successfully:
   *   - Execute the callable with the number.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename T, typename Fn>
  auto Number(Fn fn) noexcept -> CallableReturnsNoResult<Fn, T, R> {
    static_assert(ReturnsVoid<Fn, T>::value, "Must return void");
    return this->Number<T>([&fn](T n) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(n);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following number value and executes the given callable.
   * \tparam T Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no number comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the number of type T and return Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the number could be parsed successfully:
   *   - Execute the callable with the number.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename T, typename Fn>
  auto Number(Fn fn) noexcept -> CallableReturnsResult<Fn, T, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following string value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no string comes next}
   *
   * \details The callable must take the string as an ara::core::StringView and return void. The provided StringView is
   *          only valid until any other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the string could be parsed successfully:
   *   - Execute the callable with the string.
   *   - Return an empty Result.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto String(Fn fn) noexcept -> CallableReturnsNoResult<Fn, ara::core::StringView, R> {
    static_assert(ReturnsVoid<Fn, ara::core::StringView>::value, "Must return void");
    return this->String([&fn](ara::core::StringView s) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(s);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following string value and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no string comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the string as an ara::core::StringView and return Result<void>. The provided
   *          StringView is only valid until any other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the string could be parsed successfully:
   *   - Execute the callable with the string.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto String(Fn fn) noexcept -> CallableReturnsResult<Fn, ara::core::StringView, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following string value and checks if it is a specific string.
   * \param string to compare with.
   *
   * \return kRunning if the strings are the same.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if the strings are not equal or no string comes next}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   *
   * \internal
   * - Call the method String.
   *   - If the parsed string is equal to passed string return an empty result.
   *   - Otherwise return an error.
   * \endinternal
   */
  auto String(ara::core::StringView string) noexcept -> R {
    return this->String([&string](ara::core::StringView str_view) {
      return MakeResult(string == str_view, JsonErrc::kUserValidationFailed, "Incorrect string received");
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of numbers and executes the given callable on every element.
   * \tparam T Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of numbers comes next}
   *
   * \details The callable must take the current array index as a std::size_t, the current number of type T, and return
   *          void.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of numbers and execute the callable with every number.
   * \endinternal
   */
  template <typename T, typename Fn>
  auto NumberArray(Fn fn) noexcept -> ArrayCallableReturnsNoResult<Fn, T, R> {
    static_assert(ArrayReturnsVoid<Fn, T>::value, "Must return void");
    return this->NumberArray<T>([&fn](std::size_t n, T number) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(n, number);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of numbers and executes the given callable on every element.
   * \tparam T Type of number.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of numbers comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the current array index as a std::size_t, the current number of type T, and return
   *          Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of numbers and execute the callable with every number.
   * \endinternal
   */
  template <typename T, typename Fn>
  auto NumberArray(Fn fn) noexcept -> ArrayCallableReturnsResult<Fn, T, R> {
    static_assert(ArrayReturnsResultVoid<Fn, T>::value, "Must return Result<void>");
    return this->Array([this, &fn](std::size_t n) {
      return this
          // VCA_VAJSON_LAMBDA_CAPTURE
          ->Number<T>(
              // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
              [&fn, n](T number) {
                // VCA_VAJSON_LAMBDA_CAPTURE
                return std::forward<Fn>(fn)(n, number);
              })
          .Drop();
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of strings and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of strings comes next}
   *
   * \details The callable must take the current array index as a std::size_t, the current string as an
   *          ara::core::StringView, and return void. The provided StringView is only valid until any other method or
   *          parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of strings and execute the callable with every string.
   * \endinternal
   */
  template <typename Fn>
  auto StringArray(Fn fn) noexcept -> ArrayCallableReturnsNoResult<Fn, ara::core::StringView, R> {
    static_assert(ArrayReturnsVoid<Fn, ara::core::StringView>::value, "Must return void");
    return this->StringArray([&fn](std::size_t n, ara::core::StringView s) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(n, s);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of strings and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of strings comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the current array index as a std::size_t, the current string as an
   *          ara::core::StringView, and return Result<void>. The provided StringView is only valid until any other
   *          method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of strings and execute the callable with every string.
   * \endinternal
   */
  template <typename Fn>
  auto StringArray(Fn fn) noexcept -> ArrayCallableReturnsResult<Fn, ara::core::StringView, R> {
    static_assert(ArrayReturnsResultVoid<Fn, ara::core::StringView>::value, "Must return Result<void>");
    return this->Array([this, &fn](std::size_t n) {
      return this
          // VCA_VAJSON_LAMBDA_CAPTURE
          ->String(
              // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
              [&fn, n](ara::core::StringView s) {
                // VCA_VAJSON_LAMBDA_CAPTURE
                return std::forward<Fn>(fn)(n, s);
              })
          .Drop();
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of bools and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of bools comes next}
   *
   * \details The callable must take the current array index as a std::size_t, the current bool value, and return void.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of bools and execute the callable with every string.
   * \endinternal
   */
  template <typename Fn>
  auto BoolArray(Fn fn) noexcept -> ArrayCallableReturnsNoResult<Fn, bool, R> {
    static_assert(ArrayReturnsVoid<Fn, bool>::value, "Must return void");
    return this->BoolArray([&fn](std::size_t n, bool b) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(n, b);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array of bools and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array of bools comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the current array index as a std::size_t, the current bool value, and return
   * Result<void>.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array of bools and execute the callable with every string.
   * \endinternal
   */
  template <typename Fn>
  auto BoolArray(Fn fn) noexcept -> ArrayCallableReturnsResult<Fn, bool, R> {
    static_assert(ArrayReturnsResultVoid<Fn, bool>::value, "Must return Result<void>");
    return this->Array([this, &fn](std::size_t n) {
      return this
          // VCA_VAJSON_LAMBDA_CAPTURE
          ->Bool(
              // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
              [&fn, n](bool b) {
                // VCA_VAJSON_LAMBDA_CAPTURE
                return std::forward<Fn>(fn)(n, b);
              })
          .Drop();
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following binary content and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no binary content comes next}
   * \error{The error returned by the callable}
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
   * \vpublic
   *
   * \internal
   * - If the binary content could be parsed successfully:
   *   - Execute the callable with the binary content.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Binary(Fn fn) noexcept -> CallableReturnsResult<Fn, Bytes, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following binary content and executes the given callable.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no binary content comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the binary content as ara::core::Span<char> and return void. The provided Span is
   *          only valid until any other method or parser operating on the same document is called.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - If the binary content could be parsed successfully:
   *   - Execute the callable with the binary content.
   *   - If the callable succeeds, return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Fn>
  auto Binary(Fn fn) noexcept -> CallableReturnsNoResult<Fn, Bytes, R> {
    static_assert(ReturnsVoid<Fn, Bytes>::value, "Must return void");
    return this->Binary([&fn](Bytes view) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(view);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array comes next}
   *
   * \details The callable must take the current array index as a std::size_t and return void.
   *          The callable must consume all tokens representing the element.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array and execute the callable with every array element.
   * \endinternal
   */
  template <typename Fn>
  auto Array(Fn fn) noexcept -> CallableReturnsNoResult<Fn, std::size_t, R> {
    static_assert(ReturnsVoid<Fn, std::size_t>::value, "Must return void");
    return this->Array([&fn](std::size_t n) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      std::forward<Fn>(fn)(n);
      return Result<void>{};
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following array and executes the given callable on every element.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no array comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the current array index as a std::size_t and return Result<void>.
   *          The callable is expected to have consumed all tokens representing the element if it returns a successful
   *          Result. If the callable is unable to consume all tokens it must return an error Result.
   *
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   *
   * \internal
   * - Parse the array and execute the callable with every array element.
   * \endinternal
   */
  template <typename Fn>
  auto Array(Fn fn) noexcept -> CallableReturnsResult<Fn, std::size_t, R>;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following object and executes the given callable on every key.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \param object_already_open Specify if the object has already been opened. Defaults to false.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no object comes next}
   *
   * \details The callable must take the current object key as an ara::core::StringView and return void. The provided
   *          StringView is only valid until any other method or parser operating on the same document is called.
   *          The callable must consume all tokens representing the value related to the key.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   *
   * \internal
   * - Parse the object and execute the callable with every key.
   * \endinternal
   */
  template <typename Fn>
  auto Object(Fn fn, bool object_already_open = false) noexcept
      -> CallableReturnsNoResult<Fn, ara::core::StringView, R> {
    static_assert(ReturnsVoid<Fn, ara::core::StringView>::value, "Must return void");
    return Object(
        [&fn](ara::core::StringView key) {
          // VCA_VAJSON_LAMBDA_CAPTURE
          std::forward<Fn>(fn)(key);
          return Result<void>{};
        },
        object_already_open);
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Parses the following object and executes the given callable on every key.
   * \tparam Fn Type of callable.
   * \param fn Callable.
   * \param object_already_open Specify if the object has already been opened. Defaults to false.
   * \return kRunning on success, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no object comes next}
   * \error{The error returned by the callable}
   *
   * \details The callable must take the current key as an amsr::core::StringView and return Result<void>. The provided
   *          StringView is only valid until any other method or parser operating on the same document is called.
   *          The callable is expected to have consumed all tokens representing the value related to the key if it
   *          returns a successful Result. If the callable is unable to consume all tokens it must return an error
   *          Result.
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
   * - Parse the object and execute the callable with every key.
   * \endinternal
   */
  template <typename Fn>
  auto Object(Fn fn, bool object_already_open = false) noexcept -> CallableReturnsResult<Fn, amsr::core::StringView, R>;

 private:
  /*!
   * \brief Reference to the JSON document to parse.
   */
  JsonData& doc_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_H_
