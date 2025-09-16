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
 *        \brief  Definition of class Server.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *   \complexity  The high cyclomatic complexity of the class Server from the equally named unit results from the high
 *                number of methods provided to the user and the internal error and state handling. A significant part
 *                of the complexity is caused by precondition checks according to the API pattern which are considered
 *                trivial and can be tested easily.
 *                From design perspective, the side channel, the memory management and the Zero-Copy logic are already
 *                moved to other units to reduce complexity. The receiver handling with the corresponding state pattern
 *                is intended to be part of the Server and also the resources used for Zero-Copy communication are
 *                shared within this unit. Therefore, the risk is considered low and there wouldn't be any benefit by
 *                changing the design.
 *
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_H_

#include <cstdint>
#include <memory>

#include "vac/container/static_vector.h"
#include "vac/testing/test_adapter.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/common/logging_cfg.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_technology.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/server_memory_manager.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/receiver.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_interface.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_state.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/server_side_channel.h"

#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"

#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

class Server final : public ServerInterface {
 public:
  /*!
   * \brief Type used for a handle.
   */
  using Handle = ::amsr::zero_copy_common::logic::internal::Handle;

  /*!
   * \brief       Construct a Server.
   * \details     Intended to be used by the respective builder only.
   * \param       group                            Group of handles for the server.
   * \param       number_of_slots                  Number of slots.
   *                                               Must be greater than zero.
   * \param       slot_content_size                Slot content size.
   *                                               Must be greater than zero.
   * \param       slot_content_alignment           Slot content alignment.
   *                                               Must be greater than zero.
   *                                               Must be a power of two.
   * \param       memory_technology                Memory technology.
   * \param       max_number_receivers             Maximum number of receivers.
   *                                               Must be greater than zero.
   *                                               Must not exceed std::numeric_limits<std::ptrdiff_t>::max().
   * \param       server_memory_manager            Server memory manager used for allocation and mapping.
   *                                               Must be configured with \p number_of_slots, \p slot_content_size,
   *                                               \p slot_content_alignment, \p max_number_receivers and
   *                                               \p memory_technology. This cannot be checked.
   * \param       slot_memory                      Read-writable slot memory.
   *                                               Must not contain a nullptr.
   *                                               Must be allocated with the \p server_memory_manager. This cannot be
   *                                               checked.
   * \param       server_queue_memory              Read-writable server queue memory.
   *                                               Must not contain a nullptr.
   *                                               Must be allocated with the \p server_memory_manager. This cannot be
   *                                               checked.
   * \param       logic_server_ptr                 The logic server.
   *                                               Must not contain a nullptr.
   *                                               The slot descriptors configured for this \p logic_server_ptr must
   *                                               refer to the \p slot_memory and must be added in sequential ascending
   *                                               order. This is not checked.
   *                                               The slot descriptors must each refer to the \p slot_content_size
   *                                               and \p slot_content_alignment. This is not checked.
   *                                               Must be configured with the same parameters \p group,
   *                                               \p number_of_slots and \p max_number_receivers as this instance. This
   *                                               is not checked.
   * \param       on_receiver_state_transition_cbk Callback that is called each time a state transition happens.
   *                                               Must not contain a nullptr.
   *                                               Must stay valid for the whole lifetime of this server. This cannot be
   *                                               checked.
   *
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-InitializationServer
   * \vprivate    Component private
   */
  Server(Handle::Group const& group, NumSlots number_of_slots, SlotContentSize slot_content_size,
         SlotContentAlignment slot_content_alignment, MemoryTechnology memory_technology,
         MaxNumberReceivers max_number_receivers, memory_manager::ServerMemoryManager server_memory_manager,
         ReadWritableMemoryPtr slot_memory, ReadWritableMemoryPtr server_queue_memory, LogicServerPtr logic_server_ptr,
         OnReceiverStateTransitionCallback on_receiver_state_transition_cbk) noexcept;

