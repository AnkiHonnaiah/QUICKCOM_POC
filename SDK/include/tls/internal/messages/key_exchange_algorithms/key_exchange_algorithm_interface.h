/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  key_exchange_algorithm_interface.h
 *        \brief  An abstraction for all key exchange mechanisms used in TLS library.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_KEY_EXCHANGE_ALGORITHM_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_KEY_EXCHANGE_ALGORITHM_INTERFACE_H_

#include "ara/core/vector.h"

#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/messages/message_manager.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace key_exchange_algorithms {

/*! \brief Abstraction for key exchange algorithms. */
class KeyExchangeAlgorithmInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  KeyExchangeAlgorithmInterface() = default;

  /*!
   * \brief Destructor.
   */
  virtual ~KeyExchangeAlgorithmInterface() noexcept = default;

  KeyExchangeAlgorithmInterface(KeyExchangeAlgorithmInterface const&) = delete;
  KeyExchangeAlgorithmInterface(KeyExchangeAlgorithmInterface&&) = delete;
  KeyExchangeAlgorithmInterface& operator=(KeyExchangeAlgorithmInterface const&) = delete;
  KeyExchangeAlgorithmInterface& operator=(KeyExchangeAlgorithmInterface&&) = delete;

  /*!
   * \brief Creates the server key exchange message.
   * \param[in] msg_mgr Reference to the message manager which owns all handshake messages.
   * \pre -
   * \context Handshake
   * \reentrant FALSE
   * \synchronous TRUE
   * \error TlsCommErrorCode::kTlsErrInvalidArgument if the PSK Id Hint is empty. (PSK cipher suites)
   * \error TlsCommErrorCode::kTlsCryptoAdapterFailure if there is an error in the crypto adapter while preparing the
   * server key exchange message. (DH cipher suites)
   * \return Reference to the HandshakeMessage referring to the concrete ServerKeyExchange message.
   */
  virtual amsr::core::Result<messages::HandshakeMessage&> PrepareServerKeyExchangeMessage(
      messages::MessageManager& msg_mgr) const noexcept = 0;

  /*!
   * \brief Creates the client key exchange message.
   * \param[in] msg_mgr Reference to the message manager which owns all handshake messages.
   * \pre -
   * \context Handshake
   * \reentrant FALSE
   * \synchronous TRUE
   * \error TlsCommErrorCode::kTlsPskIdentityNotFound If the PSK ID could not be found.
   * \return Reference to the HandshakeMessage referring to the concrete ClientKeyExchange message.
   */
  virtual amsr::core::Result<messages::HandshakeMessage&> PrepareClientKeyExchangeMessage(
      messages::MessageManager& msg_mgr) const noexcept = 0;

  /*!
   * \brief Handles the reception of a client key exchange message from network.
   * \param[in] raw_message The raw message received from the network.
   * \param[in, out] msg_mgr The message manager to assign the received message to.
   * \error TlsCommErrorCode::kTlsErrDeserialize if deserialization fails.
   * \return void on success.
   */
  virtual amsr::core::Result<void> OnClientKeyExchangeMessageReceived(
      ara::core::Vector<std::uint8_t> const& raw_message, messages::MessageManager& msg_mgr) const noexcept = 0;
  /*!
   * \brief Handles the reception of a server key exchange message from network.
   * \param[in] raw_message The raw message received from the network.
   * \param[in, out] msg_mgr The message manager to assign the received message to.
   * \error TlsCommErrorCode::kTlsErrDeserialize if deserialization fails.
   * \return void on success.
   */
  virtual amsr::core::Result<void> OnServerKeyExchangeMessageReceived(
      ara::core::Vector<std::uint8_t> const& raw_message, messages::MessageManager& msg_mgr) noexcept = 0;
};

}  // namespace key_exchange_algorithms
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_KEY_EXCHANGE_ALGORITHM_INTERFACE_H_
