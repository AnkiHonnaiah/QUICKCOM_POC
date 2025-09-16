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
 *        \brief  Definition of class SideChannelImpl.
 *         \unit  ZeroCopyCommon::MemCon::SafeIPCSideChannel
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_IMPL_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_IMPL_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "amsr/ipc/ipc_types.h"
#include "amsr/zero_copy_common/external/ipc/connection.h"

#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "osabstraction/io/io_buffer.h"

#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace safe_ipc_side_channel {

/*!
 * \brief Implementation of a side channel using a SafeIPC connection.
 * \vprivate Component Private
 */
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_SideChannelImpl
class SideChannelImpl final : public std::enable_shared_from_this<SideChannelImpl> {
 public:
  /*!
   * \brief Type used for a message.
   */
  using Message = side_channel::SideChannelInterface::Message;

  /*!
   * \brief Type used for the callback that is called each time a notification is received.
   * \param result_notification Result indicating whether a valid notification was received.
   *                            Only notifications with value 0x00 are valid.
   *                            A positive result indicates that a valid notification was received.
   *                            Otherwise, the result contains the error code
   *                            ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-SafeIPCSideChannel
   * \vprivate Component Private
   */
  using OnNotificationExtendedCallback = side_channel::SideChannelInterface::OnNotificationExtendedCallback;

  /*!
   * \brief Type used for the callback that is called each time a message is received.
   * \param result_message Result containing a message that contains a view to the received byte data and optionally a
   *                       memory exchange handle. The memory exchange handle may be a nullptr.
   *                       The view is only valid during the execution of this callback.
   *                       Otherwise, the result contains an error code indicating the cause of the error:
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                           communication peer closed the connection.
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                           communication peer crashed.
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could
   *                           not be received for any other reason.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  using OnMessageCallback = side_channel::SideChannelInterface::OnMessageCallback;

  /*!
   * \brief Construct a SideChannelImpl.
   * \details The connection is closed after it is used by the side channel.
   * \param connection SafeIPC connection to communication with the communication peer.
   *                   Must be connected.
   *                   The send buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize().
   *                   The receive buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize().
   *                   Must not be used for the whole lifetime of this side channel. This cannot be checked.
   *                   Must stay valid for the whole lifetime of this side channel. This cannot be checked.
   * \return Shared pointer to the constructed SideChannelImpl. Never a nullptr.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   */
  static auto Construct(::amsr::zero_copy_common::external::ipc::Connection& connection) noexcept
      -> std::shared_ptr<SideChannelImpl>;

  /*!
   * \brief Do not construct a SideChannelImpl by copying.
   * \vprivate Component Private
   */
  SideChannelImpl(SideChannelImpl const&) noexcept = delete;

  /*!
   * \brief Do not construct a SideChannelImpl by moving.
   * \vprivate Component Private
   */
  SideChannelImpl(SideChannelImpl&&) noexcept = delete;

  /*!
   * \brief Do not copy another SideChannelImpl into this instance.
   * \vprivate Component Private
   */
  auto operator=(SideChannelImpl const&) noexcept -> SideChannelImpl& = delete;

  /*!
   * \brief Do not move another SideChannelImpl into this instance.
   * \vprivate Component Private
   */
  auto operator=(SideChannelImpl&&) noexcept -> SideChannelImpl& = delete;

