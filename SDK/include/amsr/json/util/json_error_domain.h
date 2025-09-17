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
/**        \file     json_error_domain.h
 *         \brief    Contains error handling related types.
 *
 *         \details  Contains Result alias, error domain & error codes.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_JSON_ERROR_DOMAIN_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_JSON_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/result.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
/*!
 * \brief Null-terminated C-string.
 *
 * \vprivate
 */
using CStr = char const*;

/*!
 * \brief Unqualified access to Result.
 * \tparam T Type of value.
 *
 * \vprivate
 */
template <typename T>
using Result = amsr::core::Result<T>;

/*!
 * \brief Unqualified access to ErrorDomain.
 *
 * \vprivate
 */
using ErrorDomain = ara::core::ErrorDomain;

/*!
 * \brief Unqualified access to ErrorCode.
 *
 * \vprivate
 */
using ErrorCode = amsr::core::ErrorCode;

/*!
 * \brief All error codes used by vaJson.
 *
 * \vprivate
 */
enum class JsonErrc : ErrorDomain::CodeType {
  kNotInitialized = 1,
  kInvalidJson,
  kUserValidationFailed,
  kStreamFailure
};

/*!
 * \brief An exception type for all JSON related errors.
 *
 * \vpublic
 */
class JsonException final : public ara::core::Exception {
 public:
  /*!
   * \brief Inherited constructor.
   * \vprivate Product Private
   */
  using Exception::Exception;
};

/*!
 * \brief An error domain for all JSON related errors.
 *
 * \vprivate Product Private
 */
class JsonErrorDomain final : public ErrorDomain {
  /*!
   * \brief Unique domain ID.
   * \vprivate Product Private
   */
  constexpr static IdType kId{0x424242};

 public:
  /*!
   * \brief Implements the Errc interface-type.
   * \vprivate Product Private
   */
  using Errc = JsonErrc;

  /*!
   * \brief Implements the Exception interface-type.
   * \vprivate Product Private
   */
  using Exception = JsonException;

  /*!
   * \brief Constructs a JsonErrorDomain type.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr JsonErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the name of the ErrorDomain.
   * \return The name.
   * \vprivate Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto Name() const noexcept -> CStr final { return "Json"; }

  /*!
   * \brief Converts an error code into a message.
   * \param error_code to transform.
   * \return The transformed message.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If the error code is within bounds of the messages array:
   *   - Return the message from the array.
   * - Otherwise:
   *   - Return "unknown error".
   * \endinternal
   */
  auto Message(CodeType error_code) const noexcept -> CStr final {
    constexpr static std::array<CStr, 5> kMessages{{
        "Unknown error.",
        "kNotInitialized: Result was not initialized.",
        "kInvalidJson: Invalid JSON was encountered.",
        "kUserValidationFailed: The user aborted due to a validation failure.",
        "kStreamFailure: The underlying file or character stream encountered an error.",
    }};

    if ((error_code < 1) || (error_code > 4)) {
      error_code = 0;
    }

    // VECTOR NC AutosarC++17_10-A4.7.1: MD_JSON_AutosarC++17_10-A4.7.1_truncating_cast
    // VECTOR NC AutosarC++17_10-M0.3.1, AutosarC++17_10-M5.0.16: MD_JSON_AutosarC++17_10-M0.3.1_OOB
    return kMessages[static_cast<std::size_t>(error_code)];
  }

  /*!
   * \brief Converts an error code into an exception.
   * \param error_code to throw.
   *
   * \throws JsonException with the passed error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace detail {
/*!
 * \brief JsonErrorDomain singleton.
 * \vprivate Product Private
 */
constexpr JsonErrorDomain kJsonErrorDomain;
}  // namespace detail

/*!
 * \brief Gets the error domain singleton.
 * \return The error domain singleton.
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr auto GetJsonDomain() noexcept -> ErrorDomain const& { return detail::kJsonErrorDomain; }

/*!
 * \brief Creates an error code from JsonErrorDomain.
 * \param code Specific error code.
 * \param data Vendor defined support data.
 * \param message Optional message for this error.
 * \return The constructed error code.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Return an ErrorCode created from the given arguments.
 * \endinternal
 */
inline constexpr auto MakeErrorCode(JsonErrorDomain::Errc code, ErrorDomain::SupportDataType data,
                                    CStr message) noexcept -> ErrorCode {
  return {static_cast<ErrorDomain::CodeType>(code), GetJsonDomain(), data, message};
}

/*!
 * \brief Creates an error code from JsonErrorDomain.
 * \param code Specific error code.
 * \param message Optional message for this error.
 * \return The constructed error code.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Return an ErrorCode created from the given arguments where SupportData is not set.
 * \endinternal
 */
inline constexpr auto MakeErrorCode(JsonErrorDomain::Errc code, CStr message) noexcept -> ErrorCode {
  return {static_cast<ErrorDomain::CodeType>(code), GetJsonDomain(), 0, message};
}

