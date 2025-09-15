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
/*!        \file  someip_daemon_client/internal/request_local_server_controller.h
 *        \brief  Request Local Server service controller of SomeIpDaemonClient.
 *
 *      \details  Handling request local server commands from/to SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_LOCAL_SERVER_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_LOCAL_SERVER_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/promise.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "someip_daemon_client/internal/controller_interface.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "someip_daemon_client/internal/types.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Type-alias for IPC protocol control message return code.
 */
using ControlMessageReturnCode = amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

/*!
 * \brief Class for Request Local Server control command sent from application to SOME/IP daemon.
 * \tparam SomeIpDaemonClientSelected Base SomeIpDaemonClient class used to access related Client. Contains the
 *         following required type definition:
 *         - typename SomeIpDaemonClientSelected::MessageSenderType
 */
template <typename SomeIpDaemonClientSelected>
class RequestLocalServerController : public ControllerInterface {
 public:
  /*!
   * \brief Type-alias for MessageSender.
   */
  using MessageSender = typename SomeIpDaemonClientSelected::MessageSenderType;

  /*!
   * \brief Type-alias for promise type for ControlCommandReturnCode.
   */
  using ReturnCodePromiseType = amsr::core::Optional<amsr::core::Promise<ControlMessageReturnCode>>;

  /*!
   * \brief Constructor sets the reference to the request local server command connection which is used to send out
   *        request local server commands to the SOME/IP daemon.
   * \details This object will be constructed once per SOME/IP binding application by the module SomeIpDaemonClient.
   * \param[in] message_sender Reference to message sender.
   * \pre -
   * \context     Init
   * \reentrant   FALSE
   * \threadsafe  TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit RequestLocalServerController(MessageSender& message_sender) noexcept
      : ControllerInterface{}, message_sender_{message_sender} {}

  /*!
   * \brief Destructor of RequestLocalServerController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  ~RequestLocalServerController() noexcept override = default;

  RequestLocalServerController(RequestLocalServerController const&) = delete;
  auto operator=(RequestLocalServerController const&) -> RequestLocalServerController& = delete;
  RequestLocalServerController(RequestLocalServerController const&&) = delete;
  auto operator=(RequestLocalServerController const&&) -> RequestLocalServerController& = delete;

  /*!
   * \brief Requests ownership of a local server to the SOME/IP daemon.
   * \details After calling this function, an application owns a local service instance at the SOME/IP daemon and
   *          no other skeleton can be created for that specific service instance id.
   * \param[in] provided_service_instance Object of structure ProvidedServiceInstanceType
   *            containing data members service_id, instance_id, major_version and minor_version.
   * \return Future containing return code to indicate whether the request is successfully transmitted or not.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected, sending in disconnected state.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   * \pre -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock mutex.
   * - If there is no pending call to RequestLocalServer
   *   - Create a promise.
   * - Otherwise, Log error and Abort.
   * - Serialize a command control request packet with service_id, instance_id, major_version and minor_version.
   * - Asynchronously send a RequestLocalServer command with the previously serialized request packet.
   * - If the send failed synchronously:
   *   - Set the promise error result.
   *   - Reset the promise.
   * - Return the future containing the call result or the error result on send failure.
   * \endinternal
   */
  auto operator()(ProvidedServiceInstanceType provided_service_instance) noexcept
      -> amsr::core::Future<ControlMessageReturnCode> {
    // PTP-B-SomeipDaemonClient-RequestLocalServerController_RequestLocalServer
    amsr::core::Future<ControlMessageReturnCode> future{};
    amsr::core::Result<void> send_result{};
    std::lock_guard<std::mutex> const lock{request_local_server_lock_};
    if (!request_local_server_rc_promise_.has_value()) {
      request_local_server_rc_promise_.emplace();  // VCA_SDCL_FUNCTION_NO_PRECONDITION
      future = request_local_server_rc_promise_.value().get_future();
    } else {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "RequestLocalServer command is called again before receiving the response from the previous "  // VCA_SDCL_STRING_STREAM_USAGE
                 "call.";  // VCA_SDCL_STRING_STREAM_USAGE
                           // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // Serialize request packet
    vac::memory::allocator::MemoryBufferPtr request_packet{serializer_.Serialize(
        provided_service_instance, amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength)};

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Sending Request Local Server control command: "; }, {__func__},
                     {__LINE__});

    // Asynchronous command request to SOME/IP daemon
    // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    send_result = message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(request_packet));
    // PTP-E-SomeipDaemonClient-RequestLocalServerController_RequestLocalServer

    if (!send_result.HasValue()) {
      logger_.LogVerbose(
          [&send_result](ara::log::LogStream& s) {
            s << "Sending of RequestLocalServer command message failed. ";
            // VCA_SDCL_LNG03_OBJECT_IS_VALID
            amsr::core::StringView const error_message{send_result.Error().Message()};
            s << "Error: " << error_message;
          },
          {__func__}, {__LINE__});
      request_local_server_rc_promise_.value().SetError(send_result.Error());
      request_local_server_rc_promise_.reset();
    }
    return future;
  }

