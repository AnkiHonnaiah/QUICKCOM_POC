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
/*!       \file
 *        \brief Definition of class ClientSideChannel.
 *        \unit ZeroCopyCommon::MemCon::SideChannel
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_CLIENT_SIDE_CHANNEL_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_CLIENT_SIDE_CHANNEL_H_

#include <cstdint>
#include <memory>
#include <mutex>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message_router.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/client_callbacks.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace side_channel {

// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_SideChannel
/*!
 * \brief Client-side implementation of a side channel.
 * \trace DSGN-MemCon-SideChannelProtocol
 * \vprivate Component Private
 */
class ClientSideChannel final
    : private ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter {
 public:
  /*!
   * \brief Construct a ClientSideChannel.
   * \param side_channel_ptr Side channel to communicate with the server.
   *                         Must not be a nullptr.
   * \param on_error_cbk Callback that is called each time an error on the side channel is encountered.
   *                     Must not contain a nullptr.
   *                     Must stay valid until it is not used anymore, i.e., IsInUse() returns false.
   *                     This cannot be checked.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ClientSideChannel(std::unique_ptr<SideChannelInterface> side_channel_ptr, OnErrorCallback on_error_cbk) noexcept;

  /*!
   * \brief Do not construct a ClientSideChannel by copying.
   * \vprivate Component Private
   */
  ClientSideChannel(ClientSideChannel const&) = delete;

  /*!
   * \brief Do not construct a ClientSideChannel by moving.
   * \vprivate Component Private
   */
  ClientSideChannel(ClientSideChannel&&) = delete;

  /*!
   * \brief Do not copy another ClientSideChannel into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientSideChannel const&) -> ClientSideChannel& = delete;

  /*!
   * \brief Do not move another ClientSideChannel into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientSideChannel&&) -> ClientSideChannel& = delete;

  /*!
   * \brief Destruct the ClientSideChannel.
   * \context ANY
   * \pre The client side channel must not be in use, i.e., IsInUse() must return false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~ClientSideChannel() noexcept;

  /*!
   * \brief Start to receive messages.
   * \details Messages are received asynchronously.
   *          The respective callback of \p cbks is called each time the corresponding message is received.
   * \param cbks Callbacks that are called each time the corresponding message is received.
   *             Must not contain any nullptr.
   *             Must stay valid until it is not used anymore, i.e., IsInUse() returns false. This cannot be checked.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \pre Must only be called once.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void StartMessageReception(ClientCallbacks cbks) noexcept;

  /*!
   * \brief Query whether the OnNotificationCallback is in use.
   * \details The OnNotificationCallback is in use when there was a call to StartListening() that was not followed by a
   *          call to StopListening() or Close().
   *          Additionally, a previously registered OnNotificationCallback may still be in use even after a call to
   *          StopListening() or Close(). This function checks both conditions.
   * \return True, if a OnNotificationCallback is in use. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOnNotificationCallbackInUse() const noexcept -> bool;

  /*!
   * \brief Send a StartListening message and register a callback to be notified.
   * \details Notifications are received asynchronously.
   *          The \p on_notification_callback is called each time a notification is received.
   * \param on_notification_cbk Callback that is called each time a notification is received.
   *                            Must not contain a nullptr.
   *                            Must stay valid until it is not used anymore, i.e., IsOnNotificationCallbackInUse() or
   *                            IsInUse() return false. This cannot be checked.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *        closed the connection.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer crashed.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be sent for
   *        any other reason.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \pre If a callback was previously registered using this function, it must be deregistered using the StopListening()
   *      function.
   * \pre If a callback was previously registered using this function, it must no longer be in use.
   *      I.e., IsOnNotificationCallbackInUse() must return false.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   */
  auto StartListening(OnNotificationCallback on_notification_cbk) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Deregister the callback to be notified and optionally send a StopListening message.
   * \details An error can only occur when \p send_message is set to true.
   * \param send_message Indicates whether a StopListening message should be sent.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *        closed the connection.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer crashed.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be sent for
   *        any other reason.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \pre Must only be called when there was a call to StartListening() and not yet a call to StopListening() after.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   */
  auto StopListening(bool send_message) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Close the side channel.
   * \details Deregisters all callbacks. Stops future calls to callbacks but callbacks may currently be executing.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   */
  void Close() noexcept;

  /*!
   * \brief Query whether the side channel is open, i.e. Close() was not called before.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   */
  auto IsOpen() const noexcept -> bool;

  /*!
   * \brief Query whether the side channel is in use.
   * \details The side channel is in use when there was not yet a call to Close().
   *          Additionally, if there was a call to Close(), a callback may still be in use even after a call to Close().
   *          This function checks both conditions.
   * \return True, if the side channel is in use. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Component Private
   */
  auto IsInUse() const noexcept -> bool;

  /*!
   * \brief Send an AckConnection message.
   * \param queue_memory_config Queue configuration to be sent.
   * \param queue_memory_exchange_handle Memory exchange handle for the queue memory to be sent.
   *                                     Must not be a nullptr.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *        closed the connection.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer crashed.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be sent for
   *        any other reason.
   * \context ANY
   * \pre Must not be called after calling Close().
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto AckConnection(QueueMemoryConfig queue_memory_config,
                     MemoryExchangeHandlePtr queue_memory_exchange_handle) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Send a Shutdown message.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the communication peer
   *        closed the connection.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer crashed.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could not be sent for
   *        any other reason.
   * \context ANY
   * \pre Must not be called after calling Close().
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto Shutdown() noexcept -> ::amsr::core::Result<void>;

 private:
  /*!
   * \brief Type used for the connection status of the side channel.
   */
  enum class ConnectionStatus : std::uint8_t { kOpen = 0, kClosed = 1 };

  /*!
   * \brief Type used for the buffer of the connection request slot memory.
   */
  struct ConnectionRequestSlotMemoryBuffer {
    SlotMemoryConfig slot_memory_config{};
    MemoryExchangeHandlePtr slot_memory_exchange_handle{nullptr};
  };

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
   * \brief Query whether client callbacks are present.
   * \return True, if client callbacks are present. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AreClientCallbacksPresent() const noexcept -> bool;

  /*!
   * \brief Query whether the side channel is in use.
   * \details The side channel is in use when there was not yet a call to Close().
   *          Additionally, if there was a call to Close(), a callback may still be in use even after a call to Close().
   *          This function checks both conditions.
   * \return True, if the side channel is in use. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsInUseHelper() const noexcept -> bool;

  /*!
   * \brief Query whether the OnNotificationCallback is present.
   * \return True, if the OnNotificationCallback is present. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsOnNotificationCallbackPresent() const noexcept -> bool;

  /*!
   * \brief Query whether the reception of a connection request is in progress.
   * \return True, if the reception of a connection request is in progress. False, otherwise.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsConnectionRequestReceptionInProgress() const noexcept -> bool;

  /*!
   * \brief Reset the reception of a connection request that is in progress.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ConnectionRequestReceptionReset() noexcept;

  /*!
   * \brief Call the error callback with the given error code.
   * \param error_code Error code to pass to the error callback.
   * \context REACTOR
   * \pre The mutex must be locked when calling this function. This cannot be checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void CallOnErrorCallback(::amsr::core::ErrorCode error_code) noexcept;

  /*!
   * \brief Helper used as the receive callback.
   * \details Parses the received message and calls the respective callback with the received information (if any).
   *          If any error on the side channel is encountered, the user-provided error callback is called with the
   *          respective error code.
   * \param result_message Result containing a message that contains a view to the received byte data and optionally a
   *                       memory exchange handle. The memory exchange handle may be a nullptr.
   *                       The view must be valid during the execution of this callback. This cannot be checked.
   *                       Otherwise, the result must contain an error code indicating the cause of the error:
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                           communication peer closed the connection.
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                           communication peer crashed.
   *                         - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the message could
   *                           not be received for any other reason.
   *                         This cannot be checked.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReceiveCallbackHelper(::amsr::core::Result<Message> result_message) noexcept;

  /*!
   * \brief Helper used as the notification callback.
   * \details Calls the user-provided notification callback, if registered. Otherwise, the notification is dropped.
   * \param result_notification Result indicating whether a valid notification was received.
   *                            A positive result indicates that a valid notification was received.
   *                            Otherwise, the result must contain the error code
   *                            ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError.
   *                            This is not checked.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void NotificationCallbackHelper(::amsr::core::Result<void> result_notification) noexcept;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnConnectionRequestSlotMemory
   */
  void OnConnectionRequestSlotMemory(::amsr::core::Result<protocol_message::ConnectionRequestSlotMemory> const& result,
                                     MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnConnectionRequestQueueMemory
   */
  void OnConnectionRequestQueueMemory(
      ::amsr::core::Result<protocol_message::ConnectionRequestQueueMemory> const& result,
      MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnAckQueueInitialization
   */
  void OnAckQueueInitialization(::amsr::core::Result<protocol_message::AckQueueInitialization> const& result,
                                MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnShutdown
   */
  void OnShutdown(::amsr::core::Result<protocol_message::Shutdown> const& result,
                  MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnTermination
   */
  void OnTermination(::amsr::core::Result<protocol_message::Termination> const& result,
                     MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnAckConnection
   */
  void OnAckConnection(::amsr::core::Result<protocol_message::AckConnection> const& result,
                       MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnStartListening
   */
  void OnStartListening(::amsr::core::Result<protocol_message::StartListening> const& result,
                        MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnStopListening
   */
  void OnStopListening(::amsr::core::Result<protocol_message::StopListening> const& result,
                       MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \brief The mutex of this instance.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief Connection status of this client side channel.
   * \details Accessed in ANY context.
   */
  ConnectionStatus connection_status_{ConnectionStatus::kOpen};

  /*!
   * \brief Optional containing a slot memory config extracted from the first part of the connection request message.
   * \details Set on OnConnectionRequestSlotMemory().
   *          Unset on any other callback.
   *          If set, this indicates that the reception of a connection request is in progress. I.e., the first part
   *          of the connection request message was already received, the second part of the connection request message
   *          is expected to be received next.
   *          Accessed in ANY context.
   */
  ::amsr::core::Optional<ConnectionRequestSlotMemoryBuffer> opt_connection_request_slot_memory_{::amsr::core::nullopt};

  /*!
   * \brief Callback that is called each time a notification is received.
   * \details Set on StartListening().
   *          Unset on StopListening() or Close().
   *          Accessed in ANY context.
   */
  ::amsr::core::Optional<OnNotificationCallback> opt_on_notification_cbk_{::amsr::core::nullopt};

  /*!
   * \brief Callbacks that are called when a message is received.
   * \details Set on StartReceiving().
   *          Unset on Close().
   *          Accessed in ANY context.
   */
  ::amsr::core::Optional<ClientCallbacks> opt_client_cbks_{::amsr::core::nullopt};

  /*!
   * \brief Side channel to communicate with the server.
   * \details Accessed in ANY context.
   */
  std::unique_ptr<SideChannelInterface> const side_channel_ptr_;

  /*!
   * \brief Callback that is called when any error occurs on this side channel.
   * \details Accessed in REACTOR context.
   */
  OnErrorCallback on_error_cbk_;

  /*!
   * Friend declaration for testing purposes.
   */
  FRIEND_TEST(UT__SideChannel__ClientSideChannel, ServerSideOnMessages);
};

/*!
 * \exclusivearea ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::mutex_
 * Serializes all the ClientSideChannel API calls and the asynchronous callbacks triggered from the side channel.
 *
 * \protects All members of a ClientSideChannel object.
 * \usedin   ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::~ClientSideChannel
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::StartMessageReception
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::IsOnNotificationCallbackInUse
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::StartListening
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::StopListening
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::Close
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::IsInUse
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::AckConnection
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::Shutdown
 * \exclude  ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnErrorCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnConnectionRequestCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnAckQueueInitializationCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnShutdownCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnTerminationCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ClientSideChannel::OnNotificationCallback
 * \length   LONG The lock is held for the complete API calls except the invocation of the OnErrorCallback,
 *           OnConnectionRequestCallback, OnAckQueueInitializationCallback, OnShutdownCallback, OnTerminationCallback
 *           and OnNotificationCallback to the user context.
 * \endexclusivearea
 */

}  // namespace side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_CLIENT_SIDE_CHANNEL_H_
