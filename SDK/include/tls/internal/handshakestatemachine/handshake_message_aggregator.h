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
/*!        \file  handshake_message_aggregator.h
 *         \brief  This file declares HandshakeMessageAggregator class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_MESSAGE_AGGREGATOR_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_MESSAGE_AGGREGATOR_H_

#include <cstdint>
#include <deque>
#include <map>
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

/*!
 * \brief Aggregates received handshake data from record protocol into complete handshake messages and sends them to the
 * handshake state machine. Implements the RecordToHandshakeCallbackInterface so that the RecordProtocol can send the
 * received data to this class.
 * \vprivate This class is component private.
 */
class HandshakeMessageAggregator final : public record_protocol::RecordToHandshakeCallbackInterface {
 public:
  /*!
   * \brief Constructor for the aggregator. Takes an interface pointer for where to send the aggregated data.
   * \param[in] handshake_interface Reference to a handshake callback interface
   * \reentrant FALSE
   */
  explicit HandshakeMessageAggregator(
      record_protocol::RecordToHandshakeCallbackInterface& handshake_interface) noexcept;
  /*! \brief Not copy constructible. */
  HandshakeMessageAggregator(HandshakeMessageAggregator const&) = delete;
  /*! \brief Not move constructible. */
  HandshakeMessageAggregator(HandshakeMessageAggregator&&) = delete;
  /*! \brief Not copy assignable. */
  HandshakeMessageAggregator& operator=(HandshakeMessageAggregator const&) = delete;
  /*! \brief Not move assignable. */
  HandshakeMessageAggregator& operator=(HandshakeMessageAggregator&&) = delete;
  /*! \brief Default destructor */
  ~HandshakeMessageAggregator() noexcept final = default;

