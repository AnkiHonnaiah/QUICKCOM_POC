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
/*!        \file  record_protocol.h
 *         \brief This file contains the declaration of constants, functions and classes that are
 *                implemented and used in record_protocol_central.cpp,
 *                record_protocol_crypto.cpp,
 *                record_protocol_receive.cpp and
 *                record_protocol_send.cpp files.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_H_

#include <cstddef>            // size_t
#include <cstdint>            // uint8_t, uint16_t, uint64_t
#include <functional>         // reference_wrapper, ref
#include <memory>             // unique_ptr, make_unique
#include "ara/core/vector.h"  // vector

#include "amsr/core/optional.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/messages/alert_message.h"
#include "tls/internal/messages/change_cipher_spec_message.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/record_protocol/bulk_encryption/aes_gcm_mac.h"
#include "tls/internal/record_protocol/bulk_encryption/bulk_encryption_interface.h"
#include "tls/internal/record_protocol/bulk_encryption/null_mac.h"
#include "tls/internal/record_protocol/bulk_encryption/null_null.h"
#include "tls/internal/record_protocol/cipher_suite.h"
#include "tls/internal/record_protocol/compression_suites/compression_suite_interface.h"
#include "tls/internal/record_protocol/compression_suites/null_compression.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"
#include "tls/internal/record_protocol/record_protocol_framer.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/internal/record_protocol/security_parameters.h"
#include "tls/internal/record_protocol/send_fragmenter.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class CT__TlsCommProviders_ServerStateConnectedOnHandshakeMessageReceived_Test;
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class CT__TlsCommProviders__OnHandshakeMessageInEncryptedServerState_OnHandshakeMessageInEncryptedServerState_Test;
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class CT__TlsCommProviders__OnHandshakeMessageInEncryptedClientState_OnHandshakeMessageInEncryptedClientState_Test;

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief An implementation of the RecordProtocolInterface according to the TLS (https://tools.ietf.org/pdf/rfc5246.pdf)
 * and DTLS (https://tools.ietf.org/pdf/rfc6347.pdf) specifications.
 * \trace SPEC-6493261, SPEC-6477472
 * \vprivate This class is component private.
 */
class RecordProtocol final : public RecordProtocolInterface {
 public:
  /*!
   * \brief Construct a new RecordProtocol object
   * \param[in] config A TLS configuration.
   * \param[in] crypto_adapter A crypto adapter.
   * \param[in] psk_config The pre-shared key configuration.
   */
  RecordProtocol(tls::TlsCommConfigurationInformation const& config,
                 tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter,
                 config::PskConfig const& psk_config) noexcept;

  RecordProtocol(RecordProtocol const&) noexcept = delete;
  RecordProtocol(RecordProtocol&&) noexcept = delete;

  auto operator=(RecordProtocol const&) noexcept -> RecordProtocol& = delete;
  auto operator=(RecordProtocol&&) noexcept -> RecordProtocol& = delete;

  /*! \brief Destroy the RecordProtocol object */
  ~RecordProtocol() noexcept final = default;

  // Life cycle methods for an existing instance
  /*!
   * \brief Make RecordProtocol instance ready for use. Sets role of RecordProtocol
   * \param[in] role_to_take TLS or DTLS.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-Rfcs
   * \vprivate This method is component private.
   */
  void OpenForAction(EnTlsCommRole role_to_take) noexcept;

