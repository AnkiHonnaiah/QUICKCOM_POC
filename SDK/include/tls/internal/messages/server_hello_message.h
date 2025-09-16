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
/*!       \file   server_hello_message.h
 *        \brief  Class describing a TLS/DTLS Server Hello message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Server Hello Message
 * \details This ServerHelloMessage implementation does not support extensions field
 * \trace SPEC-8264449
 * \vprivate This class is component private.
 */
class ServerHelloMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new ServerHelloMessage object representing a Server Hello Message
   * \reentrant FALSE
   */
  ServerHelloMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqServerHello,
                         handshakestatemachine::HandshakeMessageHandle::kServerHello) {
    included_in_finish_calc_ = true;
  }

  /*! \brief Destructor */
  ~ServerHelloMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  ServerHelloMessage(ServerHelloMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ServerHelloMessage(ServerHelloMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ServerHelloMessage& operator=(ServerHelloMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ServerHelloMessage& operator=(ServerHelloMessage&&) = delete;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \trace SPEC-8264449
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \trace SPEC-8264449
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Set RandomBytes for this message
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] random Byte vector with random bytes
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is reported if an invalid argument was received
   * \trace SPEC-8264449
   * \vprivate This method is component private.
   */
  void SetRandom(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& random) noexcept;
  /*!
   * \brief Set Extensions for this message
   * \param[in] extensions Byte vector with serialized extensions
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is reported if an invalid argument was received
   * \vprivate This method is component private.
   */
  void SetServerHelloExtensions(ara::core::Vector<std::uint8_t> const& extensions) noexcept;
  /*!
   * \brief Set CipherSuite for this message
   * \param[in] cipher_suite A 2-byte value specifying the CipherSuite.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264449
   */
  void SetCipherSuite(std::uint16_t cipher_suite) noexcept { cipher_suite_ = cipher_suite; }

  /*!
   * \brief Get CipherSuite for this message
   * \return uint16_t with the CipherSuite
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264449
   */
  std::uint16_t GetCipherSuite() const noexcept { return cipher_suite_; }

  /*!
   * \brief Get RandomBytes for this message
   * \return uint8_t vector with messageRandomByte
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264449
   */
  ara::core::Vector<std::uint8_t> const& GetRandomBytes() const noexcept { return random_; }

  /*!
   * \brief Get the Protocol Version for this message packed into one 16 bit unsigned integer
   * \details The high part contains the major and the low part the minor
   * \return the protcol version packed into one uint16_t
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  std::uint16_t GetProtocolVersion() const noexcept;

  /*!
   * \brief Get Extensions of this message
   * \return uint8_t vector with Extensions
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> const& GetServerHelloExtensions() const noexcept { return extensions_; }

  /*!
   * \brief Check if the protocol version of this message is supported
   * \param[in, out] cxt Reference to TlsCommFlowContext used for error reporting
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void CheckProtocolVersion(common::TlsCommFlowContext& cxt) const noexcept;

  /*!
   * \brief Set ProtocolVersion to default value for this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264449
   * \trace SPEC-6562058
   */
  void DefaultSettings() noexcept;

  /*!
   * \brief Clear ProtocolVersion, RandomBytes, SessionId, CipherSuite and CompressionMethod for
   * this message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-8264449
   */
  void Cleanup() noexcept;

  /*! \brief Constant with HandshakeType(2) = server_hello */
  static std::uint8_t const kHandshakeType{
      static_cast<uint8_t>(handshakestatemachine::HandshakeMessageHandle::kServerHello)};

 private:
  /*!\brief Major version of TLS version */
  std::uint8_t major_version_{0U};
  /*!\brief Minor version of TLS version */
  std::uint8_t minor_version_{0U};
  /*!\brief Random value */
  ara::core::Vector<std::uint8_t> random_{};
  /*!\brief Session ID */
  ara::core::Vector<std::uint8_t> session_id_{};
  /*!\brief Cipher suite */
  std::uint16_t cipher_suite_{0U};
  /*!\brief Compression algorithm */
  std::uint8_t compression_method_{0U};
  /*!\brief extensions */
  ara::core::Vector<std::uint8_t> extensions_{};
  /*! \brief Friend test declaration for Serialize */
  FRIEND_TEST(Cdd__ServerHelloMessage, Serialize);
  /*! \brief Friend test declaration for Cleanup ServerHelloMessage*/
  FRIEND_TEST(Cdd__ServerHelloMessage, Cleanup);
  /*! \brief Friend test declaration for Cleanup MessageManager*/
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declaration for Cleanup MessageManager*/
  FRIEND_TEST(UT__MessageManager, Cleanup);
  /*! \brief Friend test declaration for Serialize */
  FRIEND_TEST(UT__ServerHelloMessage, Serialize);
  /*! \brief Friend test declaration for Cleanup ServerHelloMessage*/
  FRIEND_TEST(UT__ServerHelloMessage, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_HELLO_MESSAGE_H_