  /*!
   * \brief Sets the TLS type (TLS/DTLS)
   * \param[in,out] tls_type The TLS type
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetTlsType(common::TlsType tls_type) noexcept;
  /*!
   * \brief Cleans the objects internal data.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;
  /*!
   * \copydoc record_protocol::RecordToHandshakeCallbackInterface::CloseRequest
   * \vprivate This method is component private.
   */
  void CloseRequest(common::TlsCommFlowContext& cxt) noexcept final;
  /*!
   * \copydoc record_protocol::RecordToHandshakeCallbackInterface::OnHandshakeData
   * \details Potential errors will be propagated to the caller of this function. Please check the status of the flow
   * context after calling this function.
   * \trace SPEC-6562070
   * \vprivate This method is component private.
   */
  void OnHandshakeData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer,
                       bool retransmit) noexcept final;
  /*!
   * \copydoc record_protocol::RecordToHandshakeCallbackInterface::OnAlertData
   * \error TlsCommErrorCode::kTlsErrInternal is returned if calling of onAlertData failed.
   * \vprivate This method is component private.
   */
  void OnAlertData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer) noexcept final;
  /*!
   * \copydoc record_protocol::RecordToHandshakeCallbackInterface::OnChangeCipherData
   * \error TlsCommErrorCode::kTlsErrInternal is returned if calling of OnChangeCipherData failed.
   * \vprivate This method is component private.
   */
  void OnChangeCipherData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer) noexcept final;

 private:
  /*!
   * \brief MessageFragmentCollector struct used to keep track of received fragments of handshake messages
   */
  struct MessageFragmentCollector {
    /*! \brief Expected length of the message */
    std::uint32_t total_length_{0U};

    /*! \brief Length of the collected message */
    std::uint32_t collected_{0U};

    /*!
     * \brief Container for keeping track of which fragments have already been processed. Fragment offset is unique.
     */
    ara::core::Vector<std::uint32_t> processed_offsets_{};

    /*! \brief Buffer of the data */
    ara::core::Vector<std::uint8_t> buffer_{};

    /*! \brief Flag indicating if this MessageFragmentCollect is new */
    bool new_entry_{true};
  };

  /*! \brief Sequence number of the next message */
  std::uint16_t next_message_seq_nbr_{0U};

  /*! \brief Initializing the TLS type */
  common::TlsType tls_type_{common::TlsType::kTls};

  /*! \brief Map which keeps track of the current handshake message in sequence to defragment */
  std::map<std::uint16_t, MessageFragmentCollector> dtls_defragmentation_map_{};

  /*! \brief MessageFragmentCollector to use for accumulation of fragmented TLS handshake messages  */
  MessageFragmentCollector tls_mfc_{};

  /*! \brief Flag to keep track of if the next received message is part of a fragmented TLS handshake message */
  bool tls_is_expecting_fragments{false};

  /*! \brief A reference to the HandshakeStateMachine interface specific for this HandshakeMessageAggregator */
  record_protocol::RecordToHandshakeCallbackInterface& handshake_interface_;

  /*! \brief A return value type containing additional information about (D)TLS handshake headers */
  struct HandshakeMessageCheckResult {
    /*! \brief flag indicating if a supplied message is complete or fragmented */
    bool is_complete_{false};
    /*! \brief The length of the complete message */
    std::uint32_t message_length_{0U};
    /*! \brief The offset of the message where a fragment starts */
    std::uint32_t fragment_offset_{0U};
    /*! \brief The length of the fragment */
    std::uint32_t fragment_length_{0U};
  };

  /*!
   * \brief Peforms a check for if the buffer contains enough data for a complete DTLS handshake fragment
   * \param[in] buffer A reference to the buffer containing header and fragment data
   * \return The result of the check and additional data from the header
   * \reentrant FALSE
   */
  static HandshakeMessageCheckResult CheckForCompleteDTLSHandshakeMessageFragment(
      std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Peforms a check for if the buffer contains enough data for a complete TLS handshake message
   * \param[in] buffer A reference to the buffer containing header and message data
   * \return The result of the check and additional data from the header
   * \reentrant FALSE
   */
  static HandshakeMessageCheckResult CheckForCompleteTLSHandshakeMessage(
      std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Extract the message length value from the header contained in the buffer
   * \param[in] buffer A reference to the buffer containing header information
   * \return The message length
   * \reentrant FALSE
   */
  static std::uint32_t GetMessageLength(std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Gets the MessageSeq number from a serialized DTLS handshake message.
   * \param[in] buffer The serialized DTLS handshake message.
   * \return uint16_t An integer describing the deserialized message sequence field or 0 if DTLS header is not complete.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is private to AMSR product
   */
  static std::uint16_t GetMessageSeqNo(std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Gets the fragment offset from a serialized DTLS handshake message.
   * \param[in] buffer The serialized DTLS handshake message.
   * \return uint32_t An integer describing the fragment offset of this message, or 0 if DTLS header is not complete.
   * \context Any
   * \pre -
   * \reentrant FALSE
   */
  static std::uint32_t GetDtlsFragmentOffset(std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Extract the DTLS fragment length value from the header contained in the buffer
   * \param[in] buffer A reference to the buffer containing header information
   * \return The fragment length
   * \reentrant FALSE
   */
  static std::uint32_t GetDtlsFragmentLength(std::deque<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Handles received TLS handshake messages
   * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in, out] message_queue A reference to the message queue containing messages to process
   * \reentrant FALSE
   */
  void OnTLSHandshakeData(common::TlsCommFlowContext& cxt, std::deque<std::uint8_t>& message_queue) noexcept;

  /*!
   * \brief Handles received DTLS handshake messages
   * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in, out] message_queue A reference to the message queue containing messages to process
   * \reentrant FALSE
   */
  void OnDTLSHandshakeData(common::TlsCommFlowContext& cxt, std::deque<std::uint8_t>& message_queue) noexcept;

  /*! \brief Friend test declaration for GetMessageSeqNo */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, GetMessageSeqNo);
  /*! \brief Friend test declaration for GetMessageLength */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, GetMessageLength);
  /*! \brief Friend test declaration for GetDtlsFragmentOffset */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, GetDtlsFragmentOffset);
  /*! \brief Friend test declaration for GetDtlsFragmentLength */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, GetDtlsFragmentLength);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, Cleanup);
  /*! \brief Friend test declaration for CheckForCompleteTlsHandshakeMessageInvalidArgument */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, CheckForCompleteTlsHandshakeMessageInvalidArgument);
  /*! \brief Friend test declaration for CheckForCompleteDtlsHandshakeFragmentInvalidArgument */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, CheckForCompleteDtlsHandshakeFragmentInvalidArgument);
  /*! \brief Friend test declaration for CloseRequestContractViolation */
  FRIEND_TEST(Cdd__handshakemessage__aggregation, CloseRequestContractViolation);

  /*! \brief Friend test declaration for GetMessageSeqNo */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, GetMessageSeqNo);
  /*! \brief Friend test declaration for GetMessageLength */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, GetMessageLength);
  /*! \brief Friend test declaration for GetDtlsFragmentOffset */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, GetDtlsFragmentOffset);
  /*! \brief Friend test declaration for GetDtlsFragmentLength */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, GetDtlsFragmentLength);
  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, Cleanup);
  /*! \brief Friend test declaration for CheckForCompleteTlsHandshakeMessageInvalidArgument */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, CheckForCompleteTlsHandshakeMessageInvalidArgument);
  /*! \brief Friend test declaration for CheckForCompleteDtlsHandshakeFragmentInvalidArgument */
  FRIEND_TEST(UT__HandshakeStateMachine__Aggregation, CheckForCompleteDtlsHandshakeFragmentInvalidArgument);
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_MESSAGE_AGGREGATOR_H_