  /*!
   * \brief This operation will make RecordProtocol instance ready to close down and resets
   * important variables to default values
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-CloseConnection
   * \vprivate This method is component private.
   */
  void CloseDown() noexcept;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::RegisterHandshakeCallback
   * \vprivate This method is component private.
   */
  void RegisterHandshakeCallback(RecordToHandshakeCallbackInterface* handshake_callback) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::RegisterTlsUserCallback
   * \vprivate This method is component private.
   */
  void RegisterTlsUserCallback(RecordToTlsUserCallbackInterface* tls_user_callback) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::Disconnect
   * \vprivate This method is component private.
   */
  void Disconnect() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::Connect
   * \vprivate This method is component private.
   */
  void Connect() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::Cleanup
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::HandleOutgoingAlertMessage
   * \vprivate This method is component private.
   */
  void HandleOutgoingAlertMessage(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::SendMessage
   * \vprivate This method is component private.
   * \trace  SPEC-6561844, SPEC-6561843, SPEC-6561828
   */
  void SendMessage(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer, ContentType const type) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::SendAlertMessage
   * \vprivate This method is component private.
   */
  void SendAlertMessage(common::TlsCommFlowContext& context, messages::AlertLevel const& alert_level,
                        messages::AlertDescription const& alert_description) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::HandleReceivedDataFromTransport
   * \vprivate This method is component private.
   * \trace SPEC-8799955, SPEC-6477467, SPEC-6493126, SPEC-8799956, SPEC-8799976, SPEC-6562009, SPEC-6562010
   * \trace SPEC-6562015, SPEC-6562033, SPEC-6562034, SPEC-6562040, SPEC-6562044, SPEC-6562045, SPEC-6562073
   * \trace SPEC-6562074, SPEC-6562085, SPEC-6562820, SPEC-6181136
   */
  void HandleReceivedDataFromTransport(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::ResetSecurityParameters
   * \vprivate This method is component private.
   */
  void ResetSecurityParameters() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::SetReadSecurityParameters
   * \vprivate This method is component private.
   */
  void SetReadSecurityParameters(common::TlsCommFlowContext& cxt, SecurityParameters const& sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::SetWriteSecurityParameters
   * \vprivate This method is component private.
   * \trace SPEC-6562007
   */
  void SetWriteSecurityParameters(common::TlsCommFlowContext& cxt, SecurityParameters const& sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::IncreaseWriteEpoch
   * \vprivate This method is component private.
   * \trace SPEC-6562006, SPEC-6562007, SPEC-6562008
   */
  void IncreaseWriteEpoch() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::IncreaseReadEpoch
   * \vprivate This method is component private.
   * \trace SPEC-6562006, SPEC-6562007, SPEC-6562008
   */
  void IncreaseReadEpoch() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::DecreaseWriteEpoch
   * \vprivate This method is component private.
   */
  void DecreaseWriteEpoch() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::DecreaseReadEpoch
   * \vprivate This method is component private.
   */
  void DecreaseReadEpoch() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::UseMostRecentClientHelloSequenceNumber
   * \vprivate This method is component private.
   */
  void UseMostRecentClientHelloSequenceNumber() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::OnCipherSuiteSelected
   * \vprivate This method is component private.
   */
  void OnCipherSuiteSelected(SupportedCipherSuites const cipher_suite) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::RecordProtocolInterface::AccessCurrentCipherSuite
   * \vprivate This method is component private.
   */
  auto AccessCurrentCipherSuite() noexcept -> CipherSuite& final;

  /*!
   * \brief Set the TlsType for the record protocol
   * \param[in] type The TLSType to run on, IE TLSType::TLS or TLSType::DTLS
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  void SetTlsType(common::TlsType type) noexcept;

 private:
  /*!
   * \brief HandleReceivedDataFromTransport helper function processing TLS records
   * \param[in] cxt The context.
   * \param[in] rp_context The current RecordProtocolContext
   * \param[in] record A record to process
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void HandleReceivedDataFromTransportTLS(common::TlsCommFlowContext& cxt, RecordProtocolContext const rp_context,
                                          Record const& record) noexcept;

  /*!
   * \brief HandleReceivedDataFromTransport helper function processing DTLS records
   * \param[in] cxt The context.
   * \param[in] rp_context The current RecordProtocolContext
   * \param[in] record A record to process
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void HandleReceivedDataFromTransportDTLS(common::TlsCommFlowContext& cxt, RecordProtocolContext const rp_context,
                                           Record const& record) noexcept;

  /*! \brief The RecordToHandshakeCallbackInterface to use when communicating with the handshake layer. */
  RecordToHandshakeCallbackInterface* record_to_handshake_callback_{nullptr};

  /*!
   * \brief The RecordToTlsUserCallbackInterface to use when communicating with the application
   * or transport layer.
   */
  RecordToTlsUserCallbackInterface* record_to_user_callback_{nullptr};

  /*!
   * \brief SendMessage helper function processing plain text records
   * \param[in] type The context type.
   * \param[in,out] send_cxt TlsCommFlowContext recording contextual information about the send operation.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SendMessagePlainTextProcessor(ContentType const type, common::TlsCommFlowContext& send_cxt) noexcept;

  /*!
   * \brief Compress, encrypt and send a HelloVerifyRequest handshake message via the registered
   * RecordToTlsUserCallbackInterface.
   * \param[in,out] cxt TlsCommFlowContext recording contextual information about the send operation.
   * \param[in] buffer The HelloVerifyRequest handshake message to send.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SendHelloVerifyRequest(common::TlsCommFlowContext& cxt, ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \brief Triggers the correct receive callback function depending on the ContentType of the
   * Plaintext object.
   * \details Errors that are reported by callback functions are propagated to the caller of this method
   * \param[in,out] cxt The TlsCommFlowContext where context information about the operation can be
   * recorded.
   * \param[in] plain_text A unique pointer to the Plaintext record whose data will be sent to the callback
   * determined by the ContentType in the Plaintext.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-SecCom-TransportServices, SPEC-6561840, DSGN-SecCom-HandleRecordProtocolMessages, SPEC-6562011
   */
  void TriggerCallback(common::TlsCommFlowContext& cxt,
                       std::unique_ptr<record_protocol::PlainText> plain_text) noexcept;

  /*!
   * \brief Updates the sliding window if MAC verification succeeded for a given record
   * \param[in] context A const reference to the current RecordProtocolContext
   * \context ANY
   * \pre Called only in DTLS mode
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562043
   */
  void UpdateSlidingWindow(RecordProtocolContext const& context) noexcept;

  /*!
   * \brief Updates the sequence counters after sending a message.
   * \param[in,out] cxt The TlsCommFlowContext where context information about the operation can be
   * recorded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void PostSendUpdate(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Calculates the current record size limit.
   *
   * \details Uses MTU and/or record_size_limit TLS extension if present.
   *
   * \param[in, out] flow_context The flow context object used to track errors.
   * \return The calculated record size; or a null optional if an error occurs.
   *
   * \pre -
   *
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto CalcCurrentRecordSizeLimit(common::TlsCommFlowContext& flow_context) const noexcept
      -> amsr::core::Optional<std::size_t>;

  /*!
   * \brief Gets the record expansion for the current security parameters, mode and cipher suite.
   * \param[in] sp The security parameters
   * \param[in] mode The mode, TLS or DTLS.
   * \param[in] cipher_suite The current cipher suite.
   * \return The record expansion.
   */
  static auto CalcRecordExpansion(SecurityParameters const& sp, common::TlsType const& mode,
                                  SupportedCipherSuites const& cipher_suite) noexcept -> std::uint16_t;

  /*!
   * \brief A TLS configuration.
   */
  tls::TlsCommConfigurationInformation const config_;

  /*!
   * \brief Reference to the current crypto adapter.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter_;

  // Configuration options for changing the Record Protocol
  /*! \brief The mode in which the record protocol is operating, either DTLS or TLS mode */
  common::TlsType tls_type_{common::TlsType::kTls};

  /*! \brief The size of the record protocol header */
  uint8_t header_size_{0U};

  /*! \brief The bool value to decide if application is allowed */
  bool allow_application_{false};

  /*! \brief Buffer to be send */
  ara::core::Vector<uint8_t> send_buffer_{};

  /*! \brief Serialized buffer to be send */
  ara::core::Vector<uint8_t> send_buffer_serialized_{};

  /*! \brief Buffer to be received */
  ara::core::Vector<uint8_t> receive_buffer_{};

  /*!
   * \brief The epoch number corresponding to the current record
   * \details Epoch is updated each time change_cipher is sent/received and therefore read and write epoch is needed
   * since we can still expect to receive on the "old" epoch while transmitting on the new and vice versa.
   * \trace SPEC-6562008
   */
  uint16_t read_epoch_{0U};

  /*!
   * \brief The epoch number corresponding to the current record
   * \details Epoch is updated each time change_cipher is sent/received and therefore read and write epoch is needed
   * since we can still expect to receive on the "old" epoch while transmitting on the new and vice versa.
   * \trace SPEC-6562008
   */
  uint16_t write_epoch_{0U};

  /*! \brief The sequence number under DTLS mode */
  ara::core::Vector<uint64_t> sequence_number_dtls_{0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};

  /*! \brief The sequence number under TLS mode */
  uint64_t sequence_number_tls_{0U};

  /*! \brief A read sequence counter */
  uint64_t read_sequence_counter_{0U};

  /*! \brief Current read security parameters */
  amsr::core::Optional<SecurityParameters> current_read_security_params_{amsr::core::nullopt};

  /*! \brief Current write security parameters */
  amsr::core::Optional<SecurityParameters> current_write_security_params_{amsr::core::nullopt};

  /*! \brief Tls framer */
  RecordProtocolFramer tls_framer_{kTlsMessageHeaderSize};

  /*! \brief Dtls framer */
  RecordProtocolFramer dtls_framer_{kDtlsMessageHeaderSize};

  /*! \brief Framer wrapper */
  std::reference_wrapper<RecordProtocolFramer> framer_{std::ref(tls_framer_)};

  /*! \brief Tls Fragmenter to send */
  record_protocol::SendFragmenterTls tls_send_fragmenter_{};

  /*! \brief Dtls Fragmenter to send */
  record_protocol::SendFragmenterDtls dtls_send_fragmenter_{};

  /*! \brief Reference wrapper of Fragmenter */
  std::reference_wrapper<record_protocol::SendFragmenter> send_fragmenter_{std::ref(tls_send_fragmenter_)};

  /*! \brief Unique pointer wrapping a CompressionSuiteInterface */
  std::unique_ptr<record_protocol::compression_suites::CompressionSuiteInterface> compression_suite_write_{
      std::make_unique<record_protocol::compression_suites::NullCompression>()};

  /*! \brief Expected Length of Record */
  uint16_t expected_length_of_record_{0U};

  /*! \brief Record expansion */
  uint16_t record_expansion_{0U};

  /*! \brief Record Size Limit (from TLS Extension) */
  std::size_t record_size_limit_{common::kMaxFragmentSize};

  /*!
   * \brief The last highest sequence number that has been verified
   * \details Its value is also the right side for the sliding window
   */
  uint64_t last_seq_{0U};

  /*! \brief Bitmap which keeps track of verified records within the sliding window */
  uint64_t sliding_window_bitmap_{0U};

  /*! \brief The sequence number of the most recent ClientHello message, only used in DTLS for cookie verification */
  uint64_t most_recent_client_hello_seq_nr_{0U};

  /*!
   * \brief The pre-shared key configuration.
   */
  config::PskConfig const& psk_config_;

  /*!
   * \brief The current cipher suite which is selected during a handshake procedure.
   */
  amsr::core::Optional<CipherSuite> current_cipher_suite_{amsr::core::nullopt};

  /*! \brief Friend test declaration for Connect */
  FRIEND_TEST(Cdd__RecordProtocol, Connect);

  /*! \brief Friend test declaration for Disconnect */
  FRIEND_TEST(Cdd__RecordProtocol, Disconnect);

  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(Cdd__RecordProtocol, Cleanup);

  /*! \brief Friend test declaration for SetupCryptoAdapter */
  FRIEND_TEST(Cdd__RecordProtocol, SetupCryptoAdapter);

  /*! \brief Friend test declaration for RecordProtocol */
  FRIEND_TEST(Cdd__RecordProtocol, RecordProtocol);

  /*! \brief Friend test declaration for OpenForAction */
  FRIEND_TEST(Cdd__RecordProtocol, OpenForAction);

  /*! \brief Friend test declaration for OpenForActionNegative */
  FRIEND_TEST(Cdd__RecordProtocol, OpenForActionNegative);

  /*! \brief Friend test declaration for CloseDown */
  FRIEND_TEST(Cdd__RecordProtocol, CloseDown);

  /*! \brief Friend test declaration for SendMessageNegativeOne */
  FRIEND_TEST(Cdd__RecordProtocol, SendMessageNegativeOne);

  /*! \brief Friend test declaration for SendMessageNegativeTwo */
  FRIEND_TEST(Cdd__RecordProtocol, SendMessageNegativeTwo);

  /*! \brief Friend test declaration for TriggerCallbackNegative */
  FRIEND_TEST(Cdd__RecordProtocol, TriggerCallbackNegative);

  /*! \brief Friend test declaration for TriggerCallbackSendAlert */
  FRIEND_TEST(Cdd__RecordProtocol, TriggerCallbackSendAlert);

  /*! \brief Friend test declaration for TriggerCallbackSendAlertWhenChangeCipherMsgIsMalformed */
  FRIEND_TEST(Cdd__RecordProtocol, TriggerCallbackSendAlertWhenChangeCipherMsgIsMalformed);

  /*! \brief Friend test declaration for TriggerCallbackNullFragment */
  FRIEND_TEST(Cdd__RecordProtocol, TriggerCallbackNullFragment);

  /*! \brief Friend test declaration for EncryptDataNegative */
  FRIEND_TEST(Cdd__RecordProtocol, EncryptDataNegative);

  /*! \brief Friend test declaration for DecryptDataNegativeOne */
  FRIEND_TEST(Cdd__RecordProtocol, DecryptDataNegativeOne);

  /*! \brief Friend test declaration for DecryptDataNegativeTwo */
  FRIEND_TEST(Cdd__RecordProtocol, DecryptDataNegativeTwo);

  /*! \brief Friend test declaration for DecryptNegative */
  FRIEND_TEST(Cdd__RecordProtocol, DecryptNegative);

  /*! \brief Friend test declaration for VerifyMacNegative */
  FRIEND_TEST(Cdd__RecordProtocol, VerifyMacNegative);

  /*! \brief Friend test declaration for SendDataNegative */
  FRIEND_TEST(Cdd__RecordProtocol, SendDataNegative);

  /*! \brief Friend test declaration for TriggerCallbackUnitTest */
  FRIEND_TEST(Cdd__RecordProtocol, TriggerCallbackUnitTest);

  /*! \brief Friend test declaration for HandleReceivedDataFromTransportSlidingWindowMechanism */
  FRIEND_TEST(Cdd__RecordProtocol, HandleReceivedDataFromTransportSlidingWindowMechanism);

  /*! \brief Friend test declaration for UpdateSlidingWindow */
  FRIEND_TEST(Cdd__RecordProtocol, UpdateSlidingWindow);

  /*! \brief Friend test declaration for DisconnectOnMaxSequenceNumber */
  FRIEND_TEST(Cdd__RecordProtocol, DisconnectOnMaxSequenceNumber);

  /*! \brief Friend test declaration for DisconnectOnMaxWriteSequenceCounter */
  FRIEND_TEST(Cdd__RecordProtocol, DisconnectOnMaxWriteSequenceCounter);

  /*! \brief Friend test declaration for DisconnectOnMaxWriteSequenceCounter */
  FRIEND_TEST(UT__RecordProtocol, DisconnectOnMaxWriteSequenceCounter);

  /*! \brief Friend test declaration for DisconnectOnMaxWriteSequenceCounter */
  FRIEND_TEST(UT__ClientHelloMessage_Clear_Test, DisconnectOnMaxWriteSequenceCounter);

  /*! \brief Friend test declaration for RegisterCallbacks */
  FRIEND_TEST(Cdd__RecordProtocol, RegisterCallbacks);

  /*! \brief Friend test declaration for SecurityParameters */
  FRIEND_TEST(Cdd__RecordProtocol, SecurityParameters);

  /*! \brief Friend test declaration for UseMostRecentClientHelloSequenceNumber */
  FRIEND_TEST(Cdd__RecordProtocol, UseMostRecentClientHelloSequenceNumber);

  /*! \brief Friend test declaration for ReceiveSingleRecordWithInvalidMacTLS */
  FRIEND_TEST(Cdd__RecordProtocol, ReceiveSingleRecordWithInvalidMacTLS);

  /*! \brief Friend test declaration for ReceiveSingleRecordWithInvalidMacDTLS */
  FRIEND_TEST(Cdd__RecordProtocol, ReceiveSingleRecordWithInvalidMacDTLS);

  /*! \brief Friend test declaration for ReceivingEmptyHandshakeRecordTLS */
  FRIEND_TEST(Cdd__RecordProtocol, ReceivingEmptyHandshakeRecordTLS);

  /*! \brief Friend test declaration for ReceivingEmptyHandshakeRecordDTLS */
  FRIEND_TEST(Cdd__RecordProtocol, ReceivingEmptyHandshakeRecordDTLS);

  /*! \brief Friend test declaration for SendMessageWithNullCompressionSuiteWrite */
  FRIEND_TEST(Cdd__RecordProtocol, SendMessageWithNullCompressionSuiteWrite);

  /*! \brief Friend test declaration for ReadEpoch */
  FRIEND_TEST(Cdd__RecordProtocol, ReadEpoch);

  /*! \brief Friend test declaration for WriteEpoch */
  FRIEND_TEST(Cdd__RecordProtocol, WriteEpoch);

  /*! \brief Friend test declaration to verify record expansion value. */
  FRIEND_TEST(Cdd__RecordProtocol, SecurityParametersRecordExpansion);

  /*! \brief Friend test declaration for Connect */
  FRIEND_TEST(UT__RecordProtocol, Connect);

  /*! \brief Friend test declaration for Disconnect */
  FRIEND_TEST(UT__RecordProtocol, Disconnect);

  /*! \brief Friend test declaration for Cleanup */
  FRIEND_TEST(UT__RecordProtocol, Cleanup);

  /*! \brief Friend test declaration for SetupCryptoAdapter */
  FRIEND_TEST(UT__RecordProtocol, SetupCryptoAdapter);

  /*! \brief Friend test declaration for RecordProtocol */
  FRIEND_TEST(UT__RecordProtocol, RecordProtocol);

  /*! \brief Friend test declaration for OpenForAction */
  FRIEND_TEST(UT__RecordProtocol, OpenForAction);

  /*! \brief Friend test declaration for OpenForActionNegative */
  FRIEND_TEST(UT__RecordProtocol, OpenForActionNegative);

  /*! \brief Friend test declaration for CloseDown */
  FRIEND_TEST(UT__RecordProtocol, CloseDown);

  /*! \brief Friend test declaration for SendMessageNegativeOne */
  FRIEND_TEST(UT__RecordProtocol, SendMessageNegativeOne);

  /*! \brief Friend test declaration for SendMessageNegativeTwo */
  FRIEND_TEST(UT__RecordProtocol, SendMessageNegativeTwo);

  /*! \brief Friend test declaration for TriggerCallbackNegative */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackNegative);

  /*! \brief Friend test declaration for TriggerCallbackSendAlert */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackSendAlert);

  /*! \brief Friend test declaration for TriggerCallbackSendAlertWhenChangeCipherMsgIsMalformed */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackSendAlertWhenChangeCipherMsgIsMalformed);

  /*! \brief Friend test declaration for TriggerCallbackNullFragment */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackNullFragment);

