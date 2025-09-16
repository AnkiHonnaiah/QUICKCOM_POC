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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  crypto/x509provider/x509/attribute_certificate.h
 *        \brief  Extended attribute certificate interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/x509/attribute_certificate.h"
#include "crypto/x509provider/x509/certificate.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief AttributeCertificate implementation.
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 * \vprivate Component Private
 */
class AttributeCertificate : public ara::crypto::x509::AttributeCertificate {
 public:
  /*!
   * \brief Get the distinguished name corresponding to the subject name of the holder certificate.
   * \return distinguished name the holder certificate
   * \context ANY
   * \pre DN has been set
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetHolderDn() const noexcept -> ara::crypto::x509::X509DN const* = 0;

  /*!
   * \brief Get the serial number of the holder certificate.
   * \return serial number of the holder certificate
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetHolderSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief Get the distinguished name identifying the issuer.
   * \return distinguished name
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  virtual auto GetIssuerDn() const noexcept -> ara::crypto::x509::X509DN const* = 0;

  /*!
   * \brief Get the serial number of the attribute certificate.
   * \return Serial number
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  virtual auto GetSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief Get the authority key identifier.
   * \context ANY
   * \pre -
   * \return The Key identifier of the key that signed this certificate
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetAuthorityKeyIdentifier() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief Sets the certificate status.
   * \param[in] status The status to be set.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto SetStatus(Status status) const noexcept -> void = 0;

  /*!
   * \brief Verify signature of the attribute certificate.
   * \param[in] ca_cert The issuer CA certificate to verify against
   * \return true if the signature of this attribute certificate was verified successfully, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto VerifyMe(Certificate const& ca_cert) const noexcept -> bool = 0;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_H_
