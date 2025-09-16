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
/*!
 *             \file  tls_comm_providers.h
 *            \brief  This file includes declaration of classes implementing most of the interfaces provided in the
 *                    external API.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_PROVIDERS_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_PROVIDERS_H_

#include <memory>
#include <utility>

#include "ara/log/logger.h"
#include "tls/internal/common/timer_extension.h"
#include "tls/internal/handshake_to_user_callback_implementation.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/internal/record_to_user_callback_implementation.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/diagnostics_interface.h"
#include "tls/public/tls_comm_callback_interfaces.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls/public/tls_comm_interfaces.h"

namespace amsr {
namespace tls {

/*!
 * \brief Set the TLS global logger.
 * \details This functionality will be DEPRECATED, the logger will be set at component initialization.
 * \param[in] logger A reference to the new logger.
 */
void SetLogger(ara::log::Logger& logger) noexcept;

/*!
 * \brief Unset the TLS global logger.
 * \details This functionality will be DEPRECATED, the logger will be unset at component de-initialization.
 */
void UnsetLogger() noexcept;

/*!
 * \brief Implementation of the TlsCommBase interface for client providers.
 * \vprivate Vector product private API.
 */
class TlsClientCommProvider final : public TlsCommBase {
 public:
  /*!
   * \brief Create a new (D)TLS client provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> Create(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      amsr::timer::TimerManager& timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*!
   * \brief Create a new (D)TLS client provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> Create(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      amsr::steady_timer::TimerManagerInterface* timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*!
   * \brief The constructor
   * \param[in] config The configuration for this provider.
   * \param[in] uptr_psk_config The pre-shared key configuration.
   * \param[in] crypto_adapter The crypto adapter this provider shall use.
   * \param[in] ptr_callback_holder A pointer to the callback.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler for this provider.
   * \vprivate
   */
  TlsClientCommProvider(TlsCommConfigurationInformation config, config::PskConfig::Uptr uptr_psk_config,
                        std::unique_ptr<tls_crypto_adapter::internal::CryptoAdapterInterface> crypto_adapter,
                        TlsCommCallbackInterface* ptr_callback_holder, amsr::core::StringView message_flow_name,
                        std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler) noexcept;

  /*! \brief Not copy constructible. */
  TlsClientCommProvider(TlsClientCommProvider const&) = delete;

  /*! \brief Not move constructible. */
  TlsClientCommProvider(TlsClientCommProvider&&) = delete;

  /*! \brief Not copy assignable. */
  TlsClientCommProvider& operator=(TlsClientCommProvider const&) = delete;

  /*! \brief Not move assignable. */
  TlsClientCommProvider& operator=(TlsClientCommProvider&&) = delete;

  /*! \brief The destructor declaration. Override from inherited class. */
  ~TlsClientCommProvider() noexcept final = default;

  /*!
   * \copydoc TlsCommBase::GetProviderRole
   * \vprivate Vector product private API.
   */
  EnTlsCommRole GetProviderRole() const noexcept final;

  /*!
   * \copydoc TlsCommBase::GetConnectionState
   * \vprivate Vector product private API.
   */
  EnTlsCommRoleState GetConnectionState() const noexcept final;

  /*!
   * \copydoc TlsCommBase::CloseConnection
   * \vprivate Vector product private API.
   */
  void CloseConnection() noexcept final;

  /*!
   * \copydoc TlsCommBase::InitiateHandshake
   * \trace DSGN-SecCom-ConnectionStates
   * \vprivate Vector product private API.
   */
  void InitiateHandshake() noexcept final;

