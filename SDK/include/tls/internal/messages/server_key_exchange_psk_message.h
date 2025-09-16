/**********************************************************************************************************************
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
/*!        \file  server_key_exchange_psk_message.h
 *        \brief  Class describing a TLS/DTLS Server Key Exchange Message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_PSK_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_PSK_MESSAGE_H_

#include <cstdint>
#include <string>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Server Key Exchange Message
 * \details This implementation is only for psk (Pre Shared Key) KeyExchangeAlgorithm (See RFC4279) so the only field in
 * the ServerKeyExchangePskMessage is the psk_identity_hint.
 * \trace SPEC-6561918, SPEC-6561919, SPEC-6562784, DSGN-SecCom-PSK
 * \vprivate This class is component private.
 */
class ServerKeyExchangePskMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new ServerKeyExchangePskMessage object representing a Server Key Exchange Message
   * \reentrant FALSE
   */
  ServerKeyExchangePskMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqServerKeyExchange,
                         handshakestatemachine::HandshakeMessageHandle::kServerKeyExchange) {
    included_in_finish_calc_ = true;
  }

  /*! \brief Destructor */
  ~ServerKeyExchangePskMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  ServerKeyExchangePskMessage(ServerKeyExchangePskMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ServerKeyExchangePskMessage(ServerKeyExchangePskMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ServerKeyExchangePskMessage& operator=(ServerKeyExchangePskMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ServerKeyExchangePskMessage& operator=(ServerKeyExchangePskMessage&&) = delete;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \vprivate This method is component private.
   * \trace SPEC-6562784
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Sets the psk_identity_hint field of the ServerKeyExchange message using the data from the supplied vector.
   * \param[in] psk_id_hint uint8_t vector with new PskIdentityHint
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  void SetPskIdentityHint(ara::core::Vector<std::uint8_t> const& psk_id_hint) noexcept {
    psk_identity_hint_ = psk_id_hint;
  }

  /*!
   * \brief Gets the psk_identity_hint field for this ServerKeyExchange message.
   * \return A byte vector reference containing the psk_identity_hint
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> const& GetPskIdentityHint() const noexcept { return psk_identity_hint_; }

  /*!
   * \brief Gets the psk_identity_hint field for this ServerKeyExchange message in std::string
   * \return A std::string object containing the psk_identity_hint
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  std::string GetPskIdentityHintStr() const noexcept;

  /*!
   * \brief Clear the psk_identity_hint for this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*! \brief Constant with HandshakeType(12) = server_key_exchange */
  static std::uint8_t const kHandshakeType{
      static_cast<std::uint8_t>(handshakestatemachine::HandshakeMessageHandle::kServerKeyExchange)};

 private:
  /*!
   * \brief PSK identity hint
   * \trace SPEC-6562783
   */
  ara::core::Vector<std::uint8_t> psk_identity_hint_{};

  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_PSK_MESSAGE_H_
