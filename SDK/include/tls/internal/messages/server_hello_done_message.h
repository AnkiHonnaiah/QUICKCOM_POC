/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   server_hello_done_message.h
 *        \brief  Class describing a TLS/DTLS Server Hello Done Message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_DONE_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_DONE_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Server Hello Done Message
 * \trace SPEC-6561932, SPEC-6561933
 * \vprivate This method is component private.
 */
class ServerHelloDoneMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create new ServerHelloDoneMessage object representing a Server Hello Done Message
   * \reentrant TRUE
   */
  ServerHelloDoneMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqServerHelloDone,
                         handshakestatemachine::HandshakeMessageHandle::kServerHelloDone) {
    included_in_finish_calc_ = true;
  }

  /*!
   * \brief Destructor
   */
  ~ServerHelloDoneMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  ServerHelloDoneMessage(ServerHelloDoneMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ServerHelloDoneMessage(ServerHelloDoneMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ServerHelloDoneMessage& operator=(ServerHelloDoneMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ServerHelloDoneMessage& operator=(ServerHelloDoneMessage&&) = delete;

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
   * \brief Constant with HandshakeType(14) = server_hello_done
   */
  static std::uint8_t const kHandshakeType{
      static_cast<std::uint8_t>(handshakestatemachine::HandshakeMessageHandle::kServerHelloDone)};
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_DONE_MESSAGE_H_
