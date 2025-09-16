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
/*!        \file  someip_daemon_client/internal/start_service_discovery_controller.h
 *        \brief  Start service discovery controller of SomeIpDaemonClient
 *
 *      \details  Handling start service discovery service commands from/to SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_START_SERVICE_DISCOVERY_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_START_SERVICE_DISCOVERY_CONTROLLER_H_

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
 * \brief Class for Start Service discovery control command sent from application to SOME/IP daemon.
 * \tparam SomeIpDaemonClientSelected Base SomeIpDaemonClient class used to access related Client. Contains the
 *         following required type definition:
 *         - typename SomeIpDaemonClientSelected::MessageSenderType
 */
template <typename SomeIpDaemonClientSelected>
class StartServiceDiscoveryController final {
 public:
  /*!
   * \brief Type-alias for MessageSender.
   */
  using MessageSender = typename SomeIpDaemonClientSelected::MessageSenderType;

  /*!
   * \brief Type-alias for IPC protocol control message return code.
   */
  using ControlMessageReturnCode = amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Constructor sets the reference to the command connection which is used to send out start service discovery
            commands to the SOME/IP daemon.
   * \details This object will be constructed once per SOME/IP binding application by the module SomeIpDaemonClient.
   * \param[in] message_sender Reference to message sender.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit StartServiceDiscoveryController(MessageSender& message_sender) noexcept : message_sender_{message_sender} {}

  /*!
   * \brief Destructor of StartServiceDiscoveryController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  ~StartServiceDiscoveryController() noexcept = default;

  StartServiceDiscoveryController(StartServiceDiscoveryController const&) = delete;

  auto operator=(StartServiceDiscoveryController const&) -> StartServiceDiscoveryController& = delete;

  StartServiceDiscoveryController(StartServiceDiscoveryController const&&) = delete;

  auto operator=(StartServiceDiscoveryController const&&) -> StartServiceDiscoveryController& = delete;

  /*!
   * \brief Requests a start of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process requests destined to this service
   *          instance. Any errors from remote end, IE someipdaemon, are not returned.
   * \param[in] required_service_instance Object of structure ProxySDRequiredServiceInstanceType containing
   *            data members service_id, instance_id, major_version and minor_version.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected, sending in disconnected state.
   * \error SomeIpDaemonClientErrc::kDisconnected If disconnect is called before response is received.
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
   * - Send a StartServiceDiscovery command with the previously serialized request packet.
   * - Return the result.
   * \endinternal
   */
  auto operator()(ProxySDRequiredServiceInstanceType const& required_service_instance) noexcept
      -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-StartServiceDiscoveryController_StartServiceDiscovery
    std::lock_guard<std::mutex> const lock{start_service_discovery_lock_};

    // Serialize request packet
    vac::memory::allocator::MemoryBufferPtr request_packet{serializer_.Serialize(
        required_service_instance, amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength)};

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Sending Start Service Discovery control command: "; },
                     {__func__}, {__LINE__});

    // Asynchronous command request to SOME/IP daemon
    amsr::core::Result<void> const result{
        // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
        message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(request_packet))};
    // PTP-E-SomeipDaemonClient-StartServiceDiscoveryController_StartServiceDiscovery

    return result;
  }

 private:
  /*!
   * \brief StartServiceDiscoveryController logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"StartServiceDiscoveryController"}};

  /*!
   * \brief Reference to the message sender used to send out command requests to the SOME/IP daemon and forwards
   *        responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kStartServiceDiscovery};

  /*!
   * \brief StartServiceDiscoveryController lock.
   */
  std::mutex start_service_discovery_lock_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_START_SERVICE_DISCOVERY_CONTROLLER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::StartServiceDiscoveryController::start_service_discovery_lock_
 *                Ensures that there is no concurrent access to the message_sender_ and to the memory
 *                buffers for the start service discovery request.
 *
 * \protects      ::amsr::someip_daemon_client::internal::StartServiceDiscoveryController::serializer_
 *                ::amsr::someip_daemon_client::internal::StartServiceDiscoveryController::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::StartServiceDiscoveryController::operator()
 *
 * \length        LONG
 *                Serializing request message, sending the message to the message_sender_.
 * \endexclusivearea
 */
