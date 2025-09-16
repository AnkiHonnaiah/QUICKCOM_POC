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
/*!        \file  ara/crypto/cryp/x509_cert_request.h
 *        \brief  Certificate Request object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_CERT_REQUEST_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_CERT_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_object.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "ara/crypto/cryp/x509_public_key_info.h"
#include "ara/crypto/cryp/x509_signature.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Certificate Request object interface.
 * \vpublic
 */
class X509CertRequest : public CryptoObject, public Serializable {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kCertSignRequest};

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<X509CertRequest const, CustomDeleter>;  // Attention! Deviation: Added CustomDeleter
                                                                        // since class inherits from CustomDisposable

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  /*!
   * \brief Verify self-signed signature of the certificate request.
   * \param[in] hash  a temporary hash-function context that should be used in the call (the Alg ID can be got as
   *            \c X509Signature::GetPrimitiveId())
   * \param[in] verifier  a temporary signature verification context that should be used in the call (the Alg ID can
   *            be got as \c X509Signature::GetRequiredHashAlgId())
   * \return \c true if the signature is correct and \c false otherwise
   * \details This method uses key values and domain parameters stored inside the object!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if the \c hash or \c verifier arguments are configured
   *                for algorithms different from the one applied for signature of this certification request
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> Verify(HashFunctionCtx& hash, VerifierPublicCtx& verifier) const noexcept = 0;

  /*!
   * \brief Return format version of the certificate request.
   * \return Format version of the certificate request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t Version() const noexcept = 0;

  /*!
   * \brief Return signature object of the request.
   * \return Signature object of the request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual X509Signature const& Signature() const noexcept = 0;

  /*!
   * \brief Return subject public key included to the request.
   * \return Subject public key included to the request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual X509PublicKeyInfo const& SubjectPublicKeyInfo() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_CERT_REQUEST_H_
