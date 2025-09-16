/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   amsr/asn1/asn1_error_domain.h
 *        \brief  ASN.1 Error domain.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_ERROR_DOMAIN_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief Specifies the internal errors that can occur while parsing/building.
 * \vprivate Product Private
 */
enum class Asn1Errc : amsr::core::ErrorDomain::CodeType {
  kIncompleteInput = 101, /*!< Reached end of stream before completion. */
  kUnknownEncoding,       /*!< Encoding type is not recognized. */
  kUnknownTag,            /*!< Tag number is invalid. */
  kUnsupportedTag,        /*!< Unsupported universal tag number found. */
  kInvalidContent,        /*!< Content data is not as per specification. */
  kUnsupportedNumeric,    /*!< Numeric value could not be handled. */
  kRuntimeFault,          /*!< Builder rejected current input. */
  kEncodeNoError,         /*!< No encoding errors. */
  kInvalidUnusedBits,     /*!< Unused bits is greater than 7. */
  kUnsupportedBase,       /*!< Unsupported base(8, 16) Real data */
  kConstraintCheckFail,   /*!< Constraint check failure. */
  kInvalidInput           /*!< Invalid input parameter. */
};

/*!
 * \brief An interface of an ASN.1 exception.
 * \vprivate Product Private
 */
class Asn1Exception : public amsr::core::Exception {
 public:
  /*!
   * \brief Constructs an Asn1Exception.
   * \vprivate Product Private
   */
  using amsr::core::Exception::Exception;
};

/*!
 * \brief Interface of a general ASN.1 Logic Error exception.
 * \details If the incorrectness of the API call must be obvious before the call execution.
 * \vprivate Product Private
 */
class LogicException : public Asn1Exception {
 public:
  /*!
   * \brief Constructs a LogicException.
   * \vprivate Product Private
   */
  using Asn1Exception::Asn1Exception;
};

/*!
 * \brief Interface of a general ASN.1 Runtime Error exception.
 * \details If the incorrectness of an API call can be detected at runtime only.
 * \vprivate Product Private
 */
class RuntimeException : public Asn1Exception {
 public:
  /*!
   * \brief Constructs a RuntimeException.
   * \vprivate Product Private
   */
  using Asn1Exception::Asn1Exception;
};

/*!
 * \brief Interface of an Unexpected Value exception.
 * \details If some non-expected values are passed, but their incorrectness can be detected at runtime only.
 * \vprivate Product Private
 */
class UnexpectedValueException : public RuntimeException {
 public:
  /*!
   * \brief Constructs a RuntimeException.
   * \vprivate Product Private
   */
  using RuntimeException::RuntimeException;
};

/*!
 * \brief Error domain for errors originating from ara::asn1::Asn1Parser/from ara::asn1::Asn1Builder.
 * \vprivate Product Private
 */
class Asn1ErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief The Asn.1 error domain id
   */
  constexpr static ErrorDomain::IdType kId{0x000000000000011};
  // FIXME: Update correct kId

 public:
  /*!
   * \brief Default constructor.
   * \vprivate Product Private
   */
  constexpr Asn1ErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief  Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
   * \return "Asn1Error".
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Name() const noexcept -> StringType final { return "Asn1Error"; }

  /*!
   * \brief  Translates an error code value into a text message.
   * \param  error_code The error code value to translate.
   * \return The text message, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Message(ErrorDomain::CodeType error_code) const noexcept -> StringType final {
    StringType msg{"Unknown Error Code"};
    if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kIncompleteInput) == error_code) {
      msg = "Reached end of stream before completion";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnknownEncoding) == error_code) {
      msg = "Encoding type is not recognized";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnknownTag) == error_code) {
      msg = "Tag number is unknown";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnsupportedTag) == error_code) {
      msg = "Unsupported universal tag number found";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kInvalidContent) == error_code) {
      msg = "Content data does not comply with encoding rule";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnsupportedNumeric) == error_code) {
      msg = "Numeric value could not be handled.";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kConstraintCheckFail) == error_code) {
      msg = "Constraint check failure.";
    } else if (static_cast<ErrorDomain::CodeType>(Asn1Errc::kRuntimeFault) == error_code) {
      msg = "Builder rejected current input.";
    } else {
      // Unknown error code
    }
    return msg;
  }

  /*!
   * \brief Throws the given error_code as Exception.
   * \details If the code is compiled without exceptions, this function call will terminate.
   * \param[in] error_code error code to be thrown.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    // TODO(lwech) Check if this check is even complete.
    switch (error_code.Value()) {
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kIncompleteInput):
        vac::language::ThrowOrTerminate<RuntimeException>(error_code);
        break;
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnknownEncoding):
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnknownTag):
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnsupportedTag):
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kInvalidContent):
      case static_cast<ErrorDomain::CodeType>(Asn1Errc::kUnsupportedNumeric):
        vac::language::ThrowOrTerminate<UnexpectedValueException>(error_code);
        break;
      default:
        vac::language::ThrowOrTerminate<Asn1Exception>(error_code);
        break;
    }
  }
};

namespace internal {
/*!
 * \brief The single global Asn1ErrorDomain instance.
 * \vprivate Product Private
 */
constexpr Asn1ErrorDomain g_Asn1ErrorDomain;
}  // namespace internal

/*!
 * \brief  Obtain the reference to the single global Asn1ErrorDomain instance.
 * \return Reference to the Asn1ErrorDomain instance.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
inline constexpr auto GetAsn1ErrorDomain() noexcept -> amsr::core::ErrorDomain const& {
  return internal::g_Asn1ErrorDomain;
}

/*!
 * \brief  Create a new ErrorCode for Asn1ErrorDomain with the given support data type and message.
 * \param  code Enumeration value from Asn1Errc.
 * \param  data Vendor-defined supplementary value.
 * \param  message User-defined context message (can be nullptr).
 * \return The new ErrorCode instance.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
inline constexpr auto MakeErrorCode(Asn1Errc code, amsr::core::ErrorDomain::SupportDataType data,
                                    char const* message) noexcept -> amsr::core::ErrorCode {
  return amsr::core::ErrorCode{static_cast<amsr::core::ErrorDomain::CodeType>(code), GetAsn1ErrorDomain(), data,
                               message};
}

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_ERROR_DOMAIN_H_
