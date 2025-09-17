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
/*!        \file  proxy/cryp/key_derivation_function_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::KeyDerivationFunctionCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::KeyDerivationFunctionCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::KeyDerivationFunctionCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class KeyDerivationFunctionCtx : public BaseProxy, public ara::crypto::cryp::KeyDerivationFunctionCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<KeyDerivationFunctionCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  KeyDerivationFunctionCtx(ara::crypto::cryp::CryptoProvider& provider_reference,
                           amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
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
   * \return Unique pointer to the newly created object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static std::unique_ptr<KeyDerivationFunctionCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<KeyDerivationFunctionCtx, ara::crypto::CustomDeleter>(new KeyDerivationFunctionCtx(
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
   * \brief Reset the context.
   * \param[in] params The domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If (params != nullptr), the key derivation context doesn't
                                                           support domain parameters
   * \reentrant TRUE
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
   * \brief Configure the number of iterations that will be applied by default.
   * \details Implementation can restrict minimal and/or maximal value of the iterations number.
   * \param[in] iterations Required number of the base function iterations (0 means implementation default number).
   * \return Actual number of iterations configured in the context now.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint32_t ConfigIterations(std::uint32_t iterations) noexcept override;

  /*!
   * \brief Derive a symmetric key from provided key material (with optional public salt).
   * \param[in] target_alg_id  The target symmetric algorithm identifier (also defines a target key-length).
   * \param[in] allowed_usage  The allowed usage scope of the target key.
   * \param[in] source_km  The source key material for the key derivation execution.
   * \param[in] salt  An optional salt value (if used, it should be unique for each instance of the target key).
   * \param[in] ctx_label  An optional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties).
   * \param[in] is_session  The "session" (or "temporary") attribute for the target key (if \c true).
   * \param[in] is_exportable  The exportability attribute for the target key (if \c true).
   * \param[in] params  An optional pointer to Domain Parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89).
   * \param[in] reserved_index  An optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created instance of derived symmetric key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kBadAlloc If (reserved_index != ara::crypto::cryp::kAllocObjectOnHeap).
   * \error ara::crypto::SecurityErrc::kIncompatibleObject Algorithm ID or key material is incompatible.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the salt or the label is empty.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Key derivation failed.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> DeriveKey(
      AlgId target_alg_id, ara::crypto::cryp::Key::Usage allowed_usage, ara::crypto::cryp::KeyMaterial const& source_km,
      ara::crypto::ReadOnlyMemRegion salt, ara::crypto::ReadOnlyMemRegion ctx_label, bool is_session,
      bool is_exportable, ara::crypto::cryp::DomainParameters::Sptrc params,
      ara::crypto::cryp::ReservedObjectIndex reserved_index, std::uint32_t iterations) const noexcept override;

  /*!
   * \brief Derive a symmetric key from provided key material (with secret salt).
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target key-length).
   * \param[in] allowed_usage Allowed usage scope of the target key.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt  The secret salt value (an additional secret value independent from the \c source_km).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key (if true).
   * \param[in] params An optional pointer to Domain Parameters required for full specification
   *                   of the symmetric transformation (e.g. variable S-boxes of GOST28147-89).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created instance of derived symmetric key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> DeriveKey(
      AlgId target_alg_id, ara::crypto::cryp::Key::Usage allowed_usage, ara::crypto::cryp::KeyMaterial const& source_km,
      ara::crypto::cryp::SecretSeed const& salt, ara::crypto::ReadOnlyMemRegion ctx_label, bool is_session,
      bool is_exportable, ara::crypto::cryp::DomainParameters::Sptrc params,
      ara::crypto::cryp::ReservedObjectIndex reserved_index, std::uint32_t iterations) const noexcept override;

  /*!
   * \brief Derive a "slave" key material (secret seed) from provided "master" key material (with optional public salt).
   * \details The byte sequence provided via argument ctx_label can include a few
   *          fields with different meaning separated by single 0x00 byte.
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target seed-length).
   * \param[in] allowed_usage Allowed usage scope of the target seed.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt Optional salt value (if used, it should be unique for each instance of the target key).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key material (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key material (if true).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created SecretSeed object, which keeps the derived key material.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SecretSeed::Uptrc> DeriveSeed(
      AlgId target_alg_id, ara::crypto::cryp::SecretSeed::Usage allowed_usage,
      ara::crypto::cryp::KeyMaterial const& source_km, ara::crypto::ReadOnlyMemRegion salt,
      ara::crypto::ReadOnlyMemRegion ctx_label, bool is_session, bool is_exportable,
      ara::crypto::cryp::ReservedObjectIndex reserved_index, std::uint32_t iterations) const noexcept override;

  /*!
   * \brief Derive a "slave" key material (secret seed) from provided "master" key material (with secret salt).
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target seed-length).
   * \param[in] allowed_usage Allowed usage scope of the target seed.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt A secret salt value (an additional secret value independent from the source_km).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key material (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key material (if true).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created SecretSeed object, which keeps the derived key material.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SecretSeed::Uptrc> DeriveSeed(
      AlgId target_alg_id, ara::crypto::cryp::SecretSeed::Usage allowed_usage,
      ara::crypto::cryp::KeyMaterial const& source_km, ara::crypto::cryp::SecretSeed const& salt,
      ara::crypto::ReadOnlyMemRegion ctx_label, bool is_session, bool is_exportable,
      ara::crypto::cryp::ReservedObjectIndex reserved_index, std::uint32_t iterations) const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable ara::core::String primitive_name_;              /*!< Primitive name */
};
}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_