  /*!
   * \copydoc TlsCommBase::HandleReadFromCommParty
   * \vprivate Vector product private API.
   */
  TlsCommErrorCode HandleReadFromCommParty(ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc TlsCommBase::HandleReadFromTransport
   * \vprivate Vector product private API.
   */
  TlsCommErrorCode HandleReadFromTransport(ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \brief Makes the TLS provider instance ready for use. Shall be called only once during the objects lifetime.
   * \details
   * This operation must be called before any other operations belonging to the instance since it will make necessary
   * initiations needed for its functionality. It will also call corresponding operations for its contained objects.
   * \param[in] timer_manager
   * This parameter provides the timer manager object that should be used by this instance of the communication
   * provider.
   * The timer manager is used internally to register various timers which will handle timeouts when needed.
   * The timer manager and the calls to registered timeout callbacks are handled by the reactor thread when a
   * timeout occurs.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \trace DSGN-SecCom-CipherService
   * \vprivate Vector product private API.
   */
  void OpenForAction(internal::common::TlsTimerManager timer_manager) noexcept;

  /*!
   * \brief This operation will close down the communication provider and return dynamic resources.
   * \details After the call to this operation, the object shall not be used. It shall be returned to the pool.
   * The operation will call corresponding operations on objects contained by this instance.
   * The purpose with the operation is to return all allocated resources, clear relevant variables and return
   * to a state which will allow a call to OpenForAction().
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  void CloseDown() noexcept;

  /*!
   * \brief Gets the internal flow context as reference
   * \return A reference of the internal flow context
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  internal::common::TlsCommFlowContext& GetFlowContext() noexcept;

  /*!
   * \brief Gets the internal user to callback pointer
   * \return A pointer of the user to callback
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  TlsCommCallbackInterface* GetPtrToUserCallback() const noexcept;

  /*!
   * \brief Sets the internal curent state
   * \param[in] state Curent state
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  void SetCurrentState(EnTlsCommRoleState state) noexcept;

 private:
  /*!
   * \brief Create a new (D)TLS client provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> WrappedCreate(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      internal::common::TlsTimerManager timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*! \brief A configuration for this provider */
  TlsCommConfigurationInformation const config_;

  /*! \brief The pre-shared key configuration. */
  config::PskConfig::Uptr const uptr_psk_config_;

  /*! \brief A crypto adapter for this provider */
  std::unique_ptr<tls_crypto_adapter::internal::CryptoAdapterInterface> crypto_adapter_;

  /*! \brief A pointer to TlsCommCallbackInterface instance */
  TlsCommCallbackInterface* ptr_to_user_callbacks_;

  /*! \brief An instance of HandshakeToUserCallbackImpl */
  internal::HandshakeToUserCallbackImpl<TlsClientCommProvider> handshake_to_user_callback_;

  /*! \brief An instance of RecordToUserCallbackImpl */
  internal::RecordToUserCallbackImpl<TlsClientCommProvider> record_to_user_callback_;

  /*! \brief An instance of MessageManager */
  internal::messages::MessageManager included_message_manager_{};

  /*! \brief An instance of RecordProtocol */
  internal::record_protocol::RecordProtocol included_record_protocol_{config_, *crypto_adapter_, *uptr_psk_config_};

  /*! \brief An instance of HandshakeStateMachine */
  internal::handshakestatemachine::HandshakeStateMachine included_state_machine_{config_,
                                                                                 *uptr_psk_config_,
                                                                                 *crypto_adapter_,
                                                                                 included_message_manager_,
                                                                                 included_record_protocol_,
                                                                                 handshake_to_user_callback_};

  /*! \brief An instance of EnTlsCommRoleState */
  EnTlsCommRoleState current_state_{EnTlsCommRoleState::kRoleStateNotConnected};

  /*! \brief An instance of EnTlsCommRole */
  EnTlsCommRole current_role_{EnTlsCommRole::kCommRoleNotDefined};

  /*! \brief Used for execution flow tracing and error reporting */
  internal::common::TlsCommFlowContext flow_context_{};

  /*! Friend test declaration for HandshakeToUserCallbackImpl */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, HandshakeToUserCallbackImpl);
  /*! Friend test declarations for RecordToUserCallbackImpl */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, RecordToUserCallbackImpl);
  /*! Friend test declarations for HandleReadFromCommParty_HandleReadFromTransport*/
  FRIEND_TEST(Cdd__TLSTestFixtureBase, HandleReadFromCommParty_HandleReadFromTransport);
  /*! Friend test declarations for NullPointerToTlsClientCallBackInterface*/
  FRIEND_TEST(Cdd__TLSTestFixtureBase, NullPointerToTlsClientCallBackInterface);
  /*! \brief Friend test declaration for ClientTimeout */
  FRIEND_TEST(CT__TlsCommProviders__ClientTimeout, ClientTimeout);
  /*! \brief Friend test declaration for OnHandshakeMessageInEncryptedClientState */
  FRIEND_TEST(internal::record_protocol::CT__TlsCommProviders__OnHandshakeMessageInEncryptedClientState,
              OnHandshakeMessageInEncryptedClientState);
  /*! \brief Friend test declaration for ReportTlsExtensions() */
  FRIEND_TEST(UT__TlsCommProviders, ReportTlsExtensions);
  /*! \brief Friend test declaration for ClientTimeoutInClientStateClientHello() */
  FRIEND_TEST(CT__TlsCommProviders, ClientTimeoutInClientStateClientHello);
  /*! \brief Friend test declaration for ClientTimeoutInClientStateServerHello() */
  FRIEND_TEST(CT__TlsCommProviders, ClientTimeoutInClientStateServerHello);
  /*! \brief Friend test declaration for ClientTimeoutInClientStateServerChangeCipherSpec() */
  FRIEND_TEST(CT__TlsCommProviders, ClientTimeoutInClientStateServerChangeCipherSpec);
};

/*!
 * \brief Implementation of the TlsCommBase interface for server providers
 * \vprivate Vector product private API.
 */
