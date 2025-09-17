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
/*!        \file  ara/crypto/x509/x509_provider.h
 *        \brief  X.509 Provider interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_PROVIDER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>

#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/provider_info.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "ara/crypto/cryp/x509_cert_request.h"
#include "ara/crypto/cryp/x509_public_key_info.h"
#include "ara/crypto/x509/attribute_certificate.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/ocsp_request.h"
#include "ara/crypto/x509/ocsp_response.h"
#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/x509_extensions.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief X.509 Provider interface.
 * \details The X.509 Provider supports two internal storages: volatile (or session) and persistent.
 * \details All X.509 objects created by the provider should have an actual reference to their parent X.509 Provider.
 * \details The X.509 Provider can be destroyed only after destroying of all its daughterly objects.
 * \details Each method of this interface that creates a X.509 object is non-constant,
 *          because any such creation increases a references counter of the X.509 Provider.
 * \details Any user of this interface should create shared pointers to it only by calls of
 *          the method shared_from_this()!
 * \vpublic
 */
class X509Provider : public std::enable_shared_from_this<X509Provider>, public ara::crypto::ProviderInfo {
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<X509Provider>;

  /*!
   * \brief Type of an internal index inside the certificate storage.
   * \vpublic
   */
  using StorageIndex = std::size_t;

  /*!
   * \brief Reserved "invalid index" value for navigation inside the certificate storage.
   * \vpublic
   */
  static StorageIndex const kInvalidIndex{std::numeric_limits<StorageIndex>::max()};

