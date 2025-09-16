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
 *        \brief  Definition of class StateConnecting.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTING_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTING_H_

#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "vac/language/location.h"

#include "amsr/zero_copy_common/memcon/internal/server/receiver_id.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/server_side_channel.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {
namespace remote {
namespace states {

/*!
 * \brief StateConnecting state.
 * \vprivate Component Private
 */
class StateConnecting final : public StateBase {
 public:
  /*!
   * \brief       Construct a StateConnecting.
   * \details     This state is intended to be managed by the given \p state_machine. Thus, it is expected that when
   *              this state is fully constructed, this state is the current state of the \p state_machine.
   * \param       state_machine         State machine that manages this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logger                Logger of this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       side_channel          Side channel to communicate with the server.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       receiver_id           ReceiverId for this receiver.
   *                                    Must be unique regarding its index among all currently active receivers
   *                                    managed by the server instance. This cannot be checked.
   *                                    The index must be less than the maximum number of receivers configured. This
   *                                    cannot be checked.
   * \param       receiver_class_handle Receiver class handle.
   *                                    Must be valid for the \p logic_server_ptr. This cannot be checked.
   * \param       logic_server_ptr      Logic server instance the receiver belongs to.
   *                                    Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                    Must stay valid for the whole lifetime of the state. This cannot be checked.
   *                                    Must be configured using the memory the \p slot_memory_exchange_handle_ptr
   *                                    refers to. This cannot be checked.
   * \param       server_memory_manager Server memory manager for the server the receiver belongs to.
   *                                    Must stay valid for the whole lifetime of the state. This cannot be checked.
   *                                    Must be configured with the same parameters as the \p logic_server_ptr. This
   *                                    cannot be checked.
   * \param       slot_memory_config    Configuration of the slot memory.
   *                                    Must fit to the configuration of \p server_memory_manager. This cannot be
   *                                    checked.
   * \param       slot_memory_exchange_handle_ptr  Memory exchange handle of the slot memory.
   *                                               Must not be a nullptr. This is NOT CHECKED as this is a
   *                                               unit-internal API.
   *                                               Must refer to the slot memory allocated using the
   *                                               \p server_memory_manager. This cannot be checked.
   * \param       queue_memory_exchange_handle_ptr Memory exchange handle of the server queue memory.
   *                                               Must not be a nullptr. This is NOT CHECKED as this is a
   *                                               unit-internal API.
   *                                               Must refer to the queue memory allocated using the
   *                                               \p server_memory_manager. This cannot be checked.
   * \param       queue_memory_view                View to the whole server queue memory.
   *                                               Must refer to the queue memory allocated using the
   *                                               \p server_memory_manager. This cannot be checked.
   *                                               Must stay valid for the whole lifetime of this state. This cannot be
   *                                               checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  StateConnecting(StateMachineBase& state_machine, ::ara::log::Logger& logger,
                  side_channel::ServerSideChannel& side_channel, ReceiverId receiver_id,
                  ClassHandle receiver_class_handle, LogicServerPtr& logic_server_ptr,
                  memory_manager::ServerMemoryManager const& server_memory_manager,
                  SlotMemoryConfig const& slot_memory_config, MemoryExchangeHandlePtr slot_memory_exchange_handle_ptr,
                  MemoryExchangeHandlePtr queue_memory_exchange_handle_ptr,
                  ReadWritableMemoryView queue_memory_view) noexcept;

  /*!
   * \brief Do not construct a StateConnecting by copying.
   * \vprivate Component Private
   */
  StateConnecting(StateConnecting const&) noexcept = delete;

  /*!
   * \brief Do not construct a StateConnecting by moving.
   * \vprivate Component Private
   */
  StateConnecting(StateConnecting&&) noexcept = delete;

  /*!
   * \brief Do not copy another StateConnecting into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateConnecting const&) noexcept -> StateConnecting& = delete;

  /*!
   * \brief Do not move another StateConnecting into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateConnecting&&) noexcept -> StateConnecting& = delete;

  /*!
   * \brief Destruct the StateConnecting.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~StateConnecting() noexcept final = default;

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
   * \brief Type used for writing queue ends.
   */
  using WritingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface>;

  /*!
   * \brief Type used for reading queue ends.
   */
  using ReadingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface>;

  /*!
   * \brief Sub-states of the state connecting.
   * \vprivate Component Private
   */
  enum class ConnectingState : std::uint8_t { kNotConnected = 0, kConnectionStarted = 1 };

  /*!
   * \brief Sub-states of the state connecting.
   * \vprivate Component Private
   */
  enum class HandshakeProtocolState : std::uint8_t { kNotStarted = 0, kExpectAckConnection = 1, kSendFailed = 2 };

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
   * \brief The current sub-state of this state connecting.
   */
  ConnectingState sub_state_{ConnectingState::kNotConnected};

  /*!
   * \brief The current protocol state of this state connecting.
   */
  HandshakeProtocolState protocol_state_{HandshakeProtocolState::kNotStarted};

  /*!
   * \brief The logger of this state connecting.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The server side channel.
   */
  side_channel::ServerSideChannel& side_channel_;

  /*!
   * \brief The receiver id of the receiver this state belongs to.
   */
  ReceiverId receiver_id_;

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
   * \brief The server memory manager.
   * \details This reference is valid for the whole lifetime of this receiver.
   */
  memory_manager::ServerMemoryManager const& server_memory_manager_;

  /*!
   * \brief The configuration of the slot memory.
   */
  SlotMemoryConfig slot_memory_config_;

  /*!
   * \brief Memory exchange handle of the slot memory.
   * \details The class invariant ensures that this pointer is valid until ConnectingState::kConnectionStarted is
   *          reached.
   */
  MemoryExchangeHandlePtr slot_memory_exchange_handle_ptr_;

  /*!
   * \brief Memory exchange handle of the server queue memory.
   * \details The class invariant ensures that this pointer is valid until ConnectingState::kConnectionStarted is
   *          reached.
   */
  MemoryExchangeHandlePtr queue_memory_exchange_handle_ptr_;

  /*!
   * \brief Read-writable memory view to the whole server queue memory.
   * \details The read-writable view stays valid for the whole lifetime of this state.
   */
  ReadWritableMemoryView queue_memory_view_;
};

}  // namespace states
}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CONNECTING_H_
