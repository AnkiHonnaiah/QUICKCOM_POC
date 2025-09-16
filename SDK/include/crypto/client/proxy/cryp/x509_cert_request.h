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
/*!        \file  proxy/cryp/x509_cert_request.h
 *        \brief  Proxy implementation for ara::crypto::cryp::X509CertRequest.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::X509CertRequest interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_CERT_REQUEST_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_CERT_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/x509_cert_request.h"
#include "crypto/client/base_proxy.h"
#include "x509_public_key_info.h"
#include "x509_signature.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::X509CertRequest
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class X509CertRequest : public BaseProxy, public ara::crypto::cryp::X509CertRequest {
 public:
  /*!
   * \brief Unique constant pointer to the proxy.
   * \vprivate Component Private
   */
  using Uptrc = std::unique_ptr<X509CertRequest const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  X509CertRequest(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
                  amsr::sec::ipc::MessageBuilder& message_builder,
                  amsr::sec::ipc::MessageSerializer& message_serializer,
                  amsr::sec::ipc::MessageDeserializer& message_deserializer);

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
  static std::unique_ptr<X509CertRequest, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<X509CertRequest, ara::crypto::CustomDeleter>(new X509CertRequest(
        provider_reference, transporter, message_builder, message_serializer, message_deserializer));
  }

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
  void Release() noexcept override { delete this; }

  /*!
   * \brief Get the object's type.
   * \return Returns the type of this object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Type GetObjectType() const noexcept override;
  /*!
   * \brief Returns the "session" (or "temporary") attribute of the object.
   * \details A temporary object cannot be saved to a non-volatile trusted container!
   * \details A temporary object will be securely destroyed together with this interface instance!
   * \details A non-session object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is temporary (i.e. its life time is limited by the current session only).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsSession() const noexcept override;
  /*!
   * \brief Returns the "exportable" attribute of the object.
   * \details An exportable object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is exportable (i.e. if it can be exported
   *         outside the trusted environment of the Crypto Provider).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsExportable() const noexcept override;
  /*!
   * \brief Returns the object's UID if it is assigned to the object.
   * \details An object that has no an assigned COUID cannot be (securely)
   *          serialized / exported or saved to a non-volatile container.
   * \details An object should not have a COUID if it is session and non-exportable
   *          simultaneously or if it is incomplete yet (last is applicable for domain parameters only).
   * \details A few related objects of different types can share a single COUID (e.g. private and public keys),
   *          but a combination of COUID and object type must be unique always!
   * \param[out] object_id Optional pointer to a buffer for the object's UID saving.
   * \return true if the object has assigned COUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetObjectId(ara::crypto::CryptoObjectUid* object_id) const noexcept override;
  /*!
   * \brief Returns an identifier of an object from which depends the current one.
   * \details If (object_id != nullptr) but the current object has no
   *          dependence from other one then the object_id should be filled by zeros.
   * \param[out] object_id Optional pointer to a buffer for the target object UID.
   * \return Target object type for existing dependence or Type::UNKNOWN
   *         if the current object doesn't depend from any other one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Type HasDependence(ara::crypto::CryptoObjectUid* object_id) const noexcept override;
  /*!
   * \brief Returns a storage size of the object.
   * \return Size in bytes of the objects required for its storage.
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t StorageSize() const noexcept override;
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Saves itself to provided trusted container.
   * \param[in] container A target trusted container.
   * \param[in] referenced A pointer to another trusted container that keeps referenced crypto object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Save(ara::crypto::TrustedContainer& container,
                               ara::crypto::TrustedContainer* referenced = nullptr) const noexcept override;

  /*!
   * \brief Get a unified name of the primitive.
   * \return The unified name of the crypto primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::StringView const GetPrimitiveName() const noexcept override;
  /*!
   * \brief Get the primitive ID.
   * \return The binary Crypto Primitive ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId GetPrimitiveId() const noexcept override;
  /*!
   * \brief Get the category of the crypto primitive.
   * \return The category of the crypto primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Category GetCategory() const noexcept override;
  // VECTOR Next Construct AutosarC++17_10-M9.3.1: MD_CRYPTO_AutosarC++17_10-M9.3.1
  /*!
   * \brief Get the crypto provider of the context.
   * \return A reference to the crypto provider of the context.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept override { return provider_reference_; }

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
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Format ID not supported.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer is too small to hold the CertRequest.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id =
          ara::crypto::Serializable::kFormatDefault) const noexcept override;

  /*!
   * \brief Verify self-signed signature of the certificate request.
   * \param[in] hash  a temporary hash-function context that should be used in the call (the Alg ID can be got as
   *            \c X509Signature::GetPrimitiveId())
   * \param[in] verifier  a temporary signature verification context that should be used in the call (the Alg ID can
   *            be got as \c X509Signature::GetRequiredHashAlgId())
   * \return \c true if the signature is correct and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> Verify(ara::crypto::cryp::HashFunctionCtx& hash,
                                 ara::crypto::cryp::VerifierPublicCtx& verifier) const noexcept override;

  /*!
   * \brief Return format version of the certificate request.
   * \return Format version of the certificate request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint32_t Version() const noexcept override;
  /*!
   * \brief Return signature object of the request.
   * \return Signature object of the request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::X509Signature const& Signature() const noexcept override;
  /*!
   * \brief Return subject public key included to the request.
   * \return Subject public key included to the request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::X509PublicKeyInfo const& SubjectPublicKeyInfo() const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable std::unique_ptr<crypto::client::proxy::cryp::X509Signature, ara::crypto::CustomDeleter>
      signature_; /*!< Member proxy that needs to be kept to be able to return a const reference */
  mutable std::unique_ptr<crypto::client::proxy::cryp::X509PublicKeyInfo, ara::crypto::CustomDeleter>
      pub_key_info_; /*!< Member proxy that needs to be kept to be able to return a const reference */
  mutable ara::core::String primitive_name_; /*!< Primitive name */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_CERT_REQUEST_H_
