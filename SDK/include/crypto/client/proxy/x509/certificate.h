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
/*!        \file  proxy/x509/certificate.h
 *        \brief  Proxy implementation for ara::crypto::x509::Certificate.
 *
 *      \details  Client side implementation of the ara::crypto::x509::Certificate interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_CERTIFICATE_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/crypto/x509/certificate.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Client-side implementation for the proxy of ara::crypto::x509::Certificate.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class Certificate final : public BaseProxy, public ara::crypto::x509::Certificate {
 public:
  /*!
   * \brief Constructor
   * \param[in] provider_reference Reference to the creating crypto provider
   * \param[in] transporter Transporter used for server/client communication
   * \param[in] message_builder Builder for message generation
   * \param[in] message_serializer Serializer for request serialization
   * \param[in] message_deserializer Deserializer for response deserialization
   * \vprivate Component Private
   */
  explicit Certificate(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
                       amsr::sec::ipc::MessageBuilder& message_builder,
                       amsr::sec::ipc::MessageSerializer& message_serializer,
                       amsr::sec::ipc::MessageDeserializer& message_deserializer)
      : BaseProxy{transporter, message_builder, message_serializer, message_deserializer},
        ara::crypto::x509::Certificate(),
        provider_reference_{provider_reference},
        pub_key_info_{nullptr},
        subject_dn_{nullptr},
        issuer_dn_{nullptr},
        logger_{common::util::kClientLoggingContext, ""_sv} {}

  /*!
   * \brief Create function for instance generation without access to new/delete operator (which are protected)
   * \param[in] provider_reference Reference to the creating crypto provider
   * \param[in] transporter Transporter used for server/client communication
   * \param[in] message_builder Builder for message generation
   * \param[in] message_serializer Serializer for request serialization
   * \param[in] message_deserializer Deserializer for response deserialization
   * \return Unique pointer to the newly created object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static std::unique_ptr<Certificate, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~Certificate() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  Certificate(Certificate const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  Certificate(Certificate&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  Certificate& operator=(Certificate const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  Certificate& operator=(Certificate&& /*other*/) = delete;

  /*!
   * \brief Call the destructor and release the memory.
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
  void Release() noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat The format is not supported.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer is too small to hold the encoded certificate.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept final;

  /*!
   * \brief Load the subject public key information object to realm of specified crypto provider.
   * \details If (crypto_provider == nullptr) then X509PublicKeyInfo object
              will be loaded in realm of the Stack-default Crypto Provider
   * \param[in] crypto_provider shared pointer of a target Crypto Provider, where the public key will be used.
   * \return constant reference of the subject public key interface
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to create key info.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::X509PublicKeyInfo const& SubjectPubKey(
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept final;

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
  ara::crypto::x509::X509DN const& SubjectDn() const noexcept final;

  /*!
   * \brief Find out whether this is a CA request.
   * \return true if it is a CA request and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsCa() const noexcept final;

  /*!
   * \brief Get the constraint on the path length defined in the Basic Constraints extension.
   * \return certification path length limit.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint32_t GetPathLimit() const noexcept final;

  /*!
   * \brief Get the key constraints for the key associated with this PKCS#10 object.
   * \return key constraints.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeyConstraints GetConstraints() const noexcept final;

  /*!
   * \brief Get the X.509 version of this certificate object.
   * \return X.509 version
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint32_t X509Version() const noexcept final;

  /*!
   * \brief Check whether this certificate belongs to a root CA.
   * \return true if this certificate belongs to a root CA (i.e. the certificate is self-signed)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsRoot() const noexcept final;

  /*!
   * \brief Get the issuer certificate DN.
   * \return Issuer DN of this certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::x509::X509DN const& IssuerDn() const final;

  /*!
   * \brief Get the "Not Before" of the certificate.
   * \return "Not Before" of the certificate
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::time_t StartTime() const noexcept final;

  /*!
   * \brief Get the "Not After" of the certificate.
   * \return "Not After" of the certificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::time_t EndTime() const noexcept final;

  /*!
   * \brief Get the serial number of this certificate.
   * \details If (sn.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] sn an optional output buffer for storing the serial number.
   * \return size of the certificate serial number in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The buffer is too small to store the serila number.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to get the serial number.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> SerialNumber(ara::crypto::WritableMemRegion sn) const noexcept final;

  /*!
   * \brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
   * \details If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] id the optional output buffer.
   * \return size of the DER encoded AuthorityKeyIdentifier in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to get the AuthorityKeyIdentifier.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The buffer is too small to store the
   * AuthorityKeyIdentifier.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> AuthorityKeyId(ara::crypto::WritableMemRegion id) const noexcept final;

  /*!
   * \brief Get the DER encoded SubjectKeyIdentifier of this certificate.
   * \details If (id.empty() == true) then this method only returns required size of the output buffer.
   * \param[out] id  the optional output buffer.
   * \return size of the DER encoded SubjectKeyIdentifier in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to get the SubjectKeyIdentifier.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The buffer is too small to store the SubjectKeyIdentifier.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> SubjectKeyId(ara::crypto::WritableMemRegion id) const noexcept final;

  /*!
   * \brief Verify signature of the certificate.
   * \details Call with (ca_cert == nullptr) is applicable only if this is a certificate of a root CA.
   *          Note that this method ONLY verifies the signature and nothing else.
   * \param[in] ca_cert the optional pointer to a Certification Authority certificate
   *                   used for signature of the current one.
   * \return true if this certificate was verified successfully and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool VerifyMe(ara::crypto::x509::Certificate const* ca_cert) const noexcept final;

  /*!
   * \brief Calculate a fingerprint from the whole certificate.
   * \param[out] fingerprint output buffer for the fingerprint storage.
   * \param[in] hash_ctx an initialized hash function context.
   * \return number of bytes actually saved to the output buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to calculate the fingerprint.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetFingerprint(ara::crypto::ReadWriteMemRegion fingerprint,
                                                ara::crypto::cryp::HashFunctionCtx& hash_ctx) const noexcept final;

  /*!
   * \brief Return last verification status of the certificate.
   * \return the certificate verification status.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Status GetStatus() const noexcept final;

  /*!
   * \brief Return last verification status of the certificate.
   * \return the certificate verification status.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSignatureAlgorithm() const noexcept -> ara::crypto::CryptoAlgId final;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */

  mutable std::unique_ptr<ara::crypto::cryp::X509PublicKeyInfo, ara::crypto::CustomDeleter>
      pub_key_info_; /*!< Proxy for the public key info member of \c ara::crypto::x509::Certificate. */
  mutable std::unique_ptr<ara::crypto::x509::X509DN, ara::crypto::CustomDeleter>
      subject_dn_; /*!< Proxy for the distinguished name of the \c ara::crypto::x509::Certificate subject. */
  mutable std::unique_ptr<ara::crypto::x509::X509DN, ara::crypto::CustomDeleter>
      issuer_dn_; /*!< Proxy for the distinguished name of the \c ara::crypto::x509::Certificate issuer. */
  common::util::Logger logger_; /*!< Logging instance. */
};

}  // namespace x509
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_CERTIFICATE_H_
