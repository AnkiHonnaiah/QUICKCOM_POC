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
/*!        \file  proxy/cryp/authn_stream_cipher_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::AuthnStreamCipherCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::AuthnStreamCipherCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_AUTHN_STREAM_CIPHER_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_AUTHN_STREAM_CIPHER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/cryp/authn_stream_cipher_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::AuthnStreamCipherCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class AuthnStreamCipherCtx : public BaseProxy, public ara::crypto::cryp::AuthnStreamCipherCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<AuthnStreamCipherCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  AuthnStreamCipherCtx(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
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
  static std::unique_ptr<AuthnStreamCipherCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<AuthnStreamCipherCtx, ara::crypto::CustomDeleter>(new AuthnStreamCipherCtx(
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
   * \brief Gets maximal supported size of associated public data.
   * \return Maximal supported size of associated public data in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetMaxAssociatedDataSize() const noexcept override;

  /*!
   * \brief Get the output digest size.
   * \return Size of the full output from this digest-function in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetDigestSize() const noexcept override;

  /*!
   * \brief Initialize the context for a new data stream processing.
   * \details To use the context for a new operation, the Reset() method then the SetKey() method must be called.
   * \param[in] iv an Initialization Vector (IV) or "nonce" value
   * \context ANY
   * \pre Key is set using the SetKey() method
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called Start() in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Key is not set.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The given IV or the key does not have a supported size.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to start.
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
   * \brief Update the digest calculation context by a new part of the message.
   * \details This method is dedicated for cases where the KeyMaterial is a part of the message.
   * \param[in] in A key material that should be processed.
   * \context ANY
   * \pre -
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
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to update.
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
   * \error ara::crypto::SecurityErrc::kInvalidInputSize Input size invalid.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to update.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Update(ara::crypto::Byte in) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
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
   * \pre Digest processing finished using the FinishBytes() method.
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
   * \pre Digest processing finished using the FinishBytes() method.
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
   * \brief Checks if the crypto context requires initialization by a key value.
   * \return true if the crypto context requires initialization by a key value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsKeyedContext() const noexcept override;

  /*!
   * \brief Check if the crypto context is already initialized and ready to use. It checks all required values,
   * including: domain parameters, key value, IV/seed, etc.
   * \return true if the crypto context is completely initialized and ready to use, and false otherwise
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
   * \param[out] parameters_uid  optional pointer to the output buffer for the target  COUID saving
   * \return \c true if the context has assigned domain parameters object and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetParametersUid(ara::crypto::CryptoObjectUid* parameters_uid = nullptr) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Reset the context.
   * \param[in] params The domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If (params != nullptr), the context doesn't
                                                           support domain parameters
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Reset(ara::crypto::cryp::DomainParameters::Sptrc params) noexcept override;

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
   * \brief Gets vendor specific ID of the primitive.
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
   * \brief Gets the category of the primitive.
   * \return The category of the primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
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
   * \param[out] key_id Optional pointer to a buffer for saving an COUID of a key now loaded to the context.
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
   * \brief Returns default expected size of Initialization Vector (IV).
   * \return Default expected size of IV in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetIvSize() const noexcept override;

  /*!
   * \brief Returns block size of base algorithm.
   * \return Size of block in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetBlockSize() const noexcept override;

  /*!
   * \brief Checks the operation mode for the bytewise property.
   * \return true if the mode can process a message the byte by byte (without padding) or
   *         false if only the block by block (with padding).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsBytewiseMode() const noexcept override;

  /*!
   * \brief Checks if the seek operation is supported in the current mode.
   * \return true the seek operation is supported in the current mode or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsSeekableMode() const noexcept override;

  /*!
   * \brief Verifies validity of specific IV length.
   * \param[in] iv_size Length of the IV in bytes.
   * \return true if provided IV length is supported by the algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsValidIvSize(std::size_t iv_size) const noexcept override;

  /*!
   * \brief Set the position of the next byte within the stream of the encryption/decryption gamma.
   * \param[in] offset  the offset value in bytes, relative to begin or current position in the gamma stream
   * \param[in] from_begin  the starting point for positioning within the stream: from begin (if \c true) or from
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Seek(std::int64_t offset, bool from_begin) noexcept override;

  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \details It is a copy-optimized method that doesn't use the internal cache buffer! It can be used only before
   *       processing of any non-aligned to the block-size boundary data.
   *       Pointers to the input and output buffers must be aligned to the block-size boundary!
   * \details The input and output buffers may completely coincide, but they must not partially intersect!
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Unable to process blocks while having data cached.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The provided input is too large or not aligned to the block
   * size.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Process failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ProcessBlocks(ara::crypto::WritableMemRegion out,
                                        ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[in,out] in_out  an input and output data buffer, i.e. the whole buffer should be updated
   * \note It is a copy-optimized method that doesn't use internal cache buffer! It can be used up to first non-block
   *       aligned data processing.
   *       <b>Pointer to the input-output buffer must be aligned to the block-size boundary!</b>
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Unable to process blocks while having data cached.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The provided input is too large or not aligned to the block
   * size. \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Process failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ProcessBlocks(ara::crypto::ReadWriteMemRegion in_out) noexcept override;

  /*!
   * \brief Processe a non-final part of message (that is not aligned to the block-size boundary).
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \return Actual size of output data (i.e. the number of leading bytes updated in the output buffer)
   * \note The input and output buffers must not intersect!
   * \note This method is "copy inefficient", therefore it should be used only in conditions when an application
   *       cannot control the chunking of the original message!
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The provided input is too large.
   * \error ara::crypto::SecurityErrc::kInOutBuffersIntersect The provided input and output buffers intersect.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Process failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ProcessBytes(ara::crypto::WritableMemRegion out,
                                              ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Processes the final part of message (that may be not aligned to the block-size boundary).
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \return actual size of output data (i.e. the number of leading bytes that were updated in the output buffer)
   * \note The input and output buffers must not intersect!
   * \note Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
   * \note This method may be used for processing of a whole message in a single call!
   * \context ANY
   * \pre Key is set using the SetKey() method and digest is initialized using the Start() method.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted Called in wrong state.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize The provided input is too large.
   * \error ara::crypto::SecurityErrc::kInOutBuffersIntersect The provided input and output buffers intersect.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Process failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> FinishBytes(ara::crypto::WritableMemRegion out,
                                             ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Set (deploy) a key to the context.
   * \param[in] key  the source key object
   * \param[in] direct_transform  the "direction" indicator: deploy the key for direct transformation (if \c true)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Called in wrong state.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If provided key objectis incompatible.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize Given key has an invalid length.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifierUnable to derive algorithm name.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetKey(ara::crypto::cryp::SymmetricKey const& key, bool direct_transform) noexcept override;

  /*!
   * \brief Gets configured "direction" of the transformation: direct (e.g. encryption) or reverse (e.g. decryption).
   * \return true if the context configured for a direct transformation and false if for a reverse one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsDirectTransform() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get actual bit-length of an IV loaded to the context.
   * \param[out] iv_uid  optional pointer to a buffer for saving an   COUID of a IV object now loaded to the context
   * \return actual length of the IV (now set to the algorithm context) in bits
   * \note If any IV was not set to the context yet then 0 is returned.
   * \note If the context was initialized by a \c SecretSeed object then the output buffer \c *iv_uid must be filled
   *       by COUID of this loaded IV object, in other cases \c *iv_uid must be filled by all zeros.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualIvBitLength(ara::crypto::CryptoObjectUid* iv_uid = nullptr) const noexcept override;

  /*!
   * \brief Count number of bytes now kept in the context cache.
   * \return number of bytes now kept in the context cache
   * \note In block-wise modes if an application has supplied input data chunks with incomplete last block
   *        then the context saves the rest part of the last (incomplete) block to internal "cache" memory
   *        and wait a next call for additional input to complete this block.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t CountBytesInCache() const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
};
}  // namespace cryp
}  // namespace proxy
}  // namespace client

}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_AUTHN_STREAM_CIPHER_CTX_H_
