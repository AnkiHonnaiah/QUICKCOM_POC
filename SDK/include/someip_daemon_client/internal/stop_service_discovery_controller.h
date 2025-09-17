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
/*!        \file  someip_daemon_client/internal/stop_service_discovery_controller.h
 *        \brief  StopServiceDiscovery API of SomeIpDaemonClient not requiring responses.
 *
 *      \details  Send stop service discovery command not requiring responses from SomeIpDaemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_STOP_SERVICE_DISCOVERY_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_STOP_SERVICE_DISCOVERY_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

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
 * \brief Class for Stop service discovery control command sent from application to SOME/IP daemon that require no
 * responses.
 * \tparam SomeIpDaemonClientSelected Base SomeIpDaemonClient class used to access related Client Contains
 * the following required type definition:
 *         - typename SomeIpDaemonClientSelected::MessageSenderType
 */
template <typename SomeIpDaemonClientSelected>
class StopServiceDiscoveryController final {
 public:
  /*!
   * \brief Type-alias for MessageSender.
   */
  using MessageSender = typename SomeIpDaemonClientSelected::MessageSenderType;

  /*!
   * \brief Constructor sets the reference to the command connection which is used to send out
   * stop service discovery control command to the SOME/IP daemon.
   * \details This object will be constructed once per SOME/IP binding application by the module
   * SomeIpDaemonClient.
   * \param[in] message_sender Reference to command connection.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit StopServiceDiscoveryController(MessageSender& message_sender) noexcept : message_sender_{message_sender} {}

  /*!
   * \brief Destructor of StopServiceDiscoveryController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  ~StopServiceDiscoveryController() noexcept = default;

  StopServiceDiscoveryController(StopServiceDiscoveryController const&) = delete;

  auto operator=(StopServiceDiscoveryController const&) -> StopServiceDiscoveryController& = delete;

  StopServiceDiscoveryController(StopServiceDiscoveryController const&&) = delete;

  auto operator=(StopServiceDiscoveryController const&&) -> StopServiceDiscoveryController& = delete;

  /*!
   * \brief Requests a stop of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must stop processing requests destined to this service
   * instance.
   * \param[in] required_service_instance Object of structure ProxySDRequiredServiceInstanceType containing
   * data members of service_id, instance_id, major_version and minor_version.
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
   *
   * \internal
   * - Lock mutex.
   * - Serialize a command control request packet with service_id, instance_id, major_version and minor_version.
   * - Send a StopServiceDiscovery command with the previously serialized request packet.
   * - Return the send result.
   * \endinternal
   */
  auto operator()(ProxySDRequiredServiceInstanceType const& required_service_instance) noexcept
      -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-StopServiceDiscoveryController_StopServiceDiscovery

    std::lock_guard<std::mutex> const lock_guard{lock_};

    // Serialize request packet
    vac::memory::allocator::MemoryBufferPtr request_packet{serializer_.Serialize(
        required_service_instance, amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength)};

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Sending Stop Service Discovery control command: "; },
                     {__func__}, {__LINE__});

    // PTP-E-SomeipDaemonClient-StopServiceDiscoveryController_StopServiceDiscovery
    // Asynchronous command request to SOME/IP daemon
    // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    return message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(request_packet));
  }

 private:
  /*!
   * \brief StopServiceDiscoveryController logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"StopServiceDiscoveryController"}};

  /*!
   * \brief Reference to the command connection used to send out command requests to the SOME/IP daemon and forwards
   * responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief StopServiceDiscoveryController lock.
   */
  std::mutex lock_{};

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kStopServiceDiscovery};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_STOP_SERVICE_DISCOVERY_CONTROLLER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::StopServiceDiscoveryController::lock_
 *                Ensures that there is no concurrent read/write to the message_sender_ instance and to the memory
 *                buffers for the stop service discovery controller
 *
 * \protects      ::amsr::someip_daemon_client::internal::StopServiceDiscoveryController::serializer_
 *                ::amsr::someip_daemon_client::internal::StopServiceDiscoveryController::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::StopServiceDiscoveryController::operator()
 *
 * \length        LONG
 *                Serializing request message and sending the message to the message_sender_
 * \endexclusivearea
 */
