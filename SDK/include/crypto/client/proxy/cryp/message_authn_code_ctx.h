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
/*!        \file  proxy/cryp/message_authn_code_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::MessageAuthnCodeCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::MessageAuthnCodeCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_MESSAGE_AUTHN_CODE_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_MESSAGE_AUTHN_CODE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::MessageAuthnCodeCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class MessageAuthnCodeCtx : public BaseProxy, public ara::crypto::cryp::MessageAuthnCodeCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<MessageAuthnCodeCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Constructor.
   * \details Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   *
   * \pre -
   * \vprivate Component Private
   */
  MessageAuthnCodeCtx(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
                      amsr::sec::ipc::MessageBuilder& message_builder,
                      amsr::sec::ipc::MessageSerializer& message_serializer,
                      amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Constructor.
   * \details Create function for instance generation without access to new/delete operator (which are protected)
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
  static std::unique_ptr<MessageAuthnCodeCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    return std::unique_ptr<MessageAuthnCodeCtx, ara::crypto::CustomDeleter>(new MessageAuthnCodeCtx(
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
   * \brief Get the output digest size.
   * \return Size of the full output from the digest function in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetDigestSize() const noexcept override;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \details This method is dedicated for cases where the KeyMaterial is a part of the message.
   * \param[in] in A key material that should be processed.
   * \context ANY
   * \pre Digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Update(ara::crypto::cryp::KeyMaterial const& in) noexcept override;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \param[in] in A part of input message that should be processed.
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called Updated in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize Input size invalid.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error during mac update.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Update(ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \param[in] in A part of input message that should be processed.
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called Updated in wrong state.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error during mac update.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Update(ara::crypto::Byte in) noexcept override;

  /*!
   * \brief Start, Update and finish the digest calculation context by a message in one shot.
   * \details Only after call of this method the digest can be signed, verified, extracted or compared!
   * \details This method is convenient for processing of constant tags.
   * \param[in] in The input message that should be processed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by required domain parameters
   *        or key object
   * \error SecurityErrorDomain::kUnsupported if the base algorithm (or its current implementation) principally doesn't
   *        support an IV-less variation
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  ara::core::Result<void> StartUpdateFinish(ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Finish the digest calculation and optionally produce the "signature" object.
   * \param[in] make_signature_object If this argument is true then
   *                                  the method will also produce the signature object.
   * \param[in] reserved_index Index of reserved Object slot that should be used for
   *                           this allocation or default marker.
   * \return Unique smart pointer to created signature object, if (make_signature_object == true) or
   *         nullptr if (make_signature_object == false).
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported If (make_signature_object == true)
                                                    Creation of signature is not supported by mac contexts
   * \error ara::crypto::SecurityErrc::kBadAlloc If (reserved_index != ara::crypto::cryp::kAllocObjectOnHeap)
                                                 Only allocation on heap is currently supported.
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called Finish() in wrong state.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error in finalizing the mac.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::Signature::Uptrc> Finish(
      bool make_signature_object, ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

  /*!
   * \brief Finish the digest calculation.
   * \context ANY
   * \pre Digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called Finish() in wrong state.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error in finalizing the hash.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Finish() noexcept override;

  /*!
   * \brief Get requested part of calculated digest to existing memory buffer.
   * \param[out] output An output buffer for storing the requested digest fragment (or fully).
   * \param[in] offset Position of the first byte of digest that should be placed to the output buffer.
   * \return Number of digest bytes really stored to the output buffer.
   * \context ANY
   * \pre Digest processing finished using the Finish() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotFinished Digest processing not finished.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetDigest(ara::crypto::WritableMemRegion output,
                                           std::size_t offset) const noexcept override;

  /*!
   * \brief Compare the calculated digest against an expected value.
   * \param[in] expected A memory region containing an expected digest value.
   * \param[in] offset Position of first byte in calculated digest for the comparison starting.
   * \return true if the expected bytes sequence is identical to first bytes of calculated digest.
   * \context ANY
   * \pre Digest processing finished using the Finish() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotFinished Digest processing not finished.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> Compare(ara::crypto::ReadOnlyMemRegion expected, std::size_t offset) const noexcept override;

  /*!
   * \brief Check the calculated digest against an expected "signature" object.
   * \param[in] expected The signature object containing an expected digest value.
   * \return true if value and meta-information of the provided "signature" object is identical to calculated digest
   *         and current configuration of the context respectively; but false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> Check(ara::crypto::cryp::Signature const& expected) const noexcept override;

  /*!
   * \brief Check if the context is keyed.
   * \return true If the context is keyed and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsKeyedContext() const noexcept override;

  /*!
   * \brief Check if the crypto context is already initialized and ready to use.
   * \return true If the context is initialized and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsInitialized() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the COUID of a domain parameters object assigned to the context.
   * \param[out] parameters_uid  optional pointer to the output buffer for the target  COUID saving.
   * \return true If the context has assigned domain parameters object and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetParametersUid(ara::crypto::CryptoObjectUid* parameters_uid = nullptr) const noexcept override;

  /*!
   * \brief Reset the mac context.
   * \param[in] params The domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If (params != nullptr), the mac context doesn't
                                                           support domain parameters
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Reset(ara::crypto::cryp::DomainParameters::Sptrc params) noexcept override;

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

  /*!
   * \brief Get the crypto provider of the mac context.
   * \return A reference to the crypto provider of the mac context.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept override;

  /*!
   * \brief Verify support of specific key length by the context.
   * \param[in] key_bit_length Length of the key in bits.
   * \return true if provided value of the key length is supported by the context.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsKeyBitLengthSupported(std::size_t key_bit_length) const noexcept override;

  /*!
   * \brief Returns minimal supported key length in bits.
   * \return Minimal supported length of the key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetMinKeyBitLength() const noexcept override;

  /*!
   * \brief Returns maximal supported key length in bits.
   * \return Maximal supported length of the key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetMaxKeyBitLength() const noexcept override;

  /*!
   * \brief Get actual bit-length of a key loaded to the context.
   * \details If any key was not set to the context yet then 0 is returned.
   * \param[out] key_id Pointer to a buffer for saving an COUID of a key now loaded to the context.
   * \return Actual length of a key (now set to the algorithm context) in bits.
   * \context ANY
   * \pre Key is set using the SetKey() method.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualKeyBitLength(ara::crypto::CryptoObjectUid* key_id) const noexcept override;

  /*!
   * \brief Set (deploy) a key to the context.
   * \param[in] key  the source key object
   * \param[in] direct_transform  the "direction" indicator: deploy the key for direct transformation (if \c true)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the context is not in the initial state
                                                          e.g. Update() or Finish() is called.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If provided key objectis incompatible.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetKey(ara::crypto::cryp::SymmetricKey const& key, bool direct_transform) noexcept override;

  /*!
   * \brief Gets configured "direction" of the transformation.
   * \return true if the context configured for a direct transformation and false if for a reverse one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsDirectTransform() const noexcept override;

  /*!
   * \brief Get default expected size of the Initialization Vector (IV) or nonce.
   * \return 0 IV not supported by mac contexts
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetIvSize() const noexcept override;

  /*!
   * \brief Get the block size of the base algorithm.
   * \return size of the block in bytes
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetBlockSize() const noexcept override;

  /*!
   * \brief Verify validity of specific Initialization Vector (IV) length.
   * \param[in] iv_size the length of the IV in bytes.
   * \return true if provided IV length is supported by the algorithm and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsValidIvSize(std::size_t iv_size) const noexcept override;

  /*!
   * \brief Initialize the context for a new data stream processing.
   * \details To use the mac context for a new operation, the Reset() method then the SetKey() method must be called.
   * \param[in] iv an Initialization Vector (IV) or "nonce" value
   * \context ANY
   * \pre Call the Reset() method then the SetKey() method if the context has been already used.
   *      e.g. Update() or Finish() is called.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported If iv is not empty
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Called Start in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The provided key is too large.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to start the mac context.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Start(ara::crypto::ReadOnlyMemRegion iv) noexcept override;

  /*!
   * \brief Initialize the context for a new data stream processing.
   * \param[in] iv the Initialization Vector (IV) or "nonce" object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Start(ara::crypto::cryp::SecretSeed const& iv) noexcept override;

  /*!
   * \brief Get actual bit-length of an IV loaded to the context.
   * \param[out] iv_uid pointer to a buffer for saving an COUID of a IV object now loaded to the context.
   * \return actual length of the IV in bits
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualIvBitLength(ara::crypto::CryptoObjectUid* iv_uid) const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_MESSAGE_AUTHN_CODE_CTX_H_
