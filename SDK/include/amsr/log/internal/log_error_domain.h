/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       log_error_domain.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_LOG_ERROR_DOMAIN_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_LOG_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/core/error_code.h"    // IWYU pragma: export
#include "amsr/core/error_domain.h"  // IWYU pragma: export
#include "amsr/core/exception.h"     // IWYU pragma: export
#include "amsr/core/result.h"        // IWYU pragma: export
#include "amsr/core/string_view.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Null-terminated C-string
 */
using CStr = char const*;

/*!
 * \brief Alias for ErrorCode.
 */
using ErrorCode = core::ErrorCode;

/*!
 * \brief An alias of core::Result for vajson
 *
 * \tparam T The type of value
 * \tparam E The type of error
 */
template <typename T, typename E = ErrorCode>
using Result = core::Result<T, E>;

/*!
 * \brief Alias for ErrorDomain.
 */
using ErrorDomain = core::ErrorDomain;

/*!
 * \brief Contains all error codes thrown by LogAndTrace
 *
 * \unit 	LogAndTrace::Common::ErrorHandling::LogErrc
 */
enum class LogErrc : ErrorDomain::CodeType {
  kNotInitialized = 1,
  kInitializationFailed,
  kBufferTooSmall,
  kRuntimeError,
  kInitializationFailedUnableToOpenSinkFile,
  kInitializationFailedAmsrProcessShortNamePathNotSet,
  kInitializationFailedConfigValidationFileError,
  kInitializationFailedConfigValidationFileNotFound,
  kInitializationFailedConfigValidationFileInvalidArgument,
  kInitializationFailedConfigFileError,
  kInitializationFailedConfigFileNotFound,
  kInitializationFailedConfigParsingError,
};

/*!
 * \brief Exception type for all LogAndTrace related errors
 *
 * \unit LogAndTrace::Common::ErrorHandling::LogException
 */
class LogException final : public core::Exception {
 public:
  /*! \brief Inherit the constructor */
  using Exception::Exception;
};

/*!
 * \brief The error domain for all LogAndTrace related errors
 *
 * \unit 	LogAndTrace::Common::ErrorHandling::ErrorDomain
 */
class LogErrorDomain final : public ErrorDomain {
  /*!
   * \brief The unique domain id
   */
  constexpr static IdType kId{0x123123u};

 public:
  /*!
   * \brief Implements the Errc interface-type
   */
  using Errc = LogErrc;
  /*!
   * \brief Implements the Exception interface-type
   */
  using Exception = LogException;

  /*!
   * \brief Default constructor
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr LogErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the name of the Error domain
   *
   * \returns The name of the Error domain
   * \spec
   *   requires true;
   * \endspec
   */
  auto Name() const noexcept -> CStr final;

  /*!
   * \brief Converts an error code into a message
   *
   * \param error_code The error code to transform
   * \return The transformed message
   * \spec
   *   requires true;
   * \endspec
   */
  auto Message(CodeType error_code) const noexcept -> CStr final;

  /*!
   * \brief Converts the error into an exception
   * \param error_code The error code to throw
   * \throws Exception with the passed error code
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(ErrorCode const& error_code) const noexcept(false) final;
};

namespace detail {
/*!
 * \brief The Log error domain singleton
 */
constexpr LogErrorDomain kLogErrorDomain;
}  // namespace detail

/*!
 * \brief Gets the error domain singleton
 *
 * \returns The error domain singleton
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto GetLogDomain() noexcept -> ErrorDomain const& { return detail::kLogErrorDomain; }

/*!
 * \brief Creates an error code from LogErrorDomain
 *
 * \param code    The specific error code
 * \param data    Vendor defined support data
 * \param message An optional message for this error
 * \returns       The constructed error code
 * \spec
 *   requires true;
 * \endspec
 *
 * \unit LogAndTrace::Common::ErrorHandling::LogErrc
 */
