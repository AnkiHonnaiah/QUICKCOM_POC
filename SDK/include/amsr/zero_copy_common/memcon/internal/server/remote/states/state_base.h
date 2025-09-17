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
 *        \brief  Definition of class StateBase.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_BASE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_BASE_H_

#include <cstdint>
#include <memory>
#include <tuple>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_interface.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/server_memory_manager.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {
namespace remote {
namespace states {

/*!
 * \brief Forward declaration of the class StateMachineBase.
 * \details Required to prevent cyclic includes.
 */
class StateMachineBase;

/*!
 * \brief State base class.
 * \vprivate Component Private
 */
class StateBase {
 public:
  /*!
   * \brief Type to hold information on whether a slot was dropped for a receiver class.
   */
  using DroppedInformation = logic::internal::ServerInterface::DroppedInformation;

  /*!
   * \brief Destruct the StateBase.
   * \vprivate Component Private
   */
  virtual ~StateBase() noexcept = default;

  /*!
   * \brief Do not copy another StateBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateBase const&) noexcept -> StateBase& = delete;

  /*!
   * \brief Do not move another StateBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateBase&&) noexcept -> StateBase& = delete;

  /*!
   * \brief       Handle a connection ack.
   * \details     Implements state-dependent handling of connection acks.
   *              May request a state transition.
   *              A connection ack is expected when the current state is ReceiverState::kConnecting and after
   *              Connect() was called.
   *              Any error that happens when processing the connection ack results in a state transition
   *              to ReceiverState::kCorrupted.
   *              A connection ack in state in ReceiverState::kConnected results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A connection ack in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \param       queue_memory_config        Configuration of the client queue memory.
   * \param       memory_exchange_handle_ptr Memory exchange handle of the client queue memory.
   *                                         Must not be a nullptr. This is not checked.
   * \return      Optional containing a pair of a readable client queue memory and a receiver handle for the Logic,
   *              if the connection handshake was successfully completed. Empty optional, otherwise.
   * \context     REACTOR
   * \pre         Must not be called for ReceiverState::kConnecting and before Connect() is called. This is not checked.
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto OnAckConnection(QueueMemoryConfig queue_memory_config,
                               MemoryExchangeHandlePtr memory_exchange_handle_ptr) noexcept
      -> ::amsr::core::Optional<std::pair<ReadableMemoryPtr, logic::internal::ReceiverHandle>> = 0;

  /*!
   * \brief       Handle a shutdown.
   * \details     Implements state-dependent handling of shutdowns.
   *              May request a state transition.
   *              A shutdown in ReceiverState::kConnecting or ReceiverState::kConnected results in a state transition
   *              to ReceiverState::kDisconnected.
   *              If a shutdown occurs in ReceiverState::kConnected the receiver is deregistered from the logic server
   *              and the receiver handle is invalidated.
   *              A shutdown in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \context     REACTOR
   * \pre         Must not be called for ReceiverState::kConnecting and before Connect() is called. This is not checked.
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void OnShutdown() noexcept = 0;

  /*!
   * \brief       Handle a start listening.
   * \details     Implements state-dependent handling of start listenings.
   *              May request a state transition.
   *              A start listening is expected in ReceiverState::kConnected alternating with stop listening. If two
   *              consecutive start listenings without stop listening in between are received, it results in a state
   *              transition to ReceiverState::kCorrupted.
   *              A start listening in ReceiverState::kConnecting results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A start listening in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \context     REACTOR
   * \pre         Must not be called for ReceiverState::kConnecting and before Connect() is called. This is not checked.
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void OnStartListening() noexcept = 0;

  /*!
   * \brief       Handle a stop listening.
   * \details     Implements state-dependent handling of stop listenings.
   *              May request a state transition.
   *              A stop listening is expected in ReceiverState::kConnected alternating with start listening. If stop
   *              listening is triggered without a preceding start listening or two consecutive stop listenings without
   *              start listening in between are received, it results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A stop listening in ReceiverState::kConnecting results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A stop listening in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \context     REACTOR
   * \pre         Must not be called for ReceiverState::kConnecting and before Connect() is called. This is not checked.
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void OnStopListening() noexcept = 0;

  /*!
   * \brief       Handle an error.
   * \details     Implements state-dependent error handling.
   *              May request a state transition.
   *              An error in any state (but ReceiverState::kCorrupted and ReceiverState::kDisconnected) results in a
   *              state transition to ReceiverState::kCorrupted.
   *              An error in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \param       error_code Error code that identifies the cause of the error.
   *                         Must be one of the following error codes:
   *                           - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                             peer closed the connection without adhering to the protocol.
   *                           - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the peer
   *                             crashed.
   *                           - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If any protocol
   *                             error is noticed during the communication with the peer.
   *                         This is not checked.
   * \context     REACTOR
   * \pre         Must not be called for ReceiverState::kConnecting and before Connect() is called. This is not checked.
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void OnError(::amsr::core::ErrorCode error_code) noexcept = 0;

  /*!
   * \brief       Start connecting to the remote by triggering the asynchronous handshake.
   * \details     Implements state-dependent handling of a connection start trigger.
   *              May trigger a state transition.
   *              The first protocol message is sent to the remote containing slot memory configuration, slot memory
   *              exchange handle, server queue memory configuration and server queue memory exchange handle.
   *              Additionally, asynchronous reception is started and all passed callbacks can be called from now.
   *              If the asynchronous handshake is done successfully the ReceiverState will change to
   *              ReceiverState::kConnected.
   *              If any error occurs during the handshake, the receiver state will change to ReceiverState::kCorrupted.
   *              May request a state transition.
   * \param       on_ack_connection_cbk  Callback that is called each time an connection ack on the side channel is
   *                                     received.
   *                                     Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                     Must stay valid until it is not used anymore. This cannot be checked.
   * \param       on_start_listening_cbk Callback that is called each time an start listening on the side
   *                                     channel is received.
   *                                     Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                     Must stay valid until it is not used anymore. This cannot be checked.
   * \param       on_stop_listening_cbk  Callback that is called each time an stop listening on the side
   *                                     channel is received.
   *                                     Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                     Must stay valid until it is not used anymore. This cannot be checked.
   * \param       on_shutdown_cbk        Callback that is called each time an shutdown on the side channel
   *                                     is received.
   *                                     Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                     Must stay valid until it is not used anymore. This cannot be checked.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedReceiverState If the current state is
   *              not ReceiverState::kConnecting or if the current state is ReceiverState::kConnecting and this method
   *              has already been called before.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer
   *              crashed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the first handshake message
   *              to the receiver could not be sent for any other reason.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto Connect(OnAckConnectionCallback on_ack_connection_cbk, OnStartListeningCallback on_start_listening_cbk,
                       OnStopListeningCallback on_stop_listening_cbk, OnShutdownCallback on_shutdown_cbk) noexcept
      -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Query whether this receiver is in use to receive messages.
   * \details     Note that this function always returns true if the receiver is not in state
   *              ReceiverState::kDisconnected. The state ReceiverState::kDisconnected can be reached either with a
   *              shutdown by the remote receiver, a call to Terminate() or a call to HandleServerShutdown() for this
   *              the server.
   *              Additionally, asynchronous reception of messages may currently be ongoing even after the receiver is
   *              in state ReceiverState::kDisconnected.
   *              When this function returns false once for a receiver, it will always return false for all future calls
   *              regarding the same receiver.
   * \return      True, if the receiver is not in state ReceiverState::kDisconnected or any provided callback is
   *              currently executing. False, otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

  /*!
   * \brief       Terminate this receiver.
   * \details     After a call to this function the receiver is in state ReceiverState::kDisconnected.
   *              Deregisters all callbacks on the side channel and stops asynchronous reception of messages. No
   *              callback will be called anymore but a callback might currently be ongoing. IsInUse() can be used after
   *              this call to check if any provided callback is still executing.
   *              A termination message to the peer is only sent if Connect() was successfully called before and the
   *              client is not yet in state ReceiverState::kCorrupted or ReceiverState::kDisconnected.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedReceiverState If the receiver is
   *              in state ReceiverState::kDisconnected.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto Terminate() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Handle a server-side shutdown for this receiver.
   * \details     Implements state-dependent handling of a server shutdown.
   *              After a call to this method, the receiver is in state ReceiverState::kDisconnected.
   *              Additionally, all related callbacks are deregistered and thus, stops future asynchronous reception of
   *              messages. However, asynchronous reception of messages may currently be ongoing.
   *              If the receiver is already in state ReceiverState::kDisconnected, there is no action.
   *              A shutdown message is sent to the receiver if its current state is ReceiverState::kConnected or
   *              ReceiverState::kConnecting when Connect() was already called.
   *              May request a state transition.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer
   *              crashed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the first handshake message
   *              to the receiver could not be sent for any other reason.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto HandleServerShutdown() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Handle, if a new slot was sent by the server.
   * \details     Implements state-dependent handling for a newly sent slot.
   *              The receiver itself decides if a notification will be sent based on its internal state. Notifications
   *              are requested/stopped asynchronously with OnStartListening() and OnStopListening().
   *              Notifications are dropped if the message itself was dropped because of the receiver class limit.
   *              Notifications are only sent in state ReceiverState::kConnected when it was requested by the receiver.
   *              May request a state transition.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the notification could not be
   *              sent for any reason.
   * \param       dropped_info Information on whether a slot was dropped for a receiver class.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto NotifyNewSlotSent(DroppedInformation const& dropped_info) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Get the receiver state that corresponds to this state.
   * \return      Receiver state that corresponds to this state.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetReceiverState() const noexcept -> ReceiverState;

  /*!
   * \brief       Check and handle if this receiver caused a corruption in the Zero-Copy Logic.
   * \details     Implements state-dependent handling for a corruption detected by the Zero-Copy Logic.
   *              If a corruption in the Zero-Copy Logic was caused by this receiver, the receiver state will change to
   *              ReceiverState::kCorrupted.
   *              May request a state transition.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void CheckAndHandleLogicCorruption() noexcept = 0;

 protected:
  /*!
   * \brief       Construct a StateBase.
   * \details     This state is intended to be managed by the given \p state_machine. Thus, it is expected that when
   *              this state is fully constructed, this state is the current state of the \p state_machine.
   * \param       state_machine   State machine that manages this state.
   *                              Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logger          Logger of this state.
   *                              Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       receiver_state  Receiver state that corresponds to this state.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  StateBase(StateMachineBase& state_machine, ::ara::log::Logger& logger, ReceiverState receiver_state) noexcept;

  /*!
   * \brief       Log and request a transition to another state.
   * \param       to_receiver_state Receiver state specifying the state to transition to.
   *                                Must not be ReceiverState::kConnecting.
   *                                Must not be the currently active state.
   * \param       opt_error_code    Optional containing an error code if a state transition to ReceiverState::kCorrupted
   *                                is requested. It can also contain an error code when a state transition to
   *                                ReceiverState::kDisconnected is requested.
   *
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  void LogAndTransitionToRequest(ReceiverState to_receiver_state,
                                 ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code) noexcept;

  /*!
   * \brief Construct a StateBase by copying.
   * \vprivate Component Private
   */
  StateBase(StateBase const&) noexcept = default;

  /*!
   * \brief Construct a StateBase by moving.
   * \vprivate Component Private
   */
  StateBase(StateBase&&) noexcept = default;

 private:
  /*!
   * \brief The state machine of this state base.
   */
  StateMachineBase& state_machine_;

  /*!
   * \brief The logger of this state base.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The receiver state that corresponds to this state.
   */
  ReceiverState receiver_state_;
};

/*!
 * \brief       Stream any state to a log stream.
 * \param       stream Log stream to stream to.
 * \param       state  State to stream to the log stream.
 * \return      The same log stream as \p stream.
 * \context     ANY
 * \pre         -
 *
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto operator<<(::ara::log::LogStream& stream, StateBase const& state) noexcept -> ::ara::log::LogStream&;

}  // namespace states
}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_BASE_H_
