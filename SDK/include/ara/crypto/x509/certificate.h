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
/*!        \file  ara/crypto/x509/certificate.h
 *        \brief  X.509 Certificate interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERTIFICATE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/std_api.h"
#include "basic_cert_info.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief X.509 Certificate interface.
 * \vpublic
 */
class Certificate : public BasicCertInfo,
                    public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface CustomDisposable
                                                            // (CustomDeleter requires interface)
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<Certificate, ara::crypto::CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<Certificate const, ara::crypto::CustomDeleter>;

  // Attention! Deviation: The enum value kRevoked is not standardized
  /*!
   * \brief Certificate verification status.
   * \details Storage type: 8 bit unsigned integer.
   * \vpublic
   */
  enum class Status : std::uint8_t {
    kValid = 0,   /*!< The certificate is valid */
    kInvalid = 1, /*!< The certificate is invalid */
    kUnknown = 2, /*!< The status of the certificate is not known yet */
    kNoTrust = 3, /*!< No valid path to a root of trust could be found */
    kExpired = 4, /*!< The certificate is already expired (its validity period has ended) */
    kFuture = 5,  /*!< The certificate's validity period is not started yet */
    kRevoked = 6  /*!< The certificate is revoked */
  };

  /*!
   * \brief Get the X.509 version of this certificate object.
   * \return X.509 version
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t X509Version() const noexcept = 0;

  /*!
   * \brief Check whether this certificate belongs to a root CA.
   * \return true if the TrustMaster has set this certificate as root
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsRoot() const noexcept = 0;

  /*!
   * \brief Get the issuer certificate DN.
   * \return Issuer DN of this certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual X509DN const& IssuerDn() const = 0;

  /*!
   * \brief Get the "Not Before" of the certificate.
   * \return "Not Before" of the certificate
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual time_t StartTime() const noexcept = 0;

  /*!
   * \brief Get the "Not After" of the certificate.
   * \return "Not After" of the certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual time_t EndTime() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the serial number of this certificate.
   * \details If (sn.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] sn an optional output buffer for storing the serial number.
   * \return size of the certificate serial number in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if (sn.empty() == false),
   *                   but its size is not enough for storing the output value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> SerialNumber(
      ara::crypto::WritableMemRegion sn = ara::crypto::WritableMemRegion()) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
   * \details If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] id the optional output buffer.
   * \return size of the DER encoded AuthorityKeyIdentifier in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if (id.empty() == false),
   *                   but its size is not enough for storing the output value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> AuthorityKeyId(
      ara::crypto::WritableMemRegion id = ara::crypto::WritableMemRegion()) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the DER encoded SubjectKeyIdentifier of this certificate.
   * \details If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] id  the optional output buffer.
   * \return size of the DER encoded SubjectKeyIdentifier in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if (id.empty() == false),
   *                   but its size is not enough for storing the output value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> SubjectKeyId(
      ara::crypto::WritableMemRegion id = ara::crypto::WritableMemRegion()) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Verify signature of the certificate.
   * \details Call with (ca_cert == nullptr) is applicable only if this is a certificate of a root CA.
   *          Note that this method ONLY verifies the signature and nothing else.
   * \param[in] ca_cert the optional pointer to a Certification Authority certificate
   *                   used for signature of the current one.
   * \return true if this certificate was verified successfully and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool VerifyMe(Certificate const* ca_cert = nullptr) const noexcept = 0;

  /*!
   * \brief Calculate a fingerprint from the whole certificate.
   * \details The produced fingerprint value saved to the output buffer starting from the least significant.
   * \details If the capacity of the output buffer is less than the digest size then the digest
   *          will be truncated and only leading bytes will be saved.
   * \details If the capacity of the output buffer is higher than the digest size then only leading
   *          bytes of the buffer will be updated.
   * \param[out] fingerprint output buffer for the fingerprint storage.
   * \param[in] hash_ctx an initialized hash function context.
   * \return number of bytes actually saved to the output buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompleteArgState  if the hash_ctx context is not
   *                   initialized by required domain parameters.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetFingerprint(
      ara::crypto::ReadWriteMemRegion fingerprint, ara::crypto::cryp::HashFunctionCtx& hash_ctx) const noexcept = 0;

  /*!
   * \brief Return last verification status of the certificate.
   * \details This is NOT affected by VerifyMe() and will return kUnknown until first verification at the X509Provider.
   * \return the certificate verification status.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Status GetStatus() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_CERTIFICATE_H_
