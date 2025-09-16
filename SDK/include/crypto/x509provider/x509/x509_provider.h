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
/*!        \file  crypto/x509provider/x509/x509_provider.h
 *        \brief  Provides X.509 provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_PROVIDER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_PROVIDER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/x509/x509_provider.h"
#include "vac/testing/test_adapter.h"

#include "crypto/common/util/logger.h"
#include "crypto/x509provider/x509/certificate_store.h"
#include "crypto/x509provider/x509/csr_store.h"

namespace crypto {
namespace x509provider {
namespace x509 {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief X509 provider implementation.
 * \vprivate Component Private
 */
class X509Provider : public ara::crypto::x509::X509Provider {
 public:
  /*!
   * \brief Constructs a new X509Provider.
   * \param[in] crypto_provider CryptoProvider to be used in this X509 provider
   * \param[in] storage_root Path to the storage root
   * \param[in] certificate_store CertificateStore to be used by this X509 provider, if parameter is nullptr a default
   *                              instance will be created
   * \param[in] persistency_provider PersistencyProvider to be used by this X509 provider, if parameter is nullptr a
   *                                 default instance will be created
   * \context ANY
   * \pre crypto_provider != nullptr
   * \threadsafe TRUE If config is not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  X509Provider(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider, amsr::core::String const& storage_root,
               std::unique_ptr<CertificateStore> certificate_store = nullptr,
               std::unique_ptr<PersistencyProvider> persistency_provider = nullptr);

  /*!
   * \brief Get the provider's version
   * \return version
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetProviderVersion() const noexcept override;

  /*!
   * \brief Returns a human readable name of the Provider.
   * \details Life-time of the returned string is not less than the Provider instance life-time.
   * \return A pointer to null-terminated string with Provider Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  char const* GetProviderName() const noexcept override;

  /*!
   * \brief Get type of the Provider.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::ProviderType GetProviderType() const noexcept override;

  /*!
   * \brief Returns the Provider's Globally Unique Identifier (GUID).
   * \param[out] provider_uid An output buffer for the Provider's GUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The provider_uid  argument must not be accessed by another at the same time.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void GetProviderUid(ara::crypto::Guid& provider_uid) const noexcept override;

  /*!
   * \brief Create an empty X.500 Distinguished Name (DN) structure.
   * \param[in] capacity  number of bytes that should be reserved for the content of the target X509DN object.
   * \return Unique smart pointer to created.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptr> CreateEmptyDn(std::size_t capacity) noexcept override;

  /*!
   * \brief Create completed X.500 Distinguished Name structure from the provided string representation.
   * \details Method not supported.
   * \param[in] dn string representation of the Distinguished Name.
   * \return unique smart pointer for the created X509DN object.
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptrc> BuildDn(ara::core::StringView dn) noexcept override;

  /*!
   * \brief Decode X.500 Distinguished Name structure from the provided serialized format.
   * \param[in] dn DER representation of the Distinguished Name.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer for the created X509DN object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE If dn is not accessed by another thread.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If the given format is not supported.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the given DN is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptrc> DecodeDn(
      ara::crypto::ReadOnlyMemRegion dn, ara::crypto::Serializable::FormatId format_id) noexcept override;

  /*!
   * \brief Parse a serialized representation of the certificate and create its instance.
   * \details Off-line validation of the parsed certificate may be done via call VerifyCertByCrl().
   * \details After validation the certificate may be imported to the session or persistent storage
   *          for following search and usage.
   * \details If the parsed certificate is not imported then it will be lost after destroy of the returned instance!
   * \details Only imported certificate may be found by a search and applied for automatic verifications!
   * \details For PEM certificates only label 'CERTIFICATE' is supported.
   * \param[in] cert DER/PEM-encoded certificate.
   * \param[in] format_id input format identifier (kFormatDefault means DER encoded).
   * \return unique smart pointer to created certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE If cert is not accessed by another thread.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id ==
   * ara::crypto::Serializable::kFormatRawValueOnly).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Parsing_ASN1
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Parsing_PEM
   */
  ara::core::Result<ara::crypto::x509::Certificate::Uptr> ParseCert(
      ara::crypto::ReadOnlyMemRegion cert, ara::crypto::Serializable::FormatId format_id) noexcept override;

