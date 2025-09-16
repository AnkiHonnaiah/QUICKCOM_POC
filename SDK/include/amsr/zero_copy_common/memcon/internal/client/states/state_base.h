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
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_BASE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_BASE_H_

#include <cstdint>
#include <memory>
#include <tuple>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/logic/internal/client_interface.h"

#include "amsr/zero_copy_common/memcon/internal/client/client_interface.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_state.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_callbacks.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {
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
   * \brief Type used for a slot token.
   */
  using SlotToken = ClientInterface::SlotToken;

  /*!
   * \brief Type used for logic clients.
   */
  using LogicClientPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ClientInterface>;

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
   * \brief Handle a connection request.
   * \details Implements state-dependent handling of connection requests.
   *          May request a state transition.
   *          A connection request is expected when the client is in state ClientState::kConnecting and after Connect()
   *          was called. Any error that happens when processing the connection request results in a state transition
   *          to ClientState::kCorrupted.
   *          A connection request in any other client state (but ClientState::kCorrupted and
   *          ClientState::kDisconnected) results in a state transition to ClientState::kCorrupted.
   *          A connection request in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \param slot_memory_config Configuration of the slot memory.
   * \param slot_memory_exchange_handle Memory exchange handle of the slot memory.
   *                                    Must not be a nullptr. This is not checked.
   * \param server_queue_memory_config Configuration of the server queue memory.
   * \param server_queue_memory_exchange_handle Memory exchange handle of the server queue memory.
   *                                            Must not be a nullptr. This is not checked.
   * \context REACTOR
   * \pre Must not be called when the client is in state ClientState::kConnecting and before Connect() is called.
   *      This is not checked.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnConnectionRequestCallback(SlotMemoryConfig slot_memory_config,
                                           MemoryExchangeHandlePtr slot_memory_exchange_handle,
                                           QueueMemoryConfig server_queue_memory_config,
                                           MemoryExchangeHandlePtr server_queue_memory_exchange_handle) noexcept = 0;

  /*!
   * \brief Handle a queue initialization ack.
   * \details Implements state-dependent handling of queue initialization acks.
   *          May request a state transition.
   *          A queue initialization ack is expected when the client is in state ClientState::kConnecting and after a
   *          connection request was successfully processed.
   *          A queue initialization ack in any other client state (but ClientState::kCorrupted and
   *          ClientState::kDisconnected) results in a state transition to ClientState::kCorrupted.
   *          A queue initialization ack in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \return Optional containing a tuple of slot memory, server queue memory, client queue memory and the logic client,
   *         if the client is in state ClientState::kConnecting and a previous connection request was successfully
   *         processed. Empty optional, otherwise.
   * \context REACTOR
   * \pre Must not be called when the client is in state ClientState::kConnecting and before Connect() is called.
   *      This is not checked.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto OnAckQueueInitializationCallback() noexcept -> ::amsr::core::Optional<
      std::tuple<ReadableMemoryPtr, ReadableMemoryPtr, ReadWritableMemoryPtr, LogicClientPtr>> = 0;

  /*!
   * \brief Handle a shutdown.
   * \details Implements state-dependent handling of shutdowns.
   *          May request a state transition.
   *          A shutdown in ClientState::kConnecting results in a state transition to ClientState::kDisconnected.
   *          A shutdown in ClientState::kConnected results in a state transition to ClientState::kDisconnectedRemote.
   *          A shutdown in ClientState::kDisconnectedRemote results in a state transition to ClientState::kCorrupted.
   *          A shutdown in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \context REACTOR
   * \pre Must not be called when the client is in state ClientState::kConnecting and before Connect() is called.
   *      This is not checked.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnShutdownCallback() noexcept = 0;

  /*!
   * \brief Handle a termination.
   * \details Implements state-dependent handling of terminations.
   *          May request a state transition.
   *          A termination in any state (but ClientState::kCorrupted and ClientState::kDisconnected) results in a state
   *          transition to ClientState::kCorrupted.
   *          A termination in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \context REACTOR
   * \pre Must not be called when the client is in state ClientState::kConnecting and before Connect() is called.
   *      This is not checked.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnTerminationCallback() noexcept = 0;

  /*!
   * \brief Handle an error.
   * \details Implements state-dependent error handling.
   *          May request a state transition.
   *          Any error in ClientState::kConnecting or ClientState::kConnected results in a state transition to
   *          ClientState::kCorrupted.
   *          The error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError in state
   *          ClientState::kDisconnectedRemote is ignored. Any other error in ClientState::kDisconnectedRemote results
   *          in a state transition to ClientState::kCorrupted.
   *          Any error in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \param error_code Error code identifying the cause of the error.
   *                   Must be one of the following error codes:
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the peer
   *                       closed the connection without adhering to the protocol.
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the peer crashed.
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If any protocol error is
   *                       noticed during the communication with the peer.
   *                     This is not checked.
   * \context REACTOR
   * \pre Must not be called when the client is in state ClientState::kConnecting and before Connect() is called.
   *      This is not checked.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnErrorCallback(::amsr::core::ErrorCode error_code) noexcept = 0;

  /*!
   * \brief Connect the client.
   * \details After a successful call to this method, the client is ready to participate in the connection establishment
   *          handshake.
   *          The connection establishment handshake is initiated by the server.
   *          Once a state transition to a new state has happened, the OnStateTransitionCallback is called with the
   *          new state.
   *          May request a state transition.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnecting or if the client is in state ClientState::kConnecting and this method has already
   *        been called before.
   * \param cbks Callbacks that are called each time the corresponding message is received.
   *             Must not contain any nullptr.
   *             Must stay valid until it is not used anymore, i.e., IsInUse() returns false. This cannot be checked.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto Connect(side_channel::ClientCallbacks cbks) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Disconnect the client.
   * \details After a call to this method, the client is in state ClientState::kDisconnected.
   *          Stops future asynchronous reception of messages and notifications.
   *          Thus, this stops future calls to the OnStateTransitionCallback and the OnNotificationCallback.
   *          However, asynchronous reception of messages and notifications may currently be ongoing.
   *          May request a state transition.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is in state
   *        ClientState::kDisconnected.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kDisconnected.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kDisconnected.
   * \context ANY
   * \pre All received slot tokens must be released.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto Disconnect() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Get the client state that corresponds to this state.
   * \return Client state that corresponds to this state.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetClientState() const noexcept -> ClientState;

  /*!
   * \brief Start listening to notifications.
   * \details After a successful call to this method, the client is still in ClientState::kConnected but in notified
   *          mode.
   *          Requests the server to start sending a notification each time a slot is sent to this client.
   *          The OnNotificationCallback is called each time when a notification is received.
   *          May request a state transition.
   * \param on_notification_callback Callback that is called each time when a notification is received.
   *                                 Must not contain a nullptr. This is not checked.
   *                                 Must stay valid until it is not used anymore. This cannot be checked.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected and polling mode or if a previously configured OnNotificationCallback is still in
   *        use, i.e. IsOnNotificationCallbackInUse() returns true.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kCorrupted.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto StartListening(OnNotificationCallback on_notification_callback) noexcept
      -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Stop listening to notifications.
   * \details After a successful call to this method, the client is still in ClientState::kConnected but in polling
   *          mode.
   *          Requests the server to stop sending a notification each time a slot is sent to this client.
   *          Stops future asynchronous reception of notifications.
   *          Thus, this stops future calls to the OnNotificationCallback.
   *          However, asynchronous reception of notifications may currently be ongoing.
   *          May request a state transition.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected and notified mode.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kCorrupted.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto StopListening() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Receive a slot that was sent from the server.
   * \details A received slot may be used with the AccessSlotContent() and ReleaseSlot() methods.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          This receive operation must synchronize with ServerInterface::SendSlot() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, after the server has sent a slot by calling ServerInterface::SendSlot(), all clients belonging
   *          to the same Zero-Copy instance must be able to receive the sent slot.
   *          Note that either all clients that are not rate-limited are able to receive a slot or none is.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   *          May request a state transition.
   * \return Optional containing a SlotToken representing the received slot if available, empty Optional otherwise.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected or ClientState::kDisconnectedRemote.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server. Duplicate reception of a slot is treated as a protocol error.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto ReceiveSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> = 0;

  /*!
   * \brief Access the content of a received slot.
   * \details Access is read-only.
   * \param token SlotToken representing the slot to access the content for.
   * \return Read-only view of the content of the slot represented by the \p token.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is in state
   *        ClientState::kCorrupted.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto AccessSlotContent(SlotToken const& token) const noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> = 0;

  /*!
   * \brief Release a previously received slot to the server.
   * \details A released slot may be received again.
   *          This operation requires to give-up ownership of the \p token.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          This release operation must synchronize with ServerInterface::ReclaimSlots() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, if a client releases a slot using this method, the server belonging to the same Zero-Copy
   *          instance is able to reclaim the slot from the client.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   *          May request a state transition.
   * \param token SlotToken representing the slot to release.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto ReleaseSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> = 0;

 protected:
  /*!
   * \brief Construct a StateBase.
   * \details This state is intended to be managed by the given \p state_machine. Thus, it is expected that when this
   *          state is fully constructed, this state is the current state of the \p state_machine.
   * \param state_machine State machine that manages this state.
   *                      Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logger Logger of this state.
   *               Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param client_state Client state that corresponds to this state.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  StateBase(StateMachineBase& state_machine, ::ara::log::Logger& logger, ClientState client_state) noexcept;

  /*!
   * \brief Log and request a transition to another state.
   * \param to_client_state Client state specifying the state to transition to.
   *                        Must not be ClientState::kConnecting. This is not checked.
   *                        Must not be the currently active state. This is not checked.
   * \param opt_error_code  Optional containing an error code if a state transition to ReceiverState::kCorrupted is
   *                        requested.
   *                        Must be one of the following error codes:
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                            peer closed the connection without adhering to the protocol.
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the peer
   *                            crashed.
   *                          - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If any protocol
   *                            error is noticed during the communication with the peer.
   *                        This is not checked.
   * \context ANY
   * \pre There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogAndTransitionToRequest(ClientState to_client_state,
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
   * \brief The client state that corresponds to this state.
   */
  ClientState client_state_;
};

/*!
 * \brief Stream any state to a log stream.
 * \param stream Log stream to stream to.
 * \param state State to stream to the log stream.
 * \return The same log stream as \p stream.
 * \context ANY
 * \pre -
 *
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator<<(::ara::log::LogStream& stream, StateBase const& state) noexcept -> ::ara::log::LogStream&;

}  // namespace states
}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_BASE_H_
