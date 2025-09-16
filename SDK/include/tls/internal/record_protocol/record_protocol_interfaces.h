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
/*!       \file     record_protocol_interfaces.h
 *        \brief    This file contains the declarations for record protocol interfaces with other protocols
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_INTERFACES_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_INTERFACES_H_

#include <cstdint>            // uint8_t
#include "ara/core/vector.h"  // vector

#include "tls/internal/messages/alert_message.h"
#include "tls/internal/messages/change_cipher_spec_message.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/record_protocol/cipher_suite.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/security_parameters.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief Callback interface used by the record protocol to communicate information to the Handshake protocol.
 * \vprivate This class is component private.
 */
class RecordToHandshakeCallbackInterface {
 public:
  /*!
   * \brief Default constructible.
   */
  RecordToHandshakeCallbackInterface() = default;

  RecordToHandshakeCallbackInterface(RecordToHandshakeCallbackInterface const&) = delete;
  RecordToHandshakeCallbackInterface(RecordToHandshakeCallbackInterface&&) = delete;

  auto operator=(RecordToHandshakeCallbackInterface const&) -> RecordToHandshakeCallbackInterface& = delete;
  auto operator=(RecordToHandshakeCallbackInterface&&) -> RecordToHandshakeCallbackInterface& = delete;

  /*!
   * \brief Virtual destructor.
   */
  virtual ~RecordToHandshakeCallbackInterface() noexcept = default;

  /*!
   * \brief Callback for when record protocol sends a request for closing the connection.
   * \param[in,out] cxt The TlsCommFlowContext containing context information about what happened.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void CloseRequest(common::TlsCommFlowContext& cxt) noexcept = 0;

  /*!
   * \brief Callback for when data has been received that is part of a handshake message.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The buffer containing the received data.
   * \param[in] retransmit Set to true if DTLS handshake aggregator finds this a retransmit.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-HandleHandshakeProtocolMessages
   */
  virtual void OnHandshakeData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer,
                               bool retransmit) noexcept = 0;

  /*!
   * \brief Callback for when data has been received that is part of an alert message.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The buffer containing the received data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages
   */
  virtual void OnAlertData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer) noexcept = 0;

  /*!
   * \brief Callback for when data has been received that is part of a change cipher spec message.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The buffer containing the received data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-HandleChangeCipherSpecProtocolMessages
   */
  virtual void OnChangeCipherData(common::TlsCommFlowContext& cxt,
                                  ara::core::Vector<std::uint8_t>& buffer) noexcept = 0;
};

/*!
 * \brief Callback interface used by the record protocol to communicate with the transport layer and the application
 *        layer.
 * \vprivate This class is component private.
 */
class RecordToTlsUserCallbackInterface {
 public:
  /*!
   * \brief Default Constructor.
   */
  RecordToTlsUserCallbackInterface() noexcept = default;

  RecordToTlsUserCallbackInterface(RecordToTlsUserCallbackInterface const&) = delete;
  RecordToTlsUserCallbackInterface(RecordToTlsUserCallbackInterface&&) noexcept = delete;

  auto operator=(RecordToTlsUserCallbackInterface const&) -> RecordToTlsUserCallbackInterface& = delete;
  auto operator=(RecordToTlsUserCallbackInterface&&) noexcept -> RecordToTlsUserCallbackInterface& = delete;

  /*! \brief Destroy the Record To Tls User Callback Interface object */
  virtual ~RecordToTlsUserCallbackInterface() noexcept = default;

  /*!
   * \brief The callback used by the record protocol to send data to the transport layer, in most cases this would
   * be the network layer.
   * \param[in] buffer The buffer containing the data to send.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void OnSendToTransport(ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief The callback used by the record protocol to send data to the application layer, in most cases this would
   * be implemented by the receiving application that wants to use or pass on the data.
   * \param[in] buffer The buffer containing the data received.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-HandleApplicationDataProtocolMessages
   */
  virtual void OnSendToCommParty(ReadOnlyBufferView buffer) noexcept = 0;
};

/*!
 * \brief The HandshakeLayerEventInterface defines API's related to events which occur during a handshake and need to
 *        be communicated with the upper record protocol layer.
 */
