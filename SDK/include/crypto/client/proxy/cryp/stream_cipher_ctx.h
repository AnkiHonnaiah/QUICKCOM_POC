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
/*!        \file  proxy/cryp/stream_cipher_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::StreamCipherCtx
 *      \details  Client side implementation of the ara::crypto::cryp::StreamCipherCtx interface that transparently
 *                communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_STREAM_CIPHER_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_STREAM_CIPHER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::StreamCipherCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class StreamCipherCtx : public BaseProxy, public ara::crypto::cryp::StreamCipherCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<StreamCipherCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  StreamCipherCtx(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
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
  static std::unique_ptr<StreamCipherCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    return std::unique_ptr<StreamCipherCtx, ara::crypto::CustomDeleter>(new StreamCipherCtx(
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
   * \brief Initialize the context for a new data stream processing or generation (depending from the primitive).
   * \param[in] iv an optional Initialization Vector (IV) or "nonce" value
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted  The Function called in invalid state.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder The Symmetric Key is not set.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize Either the given IV or
   *        the key does not have a supported size.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error occurred during AES-128-CBC operation.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Start(ara::crypto::ReadOnlyMemRegion iv) noexcept override;

  /*!
   * \brief Initialize the context for a new data stream processing or generation (depending from the primitive).
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
   * \brief Checks if the crypto context requires initialization by a key value.
   * \return \c true if the crypto context requires initialization by a key value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsKeyedContext() const noexcept override;

  /*!
   *  \brief Check if the crypto context is already initialized and ready to use. It checks all required values,
   * including: domain parameters, key value, IV/seed, etc.
   * \return \c true iff current state is kInitialized.
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
   * \param[out] parameters_uid  optional pointer to the output buffer for the target COUID saving
   * \return \c true if the context has assigned domain parameters object and \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetParametersUid(ara::crypto::CryptoObjectUid* parameters_uid = nullptr) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Clear the crypto context and set the domain parameters to it.
   * \param[in] params  an optional shared pointer to a domain parameters object, which should be set to this
   *  crypto context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Reset(ara::crypto::cryp::DomainParameters::Sptrc params = nullptr) noexcept override;

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
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetActualKeyBitLength(ara::crypto::CryptoObjectUid* key_id) const noexcept override;

  /*!
   * \brief Get default expected size of the Initialization Vector (IV) or nonce.
   * \return default expected size of IV in bytes
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetIvSize() const noexcept override;

  /*!
   * \brief Get block (or internal buffer) size of the base algorithm.
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
   * \brief Checks the operation mode for the bytewise property.
   * \return \c true if the mode can process a message byte by byte (without padding) or
   *         false if only block by block (with padding).
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
   * \return \c true the seek operation is supported in the current mode or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsSeekableMode() const noexcept override;

  /*!
   * \brief Verify validity of specific Initialization Vector (IV) length.
   * \param[in] iv_size the length of the IV in bytes
   * \return \c true if provided IV length is supported by the algorithm and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsValidIvSize(std::size_t iv_size) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Set the position of the next byte within the stream of the encryption/decryption gamma.
   * \param[in] offset  the offset value in bytes, relative to begin or current position in the gamma stream
   * \param[in] from_begin  the starting point for positioning within the stream: from begin (if \c true) or from
   *            current position (if \c false)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Seek(std::int64_t offset, bool from_begin = true) noexcept override;

  // Attention! Deviation: This implementation is not copy-optimized and uses the internal cache buffer.
  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \details It is a copy-optimized method that doesn't use the internal cache buffer! It can be used only before
   *       processing of any non-aligned to the block-size boundary data.
   *       Pointers to the input and output buffers must be aligned to the block-size boundary!
   * \details The input and output buffers may completely coincide, but they must not partially intersect!
   * \details FinishBytes has to be called after ProcessBlocks finished (last 16 bytes of generated output are only
   * written to output buffer by FinishBytes).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by the block size (see
   * \c GetBlockSize())
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   * the \c Start() method
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted  Method called before starting the data processing.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error occurred during AES-128-CBC operation.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ProcessBlocks(ara::crypto::WritableMemRegion out,
                                        ara::crypto::ReadOnlyMemRegion in) noexcept override;

  // Attention! Deviation: This implementation is not copy-optimized and uses the internal cache buffer.
  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[in,out] in_out  an input and output data buffer, i.e. the whole buffer should be updated
   * \details It is a copy-optimized method that doesn't use internal cache buffer! It can be used up to first non-block
   *       aligned data processing.
   *       Pointer to the input-output buffer must be aligned to the block-size boundary!
   * \details FinishBytes has to be called after ProcessBlocks finished (last 16 bytes of generated output are only
   * written to output buffer by FinishBytes).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by the block size (see
   * \c GetBlockSize())
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   * the \c Start() method
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted  Method called before starting the data processing.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error occurred during AES-128-CBC operation.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ProcessBlocks(ara::crypto::ReadWriteMemRegion in_out) noexcept override;

  /*!
   * \brief Processe a non-final part of message (that is not aligned to the block-size boundary).
   * \param[out] out An output data buffer
   * \param[in] in An input data buffer
   * \return actual size of output data (i.e. the number of leading bytes updated in the output buffer)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ProcessBytes(ara::crypto::WritableMemRegion out,
                                              ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Processes the final part of message.
   * \param[out] out An output data buffer
   * \param[in] in An input data buffer
   * \return actual size of output data (i.e. the number of leading bytes that were updated in the output buffer)
   * \details The input and output buffers must not intersect!
   * \details Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
   * \details This method may be used for processing of a whole message in a single call (in any mode)!
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
   *                the block size (see \c GetBlockSize())
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   *                the \c Start() method
   * \error ara::crypto::SecurityErrc::kProcessingNotStarted  Method called before starting the data processing.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided output buffer is not large enough.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Error occurred during AES-128-CBC operation.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> FinishBytes(ara::crypto::WritableMemRegion out,
                                             ara::crypto::ReadOnlyMemRegion in) noexcept override;

  /*!
   * \brief Set (deploy) a key to the symmetric algorithm context.
   * \param[in] key  the source key object
   * \param[in] direct_transform  the "direction" indicator: deploy the key for direct transformation (if \c true)
   *            or for reverse one (if \c false)
   * \details If domain parameters are principally required for a crypto algorithm assigned to this context then they
   *        must be set before the loading of a key value (via call of the \c CryptoContext::Reset())!
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Function called in invalid state.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject  The provided key object is invalid.
   * \error ara::crypto::SecurityErrc::kInvalidInputSize    Given key has an invalid length.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier  Unable to derive algorithm name.
   * \error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
   *                (taking into account the direction specified by \c direct_transform) is prohibited by
   *                the "allowed usage" restrictions of provided key object
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

  /*!
   * \brief Count number of bytes now kept in the context cache.
   * \return number of bytes now kept in the context cache
   * \note In block-wise modes if an application has supplied input data chunks with incomplete last block
   *        then the context saves the rest part of the last (incomplete) block to internal "cache" memory
   *        and wait a next call for additional input to complete this block.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  size_t CountBytesInCache() const noexcept override;

  /*!
   * \brief Get actual bit-length of an IV loaded to the context.
   * \param[out] iv_uid optional pointer to a buffer for saving an COUID of a IV object now loaded to the context
   * \return If context was initialized actual length of the IV in bits, otherwise 0.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  size_t GetActualIvBitLength(ara::crypto::CryptoObjectUid* iv_uid) const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
};
}  // namespace cryp
}  // namespace proxy
}  // namespace client

}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_STREAM_CIPHER_CTX_H_