  /*!
   * \brief       Destruct the Server.
   * \details     The preconditions ensure that this server is only destructed when the end state is reached and when
   *              it's ensured that no asynchronous reception of messages is currently ongoing.
   *              As for any other object, the user of this class is responsible for general lifetime concerns
   *              (including captures of this server in other asynchronous tasks).
   * \context     ANY
   * \pre         The server must be shut down, i.e. GetServerState() must return ServerState::kDisconnected.
   * \pre         The server must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  ~Server() noexcept final;

  /*!
   * \brief Do not construct a Server by copying.
   * \vprivate Product Private
   */
  Server(Server const&) noexcept = delete;

  /*!
   * \brief Do not construct a Server by moving.
   * \vprivate Product Private
   */
  Server(Server&&) noexcept = delete;

  /*!
   * \brief Do not copy another Server into this instance.
   * \vprivate Product Private
   */
  auto operator=(Server const&) noexcept -> Server& = delete;

  /*!
   * \brief Do not move another Server into this instance.
   * \vprivate Product Private
   */
  auto operator=(Server&&) noexcept -> Server& = delete;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::GetServerState
   */
  auto GetServerState() const noexcept -> ServerState final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::CanAddReceiver
   */
  auto CanAddReceiver() const noexcept -> ::amsr::core::Result<bool> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::AddReceiver
   */
  auto AddReceiver(ClassHandle receiver_class_handle, std::unique_ptr<SideChannelInterface> side_channel_ptr) noexcept
      -> ::amsr::core::Result<ReceiverId> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::ConnectReceiver
   */
  auto ConnectReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::GetReceiverState
   */
  auto GetReceiverState(ReceiverId const& receiver_id) const noexcept
      -> std::pair<ReceiverState, ::amsr::core::Optional<::amsr::core::ErrorCode>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::TerminateReceiver
   */
  auto TerminateReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::IsReceiverInUse
   */
  auto IsReceiverInUse(ReceiverId const& receiver_id) const noexcept -> ::amsr::core::Result<bool> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::RemoveReceiver
   */
  auto RemoveReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::Shutdown
   */
  auto Shutdown() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::AcquireSlot
   */
  auto AcquireSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::UnacquireSlot
   */
  auto UnacquireSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::AccessSlotContent
   */
  auto AccessSlotContent(SlotToken const& token) noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::SendSlot
   */
  auto SendSlot(SlotToken token, DroppedInformation& dropped_info) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc amsr::zero_copy_common::memcon::internal::server::ServerInterface::ReclaimSlots
   */
  auto ReclaimSlots() noexcept -> ::amsr::core::Result<void> final;

 private:
  /*!
   * \brief Type used for the receiver container.
   */
  using ReceiverContainer = ::vac::container::StaticVector<::amsr::core::Optional<remote::Receiver>>;

  /*!
   * \brief       Call the given callable and call the OnReceiverStateTransitionCallback if a state transition happened.
   * \details     The transition request must have been requested using the TransitionToRequest() method.
   *              Does not call the OnReceiverStateTransitionCallback, if no state transition was requested and handled.
   * \tparam      F Type of the callable.
   * \param       receiver_index Index to the receiver the callable must be called for.
   *                             The given index must refer to a valid receiver object for this unit. This is NOT
   *                             CHECKED as this is a unit-internal API.
   * \param       callable       Callable to be called. The return parameter of the callable returns true if a state
   *                             transition has happened inside, false otherwise.
   *                             The callable must have the signature bool(remote::Receiver&). This is not checked.
   * \context     REACTOR
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  template <typename F>
  void CallAndInformAboutStateTransition(MaxNumberReceivers::Type receiver_index, F&& callable) noexcept;

  /*!
   * \brief       Check if any asynchronous operation on this server instance is currently ongoing.
   * \details     Note that this function always returns true if Shutdown() was not called yet. Additionally,
   *              asynchronous reception of messages for any receiver may currently be ongoing even after a call to
   *              Shutdown().
   *              When this function returns false once, it will always return false for all future calls.
   * \return      True if any asynchronous operation on this server is ongoing, false otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto IsInUseHelper() const noexcept -> bool;

  /*!
   * \brief       Assert that the given receiver id is valid for this instance.
   * \param       receiver_id Receiver id to check for validity.
   *                          Must be valid for this instance which means that it must refer to an active receiver.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void AssertReceiverIdValid(ReceiverId const& receiver_id) const noexcept;

  /*!
   * \brief Mutex to secure the exclusive access to the class members.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief Free running counter to get unique receiver ids.
   */
  ReceiverId::IdType receiver_id_counter_{0};

