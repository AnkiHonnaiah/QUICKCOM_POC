/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  client_key_exchange_dh_message.h
 *         \brief  Class declaration for ClientKeyExchangeDhMessage.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_DH_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_DH_MESSAGE_H_

#include <cstdint>
#include <string>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/extensions/ec_extension.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class declaration for Client Key Exchange Diffie-Hellman message that contains client's ECDHE public key.
 */
class ClientKeyExchangeDhMessage : public HandshakeMessage {
 public:
  /*!
   * \brief Constructs a client key exchange message with Diffie-Hellman algorithm and initializes the base class.
   */
  ClientKeyExchangeDhMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqClientKeyExchange,
                         handshakestatemachine::HandshakeMessageHandle::kClientKeyExchange) {
    included_in_finish_calc_ = true;
  }
  /*!
   * \brief Type alias for byte string representation of an elliptic curve point, the key has fixed size of 32 bytes.
   */
  using ECPoint = amsr::core::Array<std::uint8_t, 32U>;

  /*!
   * \brief Specifies the ECDH public key.
   */
  struct ClientECDiffieHellmanPublic {
    /*! \brief Public key. */
    ECPoint ecdh_yc;
  };

  /*!
   * \brief Destructor.
   */
  ~ClientKeyExchangeDhMessage() noexcept override = default;

  /*!
   * \brief No copy construction.
   */
  ClientKeyExchangeDhMessage(ClientKeyExchangeDhMessage const&) = delete;

  /*!
   * \brief No move construction.
   */
  ClientKeyExchangeDhMessage(ClientKeyExchangeDhMessage&&) = delete;

  /*!
   * \brief Not copy assignable.
   */
  ClientKeyExchangeDhMessage& operator=(ClientKeyExchangeDhMessage const&) = delete;

  /*!
   * \brief Not move assignable.
   */
  ClientKeyExchangeDhMessage& operator=(ClientKeyExchangeDhMessage&&) = delete;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Clear the content for this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*!
   * \brief Gets the public key field of the ClientKeyExchange message.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \return Public key.
   * \pre Must be called after deserialization.
   * \vprivate This method is component private.
   */
  ECPoint GetPublicKey() const noexcept;

  /*!
   * \brief Sets the public key.
   * \param[in] public_key ECDH public key.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetPublicKey(ECPoint const public_key) noexcept;

  /*!
   * \brief Set the generated ECDH private key.
   * \param[in] private_key The generated ECDH private key.
   */
  void SetPrivateKey(ara::crypto::cryp::PrivateKey::Uptrc private_key) noexcept;
  /*!
   * \brief Gets the ECDH private key.
   * \return Unique pointer to the PrivateKey.
   */
  ara::crypto::cryp::PrivateKey::Uptrc GetPrivateKey() noexcept;

 private:
  /*!
   * \brief Specifies the ECDH public key and associated domain parameters.
   */
  ClientECDiffieHellmanPublic exchange_key_{};
  /*!
   * \brief The ECDH private key.
   */
  ara::crypto::cryp::PrivateKey::Uptrc private_key_{};
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_DH_MESSAGE_H_