constexpr auto MakeErrorCode(LogErrorDomain::Errc code, ErrorDomain::SupportDataType data, CStr message) noexcept
    -> ErrorCode {
  return {static_cast<ErrorDomain::CodeType>(code), GetLogDomain(), data, message};
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1_false_positive_rvalue_overload
/*!
 * \brief Returns a positive empty result
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto Ok() noexcept -> Result<void> { return Result<void>{}; }

/*!
 * \brief Returns a positive nonempty result
 * \tparam T  Must meet the type requirements of amsr::core::Result<T>.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
constexpr auto Ok(T&& value) noexcept -> Result<T> {
  return Result<T>::FromValue(std::forward<T>(value));
}

/*!
 * \brief Creates an error code from an error code enum value.
 *
 * \tparam Code        The Errc enum to use.
 *                     Must be convertible to amsr::log::internal::LogErrorDomain::Errc.
 * \tparam SupportData The data to be used as support data.
 *                     Must be convertible to amsr::log::internal::ErrorDomain::SupportDataType.
 * \param code         The specific error code.
 * \param message      An optional message for this error.
 * \param support_data Optionally defined vendor defined support data.
 * \returns            The constructed error code.
 * \spec
 *   requires true;
 * \endspec
 *
 * \unit LogAndTrace::Common::ErrorHandling::LogErrc
 */
template <typename Code, typename SupportData = ErrorDomain::SupportDataType>
// VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
constexpr auto Err(Code code, CStr message = "", SupportData support_data = {}) noexcept -> ErrorCode {
  // VECTOR NC AutosarC++17_10-M5.0.4: MD_LOG_AutosarC++17_10-M5.0.4_irrelevant
  // VECTOR NC AutosarC++17_10-M5.0.6: MD_LOG_AutosarC++17_10-M5.0.6_irrelevant
  // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
  ErrorDomain::SupportDataType const data{static_cast<ErrorDomain::SupportDataType>(support_data)};
  return MakeErrorCode(code, data, message);
}

/*!
 * \brief Creates an error code from errc & error code values
 *
 * \tparam Code The Errc enum to use.
 *              Must be convertible to amsr::log::internal::LogErrorDomain::Errc.
 * \param code  The specific error code
 * \param ec    The underlying error code.
 * \returns     The constructed error code
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Code>
constexpr auto ChainErr(Code code, core::ErrorCode ec) noexcept -> ErrorCode {
  return MakeErrorCode(code, ec.SupportData(), ec.Message().data());
}

/*!
 * \brief Creates a function object for converting error codes to the stated errc.
 *
 * \param errc The error code to use.
 * \returns    The function object for converting errors.
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR NC AutosarC++17_10-A7.1.5: MD_LOG_AutosarC++17_10-A7.1.5_complicated_and_irrelevant_type
inline auto ConvertErrorCode(LogErrc errc) noexcept {
  return [errc](core::ErrorCode ec) { return ChainErr(errc, ec); };
}

/*!
 * \brief Creates a Result from an error code enum value
 *
 * \tparam T      The type of result to return.
 *                Must meet the type requirements of amsr::core::Result<T>.
 * \tparam Code   The Errc enum to use.
 *                Must be convertible to amsr::log::internal::LogErrorDomain::Errc.
 * \param code    The specific error code
 * \param message An optional message for this error
 * \param data    Optionally defined vendor defined support data
 * \returns       The constructed error code
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate
 */
template <typename T, typename Code>
constexpr auto Error(Code code, CStr message = "", ErrorDomain::SupportDataType data = 0) noexcept -> Result<T> {
  return Result<T>{Err(code, message, data)};
}

// VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
/*!
 * \brief Creates a Result from a boolean value
 *
 * \tparam Code   The Errc enum to use.
 *                Must meet the template requirements of amsr::log::internal::Error<void, Code>.
 * \param value   The value to convert
 * \param code    The error code to use
 * \param message The optional message to use
 * \param data    The optional support data
 * \return        Either a successful result or the specified error
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate
 */
template <typename Code>
auto MakeResult(bool value, Code code, CStr message = "", ErrorDomain::SupportDataType data = 0) noexcept
    -> Result<void> {
  return value ? Ok() : Error<void>(code, message, data);
}
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_LOG_ERROR_DOMAIN_H_