  /*!
   * \brief Destruct the SideChannelImpl.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTION_CALLED
  ~SideChannelImpl() noexcept = default;

  /*!
   * \brief Send a message.
   * \details The message is sent synchronously.
   *          A message consists of byte data (may be zero bytes) and optionally a memory exchange handle.
   *          The communication peer can receive messages using the OnMessageCallback.
   * \param message Message to be sent.
   *                The size of the byte data must not exceed the maximum message size, i.e., the size of the byte data
   *                must be less than the value returned by GetMaxMessageSize().
   *                If a memory exchange handle is sent, the message must contain a valid memory exchange handle.
   *                Otherwise, the memory exchange handle must be a nullptr. This cannot be checked.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *        closed the connection.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer crashed.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be sent for
   *        any other reason.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Send(Message message) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Notify the communication peer.
   * \details The notification is sent synchronously.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the notification could not be sent
   *        due to a protocol violation.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kDroppedNotificationError If the notification could
   *        not be sent due to resource limits.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Notify() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Start to receive messages.
   * \details Messages are received asynchronously.
   *          A message consists of byte data (may be zero bytes) and optionally a memory exchange handle.
   * \param on_message_cbk Callback that is called each time a message is received.
   *                       Must not contain a nullptr.
   *                       Must be valid until it is not used anymore. This cannot be checked.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \pre Must only be called once.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void StartMessageReception(OnMessageCallback on_message_cbk) noexcept;

  /*!
   * \brief Query whether the OnNotificationCallback is in use.
   * \details The OnNotificationCallback is in use when there was not yet a call to DeregisterOnNotificationCallback()
   *          or Close(). Additionally, asynchronous reception of a notification may currently be ongoing even after a
   *          call to DeregisterOnNotificationCallback() or Close(). This function checks both conditions.
   * \return True, if the OnNotificationCallback is in use to receive notifications. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsOnNotificationCallbackInUse() const noexcept -> bool;

  /*!
   * \brief Register a OnNotificationCallback to receive notifications.
   * \details Notifications are received asynchronously.
   * \param on_notification_cbk Callback that is called each time a notification is received.
   *                            Must not contain a nullptr.
   *                            Must be valid until it is not used anymore. This cannot be checked.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \pre The previously registered OnNotificationCallback must not still be in use.
   *      I.e., IsOnNotificationCallbackInUse() must return false.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RegisterOnNotificationCallback(OnNotificationExtendedCallback on_notification_cbk) noexcept;

  /*!
   * \brief Deregister the OnNotificationCallback to receive notifications.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \pre There must have been a call to RegisterOnNotificationCallback() and not yet a call to
   *      DeregisterOnNotificationCallback() afterwards.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void DeregisterOnNotificationCallback() noexcept;

  /*!
   * \brief Close the side channel.
   * \details Deregisters all callbacks.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Close() noexcept;

  /*!
   * \brief Query whether the side channel is in use to receive messages or notifications.
   * \details The side channel is in use to receive messages or notifications when there was not yet a call to Close().
   *          Additionally, asynchronous reception of a notification may currently be ongoing even after a call to
   *          Close(). This function checks both conditions.
   * \return True, if the side channel is in use to receive messages or notifications. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   * requires true;
   * \endspec
   */
  auto IsInUse() const noexcept -> bool;

 private:
  /*!
   * \brief Type used for connection status of the side channel.
   */
  enum class ConnectionStatus : std::uint8_t { kOpen = 0, kClosed = 1 };

  /*!
   * \brief Type used for the message buffer and view used for message reception.
   */
  struct BufferAndView {
    ::vac::memory::allocator::MemoryBufferPtr memory_buffer{nullptr};
    ::osabstraction::io::MutableIOBuffer memory_buffer_view{nullptr, 0U};
  };

  /*!
   * \brief Construct a SideChannelImpl.
   * \details The connection is closed after it is used by the side channel.
   * \param connection SafeIPC connection to communication with the communication peer.
   *                   Must be connected. This is not checked.
   *                   The send buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize(). This is not checked.
   *                   The receive buffer size must not be less than the maximum message size, as defined by
   *                   side_channel::SideChannelInterface::GetMaxMessageSize(). This is not checked.
   *                   Must not be used for the whole lifetime of this side channel. This cannot be checked.
   *                   Must stay valid for the whole lifetime of this side channel. This cannot be checked.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit SideChannelImpl(::amsr::zero_copy_common::external::ipc::Connection& connection) noexcept;

  /*!
   * \brief Assert that the connection status of this side channel is ConnectionStatus::kOpen.
   * \context ANY
   * \pre The connection status of this side channel must be ConnectionStatus::kOpen.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertConnectionOpen() const noexcept;

  /*!
   * \brief Query whether the callback that is called each time a message is received is registered.
   * \return True, if the callback that is called each time a message is received is registered. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsOnMessageCallbackRegistered() const noexcept -> bool;

  /*!
   * \brief Query whether the callback that is called each time a notification is received is registered.
   * \return True, if the callback that is called each time a notification is received is registered. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsOnNotificationCallbackRegistered() const noexcept -> bool;

  /*!
   * \brief Query whether the OnNotificationCallback is in use.
   * \details The OnNotificationCallback is in use when there was not yet a call to DeregisterOnNotificationCallback()
   *          or Close(). Additionally, asynchronous reception of a notification may currently be ongoing even after a
   *          call to DeregisterOnNotificationCallback() or Close(). This function checks both conditions.
   * \return True, if the OnNotificationCallback is in use to receive notifications. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsOnNotificationCallbackInUseHelper() const noexcept -> bool;

  /*!
   * \brief Helper used as the callback to receive notifications from the SafeIPC connection.
   * \details Calls the user-provided callback that is called each time a notification is received, if present.
   *          If the value of the notification matches the expected notification value, the user-provided callback is
   *          called with a positive result. Otherwise, the user-provided callback is called with a negative result
   *          containing ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError.
   *          See the CDD of ::amsr::ipc::NotificationCallback for the SafeIPC specification of this callback.
   * \param notification Received notification.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnNotificationCallbackHelper(::amsr::ipc::Notification notification) noexcept;

  /*!
   * \brief Helper used as the callback for the SafeIPC connection that is called when a message is available.
   * \details See the CDD of ::amsr::ipc::MessageAvailableCallback for the SafeIPC specification of this callback.
   * \param message_size Message size in byte of the available message.
   * \return View to a single IO buffer that points to the single memory segment where the available message can be
   *         copied to.
   *         The view is valid for the whole lifetime of this side channel.
   *         The pointed-to memory segment has at least the size of
   *         side_channel::SideChannelInterface::GetMaxMessageSize().
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnMessageAvailableCallbackHelper(std::size_t message_size) noexcept
      -> ::amsr::core::Span<::osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief Helper used as the callback for the SafeIPC connection that is called when a message is received.
   * \details Calls the user-provided callback that is called each time a message is received, if present.
   *          If the receive_result contains a positive result, the user-provided callback is called with a positive
   *          result containing a view to the received message and optionally a memory exchange handle.
   *          Otherwise, the user-provided callback is called with a negative result containing
   *            - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *              closed the connection.
   *            - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer
   *              crashed.
   *            - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be
   *              received for any other reason.
   *          Continues to receive messages asynchronously afterwards, if the side channel has not been closed in the
   *          meantime.
   *          See the CDD of ::amsr::ipc::ReceiveCompletionExtendedCallback for the SafeIPC specification of this
   *          callback.
   * \param receive_result Result of the message reception.
   *                       Either a positive result containing the size of the received message and an optional memory
   *                       exchange handle or a negative result with an error code indicating the cause for the error.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnReceiveCompletionExtendedCallbackHelper(
      ::amsr::core::Result<::amsr::ipc::ReceiveResult> receive_result) noexcept;

  /*!
   * \brief Receive a message on the SafeIPC connection asynchronously.
   * \context ANY
   * \pre Must only be called when it was not yet called at all or when a message was received asynchronously after each
   *      call to this function.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReceiveMessage() noexcept;

  /*!
   * \brief The mutex of this instance.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief Notification value to be sent and to be received.
   * \details If another notification value is received, this is treated as an error.
   */
  ::amsr::ipc::Notification notification_{0U};

