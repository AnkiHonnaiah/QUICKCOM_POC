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
 *        \brief  Definition of class Receiver.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_RECEIVER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_RECEIVER_H_

#include <cstdint>
#include <memory>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/memory/memory_exchange_handle_interface.h"
#include "amsr/memory/readable_memory_interface.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_id.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

#include "amsr/zero_copy_common/memcon/internal/memory_manager/server_memory_manager.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/server_side_channel.h"

#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_machine_base.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {
namespace remote {

class Receiver final : private states::StateMachineBase {
 public:
  /*!
   * \brief Type to hold information on whether a slot was dropped for a receiver class.
   */
  using DroppedInformation = logic::internal::ServerInterface::DroppedInformation;

  /*!
   * \brief       Construct a Receiver.
   * \details     The receiver will setup its initial state to ReceiverState::kConnecting.
   *              Communication over the side channel does not directly start. To start the connection handshake and
   *              enable the asynchronous reception of messages, Connect() must be called.
   *              See the CDD of Connect() for more details.
   * \param       logger                           Logger of this receiver.
   *                                               Must stay valid for the whole lifetime of this state. This cannot be
   *                                               checked.
   * \param       receiver_id                      ReceiverId for this receiver.
   *                                               Must be unique regarding its index among all currently active
   *                                               receivers managed by the server instance. This cannot be checked.
   *                                               The index must be less than the maximum number of receivers
   *                                               configured. This cannot be checked.
   * \param       receiver_class_handle            Handle representing the receiver class that this receiver belongs to.
   *                                               Must refer to a valid receiver class in the \p logic_server_ptr.
   *                                               This cannot be checked.
   * \param       side_channel_ptr                 Side channel to communicate with the server.
   *                                               Must not be a nullptr. This is NOT CHECKED as this is a
   *                                               unit-internal API.
   *                                               Must stay valid for the whole lifetime of the receiver. This cannot
   *                                               be checked.
   * \param       on_error_cbk                     Callback that is called each time an error on the side channel is
   *                                               encountered.
   *                                               Must not contain a nullptr. This is NOT CHECKED as this is a
   *                                               unit-internal API.
   *                                               Must stay valid until it is not used anymore. This cannot be checked.
   * \param       logic_server_ptr                 Logic server instance the receiver belongs to.
   *                                               Must not contain a nullptr. This is NOT CHECKED as this is a
   *                                               unit-internal API.
   *                                               Must stay valid for the whole lifetime of the receiver. This cannot
   *                                               be checked.
   *                                               Must be configured using the memory the
   *                                               \p slot_memory_exchange_handle_ptr refers to. This cannot be checked.
   * \param       server_memory_manager            Server memory manager for the server the receiver belongs to.
   *                                               Must stay valid for the whole lifetime of the receiver. This cannot
   *                                               be checked.
   *                                               Must be configured with the same parameters as the
   *                                               \p logic_server_ptr. This cannot be checked.
   * \param       slot_memory_config               Configuration of the slot memory.
   *                                               Must fit to the configuration of \p server_memory_manager. This
   *                                               cannot be checked.
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
   *                                               Must stay valid until state ReceiverState::kDisconnected is reached.
   *                                               This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  Receiver(::ara::log::Logger& logger, ReceiverId receiver_id, ClassHandle receiver_class_handle,
           std::unique_ptr<side_channel::SideChannelInterface> side_channel_ptr, OnErrorCallback on_error_cbk,
           LogicServerPtr& logic_server_ptr, memory_manager::ServerMemoryManager const& server_memory_manager,
           SlotMemoryConfig const& slot_memory_config, MemoryExchangeHandlePtr slot_memory_exchange_handle_ptr,
           MemoryExchangeHandlePtr queue_memory_exchange_handle_ptr, ReadWritableMemoryView queue_memory_view) noexcept;

  /*!
   * \brief       Destruct the receiver.
   * \details     The preconditions ensure that this receiver is only destructed when the end state is reached and when
   *              it's ensured that no asynchronous reception of messages is currently ongoing.
   *              As for any other object, the user of this class is responsible for general lifetime concerns
   *              (including captures of this receiver in other asynchronous tasks).
   * \context     ANY
   * \pre         The receiver must be in state ReceiverState::kDisconnected.
   * \pre         The receiver must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  ~Receiver() noexcept final;

  /*!
   * \brief Do not construct a Receiver by copying.
   * \vprivate Component Private
   */
  Receiver(Receiver const&) noexcept = delete;

  /*!
   * \brief Do not construct a Receiver by moving.
   * \vprivate Component Private
   */
  Receiver(Receiver&&) noexcept = delete;

