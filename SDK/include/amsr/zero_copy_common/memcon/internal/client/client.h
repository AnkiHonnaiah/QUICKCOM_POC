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
 *        \brief  Definition of class Client.
 *         \unit  ZeroCopyCommon::MemCon::Client
 *   \complexity  The high complexity of the class Client from the equally named unit results from:
 *                - High cyclomatic complexity caused by the number of methods provided to the user and the proper
 *                  internal error and state handling. Also precondition checks cause a high amount of complexity which
 *                  are considered trivial.
 *                - High maximum call-tree complexity caused by the Connect() function which introduces many callbacks
 *                  triggering the state-based handling. The used state pattern which simplifies the maintainability of
 *                  the code results in a higher call-tree complexity by delegating calls to the actual state. The
 *                  metric limit is only exceeded slightly which is considered as a low risk.
 *                - High number of inter-module calls is caused by getting views from the memory handles of the
 *                  libosabstraction. The metric is only exceeded by 1 and there is no risk detected.
 *                In summary, due to the used state pattern and the multiple APIs provided to the user the exceeded
 *                complexity is considered to only introduce a low risk which is verified by testing.
 *                Changes in the design won't be beneficial as all shared resources are combined in the Client unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_H_

#include <cstdint>
#include <memory>
#include <mutex>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/logic/internal/client_interface.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"

#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_common/memcon/internal/client/client_interface.h"
#include "amsr/zero_copy_common/memcon/internal/client/states/state_machine_base.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_side_channel.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief Implementation of ClientInterface.
 * \vprivate Component Private
 */
// VECTOR NL AutosarC++17_10-A10.1.1: MD_ZEROCOPYCOMMON_A10.1.1_multipleInheritanceEssentiallyInterface
class Client final : public ClientInterface, private states::StateMachineBase {
 public:
  /*!
   * \brief Base type used for handles.
   */
  using Handle = logic::internal::Handle;

  /*!
   * \brief Type used for the side channel interface.
   */
  using SideChannelInterface = ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface;

  /*!
   * \brief Construct a Client.
   * \details Intended to be used by the respective builder only.
   * \param group Group of handles for the client.
   * \param opt_slot_content_size Optional slot content size.
   *                              Must be greater than zero.
   * \param opt_slot_content_alignment Optional slot content alignment.
   *                                   Must be greater than zero.
   *                                   Must be a power of two.
   * \param memory_technology Memory technology.
   * \param on_state_transition_callback Callback that is called each time a state transition happens.
   *                                     Must not contain a nullptr.
   *                                     Must stay valid for the whole lifetime of this client. This cannot be checked.
   * \param side_channel_ptr Side channel to communicate with the server.
   *                         Must not be a nullptr.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-InitializationClient
   * \vprivate Component Private
   */
  Client(Handle::Group group, ::amsr::core::Optional<SlotContentSize> opt_slot_content_size,
         ::amsr::core::Optional<SlotContentAlignment> opt_slot_content_alignment, MemoryTechnology memory_technology,
         OnStateTransitionCallback on_state_transition_callback,
         std::unique_ptr<SideChannelInterface> side_channel_ptr) noexcept;

  /*!
   * \brief Do not construct a Client by copying.
   * \vprivate Component Private
   */
  Client(Client const&) noexcept = delete;

  /*!
   * \brief Do not construct a Client by moving.
   * \vprivate Component Private
   */
  Client(Client&&) noexcept = delete;

  /*!
   * \brief Do not copy another Client into this instance.
   * \vprivate Component Private
   */
  auto operator=(Client const&) noexcept -> Client& = delete;

  /*!
   * \brief Do not move another Client into this instance.
   * \vprivate Component Private
   */
  auto operator=(Client&&) noexcept -> Client& = delete;

