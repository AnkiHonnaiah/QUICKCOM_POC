/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   client_hello_message.h
 *        \brief  Class describing a TLS/DTLS Client Hello message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_HELLO_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_HELLO_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Client Hello message
 * \trace SPEC-8264448, SPEC-6562054, SPEC-6562098
 * \vprivate This class is component private.
 */
class ClientHelloMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new ClientHelloMessage object representing a Client Hello message
   * \reentrant TRUE
   * */
  ClientHelloMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqClientHello,
                         handshakestatemachine::HandshakeMessageHandle::kClientHello) {
    included_in_finish_calc_ = true;
  }

  /*! \brief Destructor */
  ~ClientHelloMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  ClientHelloMessage(ClientHelloMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ClientHelloMessage(ClientHelloMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ClientHelloMessage& operator=(ClientHelloMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ClientHelloMessage& operator=(ClientHelloMessage&&) = delete;

  /*!\copydoc ::amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \error TlsCommErrorCode::kTlsErrSerialize is reported if data is missing in ClientHello. Either random_,
   * cipher_suites_ or compression_methods_ is empty
   * \trace SPEC-8264448, SPEC-6562054
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!\copydoc ::amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \vprivate This method is component private.
   * \trace SPEC-8264448, SPEC-6561875, SPEC-6562054
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Set ProtocolVersion, CipherSuites and CompressionMethod to default for this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-8264448, SPEC-8264447, SPEC-8797234
   * \vprivate This method is component private.
   */
  void DefaultSettings() noexcept;

  /*!
   * \brief Set RandomBytes for this message
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] random uint8_t vector with random bytes
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrUnexpectedInternalParam is reported if supplied random size does not match the
   * expected value
   * \trace SPEC-8264448
   * \vprivate This method is component private.
   */
  void SetRandom(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& random) noexcept;

  /*!
   * \brief Set CipherSuites for this message
   * \param[in] cipher_suites uint16_t vector with CipherSuite to set
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  void SetCipherSuites(ara::core::Vector<std::uint16_t> const& cipher_suites) noexcept {
    cipher_suites_ = cipher_suites;
  }

  /*!
   * \brief Set the Cookie of this message
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] cookie uint8_t vector with the Cookie to set
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrUnexpectedInternalParam is reported if failed to set cookie, Cookie size is too big
   * \trace SPEC-8264448, SPEC-6562054
   * \vprivate This method is component private.
   */
  void SetCookie(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& cookie) noexcept;

  /*!
   * \brief Sets the  extension data
   * \param[in] extensions The extension data as a vector of uint8_t
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  void SetClientHelloExtensions(ara::core::Vector<std::uint8_t> const& extensions) noexcept {
    extensions_ = extensions;
  }

  /*!
   * \brief Get RandomBytes for this message
   * \return uint8_t vector with messageRandomByte
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint8_t> const& GetRandomBytes() const noexcept { return random_; }

  /*!
   * \brief Get CipherSuites for this message
   * \return uint16_t vector with CipherSuites
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint16_t> const& GetCipherSuites() const noexcept { return cipher_suites_; }

  /*!
   * \brief Get the Cookie of this message
   * \return uint8_t vector with Cookie
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint8_t> const& GetCookie() const noexcept { return cookie_; }

  /*!
   * \brief Get the SessionId of this message
   * \return uint8_t vector with SessionId
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint8_t> const& GetSessionId() const noexcept { return session_id_; }

  /*!
   * \brief Get CompressionMethods of this message
   * \return uint8_t vector with CompressionMethods
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint8_t> const& GetCompressionMethods() const noexcept { return compression_methods_; }

  /*!
   * \brief Get the version of this message
   * \return record_protocol::ProtocolVersion the protocol version
   * \context ANY
   * \pre -
   * \reentrant TRUE
   */
  record_protocol::ProtocolVersion const GetVersion() const noexcept { return {major_version_, minor_version_}; }

  /*!
   * \brief Get Extensions of this message
   * \return uint8_t vector with Extensions
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-8264448
   */
  ara::core::Vector<std::uint8_t> const& GetClientHelloExtensions() const noexcept { return extensions_; }

  /*!
   * \brief Clear ProtocolVersion, RandomBytes, SessionId, CipherSuites, Cookie, CompressionMethods and extensions for
   * this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-8264448
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*!
   * \brief Check if the protocol version of this message is supported
   * \param[in, out] cxt Reference to TlsCommFlowContext used for error reporting
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void CheckProtocolVersion(common::TlsCommFlowContext& cxt) const noexcept;

  /*! \brief Constant with HandshakeType(1) = client_hello */
  static std::uint8_t const kHandshakeType{
      static_cast<uint8_t>(handshakestatemachine::HandshakeMessageHandle::kClientHello)};

 private:
  /*!\brief Major version of TLS version */
  std::uint8_t major_version_{0U};
  /*!\brief Minor version of TLS version */
  std::uint8_t minor_version_{0U};
  /*!\brief Random value */
  ara::core::Vector<std::uint8_t> random_{};
  /*!\brief Session ID */
  ara::core::Vector<std::uint8_t> session_id_{};
  /*!\brief Cipher suite this variable determines all the supported cipher suites */
  ara::core::Vector<std::uint16_t> cipher_suites_{};
  /*!\brief Compression algorithm this variable determines all the supported compression algorithms */
  ara::core::Vector<std::uint8_t> compression_methods_{};
  /*!\brief Cookie */
  ara::core::Vector<std::uint8_t> cookie_{};
  /*!\brief extensions */
  ara::core::Vector<std::uint8_t> extensions_{};

  /*! \brief Friend test declaration for SerializeError */
  FRIEND_TEST(Cdd__ClientHelloMessage, SerializeError);
  /*! \brief Friend test declaration for Deserialize */
  FRIEND_TEST(Cdd__ClientHelloMessage, Deserialize);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(Cdd__ClientHelloMessage, Cleanup);
  /*! \brief Friend test declaration for Clear */
  FRIEND_TEST(Cdd__ClientHelloMessage, Clear);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(Cdd__TLSServerStateDisconnected, OnHandshakeMessageReceived);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(UT__MessageManager, Cleanup);
  /*! \brief Friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(UT__ServerHandshakeStatePool_TlsServerStateDisconnected, OnHandshakeMessageReceived);
  /*! \brief Friend test declaration for SerializeError */
  FRIEND_TEST(UT__ClientHelloMessage, SerializeError);
  /*! \brief Friend test declaration for Deserialize */
  FRIEND_TEST(UT__ClientHelloMessage, Deserialize);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(UT__ClientHelloMessage, Cleanup);
  /*! \brief Friend test declaration for Clear */
  FRIEND_TEST(UT__ClientHelloMessage, Clear);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CLIENT_HELLO_MESSAGE_H_
