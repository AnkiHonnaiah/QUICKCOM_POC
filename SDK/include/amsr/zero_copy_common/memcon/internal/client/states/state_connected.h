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
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTED_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTED_H_

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/memcon/internal/client/states/state_base.h"
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
 * \brief StateConnected state.
 * \vprivate Component Private
 */
class StateConnected final : public StateBase {
 public:
  /*!
   * \brief Construct a StateConnected.
   * \details This state is intended to be managed by the given \p state_machine. Thus, it is expected that when this
   *          state is fully constructed, this state is the current state of the \p state_machine.
   * \param state_machine State machine that manages this state.
   *                      Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logger Logger of this state.
   *               Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param side_channel Side channel to communicate with the server.
   *                     Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logic_client Logic client of this state.
   *                     Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param slot_tokens_given_out Number of slot tokens given out.
   *                              Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  StateConnected(StateMachineBase& state_machine, ::ara::log::Logger& logger,
                 side_channel::ClientSideChannel& side_channel, LogicClientPtr& logic_client,
                 NumSlots::Type& slot_tokens_given_out) noexcept;

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
   * \brief Sub-states of the state connected.
   * \vprivate Component Private
   */
  enum class ConnectedState : std::uint8_t { kPolling = 0, kNotified = 1 };

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
   * \brief Handle a disconnect.
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
   * \brief The current sub-state of this state connected.
   */
  ConnectedState sub_state_{ConnectedState::kPolling};

  /*!
   * \brief The logger of this state connected.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The side channel of this state connected.
   */
  side_channel::ClientSideChannel& side_channel_;

  /*!
   * \brief The logic client of this state connected.
   */
  LogicClientPtr& logic_client_;

  /*!
   * \brief The number of slot tokens given out of this state connected.
   */
  NumSlots::Type& slot_tokens_given_out_;
};

}  // namespace states
}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CONNECTED_H_
