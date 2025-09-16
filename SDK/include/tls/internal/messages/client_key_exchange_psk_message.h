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
/*!       \file   client_key_exchange_psk_message.h
 *        \brief  Class describing a TLS/DTLS Client Key Exchange Message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_PSK_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_PSK_MESSAGE_H_

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
 * \brief Class describing a TLS/DTLS Client Key Exchange Message
 * \details This implementation is only for psk (Pre Shared Key) KeyExchangeAlgorithm (See RFC4279) so the only field in
 * the ClientKeyExchangePskMessage is the psk_identity.
 * \trace SPEC-6561943, SPEC-6561942, SPEC-6562784, DSGN-SecCom-PSK
 * \vprivate This class is component private.
 */
class ClientKeyExchangePskMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new ClientKeyExchangePskMessage object representing a Client Key Exchange Message
   * \reentrant TRUE
   */
  ClientKeyExchangePskMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqClientKeyExchange,
                         handshakestatemachine::HandshakeMessageHandle::kClientKeyExchange) {
    included_in_finish_calc_ = true;
  }

  /*! \brief Destructor */
  ~ClientKeyExchangePskMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  ClientKeyExchangePskMessage(ClientKeyExchangePskMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ClientKeyExchangePskMessage(ClientKeyExchangePskMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ClientKeyExchangePskMessage& operator=(ClientKeyExchangePskMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ClientKeyExchangePskMessage& operator=(ClientKeyExchangePskMessage&&) = delete;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Sets the psk_identity field of the ClientKeyExchange message using the data from the supplied vector.
   * \param[in] psk_identity uint8_t vector with new PskIdentity
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-PSK, SPEC-6562784
   * \vprivate This method is component private.
   */
  void SetPskIdentity(ara::core::Vector<std::uint8_t> const& psk_identity) noexcept { psk_identity_ = psk_identity; }

  /*!
   * \brief Gets the psk_identity_field for this ClientKeyExchange message.
   * \return A const reference to the psk identity
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \trace DSGN-SecCom-PSK, SPEC-6562784
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> const& GetPskIdentity() const noexcept { return psk_identity_; }

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

  /*! \brief Constant with HandshakeType(16) = client_key_exchange */
  static std::uint8_t const kHandshakeType{
      static_cast<uint8_t>(handshakestatemachine::HandshakeMessageHandle::kClientKeyExchange)};

  /*!
   * \brief Clear PskIdentity for this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562784
   */
  void Cleanup() noexcept;

 private:
  /*!
   * \brief PSK identity
   * \trace SPEC-6562805, SPEC-6562783
   */
  ara::core::Vector<std::uint8_t> psk_identity_{};
  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_KEY_EXCHANGE_PSK_MESSAGE_H_