class TlsServerCommProvider final : public TlsCommBase {
 public:
  /*!
   * \brief Create a new (D)TLS server provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> Create(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      amsr::timer::TimerManager& timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*!
   * \brief Create a new (D)TLS server provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> Create(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      amsr::steady_timer::TimerManagerInterface* timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*!
   * \brief The constructor
   * \param[in] config The configuration for this provider.
   * \param[in] uptr_psk_config The pre-shared key configuration.
   * \param[in] crypto_adapter The crypto adapter this provider shall use.
   * \param[in] ptr_callback_holder A pointer to the callback.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler for this provider.
   * \vprivate
   */
  TlsServerCommProvider(TlsCommConfigurationInformation config, config::PskConfig::Uptr uptr_psk_config,
                        std::unique_ptr<tls_crypto_adapter::internal::CryptoAdapterInterface> crypto_adapter,
                        TlsCommCallbackInterface* ptr_callback_holder, amsr::core::StringView message_flow_name,
                        std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler) noexcept;

  /*! \brief Not copy constructible. */
  TlsServerCommProvider(TlsServerCommProvider const&) = delete;

  /*! \brief Not move constructible. */
  TlsServerCommProvider(TlsServerCommProvider&&) = delete;

  /*! \brief Not copy assignable. */
  TlsServerCommProvider& operator=(TlsServerCommProvider const&) = delete;

  /*! \brief Not move assignable. */
  TlsServerCommProvider& operator=(TlsServerCommProvider&&) = delete;

  /*! \brief The destructor declaration. Override from inherited class. */
  ~TlsServerCommProvider() noexcept final = default;

  /*!
   * \copydoc TlsClientCommProvider::OpenForAction
   * \error TlsCommErrorCode::kTlsErrInternal is reported if fails to call OpenForAction() method to
   *        include handshake state machine.
   * \trace DSGN-SecCom-CipherService
   * \vprivate Vector product private API.
   */
  void OpenForAction(internal::common::TlsTimerManager timer_manager) noexcept;

  /*!
   * \copydoc TlsClientCommProvider::CloseDown
   * \vprivate Vector product private API.
   */
  void CloseDown() noexcept;

  /*!
   * \copydoc TlsCommBase::GetProviderRole
   * \vprivate Vector product private API.
   */
  EnTlsCommRole GetProviderRole() const noexcept final;

  /*!
   * \copydoc TlsCommBase::GetConnectionState
   * \vprivate Vector product private API.
   */
  EnTlsCommRoleState GetConnectionState() const noexcept final;

  /*!
   * \copydoc TlsCommBase::InitiateHandshake
   * \trace SPEC-6561865
   * \vprivate Vector product private API.
   */
  void InitiateHandshake() noexcept final;

