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
 *        \brief  Definition of interface SideChannelInterface.
 *         \unit  ZeroCopyCommon::MemCon::SideChannel
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SIDE_CHANNEL_INTERFACE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SIDE_CHANNEL_INTERFACE_H_

#include <cstddef>

#include "amsr/core/result.h"

#include "vac/language/unique_function.h"

#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_messages.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace side_channel {

/*!
 * \brief Interface for a 1:1 side channel to be used for bidirectional communication between a server and a client.
 * \details Every side channel implementing this interface must guarantee the following properties:
 *          - Messages are received in the same order they were sent.
 *            Note: There are no ordering guarantees concerning the reception order of messages and notifications.
 *          - Every successfully sent message can be received, even if the sender crashes or closes the connection.
 *          - If the sender crashes or closes the connection, every sent message before that is received before the
 *            crash or close is received.
 *          - Successfully sent messages are buffered until reception of messages is started from the receiver-side.
 *            This means, successfully sent messages can be received even if the receiver has not started reception when
 *            they were sent.
 *          - Successfully sent notifications are not buffered when the receiver does not want to receive notifications.
 *            This means, successfully sent notifications are discarded when the receiver does not want to receive
 *            notifications when they are sent.
 *          - The side channel must be exclusively used by this interface. All messages/notifications which are sent
 *            and received via side channel must belong to this instance.
 * \trace DSGN-MemCon-SideChannelInterface
 * \vprivate Product Private
 */
class SideChannelInterface {
 public:
  /*!
   * \brief Type used for a message.
   */
  using Message = ::amsr::zero_copy_common::memcon::internal::Message;

  /*!
   * \brief Type used for the callback that is called each time a notification is received.
   * \details Only notifications with value 0x00 are valid.
   * \param result_notification Result indicating whether a valid notification was received.
   *                            A positive result indicates that a valid notification was received.
   *                            Otherwise, the result contains the error code
   *                            ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  using OnNotificationExtendedCallback =
      ::vac::language::UniqueFunction<void(::amsr::core::Result<void> result_notification)>;

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
   * \vprivate Product Private
   */
  using OnMessageCallback = ::vac::language::UniqueFunction<void(::amsr::core::Result<Message> result_message)>;

  /*!
   * \brief Destruct the SideChannelInterface.
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
  virtual ~SideChannelInterface() noexcept = 0;

  /*!
   * \brief Get the maximum message size supported by the side channel.
   * \details The maximum message size applies to sent and received messages.
   *          Every concrete side channel must support sending and receiving messages up to the maximum message size.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static constexpr auto GetMaxMessageSize() noexcept -> std::size_t {
    return ::amsr::zero_copy_common::memcon::internal::protocol_message::ProtocolMessages::GetMaxSize();
  }

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
   * \vprivate Product Private
   */
  virtual auto Send(Message message) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Notify the communication peer.
   * \details The notification is sent synchronously.
   *          Sends a notification with value 0x00.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the notification could not be sent
   *        due to a protocol violation.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kDroppedNotificationError If the notification could
   *        not be sent due to any limitation of the side channel, but no protocol violation.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto Notify() noexcept -> ::amsr::core::Result<void> = 0;

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
   * \vprivate Product Private
   */
  virtual void StartMessageReception(OnMessageCallback on_message_cbk) noexcept = 0;

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
   * \vprivate Product Private
   */
  virtual auto IsOnNotificationCallbackInUse() const noexcept -> bool = 0;

  /*!
   * \brief Register a OnNotificationCallback to receive notifications.
   * \details Notifications are received asynchronously.
   *          Expects to receive notifications with value 0x00.
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
   * \vprivate Product Private
   */
  virtual void RegisterOnNotificationCallback(OnNotificationExtendedCallback on_notification_cbk) noexcept = 0;

  /*!
   * \brief Deregister the OnNotificationCallback to receive notifications.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \pre There must have been a call to RegisterOnNotificationCallback() and not yet a call to
   *      DeregisterOnNotificationCallback() afterwards.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual void DeregisterOnNotificationCallback() noexcept = 0;

  /*!
   * \brief Close the side channel.
   * \details Deregisters all callbacks.
   * \context ANY
   * \pre The side channel must not be closed, i.e., Close() must not have been called.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual void Close() noexcept = 0;

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
   * \vprivate Product Private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Construct a SideChannelInterface.
   * \vprivate Product Private
   */
  SideChannelInterface() noexcept = default;

  /*!
   * \brief Construct a SideChannelInterface by copying.
   * \vprivate Product Private
   */
  SideChannelInterface(SideChannelInterface const&) noexcept = default;

  /*!
   * \brief Construct a SideChannelInterface by moving.
   * \vprivate Product Private
   */
  SideChannelInterface(SideChannelInterface&&) noexcept = default;

  /*!
   * \brief Copy another SideChannelInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(SideChannelInterface const&) & noexcept -> SideChannelInterface& = default;

  /*!
   * \brief Move another SideChannelInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(SideChannelInterface&&) & noexcept -> SideChannelInterface& = default;
};

inline SideChannelInterface::~SideChannelInterface() noexcept = default;

}  // namespace side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SIDE_CHANNEL_INTERFACE_H_
