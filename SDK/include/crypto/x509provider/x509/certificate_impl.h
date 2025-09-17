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
/*!        \file  crypto/x509provider/x509/certificate_impl.h
 *         \brief  X509 certificate implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_IMPL_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/asn1/structure/authority_key_identifier.h"
#include "amsr/asn1/structure/x509_certificate.h"
#include "ara/core/string.h"
#include "crypto/softwareprovider/cryp/x509_public_key_info.h"

#include "crypto/x509provider/x509/certificate.h"
#include "crypto/x509provider/x509/certificate_store_interface.h"
#include "crypto/x509provider/x509/x509_dn.h"

namespace crypto {
namespace x509provider {
namespace x509 {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief           Certification implementation.
 * \vprivate        Component Private
 */
class CertificateImpl final : public Certificate {
 public:
  /*!
   * \brief         Type alias for unique smart pointer to this class.
   * \vprivate      Component Private
   */
  using Uptr = std::unique_ptr<CertificateImpl, ara::crypto::CustomDeleter>;

  /*!
   * \brief         Type alias for unique smart pointer to this class.
   * \vprivate      Component Private
   */
  using Uptrc = std::unique_ptr<CertificateImpl const, ara::crypto::CustomDeleter>;

  /*!
   * \brief         Copy construct a new CertificateImpl instance
   * \param[in]     certificate Certificate from which the copy shall be made.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE If certificate is not accessed by another thread.
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  CertificateImpl(CertificateImpl const& certificate) noexcept;

  /*!
   * \brief         Destroy this CertificateImpl instance
   * \vprivate      Component Private
   */
  ~CertificateImpl() noexcept final = default;

  /*!
   * \brief         Move construct a new CertificateImpl instance
   * \param[in]     certificate Certificate to move construct from.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  CertificateImpl(CertificateImpl&& certificate) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateImpl const&) & -> CertificateImpl& = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateImpl&&) & noexcept -> CertificateImpl& = delete;

  /*!
   * \brief         Clone this instance.
   * \return        A unique pointer to the clone of this instance
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto Clone() const noexcept -> Certificate::Uptr final { return Create(*this); }

  /*!
   * \brief         Create a new Certificate instance.
   * \param[in]     default_crypto_provider Default crypto provider
   * \param[in]     der_encoded_certificate DER encoded certificate to be parsed
   * \param[in]     certificate_store A reference to the certificate store
   * \context       ANY
   * \pre           default_crypto_provider != nullptr
   * \error         ara::crypto::SecurityErrc::kInvalidArgument
   *                The provided certificate could not be parsed
   * \error         ara::crypto::SecurityErrc::kUnsupported
   *                Certain aspects of the certificate are not supported
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \return        Newly created certificate
   * \vprivate      Component Private
   */
  static auto Create(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                     ara::crypto::ReadOnlyMemRegion der_encoded_certificate,
                     CertificateStoreInterface const& certificate_store) noexcept -> ara::core::Result<Uptr>;

  // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
  /*!
   * \brief         Creator for the Certificate
   * \param[in]     certificate Certificate to copy from.
   * \return        Newly created certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto Create(CertificateImpl const& certificate) noexcept -> Uptr {
    return Uptr(new CertificateImpl(certificate));
  }

  /*!
   * \brief         Release allocated memory and other resources.
   * \details       May be used only if the object's memory was allocated using the new operator. The object must not be
   *                used once the method is called.
   * \context       ANY
   * \pre           The object's memory is allocated with the new operator. The memory is not released.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void Release() noexcept final { delete this; }

  /*!
   * \brief         Get the X.509 version of this certificate object.
   * \return        X.509 version
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto X509Version() const noexcept -> std::uint32_t final;

  /*!
   * \brief         Check whether this certificate belongs to a root CA.
   * \return        true if this certificate belongs to a root CA (i.e. the certificate is self-signed)
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IsRoot() const noexcept -> bool final;

  /*!
   * \brief         Get the certificate's issuer distinguished name.
   * \return        Issuer DN of this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IssuerDn() const -> X509DN const& final;

  /*!
   * \brief         Get the "Not Before" of the certificate.
   * \return        "Not Before" of the certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto StartTime() const noexcept -> time_t final;

  /*!
   * \brief         Get the "Not After" of the certificate.
   * \return        "Not After" of the certificate.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto EndTime() const noexcept -> time_t final;

  /*!
   * \brief         Get the serial number of this certificate.
   * \details       If (sn.empty() == true) then this method only returns required size of the output buffer.
   * \param[out]    sn an optional output buffer for storing the serial number.
   * \return        size of the certificate serial number in bytes.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity
   *                The buffer is too small to store the serial number.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SerialNumber(ara::crypto::WritableMemRegion sn) const noexcept -> ara::core::Result<std::size_t> final;

  /*!
   * \brief         Get the DER encoded keyIdentifier field of the AuthorityKeyIdentifier of this certificate.
   * \details       If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out]    id the optional output buffer.
   * \return        size of the DER encoded keyIdentifier in bytes.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity
   *                The buffer is too small to store the keyIdentifier
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto AuthorityKeyId(ara::crypto::WritableMemRegion id) const noexcept -> ara::core::Result<std::size_t> final;

  /*!
   * \brief         Get the DER encoded SubjectKeyIdentifier of this certificate.
   * \details       If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out]    id The optional output buffer.
   * \return        size of the DER encoded SubjectKeyIdentifier in bytes.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity
   *                The buffer is too small to store the SubjectKeyIdentifier
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SubjectKeyId(ara::crypto::WritableMemRegion id) const noexcept -> ara::core::Result<std::size_t> final;

  /*!
   * \brief         Verify signature of the certificate.
   * \details       Call with (ca_cert == nullptr) is applicable only if this is a certificate of a root CA.
   *                Note that this method ONLY verifies the signature and nothing else.
   * \param[in]     ca_cert the optional pointer to a Certification Authority certificate used for signature of the
   *                current one.
   * \return        true if this certificate was verified successfully and false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInvalidArgument
   *                If this is not a root certificate but \c ca_cert is nullptr
   * \error         SecurityErrorDomain::kEmptyContainer
   *                If the domain parameters are required, but (params == nullptr) was passed.
   * \error         SecurityErrorDomain::kIncompatibleObject
   *                If (params != nullptr), but provided domain parameters object has inappropriate type.
   * \error         SecurityErrorDomain::kIncompleteArgState
   *                If (params != nullptr), but provided domain parameters object has an incomplete state.
   * \error         SecurityErrorDomain::kBusyResource
   *                If the slot specified by \c reserved_index is still busy.
   * \error         SecurityErrorDomain::kUnreservedResource
   *                If the slot specified by \c reserved_index was not allocated.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto VerifyMe(ara::crypto::x509::Certificate const* ca_cert) const noexcept -> bool final;

  /*!
   * \brief         Calculate a fingerprint from the whole certificate.
   * \details       The produced fingerprint value saved to the output buffer starting from the least significant.
   * \details       If the capacity of the output buffer is less than the digest size then the digest will be truncated
   *                and only leading bytes will be saved.
   * \details       If the capacity of the output buffer is higher than the digest size then only leading bytes of the
   *                buffer will be updated.
   * \param[out]    fingerprint output buffer for the fingerprint storage.
   * \param[in]     hash_ctx an initialized hash function context.
   * \return        number of bytes actually saved to the output buffer.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kRuntimeFault
   *                Unable to calculate the fingerprint
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetFingerprint(ara::crypto::ReadWriteMemRegion fingerprint,
                      ara::crypto::cryp::HashFunctionCtx& hash_ctx) const noexcept
      -> ara::core::Result<std::size_t> final;

  /*!
   * \brief         Sets the certificate status.
   * \param[in]     status The status to be set.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \trace         CREQ-Crypto-X509Provider_X509_Trust_Management
   * \vprivate      Component Private
   */
  void SetStatus(Status status) const noexcept final;

  /*!
   * \brief         Return last verification status of the certificate.
   * \details       This is NOT affected by VerifyMe() and will return kUnknown until first verification at the
   *                X509Provider.
   * \return        the certificate verification status.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \trace         CREQ-Crypto-X509Provider_X509_Trust_Management
   * \vprivate      Component Private
   */
  auto GetStatus() const noexcept -> Status final;

  /*!
   * \brief         Load the subject public key information object to the realm of specified crypto provider.
   * \details       If (crypto_provider == nullptr) then X509PublicKeyInfo object will be loaded in realm of the
   *                stack default Crypto Provider
   * \param[in]     crypto_provider shared pointer of a target Crypto Provider, where the public key will be used.
   * \return        constant reference of the subject public key interface
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SubjectPubKey(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept
      -> ara::crypto::cryp::X509PublicKeyInfo const& final;

  /*!
   * \brief         Get the certificate's subject distinguished name.
   * \return        The Subject DN aof this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SubjectDn() const noexcept -> X509DN const& final;

  /*!
   * \brief         Find out whether this is a CA request.
   * \return        true if it is a CA request and false otherwise.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IsCa() const noexcept -> bool final;

  /*!
   * \brief         Get the constraint on the path length defined in the Basic Constraints extension.
   * \return        Certification path length limit.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetPathLimit() const noexcept -> std::uint32_t final;

  /*!
   * \brief         Get the key constraints for the key associated with this PKCS#10 object.
   * \return        key constraints.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetConstraints() const noexcept -> KeyConstraints final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief         Serialize itself publicly.
   * \param[out]    output The pre-allocated output buffer (it can be empty if only the required
   *                size of the output buffer is interesting).
   * \param[in]     format_id The Crypto Provider specific identifier of the output format.
   * \return        Size required for storing of the output object.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity
   *                If (output.empty() == false), but its capacity is less than required
   * \error         ara::crypto::SecurityErrc::kUnsupportedFormat
   *                If the specified format ID is not supported for this object type
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<std::size_t> final;

  /*!
   * \brief         Get signature algorithm identifier used to sign this object.
   * \return        Signature algorithm identifier.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetSignatureAlgorithm() const noexcept -> ara::crypto::CryptoAlgId final;

  /*!
   * \brief         Get the underlying ASN.1 certificate data object.
   * \return        The ASN.1 object representation of this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief         Get the authority key identifier of the certificate.
   * \return        The authority key identifier
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetAuthorityKeyId() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief         Get the subject key identifier of the certificate.
   * \return        The subject key identifier
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetSubjectKeyId() const noexcept -> ara::crypto::ReadOnlyMemRegion final;

  /*!
   * \brief         Get the extensions of the certificate.
   * \return        The the extensions contained in the certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetExtensions() const noexcept -> amsr::core::Span<Extension const> final;

  /*!
   * \brief         Determine whether this certificate is self-issued.
   * \return        True if this is self-issued, false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IsSelfIssued() const noexcept -> bool final { return SubjectDn() == IssuerDn(); }

  /*!
   * \brief         Determine whether this certificate is self-signed.
   * \return        True if this is self-signed, false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IsSelfSigned() const noexcept -> bool final;

  /*!
   * \brief         Get the certificate's issuer distinguished name (concrete type).
   * \return        The issuer DN of this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto ConcreteIssuerDn() const -> X509DN const& final { return *issuer_dn_; }

  /*!
   * \brief         Get the certificate's subject distinguished name (concrete type).
   * \return        The subject DN af this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto ConcreteSubjectDn() const noexcept -> X509DN const& final { return *subject_dn_; }

  /*!
   * \copydoc crypto::x509provider::x509::Certificate::Encode
   * \vprivate Component Private
   */
  auto Encode() const noexcept -> ara::crypto::ReadOnlyMemRegion final {
    return ara::crypto::ReadOnlyMemRegion{encoded_};
  }

