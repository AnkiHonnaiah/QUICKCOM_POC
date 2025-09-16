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
/*!        \file  crypto/x509provider/x509/cert_sign_request.h
 *        \brief  Provides X.509 certificate signing requests.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERT_SIGN_REQUEST_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERT_SIGN_REQUEST_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/asn1/structure/certification_request.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/x509_algorithm_id.h"
#include "ara/crypto/cryp/x509_public_key_info.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "crypto/softwareprovider/cryp/x509_public_key_info.h"

#include "crypto/x509provider/x509/x509_dn.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Certification sign request implementation.
 * \trace CREQ-Crypto-X509Provider_X509_Certification_Requests
 * \vprivate Component Private
 */
class CertSignRequest : public ara::crypto::x509::CertSignRequest {
 public:
  /*!
   * \brief Unique smart pointer of the concrete implementation.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<CertSignRequest, ara::crypto::CustomDeleter>;

  /*!
   * \brief Digest size.
   * \vprivate Component Private
   */
  static constexpr std::size_t kDigestSize{32};

  /*!
   * \brief Digest container type.
   * \vprivate Component Private
   */
  using DigestContainer = std::array<std::uint8_t, kDigestSize>;

  /*!
   * \brief Default destructor
   * \vprivate Component Private Component Private
   */
  ~CertSignRequest() noexcept override = default;

  /*!
   * \brief Copy constructor a CertSignRequest.
   * \vprivate Component Private
   */
  CertSignRequest(CertSignRequest const&) noexcept;

  /*!
   * \brief Deleted move constructor.
   * \vprivate Component Private
   */
  CertSignRequest(CertSignRequest&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertSignRequest const&) & noexcept -> CertSignRequest& = delete;

  /*!
   * \brief Deleted move assignment.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertSignRequest&&) & noexcept -> CertSignRequest& = delete;

  /*!
   * \brief Creator for the CSR
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] der_encoded_csr DER encoded CSR to be parsed
   * \return Newly created CSR
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument if the given data cannot be parsed or the parsed certificate is
   *                                                    invalid or if the \c default_crypto_provider is nullptr
   * \threadsafe TRUE If der_encoded_csr is not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto Create(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                     ara::crypto::ReadOnlyMemRegion der_encoded_csr) noexcept -> amsr::core::Result<Uptr>;

  /*!
   * \brief Creator for the CSR
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] asn1_csr ASN1 CSR to be parsed
   * \return Newly created CSR
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument if the given data cannot be parsed or the parsed certificate is
   *                                                    invalid or if the \c default_crypto_provider is nullptr
   * \error ara::crypto::SecurityErrc::kRuntimeFault If one of the sub-objects could not be created.
   * \threadsafe TRUE If der_encoded_csr is not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto Create(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                     amsr::asn1::structure::CertificationRequest asn1_csr) noexcept -> amsr::core::Result<Uptr>;

  // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
  /*!
   * \brief Creator for the CSR
   * \param[in] csr CSR to copy from.
   * \return Newly created CSR
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto Create(CertSignRequest const& csr) noexcept -> Uptr { return Uptr(new CertSignRequest(csr)); }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  /*!
   * \brief Creates a new x509::CertSignRequest from an the given cryp::X509CertRequest
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] x509_cert_request The cryp::X509CertRequest the x509::CertSignRequest shall be created from.
   * \return Newly created CSR
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument if the given data cannot be parsed or the parsed certificate is
   *                                                    invalid or if the \c default_crypto_provider is nullptr
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto FromX509CertRequest(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                                  ara::crypto::cryp::X509CertRequest const& x509_cert_request) noexcept
      -> amsr::core::Result<Uptr>;

  /*!
   * \brief Release allocated memory and other resources.
   * \details May be used only if the object's memory was allocated using the new operator.
           The object must not be used once the method is called.
   * \context ANY
   * \pre The object's memory is allocated with the new operator. The memory is not released.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Release() noexcept override { delete this; }

  /*!
   * \brief Verifies self-signed signature of the certificate request.
   * \return true if the signature is correct.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Verify() const noexcept -> bool override;

  /*!
   * \brief Get the challenge password for this request (if it was included to the request).
   * \param[out] password the optional pointer to an output string.
   * \return length of the password if it was provided or 0 otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE If password is not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ChallengePassword(ara::core::String* password) const noexcept -> ara::core::Result<std::size_t> override;

  /*!
   * \brief Load the subject public key information object to realm of specified crypto provider.
   * \details If (crypto_provider == nullptr) then X509PublicKeyInfo object
              will be loaded in realm of the Stack-default Crypto Provider
   * \param[in] crypto_provider shared pointer of a target Crypto Provider, where the public key will be used.
   * \return constant reference of the subject public key interface
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SubjectPubKey(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept
      -> ara::crypto::cryp::X509PublicKeyInfo const& override;

  /*!
   * \brief Get the subject DN.
   * \return subject DN.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SubjectDn() const noexcept -> X509DN const& override;

  /*!
   * \brief Find out whether this is a CA request.
   * \return true if it is a CA request and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsCa() const noexcept -> bool override;

  /*!
   * \brief Get the constraint on the path length defined in the Basic Constraints extension.
   * \return certification path length limit.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetPathLimit() const noexcept -> std::uint32_t override;

  /*!
   * \brief Get the key constraints for the key associated with this PKCS#10 object.
   * \return key constraints.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetConstraints() const noexcept -> KeyConstraints override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat The format is not supported.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The buffer is too small to hold the encoded data.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<std::size_t> override;

  /*!
   * \brief Gets the digest.
   * \return The digest for this instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetDigest() const noexcept -> DigestContainer const&;

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
  auto GetSignatureAlgorithm() const noexcept -> ara::crypto::CryptoAlgId override;

 private:
  /*!
   * \brief Create a new CertSignRequest instance
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] asn1_csr Internal data os ASN1 object
   * \param[in] subject_dn Subject distinguished name
   * \param[in] public_key_info Public key info
   * \param[in] digest Digest of this CSR
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CertSignRequest(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                  amsr::asn1::structure::CertificationRequest asn1_csr, X509DN::Uptr subject_dn,
                  ara::crypto::cryp::X509PublicKeyInfo::Uptrc public_key_info, DigestContainer digest) noexcept;

  std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider_; /*!< Default crypto provider. */
  amsr::asn1::structure::CertificationRequest asn1_csr_; /*!< ASN1 object representation of this CSR */
  X509DN::Uptr subject_dn_;                              /*!< subject distinguished name */
  mutable crypto::softwareprovider::cryp::X509PublicKeyInfo::Uptrc
      subject_public_key_info_; /*!< subject public key info */
  DigestContainer digest_;      /*!< Digest of this CSR */
};

/*!
 * \brief Equality operator for CertSignRequest objects.
 * \param[in] lhs Left hand side object for the comparison.
 * \param[in] rhs Right hand side object for the comparison.
 * \return true if both objects are considered equal, otherwise false.
 * \vprivate Component Private
 */
auto operator==(CertSignRequest const& lhs, CertSignRequest const& rhs) noexcept -> bool;

/*!
 * \brief Inequality operator for CertSignRequest objects.
 * \param[in] lhs Left hand side object for the comparison.
 * \param[in] rhs Right hand side object for the comparison.
 * \return true if both objects are not equal, otherwise false.
 * \vprivate Component Private
 */
auto operator!=(CertSignRequest const& lhs, CertSignRequest const& rhs) noexcept -> bool;

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERT_SIGN_REQUEST_H_
