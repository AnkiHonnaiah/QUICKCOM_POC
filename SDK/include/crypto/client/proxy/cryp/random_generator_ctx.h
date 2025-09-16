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
/*!        \file  proxy/cryp/random_generator_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::RandomGeneratorCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::RandomGeneratorCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_RANDOM_GENERATOR_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_RANDOM_GENERATOR_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::RandomGeneratorCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class RandomGeneratorCtx : public crypto::client::BaseProxy, public ara::crypto::cryp::RandomGeneratorCtx {
 public:
  /*!
   * \brief Shared smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Sptr = std::shared_ptr<RandomGeneratorCtx>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  RandomGeneratorCtx(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
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
  static std::unique_ptr<RandomGeneratorCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<RandomGeneratorCtx, ara::crypto::CustomDeleter>(new RandomGeneratorCtx(
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
  void Release() noexcept override { delete this; };

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
  bool GetParametersUid(ara::crypto::CryptoObjectUid* parameters_uid) const noexcept override;
  /*!
   * \brief Reset the mac context.
   * \param[in] params The domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kIncompleteArgState Domain parameters not set or don't have a seed.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject Unable to initialize the RNG.
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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetMaxKeyBitLength() const noexcept override;
  /*!
   * \brief Get actual bit-length
   * \param[out] key_id Pointer to a buffer for saving an COUID of a key now loaded to the context.
   * \return Actual length of a key (now set to the algorithm context) in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualKeyBitLength(ara::crypto::CryptoObjectUid* key_id) const noexcept override;
  /*!
   * \brief Update internal state of the RNG by additional entropy.
   * \param[in] entropy A memory region with the additional entropy value.
   * \return true if the method is supported.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool AddEntropy(ara::crypto::ReadOnlyMemRegion entropy) noexcept override;
  /*!
   * \brief Fill whole provided buffer by generated random sequence.
   * \param[out] output A target buffer for filling by the generated random sequence.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUninitializedContext Context not initialized.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Generation failed.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Generate(ara::crypto::WritableMemRegion output) noexcept override;

  /*!
   * \brief Set (deploy) a key to the context.
   * \param[in] key  the source key object
   * \param[in] direct_transform  the "direction" indicator: deploy the key for direct transformation (if \c true)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
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
   * \param[in] iv an Initialization Vector (IV) or "nonce" value
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
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

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_RANDOM_GENERATOR_CTX_H_
