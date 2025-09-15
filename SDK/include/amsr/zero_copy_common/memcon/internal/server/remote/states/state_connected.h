/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Definition of class StateConnected.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTED_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTED_H_

#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "vac/language/location.h"

#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/server_side_channel.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {
namespace remote {
namespace states {

/*!
 * \brief StateConnected state.
 * \vprivate Component Private
 */
class StateConnected final : public StateBase {
 public:
  /*!
   * \brief       Construct a StateConnected.
   * \details     This state is intended to be managed by the given \p state_machine. Thus, it is expected that when
   *              this state is fully constructed, this state is the current state of the \p state_machine.
   * \param       state_machine         State machine that manages this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logger                Logger of this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       side_channel          Side channel to communicate with the server.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       receiver_class_handle Handle representing the receiver class that this receiver belongs to.
   *                                    Must refer to a valid receiver class in the \p logic_server_ptr. This cannot be
   *                                    checked.
   * \param       logic_server_ptr      Logic server instance the receiver belongs to.
   *                                    Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                    Must stay valid for the whole lifetime of the receiver. This cannot be checked.
   * \param       opt_receiver_handle   Receiver handle of the receiver this state belongs to.
   *                                    Must be valid for the \p logic_server_ptr. This cannot be checked.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   *                                    Must contain a value. This is not checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  StateConnected(StateMachineBase& state_machine, ::ara::log::Logger& logger,
                 side_channel::ServerSideChannel& side_channel, ClassHandle const& receiver_class_handle,
                 LogicServerPtr& logic_server_ptr,
                 ::amsr::core::Optional<logic::internal::ReceiverHandle>& opt_receiver_handle) noexcept;

  /*!
   * \brief Do not construct a StateConnected by copying.
   * \vprivate Component Private
   */
  StateConnected(StateConnected const&) noexcept = delete;

  /*!
   * \brief Do not construct a StateConnected by moving.
   * \vprivate Component Private
   */
  StateConnected(StateConnected&&) noexcept = delete;

  /*!
   * \brief Do not copy another StateConnected into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateConnected const&) noexcept -> StateConnected& = delete;

  /*!
   * \brief Do not move another StateConnected into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateConnected&&) noexcept -> StateConnected& = delete;

  /*!
   * \brief Destruct the StateConnected.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~StateConnected() noexcept final = default;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::OnAckConnection
   */
  auto OnAckConnection(QueueMemoryConfig queue_memory_config,
                       MemoryExchangeHandlePtr memory_exchange_handle_ptr) noexcept
      -> ::amsr::core::Optional<std::pair<ReadableMemoryPtr, logic::internal::ReceiverHandle>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::OnShutdown
   */
  void OnShutdown() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::OnStartListening
   */
  void OnStartListening() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::OnStopListening
   */
  void OnStopListening() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::OnError
   */
  void OnError(::amsr::core::ErrorCode error_code) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::Connect
   */
  auto Connect(OnAckConnectionCallback on_ack_connection_cbk, OnStartListeningCallback on_start_listening_cbk,
               OnStopListeningCallback on_stop_listening_cbk, OnShutdownCallback on_shutdown_cbk) noexcept
      -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::Terminate
   */
  auto Terminate() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::HandleServerShutdown
   */
  auto HandleServerShutdown() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::NotifyNewSlotSent
   */
  auto NotifyNewSlotSent(DroppedInformation const& dropped_info) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase::CheckAndHandleLogicCorruption
   */
  void CheckAndHandleLogicCorruption() noexcept final;

 private:
  /*!
   * \brief Sub-states of the state connected.
   * \vprivate Component Private
   */
  enum class ConnectedState : std::uint8_t { kPolling = 0, kNotified = 1 };

  /*!
   * \brief       Handle a transition to StateCorrupted.
   * \param       error_code Error which caused the corruption of the receiver.
   *                         Must be one of the following errors:
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                            receiver closed the connection without adhering to the protocol.
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                            receiver crashed.
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server
   *                            notices a protocol error during the communication with the receiver.
   *                         This is not checked.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleCorrupted(::amsr::core::ErrorCode error_code) noexcept;

  /*!
   * \brief       Handle a transition to ReceiverState::kDisconnected.
   * \param       opt_error_code Optional error which occurred when disconnecting the receiver.
   *                             Must be one of the following errors:
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                                receiver closed the connection without adhering to the protocol.
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                                receiver crashed.
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server
   *                                notices a protocol error during the communication with the receiver.
   *                             This is not checked.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleDisconnected(::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code) noexcept;

  /*!
   * \brief The current sub-state of this state connected.
   */
  ConnectedState sub_state_{ConnectedState::kPolling};

  /*!
   * \brief The logger of this state connected.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The server side channel.
   */
  side_channel::ServerSideChannel& side_channel_;

  /*!
   * \brief The class handle representing the receiver class this receiver belongs to.
   */
  ClassHandle receiver_class_handle_;

  /*!
   * \brief The zero-copy logic server.
   * \details This reference is valid for the whole lifetime of this receiver.
   */
  LogicServerPtr& logic_server_ptr_;

  /*!
   * \brief The receiver handle which represents this receiver in the logic server.
   * \details The optional contains a value for the whole lifetime of this state.
   */
  ::amsr::core::Optional<logic::internal::ReceiverHandle>& opt_receiver_handle_;
};

}  // namespace states
}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTED_H_
