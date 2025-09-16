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
 *        \brief  Definition of class SideChannel.
 *         \unit  ZeroCopyCommon::MemCon::SafeIPCSideChannel
 *   \complexity  The high complexity of the class SideChannel from the SafeIPCSideChannel unit results from:
 *                - High max level weighted complexity and a high maximum nesting level for the StartMessageReception
 *                  method. The increased nesting level can be traced back to the mocking setup when considering the
 *                  analyzed call stack. Without the mocking setup the complexity metric would not be violated. Thus,
 *                  no action is required.
 *                - High number of inter-module calls which is also caused by the mocking setup. If the inter-module
 *                  calls from the mocking setup are subtracted, the complexity metric is not violated. Thus, no action
 *                  is required.
 *                In summary, the violation of the complexity metrics mentioned above can be traced back to the
 *                mocking setup causing some additional inter-module calls and an increased nesting level.
 *                Excluding the inter-module calls introduced by the mocking, the limit is not exceeded. Therefore,
 *                there is no risk for the production code.
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_H_

#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "amsr/zero_copy_common/external/ipc/connection.h"

#include "amsr/zero_copy_common/memcon/internal/safe_ipc_side_channel/side_channel_impl.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace safe_ipc_side_channel {

/*!
 * \brief Implementation of SideChannelInterface.
 * \vprivate Product Private
 */
class SideChannel final : public side_channel::SideChannelInterface {
 public:
  /*!
   * \brief Construct a SideChannel.
   * \details The connection is closed after it is used by the side channel.
   * \param connection SafeIPC connection to communication with the communication peer.
   *                   Must be connected.
   *                   The send buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize().
   *                   The receive buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize().
   *                   No callbacks must be registered and the data channel must be free from unrelated messages,
   *                   otherwise it will result in an error.
   *                   Must not be used for the whole lifetime of this side channel. This cannot be checked.
   *                   Must stay valid for the whole lifetime of this side channel. This cannot be checked.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-SafeIPCSideChannel
   * \vprivate Product Private
   */
  explicit SideChannel(::amsr::zero_copy_common::external::ipc::Connection& connection) noexcept;

  /*!
   * \brief Do not construct a SideChannel by copying.
   * \vprivate Product Private
   */
  SideChannel(SideChannel const&) noexcept = delete;

  /*!
   * \brief Do not construct a SideChannel by moving.
   * \vprivate Product Private
   */
  SideChannel(SideChannel&&) noexcept = delete;

  /*!
   * \brief Do not copy another SideChannel into this instance.
   * \vprivate Product Private
   */
  auto operator=(SideChannel const&) noexcept -> SideChannel& = delete;

  /*!
   * \brief Do not move another SideChannel into this instance.
   * \vprivate Product Private
   */
  auto operator=(SideChannel&&) noexcept -> SideChannel& = delete;

  /*!
   * \brief Destruct the SideChannel.
   * \details The preconditions ensure that this side channel is only destructed when it's ensured that no asynchronous
   *          reception of messages or notifications is currently ongoing.
   * \context ANY
   * \pre The side channel must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ~SideChannel() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::Send
   */
  auto Send(Message message) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::Notify
   */
  auto Notify() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::StartMessageReception
   */
  void StartMessageReception(OnMessageCallback on_message_cbk) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::IsOnNotificationCallbackInUse
   */
  auto IsOnNotificationCallbackInUse() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::RegisterOnNotificationCallback
   */
  void RegisterOnNotificationCallback(OnNotificationExtendedCallback on_notification_cbk) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::DeregisterOnNotificationCallback
   */
  void DeregisterOnNotificationCallback() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::Close
   */
  void Close() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

 private:
  /*!
   * \brief Implementation of the side channel.
   * \details Ownership of the side channel implementation is shared between this side channel and all
   *          SafeIPC connection callbacks that the side channel implementation registers itself to. This prevents the
   *          side channel implementation object to be destructed while it is still in use by the SafeIPC connection.
   */
  std::shared_ptr<SideChannelImpl> side_channel_impl_;
};

}  // namespace safe_ipc_side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_H_
