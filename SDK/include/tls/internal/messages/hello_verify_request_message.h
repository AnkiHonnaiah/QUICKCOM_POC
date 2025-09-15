/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   hello_verify_request_message.h
 *        \brief  Class describing a DTLS Hello Verify Request Message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HELLO_VERIFY_REQUEST_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HELLO_VERIFY_REQUEST_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a DTLS Hello Verify Request Message
 * \trace SPEC-8797234, SPEC-6562098
 * \vprivate This method is component private.
 */
class HelloVerifyRequestMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new HelloVerifyRequestMessage object representing a Hello Verify Request Message
   * \reentrant TRUE
   */
  HelloVerifyRequestMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqServerHelloVerifyRequest,
                         handshakestatemachine::HandshakeMessageHandle::kHelloVerifyRequest) {}

  /*! \brief Destructor */
  ~HelloVerifyRequestMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  HelloVerifyRequestMessage(HelloVerifyRequestMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  HelloVerifyRequestMessage(HelloVerifyRequestMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  HelloVerifyRequestMessage& operator=(HelloVerifyRequestMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  HelloVerifyRequestMessage& operator=(HelloVerifyRequestMessage&&) = delete;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \error TlsCommErrorCode::kTlsErrSerialize is reported if serialization fails
   * \trace SPEC-8797235, SPEC-8797235
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails
   * \trace SPEC-8797235
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Restore to default settings for this message by setting the ProtocolVersion field.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-8797235
   * \trace SPEC-6562058
   * \vprivate This method is component private.
   */
  void DefaultSettings() noexcept;

  /*!
   * \brief Get Cookie for this message
   * \return uint8_t STL container with serialized cookie
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8797235
   */
  ara::core::Vector<std::uint8_t> const& GetCookie() const noexcept { return cookie_; };

  /*!
   * \brief Set cookie for this message
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] cookie STL container with serialized cookie
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrUnexpectedInternalParam is reported if setting cookie_ fails
   * \trace SPEC-8797235
   * \vprivate This method is component private.
   */
  void SetCookie(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& cookie) noexcept;

  /*!
   * \brief Reset data for this HelloVerifyRequestMessage
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8797235
   */
  void Cleanup() noexcept;

  /*! \brief Constant with HandshakeType(3) = hello_verify_request */
  static std::uint8_t const kHandshakeType{
      static_cast<std::uint8_t>(handshakestatemachine::HandshakeMessageHandle::kHelloVerifyRequest)};

 private:
  /*! \brief Minor version of TLS version */
  std::uint8_t minor_version_{};
  /*! \brief Major version of TLS version */
  std::uint8_t major_version_{};
  /*! \brief Serialized cookie */
  ara::core::Vector<std::uint8_t> cookie_{};
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declaration for DefaultSettings */
  FRIEND_TEST(Cdd__HelloVerifyRequestMessage, DefaultSettings);
  /*! \brief Friend test declaration for Serialize */
  FRIEND_TEST(Cdd__HelloVerifyRequestMessage, Serialize);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(UT__MessageManager, Cleanup);
  /*! \brief Friend test declaration for DefaultSettings */
  FRIEND_TEST(UT__HelloVerifyRequestMessage, DefaultSettings);
  /*! \brief Friend test declaration for Serialize */
  FRIEND_TEST(UT__HelloVerifyRequestMessage, Serialize);
  /*! \brief Friend test declaration for OnHandshakeMessageReceived_NoRetransmit_HelloVerifyRequestMessage */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateClientHello,
              OnHandshakeMessageReceived_NoRetransmit_HelloVerifyRequestMessage);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HELLO_VERIFY_REQUEST_MESSAGE_H_