  /*! \brief Friend test declaration for EncryptDataNegative */
  FRIEND_TEST(UT__RecordProtocol, EncryptDataNegative);

  /*! \brief Friend test declaration for DecryptDataNegativeOne */
  FRIEND_TEST(UT__RecordProtocol, DecryptDataNegativeOne);

  /*! \brief Friend test declaration for DecryptDataNegativeTwo */
  FRIEND_TEST(UT__RecordProtocol, DecryptDataNegativeTwo);

  /*! \brief Friend test declaration for DecryptNegative */
  FRIEND_TEST(UT__RecordProtocol, DecryptNegative);

  /*! \brief Friend test declaration for VerifyMacNegative */
  FRIEND_TEST(UT__RecordProtocol, VerifyMacNegative);

  /*! \brief Friend test declaration for SendDataNegative */
  FRIEND_TEST(UT__RecordProtocol, SendDataNegative);

  /*! \brief Friend test declaration for TriggerCallbackUnitTest */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackUnitTest);

  /*! \brief Friend test declaration for HandleReceivedDataFromTransportSlidingWindowMechanism */
  FRIEND_TEST(UT__RecordProtocol, HandleReceivedDataFromTransportSlidingWindowMechanism);

  /*! \brief Friend test declaration for UpdateSlidingWindow */
  FRIEND_TEST(UT__RecordProtocol, UpdateSlidingWindow);

  /*! \brief Friend test declaration for DisconnectOnMaxSequenceNumber */
  FRIEND_TEST(UT__RecordProtocol, DisconnectOnMaxSequenceNumber);

  /*! \brief Friend test declaration for DisconnectOnMaxWriteSequenceCounter */
  FRIEND_TEST(UT__RecordProtocol__DeathTest, DisconnectOnMaxWriteSequenceCounter);

  /*! \brief Friend test declaration for RegisterCallbacks */
  FRIEND_TEST(UT__RecordProtocol, RegisterCallbacks);

  /*! \brief Friend test declaration for SecurityParameters */
  FRIEND_TEST(UT__RecordProtocol, SecurityParameters);

  /*! \brief Friend test declaration for UseMostRecentClientHelloSequenceNumber */
  FRIEND_TEST(UT__RecordProtocol, UseMostRecentClientHelloSequenceNumber);

  /*! \brief Friend test declaration for ReceiveSingleRecordWithInvalidMacTLS */
  FRIEND_TEST(UT__RecordProtocol, ReceiveSingleRecordWithInvalidMacTLS);

  /*! \brief Friend test declaration for ReceiveSingleRecordWithInvalidMacDTLS */
  FRIEND_TEST(UT__RecordProtocol, ReceiveSingleRecordWithInvalidMacDTLS);

  /*! \brief Friend test declaration for ReceivingEmptyHandshakeRecordTLS */
  FRIEND_TEST(UT__RecordProtocol, ReceivingEmptyHandshakeRecordTLS);

  /*! \brief Friend test declaration for ReceivingEmptyHandshakeRecordDTLS */
  FRIEND_TEST(UT__RecordProtocol, ReceivingEmptyHandshakeRecordDTLS);

  /*! \brief Friend test declaration for SendMessageWithNullCompressionSuiteWrite */
  FRIEND_TEST(UT__RecordProtocol, SendMessageWithNullCompressionSuiteWrite);

  /*! \brief Friend test declaration for ReadEpoch */
  FRIEND_TEST(UT__RecordProtocol, ReadEpoch);

  /*! \brief Friend test declaration for WriteEpoch */
  FRIEND_TEST(UT__RecordProtocol, WriteEpoch);

  /*! \brief Friend test declaration to verify record expansion value. */
  FRIEND_TEST(UT__RecordProtocol, SecurityParametersRecordExpansion);

  /*! \brief Friend test declaration for CalcCurrentRecordSizeLimit_high_overhead */
  FRIEND_TEST(UT__RecordProtocol, CalcCurrentRecordSizeLimit_high_overhead);

  /*! \brief Friend test declaration for AccessCurrentCipherSuite */
  FRIEND_TEST(UT__RecordProtocol, AccessCurrentCipherSuite);

  /*! \brief Friend test declaration for AccessCurrentCipherSuite_Abort */
  FRIEND_TEST(UT__RecordProtocol, AccessCurrentCipherSuite_Abort);

  /*! \brief Friend test declaration for ResetSecurityParameters */
  FRIEND_TEST(UT__RecordProtocol, ResetSecurityParameters);

  /*! \brief Friend test declaration for SendHelloVerifyRequest */
  FRIEND_TEST(UT__RecordProtocol, SendHelloVerifyRequest);

  /*! \brief Friend test declaration for SendHelloVerifyRequest_Dtls */
  FRIEND_TEST(UT__RecordProtocol, SendHelloVerifyRequest_Dtls);

  /*! \brief Friend test declaration for SendHelloVerifyRequest_Empty_Buffer */
  FRIEND_TEST(UT__RecordProtocol, SendHelloVerifyRequest_Empty_Buffer);

  /*! \brief Friend test declaration for SendHelloVerifyRequest_ExceedRecordSize */
  FRIEND_TEST(UT__RecordProtocol, SendHelloVerifyRequest_ExceedRecordSize);

  /*! \brief Friend test declaration for HandleOutgoingAlertMessage */
  FRIEND_TEST(UT__RecordProtocol, HandleOutgoingAlertMessage);

  /*! \brief Friend test declaration for DisconnectOnMaxSequenceNumber_Central */
  FRIEND_TEST(UT__RecordProtocol, DisconnectOnMaxSequenceNumber_Central);

  /*! \brief Friend test declaration for TriggerCallbackChangeCipherSpecError */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackChangeCipherSpecError);

  /*! \brief Friend test declaration for TriggerCallbackEmptyText */
  FRIEND_TEST(UT__RecordProtocol, TriggerCallbackEmptyText);

  /*! \brief Friend test declaration for SendSingleRecordEncryptedMaxDataLengthDTLS */
  FRIEND_TEST(UT__RecordProtocol, SendSingleRecordEncryptedMaxDataLengthDTLS);

  /*! \brief Friend test declaration for SendkTlsErrInternal */
  FRIEND_TEST(UT__RecordProtocol, SendkTlsErrInternal);

  /*! \brief Friend test declaration for CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Null_Sha1 */
  FRIEND_TEST(UT__RecordProtocol, CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Null_Sha1);

  /*! \brief Friend test declaration for CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_128_Cbc_Sha256 */
  FRIEND_TEST(UT__RecordProtocol, CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_128_Cbc_Sha256);

  /*! \brief Friend test declaration for CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_256_Gcm_Sha384 */
  FRIEND_TEST(UT__RecordProtocol, CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_256_Gcm_Sha384);

  /*! \brief Friend test declaration for CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_256_Cbc_Sha384 */
  FRIEND_TEST(UT__RecordProtocol, CalcRecordExpansion_Dtls_Ecdhe_Ecdsa_With_Aes_256_Cbc_Sha384);

  /*! \brief Friend test declaration for CalcRecordExpansion_Dtls_Null_With_Null_Null */
  FRIEND_TEST(UT__RecordProtocol, CalcRecordExpansion_Dtls_Null_With_Null_Null);

  /*! \brief Friend test declaration for ServerStateConnectedOnHandshakeMessageReceived */
  FRIEND_TEST(CT__TlsCommProviders, ServerStateConnectedOnHandshakeMessageReceived);

  /*! \brief Friend test declaration for OnHandshakeMessageInEncryptedServerState */
  FRIEND_TEST(CT__TlsCommProviders__OnHandshakeMessageInEncryptedServerState, OnHandshakeMessageInEncryptedServerState);

  /*! \brief Friend test declaration for OnHandshakeMessageInEncryptedClientState */
  FRIEND_TEST(CT__TlsCommProviders__OnHandshakeMessageInEncryptedClientState, OnHandshakeMessageInEncryptedClientState);
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_H_
