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
/*!        \file
 *         \brief  This file contains the declaration of HandshakeStateMachine class.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_H_

#include <cstdint>
#include <functional>
#include <memory>
#include "ara/core/vector.h"

#include "amsr/timer/timer_manager.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/common/timer_extension.h"
#include "tls/internal/handshakestatemachine/client_handshake_state_pool.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_message_aggregator.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/internal/handshakestatemachine/handshake_state_pool_interface.h"
#include "tls/internal/handshakestatemachine/handshake_to_user_callback_interface.h"
#include "tls/internal/handshakestatemachine/server_handshake_state_pool.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/internal/record_protocol/security_parameters.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief The state machine that controls the flow of the handshake negotiation.
 * \details It defines what state is the current state, and forwards incoming messages etc, to the current state. This
 * class is used for both client and server as well as for TLS and DTLS. It implements the
 * RecordToHandshakeCallbackInterface, to ensure the record layer can forward any handshake data or error to it.
 * \trace DSGN-SecCom-TrackHandshakeSteps
 * \vprivate This class is component private.
 */
class HandshakeStateMachine : public HandshakeStateMachineInterface {
 public:
  /*!
   * \brief Construct a new HandshakeStateMachine object
   * \param[in] config The current config.
   * \param[in] psk_config The pre-shared key configuration.
   * \param[in] crypto_adapter The crypto adapter to use.
   * \param[in] handshake_msg_mgr Reference to the MessageManager that should be used for this handshake state machine.
   * \param[in] record_protocol Reference to a RecordProtocolInterface used when setting
   * encryption parameters and sending messages
   * \param[in] user_callbacks Reference to an object implementing the
   * HandshakeToUserCallbackInterface which is used for callback towards the user of the TLS library. For example
   * OnConnect() which is called when the handshake has successfully negotiated an encrypted connection and is ready
   * for application data.
   * \reentrant FALSE
   */
  HandshakeStateMachine(tls::TlsCommConfigurationInformation const config, config::PskConfig const& psk_config,
                        tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter,
                        messages::MessageManager& handshake_msg_mgr,
                        record_protocol::RecordProtocolInterface& record_protocol,
                        HandshakeToUserCallbackInterface& user_callbacks) noexcept;

  HandshakeStateMachine(HandshakeStateMachine const&) = delete;
  HandshakeStateMachine(HandshakeStateMachine&&) = delete;
  HandshakeStateMachine& operator=(HandshakeStateMachine const&) = delete;
  HandshakeStateMachine& operator=(HandshakeStateMachine&&) = delete;

  /*! \brief Destroy the HandshakeStateMachine object */
  ~HandshakeStateMachine() noexcept override;

  /*! \brief Type alias for TLS extension value */
  using TlsExtensionTypeValue = HandshakeStateMachineInterface::TlsExtensionTypeValue;

  /*!
   * \brief This operation performs initialization activities that is separated from the constructor.
   * \details The operation is called once when the object is created. The object is part of a pooled structure where
   * the structure is created during the initialization of the library and the internal pool. After that, the operations
   * OpenForAction() and CloseDown() are used to control the lifecycle.
   * \param[in] role_to_take This is the role to be used by this instance of the provider.Client or Server is the only
   * trait considered in this call, not TLS/DTLS which can be changed in the OpenForAction() call.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void CreateInstance(EnTlsCommRole const role_to_take) noexcept override;

  /*!
   * \brief Makes the HandshakeStateMachine instance ready for use. It is called when fetched from the pool.
   * \details This operation must be called before any other operation belonging to the instance is used. Will make
   * necessary initiations needed for its functionality. It will also call corresponding operations in objects contained
   * by this object.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * This parameter includes two types of information. 1) Traceability data which shall be set by the caller and
   * used for internal logging activities during the execution of a flow and 2) error information that shall be set
   * in case of an error occurring during the flow. If the caller is interested in the error state after the call,
   * the caller should check for errors using the has_error variable. See TlsCommFlowContext for more information.
   * \param[in] role_to_take This is the role to be used by this instance of the provider. The different roles are
   * defined in EnTlsCommRole.
   * TLS/DTLS is the only trait considered in this call, Client/Server has to be decided on the CreateInstance() call.
   * \param[in] starting_state_handle
   * The state to start in (Normally ClientStateDisconnected/ServerStateDisconnected)
   * \param[in] crypto_suites_to_use
   * Defines the crypto suites which shall be supported by this instance of the class. The parameter is a list
   * of uint16_t values in priority order where each value corresponds to a certain crypto suite. See the
   * ListOfCryptoSuites for more details.
   * \param[in] timer_manager
   * This parameter provides the timer manager object that should be used by this instance of the communication
   * provider.
   * The timer manager is used internally to register various timers which will handle timeouts when needed.
   * The timer manager  and the calls to registered timeout callbacks are handled by the reactor thread when a
   * timeout occurs.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-CipherService
   * \vprivate This method is component private.
   */
  void OpenForAction(common::TlsCommFlowContext& cxt, EnTlsCommRole role_to_take,
                     HandshakeStateHandle starting_state_handle, ListOfCipherSuites& crypto_suites_to_use,
                     internal::common::TlsTimerManager timer_manager) noexcept override;

