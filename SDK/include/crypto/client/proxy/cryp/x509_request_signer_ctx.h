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
/*!        \file  proxy/cryp/x509_request_signer_ctx.h
 *        \brief  Proxy implementation for ara::crypto::cryp::X509RequestSignerCtx.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::X509RequestSignerCtx interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_REQUEST_SIGNER_CTX_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_REQUEST_SIGNER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/x509_request_signer_ctx.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::X509RequestSignerCtx
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class X509RequestSignerCtx : public BaseProxy, public ara::crypto::cryp::X509RequestSignerCtx {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<X509RequestSignerCtx, ara::crypto::CustomDeleter>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  X509RequestSignerCtx(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
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
  static std::unique_ptr<X509RequestSignerCtx, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    return std::unique_ptr<X509RequestSignerCtx, ara::crypto::CustomDeleter>(new X509RequestSignerCtx(
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
   * \brief Check if the context is already initialized and ready to use.
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
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Reset the context.
   * \param[in] params The domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kIncompatibleObject If the given domain parameters object is incompatible.
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
   * \return Actual length of a key (now set to the algorithm context) in bits and 0 if key is not set.
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
   * \details Method not supported
   * \vprivate Component Private
   */
  ara::core::Result<void> SetKey(ara::crypto::cryp::PrivateKey const& key) noexcept override;

  /*!
   * \brief Get the required hash size.
   * \return Required hash size in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetRequiredHashSize() const noexcept override;
  /*!
   * \brief Get the ID of the required hash algorithm.
   * \return Required hash algorithm ID or \c kAlgIdAny if the signature algorithm specification
   *         does not include a concrete hash function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId GetRequiredHashAlgId() const noexcept override;
  /*!
   * \brief Get size of the signature value produced and required by the current algorithm.
   * \return Size of the signature value in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetSignatureSize() const noexcept override;
  /*!
   * \brief Creates certification request for a private key loaded to the context.
   * \param[in] der_subject_dn A DER-encoded subject distinguished name (DN) of the private key owner.
   * \param[in] x509_extensions A DER-encoded X.509 Extensions that should be included to the certification request.
   * \param[in] version Format version of the target certification request.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for this
   *                          allocation or default marker, which says to allocate on the heap.
   * \return Unique smart pointer to created certification request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnexpectedValue The x509 extensions are not empty (currently not supported)
   *                                                    or DER encoded subject DN is empty
   *                                                    or version is invalid.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Key is invalid
   *                                                 or failed to parse the DER encoded distinguished name
   * \error ara::crypto::SecurityErrc::kUnexpectedValue Unable to create X509CertRequest.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::X509CertRequest::Uptrc> CreateCertRequest(
      ara::crypto::ReadOnlyMemRegion der_subject_dn, ara::crypto::ReadOnlyMemRegion x509_extensions,
      std::uint32_t version, ara::crypto::cryp::ReservedObjectIndex reserved_index) const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable ara::core::String primitive_name_;              /*!< Primitive name */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_X509_REQUEST_SIGNER_CTX_H_
