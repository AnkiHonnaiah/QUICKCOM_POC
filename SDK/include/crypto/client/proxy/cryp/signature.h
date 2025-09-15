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
/*!        \file  proxy/cryp/signature.h
 *        \brief  Proxy implementation for ara::crypto::cryp::Signature.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::Signature interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_SIGNATURE_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_SIGNATURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/signature.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::Signature
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class Signature : public crypto::client::BaseProxy, public ara::crypto::cryp::Signature {
 public:
  /*!
   * \brief Unique constant pointer to the proxy.
   * \vprivate Component Private
   */
  using Uptrc = std::unique_ptr<Signature const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  Signature(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
            amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
            amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Create function for instance generation without access to new/delete operator (which are protected)
   * \param[in] provider_reference Reference to the creating crypto provider
   * \param[in] transporter Transporter used for server/client communication
   * \param[in] message_builder Builder for message generation
   * \param[in] message_serializer Serializer for request serialization
   * \param[in] message_deserializer Deserializer for response deserialization
   * \return Unique pointer to the newly created object
   * \vprivate Component Private
   */
  static std::unique_ptr<Signature, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<Signature, ara::crypto::CustomDeleter>(
        // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
        new Signature(provider_reference, transporter, message_builder, message_serializer, message_deserializer));
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
   * \brief Returns the type of this object.
   * \return One of object types except Type::UNKNOWN.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
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
   * \reentrant FALSE
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
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsExportable() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetObjectId(ara::crypto::CryptoObjectUid* object_id) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns an identifier of an object from which depends the current one.
   * \details If (object_id != nullptr) but the current object has no
   *          dependence from other one then the object_id should be filled by zeros.
   * \details For signatures objects this method must return a reference to correspondent
   *          signature verification public key!
   * \details For keys objects this method should return a reference to domain parameters.
   * \details For domain parameters object this method (optionally) can return a reference
   *          to another domain parameters required for this one.
   * \param[out] object_id Optional pointer to a buffer for the target object UID.
   * \return Target object type for existing dependence or Type::UNKNOWN
   *         if the current object doesn't depend from any other one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Type HasDependence(ara::crypto::CryptoObjectUid* object_id) const noexcept override;

  /*!
   * \brief Returns a storage size of the object.
   * \return Size in bytes of the objects required for its storage.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t StorageSize() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Saves itself to provided trusted container.
   * \details An object can be saved to a container only if the container is empty and has enough capacity.
   * \details The save operation is allowed to an empty container only, i.e. any updates of objects are prohibited!
   * \details Only a non-session/non-temporary and completed object (i.e. that have a COUID) can be saved!
   * \details Only a single instance of an object is allowed in the persistent storage! Any object is uniquely
   *          identified by the combination of its COUID and type.
   * \param[in] container A target trusted container.
   * \param[in] referenced A pointer to another trusted container that keeps referenced crypto object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation  if the target or the referenced non-volatile containers
   *         were opened "for usage", i.e. if (!container.HasOwnership() || !referenced->HasOwnership())
   * \error SecurityErrorDomain::kIncompatibleObject  if the object is "session", but the container
   * \error SecurityErrorDomain::kContentRestrictions  if the object doesn't satisfy the slot restrictions \c
   *         keys::KeySlotPrototypeProps)
   * \error SecurityErrorDomain::kInsufficientCapacity  if the capacity of the target container is not enough, i.e. if
   *         (container.Capacity() < this->StorageSize())
   * \error SecurityErrorDomain::kBadObjectReference
   *   - if (referenced != nullptr), but this crypto object doesn't support referencing to other objects;
   *   - if (referenced != nullptr), but an object in referenced container cannot be referenced;
   *   - if (referenced != nullptr), but this crypto object and referenced one have incompatible for
   *     referencing types and/or algorithms;
   *   - if (referenced != nullptr) and \c referenced is volatile, but \c container is non-volatile
   * \error SecurityErrorDomain::kEmptyContainer
   *   - if (referenced != nullptr), but the referenced trusted container is empty
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Save(ara::crypto::TrustedContainer& container,
                               ara::crypto::TrustedContainer* referenced = nullptr) const noexcept override;

  /*!
   * \brief Gets a unified name of the primitive.
   * \details The crypto primitive name can be fully or partially specified.
   * \return The unified name of the crypto primitive.
   * \vprivate Component Private
   */
  ara::core::StringView const GetPrimitiveName() const noexcept override;

  /*!
   * \brief Gets vendor specific ID of the primitive.
   * \return The binary Crypto Primitive ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId GetPrimitiveId() const noexcept override;

  /*!
   * \brief Gets the category of the primitive.
   * \return The category of the primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Category GetCategory() const noexcept override;

  /*!
   * \brief Gets a reference to Crypto Provider of this primitive.
   * \return A reference to Crypto Provider instance that provides this primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept override;

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
   * \error SecurityErrorDomain::kInsufficientCapacity
   *         if (output.empty() == false), but it's capacity is less than required
   * \error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
   * \error SecurityErrorDomain::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id =
          ara::crypto::Serializable::kFormatDefault) const noexcept override;

  /*!
   * \brief Get an ID of hash algorithm used for this signature object production.
   * \return ID of used hash algorithm only (without signature algorithm specification).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId GetHashAlgId() const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable ara::core::String primitive_name_;              /*!< Primitive name */
  common::util::Logger logger_;                           /*!< Logging instance */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_SIGNATURE_H_