  /*!
   * \brief Destruct the Client.
   * \details The preconditions ensure that this client is only destructed when the end state is reached and when it's
   *          ensured that no asynchronous reception of messages or notifications is currently ongoing.
   *          As for any other object, the user of this class is responsible for general lifetime concerns (including
   *          captures of this client in other asynchronous tasks).
   * \context ANY
   * \pre The client must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~Client() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::Connect
   */
  auto Connect() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::Shutdown
   */
  auto Disconnect() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::IsOnNotificationCallbackInUse
   */
  auto IsOnNotificationCallbackInUse() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::GetState
   */
  auto GetState() const noexcept -> ClientState final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::StartListening
   */
  auto StartListening(OnNotificationCallback on_notification_callback) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::StopListening
   */
  auto StopListening() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::ReceiveSlot
   */
  auto ReceiveSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::AccessSlotContent
   */
  auto AccessSlotContent(SlotToken const& token) const noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::ReleaseSlot
   */
  auto ReleaseSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::GetSlotMemoryResourceDescriptor
   */
  auto GetSlotMemoryResourceDescriptor() const noexcept -> ::amsr::core::Result<ReadableMemoryResourceDescriptor> final;

 private:
  /*!
   * \brief Type used for logic clients.
   */
  using LogicClientPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ClientInterface>;

  /*!
   * \brief Query whether the client is in use to receive messages or notifications.
   * \details The client is in use when there was not yet a call to Disconnect(). Additionally, asynchronous reception
   *          of messages and notifications may currently be ongoing even after a call to Disconnect(). This function
   *          checks both conditions.
   * \return True, if the client is in use to receive messages or notifications. False, otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsInUseHelper() const noexcept -> bool;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateMachineBase::TransitionToConstructionHelper
   */
  void TransitionToConstructionHelper(ClientState to_state) noexcept final;

  /*!
   * \brief Call the given callable, handle the last transition request and call the OnStateTransitionCallback.
   * \details The transition request must have been requested using the TransitionToRequest() method.
   *          Does not call the OnStateTransitionCallback, if no state transition was requested and handled.
   * \tparam F Type of the callable.
   * \param callable Callable to be called.
   *                 The callable must have the signature void(). This is not checked.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename F>
  void CallAndInformAboutStateTransition(F&& callable) noexcept;

  /*!
   * \brief Handle a connection request.
   * \details Implements state-dependent handling of connection requests and optionally calls the user-provided
   *          OnStateTransitionCallback, if the handling of the connection request resulted in a state transition.
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
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnConnectionRequestCallbackHelper(SlotMemoryConfig slot_memory_config,
                                         MemoryExchangeHandlePtr slot_memory_exchange_handle,
                                         QueueMemoryConfig server_queue_memory_config,
                                         MemoryExchangeHandlePtr server_queue_memory_exchange_handle) noexcept;

  /*!
   * \brief Handle a queue initialization ack.
   * \details Implements state-dependent handling of queue initialization acks and optionally calls the user-provided
   *          OnStateTransitionCallback, if the handling of the queue initialization ack resulted in a state transition.
   *          A queue initialization ack is expected when the client is in state ClientState::kConnecting and after a
   *          connection request was successfully processed.
   *          A queue initialization ack in any other client state (but ClientState::kCorrupted and
   *          ClientState::kDisconnected) results in a state transition to ClientState::kCorrupted.
   *          A queue initialization ack in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnAckQueueInitializationCallbackHelper() noexcept;

  /*!
   * \brief Handle a shutdown.
   * \details Implements state-dependent handling of shutdowns and optionally calls the user-provided
   *          OnStateTransitionCallback, if the handling of the shutdown resulted in a state transition.
   *          A shutdown in ClientState::kConnecting results in a state transition to ClientState::kDisconnected.
   *          A shutdown in ClientState::kConnected results in a state transition to ClientState::kDisconnectedRemote.
   *          A shutdown in ClientState::kDisconnectedRemote results in a state transition to ClientState::kCorrupted.
   *          A shutdown in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnShutdownCallbackHelper() noexcept;

  /*!
   * \brief Handle a termination.
   * \details Implements state-dependent handling of terminations and optionally calls the user-provided
   *          OnStateTransitionCallback, if the handling of the termination resulted in a state transition.
   *          A termination in any state (but ClientState::kCorrupted and ClientState::kDisconnected) results in a state
   *          transition to ClientState::kCorrupted.
   *          A termination in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnTerminationCallbackHelper() noexcept;

  /*!
   * \brief Handle an error.
   * \details Implements state-dependent error handling and optionally calls the user-provided
   *          OnStateTransitionCallback, if the handling of the error resulted in a state transition.
   *          An error in any state (but ClientState::kCorrupted and ClientState::kDisconnected) results in a state
   *          transition to ClientState::kCorrupted.
   *          An error in ClientState::kCorrupted or ClientState::kDisconnected is ignored.
   * \param error_code Error code identifying the cause of the error.
   *                   Must be one of the following error codes:
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the peer
   *                       closed the connection without adhering to the protocol.
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the peer crashed.
   *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If any protocol error is
   *                       noticed during the communication with the peer.
   *                     This is not checked.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnErrorCallbackHelper(::amsr::core::ErrorCode error_code) noexcept;

  /*!
   * \brief The mutex of this instance.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief The number of currently given out slot tokens.
   */
  NumSlots::Type slot_tokens_given_out_{0};