  /*!
   * \brief Do not copy another Receiver into this instance.
   * \vprivate Component Private
   */
  auto operator=(Receiver const&) noexcept -> Receiver& = delete;

  /*!
   * \brief Do not move another Receiver into this instance.
   * \vprivate Component Private
   */
  auto operator=(Receiver&&) noexcept -> Receiver& = delete;

  /*!
   * \brief       Handle a connection ack.
   * \details     Implements state-dependent handling of connection acks.
   *              A connection ack is expected when the receiver is in state ReceiverState::kConnecting and after
   *              Connect() was called.
   *              Any error that happens when processing the connection ack results in a state transition
   *              to ReceiverState::kCorrupted.
   *              A connection ack in state in ReceiverState::kConnected results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A connection ack in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \param       client_queue_memory_config              Configuration of the client queue memory.
   * \param       client_queue_memory_exchange_handle_ptr Memory exchange handle of the client queue memory.
   *                                                      Must not be a nullptr. This is not checked.
   * \return      True, if a state transition happened. False, otherwise.
   * \context     REACTOR
   * \pre         Must not be called when the receiver is in state ReceiverState::kConnecting and before Connect() is
   *              called. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto OnAckConnection(QueueMemoryConfig client_queue_memory_config,
                       MemoryExchangeHandlePtr client_queue_memory_exchange_handle_ptr) noexcept -> bool;

  /*!
   * \brief       Handle a shutdown.
   * \details     Implements state-dependent handling of shutdowns.
   *              A shutdown in ReceiverState::kConnecting or ReceiverState::kConnected results in a state transition
   *              to ReceiverState::kDisconnected.
   *              If a shutdown occurs in ReceiverState::kConnected the receiver is deregistered from the logic server
   *              and the receiver handle is invalidated.
   *              A shutdown in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \return      True, if a state transition happened. False, otherwise.
   * \context     REACTOR
   * \pre         Must not be called when the receiver is in state ReceiverState::kConnecting and before Connect() is
   *              called. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto OnShutdown() noexcept -> bool;

  /*!
   * \brief       Handle a start listening.
   * \details     Implements state-dependent handling of start listenings.
   *              A start listening is expected in ReceiverState::kConnected alternating with stop listening. If two
   *              consecutive start listenings without stop listening in between are received, it results in a state
   *              transition to ReceiverState::kCorrupted.
   *              A start listening in ReceiverState::kConnecting results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A start listening in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \return      True, if a state transition happened. False, otherwise.
   * \context     REACTOR
   * \pre         Must not be called when the receiver is in state ReceiverState::kConnecting and before Connect() is
   *              called. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto OnStartListening() noexcept -> bool;

  /*!
   * \brief       Handle a stop listening.
   * \details     Implements state-dependent handling of stop listenings.
   *              A stop listening is expected in ReceiverState::kConnected alternating with start listening. If stop
   *              listening is triggered without a preceding start listening or two consecutive stop listenings without
   *              start listening in between are received, it results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A stop listening in ReceiverState::kConnecting results in a state transition to
   *              ReceiverState::kCorrupted.
   *              A stop listening in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \return      True, if a state transition happened. False, otherwise.
   * \context     REACTOR
   * \pre         Must not be called when the receiver is in state ReceiverState::kConnecting, this is not checked.
   * \pre         Must be called before Connect(), this is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto OnStopListening() noexcept -> bool;

  /*!
   * \brief       Handle an error.
   * \details     Implements state-dependent error handling.
   *              An error in any state (but ReceiverState::kCorrupted and ReceiverState::kDisconnected) results in a
   *              state transition to ReceiverState::kCorrupted.
   *              An error in ReceiverState::kCorrupted or ReceiverState::kDisconnected is ignored.
   * \param       error_code Error code identifying the cause of the error.
   * \return      True, if a state transition happened. False, otherwise.
   * \context     REACTOR
   * \pre         Must not be called when the receiver is in state ReceiverState::kConnecting and before Connect() is
   *              called. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto OnError(::amsr::core::ErrorCode const& error_code) noexcept -> bool;

  /*!
   * \brief       Start connecting to the receiver by triggering the asynchronous handshake.
   * \details     Implements state-dependent handling of a connection start trigger.
   *              The first protocol message is sent to the receiver containing slot memory configuration, slot memory
   *              exchange handle, server queue memory configuration and server queue memory exchange handle passed
   *              during construction of this receiver.
   *              Additionally, asynchronous reception is started and all passed callbacks can be called from now.
   *              If the asynchronous handshake is done successfully the ReceiverState will change to
   *              ReceiverState::kConnected.
   *              If any error occurs during the handshake, the receiver state will change to kCorrupted.
   *              May trigger a state transition.
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
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedReceiverState If the receiver is not
   *              in ReceiverState::kConnecting or if the receiver is in ReceiverState::kConnecting and this method has
   *              already been called before.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Connect(OnAckConnectionCallback on_ack_connection_cbk, OnStartListeningCallback on_start_listening_cbk,
               OnStopListeningCallback on_stop_listening_cbk, OnShutdownCallback on_shutdown_cbk) noexcept
      -> ::amsr::core::Result<void>;

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
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInUse() const noexcept -> bool;

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
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto Terminate() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief       Handle a server-side shutdown for this receiver.
   * \details     After a call to this method, the receiver is in state ReceiverState::kDisconnected.
   *              Additionally, all related callbacks are deregistered and thus, stops future asynchronous reception of
   *              messages. However, asynchronous reception of messages may currently be ongoing.
   *              If the receiver is already in state ReceiverState::kDisconnected, there is no action.
   *              A shutdown message is sent to the receiver if its current state is ReceiverState::kConnected or
   *              ReceiverState::kConnecting when Connect() was already called.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the receiver crashed.
   *              The receiver is now in state ReceiverState::kDisconnected.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server notices a protocol
   *              error during the communication with the server. The receiver is now in state
   *              ReceiverState::kDisconnected.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto HandleServerShutdown() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief       Notify the receiver that a new slot was sent and send a notification based on the current state.
   * \details     The receiver itself decides if a notification will be sent based on its internal state. Notifications
   *              are requested/stopped asynchronously with OnStartListening() and OnStopListening().
   *              Notifications are dropped if the message itself was dropped because of the receiver class limit.
   *              Notifications are only sent in state ReceiverState::kConnected when it was requested by the receiver.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the notification could not be
   *              sent for any reason.
   * \param       dropped_info Information on whether a slot was dropped for a receiver class.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto NotifyNewSlotSent(DroppedInformation const& dropped_info) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief       Get the current state of this receiver.
   * \return      State of this receiver.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReceiverState() const noexcept -> ReceiverState;

  /*!
   * \brief       Check and handle if this receiver caused a corruption in the Zero-Copy Logic.
   * \details     Implements state-dependent handling for a corruption detected by the Zero-Copy Logic.
   *              If a corruption in the Zero-Copy Logic was caused by this receiver, the receiver state will change to
   *              ReceiverState::kCorrupted.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  void CheckAndHandleLogicCorruption() noexcept;

  /*!
   * \brief       Get the ReceiverId of this receiver.
   * \return      ReceiverId of this receiver.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReceiverId() const noexcept -> ReceiverId;

  /*!
   * \brief       Get the current optional error code.
   * \return      The optional can only contain an error code if the current state is ReceiverState::kCorrupted or
   *              ReceiverState::kDisconnected.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetOptionalErrorCode() const noexcept -> ::amsr::core::Optional<::amsr::core::ErrorCode>;

 private:
  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateMachineBase::TransitionToConstructionHelper
   */
  void TransitionToConstructionHelper(ReceiverState to_state) noexcept final;