  /*!
   * \brief This operation will close down the HandshakeStateMachine instance and return dynamic resources.
   * \details After the call to this operation, the object shall not be used. It will be returned to the pool as part of
   * the structure. The operation will call corresponding operations on objects contained by this instance. The purpose
   * of the operation is to return all allocated resources, clear relevant variables and return to a state which will
   * allow a call to OpenForAction().
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void CloseDown() noexcept override;

  /*!
   * \brief Resets the HandshakeStateMachine into a clean state, resetting any resource used for the
   * last connection, and makes it ready to be reused.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept override;

  /*!
   * \brief Get a constant reference to the currently active HandshakeStateHandle.
   * \return A constant reference of the current HandshakeStateHandle.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  HandshakeStateHandle GetState() const noexcept override;

  /*!
   * \brief Get a constant reference to the currently related HandshakeStatePool.
   * \return A constant reference of the current HandshakeStatePool.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  HandshakeStatePoolInterface& GetStatePool() const noexcept override;

  /*!
   * \brief Get a reference to the current crypto adapter.
   * \return Reference to the current crypto adapter.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface& GetCryptoAdapter() const noexcept override;

  /*!
   * \brief Get the current TLS configuration.
   * \return Reference to the current configuration.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  tls::TlsCommConfigurationInformation const& GetConfig() const noexcept override;

  /*!
   * \brief Initiates a TLS or DTLS handshake. Can only be called if the current state is disconnected.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void InitiateHandshake(common::TlsCommFlowContext& cxt) noexcept override;

  /*!
   * \brief Disconnects from the currently active TLS or DTLS connection. Can only be called if the current state is
   * connected.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] send_close_notify "true" if a "CloseNotify" alert message must be sent before closing the connection
   *            and "false" otherwise.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561837
   * \vprivate This method is component private.
   */
  void Disconnect(common::TlsCommFlowContext& cxt, bool const send_close_notify) noexcept override;

  /*!
   * \brief Gets the next SecurityParameters i.e. the SecurityParameters that are currently being negotiated.
   * \return SecurityParameters& A reference to the currently negotiated SecurityParameters.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  record_protocol::SecurityParameters& GetNextSecurityParameters() noexcept override;

  /*!
   * \brief Get the master secret container.
   * \return A reference to the master secret container.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  tls_crypto_adapter::internal::MasterSecretContainer& GetMasterSecretContainer() noexcept override;

  /*!
   * \brief Gets a reference of the handshake message aggregator
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \return HandshakeMessageAggregator& Reference to the handshake message aggregator
   * \vprivate This method is component private.
   */
  HandshakeMessageAggregator& GetHandshakeMessageAggregator() noexcept override;

  /*!
   * \brief Get the verification secret.
   * \return The verification secret.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  std::uint32_t GetVerificationSecret() const noexcept override;

  /*!
   * \brief Set the verification secret.
   * \param[in] new_value New value to use.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SetVerificationSecret(std::uint32_t const new_value) noexcept override;

  /*!
   * \brief Check if cookie verification is enabled or not.
   * \return "true" is enabled and "false" otherwise.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  bool IsCookieVerificationEnabled() const noexcept override;

  /*!
   * \brief Set the state of cookie verification.
   * \param[in] enabled "true" to enable and "false" otherwise.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SetCookieVerificationEnabled(bool enabled) noexcept override;

  /*!
   * \brief Callback method implemented from the RecordToHandshakeCallbackInterface, which is
   * called wherever the record protocol sends a close connection request.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void CloseRequest(common::TlsCommFlowContext& cxt) noexcept override;

  /*!
   * \brief Callback method implemented from the RecordToHandshakeCallbackInterface, which is
   * called whenever there is handshake message available from the record protocol.
   * \details Potential errors will be propagated to the caller of this function. Please check the status of the flow
   * context after calling this function.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The data that arrived.
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnHandshakeData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer,
                       bool retransmit) noexcept override;

  /*!
   * \brief Callback method implemented from the RecordToHandshakeCallbackInterface, which is
   * called whenever there is alert message available from the record protocol.
   * \details Potential errors will be propagated to the caller of this function. Please check the status of the flow
   * context after calling this function.
   * \param[in,out] cxt TlsCommFlowContext to log contextual information
   * \param[in] buffer The data that arrived.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnAlertData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer) noexcept override;

  /*!
   * \brief Callback method implemented from the RecordToHandshakeCallbackInterface, which is
   * called whenever there is a change cipher message available from the record protocol.
   * \details Potential errors will be propagated to the caller of this function. Please check the status of the flow
   * context after calling this function.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The data that arrived.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer) noexcept override;

  /*!
   * \brief Called when timer expires.
   * \return True if the timer should be scheduled again. Otherwise, false. Returning true has no effect if the timer is
   * not periodic.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  bool OnTimerEvent() noexcept override;

  /*!
   * \brief Sets the timer to trigger once after N milliseconds
   * \param[in] duration_ms Duration after timer triggers in milliseconds.
   * \param[in] stop_timer Flag specifying if the timer should be stopped before setting to one shot or not.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetOneShotTimer(std::uint32_t const duration_ms, bool const stop_timer) const noexcept override;

  /*!
   * \brief Callback method implemented from the DiagnosticsInterface, which is
   * called during the handshake to get the information about TLS extensions.
   * \param[in] extensions Specifies the Tls extension type value.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void ReportTlsExtensions(ara::core::Vector<TlsExtensionTypeValue> extensions) noexcept override;

  /*!
   * \brief Get the role.
   * \return The role.
   */
  EnTlsCommRole GetRole() const noexcept override;

