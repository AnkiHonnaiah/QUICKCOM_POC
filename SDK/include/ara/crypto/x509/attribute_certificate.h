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
/*!        \file  ara/crypto/x509/attribute_certificate.h
 *        \brief  X.509 Attribute Certificate interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ATTRIBUTE_CERTIFICATE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ATTRIBUTE_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief X.509 AttributeCertificate interface.
 * \vpublic
 */
class AttributeCertificate : public ara::crypto::Serializable, public ara::crypto::CustomDisposable {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<AttributeCertificate, ara::crypto::CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<AttributeCertificate const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Attribute certificate verification status.
   * \details Storage type: 8 bit unsigned integer.
   * \vpublic
   */
  enum class Status : std::uint8_t {
    kValid = 0,    /*!< The attribute certificate is valid */
    kInvalid = 1,  /*!< The attribute certificate is invalid */
    kUnknown = 2,  /*!< Status of the attribute certificate is not known yet */
    kNoHolder = 3, /*!< The holder of the attribute certificate could not be found */
    kNoIssuer = 4, /*!< The issuer of the attribute certificate could not be found */
    kExpired = 5,  /*!< The attribute certificate is expired (its validity period has ended) */
    kFuture = 6,   /*!< The attribute certificate is not yet valid (its validity period has not started) */
  };

  /*!
   * \brief Return last verification status of the attribute certificate.
   * \return the verification status.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto GetStatus() const noexcept -> Status = 0;

  /*!
   * \brief Get the serial number of the attribute certificate.
   * \return Serial number
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto GetAttributeCertSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief Get signature algorithm identifier used to sign this object.
   * \return Signature algorithm identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto GetSignatureAlgorithm() const noexcept -> CryptoAlgId = 0;

  /*!
   * \brief Get the start time of the validity time frame of the attribute certificate.
   * \return Validity start time (notBeforeTime).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto StartTime() const noexcept -> time_t = 0;

  /*!
   * \brief Get the end time of the validity time frame of the attribute certificate.
   * \return Validity end time (notAfterTime).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto EndTime() const noexcept -> time_t = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the DER encoded AuthorityKeyIdentifier of this attribute certificate.
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
  virtual auto AuthorityKeyId(ara::crypto::WritableMemRegion id = ara::crypto::WritableMemRegion()) const noexcept
      -> ara::core::Result<std::size_t> = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ATTRIBUTE_CERTIFICATE_H_