class HandshakeLayerEventInterface {
 public:
  /*!
   * \brief Default constructor
   */
  HandshakeLayerEventInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   */
  virtual ~HandshakeLayerEventInterface() noexcept = default;

  HandshakeLayerEventInterface(HandshakeLayerEventInterface const&) noexcept = delete;
  HandshakeLayerEventInterface(HandshakeLayerEventInterface&&) noexcept = delete;

  auto operator=(HandshakeLayerEventInterface const&) noexcept -> HandshakeLayerEventInterface& = delete;
  auto operator=(HandshakeLayerEventInterface&&) noexcept -> HandshakeLayerEventInterface& = delete;

  /*!
   * \brief Request from the record protocol layer to reset the current security parameters.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void ResetSecurityParameters() noexcept = 0;

  /*!
   * \brief Request from the record protocol layer to immediately accept new read security parameters.
   * \details Errors are propagated to the caller, check the status of the TlsCommFlowContext
   * \param[in,out] cxt The TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] sp The SecurityParameters to use for subsequent read operations.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6561835, SPEC-6477473
   */
  virtual void SetReadSecurityParameters(common::TlsCommFlowContext& cxt, SecurityParameters const& sp) noexcept = 0;

  /*!
   * \brief Request from the record protocol layer to immediately accept new write security parameters.
   * \details Errors are propagated to the caller, check the status of the TlsCommFlowContext
   * \param[in,out] cxt The TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] sp The SecurityParameters to use for subsequent write operations.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6561835, SPEC-6477473
   */
  virtual void SetWriteSecurityParameters(common::TlsCommFlowContext& cxt, SecurityParameters const& sp) noexcept = 0;

  /*!
   * \brief Signals the record protocol layer to increase the write-epoch by one.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562007, SPEC-6562008
   */
  virtual void IncreaseWriteEpoch() noexcept = 0;

  /*!
   * \brief Signals the record protocol layer to increase the read-epoch by one.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562007, SPEC-6562008
   */
  virtual void IncreaseReadEpoch() noexcept = 0;

  /*!
   * \brief Signals the record protocol layer to decrease the write-epoch by one.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562007
   */
  virtual void DecreaseWriteEpoch() noexcept = 0;

  /*!
   * \brief Signals the record protocol layer to decrease the read-epoch by one.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562007
   */
  virtual void DecreaseReadEpoch() noexcept = 0;

  /*!
   * \brief Request from the record protocol layer to immediately send a HelloVerifyRequest message without encryption
   *        and without any compression.
   * \details Errors are propagated to the caller of this method via the TlsCommFlowContext
   * \param[in,out] cxt The TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] buffer The Hello Verify Request handshake message to send.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void SendHelloVerifyRequest(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief Request from record protocol layer to set the current DTLS sequence number to the sequence number of the
   *        most recent client hello message.
   * \details Should only be called from a DTLS server with cookie verification enabled prior to sending ServerHello.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562061
   */
  virtual void UseMostRecentClientHelloSequenceNumber() noexcept = 0;

  /*!
   * \brief On event callback for when a cipher suite is selected by the handshake procedure.
   * \param[in] cipher_suite The selected cipher suite.
   * \context Handshake
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void OnCipherSuiteSelected(SupportedCipherSuites const cipher_suite) noexcept = 0;

  /*!
   * \brief Gets a reference to the currently selected cipher suite.
   * \return The current cipher suite.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual auto AccessCurrentCipherSuite() noexcept -> CipherSuite& = 0;
};

/*!
 * \brief The Record Protocol interface, which is implemented by the record protocol. This interface is just
 * a bunch of virtual functions which can be mocked in testing of layers communicating with the record protocol.
 * \vprivate This class is component private.
 */
class RecordProtocolInterface : public HandshakeLayerEventInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  RecordProtocolInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   */
  ~RecordProtocolInterface() noexcept override = default;

  RecordProtocolInterface(RecordProtocolInterface const&) noexcept = delete;
  RecordProtocolInterface(RecordProtocolInterface&&) noexcept = delete;
  auto operator=(RecordProtocolInterface const&) noexcept -> RecordProtocolInterface& = delete;
  auto operator=(RecordProtocolInterface&&) noexcept -> RecordProtocolInterface& = delete;