  /*!
   * \brief Stop the timer.
   */
  void StopTimer() noexcept override;

  /*!
   * \brief Get the PSK UUID corresponding to the given PSK identity and hint.
   * \param[in] id The PSK identity the UUID shall be fetched for.
   * \param[in] hint The PSK identity hint the UUID shall be fetched for.
   * \param[in] is_server True if this communication provider is a server.
   * \return The PSK UUID corresponding to the PSK identity and hint. The zero UUID if no UUID can be found.
   * \context Any
   * \pre -
   * \threadsafe True
   * \reentrant False
   * \synchronous True
   * \vprivate This function is component private.
   */
  ara::crypto::Uuid GetPskUuid(amsr::core::StringView id, amsr::core::StringView hint,
                               bool is_server) const noexcept override;

  /*!
   * \brief Checks the context for errors.
   * \details If an error is found, an appropriate alert message is sent and the state machine transitions into
   * Disconnected state without further error checking (in OnExit of old state and OnEnter of Disconnected)
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561860, SPEC-8799970, SPEC-8799973, SPEC-6477467, SPEC-6561971, SPEC-6561864
   */
  void HandleError(common::TlsCommFlowContext& cxt) noexcept override;

 private:
  /*!
   * \brief A TLS configuration.
   */
  tls::TlsCommConfigurationInformation const config_;

  /*!
   * \brief The pre-shared key configuration.
   */
  config::PskConfig const& psk_config_;

  /*!
   * \brief Reference to the current crypto adapter.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter_;

  /*! \brief The pool containing instances of all the required states for client state machine. Either this or
   * ServerHandshakeStatePool is used. */
  ClientHandshakeStatePool clientHandshakeStatePool{};

  /*! \brief The pool containing instances of all the required states for server state machine. Either this or
   * ClientHandshakeStatePool is used. */
  ServerHandshakeStatePool serverHandshakeStatePool{};

  /*! \brief A reference to The pool containing instances of each state required, for a HandshakeStateMachine of the
   * current role_. */
  std::reference_wrapper<HandshakeStatePoolInterface> handshake_state_pool_{clientHandshakeStatePool};

  /*! \brief An Object of HandshakeStateHandle which holds the respective current state */
  HandshakeStateHandle current_state_{HandshakeStateHandle::kClientStateDisconnected};

  /*! \brief The pool containing instances of all messages required for a handshake. */
  messages::MessageManager& handshake_message_manager_;

  /*! \brief  A pointer to the record protocol layer, which is used to send data to the transport layer. */
  record_protocol::RecordProtocolInterface& record_protocol_;

  /*! \brief A pointer to the user callback registered for communication with the application layer. */
  HandshakeToUserCallbackInterface& user_callbacks_;

  /*!
   * \brief The message aggregator
   * \details Ensures that the data received by the record protocol is only forwarded to the handshake protocol in
   * singular, complete and correctly ordered handshake messages.
   */
  HandshakeMessageAggregator message_aggregator_{*this};

  /*! \brief The role of this HandshakeStateMachine, it can be: TLSClient, TLSServer, DTLSClient or DTLSServer. */
  EnTlsCommRole role_{EnTlsCommRole::kCommRoleNotDefined};

  /*! \brief Pointer to the Timer. Used by the states. */
  std::unique_ptr<common::TlsTimer> uptr_timer_{nullptr};

  /*! \brief The secret used in calculating the verification cookie */
  std::uint32_t verification_secret_{0U};

  /*! \brief The boolean value indicates if cookie verification is enabled or disabled */
  bool cookie_verification_enabled_{false};

  /*!
   * \brief Change the current HandshakeState to Disconnected.
   * \details Calling OnEnter()/OnExit() etc.
   *          NOTE: Will not do any error checking of the context.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void ChangeToDisconnectedState(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Checks if the current state wants the state machine to transit into another state
   * \details If a transition to another state needs to be done, it executes the state change. It will check for errors
   * in the context at each step; before exiting and after OnEnter.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void ChangeState(common::TlsCommFlowContext& cxt) noexcept;

  /*! \brief The name of the message flow that is currently executing */
  amsr::core::String current_msg_flow_name_{};

  /*! \brief The trace id of the message that is currently executing */
  amsr::core::String current_flow_trace_id_{};

  /*! \brief The security parameter that is currently being negotiated */
  record_protocol::SecurityParameters next_security_params_{};

  /*! \brief The MasterSecretContainer that is being used in current handshake*/
  tls_crypto_adapter::internal::MasterSecretContainer master_secret_container_{};
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_H_
