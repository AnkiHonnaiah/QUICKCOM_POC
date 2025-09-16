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
/*!        \file  crypto/x509provider/x509/attribute_certificate_impl.h
 *        \brief  Implements X.509 attribute certificates.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_IMPL_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ctime>
#include <memory>
#include <utility>

#include "amsr/asn1/structure/attribute_certificate.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "vac/testing/test_adapter.h"

#include "crypto/x509provider/x509/attribute_certificate.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief AttributeCertificateImpl implementation.
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 * \vprivate Component Private
 */
class AttributeCertificateImpl final : public crypto::x509provider::x509::AttributeCertificate {
 public:
  /*!
   * \brief Type alias for unique smart pointer to this class.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<AttributeCertificateImpl, ara::crypto::CustomDeleter>;

  /*!
   * \brief Type alias for unique smart pointer to this class.
   * \vprivate Component Private
   */
  using Uptrc = std::unique_ptr<AttributeCertificateImpl const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Default destructor
   * \vprivate Component Private
   */
  ~AttributeCertificateImpl() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \param[in] attribute_certificate Attribute certificate
   * \vprivate Component Private
   */
  AttributeCertificateImpl(AttributeCertificateImpl const& attribute_certificate) noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  AttributeCertificateImpl(AttributeCertificateImpl&&) noexcept = default;

  /*!
   * \brief Deleted copy assign operator
   * \return -
   * \vprivate Component Private
   */
  auto operator=(AttributeCertificateImpl const&) & noexcept -> AttributeCertificateImpl& = delete;

  /*!
   * \brief Default move assign operator
   * \return -
   * \vprivate Component Private
   */
  auto operator=(AttributeCertificateImpl&&) & noexcept -> AttributeCertificateImpl& = default;

  /*!
   * \brief Creator for the AttributeCertificateImpl
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] der_encoded_certificate DER encoded attribute certificate to be parsed
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument The provided attribute certificate has invalid encoding.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to parse the attribute certificate's validity dates.
   * \threadsafe TRUE If der_encoded_certificate is not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \return Newly created certificate
   * \vprivate Component Private
   */
  static auto Create(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                     ara::crypto::ReadOnlyMemRegion der_encoded_certificate) noexcept -> amsr::core::Result<Uptr>;

  /*!
   * \brief Release allocated memory and other resources.
   * \details May be used only if the object's memory was allocated using the new operator.
           The object must not be used once the method is called.
   * \context ANY
   * \pre The object's memory is allocated with the new operator.
       The memory is not released.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Release() noexcept final { delete this; }

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
  auto GetHolderDn() const noexcept -> ara::crypto::x509::X509DN const* final;

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
  auto GetHolderSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief Get the distinguished name identifying the issuer.
   * \return distinguished name
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  auto GetIssuerDn() const noexcept -> ara::crypto::x509::X509DN const* final;

  /*!
   * \brief Get the serial number of the attribute certificate.
   * \return Serial number
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  auto GetSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief Get the start time of the validity time frame.
   * \return unix time stamp
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  auto StartTime() const noexcept -> std::time_t final;

  /*!
   * \brief Get the end time of the validity time frame.
   * \return unix time stamp
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   */
  auto EndTime() const noexcept -> std::time_t final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
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
  auto AuthorityKeyId(ara::crypto::WritableMemRegion id = ara::crypto::WritableMemRegion()) const noexcept
      -> ara::core::Result<std::size_t> final;

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
  auto SetStatus(Status status) const noexcept -> void final;

  /*!
   * \brief Return last verification status of the attribute certificate.
   * \return the attribute certificate verification status.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetStatus() const noexcept -> Status final;

  /*!
   * \brief Get the serial number of the attribute certificate.
   * \return Serial number
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetAttributeCertSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief Get signature algorithm identifier used to sign this object.
   * \return Signature algorithm identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSignatureAlgorithm() const noexcept -> ara::crypto::CryptoAlgId final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is of interest).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity
   *         if (output.empty() == false), but its capacity is less than required
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<std::size_t> final;

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
  auto GetAuthorityKeyIdentifier() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

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
  auto VerifyMe(Certificate const& ca_cert) const noexcept -> bool final;

 private:
  /*!
   * \brief Construct a new AttributeCertificate instance.
   * \details Sets the certificate status to Status::kUnknown
   * \param[in] default_crypto_provider Default crypto provider
   * \context ANY
   * \pre default_crypto_provider != nullptr
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  explicit AttributeCertificateImpl(
      std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider) noexcept;

  mutable Status status_;                                                      /*!< Attribute certificate status */
  std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider_; /*!< Default crypto provider */
  amsr::asn1::structure::AttributeCertificate asn1_structure_;                 /*!< ASN.1 structure representation */
  X509DN::Uptrc issuer_dn_;                                                    /*!< Issuer distinguished name */
  X509DN::Uptrc holder_dn_;                                                    /*!< Holder distinguished name */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  time_t not_before_; /*!< Validity period not before */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  time_t not_after_; /*!< Validity period not after */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  ara::crypto::CryptoAlgId signature_algorithm_id_; /*!< Signature algorithm identifier */
  ara::core::Vector<std::uint8_t> encoded_;         /*!< DER encoded binary representation */
  crypto::common::util::Logger logger_;             /*!< Logger instance */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__AttributeCertificateImpl, ExportPublicly);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__AttributeCertificateImpl, SetStatus);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__AttributeCertificateImpl, GetStatus);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__AttributeCertificateImpl, GetToBeSigned);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__CertificateVerifier, Verify_AttributeCertificate);
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ATTRIBUTE_CERTIFICATE_IMPL_H_
