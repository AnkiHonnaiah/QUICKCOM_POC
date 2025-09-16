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
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTING_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTING_H_

#include <cstdint>
#include <memory>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/logic/internal/client_interface.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"

#include "amsr/zero_copy_common/memcon/internal/client/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/client_memory_manager.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_callbacks.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_side_channel.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {
namespace states {

/*!
 * \brief StateConnecting state.
 * \vprivate Component Private
 */
class StateConnecting final : public StateBase {
 public:
  /*!
   * \brief Base type used for handles.
   */
  using Handle = logic::internal::Handle;

  /*!
   * \brief Construct a StateConnecting.
   * \details This state is intended to be managed by the given \p state_machine. Thus, it is expected that when this
   *          state is fully constructed, this state is the current state of the \p state_machine.
   * \param state_machine State machine that manages this state.
   *                      Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logger Logger of this state.
   *               Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param side_channel Side channel to communicate with the server.
   *                     Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param group Group of handles.
   * \param opt_slot_content_size Optional slot content size.
   *                              Must be greater than zero. This is not checked.
   * \param opt_slot_content_alignment Optional slot content alignment.
   *                                   Must be greater than zero. This is not checked.
   * \param memory_technology Memory technology.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  StateConnecting(StateMachineBase& state_machine, ::ara::log::Logger& logger,
                  side_channel::ClientSideChannel& side_channel, Handle::Group group,
                  ::amsr::core::Optional<SlotContentSize> opt_slot_content_size,
                  ::amsr::core::Optional<SlotContentAlignment> opt_slot_content_alignment,
                  MemoryTechnology memory_technology) noexcept;

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
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::OnConnectionRequestCallback
   */
  void OnConnectionRequestCallback(SlotMemoryConfig slot_memory_config,
                                   MemoryExchangeHandlePtr slot_memory_exchange_handle,
                                   QueueMemoryConfig server_queue_memory_config,
                                   MemoryExchangeHandlePtr server_queue_memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::OnAckQueueInitializationCallback
   */
  auto OnAckQueueInitializationCallback() noexcept -> ::amsr::core::Optional<
      std::tuple<ReadableMemoryPtr, ReadableMemoryPtr, ReadWritableMemoryPtr, LogicClientPtr>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::OnShutdownCallback
   */
  void OnShutdownCallback() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::OnTerminationCallback
   */
  void OnTerminationCallback() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::OnErrorCallback
   */
  void OnErrorCallback(::amsr::core::ErrorCode error_code) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::Connect
   */
  auto Connect(side_channel::ClientCallbacks cbks) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::Disconnect
   */
  auto Disconnect() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::StartListening
   */
  auto StartListening(OnNotificationCallback on_notification_callback) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::StopListening
   */
  auto StopListening() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::ReceiveSlot
   */
  auto ReceiveSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::AccessSlotContent
   */
  auto AccessSlotContent(SlotToken const& token) const noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::states::StateBase::ReleaseSlot
   */
  auto ReleaseSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> final;

 private:
  /*!
   * \brief Type used for writing queue ends.
   */
  using WritingQueueEndPtr =
      ::amsr::zero_copy_common::memcon::internal::memory_manager::ClientMemoryManager::WritingQueueEndPtr;

  /*!
   * \brief Type used for reading queue ends.
   */
  using ReadingQueueEndPtr =
      ::amsr::zero_copy_common::memcon::internal::memory_manager::ClientMemoryManager::ReadingQueueEndPtr;

  /*!
   * \brief Type used for Readable slot descriptors.
   */
  using ReadableSlotDescriptorPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadableSlotDescriptor>;

  /*!
   * \brief Sub-states of the state connecting.
   * \vprivate Component Private
   */
  enum class ConnectingState : std::uint8_t { kNotConnected = 0, kInitial = 1, kIntermediate = 2, kAborted = 3 };

  /*!
   * \brief Handle a corruption.
   * \details Performs tasks to leave the current state in order to transition to ClientState::kCorrupted.
   * \param error_code Error which caused the corruption of the client.
   *                   Must be one of the following errors:
   *                    - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                      server closed the connection without adhering to the protocol.
   *                    - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                      server crashed.
   *                    - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client
   *                      notices a protocol error during the communication with the server.
   *                   This is not checked.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void HandleCorrupted(::amsr::core::ErrorCode error_code) noexcept;

  /*!
   * \brief Handle a disconnect or shutdown.
   * \details Performs tasks to leave the current state in order to transition to ClientState::kDisconnected.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void HandleDisconnected() noexcept;

  /*!
   * \brief Prepare client queue memory, slot memory and server queue memory.
   * \details Allocates client queue memory, maps slot memory and maps server queue memory.
   * \param slot_memory_exchange_handle Memory exchange handle of the slot memory.
   *                                    Must not be a nullptr. This is not checked.
   * \param server_queue_memory_exchange_handle Memory exchange handle of the server queue memory.
   *                                            Must not be a nullptr. This is not checked.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the memory could not be prepared for
   *        any reason.
   * \context ANY
   * \pre There must be a MemoryManager present. This is not checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto PrepareMemory(MemoryExchangeHandlePtr slot_memory_exchange_handle,
                     MemoryExchangeHandlePtr server_queue_memory_exchange_handle) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Prepare and validate the slot memory configuration.
   * \details Allocates client queue memory, maps slot memory and maps server queue memory.
   * \param slot_memory_config Slot memory config to validate locally configured values or getting missing configuration
   *                           values.
   * \return TRUE if the slot memory configuration is valid, FALSE otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ValidateSlotMemoryConfig(SlotMemoryConfig const& slot_memory_config) noexcept -> bool;

  /*!
   * \brief Send an ack connection and handle possible errors.
   * \details Depending on whether or not an error occurred, the sub state is either ConnectingState::kIntermediate or
   *          ConnectingState::kAborted or a state transition to ClientState::kCorrupted is requested.
   *          May request a state transition.
   * \context ANY
   * \pre There must be a MemoryManager present. This is not checked.
   * \pre There must be client queue memory present. This is not checked.
   * \pre The OS must support the creation of memory exchange handles.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SendAckConnection() noexcept;

  /*!
   * \brief The current sub-state of this state connecting.
   */
  ConnectingState sub_state_{ConnectingState::kNotConnected};

  /*!
   * \brief The number of slots.
   * \details The class invariant ensures that this optional contains a value when ConnectingState::kIntermediate
   *          is reached.
   */
  ::amsr::core::Optional<NumSlots> opt_number_slots_{::amsr::core::nullopt};

  /*!
   * \brief The client memory manager.
   * \details The class invariant ensures that this optional contains a value when ConnectingState::kIntermediate
   *          is reached.
   */
  ::amsr::core::Optional<memory_manager::ClientMemoryManager> opt_memory_manager_{::amsr::core::nullopt};

  /*!
   * \brief The readable slot memory.
   * \details The class invariant ensures that this pointer is valid when ConnectingState::kIntermediate is reached.
   */
  ReadableMemoryPtr slot_memory_{nullptr};

  /*!
   * \brief The readable server queue memory.
   * \details The class invariant ensures that this pointer is valid when ConnectingState::kIntermediate is reached.
   */
  ReadableMemoryPtr server_queue_memory_{nullptr};

  /*!
   * \brief The read-writable client queue memory.
   * \details The class invariant ensures that this pointer is valid when ConnectingState::kIntermediate is reached.
   */
  ReadWritableMemoryPtr client_queue_memory_{nullptr};

  /*!
   * \brief The free queue constructed from the server and client queue memory.
   * \details The class invariant ensures that this pointer is valid when ConnectingState::kIntermediate is reached.
   */
  WritingQueueEndPtr free_queue_{nullptr};

  /*!
   * \brief The available queue constructed from the server and client queue memory.
   * \details The class invariant ensures that this pointer is valid when ConnectingState::kIntermediate is reached.
   */
  ReadingQueueEndPtr available_queue_{nullptr};

  /*!
   * \brief The logger of this state connecting.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The side channel of this state connecting.
   */
  side_channel::ClientSideChannel& side_channel_;

  /*!
   * \brief The handle group of this state connecting.
   */
  Handle::Group group_;

  /*!
   * \brief The optional slot content size.
   */
  ::amsr::core::Optional<SlotContentSize> opt_slot_content_size_;

  /*!
   * \brief The optional slot content alignment.
   */
  ::amsr::core::Optional<SlotContentAlignment> opt_slot_content_alignment_;

  /*!
   * \brief The memory technology.
   */
  MemoryTechnology memory_technology_;
};

}  // namespace states
}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTING_H_
