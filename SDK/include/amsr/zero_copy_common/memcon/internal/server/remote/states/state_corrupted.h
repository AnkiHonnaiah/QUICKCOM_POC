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
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CORRUPTED_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CORRUPTED_H_

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
 * \brief StateCorrupted state.
 * \vprivate Component Private
 */
class StateCorrupted final : public StateBase {
 public:
  /*!
   * \brief       Construct a StateCorrupted.
   * \details     This state is intended to be managed by the given \p state_machine. Thus, it is expected that when
   *              this state is fully constructed, this state is the current state of the \p state_machine.
   * \param       state_machine         State machine that manages this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logger                Logger of this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logic_server_ptr      Logic server instance the receiver belongs to.
   *                                    Must not contain a nullptr. This is NOT CHECKED as this is a unit-internal API.
   *                                    Must stay valid for the whole lifetime of the receiver. This cannot be checked.
   * \param       opt_receiver_handle   Receiver handle of the receiver this state belongs to.
   *                                    Must be valid for the \p logic_server_ptr. This cannot be checked.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  StateCorrupted(StateMachineBase& state_machine, ::ara::log::Logger& logger, LogicServerPtr& logic_server_ptr,
                 ::amsr::core::Optional<logic::internal::ReceiverHandle>& opt_receiver_handle) noexcept;

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
   * \brief The logger of this state corrupted.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The zero-copy logic server.
   * \details This reference is valid for the whole lifetime of this receiver.
   */
  LogicServerPtr& logic_server_ptr_;

  /*!
   * \brief The receiver handle which represents this receiver in the logic server if it was already registered.
   * \details This reference is valid for the whole lifetime of this receiver.
   *          If the optional contains a value depends on its previous state. If the previous state was
   *          ReceiverState::kConnected it contains an value, otherwise it's empty.
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

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_CORRUPTED_H_
