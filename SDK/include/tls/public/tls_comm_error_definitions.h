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
/*!
 * \file  tls_comm_error_definitions.h
 *
 * \brief This file contains error declarations for the TLS library.
 *
 * \details This file defines several error strings along with their error codes which can be used for reporting errors
 *          and failures produced by the TLS library. It also includes the TLS error domain for mapping
 *          error values to their textual representation. Besides, some helper functions are also defined for
 *          creating errors within the TLS error domain and throwing them as exceptions or causing
 *          aborts (if exception-less).
 *          All the implemented classes inherit from the AUTOSAR common base classes of error declarations.
 *          For more information, please refer to "vac::language" namespace.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_ERROR_DEFINITIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_ERROR_DEFINITIONS_H_

#include <utility>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace tls {

/*! \brief TLS type definition for error code. */
using TlsErrorCode = amsr::core::ErrorCode;

/*! \brief Exception type for TlsErrorDomain.
 *  \details The TLS library does not support exceptions and this type exists solely for compatibility
 *           reasons with "TlsErrorDomain".
 */
class TlsDomainException final : public ara::core::Exception {
 public:
  /*!
   * \brief Constructs a new TlsDomainException object with a specific TlsErrorCode.
   * \details See ara::core::Exception::Exception(ErrorCode)
   * \param[in] err TLS type error code
   * \context TlsErrorDomain
   * \pre -
   * \reentrant FALSE
   */
  using ara::core::Exception::Exception;
};

/*!
 * \brief The TLS library error domain class
 * \details An error domain is the controlling entity for ErrorCode's error code values, and defines the mapping of such
 * error code values to textual representations. See amsr::core::ErrorDomain for more information.
 */
class TlsErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*!
   * \brief Enumeration defining the error codes supported by the TLS Library
   * \details Includes error codes for Crypto Adapter
   */
  enum class TlsCommErrorCode : amsr::core::ErrorDomain::CodeType {
    /*! \brief No error occurred, operation was successful */
    kTlsErrOk = 0,

    /*! \brief An internal error occurred, operation was not successful */
    kTlsErrInternal,

    /*! \brief An invalid argument was received */
    kTlsErrInvalidArgument,

    /*! \brief An internal state is invalid */
    kTlsErrInvalidState,

    /*! \brief An internal error occurred during deserialization */
    kTlsErrDeserialize,

    /*! \brief An internal error occurred during serialization */
    kTlsErrSerialize,

    /*! \brief An internal error occurred and an Alert message with description value 80 is sent */
    kTlsAlertInternalError,

    /*! \brief An unexpected message was received and an Alert message with description value 10 is sent  */
    kTlsAlertUnexpectedMessage,

    /*! \brief An error occurred during the handshake and an Alert message with description value 40 is sent  */
    kTlsAlertHandshakeFailure,

    /*! \brief An error occurred during decryption and an Alert message with description value 51 is sent  */
    kTlsAlertDecryptError,

    /*!
     * \brief Secure communication can no longer be guaranteed and an Alert message with description value 71 is sent
     */
    kTlsAlertInsufficientSecurity,

    /*! \brief Renegotiation was attempted which is not supported by the implementation */
    kTlsRenegotiationRejected,

    /*! \brief An error occurred inside the crypto adapter */
    kTlsCryptoAdapterFailure,

    /*! \brief The PSK identity was not found. */
    kTlsPskIdentityNotFound,

    /*! \brief During handshaking a message with an unexpected protocol version was received */
    kTlsErrUnexpectedVersion,

    /*! \brief An internal memory storage management error occurred */
    kTlsErrMemoryStorage,

    /*! \brief An error code to communicate with user that the connection is not established yet */
    kTlsErrCommProviderIsNotConnected,

    /*! \brief Invalid buffer length. */
    kTlsInvalidBufferLength,

    /*! \brief Too small value. */
    kTlsTooSmallValue,

    /*! \brief Too large value. */
    kTlsTooLargeValue,

    /*! \brief Duplicate value. */
    kTlsDuplicateValue,

    /*! \brief A certificate was corrupt, contained signatures that did not verify correctly, etc. */
    kTlsAlertBadCertificate
  };

  /*! \brief Not copy constructible. */
  TlsErrorDomain(TlsErrorDomain const&) = delete;

  /*! \brief Not move constructible. */
  TlsErrorDomain(TlsErrorDomain&&) = delete;

  /*! \brief Not copy assignable. */
  TlsErrorDomain& operator=(TlsErrorDomain const&) = delete;

  /*! \brief Not move assignable. */
  TlsErrorDomain& operator=(TlsErrorDomain&&) = delete;

  /*! \brief Default destructor. */
  ~TlsErrorDomain() = default;

  /*! \brief ID of the domain. */
  static constexpr amsr::core::ErrorDomain::IdType kId{0x7c73cb5f908a3ea7};

  /*! \brief Error code type of the domain. */
  using Errc = TlsCommErrorCode;

  /*! \brief Exception type of the domain. */
  using Exception = TlsDomainException;

  /*!
   * \brief Constructs a new TlsErrorDomain object.
   * \details Initializes the domain ID with "PosixErrorDomain::kId".
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is private to AMSR product.
   */
  constexpr TlsErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the TlsErrorDomain name.
   * \return A pointer to TlsErrorDomain name string.
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is private to AMSR product.
   */
  StringType Name() const noexcept final { return "TlsErrorDomain"; }

  /*!
   * \brief Gets a message string which describes the provided error code.
   * \param[in] error_code The "TlsCommErrorCode".
   * \return A pointer to the message string.
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is private to AMSR product.
   */
  // VECTOR NC Metric-HIS.VG: MD_libseccom_Metric-HIS.VG_CyclomaticComplexityAboveLimit
  StringType Message(CodeType error_code) const noexcept final {
    char const* ptr_temp_str{};
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_libseccom_A7.2.1_AssignmentOfValueOutsideEnumeratorRange
    switch (static_cast<TlsCommErrorCode>(error_code)) {
      case TlsCommErrorCode::kTlsErrOk: {
        ptr_temp_str = "TlsError: OK";
        break;
      }

      case TlsCommErrorCode::kTlsErrInternal: {
        ptr_temp_str = "TlsError: Internal error";
        break;
      }

      case TlsCommErrorCode::kTlsErrInvalidArgument: {
        ptr_temp_str = "TlsError: Invalid argument";
        break;
      }

      case TlsCommErrorCode::kTlsErrInvalidState: {
        ptr_temp_str = "TlsError: Not connected";
        break;
      }

      case TlsCommErrorCode::kTlsErrDeserialize: {
        ptr_temp_str = "TlsError: Deserialization";
        break;
      }

      case TlsCommErrorCode::kTlsErrSerialize: {
        ptr_temp_str = "TlsError: Serialization";
        break;
      }

      case TlsCommErrorCode::kTlsAlertInternalError: {
        ptr_temp_str = "TlsAlert: Internal error";
        break;
      }

      case TlsCommErrorCode::kTlsAlertUnexpectedMessage: {
        ptr_temp_str = "TlsAlert: Unexpected message";
        break;
      }

      case TlsCommErrorCode::kTlsAlertHandshakeFailure: {
        ptr_temp_str = "TlsAlert: Handshake failure";
        break;
      }

      case TlsCommErrorCode::kTlsAlertDecryptError: {
        ptr_temp_str = "TlsAlert: Decrypt error";
        break;
      }

      case TlsCommErrorCode::kTlsAlertInsufficientSecurity: {
        ptr_temp_str = "TlsAlert: Insufficient security";
        break;
      }

      case TlsCommErrorCode::kTlsRenegotiationRejected: {
        ptr_temp_str = "TlsError: Renegotiation rejected";
        break;
      }

      case TlsCommErrorCode::kTlsCryptoAdapterFailure: {
        ptr_temp_str = "TlsError: Crypto Adapter failure";
        break;
      }

      case TlsCommErrorCode::kTlsPskIdentityNotFound: {
        ptr_temp_str = "TlsError: Psk identity not found";
        break;
      }

      case TlsCommErrorCode::kTlsErrUnexpectedVersion: {
        ptr_temp_str = "TlsError: Unexpected version";
        break;
      }

      case TlsCommErrorCode::kTlsErrMemoryStorage: {
        ptr_temp_str = "TlsError: Memory storage";
        break;
      }

      case TlsCommErrorCode::kTlsErrCommProviderIsNotConnected: {
        ptr_temp_str = "TlsError: The connection has not been established yet";
        break;
      }

      case TlsCommErrorCode::kTlsInvalidBufferLength: {
        ptr_temp_str = "TlsError: Invalid buffer length";
        break;
      }

      case TlsCommErrorCode::kTlsTooSmallValue: {
        ptr_temp_str = "TlsError: Too small value";
        break;
      }

      case TlsCommErrorCode::kTlsTooLargeValue: {
        ptr_temp_str = "TlsError: Too large value";
        break;
      }

      case TlsCommErrorCode::kTlsDuplicateValue: {
        ptr_temp_str = "TlsError: Duplicate value";
        break;
      }

      case TlsCommErrorCode::kTlsAlertBadCertificate: {
        ptr_temp_str = "TlsError: Bad certificate";
        break;
      }

      default: {
        ptr_temp_str = "Unknown error code";
        break;
      }
    }

    return ptr_temp_str;
  }

  /*!
   * \brief Throws the specified exception, or terminates when exceptions are deactivated.
   * \details See vac::language::ThrowOrTerminate
   * \param[in] error_code The error code to be thrown
   * \throws TlsDomainException with the passed error code.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is private to AMSR product
   */
  void ThrowAsException(TlsErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<TlsDomainException>(error_code);
  }
};

/*! \brief Short alias for TlsErrorDomain::TlsCommErrorCode */
using TlsCommErrorCode = TlsErrorDomain::TlsCommErrorCode;

/*! \brief Global TlsErrorDomain instance. */
constexpr TlsErrorDomain kTlsErrorDomain;

/*!
 * \brief Returns a reference to the global TlsErrorDomain instance.
 * \return TlsErrorDomain instance.
 * \context ANY
 * \pre -
 * \reentrant TRUE
 * \vprivate This method is private to AMSR product.
 */
inline constexpr amsr::core::ErrorDomain const& GetTlsCommErrorDomain() { return kTlsErrorDomain; }

/*!
 * \brief Function that creates an TlsErrorCode instance.
 * \param[in,out] code The "TlsCommErrorCode".
 * \param[in,out] data Supported data to be set.
 * \param[in,out] message The message string.
 * \return The created "TlsErrorCode" instance.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is private to AMSR product.
 */
inline constexpr TlsErrorCode MakeErrorCode(TlsErrorDomain::Errc code, amsr::core::ErrorDomain::SupportDataType data,
                                            amsr::core::ErrorDomain::StringType message) {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetTlsCommErrorDomain(), data, message};
}

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_ERROR_DEFINITIONS_H_
