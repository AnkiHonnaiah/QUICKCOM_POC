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
 *         \brief  Interface class for handshake state machine.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_INTERFACE_H_

#include <cstdint>
#include <memory>

#include "amsr/core/string_view.h"
#include "amsr/timer/timer_manager.h"
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/common/timer_extension.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_message_aggregator.h"
#include "tls/internal/handshakestatemachine/handshake_state_pool_interface.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \vprivate This class is component private.
 * \brief Interface class for handshake state machine.
 */
class HandshakeStateMachineInterface : public record_protocol::RecordToHandshakeCallbackInterface,
                                       public common::TimerExtensionCallbackInterface {
 public:
  /*!
   * \brief Type alias for TLS extension value.
   */
  using TlsExtensionTypeValue = std::uint16_t;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_libseccom_A12.1.6_inheriting_constructors
  /*!
   * \brief Constructor.
   */
  HandshakeStateMachineInterface() noexcept : RecordToHandshakeCallbackInterface{}, TimerExtensionCallbackInterface{} {}

  HandshakeStateMachineInterface(HandshakeStateMachineInterface const&) noexcept = delete;

  HandshakeStateMachineInterface(HandshakeStateMachineInterface&&) noexcept = delete;

  auto operator=(HandshakeStateMachineInterface const&) noexcept -> HandshakeStateMachineInterface& = delete;

  auto operator=(HandshakeStateMachineInterface&&) noexcept -> HandshakeStateMachineInterface& = delete;

  /*!
   * \brief Destructor.
   */
  ~HandshakeStateMachineInterface() noexcept override = default;

  /*!
   * \brief     This operation performs initialization activities that is separated from the constructor.
   *
   * \param[in] role_to_take Role to be used by the deriving class instance.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void CreateInstance(EnTlsCommRole role_to_take) noexcept = 0;

  /*!
   * \brief         Makes the instance ready for use.
   *
   * \param[in,out] cxt                   The flow context used for error reporting.
   * \param[in]     role_to_take          Role to be used by the deriving class instance.
   * \param[in]     starting_state_handle The starting state.
   * \param[in]     crypto_suites_to_use  Crypto suites that shall be supported by the deriving class instance.
   * \param[in]     timer_manager         The timer manager instance.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  virtual void OpenForAction(common::TlsCommFlowContext& cxt, EnTlsCommRole role_to_take,
                             HandshakeStateHandle starting_state_handle, ListOfCipherSuites& crypto_suites_to_use,
                             internal::common::TlsTimerManager timer_manager) noexcept = 0;

  /*!
   * \brief     Close down and return resources.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void CloseDown() noexcept = 0;

  /*!
   * \brief     Reset the deriving instance into a clean state.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void Cleanup() noexcept = 0;

  /*!
   * \brief         Initiates a TLS or DTLS handshake.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  virtual void InitiateHandshake(common::TlsCommFlowContext& cxt) noexcept = 0;

  /*!
   * \brief         Disconnect from the currently active TLS or DTLS connection.
   *
   * \param[in,out] cxt               The flow context used for error reporting.
   * \param[in]     send_close_notify True if a "CloseNotify" alert message must be sent before closing the connection;
   *                                  false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  virtual void Disconnect(common::TlsCommFlowContext& cxt, bool send_close_notify) noexcept = 0;

  /*!
   * \brief     Callback which is to be invoked during the handshake to get the information about TLS extensions.
   *
   * \param[in] extensions Specifies the TLS extension type.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void ReportTlsExtensions(ara::core::Vector<TlsExtensionTypeValue> extensions) noexcept = 0;

  /*!
   * \brief         Check the context for errors.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  virtual void HandleError(common::TlsCommFlowContext& cxt) noexcept = 0;

  /*!
   * \brief     Returns the associated communication role.
   *
   * \return    A communication role.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetRole() const noexcept -> EnTlsCommRole = 0;

  /*!
   * \brief     Stop the timer.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void StopTimer() noexcept = 0;

  /*!
   * \brief     Returns PSK UUID corresponding to the given PSK identity and hint.
   *
   * \param[in] id        A PSK identity.
   * \param[in] hint      A PSK hint.
   * \param[in] is_server True if the communication provider is a server; false otherwise.
   * \return    The found PSK UUID, or a null UUID if none was found.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetPskUuid(amsr::core::StringView id, amsr::core::StringView hint, bool is_server) const noexcept
      -> ara::crypto::Uuid = 0;

  /*!
   * \brief     Sets the timer to trigger once after N milliseconds.
   *
   * \param[in] duration_ms Duration after timer triggers in milliseconds.
   * \param[in] stop_timer  Indicates whether the timer should be stopped before setting to one shot.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void SetOneShotTimer(std::uint32_t duration_ms, bool stop_timer) const noexcept = 0;

  /*!
   * \brief     Returns the next security parameters, i.e., those that are currently being negotiated.
   *
   * \return    The next security parameters.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetNextSecurityParameters() noexcept -> record_protocol::SecurityParameters& = 0;

  /*!
   * \brief     Returns the handshake state handle of the currently active state.
   *
   * \return    A handshake state handle.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetState() const noexcept -> HandshakeStateHandle = 0;

  /*!
   * \brief     Returns the associated handshake state pool.
   *
   * \return    A handshake state pool.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetStatePool() const noexcept -> HandshakeStatePoolInterface& = 0;

  /*!
   * \brief     Returns the current crypto adapter.
   *
   * \return    A crypto adapter.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetCryptoAdapter() const noexcept -> tls_crypto_adapter::internal::CryptoAdapterInterface& = 0;

  /*!
   * \brief     Returns the current TLS configuration.
   *
   * \return    A TLS configuration.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetConfig() const noexcept -> TlsCommConfigurationInformation const& = 0;

  /*!
   * \brief     Returns the associated handshake message aggregator.
   *
   * \return    A handshake message aggregator.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetHandshakeMessageAggregator() noexcept -> HandshakeMessageAggregator& = 0;

  /*!
   * \brief     Returns the associated verification secret.
   *
   * \return    A verification secret.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetVerificationSecret() const noexcept -> std::uint32_t = 0;

  /*!
   * \brief     Set the verification secret.
   *
   * \param[in] new_value The new verification secret.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void SetVerificationSecret(std::uint32_t new_value) noexcept = 0;

  /*!
   * \brief     Indicates whether cookie verification is enabled.
   *
   * \return    True if cookie verification is enabled; false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto IsCookieVerificationEnabled() const noexcept -> bool = 0;

  /*!
   * \brief     Set the state of cookie verification.
   *
   * \param[in] enabled True to enable cookie verification; false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual void SetCookieVerificationEnabled(bool enabled) noexcept = 0;

  /*!
   * \brief     Returns the associated master secret container.
   *
   * \return    A master secret container.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  virtual auto GetMasterSecretContainer() noexcept -> tls_crypto_adapter::internal::MasterSecretContainer& = 0;
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_MACHINE_INTERFACE_H_