 private:
  /*!
   * \brief         Construct a new Certificate instance.
   * \details       Sets the status of a certificate to Status::kUnknown
   * \param[in]     default_crypto_provider Default crypto provider
   * \param[in]     certificate_store A reference to the certificate store
   * \context       ANY
   * \pre           default_crypto_provider != nullptr
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  CertificateImpl(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                  CertificateStoreInterface const& certificate_store) noexcept;

  /*!
   * \brief         Parse DER encoded certificate.
   * \param[in]     der_encoded_certificate Certificate to parse
   * \return        The parsed ASN.1 structure
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE iff memory referred to by \c der_encoded_certificate is not simultaneously modified in another
   *                thread
   * \error         ara::crypto::SecurityErrc::kInvalidArgument
   *                If certificate could not be parsed successfully
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto ParseCertificate(ara::crypto::ReadOnlyMemRegion der_encoded_certificate) noexcept
      -> amsr::core::Result<amsr::asn1::structure::X509Certificate>;

  /*!
   * \brief         Parse the key constraints extension.
   * \param[in]     extensions Extension info to parse
   * \param[out]    instance Object instance to parse data into
   * \return        -
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE iff parameters are not accessed simultaneously in another thread
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto ReadKeyConstraints(ara::core::Vector<amsr::asn1::structure::Extension> const& extensions,
                                 CertificateImpl& instance) noexcept -> void;

  /*!
   * \brief         Parse the basic constraints extension.
   * \param[in]     extensions Extension info to parse
   * \param[out]    instance Object instance to parse data into
   * \return        -
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE iff parameters are not accessed simultaneously in another thread
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto ReadBasicConstraints(ara::core::Vector<amsr::asn1::structure::Extension> const& extensions,
                                   CertificateImpl& instance) noexcept -> void;

  std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider_; /*!< Default crypto provider */
  mutable Status status_;                                                      /*!< Certificate status */
  amsr::asn1::structure::X509Certificate asn1_certificate_; /*!< ASN1 object representation of this certificate */
  X509DN::Uptr subject_dn_;                                 /*!< Subject distinguished name */
  X509DN::Uptr issuer_dn_;                                  /*!< Issuer distinguished name */
  time_t not_before_;                                       /*!< Start of validity period */
  time_t not_after_;                                        /*!< End of validity period */
  mutable ara::crypto::cryp::X509PublicKeyInfo::Uptrc subject_public_key_info_; /*!< Subject public key info */
  bool is_ca_;                                                                  /*!< basicConstraints CA flag */
  std::uint32_t path_limit_;                                                    /*!< basicConstraints path limit */
  ara::crypto::x509::BasicCertInfo::KeyConstraints key_constraints_;            /*!< Key constraints */
  ara::crypto::CryptoAlgId signature_alg_id_;                                   /*!< Signature algorithm identifier */
  amsr::asn1::structure::AuthorityKeyIdentifier const* authority_key_id_;       /*!< Authority key identifier */
  amsr::asn1::structure::OctetString const* subject_key_id_;                    /*!< Subject public key info */
  ara::core::Vector<Extension> extensions_;                                     /*!< Certificate extensions */
  ara::core::Vector<std::uint8_t> encoded_;            /*!< DER encoded binary representation */
  CertificateStoreInterface const& certificate_store_; /*!< A reference to the certificate store */
  common::util::Logger logger_;                        /*!< Logging instance */

  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__Certificate, InitCertificateData);
  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__Certificate, CheckNameEncoding);
  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__Certificate, CopyConstructorFails);
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_IMPL_H_
