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
 *        \brief  Definition of class StateDisconnected.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_DISCONNECTED_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_DISCONNECTED_H_

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
 * \brief StateDisconnected state.
 * \vprivate Component Private
 */
class StateDisconnected final : public StateBase {
 public:
  /*!
   * \brief       Construct a StateDisconnected.
   * \details     This state is intended to be managed by the given \p state_machine. Thus, it is expected that when
   *              this state is fully constructed, this state is the current state of the \p state_machine.
   * \param       state_machine         State machine that manages this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       logger                Logger of this state.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \param       side_channel          Side channel to communicate with the server.
   *                                    Must stay valid for the whole lifetime of this state. This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  StateDisconnected(StateMachineBase& state_machine, ::ara::log::Logger& logger,
                    side_channel::ServerSideChannel& side_channel) noexcept;

  /*!
   * \brief Do not construct a StateDisconnected by copying.
   * \vprivate Component Private
   */
  StateDisconnected(StateDisconnected const&) noexcept = delete;

  /*!
   * \brief Do not construct a StateDisconnected by moving.
   * \vprivate Component Private
   */
  StateDisconnected(StateDisconnected&&) noexcept = delete;

  /*!
   * \brief Do not copy another StateDisconnected into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateDisconnected const&) noexcept -> StateDisconnected& = delete;

  /*!
   * \brief Do not move another StateDisconnected into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateDisconnected&&) noexcept -> StateDisconnected& = delete;

  /*!
   * \brief Destruct the StateDisconnected.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~StateDisconnected() noexcept final = default;

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
   * \brief The logger of this state disconnected.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The server side channel.
   */
  side_channel::ServerSideChannel& side_channel_;
};

}  // namespace states
}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_DISCONNECTED_H_
