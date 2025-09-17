/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!
 *          \file crypto_adapter_error_definitions.h
 *          \brief This file contains error declarations for the Crypto Adapter submodule.
 *          \details This file defines several error strings along with their error codes which can be used for
 *                   reporting errors and failures produced by the Crypto Adapter submodule, it also includes the crypto
 *                   adapter error domain for mapping error values to their textual representation. Besides, some helper
 *                   functions for creating errors within the Crypto Adapter error domain and throwing them as
 *                   exceptions. All the implemented classes inherits from the AUTOSAR common base classes of error
 *                   declarations. For more information please refer to: "vac::language" namespace.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_ERROR_DEFINITIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_ERROR_DEFINITIONS_H_

#include <utility>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*! \brief Crypto Adapter type definition for error code */
using CryptoAdapterErrorCode = amsr::core::ErrorCode;

/*! \brief Exception type for CryptoAdapterErrorDomain. */
class CryptoAdapterDomainException final : public ara::core::Exception {
 public:
  using ara::core::Exception::Exception;
};

/*!
 * \brief The Crypto Adapter interface error domain class
 * \details An error domain is the controlling entity for ErrorCode's error code values, and defines the
 * mapping of such error code values to textual representations.
 * see amsr::core::ErrorDomain
 */
class CryptoAdapterErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*! \brief Not copyable */
  CryptoAdapterErrorDomain(CryptoAdapterErrorDomain const &) = delete;

  /*! \brief Not movable */
  CryptoAdapterErrorDomain(CryptoAdapterErrorDomain &&) = delete;

  /*! \brief Not copy assignable */
  CryptoAdapterErrorDomain &operator=(CryptoAdapterErrorDomain const &) = delete;

  /*! \brief Not move assignable */
  CryptoAdapterErrorDomain &operator=(CryptoAdapterErrorDomain &&) = delete;

  /*! \brief Default destructor */
  ~CryptoAdapterErrorDomain() = default;

  /*! \brief ID of the domain. */
  static constexpr amsr::core::ErrorDomain::IdType kId{0x4e32d2cf5295b20a};

  /*! \brief Enumeration defining the error codes supported by the Crypto Adapter interface */
  enum class EnCryptoAdapterErrors : amsr::core::ErrorDomain::CodeType {
    /*! \brief No error occurred, operation was successful */
    kCryptoAdapterErrOk = 0,
    /*! \brief A requested algorithm was not supported */
    kUnsupportedAlgorithm,
    /*! \brief An exception thrown from an external function was caught */
    kCryptoProviderException,
    /*! \brief An invalid argument was received */
    kInvalidArgument,
    /*! \brief An internal state is invalid */
    kInvalidState,
    /*! \brief The PSK identity could not be found */
    kPskIdentityNotFound,
    /*! \brief The size of the initialization vector is invalid */
    kInvalidIvSize,
    /*! \brief A function context was wrongly used */
    kUsageViolation,
    /*! \brief The block size for a cipher algorithm was zero */
    kBlockSizeZero,
    /*! \brief An error occurred during a runtime process */
    kRuntimeError
  };

  /*! \brief Error code enum of the domain. */
  using Errc = EnCryptoAdapterErrors;
  /*! \brief Exception type of the domain. */
  using Exception = CryptoAdapterDomainException;

  /*!
   * \brief Constructs a new CryptoAdapterErrorDomain object
   * \details Initializes the domain id with PosixErrorDomain::kId
   * \vprivate This method is private to AMSR product
   */
  constexpr CryptoAdapterErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the CryptoAdapterErrorDomain name
   * \return A pointer to CryptoAdapterErrorDomain name string
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is private to AMSR product
   */
  StringType Name() const noexcept final { return "CryptoAdapterErrorDomain"; }

  /*!
   * \brief Gets a message string which describes the provided error code
   * \param[in] error_code The CryptoAdapterErrorCode
   * \return A pointer to the message string
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is private to AMSR product
   */
  // VECTOR NC Metric-HIS.VG: MD_libseccom_Metric-HIS.VG_CyclomaticComplexityAboveLimit
  StringType Message(CodeType error_code) const noexcept final {
    StringType return_string{nullptr};
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_libseccom_A7.2.1_AssignmentOfValueOutsideEnumeratorRange
    switch (static_cast<Errc>(error_code)) {
      case Errc::kCryptoAdapterErrOk: {
        return_string = "CryptoAdapterError: OK";
        break;
      }
      case Errc::kUnsupportedAlgorithm: {
        return_string = "CryptoAdapterError: Algorithm not supported";
        break;
      }
      case Errc::kCryptoProviderException: {
        return_string = "CryptoAdapterError: Crypto provider exception";
        break;
      }
      case Errc::kInvalidArgument: {
        return_string = "CryptoAdapterError: Invalid argument";
        break;
      }
      case Errc::kInvalidState: {
        return_string = "CryptoAdapterError: Invalid state";
        break;
      }
      case Errc::kPskIdentityNotFound: {
        return_string = "CryptoAdapterError: Psk identity not found";
        break;
      }
      case Errc::kInvalidIvSize: {
        return_string = "CryptoAdapterError: Invalid iv size";
        break;
      }
      case Errc::kBlockSizeZero: {
        return_string = "CryptoAdapterError: Block size for cipher algorithm is zero";
        break;
      }
      case Errc::kUsageViolation: {
        return_string = "CryptoAdapterError: Crypto usage violation";
        break;
      }
      case Errc::kRuntimeError: {
        return_string = "CryptoAdapterError: Crypto runtime error";
        break;
      }
      default: {
        return_string = "Unknown error code";
        break;
      }
    }
    return return_string;
  }

  /*!
   * \brief Throws the specified exception or terminates when exceptions are deactivated.
   * \details see vac::vac::language::ThrowOrTerminate
   * \param[in] error_code the error code to be thrown
   * \throws CryptoAdapterDomainException with the passed error code.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is private to AMSR product.
   */
  void ThrowAsException(CryptoAdapterErrorCode const &error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<CryptoAdapterDomainException>(error_code);
  }
};

/*! \brief Short alias for CryptoAdapterErrorDomain::EnCryptoAdapterErrors */
using EnCryptoAdapterErrors = CryptoAdapterErrorDomain::EnCryptoAdapterErrors;

/*! \brief Global CryptoAdapterErrorDomain instance. */
constexpr CryptoAdapterErrorDomain kCryptoAdapterErrorDomain;

/*!
 * \brief Returns a reference to the global CryptoAdapterErrorDomain instance.
 * \return CryptoAdapterErrorDomain instance.
 * \context ANY
 * \pre -
 * \reentrant TRUE
 * \vprivate This method is private to AMSR product
 */
inline constexpr amsr::core::ErrorDomain const &GetCryptoAdapterCommErrorDomain() { return kCryptoAdapterErrorDomain; }

/*!
 * \brief Function that creates an CryptoAdapterErrorCode instance.
 * \param[in,out] code The CryptoAdapterErrorCode
 * \param[in,out] data Support data to be set
 * \param[in,out] message The message string
 * \return The created CryptoAdapterErrorCode instance
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is private to AMSR product
 */
inline constexpr CryptoAdapterErrorCode MakeErrorCode(CryptoAdapterErrorDomain::Errc code,
                                                      amsr::core::ErrorDomain::SupportDataType data,
                                                      char const *message) {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetCryptoAdapterCommErrorDomain(), data, message};
}

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_ERROR_DEFINITIONS_H_
