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
/*!        \file  ara/crypto/x509/ocsp_response.h
 *        \brief  On-line Certificate Status Protocol (OCSP) Response Status.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_RESPONSE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_RESPONSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace x509 {

/*!
 * \brief On-line Certificate Status Protocol (OCSP) Response Status.
 * \details Storage type: 8 bit unsigned integer.
 * \vpublic
 */
enum class OcspResponseStatus : std::uint8_t {
  kSuccessful = 0,       /*!< Response has valid confirmations */
  kMalformedRequest = 1, /*!< Illegal confirmation request */
  kInternalError = 2,    /*!< Internal error in issuer */
  kTryLater = 3,         /*!< Try again later */
  kSigRequired = 5,      /*!< Must sign the request */
  kUnauthorized = 6      /*!< Request unauthorized */
};

/*!
 * \brief On-line Certificate Status Protocol (OCSP) Certificate Status.
 * \details Storage type: 8 bit unsigned integer.
 * \vpublic
 */
enum class OcspCertStatus : std::uint8_t {
  kGood = 0,    /*!< The certificate is not revoked */
  kRevoked = 1, /*!< The certificate has been revoked (either permanantly or temporarily (on hold)) */
  kUnknown = 2  /*!< The responder doesn't know about the certificate being requested */
};

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief On-line Certificate Status Protocol Response.
 * \vpublic
 */
class OcspResponse : public ara::crypto::Serializable,
                     public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface CustomDisposable
                                                             // (CustomDeleter requires interface)
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<OcspResponse, ara::crypto::CustomDeleter>;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<OcspResponse const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Get version of the OCSP response format.
   * \return OCSP response format version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_RESPONSE_H_
