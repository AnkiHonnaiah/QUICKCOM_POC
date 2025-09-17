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
/*!        \file  handshake_message.h
 *        \brief  The parent class for all handshake messages.
 *      \details  Describes the API for Serialize() and Deserialize() functions which are implemented by the child
 *                classes. Also contains convenience functions for implementing the Serialize and Deserialize functions.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HANDSHAKE_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HANDSHAKE_MESSAGE_H_

#include <cstddef>
#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Parent class for all handshake message classes.
 * \trace SPEC-6562098
 * \vprivate This class is component private.
 */
class HandshakeMessage {
 public:
  /*! * \brief The default constructor. */
  HandshakeMessage() = delete;
  /*!
   * \brief Create a new HandshakeMessage object modelling a general TLS/DTLS Handshake Message.
   * \param[in] message_seq HandshakeMessage sequence number. Only used for DTLS to keep track of the handshake messages
   * exchanged during a handshake negotiation. This is called "message_seq" in the DTLS RFC 6347 Chapter 4.2.2. Should
   * not be confused with the sequence number on the record protocol level called "sequence_number".
   * \param[in] handshake_message_handle The handshake message handle for this Handshake Message.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561859, SPEC-6562067
   * \vprivate This method is private to AMSR product
   * \trace SPEC-6561859, SPEC-6562067
   */
  explicit HandshakeMessage(std::uint16_t message_seq,
                            handshakestatemachine::HandshakeMessageHandle const handshake_message_handle) noexcept
      : message_seq_{message_seq}, handshake_message_handle_{handshake_message_handle} {}

  /*! \brief Destructor */
  virtual ~HandshakeMessage() = default;

  /*!
   * \brief Not copy constructible.
   */
  HandshakeMessage(HandshakeMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  HandshakeMessage(HandshakeMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  HandshakeMessage& operator=(HandshakeMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  HandshakeMessage& operator=(HandshakeMessage&&) = delete;

  /*!
   * \brief Serialize HandshakeMessage into uint8_t vector
   * \details Takes all the fields of this HandshakeMessage and appends them byte by byte to the buffer according to the
   * message formats described in the RFCs for TLS/DTLS.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \return A byte vector containing the serialized message
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsErrSerialize is reported if serialization fails
   * \reentrant FALSE
   * \trace DSGN-SecCom-HandleHandshakeProtocolMessages
   */
  virtual ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept = 0;

  /*!
   * \brief Deserialize uint8_t vector into HandshakeMessage
   * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer Serialized bytestream source buffer containing the handshake message.
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails
   * \reentrant FALSE
   * \trace DSGN-SecCom-HandleHandshakeProtocolMessages
   */
  virtual void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept = 0;

  /*!
   * \brief Set sequence number for this HandshakeMessage
   * \param[in] message_seq HandshakeMessage sequence number. Only used for DTLS to keep track of the handshake messages
   * exchanged during a handshake negotiation. This is called "message_seq" in the DTLS RFC 6347 Chapter 4.2.2. Should
   * not be confused with the sequence number on the record protocol level called "sequence_number".
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-6562098
   */
  void SetMessageSeq(std::uint16_t const message_seq) noexcept { message_seq_ = message_seq; }

  /*!
   * \brief Retrieve the current sequence number of this message
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \return uint16_t The current sequence number
   */
  std::uint16_t GetMessageSeq() const noexcept { return message_seq_; }

  /*!
   * \brief Checks if this message is configured as a DTLS message
   * \return true if message is DTLS, false otherwise
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  bool IsDtls() const noexcept;

  /*!
   * \brief Configure if this message is TLS or DTLS and Server or Client.
   * \details Even if the enum EnTlsCommRole used to set this contains information about two properties (server/client
   * and TLS/DTLS) for the messages only the TLS/DTLS property is needed. So using kCommRoleTlsClient or
   * kCommRoleTlsServer is actually equivalent for the message classes and at this point the size of the message header
   * is also determined and set.
   * \param[in] role_to_take Used to decide if TLS or DTLS
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetCommRole(EnTlsCommRole const role_to_take) noexcept;

  /*!
   * \brief Deserialize the (D)TLS header and validate if the actual size of the payload is as specified by the header
   * \param[in, out] cxt TlsCommFlowContext used for reporting errors
   * \param[in] message A const reference to the message buffer
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if validation failed
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void DeserializeHeaderAndValidatePayload(common::TlsCommFlowContext& cxt,
                                           ara::core::Vector<std::uint8_t> const& message) noexcept;

  /*!
   * \brief Creates a serialized (D)TLS header for an unfragmented message with the specified message length
   * \param[in] message_length The length of the message
   * \return Serialized buffer containing the header
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> GetSerializedHeaderForUnfragmentedMessage(
      std::size_t const message_length) const noexcept;

  /*!
   * \brief Returns true if this message should be included in the hash calculation for the Finish message.
   * \return True if message should be included, false otherwise.
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   *
   * \trace DSGN-SecCom-ServerHandshakeStateMachine
   * \trace DSGN-SecCom-ClientHandshakeStateMachine
   */
  bool IsIncludedInFinishCalc() const noexcept { return included_in_finish_calc_; };

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_libseccom_V11.0.2_mutableMemberShallBePrivate
  /*! \brief Instantiate variable set to roleNotDefined */
  EnTlsCommRole role_to_take_{EnTlsCommRole::kCommRoleNotDefined};
  /*! \brief Stores the expected message sequence number to be used in the handshake procedure */
  std::uint16_t message_seq_;  // Only used in DTLS
  /*! \brief Indicates if this message should be included in the calculation of the Finish message */
  bool included_in_finish_calc_{false};
  /*! \brief The HandshakeMessageHandler for this handshake message */
  handshakestatemachine::HandshakeMessageHandle const handshake_message_handle_;
  /*! \brief The size of the (D)TLS header of this message */
  std::uint32_t header_size_{handshakestatemachine::kHandshakeMessageHeaderSizeTls};
  /*! \brief The length of the last deserialized payload */
  std::uint32_t payload_length_{0U};
  // VECTOR Enable VectorC++-V11.0.2

  /*! \brief Friend test declaration for CorrectInit */
  FRIEND_TEST(UT__ServerCertificateMessage, CorrectInit);
  /*! \brief Friend test declaration for CorrectInit */
  FRIEND_TEST(Unit__ServerCertificateMessage, CorrectInit);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_HANDSHAKE_MESSAGE_H_
