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
/*!        \file  ara/crypto/x509/cert_sign_request.h
 *        \brief  Certificate Signing Request (CSR) object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/std_api.h"
#include "basic_cert_info.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Certificate Signing Request (CSR) object interface.
 * \details This interface is dedicated for complete parsing of the request content.
 * \vpublic
 */
class CertSignRequest : public BasicCertInfo,
                        public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface
                                                                // CustomDisposable (CustomDeleter requires interface)
 public:
  /*!
   * \brief Unique smart pointer of the constant interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<CertSignRequest const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<CertSignRequest, ara::crypto::CustomDeleter>;

  /*!
   * \brief Verifies self-signed signature of the certificate request.
   * \return true if the signature is correct.
   * \vpublic
   */
  virtual bool Verify() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the challenge password for this request (if it was included to the request).
   * \param[out] password the optional pointer to an output string.
   * \return length of the password if it was provided or 0 otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if (password != nullptr), but its
   * capacity is less then required for storing the password value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ChallengePassword(ara::core::String* password = nullptr) const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