  /*!
   * \brief Parse a serialized representation of an attribute certificate and create its instance.
   * \details Off-line validation of the parsed attribute certificate may be done via call VerifyAttributeCert()
   * \param[in] attribute_cert DER/PEM-encoded attribute certificate.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer to created attribute certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the attribute_cert argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \error SecurityErrorDomain::kUnsupported if the format_id argument is not supported.
   * \error SecurityErrorDomain::kBadAlloc if the certificate can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_Parsing
   */
  auto ParseAttributeCert(ara::crypto::ReadOnlyMemRegion attribute_cert,
                          ara::crypto::Serializable::FormatId format_id) const noexcept
      -> ara::core::Result<ara::crypto::x509::AttributeCertificate::Uptr> override;

  /*!
   * \brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
   * \param[in] cert_chain DER encoded certificate chain or PEM certificate chain (in form of a single BLOB)
   * \param[in] format_id input format identifier (kFormatDefault means DER encoded certificate)
   * \return number of certificates in the chain
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id ==
   * ara::crypto::Serializable::kFormatRawValueOnly).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate chain.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> CountCertsInChain(
      ara::crypto::ReadOnlyMemRegion cert_chain, ara::crypto::Serializable::FormatId format_id) const noexcept override;

  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \details For PEM certificates only label 'CERTIFICATE' is supported.
   * \param[out] outcome an output vector for imported certificates
   * \param[in] cert_chain DER encoded certificate chain or PEM certificate chain (in form of a single BLOB)
   * \param[in] format_id input format identifier (kFormatDefault means DER encoded certificate)
   * \context ANY
   * \pre -
   * \threadsafe TRUE If outcome and cert_chain are not accessed by another thread.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id ==
   * ara::crypto::Serializable::kFormatRawValueOnly).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate chain.
   * \reentrant TRUE If a separate outcome vector is used for each call.
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Parsing_ASN1, CREQ-Crypto-X509Provider_X509_Certificate_Parsing_PEM
   */
  ara::core::Result<void> ParseCertChain(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                         ara::crypto::ReadOnlyMemRegion cert_chain,
                                         ara::crypto::Serializable::FormatId format_id) noexcept override;

  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \details For PEM certificates only label 'CERTIFICATE' is supported.
   * \param[out] outcome output vector of imported certificates.
   * \param[in] cert_chain DER encoded certificates chain or PEM certificate (each certificate is presented by a
   * separate BLOB in the input vector)
   * \param[in] format_id input format identifier (kFormatDefault means DER encoded certificate)
   * \context ANY
   * \pre -
   * \threadsafe TRUE The outcome vector must not be accessed by another at the same time The cert_chain vector must not
   * be accessed by another at the same time
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id ==
   * ara::crypto::Serializable::kFormatRawValueOnly).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse one of the given certificates.
   * \reentrant TRUE If a separate outcome vector is used for each call.
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Parsing_ASN1, CREQ-Crypto-X509Provider_X509_Certificate_Parsing_PEM
   */
  ara::core::Result<void> ParseCertChain(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                         ara::core::Vector<ara::crypto::ReadOnlyMemRegion> const& cert_chain,
                                         ara::crypto::Serializable::FormatId format_id) noexcept override;

  /*!
   * \brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \param[in] cert target certificate for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate. Will never return Status::kUnknown.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The certificate must not be accessed by another at the same time
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-Crypto-X509_CertificationPathValidation
   */
  ara::crypto::x509::Certificate::Status VerifyCertByCrl(
      ara::crypto::x509::Certificate& cert, ara::core::Optional<time_t> reference_time_point) const noexcept override;

  /*!
   * \brief Verify status of the provided certification chain by locally stored CA certificates and CRLs only.
   * \details Verification status of the certificate chain is Certificate::Status::kValid only if
   *          all certificates in the chain have such status!
   * \details Certificates in the chain (container vector) must be placed from the root CA certificate (zero index) to
   *          the target end-entity certificate (last used index of the vector). Verification is executed in same order.
   * \details If the root CA (zero index) has not been imported and set as root of trust Certificate::Status::kNoTrust
   *          is returned.
   * \details If the chain verification is failed then status of the first failed certificate is returned.
   * \details This method updates the Certificate::Status associated with the certificates in the chain.
   *          In case of verification failure this affects all certificates up to the point of failure.
   *          Note this only holds true if the given chain is correctly structured matching previously stated
   *          requirements. If the input vector does not conform to the structure, i.e. element zero is not a root CA
   *          and subsequent elements do not form a uninterrupted chain the status of each certificate in the chain is
   *          not modified.
   * \details Actual path verification is only performed iff the given chain matches the structural requirements.
   * \details Verification status of an empty chain is Certificate::Status::kInvalid.
   * \param[in] chain target certificate chain for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate chain. Will never return Status::kUnknown.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The certificate chain must not be accessed by another at the same time
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::Certificate::Status VerifyCertChainByCrl(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const& chain,
      ara::core::Optional<time_t> reference_time_point) const noexcept override;

  /*!
   * \brief Verify status of the provided attribute certificate by locally stored CA certificates.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \details Attribute Certificates can only be verified, if a valid Issuer and a valid Holder are already imported,
   *          otherwise it will return NoIssuer/NoHolder.
   * \param[in] attribute_cert target certificate for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided attribute certificate. Will never return a Status::kUnknown.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_Validation
   */
  auto VerifyAttributeCert(ara::crypto::x509::AttributeCertificate& attribute_cert,
                           ara::core::Optional<time_t> reference_time_point) const noexcept
      -> ara::crypto::x509::AttributeCertificate::Status override;

  /*!
   * \brief Import Certificate Revocation List (CRL) or Delta CRL from a file.
   * \details Method not supported.
   * \param[in] crl serialized CRL or Delta CRL (in form of a BLOB).
   * \return true if the CRL is valid and false if it is already expired.
   * \vprivate Component Private
   */
  ara::core::Result<bool> ImportCrl(ara::crypto::ReadOnlyMemRegion crl) noexcept override;

  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Import the certificate to volatile or persistent storage.
   * \details Only imported certificate may be found by a search and applied for automatic verifications!
   * \details A certificate can be imported to only one of store: volatile or persistent. Therefore if you
   *          import a certificate already kept in the persistent storage to the volatile one then nothing changes.
   *          But if you import a certificate already kept in volatile to the persistent storage one then it is "moved"
   *          to the persistent realm.
   * \details If an application successfully imports a certificate that correspond to a CSR existing in the storage
   *          then this CSR should be removed.
   * \param[in] cert a valid certificate that should be imported.
   * \param[in] to_volatile if this flag true then certificate should be saved to the volatile (session) storage,
   *                        otherwise to the persistent storage.
   * \param[in] label certificate label.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if (cert.GetStatus() != Certificate::Status::kValid).
   * \error ara::crypto::SecurityErrc::kContentDuplication Certificate already exists.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to import the certificate.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  ara::core::Result<void> Import(ara::crypto::x509::Certificate const& cert, bool to_volatile,
                                 amsr::core::StringView label = amsr::core::StringView{}) noexcept override;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Find a certificate by label.
   * \param[in] label The target certificate label.
   * \return An unique smart pointer to the instantiated certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource if the InstanceSpecifier is incorrect (the certificate cannot
   *                   be found)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::Certificate::Uptr> LoadCertificate(
      amsr::core::StringView label) noexcept override;

  /*!
   * \brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
   * \param[in] cert a unique smart pointer to a certificate that should be removed.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  bool Remove(ara::crypto::x509::Certificate::Uptrc&& cert) noexcept override;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Remove certificate with specified label from the storage (volatile or persistent) and destroy it.
   * \param[in] label the certificate's label.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  bool RemoveCertificate(amsr::core::StringView label) noexcept override;

  /*!
   * \brief Removes the given certificate from the certificate storage without deleting it.
   * \param[in] cert Certificate to remove.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  virtual bool Remove(ara::crypto::x509::Certificate const* cert) noexcept;

  /*!
   * \brief Save a prepared certificate signing request (CSR) to volatile or persistent storage.
   * \details Successfully saved request is located in special area of the storage dedicated for pending requests!
   * \details Saved CSR obtains status "new" that can be changed to "pending" status when a responsible application
   *          send it to correspondent CA (see SetPendingStatus() method).
   * \details A CSR may be saved only if a trusted certificate from a CA specified by authority_dn exists in the
   * storage. This CA certificate may be trusted as a "root of trust" or via a validated "chain of trust".
   * \param[in] request a valid certificate signature request (that should be send to CA).
   * \param[in] authority_dn target certification authority (CA) to which the request is addressed.
   * \param[in] to_volatile if this flag true then the request should be saved to the volatile
   * (session) storage, otherwise to the persistent storage.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kContentDuplication CSR already exists.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier The certificate store contains no CA certificate issued by the
   * given authority.
   * \error ara::crypto::SecurityErrc::kInvalidArgument The given X509CertRequest is invalid.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed the save the X509CertRequest.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certification_Request_Management
   */
  ara::core::Result<void> SaveCertSignRequest(ara::crypto::cryp::X509CertRequest const& request,
                                              ara::crypto::x509::X509DN const& authority_dn,
                                              bool to_volatile) noexcept override;

  /*!
   * \brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
   * \details This method does nothing if the CSR already marked as "pending".
   * \details Only an application with permissions "CA Connector" has the right to call this method!
   * \param[in] request  certificate signing request that should be marked as "pending".
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument CertSignRequest not found in the csr store.
   * \error ara::crypto::SecurityErrc::kAccessViolation Permission denied.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetPendingStatus(ara::crypto::x509::CertSignRequest const& request) noexcept override;

  /*!
   * \brief Set specified CA certificate as a "root of trust".
   * \details Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
   * \details Only CA certificate can be a "root of trust"!
   * \details Multiple certificates on an ECU may be marked as the "root of trust".
   * \details Only an application with permissions "Trust Master" has the right to call this method!
   * \param[in] ca_cert a valid CA certificate that should be trusted.
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid or has not been imported yet.
   * \error SecurityErrorDomain::kIncompatibleObject if provided certificate is a CA certificate.
   * \error SecurityErrorDomain::kAccessViolation if the method called by an application without
   *                  the "Trust Master" permission.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Trust_Management
   */
  ara::core::Result<void> SetAsRootOfTrust(ara::crypto::x509::Certificate const& ca_cert) noexcept override;

  /*!
   * \brief Create OCSP request for specified certificate.
   * \details Method not supported.
   * \param[in] cert  a certificate that should be verified
   * \param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
   * \return unique smart pointer to the created OCSP request
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspRequest::Uptrc> CreateOcspRequest(
      ara::crypto::x509::Certificate const& cert, ara::crypto::cryp::SignerPrivateCtx const* signer) noexcept override;

  /*!
   * \brief Create OCSP request for specified list of certificates.
   * \details Method not supported.
   * \param[in] cert_list  a certificates' list that should be verified
   * \param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
   * \return unique smart pointer to the created OCSP request
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspRequest::Uptrc> CreateOcspRequest(
      ara::core::Vector<ara::crypto::x509::Certificate const*> const& cert_list,
      ara::crypto::cryp::SignerPrivateCtx const* signer) noexcept override;

  /*!
   * \brief Parse serialized OCSP response and create correspondent interface.
   * \details This method may be used for implementation of the "OCSP stapling".
   * \param[in] response a serialized OCSP response.
   * \return unique smart pointer to the created OCSP response instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnexpectedValue if the provided BLOB response
   *                   doesn't keep an OCSP response.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-X509Provider_X509OcspResponse_Parsing
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspResponse::Uptrc> ParseOcspResponse(
      ara::crypto::ReadOnlyMemRegion response) const noexcept override;

  /*!
   * \brief Check certificate status by directly provided OCSP response. This method may be used for implementation of
   *        the "OCSP stapling".
   * \details This method updates the Certificate::Status associated with the certificate.
   *          If the given certificate is revoked, the method updates the status of the
   *          child certificates to kRevoked.
   * \param[in] cert a certificate that should be verified.
   * \param[in] ocsp_response  an OCSP response.
   * \return true if the certificates list is verified successfully and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidArgument if the cert is invalid.
   * \error SecurityErrorDomain::kRuntimeFault if the ocsp_response is invalid.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-X509Provider_X509OcspResponse_CheckCertStatus
   * \vprivate Component Private
   */
  ara::core::Result<bool> CheckCertStatus(ara::crypto::x509::Certificate& cert,
                                          ara::crypto::x509::OcspResponse const& ocsp_response) const noexcept override;

  /*!
   * \brief Check status of a certificates list by directly provided OCSP response.
   * \details Method not supported.
   * \param[in] cert_list  a certificates list that should be verified
   * \param[in] ocsp_response  an OCSP response
   * \return \c true if the certificates list is verified successfully and false otherwise
   * \vprivate Component Private
   */
  ara::core::Result<bool> CheckCertStatus(ara::core::Vector<ara::crypto::x509::Certificate*> const& cert_list,
                                          ara::crypto::x509::OcspResponse const& ocsp_response) const noexcept override;

  /*!
   * \brief Find a certificate by the subject and issuer Distinguished Names (DN).
   * \details Argument cert_index represents an internal index of current certificate in the storage.
   * \details In order to start certificate search from begin: cert_index = kInvalidIndex
   * \param[in] subject_dn subject DN of the target certificate.
   * \param[in] issuer_dn issuer DN of the target certificate.
   * \param[in] validity_time_point a time point when the target certificate should be valid.
   * \param[in,out] cert_index an index for iteration through all suitable certificates in the storage
   *                          (input: index of previous found certificate, output: index of current found certificate).
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  ara::crypto::x509::Certificate::Uptrc FindCertByDn(ara::crypto::x509::X509DN const& subject_dn,
                                                     ara::crypto::x509::X509DN const& issuer_dn,
                                                     time_t validity_time_point,
                                                     X509Provider::StorageIndex& cert_index) noexcept override;

  /*!
   * \brief Find a certificate by its SKID & AKID.
   * \param[in] subject_key_id subject key identifier (SKID).
   * \param[in] authority_key_id optional authority key identifier (AKID).
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  ara::crypto::x509::Certificate::Uptrc FindCertByKeyIds(
      ara::crypto::ReadOnlyMemRegion subject_key_id, ara::crypto::ReadOnlyMemRegion authority_key_id) noexcept override;

  /*!
   * \brief Find a certificate by its serial number.
   * \param[in] sn serial number of the target certificate.
   * \param[in] issuer_dn authority DN.
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
   */
  ara::crypto::x509::Certificate::Uptrc FindCertBySn(ara::crypto::ReadOnlyMemRegion sn,
                                                     ara::crypto::x509::X509DN const& issuer_dn) noexcept override;

  /*!
   * \brief Find a certificate signing request (CSR) kept in the storage and waiting for the certificate.
   * \details The optional arguments should be used for filtering of the CSRs that will be found.
   * \details Argument request_index represents an internal index of current request in the storage.
   * \details In order to start requests search from begin: request_index = kInvalidIndex
   * \param[in,out] request_index an index of the last found request in the storage,
   *                             it is dedicated for iteration through all (suitable for the provided filter)
   *                             CSRs waiting for certificates (input: index of previous found CSR, output:
   *                             index of current found CSR).
   * \param[in] authority_dn optional authority DN of the target CA that should issue the certificate.
   * \param[in] subject_dn optional subject DN of the request (if only specific subject interested).
   * \param[in] pending_csr optional flag that specifies the processing status of the interested requests:
   *                       "new" (if true) or "pending" (if false).
   * \return unique smart pointer to a found certificate signing request or nullptr if nothing is found
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Certification_Request_Management
   */
  ara::crypto::x509::CertSignRequest::Uptrc FindCertSignRequest(X509Provider::StorageIndex& request_index,
                                                                ara::crypto::x509::X509DN const* authority_dn,
                                                                ara::crypto::x509::X509DN const* subject_dn,
                                                                bool pending_csr) noexcept override;

  /*!
   * \brief Find a certificate by the subject and issuer Distinguished Names (DN) using wildcards.
   * \param[out] outcome Vector for holding the found certificates.
   * \param[in] subject_dn subject DN of the target certificate.
   * \param[in] issuer_dn issuer DN of the target certificate.
   * \param[in] validity_time_point a time point when the target certificate should be valid.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kRuntimeFault Unexpected error.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509FindByWildcard
   */
  ara::core::Result<void> FindCertByDnWildcard(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                               ara::crypto::x509::X509DN const& subject_dn,
                                               ara::crypto::x509::X509DN const& issuer_dn,
                                               time_t validity_time_point) noexcept override;

  /*!
   * \brief Cleanup the volatile certificates storage.
   * \details After execution of this command the certificates previously imported to the volatile storage
   *          cannot be found by a search, but it doesn't influence to already loaded Certificate instances!
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void CleanupVolatileStorage() noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \details OIDs given with \c known_extension_oids parameters will not lead to validation failure if matching
   *          extensions are encountered and marked critical.
   * \param[in] cert target certificate for verification.
   * \param[in] known_extension_oids Vector of known extension OID strings.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate. Will never return Status::kUnknown.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_CertificationPathValidation_CustomExtensions
   */
  auto VerifyCertExt(ara::crypto::x509::Certificate& cert, ara::core::Vector<ara::core::String> known_extension_oids,
                     ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::Certificate::Status override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Verify status of the provided certification chain by locally stored CA certificates only.
   * \details Verification status of the certificate chain is Certificate::Status::kValid only if
   *          all certificates in the chain have such status!
   * \details Certificates in the chain (container vector) must be placed from the root CA certificate (zero index) to
   *          the target end-entity certificate (last used index of the vector). Verification is executed in same order.
   * \details If the root CA (zero index) has not been imported and set as root of trust Certificate::Status::kNoTrust
   *          is returned.
   * \details If the chain verification is failed then status of the first failed certificate is returned.
   * \details This method updates the Certificate::Status associated with the certificates in the chain.
   *          In case of verification failure this affects all certificates up to the point of failure.
   *          Note this only holds true if the given chain is correctly structured matching previously stated
   *          requirements. If the input vector does not conform to the structure, i.e. element zero is not a root CA
   *          and subsequent elements do not form a uninterrupted chain the status of each certificate in the chain is
   *          not modified.
   * \details Actual path verification is only performed iff the given chain matches the structural requirements.
   * \details Verification status of an empty chain is Certificate::Status::kInvalid.
   * \param[in] chain target certificate chain for verification.
   * \param[in] known_extension_oids Vector of known extension OID strings.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return Verification status of the provided certificate chain.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-Crypto-X509Provider_X509_CertificationPathValidation_CustomExtensions
   */
  auto VerifyCertChainExt(ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const& chain,
                          ara::core::Vector<ara::core::String> known_extension_oids,
                          ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::Certificate::Status override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Verify status of the provided attribute certificate by locally stored CA certificates and CRLs only.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \details OIDs given with \c known_extension_oids parameters will not lead to validation failure if matching
   *          extensions are encountered and marked critical.
   * \param[in] cert target certificate for verification.
   * \param[in] known_extension_oids Vector of known extension OID strings.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate. Will never return a Status::kUnknown.
   * \context ANY
   * \pre -s
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificateValidation_CustomExtensions
   */
  auto VerifyAttributeCertExt(ara::crypto::x509::AttributeCertificate& cert,
                              ara::core::Vector<ara::core::String> known_extension_oids,
                              ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::AttributeCertificate::Status override;

 private:
  /*!
   * \brief Revoke recursively all the certificates signed by the given certificate.
   * \param[in] certificate Issuer certificate
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RevokeChildCertificates(ara::crypto::x509::Certificate const& certificate) const noexcept;

  /*!
   * \brief Verify the signature of the given OCSP response
   * \param[in] ocsp_response OCSP response to be verified
   * \return true if signature is valid and false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault If an error occurs during signature verification
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> VerifyOcspResponse(ara::crypto::x509::OcspResponse const& ocsp_response) const noexcept;

  /*!
   * \brief Find the issuer of the given certificate
   * \param[in] cert Certificate
   * \return The issuer certificate if found
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kNoError if No Certificate was found.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::Certificate::Uptr> FindIssuerCert(
      ara::crypto::x509::Certificate const& cert) const noexcept;

  // VECTOR NC VectorC++-V8.4.8: MD_CRYPTO_VectorC++-V8.4.8_AUTOSAR
  /*!
   * \brief Internal function for parsing certificate chains.
   * \param[out] outcome Parsed certificate chain.
   * \param[in] cert_chain Certificate chain.
   * \param[in] format_id Format of the certificate chain.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The outcome argument must not be accessed by another at the same time.
   *                  The cert_chain argument must not be accessed by another at the same time.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If (format_id ==
   * ara::crypto::Serializable::kFormatRawValueOnly).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate chain.
   * \reentrant TRUE If a separate outcome vector is used for each call.
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ParseCertChainInternal(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                                 ara::crypto::ReadOnlyMemRegion cert_chain,
                                                 ara::crypto::Serializable::FormatId format_id) const noexcept;

  /*!
   * \brief Internal function to convert encapsulated text portion of PEM certificate to DER encoded data.
   * \param[in] cert PEM certificate
   * \return The encapsulated text portion as DER encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to convert the data.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static ara::core::Result<ara::core::Vector<std::uint8_t>> ConvertPEMCertificateToDER(
      ara::crypto::ReadOnlyMemRegion cert);

  /*!
   * \brief Internal function to remove any non-base64 characters (except padding) from character sequence.
   * \param[in] encapsulated_text Character sequence with non-base64 characters and base64 characters.
   * \return The remaining base64 characters including padding.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto RemoveNonBase64CharactersExceptPadding(ara::core::String encapsulated_text) -> ara::core::String;

  /*!
   * \brief Internal function to get size of text without padding (1 or 2 '=' at the end).
   * \param[in] text Character sequence with optional padding of 1 or 2 '=' at the end.
   * \return The size of the text without padding.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto GetSizeWithoutPadding(ara::core::StringView text) -> std::size_t;

  /*!
   * \brief Internal function to remove any '=' in text up to limit size of text.
   * \param[in] text Character sequence with optional '='.
   * \param[in] limit Equal signs are removed up to limit size of text.
   * \return The text without any '=' except after limit.
   * \context ANY
   * \pre limit <= text.size()
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto RemoveEqualSignExceptPadding(ara::core::StringView text, std::size_t limit) -> ara::core::String;

  ara::crypto::cryp::CryptoProvider::Sptr crypto_provider_; /*!< Crypto provider associated with this X509 provider. */
  std::unique_ptr<CertificateStore> certificate_store_;     /*!< Used by this X509 provider to store certificates */
  std::unique_ptr<CsrStore> csr_store_;                     /*!< Used by this X509 provider to store CSRs */
  std::unique_ptr<PersistencyProvider>
      persistency_provider_;            /*!< Used by this X509 provider to persist certificates. */
  crypto::common::util::Logger logger_; /*!< Logging instance */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(CT__x509provider__x509__X509Provider, CheckCertStatus__Status__Good__Name);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509Provider, CheckCertStatus__Status__Good__KeyID);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509Provider, CheckCertStatus__Status__Revoked);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509ProviderDeathTest, VerifyOcspResponse_Wrong_Subtype);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509Provider, RemoveNonBase64CharactersExceptPadding);
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_PROVIDER_H_