  /*!
   * \brief Set the promise result to SomeIpDaemonClientErrc::kDisconnected when Disconnect() called.
   * \pre -
   * \context   Reactor
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock the mutex.
   * - Check if the promise have value.
   *   - Set the promise result to SomeIpDaemonClientErrc::kDisconnected.
   *   - Reset the promises.
   * \endinternal
   */
  void SetUpPromise() noexcept {
    std::lock_guard<std::mutex> const lock{request_local_server_lock_};
    if (request_local_server_rc_promise_.has_value()) {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Disconnect() called before receiving response for the RequestLocalServer command request.";
          },
          {__func__}, {__LINE__});
      request_local_server_rc_promise_.value().SetError(SomeIpDaemonClientErrc::kDisconnected);
      request_local_server_rc_promise_.reset();
    }
  }

  /*!
   * \brief Set the value for RequestLocalServer promise.
   * \param[in] reception_buffer The message's reception buffer. Not used in RequestLocalServerController.
   * \param[in] control_header The control message header.
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock the mutex.
   * - Check if the promise was created
   *   - Set the return code as the value of the promise.
   * - Otherwise, log an error that no control command has been scheduled.
   * \endinternal
   */
  void SetPromiseValue(ReceptionBuffer const& reception_buffer,
                       amsr::someipd_app_protocol::internal::ControlMessageHeader const& control_header) override {
    // Void casting of variable reception_buffer to ignore the compiler warning.
    // warning: unused parameter 'reception_buffer' [-Wunused-parameter]
    static_cast<void>(reception_buffer);
    std::lock_guard<std::mutex> const lock{request_local_server_lock_};
    if (request_local_server_rc_promise_.has_value()) {
      request_local_server_rc_promise_.value().set_value(control_header.return_code);
      request_local_server_rc_promise_.reset();
    } else {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Received a request local server control command response but no request local server control command "
                 "has been scheduled";
          },
          {__func__}, {__LINE__});
    }
  }  // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION

 private:
  /*!
   * \brief RequestLocalServerController logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"RequestLocalServerController"}};

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kRequestLocalServer};

  /*!
   * \brief Reference to the message sender used to send out command requests to the SOME/IP daemon and forwards
   *        responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief The promise of return code for RequestLocalServer.
   */
  ReturnCodePromiseType request_local_server_rc_promise_{};

  /*!
   * \brief Lock for protecting request_local_server_rc_promise_
   */
  std::mutex request_local_server_lock_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_LOCAL_SERVER_CONTROLLER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::RequestLocalServerController::request_local_server_lock_
 *                Ensures that there is no concurrent read/write to request_local_server_rc_promise_, to the
 *                message_sender_ instance and to the memory buffers for the request local server request
 *
 * \protects      ::amsr::someip_daemon_client::internal::RequestLocalServerController::request_local_server_rc_promise_
 *                ::amsr::someip_daemon_client::internal::RequestLocalServerController::serializer_
 *                ::amsr::someip_daemon_client::internal::RequestLocalServerController::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::RequestLocalServerController::SetPromiseValue,
 *                ::amsr::someip_daemon_client::internal::RequestLocalServerController::SetUpPromise
 *                ::amsr::someip_daemon_client::internal::RequestLocalServerController::operator()
 *
 * \length        LONG
 *                Serializing request message, sending the message to the message_sender_ and setting value for
 *                request_local_server_rc_promise_
 * \endexclusivearea
 */
