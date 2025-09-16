/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  someip_daemon_client/internal/subscribe_event_controller.h
 *        \brief  Subscribe Event controller of SomeIpDaemonClient
 *
 *      \details  Handling subscribe event commands from/to SOME/IP daemon
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SUBSCRIBE_EVENT_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SUBSCRIBE_EVENT_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/types.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Class for Subscribe Event control command sent from application to SOME/IP daemon.
 * \tparam SomeIpDaemonClientSelected Base SomeIpDaemonClient class used to access related Client. Contains the
 *         following required type definition:
 *         - typename SomeIpDaemonClientSelected::MessageSenderType
 */
template <typename SomeIpDaemonClientSelected>
class SubscribeEventController final {
 public:
  /*!
   * \brief Type-alias for MessageSender.
   */
  using MessageSender = typename SomeIpDaemonClientSelected::MessageSenderType;

  /*!
   * \brief Constructor sets the reference to the subscribe event command connection which is used to send out subscribe
   *        event commands to the SOME/IP daemon.
   * \details This object will be constructed once per SOME/IP binding application
   *          by the module SomeIpDaemonClient.
   * \param[in] message_sender Reference to message sender.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SubscribeEventController(MessageSender& message_sender) noexcept : message_sender_{message_sender} {}

  /*!
   * \brief Destructor of SubscribeEventController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~SubscribeEventController() noexcept = default;

  SubscribeEventController(SubscribeEventController const&) = delete;

  auto operator=(SubscribeEventController const&) -> SubscribeEventController& = delete;

  SubscribeEventController(SubscribeEventController const&&) = delete;

  auto operator=(SubscribeEventController const&&) -> SubscribeEventController& = delete;

  /*!
   * \brief Subscribes an event of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process the event sent by this service
   *          instance. Any errors from remote end, IE someipdaemon, are not returned.
   * \param[in] required_event Object of structure RequiredEventType containing
   *            data members of service_id, instance_id, event_id, major_version and minor_version.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected, sending in disconnected state.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Lock mutex.
   * - Serialize a command control request packet with service_id, instance_id, major_version, minor_version and
   *   event_id.
   * - Send a SubscribeEvent command with the previously serialized request packet.
   * - Return the result.
   * \endinternal
   */
  auto operator()(RequiredEventType const& required_event) noexcept -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-SubscribeEventController_SubscribeEvent

    std::lock_guard<std::mutex> const lock{subscribe_event_lock_};

    // Serialize request packet
    vac::memory::allocator::MemoryBufferPtr request_packet{serializer_.Serialize(
        required_event, amsr::someipd_app_protocol::internal::kControlMessageSubscribeEventRequestPayloadLength)};

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Sending Subscribe Event control command: "; }, {__func__},
                     {__LINE__});

    // PTP-E-SomeipDaemonClient-SubscribeEventController_SubscribeEvent
    // Asynchronous command request to SOME/IP daemon
    // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    return message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(request_packet));
  }

 private:
  /*!
   * \brief SubscribeEventController logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"SubscribeEventController"}};

  /*!
   * \brief Reference to the message sender used to send out command requests to the SOME/IP daemon and forwards
   *        responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kSubscribeEvent};

  /*!
   * \brief SubscribeEventController lock.
   */
  std::mutex subscribe_event_lock_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SUBSCRIBE_EVENT_CONTROLLER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::SubscribeEventController::subscribe_event_lock_
 *                Ensures that there is no concurrent access to the message_sender_ and to the memory
 *                buffers for the subscribe event request.
 *
 * \protects      ::amsr::someip_daemon_client::internal::SubscribeEventController::serializer_
 *                ::amsr::someip_daemon_client::internal::SubscribeEventController::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::SubscribeEventController::operator()
 *
 * \length        LONG
 *                Serializing request message, sending the message to the message_sender_
 * \endexclusivearea
 */
