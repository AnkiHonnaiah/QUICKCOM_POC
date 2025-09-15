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
 *        \brief  Definition of class StateCorrupted.
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CORRUPTED_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CORRUPTED_H_

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "ara/log/logger.h"

#include "amsr/zero_copy_common/memcon/internal/client/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_callbacks.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {
namespace states {

/*!
 * \brief StateCorrupted state.
 * \vprivate Component Private
 */
class StateCorrupted final : public StateBase {
 public:
  /*!
   * \brief Construct a StateCorrupted.
   * \details This state is intended to be managed by the given \p state_machine. Thus, it is expected that when this
   *          state is fully constructed, this state is the current state of the \p state_machine.
   * \param state_machine State machine that manages this state.
   *                      Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logger Logger of this state.
   *               Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param logic_client Logic client of this state. May be a nullptr.
   *                     If not a nullptr, must stay valid for the whole lifetime of this state. This cannot be checked.
   *                     If not a nullptr, must have been informed that the communication with the server is erroneous.
   *                     This is not checked.
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
  StateCorrupted(StateMachineBase& state_machine, ::ara::log::Logger& logger, LogicClientPtr& logic_client,
                 NumSlots::Type& slot_tokens_given_out) noexcept;

  /*!
   * \brief Do not construct a StateCorrupted by copying.
   * \vprivate Component Private
   */
  StateCorrupted(StateCorrupted const&) noexcept = delete;

  /*!
   * \brief Do not construct a StateCorrupted by moving.
   * \vprivate Component Private
   */
  StateCorrupted(StateCorrupted&&) noexcept = delete;

  /*!
   * \brief Do not copy another StateCorrupted into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateCorrupted const&) noexcept -> StateCorrupted& = delete;

  /*!
   * \brief Do not move another StateCorrupted into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateCorrupted&&) noexcept -> StateCorrupted& = delete;

  /*!
   * \brief Destruct the StateCorrupted.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~StateCorrupted() noexcept final = default;

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
   * \brief The logger of this state corrupted.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The logic client of this state corrupted.
   */
  LogicClientPtr& logic_client_;

  /*!
   * \brief The number of slot tokens given out of this state corrupted.
   */
  NumSlots::Type& slot_tokens_given_out_;
};

}  // namespace states
}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_STATES_STATE_CORRUPTED_H_
