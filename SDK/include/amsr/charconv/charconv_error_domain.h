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
/*!        \file
 *        \brief  Defines an error domain for characterconversion.
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_CHARCONV_ERROR_DOMAIN_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_CHARCONV_ERROR_DOMAIN_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/array.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "amsr/core/result.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace charconv {

/*!
 * \brief Null-terminated C-string
 * \vprivate Component Private
 */
using CStr = char const*;

/*!
 * \brief Alias for ErrorDomain.
 * \vprivate Component Private
 */
using ErrorDomain = amsr::core::ErrorDomain;

/*!
 * \brief Alias for ErrorCode.
 * \vprivate Component Private
 */
using ErrorCode = amsr::core::ErrorCode;

/*!
 * \brief CharacterConversion error code enum.
 * \vprivate Product Private
 */
enum class CharconvErrc : amsr::core::ErrorDomain::CodeType {
  kUnknownError = 1,     /*!< unknown error */
  kInvalidArgument = 2,  /*!< invalid argument */
  kResultOutOfRange = 3, /*!< result out of range */
  kValueTooLarge = 4,    /*!< value to large for range */
  kNotSupported = 5,     /*!< conversion not supported */
};

/*!
 * \brief Exception type for CharconvErrorDomain.
 * \details
 * \vprivate Component Private
 */
class CharconvException final : public amsr::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   * \vprivate Component Private
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for charconv system errors.
 * \unit amsr::charconv::CharconvErrorDomain
 * \vprivate Product Private
 */
class CharconvErrorDomain final : public amsr::core::ErrorDomain {
  /*! ID of the domain */
  static constexpr ErrorDomain::IdType kId{0xc0000000001e730f};

 public:
  /*!
   * \brief Error code enum of the domain.
   * \vprivate Component Private
   */
  using Errc = CharconvErrc;
  /*!
   * \brief Exception type of the domain.
   * \vprivate Component Private
   */
  using Exception = CharconvException;

  /*!
   * \brief Constructs instance of class.
   * \details Initializes the domain id with CharconvErrorDomain::kId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \unit amsr::charconv::CharconvErrorDomain
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  constexpr CharconvErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \unit amsr::charconv::CharconvErrorDomain
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::ErrorDomain::StringType Name() const noexcept final { return "Charconv"; }

  /*!
   * \brief Returns a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \unit amsr::charconv::CharconvErrorDomain
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::ErrorDomain::StringType Message(ErrorDomain::CodeType error_code) const noexcept final {
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    constexpr static amsr::core::Array<amsr::core::ErrorDomain::StringType, 6> kMessages{{
        "Unknown error code\0",                                                       // not defined
        "Unknown error encountered\0",                                                // kUnknownError
        "Pattern does not match or range is not large enough to hold the literal\0",  // kInvalidArgument
        "Value is out of range of the corresponding type\0",                          // kResultOutOfRange
        "Value is larger than the specified range\0",                                 // kValueTooLarge
        "Conversion is not supported\0",                                              // kNotSupported
    }};

    std::size_t index{0};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {
        index = code;
      }
    }
    return kMessages[index];
  }

  /*!
   * \brief Throws the given ErrorCode as Exception.
   * \param[in] error_code error code to be thrown.
   * \remark If the code is compiled without exceptions, this function call will terminate.
   * \context ANY
   * \pre -
   * \throws CharconvException for all error codes.
   * \threadsafe FALSE
   * \exceptionsafety Exception in all cases
   * \reentrant TRUE
   * \unit amsr::charconv::CharconvErrorDomain
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};  // namespace charconv

namespace internal {
/*!
 * \brief Global CharconvErrorDomain instance.
 * \vprivate Component Private
 */
constexpr CharconvErrorDomain kCharconvErrorDomain;
}  // namespace internal

/*!
 * \brief Returns a reference to the global CharconvErrorDomain instance.
 * \return CharconvErrorDomain instance.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr amsr::core::ErrorDomain const& GetCharconvDomain() noexcept { return internal::kCharconvErrorDomain; }

/*!
 * \brief Creates an error code from CharconvErrorDomain
 * \param[in] code The specific error code
 * \param[in] data Vendor defined support data
 * \param[in] message An optional message for this error
 * \return The constructed error code
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr ErrorCode MakeErrorCode(CharconvErrorDomain::Errc code, ErrorDomain::SupportDataType data,
                                         CStr message) {
  return {static_cast<ErrorDomain::CodeType>(code), GetCharconvDomain(), data, message};
}

/*!
 * \brief Creates an error code from CharconvErrorDomain
 * \param[in] code The specific error code
 * \param[in] message An optional message for this error
 * \return The constructed error code
 * \context ANY
 * \pre Valid error domain.
 * \threadsafe FALSE
 * \reentrant TRUE
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr ErrorCode MakeErrorCode(CharconvErrorDomain::Errc code, CStr message) {
  return {static_cast<ErrorDomain::CodeType>(code), GetCharconvDomain(), 0, message};
}

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_CHARCONV_ERROR_DOMAIN_H_