/*!
 * \brief Returns an empty Result.
 * \return The empty Result.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto Ok() noexcept -> Result<void> { return Result<void>{}; }

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates a Result.
 * \tparam T Type of Result.
 * \param value to put into the Result.
 * \return The Result.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Return a Result created from the given value.
 * \endinternal
 */
template <typename T>
constexpr auto Ok(T value) noexcept -> Result<T> {
  return Result<T>::FromValue(std::forward<T>(value));
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates an error code from an error code enum value
 * \param code The specific error code
 * \param message An optional message for this error
 * \param data Optionally defined vendor defined support data
 * \returns The constructed error code
 * \vprivate component private
 */
template <typename Code>
constexpr auto Err(Code code, CStr message = "", ErrorDomain::SupportDataType data = 0) noexcept -> ErrorCode {
  return MakeErrorCode(code, data, message);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates a negative Result from an error code enum value.
 * \tparam T Type of Result.
 * \param code Specific error code.
 * \param message Optional message for this error.
 * \param data Optional vendor defined support data.
 * \return The negative Result.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 *
 * \internal
 * - Create an ErrorCode from the given arguments.
 * - Return a Result containing the ErrorCode.
 * \endinternal
 */
template <typename T, typename Code>
constexpr auto GetErrorResult(Code code, CStr message = "", ErrorDomain::SupportDataType data = 0) noexcept
    -> Result<T> {
  return Result<T>{Err(code, message, data)};
}

// VECTOR NC VectorC++-V3.9.2: MD_JSON_VectorC++-V3.9.2_arithmetic_types
/*!
 * \brief Creates a Result from a boolean value.
 * \param value to check.
 * \param code Specific error code.
 * \param message Optional message for this error.
 * \param data Optional vendor defined support data.
 * \return The empty Result if true, or the specified error.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 *
 * \internal
 * - If the value is true:
 *   - Return an empty Result.
 * - Otherwise:
 *   - Return an Error created from the given arguments.
 * \endinternal
 */
inline auto MakeResult(bool value, JsonErrorDomain::Errc code, CStr message = "",
                       ErrorDomain::SupportDataType data = 0) noexcept -> Result<void> {
  return value ? Ok() : GetErrorResult<void>(code, message, data);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates a Result from a boolean value.
 * \tparam Fn Type of function creating an error code. Must return Result<void>.
 * \param value to check.
 * \param fn Function used to create an error code.
 * \return The empty Result if true, or the return value of the function.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If the value is true:
 *   - Return an empty Result.
 * - Otherwise:
 *   - Return the Error returned by the function.
 * \endinternal
 */
template <typename Fn>
inline auto MakeResult(bool value, Fn fn) noexcept -> Result<void> {
  return value ? Ok() : Result<void>(std::forward<Fn>(fn)());
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates a Result from an Optional.
 * \tparam T Type of Optional.
 * \param value to convert.
 * \param code Specific error code.
 * \param message Optional message for this error.
 * \param data Optional vendor defined support data.
 * \return The Result containing the value of the Optional, or the specified error if the Optional is empty.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If the Optional contains a value:
 *   - Return a Result containing the value.
 * - Otherwise:
 *   - Return an Error created from the given arguments.
 * \endinternal
 */
template <typename T>
inline auto MakeResult(Optional<T> value, JsonErrorDomain::Errc code, CStr message = "",
                       ErrorDomain::SupportDataType data = 0) noexcept -> Result<T> {
  // VCA_VAJSON_EXTERNAL_CALL
  return value ? Ok<T>(std::move(*value)) : GetErrorResult<T>(code, message, data);
}

// VECTOR NC AutosarC++17_10-M0.1.8: MD_JSON_abort_effect
// VECTOR NC VectorC++-V3.9.2: MD_JSON_VectorC++-V3.9.2_arithmetic_types
/*!
 * \brief Assert that a condition holds.
 * \param value to check.
 * \param message Optional message for this error.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If the value is false:
 *   - Abort printing the passed message.
 * \endinternal
 */
inline void AssertCondition(bool value, CStr message = "") noexcept {
  if (not value) {
    amsr::core::Abort(message);
  }
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Creates an Optional from a Result.
 * \tparam T Type of value.
 * \tparam E Type of error.
 * \tparam Opt Type of Optional.
 * \param result to convert.
 * \return The Optional containing the value of the Result, or an empty Optional if the Result contains an error.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vprivate component private
 *
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the Result contains a value:
 *   - Return an Optional containing that value.
 * - Otherwise:
 *   - Return an empty Optional.
 * \endinternal
 */
template <typename T, typename E, typename Opt = Optional<T>>
auto ToOptional(amsr::core::Result<T, E> const& result) noexcept -> Opt {
  return result.Map([](T const& value) { return Opt{value}; }).ValueOr(Opt{});
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_JSON_ERROR_DOMAIN_H_