  /*!
   * \brief Connection status of this side channel.
   * \details The connection status is ConnectionStatus::kOpen until Close() is called. Then the connection status is
   *          ConnectionStatus::kClosed.
   */
  ConnectionStatus connection_status_{ConnectionStatus::kOpen};

  /*!
   * \brief Optional containing the callback that is called each time a message is received.
   * \details The class invariant ensures that this optional contains a non-nullptr value after calling
   *          StartMessageReception(). Once this optional contains a value, the value stays present until but excluding
   *          Close() is called.
   */
  ::amsr::core::Optional<OnMessageCallback> opt_on_message_cbk_{::amsr::core::nullopt};

  /*!
   * \brief Bool indicating whether the callback that is called each time a message is received is currently executing.
   */
  bool on_message_cbk_executing_{false};

  /*!
   * \brief Optional containing the callback that is called each time a notification is received.
   * \details The class invariant ensures that this optional contains a non-nullptr value after calling
   *          StartListening(). Once this optional contains a value, the value stays present until but excluding
   *          StopListening() or Close() is called.
   */
  ::amsr::core::Optional<OnNotificationExtendedCallback> opt_on_notification_cbk_{::amsr::core::nullopt};

  /*!
   * \brief Bool indicating whether the callback that is called each time a notification is received is currently
   *        executing.
   */
  bool on_notification_cbk_executing_{false};

  /*!
   * \brief SafeIPC connection to communicate with the communication peer.
   */
  ::amsr::zero_copy_common::external::ipc::Connection& connection_;

  /*!
   * \brief Message buffer and view used for message reception.
   */
  BufferAndView message_buffer_and_view_;
};

/*!
 * \exclusivearea ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::mutex_
 * Serializes all the SideChannelImpl API calls and the asynchronous callbacks triggered from the Connection.
 *
 * \protects All members of a SideChannelImpl object.
 * \usedin   ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::Send
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::Notify
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::StartMessageReception
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::IsOnNotificationCallbackInUse
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::RegisterOnNotificationCallback
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::DeregisterOnNotificationCallback
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::Close
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::IsInUse
 * \exclude  ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::OnMessageCallback
 *           ::amsr::zero_copy_common::memcon::internal::safe_ipc_side_channel::SideChannelImpl::OnNotificationExtendedCallback
 * \length   LONG The lock is held for the complete API calls except the invocation of the OnMessageCallback and the
 *           OnNotificationExtendedCallback to the user context.
 * \endexclusivearea
 */

}  // namespace safe_ipc_side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SAFE_IPC_SIDE_CHANNEL_SIDE_CHANNEL_IMPL_H_