  /*!
   * \brief Local server state.
   */
  ServerState server_state_{ServerState::kConnected};

  /*!
   * \brief The number of currently given out slot tokens.
   */
  NumSlots::Type slot_tokens_given_out_{0};

  /*!
   * \brief Container to store receiver objects.
   */
  ReceiverContainer receivers_{};

  /*!
   * \brief Group of handles for the server.
   */
  Handle::Group group_;

  /*!
   * \brief Number of slots.
   */
  NumSlots number_of_slots_;

  /*!
   * \brief Slot content size.
   */
  SlotContentSize slot_content_size_;

  /*!
   * \brief Slot content alignment.
   */
  SlotContentAlignment slot_content_alignment_;

  /*!
   * \brief Memory technology.
   */
  // VCA_ZEROCOPYCOMMON_UNUSED_PRIVAT_FIELD_FOR_FUTURE_USE
  MemoryTechnology memory_technology_;

  /*!
   * \brief Maximum number of receivers.
   */
  MaxNumberReceivers max_number_receivers_;

  /*!
   * \brief Server memory manager used for allocations and memory mappings.
   */
  memory_manager::ServerMemoryManager memory_manager_;

  /*!
   * \brief The read-writable slot memory.
   * \details The slot memory is mapped when this pointer is valid which is ensured during ServerState::kConnected.
   *          This pointer stays valid until ServerState::kDisconnected is reached, i.e. Shutdown() is called.
   */
  ReadWritableMemoryPtr slot_memory_;

  /*!
   * \brief The read-writable server queue memory.
   * \details The server queue memory is mapped when this pointer is valid which is ensured during
   *          ServerState::kConnected.
   *          This pointer stays valid until ServerState::kDisconnected is reached, i.e. Shutdown() is called.
   */
  ReadWritableMemoryPtr server_queue_memory_;

  /*!
   * \brief The logic server.
   * \details It is ensured that this pointer is valid during ServerState::kConnected.
   *          This pointer stays valid until ServerState::kDisconnected is reached, i.e. Shutdown() is called.
   */
  LogicServerPtr logic_server_ptr_;

  /*!
   * \brief   Callback that is called each time a receiver state transition happened.
   * \details The corresponding receiver id and the new receiver state is passed to the callback.
   */
  OnReceiverStateTransitionCallback on_receiver_state_transition_cbk_;

  /*!
   * \brief The logger of this server.
   */
  ::ara::log::Logger& logger_;

  /*! Friend test declaration */
  FRIEND_TEST(StatesCorruptedDisconnected__P, ReceiverCorruptedDisconnectedAsynchronousCallbacks);
};

/*!
 * \exclusivearea ::amsr::zero_copy_common::memcon::internal::server::Server::mutex_
 * Serializes all the Server API calls and the asynchronous callbacks triggered from the side channel.
 *
 * \protects All members of a Server object.
 * \usedin   ::amsr::zero_copy_common::memcon::internal::server::Server::~Server
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::GetServerState
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::CanAddReceiver
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::AddReceiver
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::ConnectReceiver
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::GetReceiverState
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::TerminateReceiver
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::IsReceiverInUse
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::RemoveReceiver
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::Shutdown
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::IsInUse
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::AcquireSlot
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::UnacquireSlot
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::AccessSlotContent
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::SendSlot
 *           ::amsr::zero_copy_common::memcon::internal::server::Server::ReclaimSlots
 * \exclude  ::amsr::zero_copy_common::memcon::internal::server::Server::OnReceiverStateTransitionCallback
 * \length   LONG The lock is held for the complete API calls except the invocation of the
 *           OnReceiverStateTransitionCallback to the user context.
 * \endexclusivearea
 */

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_H_
