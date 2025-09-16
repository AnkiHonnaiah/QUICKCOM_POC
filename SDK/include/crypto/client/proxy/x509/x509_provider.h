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
/*!        \file  proxy/x509/x509_provider.h
 *        \brief  Proxy implementation for ara::crypto::x509::X509Provider.
 *
 *      \details  Client side implementation of the ara::crypto::x509::X509Provider interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_PROVIDER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/x509_provider.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Client-side implementation for the proxy of ara::crypto::x509::X509Provider.
 * \vprivate Component Private
 */
class X509Provider final : public BaseProxy, public ara::crypto::x509::X509Provider {
 public:
  /*!
   * \brief Constructor
   * \param[in] crypto_provider CryptoProvider to be used in this X509 provider.
   * \param[in] transporter Transporter used for server/client communication
   * \vprivate Component Private
   */
  explicit X509Provider(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                        amsr::sec::ipc::Transporter& transporter)
      : BaseProxy{transporter, message_builder_, message_serializer_, message_deserializer_},
        ara::crypto::x509::X509Provider(),
        crypto_provider_{std::move(crypto_provider)},
        message_builder_{},
        message_serializer_{},
        message_deserializer_{} {}

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~X509Provider() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  X509Provider(X509Provider const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  X509Provider(X509Provider&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509Provider& operator=(X509Provider const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509Provider& operator=(X509Provider&& /*other*/) = delete;

  /*!
   * \brief Create an empty X.500 Distinguished Name (DN) structure.
   * \param[in] capacity  number of bytes that should be reserved for the content of the target X509DN object.
   * \return Unique smart pointer to created.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if the internally created name is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptr> CreateEmptyDn(std::size_t capacity) noexcept final;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Create completed X.500 Distinguished Name structure from the provided string representation.
   * \param[in] dn string representation of the Distinguished Name.
   * \return unique smart pointer for the created X509DN object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the dn argument has incorrect format.
   * \error SecurityErrorDomain::kBadAlloc if the requested object can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptrc> BuildDn(ara::core::StringView dn) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Decode X.500 Distinguished Name structure from the provided serialized format.
   * \param[in] dn DER representation of the Distinguished Name.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer for the created X509DN object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If the given format is not supported.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the given DN is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::X509DN::Uptrc> DecodeDn(
      ara::crypto::ReadOnlyMemRegion dn,
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Parse a serialized representation of the certificate and create its instance.
   * \param[in] cert DER encoded certificate.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer to created certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id != ara::crypto::Serializable::kFormatDefault).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::Certificate::Uptr> ParseCert(
      ara::crypto::ReadOnlyMemRegion cert,
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Parse a serialized representation of an attribute certificate and create its instance.
   * \details Off-line validation of the parsed attribute certificate may be done via call VerifyAttributeCert()
   * \param[in] attribute_cert DER/PEM-encoded attribute certificate.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer to created attribute certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the attriubte_cert argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \error SecurityErrorDomain::kUnsupported if the format_id argument is not supported.
   * \error SecurityErrorDomain::kBadAlloc if the certificate can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto ParseAttributeCert(ara::crypto::ReadOnlyMemRegion attribute_cert,
                          // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
                          ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault)
      const noexcept -> ara::core::Result<ara::crypto::x509::AttributeCertificate::Uptr> final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
   * \param[in] cert_chain DER encoded certificate chain (in form of a single BLOB).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return number of certificates in the chain
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id != ara::crypto::Serializable::kFormatDefault).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate chain.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> CountCertsInChain(
      ara::crypto::ReadOnlyMemRegion cert_chain,
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept final;

  // Attention! Deviation: A maximum of 20 certificates can be parsed per cert chain.
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \param[out] outcome an output vector for imported certificates.
   * \param[in] cert_chain DER encoded certificate chain (in form of a single BLOB).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \context ANY
   * \pre -
   * \threadsafe TRUE The outcome argument must not be accessed by another at the same time.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id != ara::crypto::Serializable::kFormatDefault).
   * \error ara::crypto::SecurityErrc::kInvalidArgument Failed to parse the certificate chain.
   * \reentrant TRUE If a separate outcome vector is used for each call.
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ParseCertChain(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome, ara::crypto::ReadOnlyMemRegion cert_chain,
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept final;

  // Attention! Deviation: A maximum of 20 certificates can be parsed per cert chain.
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \param[out] outcome output vector of imported certificates.
   * \param[in] cert_chain DER encoded certificates chain (each certificate is presented
   *                      by a separate BLOB in the input vector).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \context ANY
   * \pre -
   * \threadsafe TRUE The outcome vector must not be accessed by another at the same time
   *                  The cert_chain vector must not be accessed by another at the same time
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if (format_id != ara::crypto::Serializable::kFormatDefault)
   *                                                      or failed to parse one of the given certificates.
   * \reentrant TRUE If a separate outcome vector is used for each call.
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ParseCertChain(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
      ara::core::Vector<ara::crypto::ReadOnlyMemRegion> const& cert_chain,
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
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
   */
  ara::crypto::x509::Certificate::Status VerifyCertByCrl(
      ara::crypto::x509::Certificate& cert,
      ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
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
   * \details Actual path verification is only performed if the given chain matches the structural requirements.
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
      ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Verify status of the provided attribute certificate by locally stored CA certificates.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \param[in] attribute_cert target certificate for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided attribute certificate. Will never return a Status::kUnknown.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto VerifyAttributeCert(ara::crypto::x509::AttributeCertificate& attribute_cert,
                           ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::AttributeCertificate::Status final;

  /*!
   * \brief Import Certificate Revocation List (CRL) or Delta CRL from a file.
   * \param[in] crl serialized CRL or Delta CRL (in form of a BLOB).
   * \return true if the CRL is valid and false if it is already expired.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnexpectedValue if the provided BLOB is not a CRL/DeltaCRL.
   * \error SecurityErrorDomain::kRuntimeFault if the CRL validation has failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> ImportCrl(ara::crypto::ReadOnlyMemRegion crl) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
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
   *                       otherwise to the persistent storage.
   * \param[in] label certificate label.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if (cert.GetStatus() != Certificate::Status::kValid).
   * \error ara::crypto::SecurityErrc::kContentDuplication Certificate already exists.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to import the certificate.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Import(ara::crypto::x509::Certificate const& cert, bool to_volatile = true,
                                 amsr::core::StringView label = amsr::core::StringView{}) noexcept final;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Find a certificate by label.
   * \param[in] label The target certificate label.
   * \return An unique smart pointer to the instantiated certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource if the InstanceSpecifier is incorrect (the certificate cannot be
   *                   found)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::Certificate::Uptr> LoadCertificate(amsr::core::StringView label) noexcept final;

  /*!
   * \brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
   * \param[in] cert a unique smart pointer to a certificate that should be removed.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool Remove(ara::crypto::x509::Certificate::Uptrc&& cert) noexcept final;

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
   */
  bool RemoveCertificate(amsr::core::StringView label) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kContentDuplication CSR already exists.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier The certificate store contains no CA certificate issued by the
   * given authority.
   * \error ara::crypto::SecurityErrc::kInvalidArgument The given X509CertRequest is invalid.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed the save the X509CertRequest.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SaveCertSignRequest(ara::crypto::cryp::X509CertRequest const& request,
                                              ara::crypto::x509::X509DN const& authority_dn,
                                              bool to_volatile = true) noexcept final;

  /*!
   * \brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
   * \details This method does nothing if the CSR already marked as "pending".
   * \details Only an application with permissions "CA Connector" has the right to call this method!
   * \param[in] request  certificate signing request that should be marked as "pending".
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument CertSignRequest not found in the csr store.
   * \error ara::crypto::SecurityErrc::kAccessViolation Permission denied.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetPendingStatus(ara::crypto::x509::CertSignRequest const& request) noexcept final;

  /*!
   * \brief Set specified CA certificate as a "root of trust".
   * \details Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
   * \details Only CA certificate can be a "root of trust"!
   * \details Multiple certificates on an ECU may be marked as the "root of trust".
   * \details Only an application with permissions "Trust Master" has the right to call this method!
   * \param[in] ca_cert a valid CA certificate that should be trusted.
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if (cert.GetStatus() != Certificate::Status::kValid)
   * \error ara::crypto::SecurityErrc::kIncompatibleObject The given cetificate is not a CA certificate.
   * \error ara::crypto::SecurityErrc::kAccessViolation Permission denied.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetAsRootOfTrust(ara::crypto::x509::Certificate const& ca_cert) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create OCSP request for specified certificate.
   * \details This method may be used for implementation of the "OCSP stapling".
   * \param[in] cert a certificate that should be verified.
   * \param[in] signer an optional pointer to initialized signer context (if the request should be signed).
   * \return unique smart pointer to the created OCSP request.
   * \details Method not supported.
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspRequest::Uptrc> CreateOcspRequest(
      ara::crypto::x509::Certificate const& cert,
      ara::crypto::cryp::SignerPrivateCtx const* signer = nullptr) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create OCSP request for specified list of certificates.
   * \details Method not supported.
   * \details This method may be used for implementation of the "OCSP stapling".
   * \param[in] cert_list a certificates' list that should be verified.
   * \param[in] signer an optional pointer to initialized signer context (if the request should be signed).
   * \return unique smart pointer to the created OCSP request.
   * \details Method not supported.
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspRequest::Uptrc> CreateOcspRequest(
      ara::core::Vector<ara::crypto::x509::Certificate const*> const& cert_list,
      ara::crypto::cryp::SignerPrivateCtx const* signer = nullptr) noexcept final;

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
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::x509::OcspResponse::Uptrc> ParseOcspResponse(
      ara::crypto::ReadOnlyMemRegion response) const noexcept final;

  /*!
   * \brief Check certificate status by directly provided OCSP response.
    This method may be used for implementation of the "OCSP stapling".
   * \details This method updates the Certificate::Status associated with the certificate.
   *          If the given certificate is revoked, the method updates the status of the
   *          child certificates to kRevoked.
   * \param[in] cert a certificate that should be verified.
   * \param[in] ocsp_response  an OCSP response.
   * \return true if the certificates list is verified successfully and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the cert is invalid.
   * \error SecurityErrorDomain::kRuntimeFault if the ocsp_response is invalid.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> CheckCertStatus(ara::crypto::x509::Certificate& cert,
                                          ara::crypto::x509::OcspResponse const& ocsp_response) const noexcept final;

  /*!
   * \brief Check status of a certificates list by directly provided OCSP response.
   *  This method may be used for implementation of the "OCSP stapling".
   * \details Method not supported.
   * \details This method updates the Certificate::Status associated with the certificates in the list.
   * \param[in] cert_list a certificates list that should be verified.
   * \param[in] ocsp_response an OCSP response.
   * \return true if the certificates list is verified successfully and false otherwise.
   * \vprivate Component Private
   */
  ara::core::Result<bool> CheckCertStatus(ara::core::Vector<ara::crypto::x509::Certificate*> const& cert_list,
                                          ara::crypto::x509::OcspResponse const& ocsp_response) const noexcept final;

  /*!
   * \brief Find a certificate by the subject and issuer Distinguished Names (DN).
   * \details Argument cert_index represents an internal index of current certificate in the storage.
   * \details In order to start certificate search from begin: cert_index = kInvalidIndex
   * \param[in] subject_dn subject DN of the target certificate.
   * \param[in] issuer_dn issuer DN of the target certificate.
   * \param[in] validity_time_point a time point when the target certificate should be valid.
   * \param[in,out] cert_index an index for iteration through all suitable certificates in the storage
   *                          (input: index of previous found cerificate, output: index of current found cerificate).
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::Certificate::Uptrc FindCertByDn(
      ara::crypto::x509::X509DN const& subject_dn, ara::crypto::x509::X509DN const& issuer_dn,
      time_t validity_time_point, ara::crypto::x509::X509Provider::StorageIndex& cert_index) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Find a certificate by its SKID & AKID.
   * \param[in] subject_key_id subject key identifier (SKID).
   * \param[in] authority_key_id optional authority key identifier (AKID).
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::Certificate::Uptrc FindCertByKeyIds(
      ara::crypto::ReadOnlyMemRegion subject_key_id,
      ara::crypto::ReadOnlyMemRegion authority_key_id = ara::crypto::ReadOnlyMemRegion()) noexcept final;

  /*!
   * \brief Find a certificate by its serial number.
   * \param[in] sn serial number of the target certificate.
   * \param[in] issuer_dn authority DN.
   * \return unique smart pointer to a found certificate or nullptr if nothing is found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::Certificate::Uptrc FindCertBySn(ara::crypto::ReadOnlyMemRegion sn,
                                                     ara::crypto::x509::X509DN const& issuer_dn) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::CertSignRequest::Uptrc FindCertSignRequest(
      ara::crypto::x509::X509Provider::StorageIndex& request_index,
      ara::crypto::x509::X509DN const* authority_dn = nullptr, ara::crypto::x509::X509DN const* subject_dn = nullptr,
      bool pending_csr = false) noexcept final;

  /*!
   * \brief Cleanup the volatile certificates storage.
   * \details After execution of this command the certificates previously imported to the volatile storage
   *          cannot be found by a search, but it doesn't influence to already loaded Certificate instances!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void CleanupVolatileStorage() noexcept final;

  /*!
   * \brief Get the provider version.
   * \return The version of the provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetProviderVersion() const noexcept final;

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
  char const* GetProviderName() const noexcept final;

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
  ara::crypto::ProviderType GetProviderType() const noexcept final;

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
  void GetProviderUid(ara::crypto::Guid& provider_uid) const noexcept final;

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
   */
  ara::core::Result<void> FindCertByDnWildcard(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                               ara::crypto::x509::X509DN const& subject_dn,
                                               ara::crypto::x509::X509DN const& issuer_dn,
                                               time_t validity_time_point) noexcept final;

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
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto VerifyCertExt(ara::crypto::x509::Certificate& cert, ara::core::Vector<ara::core::String> known_extension_oids,
                     ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::Certificate::Status final;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
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
   */
  auto VerifyCertChainExt(ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const& chain,
                          ara::core::Vector<ara::core::String> known_extension_oids,
                          ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::Certificate::Status final;

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
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto VerifyAttributeCertExt(ara::crypto::x509::AttributeCertificate& cert,
                              ara::core::Vector<ara::core::String> known_extension_oids,
                              ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::AttributeCertificate::Status final;

 private:
  ara::crypto::cryp::CryptoProvider::Sptr crypto_provider_;  /*!< Crypto provider. */
  amsr::sec::ipc::MessageBuilder message_builder_;           /*!< Message builder for communication */
  amsr::sec::ipc::MessageSerializer message_serializer_;     /*!< Message serializer for communication */
  amsr::sec::ipc::MessageDeserializer message_deserializer_; /*!< Message deserializer for communication */
};

}  // namespace x509
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_PROVIDER_H_
