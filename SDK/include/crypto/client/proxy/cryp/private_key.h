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
/*!        \file  proxy/cryp/private_key.h
 *        \brief  Proxy implementation for ara::crypto::cryp::PrivateKey.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::PrivateKey interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_PRIVATE_KEY_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_PRIVATE_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/private_key.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::PrivateKey
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class PrivateKey : public BaseProxy, public ara::crypto::cryp::PrivateKey {
 public:
  /*!
   * \brief Unique constant smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptrc = std::unique_ptr<PrivateKey const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  PrivateKey(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
             amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
             amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~PrivateKey() noexcept override = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  PrivateKey(PrivateKey const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  PrivateKey(PrivateKey&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  PrivateKey& operator=(PrivateKey const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  PrivateKey& operator=(PrivateKey&& /*other*/) & noexcept = delete;

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
  static std::unique_ptr<PrivateKey, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<PrivateKey, ara::crypto::CustomDeleter>(
        // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
        new PrivateKey(provider_reference, transporter, message_builder, message_serializer, message_deserializer));
  }

  /*!
   *
   * \brief Get the public key correspondent to this private key.
   * \param[in] params  an optional pointer to domain parameters required for full specification of the
   *            transformation
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the public key correspondent to this private key
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Public key is not loaded.
   * \error ara::crypto::SecurityErrc::kUnsupported The key's algorithm ID is not supported.
   * \error ara::crypto::SecurityErrc::kInvalidArgument Trying to create ECC Key from incompatible or empty container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> GetPublicKey(
      ara::crypto::cryp::DomainParameters::Sptrc params,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) const noexcept override;

  /*!
   * \brief Get allowed usages of this object.
   * \return A combination of bit-flags that specifies allowed applications of the object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::RestrictedUseObject::Usage GetAllowedUsage() const noexcept override;

  /*!
   * \brief Check compatibility of this key with an algorithm specified by an ID.
   * \param[in] alg_id Target Algorithm ID.
   * \return true if the key is compatible with the algorithm specified by the alg_id and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsCompatible(AlgId alg_id) const noexcept override;

  /*!
   * \brief Check compatibility of this key with a crypto transformation configured in the keyed context.
   * \param[in] context Target keyed context.
   * \return true if the key is compatible with the transformation of the context and false otherwise.
   * \details This method compares not only the Crypto Primitive IDs, but also the \a COUID of domain parameters
   *       objects associated with the key object and with provided context! If the COUIDs differ then this
   *       method returns \c false.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsCompatible(ara::crypto::cryp::KeyedContext const& context) const noexcept override;

  /*!
   * \brief Get actual type of this key object.
   * \return Identifier of the top-level interface type of the key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::KeyType GetKeyType() const noexcept override;

  /*!
   * \brief Get publicity of the key.
   * \return \c true if the key is public and false if the key is private or secret.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsPublic() const noexcept override;

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
   * \brief Get actual bit-length.
   * \return Actual length the key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualKeyBitLength() const noexcept override;

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
   * \error ara::crypto::SecurityErrc::kInvalidArgument The provided trusted container is invalid or not compatible.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Save(ara::crypto::TrustedContainer& container,
                               ara::crypto::TrustedContainer* referenced = nullptr) const noexcept override;

  /*!
   * \brief Gets a unified name of the primitive.
   * \details The crypto primitive name can be fully or partially specified.
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
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept override;

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

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable ara::core::String primitive_name_;              /*!< Name of the primitive. */
};

}  //  namespace cryp
}  //  namespace proxy
}  //  namespace client
}  //  namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_PRIVATE_KEY_H_