  /*!
   * \brief A short alias for Algorithm ID type definition.
   * \vpublic
   */
  using AlgId = ara::crypto::cryp::CryptoPrimitiveId::AlgId;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Create an empty X.500 Distinguished Name (DN) structure.
   * \param[in] capacity  number of bytes that should be reserved for the content of the target X509DN object.
   * \return Unique smart pointer to created.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadAlloc if the requested object can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<X509DN::Uptr> CreateEmptyDn(std::size_t capacity) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual ara::core::Result<X509DN::Uptrc> BuildDn(ara::core::StringView dn) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Decode X.500 Distinguished Name structure from the provided serialized format.
   * \param[in] dn DER/PEM-encoded representation of the Distinguished Name.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer for the created X509DN object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the dn argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value-
   * \error SecurityErrorDomain::kBadAlloc if the requested object can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<X509DN::Uptrc> DecodeDn(
      ara::crypto::ReadOnlyMemRegion dn,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Parse a serialized representation of the certificate and create its instance.
   * \details Off-line validation of the parsed certificate may be done via call VerifyCertByCrl().
   * \details After validation the certificate may be imported to the session or persistent storage
   *          for following search and usage.
   * \details If the parsed certificate is not imported then it will be lost after destroy of the returned instance!
   * \details Only imported certificate may be found by a search and applied for automatic verifications!
   * \param[in] cert DER/PEM-encoded certificate.
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return unique smart pointer to created certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the cert argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \error SecurityErrorDomain::kUnsupported if the format_id argument is not supported.
   * \error SecurityErrorDomain::kBadAlloc if the certificate can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<Certificate::Uptr> ParseCert(
      ara::crypto::ReadOnlyMemRegion cert,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
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
   * \vpublic
   */
  virtual auto ParseAttributeCert(
      ara::crypto::ReadOnlyMemRegion attribute_cert,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<AttributeCertificate::Uptr> = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
   * \param[in] cert_chain DER/PEM-encoded certificate chain (in form of a single BLOB).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \return number of certificates in the chain.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the cert_chain argument cannot be pre-parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> CountCertsInChain(
      ara::crypto::ReadOnlyMemRegion cert_chain,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \details Off-line validation of the parsed certification chain may be done via call VerifyCertChainByCrl().
   * \details After validation the certificates may be saved to the session or persistent storage
   *          for following search and usage.
   * \details If the certificates are not imported then they will be lost after destroy of the returned instances!
   * \details Only imported certificates may be found by a search and applied for automatic verifications!
   * \details Certificates in the outcome vector will be placed from the root CA certificate (zero index) to
   *          the final end-entity certificate (last used index of the vector).
   * \param[out] outcome an output vector for imported certificates.
   * \param[in] cert_chain DER/PEM-encoded certificate chain (in form of a single BLOB).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if the capacity of outcome vector is less
   *                   than actual number of certificates in the chain.
   * \error SecurityErrorDomain::kInvalidArgument if the cert_chain argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \error SecurityErrorDomain::kBadAlloc if the certificate can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ParseCertChain(
      ara::core::Vector<Certificate::Uptr>& outcome, ara::crypto::ReadOnlyMemRegion cert_chain,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Parse a serialized representation of the certificate chain and create their instances.
   * \details Off-line validation of the imported certification chain may be done via call VerifyCertChainByCrl().
   * \details After validation the certificates may be imported to the session or persistent storage for
   *          following search and usage.
   * \details Capacity of the outcome vector must be equal to the size of the cert_chain vector.
   * \details If the certificates are not imported then they will be lost after destroy of the returned instances!
   * \details Only imported certificates may be found by a search and applied for automatic verifications!
   * \details Certificates in the outcome vector will be placed from the root CA certificate (zero index) to
   *          the final end-entity certificate (last used index of the vector).
   * \param[out] outcome output vector of imported certificates.
   * \param[in] cert_chain DER/PEM-encoded certificates chain (each certificate is presented
   *                      by a separate BLOB in the input vector).
   * \param[in] format_id input format identifier (kFormatDefault means auto-detect).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if capacity of the outcome vector is less
   *                   than number of elements in the cert_chain.
   * \error SecurityErrorDomain::kInvalidArgument if an element of cert_chain argument cannot be parsed.
   * \error SecurityErrorDomain::kUnknownIdentifier if the format_id argument has unknown value.
   * \error SecurityErrorDomain::kBadAlloc  if the certificate can not be allocated dynamically.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ParseCertChain(
      ara::core::Vector<Certificate::Uptr>& outcome,
      ara::core::Vector<ara::crypto::ReadOnlyMemRegion> const& cert_chain,
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \param[in] cert target certificate for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Certificate::Status VerifyCertByCrl(Certificate& cert,
                                              ara::core::Optional<time_t> reference_time_point = ara::core::nullopt)
      const noexcept = 0;  // Attention! Deviation: const, optional reference_time_point parameter

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \return verification status of the provided certificate chain.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Certificate::Status VerifyCertChainByCrl(
      ara::core::Vector<Certificate::Uptr> const& chain,
      ara::core::Optional<time_t> reference_time_point =
          ara::core::nullopt) const noexcept = 0;  // Attention! Deviation: optional reference_time_point parameter

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Verify status of the provided attribute certificate by locally stored CA certificates.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \param[in] attribute_cert target certificate for verification.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided attribute certificate. Will never return a Status::kUnknown
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto VerifyAttributeCert(AttributeCertificate& attribute_cert,
                                   ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> AttributeCertificate::Status = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual ara::core::Result<bool> ImportCrl(ara::crypto::ReadOnlyMemRegion crl) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_AutosarC++17_10-V8.3.1_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \param[in] label certificate label.
   * \param[in] to_volatile if this flag true then certificate should be saved to the volatile (session) storage,
   *                       otherwise to the persistent storage.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid.
   * \error SecurityErrorDomain::kIncompatibleObject if provided certificate has partial collision with
   *                   a matched CSR in the storage.
   * \error SecurityErrorDomain::kContentDuplication if the provided certificate already exists
   *                   in the storage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Import(Certificate const& cert, bool to_volatile = true,
                                         amsr::core::StringView label = amsr::core::StringView{}) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual ara::core::Result<Certificate::Uptr> LoadCertificate(amsr::core::StringView label) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
   * \param[in] cert a unique smart pointer to a certificate that should be removed.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool Remove(Certificate::Uptrc&& cert) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Remove certificate with specified label from the storage (volatile or persistent) and destroy it.
   * \param[in] label the certificate's label.
   * \return true if the certificate was found and removed from the storage, false if it was not found.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool RemoveCertificate(amsr::core::StringView label) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Save a prepared certificate signing request (CSR) to volatile or persistent storage.
   * \details Successfully saved request is located in special area of the storage dedicated for pending requests!
   * \details Saved CSR obtains status "new" that can be changed to "pending" status when a responsible application
   *          send it to correspondent CA (see SetPendingStatus() method).
   * \details A CSR may be saved only if a trusted certificate from a CA specified by authority_dn exists in the
   * storage. This CA certificate may be trusted as a "root of trust" or via a validated "chain of trust".
   * \param[in] request a valid certificate signature request (that should be send to CA).
   * \param[in] authority_dn target certification authority (CA) to which the request is addressed.
   * \param[in] to_volatile if this flag true then the request should be saved to the volatile (session)
   * storage, otherwise to the persistent storage.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certification request is invalid.
   * \error SecurityErrorDomain::kUnknownIdentifier if the provided authority DN is unknown in the system (CA is
   * "registered" in the system only if its certificate is trusted).
   * \error SecurityErrorDomain::kContentDuplication if the provided CSR for same CA already exists
   * in the storage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SaveCertSignRequest(ara::crypto::cryp::X509CertRequest const& request,
                                                      X509DN const& authority_dn, bool to_volatile = true) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
   * \details This method do nothing if the CSR already marked as "pending".
   * \details Only an application with permissions "CA Connector" has the right to call this method!
   * \param[in] request  certificate signing request that should be marked as "pending".
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation if the method called by an application without the
   *                   "CA Connector" permission.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetPendingStatus(CertSignRequest const& request) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set specified CA certificate as a "root of trust".
   * \details Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
   * \details Only CA certificate can be a "root of trust"!
   * \details Multiple certificates on an ECU may be marked as the "root of trust".
   * \details Only an application with permissions "Trust Master" has the right to call this method!
   * \param[in] ca_cert a valid CA certificate that should be trusted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid or has not been imported yet.
   * \error SecurityErrorDomain::kIncompatibleObject if provided certificate is not a CA certificate.
   * \error SecurityErrorDomain::kAccessViolation if the method called by an application without
   *                  the "Trust Master" permission.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetAsRootOfTrust(Certificate const& ca_cert) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Create OCSP request for specified certificate.
   * \details This method may be used for implementation of the "OCSP stapling".
   * \param[in] cert a certificate that should be verified.
   * \param[in] signer an optional pointer to initialized signer context (if the request should be signed).
   * \return unique smart pointer to the created OCSP request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid.
   * \error SecurityErrorDomain::kIncompleteArgState if the signer context is not initialized by a key.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
      Certificate const& cert, ara::crypto::cryp::SignerPrivateCtx const* signer = nullptr) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Create OCSP request for specified list of certificates.
   * \details This method may be used for implementation of the "OCSP stapling".
   * \param[in] cert_list a certificates' list that should be verified.
   * \param[in] signer an optional pointer to initialized signer context (if the request should be signed).
   * \return unique smart pointer to the created OCSP request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificates are invalid.
   * \error SecurityErrorDomain::kIncompleteArgState if the signer context is not initialized by a key.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
      ara::core::Vector<Certificate const*> const& cert_list,
      ara::crypto::cryp::SignerPrivateCtx const* signer = nullptr) noexcept = 0;

  // Attention! Deviation: The method returns ara::core::Result<OcspResponse::Uptrc> instead of OcspResponse::Uptrc
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
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
   * \vpublic
   */
  virtual ara::core::Result<OcspResponse::Uptrc> ParseOcspResponse(
      ara::crypto::ReadOnlyMemRegion response) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
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
   * \vpublic
   */
  virtual ara::core::Result<bool> CheckCertStatus(Certificate& cert,
                                                  OcspResponse const& ocsp_response) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Check status of a certificates list by directly provided OCSP response.
   *  This method may be used for implementation of the "OCSP stapling".
   * \details This method updates the Certificate::Status associated with the certificates in the list.
   * \param[in] cert_list a certificates list that should be verified.
   * \param[in] ocsp_response an OCSP response.
   * \return true if the certificates list is verified successfully and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument if the provided certificates are invalid.
   * \error SecurityErrorDomain::kRuntimeFault if the ocsp_response is invalid.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> CheckCertStatus(ara::core::Vector<Certificate*> const& cert_list,
                                                  OcspResponse const& ocsp_response) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual Certificate::Uptrc FindCertByDn(X509DN const& subject_dn, X509DN const& issuer_dn, time_t validity_time_point,
                                          StorageIndex& cert_index) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual Certificate::Uptrc FindCertByKeyIds(
      ara::crypto::ReadOnlyMemRegion subject_key_id,
      ara::crypto::ReadOnlyMemRegion authority_key_id = ara::crypto::ReadOnlyMemRegion()) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual Certificate::Uptrc FindCertBySn(ara::crypto::ReadOnlyMemRegion sn, X509DN const& issuer_dn) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual CertSignRequest::Uptrc FindCertSignRequest(StorageIndex& request_index, X509DN const* authority_dn = nullptr,
                                                     X509DN const* subject_dn = nullptr,
                                                     bool pending_csr = false) noexcept = 0;
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Cleanup the volatile certificates storage.
   * \details After execution of this command the certificates previously imported to the volatile storage
   *          cannot be found by a search, but it doesn't influence to already loaded Certificate instances!
   * \details This method intentionally does nothing since it can cause unintended side effects on other applications.
   *          To remove volatile certificates, use RemoveCertificate on each individual certificate explicitly.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void CleanupVolatileStorage() noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
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
   * \vpublic
   */
  virtual ara::core::Result<void> FindCertByDnWildcard(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& outcome,
                                                       ara::crypto::x509::X509DN const& subject_dn,
                                                       ara::crypto::x509::X509DN const& issuer_dn,
                                                       time_t validity_time_point) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \vprivate Product Private
   */
  virtual auto VerifyCertExt(Certificate& cert, ara::core::Vector<ara::core::String> known_extension_oids,
                             ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> Certificate::Status = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
  virtual auto VerifyCertChainExt(ara::core::Vector<Certificate::Uptr> const& chain,
                                  ara::core::Vector<ara::core::String> known_extension_oids,
                                  ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> Certificate::Status = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Verify status of the provided attribute certificate by locally stored CA certificates and CRLs only.
   * \details This method updates the Certificate::Status associated with the certificate.
   * \details OIDs given with \c known_extension_oids parameter will not lead to validation failure if matching
   *          extensions are encountered and marked critical.
   * \details Since the attribute certificate validation according to RFC5755 requires the issuer & holder certificates
   *          to be validated as well, the \c known_extension_oids parameter given with this function needs to contain
   *          the respective OIDs for the certificates as well if required!
   * \param[in] cert target certificate for verification.
   * \param[in] known_extension_oids Vector of known extension OID strings.
   * \param[in] reference_time_point (optional) a time point to be used as reference for validity verification.
   * \return verification status of the provided certificate. Will never return a Status::kUnknown
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto VerifyAttributeCertExt(
      AttributeCertificate& cert, ara::core::Vector<ara::core::String> known_extension_oids,
      ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> AttributeCertificate::Status = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_PROVIDER_H_