  /*!
   * \brief Send a buffer via the registered RecordToTlsUserCallbackInterface using
   * OnSendToTransport callback using current write SecurityParameters.
   * \details Sending the messages of types AlertMessage is handled by this function.
   * \param[in,out] cxt TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] buffer The serialized message to be sent.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void HandleOutgoingAlertMessage(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief Compress, encrypt and send a buffer via the registered RecordToTlsUserCallbackInterface using
   * OnSendToTransport callback using current write SecurityParameters.
   * \details Sending the messages of types AlertMessage, HandshakeMessage, ChangeSpecCipherMessage and CommMessage is
   * handled by this function.
   * \param[in,out] cxt TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] buffer The serialized message to be sent.
   * \param[in] type The type of the message.
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is reported to the TlsCommFlowContext if the data buffer
   * is empty and the content type is not application data
   * \error TlsCommErrorCode::kTlsErrInternal is reported to the TlsCommFlowContext if the current
   * MTU configuration is less than or equal to the calculated overhead for record expansion, or the current
   * size of the send buffer exceeds the configured MTU.
   * \reentrant FALSE
   * \trace DSGN-SecCom-TransportServices, SPEC-6561834, SPEC-6493108, DSGN-SecCom-HandleRecordProtocolMessages,
   * SPEC-6561980
   * \trace SPEC-6561829, DSGN-SecCom-HandleChangeCipherSpecProtocolMessages
   * \trace DSGN-SecCom-HandleAlertProtocolMessages
   * \trace SPEC-6562068, SPEC-6562069, SPEC-8792579
   */
  virtual void SendMessage(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer,
                           record_protocol::ContentType const type) noexcept = 0;

  /**
   * \brief Prepares an alert message with the provided attributes, logs it, and subsequently sends it.
   * \details This function is provided for convenience, intended to be implemented using SendMessage. It's perfectly
   * valid to send alert messages manually using SendMessage, but it's encouraged to use this function to minimize
   * duplication and maximize consistency.
   *
   * \param[in,out] context the context object used to record various send information.
   * \param[in] alert_level the alert message level.
   * \param[in] alert_description the alert message description.
   *
   * \pre -
   *
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  virtual void SendAlertMessage(common::TlsCommFlowContext& context, messages::AlertLevel const& alert_level,
                                messages::AlertDescription const& alert_description) noexcept = 0;

  /*!
   * \brief Deserialize, decrypt and forward data received to the correct callback using current
   * read SecurityParameters.
   * \details Errors that could not be handled locally are propagated to the caller of this method via
   * TlsCommFlowContext
   * \param[in,out] cxt The TlsCommFlowContext used for tracing of code logic and error reporting.
   * The caller to this method must evaluate the state of the TlsCommFlowContext when it has returned.
   * \param[in] buffer
   * The buffer containing the input data.
   * \context ANY
   * \pre This method must not be called if the TlsCommFlowContext already has an error
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562042, SPEC-6562041, SPEC-6562043, SPEC-6477467, SPEC-8799976
   * \trace DSGN-SecCom-HandleApplicationDataProtocolMessages, DSGN-SecCom-HandleRecordProtocolMessages
   */
  virtual void HandleReceivedDataFromTransport(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief Register a new callback interface to use when communicating with the handshake layer.
   * \param[in] handshake_callback The interface containing implementations of the RecordToHandshakeCallbackInterface.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void RegisterHandshakeCallback(RecordToHandshakeCallbackInterface* handshake_callback) noexcept = 0;

  /*!
   * \brief Register a new callback interface to use when communicating with the application and
   * transport layers.
   * \param[in] tls_user_callback The interface containing implementation of the RecordToTlsUserCallbackInterface.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void RegisterTlsUserCallback(RecordToTlsUserCallbackInterface* tls_user_callback) noexcept = 0;

  /*!
   * \brief Tell the record layer to disconnect, meaning that it will disallow, the sending and receiving of
   * application layer messages.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void Disconnect() noexcept = 0;

  /*!
   * \brief Tell the record layer to connect, meaning that it will allow the sending and receiving of
   * application layer messages.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void Connect() noexcept = 0;

  /*!
   * \brief Cleanup any resources used by the record protocol for the current connection. This will reset the
   * record protocol to a state where a new connection can safely be set up.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void Cleanup() noexcept = 0;
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_INTERFACES_H_
