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
/*!        \file  proxy/cryp/key_agreement_private_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::KeyAgreementPrivateCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::KeyAgreementPrivateCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/key_agreement_private_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::KeyAgreementPrivateCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class KeyAgreementPrivateCtx : public BaseProxy, public ara::crypto::cryp::KeyAgreementPrivateCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<KeyAgreementPrivateCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  KeyAgreementPrivateCtx(ara::crypto::cryp::CryptoProvider& provider_reference,
                         amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
                         amsr::sec::ipc::MessageSerializer& message_serializer,
                         amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept;

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
  static auto Create(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
                     amsr::sec::ipc::MessageBuilder& message_builder,
                     amsr::sec::ipc::MessageSerializer& message_serializer,
                     amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept
      -> std::unique_ptr<KeyAgreementPrivateCtx, ara::crypto::CustomDeleter> {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<KeyAgreementPrivateCtx, ara::crypto::CustomDeleter>(new KeyAgreementPrivateCtx(
        provider_reference, transporter, message_builder, message_serializer, message_deserializer));
  }

  /*!
   * \brief Produce a common secret seed via execution of the key-agreement algorithm between this private key
   *           and a public key of another side.
   * \details Produced \c SecretSeed object has following attributes: session, non-exportable, AlgID (this Key-Agreement
   *       Algorithm ID).
   * \param[in] other_side_key  the public key of the other side of the Key-Agreement
   * \param[in] allowed_usage  the allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique pointer to \c SecretSeed object, which contains the key material produced by the Key-Agreement
   *          algorithm
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to different
   *                algorithms or reference to different domain parameters
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not
   *                allocated
   * \error SecurityErrorDomain::kInsufficientResource  if capacity of slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AgreeSeed(ara::crypto::cryp::PublicKey const& other_side_key, ara::crypto::AllowedUsageFlags allowed_usage,
                 ara::crypto::cryp::ReservedObjectIndex reserved_index) const noexcept
      -> ara::core::Result<ara::crypto::cryp::SecretSeed::Uptrc> override;

  /*!
   * \brief Produce a common symmetric key via execution of the key-agreement algorithm between this private key
   *           and a public key of another side.
   * \details Produced \c SymmetricKey object has following attributes: session, exportable.
   * \details This method can be used for direct production of the target key, without creation of the intermediate
   *       \c SecretSeed object.
   * \param[in] other_side_key  the public key of the other side of the Key-Agreement
   * \param[in] kdf  the Context of a Key Derivation Function, which should be used for the target key production
   * \param[in] target_alg_id  identifier of the target symmetric algorithm (also defines a target key-length)
   * \param[in] allowed_usage  the allowed usage scope of the target key
   * \param[in] salt  an optional salt value (if used, it should be unique for each instance of the target key)
   * \param[in] ctx_label  an optional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties)
   * \param[in] params  an optional pointer to Domain Parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return a unique pointer to \c SecretSeed object, which contains the key material produced by the Key-Agreement
   *          algorithm
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to different
   *                algorithms or reference to different domain parameters
   *                or if (params != nullptr), but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object is in an incompleted state
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not
   *                allocated
   * \error SecurityErrorDomain::kInsufficientResource  if capacity of slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AgreeKey(ara::crypto::cryp::PublicKey const& other_side_key, ara::crypto::cryp::KeyDerivationFunctionCtx& kdf,
                AlgId target_alg_id, ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage,
                ara::crypto::ReadOnlyMemRegion salt, ara::crypto::ReadOnlyMemRegion ctx_label,
                ara::crypto::cryp::DomainParameters::Sptrc params,
                ara::crypto::cryp::ReservedObjectIndex reserved_index) const noexcept
      -> ara::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> override;

  /*!
   * \brief Set (load) a key to the algorithm context.
   * \param[in] key  a reference to a source key
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if provided key object is incompatible with
   *                an algorithm assigned to this private key context
   * \error SecurityErrorDomain::kBadObjectReference  if provided private key object references to
   *                an instance of the domain parameters different from the one loaded to the context,
   *                i.e. if the COUID of the domain parameters in the context (see \c GetParametersUid()) is
   *                not equal to the COUID referenced from the private key object
   * \error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
   *                is prohibited by the "allowed usage" restrictions of provided key object
   *                restrictions of provided key object.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetKey(ara::crypto::cryp::PrivateKey const& key) noexcept -> ara::core::Result<void> override;

  /*!
   * \brief Verify support of specific key length by the context.
   * \param[in] key_bit_length Length of the key in bits.
   * \return true if provided value of the key length is supported by the context.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsKeyBitLengthSupported(std::size_t key_bit_length) const noexcept -> bool override;

  /*!
   * \brief Returns minimal supported key length in bits.
   * \return Minimal supported length of the key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetMinKeyBitLength() const noexcept -> std::size_t override;

  /*!
   * \brief Returns maximal supported key length in bits.
   * \return Maximal supported length of the key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetMaxKeyBitLength() const noexcept -> std::size_t override;

  /*!
   * \brief Get actual bit-length of a key loaded to the context.
   * \details If any key was not set to the context yet then 0 is returned.
   * \param[out] key_id Optional pointer to a buffer for saving an COUID of a key now loaded to the context.
   * \return Actual length of a key (now set to the algorithm context) in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetActualKeyBitLength(ara::crypto::CryptoObjectUid* key_id) const noexcept -> std::size_t override;

  /*!
   * \brief Checks if the crypto context requires initialization by a key value.
   * \return true if the crypto context requires initialization by a key value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsKeyedContext() const noexcept -> bool override;

  /*!
   * \brief Check if the crypto context is already initialized and ready to use. It checks all required values,
   * including: domain parameters, key value, IV/seed, etc.
   * \return true if the crypto context is completely initialized and ready to use, and false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsInitialized() const noexcept -> bool override;

  /*!
   * \brief Clear the crypto context and set the domain parameters to it.
   * \param[in] params  an optional shared pointer to a domain parameters object, which should be set to this
   *  crypto context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if provided domain parameters object is incompatible
   *                with an algorithm assigned to this cryptographic context
   * \error SecurityErrorDomain::kIncompleteArgState  if provided domain parameters object has incomplete state
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Reset(ara::crypto::cryp::DomainParameters::Sptrc params) noexcept -> ara::core::Result<void> override;

  /*!
   * \brief Get the COUID of a domain parameters object assigned to the context.
   * \param[out] parameters_uid  optional pointer to the output buffer for the target COUID saving
   * \return \c true if the context has assigned domain parameters object and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetParametersUid(ara::crypto::CryptoObjectUid* parameters_uid) const noexcept -> bool override;

  /*!
   * \brief Gets a unified name of the primitive.
   * \details The crypto primitive name can be fully or partially specified.
   * \return The unified name of the crypto primitive.
   * \vprivate Component Private
   */
  auto GetPrimitiveName() const noexcept -> ara::core::StringView const override;

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
  auto GetPrimitiveId() const noexcept -> AlgId override;

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
  auto GetCategory() const noexcept -> Category override;

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
  auto MyProvider() const noexcept -> ara::crypto::cryp::CryptoProvider& override;

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
  mutable ara::core::String primitive_name_;              /*!< Primitive name */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_