  /*!
   * \brief The readable slot memory.
   * \details The slot memory is mapped when this pointer is valid.
   *          The class invariant ensures that this pointer is valid when ClientState::kConnected is reached.
   *          Once valid, this pointer stays valid until but excluding ClientState::kDisconnected is reached.
   */
  ReadableMemoryPtr slot_memory_{nullptr};

  /*!
   * \brief The readable server queue memory.
   * \details The server queue memory is mapped when this pointer is valid.
   *          The class invariant ensures that this pointer is valid when ClientState::kConnected is reached.
   *          Once valid, this pointer stays valid until but excluding ClientState::kDisconnected is reached.
   */
  ReadableMemoryPtr server_queue_memory_{nullptr};

  /*!
   * \brief The read writable client queue memory.
   * \details The client queue memory is mapped when this pointer is valid.
   *          The class invariant ensures that this pointer is valid when ClientState::kConnected is reached.
   *          Once valid, this pointer stays valid until but excluding ClientState::kDisconnected is reached.
   */
  ReadWritableMemoryPtr client_queue_memory_{nullptr};

  /*!
   * \brief The logic client.
   * \details The class invariant ensures that this pointer is valid when ClientState::kConnected is reached.
   *          Once valid, this pointer stays valid until but excluding ClientState::kDisconnected is reached.
   */
  LogicClientPtr logic_client_{nullptr};

  /*!
   * \brief The logger of this client.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief Callback that is called each time a state transition happened.
   * \details The new client state is passed to the callback.
   */
  OnStateTransitionCallback on_state_transition_callback_;

  /*!
   * \brief The side channel of this client.
   */
  side_channel::ClientSideChannel side_channel_;

  /*! Friend test declaration */
  FRIEND_TEST(ClientStatesCorruptedDisconnected__P, AllCallbacks);
};

/*!
 * \exclusivearea ::amsr::zero_copy_common::memcon::internal::client::Client::mutex_
 * Serializes all the Client API calls and the asynchronous callbacks triggered from the side channel.
 *
 * \protects All members of a Client object.
 * \usedin   ::amsr::zero_copy_common::memcon::internal::client::Client::~Client
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::Connect
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::Disconnect
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::IsOnNotificationCallbackInUse
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::IsInUse
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::GetState
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::StartListening
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::StopListening
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::ReceiveSlot
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::AccessSlotContent
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::ReleaseSlot
 *           ::amsr::zero_copy_common::memcon::internal::client::Client::GetSlotMemoryResourceDescriptor
 * \exclude  ::amsr::zero_copy_common::memcon::internal::client::Client::OnStateTransitionCallback
 * \length   LONG The lock is held for the complete API calls except the invocation of the OnStateTransitionCallback
 *           to the user context.
 * \endexclusivearea
 */

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_H_