  /*!
   * \copydoc TlsCommBase::HandleReadFromCommParty
   * \vprivate Vector product private API.
   */
  TlsCommErrorCode HandleReadFromCommParty(ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc TlsCommBase::HandleReadFromTransport
   * \vprivate Vector product private API.
   */
  TlsCommErrorCode HandleReadFromTransport(ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc TlsCommBase::CloseConnection
   * \vprivate Vector product private API.
   */
  void CloseConnection() noexcept final;

  /*!
   * \brief Gets the internal flow context as reference
   * \return A reference of the internal flow context
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  internal::common::TlsCommFlowContext& GetFlowContext() noexcept;

  /*!
   * \brief Gets the internal user to callback pointer
   * \return A pointer of the user to callback
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  TlsCommCallbackInterface* GetPtrToUserCallback() const noexcept;

  /*!
   * \brief Sets the internal curent state
   * \param[in] state curent state
   * \context Any
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \vprivate Vector product private API.
   */
  void SetCurrentState(EnTlsCommRoleState state) noexcept;

 private:
  /*!
   * \brief Create a new (D)TLS server provider.
   * \param[in] config The TLS configuration.
   * \param[in] ptr_callback_holder
   * A pointer to an object implementing the communication callback interface. (See TlsCommCallbackInterface).
   * The lifecycle of this object shall be handled by the caller of this operation. This pointer is only stored and
   * used internally. It should be valid as long as this instance is in scope and used.
   * \param[in] timer_manager The timer manager.
   * \param[in] uptr_psk_config A unique pointer to the PSK configuration.
   * \param[in] message_flow_name A string identifying the provider in the log.
   * \param[in] diagnostic_handler The diagnostic handler.
   * \return A result containing a unique pointer to the (D)TLS provider.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidArgument
   *        If the provided configuration is not valid.
   * \error amsr::tls::TlsErrorDomain::TlsCommErrorCode::kTlsErrInvalidState
   *        If an error occurred in the construction of the provider.
   */
  static ara::core::Result<TlsCommProviderUniquePtr> WrappedCreate(
      TlsCommConfigurationInformation config, TlsCommCallbackInterface* ptr_callback_holder,
      internal::common::TlsTimerManager timer_manager, config::PskConfig::Uptr uptr_psk_config,
      amsr::core::StringView message_flow_name = amsr::core::StringView{""},
      std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler = nullptr) noexcept;

  /*! \brief A configuration for this provider */
  TlsCommConfigurationInformation const config_;

  /*! \brief The pre-shared key configuration. */
  config::PskConfig::Uptr const uptr_psk_config_;

  /*! \brief A crypto adapter for this provider */
  std::unique_ptr<tls_crypto_adapter::internal::CryptoAdapterInterface> crypto_adapter_;

  /*! \brief A pointer to TlsCommCallbackInterface instance */
  TlsCommCallbackInterface* ptr_to_user_callbacks_;

  /*! \brief An instance of HandshakeToUserCallbackImpl */
  internal::HandshakeToUserCallbackImpl<TlsServerCommProvider> handshake_to_user_callback_;

  /*! \brief An instance of RecordToUserCallbackImpl */
  internal::RecordToUserCallbackImpl<TlsServerCommProvider> record_to_user_callback_;

  /*! \brief An instance of MessageManager */
  internal::messages::MessageManager included_message_manager_{};

  /*! \brief An instance of RecordProtocol */
  internal::record_protocol::RecordProtocol included_record_protocol_{config_, *crypto_adapter_, *uptr_psk_config_};

  /*! \brief An instance of HandshakeStateMachine */
  internal::handshakestatemachine::HandshakeStateMachine included_state_machine_{config_,
                                                                                 *uptr_psk_config_,
                                                                                 *crypto_adapter_,
                                                                                 included_message_manager_,
                                                                                 included_record_protocol_,
                                                                                 handshake_to_user_callback_};

  /*! \brief An instance of EnTlsCommRoleState */
  EnTlsCommRoleState current_state_{EnTlsCommRoleState::kRoleStateNotConnected};

  /*! \brief An instance of EnTlsCommRole */
  EnTlsCommRole current_role_{EnTlsCommRole::kCommRoleNotDefined};

  /*! \brief Used for execution flow tracing and error reporting */
  internal::common::TlsCommFlowContext flow_context_{};

  /*! \brief Friend test declaration for DTlsMbedClientReconnectWithCookieSendMultipleApp() */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsMbedClientReconnectWithCookieSendMultipleApp);
  /*! \brief Friend test declaration for DTlsVectorClientReconnectWithCookieSendMultipleApp() */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsVectorClientReconnectWithCookieSendMultipleApp);
  /*! \brief Friend test declaration for DTlsMbedClientReconnectWithCookieSendMultipleAppAES() */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsMbedClientReconnectWithCookieSendMultipleAppAES);
  /*! \brief Friend test declaration for DTlsVectorClientReconnectWithCookieSendMultipleAppAES() */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsVectorClientReconnectWithCookieSendMultipleAppAES);
  /*! \brief Friend test declaration for DTlsVectorClientReconnectWithForgedClientHello() */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsVectorClientReconnectWithForgedClientHello);
  /*! \brief Friend test declaration for HandshakeToUserCallbackImpl() */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, HandshakeToUserCallbackImpl);
  /*! \brief Friend test declaration for RecordToUserCallbackImpl() */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, RecordToUserCallbackImpl);
  /*! \brief Friend test declaration for HandleReadFromCommParty_HandleReadFromTransport() */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, HandleReadFromCommParty_HandleReadFromTransport);
  /*! \brief Friend test declaration for NullPointerToTlsServerCallBackInterface() */
  FRIEND_TEST(Cdd__TLSTestFixtureBase, NullPointerToTlsServerCallBackInterface);
  /*! \brief Friend test declaration for ServerTimeout */
  FRIEND_TEST(CT__TlsCommProviders__ServerTimeout, ServerTimeout);
  /*! \brief Friend test declaration for DTlsVectorClientReconnectWithCookieSendMultipleApp */
  FRIEND_TEST(CT__TlsCommProviders, DTlsVectorClientReconnectWithCookieSendMultipleApp);
  /*! \brief Friend test declaration for ServerStateConnectedOnHandshakeMessageReceived */
  FRIEND_TEST(internal::record_protocol::CT__TlsCommProviders, ServerStateConnectedOnHandshakeMessageReceived);
  /*! \brief Friend test declaration for OnHandshakeMessageInEncryptedServerState */
  FRIEND_TEST(internal::record_protocol::CT__TlsCommProviders__OnHandshakeMessageInEncryptedServerState,
              OnHandshakeMessageInEncryptedServerState);
  /*! \brief Friend test declaration for ReportTlsExtensions() */
  FRIEND_TEST(UT__TlsCommProviders, ReportTlsExtensions);
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_PROVIDERS_H_
