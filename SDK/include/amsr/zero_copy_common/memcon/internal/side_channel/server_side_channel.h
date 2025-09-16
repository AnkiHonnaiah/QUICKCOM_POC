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
 *        \brief Definition of class ServerSideChannel.
 *        \unit ZeroCopyCommon::MemCon::SideChannel
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_SIDE_CHANNEL_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_SIDE_CHANNEL_H_

#include <cstdint>
#include <memory>
#include <mutex>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message_router.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/server_callbacks.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace side_channel {

// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_SideChannel
/*!
 * \brief Server-side implementation of a side channel.
 * \trace DSGN-MemCon-SideChannelProtocol
 * \vprivate Component Private
 */
class ServerSideChannel final
    : private ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter {
 public:
  /*!
   * \brief Construct a ServerSideChannel.
   * \details All callbacks may be used until IsReceiveInUse() returns false. See the CDD of IsReceiveInUse() for more
   *          details.
   * \param side_channel_ptr Side channel to communicate with the client.
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
  ServerSideChannel(std::unique_ptr<SideChannelInterface> side_channel_ptr, OnErrorCallback on_error_cbk) noexcept;

  /*!
   * \brief Do not construct a ServerSideChannel by copying.
   * \vprivate Component Private
   */
  ServerSideChannel(ServerSideChannel const&) = delete;

  /*!
   * \brief Do not construct a ServerSideChannel by moving.
   * \vprivate Component Private
   */
  ServerSideChannel(ServerSideChannel&&) = delete;

  /*!
   * \brief Do not copy another ServerSideChannel into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerSideChannel const&) -> ServerSideChannel& = delete;

  /*!
   * \brief Do not move another ServerSideChannel into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerSideChannel&&) -> ServerSideChannel& = delete;

  /*!
   * \brief Destruct the ServerSideChannel.
   * \context ANY
   * \pre The server side channel must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~ServerSideChannel() noexcept;

  /*!
   * \brief Start to receive messages.
   * \details Messages are received asynchronously.
   *          The respective callback of \p cbks is called each time the corresponding message is received.
   *          The \p cbks may be used until IsInUse() returns false. See the CDD of IsInUse() for more details.
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
  void StartMessageReception(ServerCallbacks cbks) noexcept;

  /*!
   * \brief Close the side channel.
   * \details Deregisters all callbacks. Stops future calls to callbacks but callbacks may currently be executing.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  void Close() noexcept;

  /*!
   * \brief Query whether the side channel is in use.
   * \details The side channel is in use when there was not yet a call to Close().
   *          Additionally, if there was a call to Close(), a callback may still be in use even after a call to Close().
   *          This function checks both conditions.
   * \return True, if the side channel is in use. False, otherwise.
   * \context ANY
   * \pre Must not be called after calling Close().
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsInUse() const noexcept -> bool;

  /*!
   * \brief Send a ConnectionRequest message.
   * \param slot_memory_config Slot configuration to be sent.
   * \param slot_memory_exchange_handle Exchange handle for the slot memory to be sent.
   *                                    Must not be a nullptr.
   * \param queue_memory_config Queue configuration to be sent.
   * \param queue_memory_exchange_handle Exchange handle for the queue memory to be sent.
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
   */
  auto ConnectionRequest(SlotMemoryConfig slot_memory_config, MemoryExchangeHandlePtr slot_memory_exchange_handle,
                         QueueMemoryConfig queue_memory_config,
                         MemoryExchangeHandlePtr queue_memory_exchange_handle) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Send an AckQueueInitialization message.
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
   */
  auto AckQueueInitialization() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Send a notification.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the notification could not be sent
   *        due to a protocol violation.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kDroppedNotificationError If the notification could
   *        not be sent due to any limitation of the side channel, but no protocol violation.
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
  auto Notification() noexcept -> ::amsr::core::Result<void>;

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
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto Shutdown() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Send a Termination message.
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
  auto Termination() noexcept -> ::amsr::core::Result<void>;

 private:
  /*!
   * \brief Type used for connection status of the side channel.
   */
  enum class ConnectionStatus : std::uint8_t { kOpen = 0, kClosed = 1 };

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
   * \brief Query if server callbacks are present.
   * \return True, if server callbacks are present. False, otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AreServerCallbacksPresent() const noexcept -> bool;

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
   * \brief Call the error callback with the given error code.
   * \param error_code Error code to pass to the error callback.
   * \context REACTOR
   * \pre The mutex must be locked when calling this function. This cannot be checked.
   *      Receiving must not be stopped when calling this function. I.e., IsReceivingStopped() must return false.
   *      This is not checked.
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
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnShutdown
   */
  void OnShutdown(::amsr::core::Result<protocol_message::Shutdown> const& result,
                  MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

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
   * \copydoc ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessageRouter::OnTermination
   */
  void OnTermination(::amsr::core::Result<protocol_message::Termination> const& result,
                     MemoryExchangeHandlePtr memory_exchange_handle) noexcept final;

  /*!
   * \brief The mutex of this instance.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief Connection status of this server side channel.
   * \details Accessed in ANY context.
   */
  ConnectionStatus connection_status_{ConnectionStatus::kOpen};

  /*!
   * \brief Callbacks that are called when a message is received.
   * \details Set on StartMessageReception().
   *          Unset on Close().
   *          Accessed in ANY context.
   */
  ::amsr::core::Optional<ServerCallbacks> opt_server_cbks_{::amsr::core::nullopt};

  /*!
   * \brief Side channel to communicate with the client.
   * \details Accessed in ANY context.
   */
  std::unique_ptr<SideChannelInterface> const side_channel_ptr_;

  /*!
   * \brief Callback that is called when any error occurs on this side channel.
   * \details Accessed in REACTOR context.
   */
  OnErrorCallback on_error_cbk_;
};

/*!
 * \exclusivearea ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::mutex_
 * Serializes all the ServerSideChannel API calls and the asynchronous callbacks triggered from the side channel.
 *
 * \protects All members of a ServerSideChannel object.
 * \usedin   ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::~ServerSideChannel
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::StartMessageReception
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::Close
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::IsInUse
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::ConnectionRequest
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::AckQueueInitialization
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::Notification
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::Shutdown
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::Termination
 * \exclude  ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::OnErrorCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::OnAckConnectionCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::OnStartListeningCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::OnStopListeningCallback
 *           ::amsr::zero_copy_common::memcon::internal::side_channel::ServerSideChannel::OnShutdownCallback
 * \length   LONG The lock is held for the complete API calls except the invocation of the OnErrorCallback,
 *           OnAckConnectionCallback, OnStartListeningCallback, OnStopListeningCallback and OnShutdownCallback to the
 *           user context.
 * \endexclusivearea
 */

}  // namespace side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_SIDE_CHANNEL_H_