  /*!
   * \brief Optional ReceiverHandle (from logic) which has value when receiver was added to the Zero-Copy Logic.
   * \details The class invariant ensures that this optional contains a value when ReceiverState::kConnected is
   *          reached.
   *          Once valid, this optional contains a value until but excluding ReceiverState::kDisconnected is reached.
   */
  ::amsr::core::Optional<logic::internal::ReceiverHandle> opt_receiver_handle_{::amsr::core::nullopt};

  /*!
   * \brief The readable client queue memory.
   * \details The client queue memory is mapped when this pointer is valid.
   *          The class invariant ensures that this pointer is valid when ReceiverState::kConnected is reached.
   *          Once valid, this pointer stays valid until but excluding ReceiverState::kDisconnected is reached.
   */
  ReadableMemoryPtr readable_queue_memory_{nullptr};

  /*!
   * \brief The logger of this instance.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief ReceiverId for unique identification of this Receiver.
   */
  ReceiverId receiver_id_;

  /*!
   * \brief ClassHandle used for this receiver.
   */
  ClassHandle receiver_class_handle_;

  /*!
   * \brief ServerSideChannel used for this receiver.
   */
  side_channel::ServerSideChannel server_side_channel_;

  /*!
   * \brief Logic server instance.
   * \details This pointer stays valid during the whole lifetime of this receiver.
   */
  LogicServerPtr& logic_server_ptr_;
};

}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_RECEIVER_H_
