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
/*!        \file  someip_daemon_client/internal/request_service_controller.h
 *        \brief  Request service controller of SomeIpDaemonClient.
 *
 *      \details  Handling request service commands from/to SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_SERVICE_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_SERVICE_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/promise.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_daemon_client/external/libvac/flexible_unique_memory_buffer_allocator.h"
#include "ara/core/memory_resource.h"
#include "ara/log/logstream.h"
#include "someip_daemon_client/internal/common_types.h"
#include "someip_daemon_client/internal/controller_interface.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "someip_daemon_client/internal/types.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "vac/memory/allocator/memory_buffer_interface.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Class for Request Service control command sent from application to SOME/IP daemon.
 * \tparam SomeIpDaemonClientSelected Base SomeIpDaemonClient class used to access related Client. Contains the
 *         following required type definition:
 *         - typename SomeIpDaemonClientSelected::MessageSenderType
 */
template <typename SomeIpDaemonClientSelected>
class RequestServiceController : public ControllerInterface {
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
   * \brief Type-alias for deserialization Reader.
   */
  using Reader = ::amsr::someipd_app_protocol::internal::Reader;

  /*!
   * \brief IPC protocol control message client ID.
   */
  using ClientId = amsr::someipd_app_protocol::internal::ControlMessageClientId;

  /*!
   * \brief Type-alias for promise type for ClientId.
   */
  using RequestServicePromiseType = amsr::core::Optional<amsr::core::Promise<ClientId>>;

  /*!
   * \brief Constructor sets the reference to the request service command connection which is used to send out request
   *        service commands to the SOME/IP daemon.
   * \details This object will be constructed once per SOME/IP binding application by the module SomeIpDaemonClient.
   * \param[in] message_sender Reference to message sender.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit RequestServiceController(MessageSender& message_sender) noexcept
      : ControllerInterface{}, message_sender_{message_sender} {}

  /*!
   * \brief Destructor of RequestServiceController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  ~RequestServiceController() noexcept override = default;

  RequestServiceController(RequestServiceController const&) = delete;

  auto operator=(RequestServiceController const&) -> RequestServiceController& = delete;

  RequestServiceController(RequestServiceController const&&) = delete;

  auto operator=(RequestServiceController const&&) -> RequestServiceController& = delete;

  /*!
   * \brief Requests a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process requests destined to this service
   * instance.
   * \param[in] proxy_creation_required_service_instance Object of structure ProxyCreationRequiredServiceInstanceType
   * containing data members service_id, instance_id, major_version and minor_version.
   * \return Future containing client ID from the SOME/IP daemon. Or an error code if IPC transmission fails.
   * \error  SomeIpDaemonClientErrc::kAccessDenied If the application has no right to request a service.
   * \error  SomeIpDaemonClientErrc::kIncorrectResponseMsgLength If the message deserialization failed.
   * \error  SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   * \error  SomeIpDaemonClientErrc::kDisconnected If disconnect is called before response is received.
   * \error  SomeIpDaemonClientErrc::kPayloadEmpty If an empty response message payload is received.
   * \error  SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock mutex.
   * - If there is no pending RequestService call
   *   - Create a promise.
   * - Otherwise, Log error and Abort.
   * - Serialize a request service request packet with service_id, instance_id, major_version and minor_version.
   * - Asynchronously send a RequestService command with the previously serialized request packet.
   * - If the send failed synchronously, log and
   *   - Set the promise error result.
   *   - Reset the promise.
   * - Return the future containing of the client ID or the error result on send failure.
   * \endinternal
   */
  auto operator()(ProxyCreationRequiredServiceInstanceType const& proxy_creation_required_service_instance) noexcept
      -> amsr::core::Future<ClientId> {
    // PTP-B-SomeipDaemonClient-RequestServiceController_RequestService

    amsr::core::Future<ClientId> future{};
    amsr::core::Result<void> send_result{};
    std::lock_guard<std::mutex> const lock{request_service_lock_};
    if (!is_request_service_rc_promise_in_use_) {
      future = request_service_rc_promise_.get_future();
      is_request_service_rc_promise_in_use_ = true;
    } else {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "RequestService command is called again before receiving the response from the previous call.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // Serialize request packet
    vac::memory::allocator::MemoryBufferPtr request_packet{
        serializer_.Serialize(proxy_creation_required_service_instance,
                              amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength)};

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Sending Request Service control command: "; }, {__func__},
                     {__LINE__});

    // Asynchronous command request to SOME/IP daemon
    // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    send_result = message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(request_packet));
    // PTP-E-SomeipDaemonClient-RequestServiceController_RequestService

    if (!send_result.HasValue()) {
      logger_.LogVerbose(
          [&send_result](ara::log::LogStream& s) {
            s << "Sending of RequestService command message failed. ";
            // VCA_SDCL_LNG03_OBJECT_IS_VALID
            amsr::core::StringView const error_message{send_result.Error().Message()};
            s << "Error: " << error_message;
          },
          {__func__}, {__LINE__});
      request_service_rc_promise_.SetError(send_result.Error());
      ResetRequestServiceRcPromise();
    }
    return future;
  }

  /*!
   * \brief Set the promise result to SomeIpDaemonClientErrc::kDisconnected when Disconnect() called.
   * \pre -
   * \context  Reactor
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock the mutex.
   * - Check if the request service promise have value.
   *   - Set the promise result to SomeIpDaemonClientErrc::kDisconnected.
   *   - Reset the promise.
   * \endinternal
   */
  void SetUpPromise() noexcept {
    std::lock_guard<std::mutex> const lock{request_service_lock_};
    if (is_request_service_rc_promise_in_use_) {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Disconnect() called before receiving response for the RequestService command request.";
          },
          {__func__}, {__LINE__});
      request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kDisconnected);
      ResetRequestServiceRcPromise();
    }
  }

  /*!
   * \brief Set the value for RequestService promise.
   * \param[in] reception_buffer The message's reception buffer.
   * \param[in] control_header The control message header.
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Lock the mutex.
   * - Check if the promise was created
   *   - Check if the message body has value, fill it to the response memory buffer.
   *     - If the return code is of type "kOk"
   *       - Deserialize client ID using the response.
   *       - If the deserialization is successful and return is kOk,
   *         - Set client ID as the deserialized client ID.
   *       - else
   *         - Log error for incorrect length of the response.
   *         - Abort as deserialization failed.
   *     - else if the return code is kRequestServiceAccessDenied,
   *         - Log error that access denied.
   *     - else if the return code is kRequestServiceClientIdsOverflow,
   *         - Log error that client Ids overflow.
   *     - else if the return code is kRequestServiceRemoteServerNotFound or
   *       kRequestServiceRequiredServiceInstanceNotFound,
   *         - Log error that a configuration mismatch happened.
   *     - else
   *       - Abort as communication with SOME/IP Daemon failed.
   *   - otherwise abort as response message payload is empty.
   * - Otherwise, log an error that no control command has been scheduled.
   * \endinternal
   */
  void SetPromiseValue(ReceptionBuffer const& reception_buffer,
                       amsr::someipd_app_protocol::internal::ControlMessageHeader const& control_header) override {
    std::lock_guard<std::mutex> const lock{request_service_lock_};
    // PTP-B-SomeipDaemonClient-RequestServiceController_SetPromiseValue
    if (is_request_service_rc_promise_in_use_) {
      if (reception_buffer.receive_message_body) {
        vac::memory::allocator::MemoryBuffer::MemoryBufferView const memory_buffer_view{
            // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
            reception_buffer.receive_message_body->GetView(0U)};
        std::size_t const receive_size{memory_buffer_view.size()};
        amsr::someip_daemon_client::external::libvac::FlexibleUniqueMemoryBufferAllocator<
            vac::memory::NonZeroingAllocator<std::uint8_t>>
            allocator{};
        amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> allocated_packet_result{
            allocator.Allocate(receive_size)};  // VCA_SDCL_SPC_15_OTHER_COMPONENT_FUNCTION

        if (!allocated_packet_result.HasValue()) {
          amsr::core::Abort("Failed to allocate memory buffer.");
        }
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10_A18.5.8_false_positive
        vac::memory::allocator::MemoryBufferPtr const response_memory_buffer{
            std::move(allocated_packet_result.Value())};
        // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
        std::memcpy(response_memory_buffer->GetView(0U).data(), memory_buffer_view.data(), receive_size);

        // Deserialize client ID.
        if (control_header.return_code == ControlMessageReturnCode::kOk) {
          // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
          std::size_t const response_packet_length{response_memory_buffer->GetView(0U).size()};
          vac::memory::allocator::MemoryBuffer::MemoryBufferView const packet_view{
              // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
              response_memory_buffer->GetView(0U)};
          Reader::BufferView const buffer_view{packet_view};

          // VECTOR Enable AutosarC++17_10-M5.2.8
          Reader reader{buffer_view};

          // PTP-B-SomeipDaemonClient-RequestServiceController_RequestService_Deserialize
          amsr::core::Optional<amsr::someipd_app_protocol::internal::ControlMessageRequestServiceResponsePayload> const
              payload{amsr::someipd_app_protocol::internal::DeserMessageHeaders::
                          DeserializePayloadForControlMessageRequestServiceResponse(reader)};

          // PTP-E-SomeipDaemonClient-RequestServiceController_RequestService_Deserialize
          if (payload.has_value()) {
            request_service_rc_promise_.set_value(payload->client_id);
            ResetRequestServiceRcPromise();
          } else {
            request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kIncorrectResponseMsgLength);
            logger_.LogFatalAndAbort(
                [&response_packet_length](logging::AraComLogger::StringStream& s) {
                  s << "Failed to deserialize the RequestService command control response. Received size: ";  // VCA_SDCL_STRING_STREAM_USAGE
                  s << std::to_string(response_packet_length);              // VCA_SDCL_STRING_STREAM_USAGE
                  s << ", Expected size: ";                                 // VCA_SDCL_STRING_STREAM_USAGE
                  s << std::to_string(                                      // VCA_SDCL_STRING_STREAM_USAGE
                      amsr::someipd_app_protocol::internal::                // VCA_SDCL_STRING_STREAM_USAGE
                      kControlMessageRequestServiceResponsePayloadLength);  // VCA_SDCL_STRING_STREAM_USAGE
                                                                            // VCA_SDCL_STRING_STREAM_USAGE
                },
                static_cast<char const*>(__func__), __LINE__);
          }
        } else {  // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
          request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kNotConnected);
          logger_.LogFatalAndAbort(
              [](logging::AraComLogger::StringStream& s) {
                s << "Communication with SOME/IP Daemon failed.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                   // VCA_SDCL_STRING_STREAM_USAGE
              },
              static_cast<char const*>(__func__), __LINE__);
        }
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
      } else if (control_header.return_code == ControlMessageReturnCode::kRequestServiceAccessDenied) {
        logger_.LogError([](ara::log::LogStream& s) { s << "RequestServiceController::RequestService: Access Denied"; },
                         {__func__}, {__LINE__});
        request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kAccessDenied);
      } else if (control_header.return_code == ControlMessageReturnCode::kRequestServiceClientIdsOverflow) {
        logger_.LogError(
            [](ara::log::LogStream& s) { s << "RequestServiceController::RequestService: Client Ids overflow"; },
            {__func__}, {__LINE__});
        request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kClientIdsOverflow);
      } else if ((control_header.return_code == ControlMessageReturnCode::kRequestServiceRemoteServerNotFound) ||
                 (control_header.return_code ==
                  ControlMessageReturnCode::kRequestServiceRequiredServiceInstanceNotFound)) {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "RequestServiceController::RequestService: Configuration mismatch when requesting a local server";
            },
            {__func__}, {__LINE__});
        request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kConfigurationError);
      } else {
        request_service_rc_promise_.SetError(SomeIpDaemonClientErrc::kPayloadEmpty);
        logger_.LogFatalAndAbort(
            [](logging::AraComLogger::StringStream& s) {
              s << "Received empty response message payload.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                // VCA_SDCL_STRING_STREAM_USAGE
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Received a Request Service control command response but no Request Service control command has been "
                 "scheduled";
          },
          {__func__}, {__LINE__});
    }
    // PTP-E-SomeipDaemonClient-RequestServiceController_SetPromiseValue
  }  // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION

 private:
  /*!
   * \brief  Reset the value of the promise and reset the is_promise_in_use_ flag.
   */
  void ResetRequestServiceRcPromise() noexcept {
    amsr::core::Promise<ClientId> new_request_service_rc_promise{ara::core::GetDefaultResource()};
    request_service_rc_promise_.swap(new_request_service_rc_promise);
    is_request_service_rc_promise_in_use_ = false;
  }

  /*!
   * \brief RequestServiceController logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"RequestServiceController"}};

  /*!
   * \brief Reference to the message sender used to send out command requests to the SOME/IP daemon and forwards
   *        responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kRequestService};

  /*!
   * \brief The promise of return code for RequestService.
   */
  amsr::core::Promise<ClientId> request_service_rc_promise_{ara::core::GetDefaultResource()};

  /*!
   * \brief  A flag to indicate that the request service rc promise is in use.
   */
  bool is_request_service_rc_promise_in_use_{false};

  /*!
   * \brief Lock for protecting request_service_rc_promise_
   */
  std::mutex request_service_lock_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_REQUEST_SERVICE_CONTROLLER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::RequestServiceController::request_service_lock_
 *                Ensures that there is no concurrent read/write to request_service_rc_promise_, to the message_sender_
 *                instance and to the memory buffers for the request service request
 *
 * \protects      ::amsr::someip_daemon_client::internal::RequestServiceController::request_service_rc_promise_
 *                ::amsr::someip_daemon_client::internal::RequestServiceController::serializer_
 *                ::amsr::someip_daemon_client::internal::RequestServiceController::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::RequestServiceController::SetPromiseValue,
 *                ::amsr::someip_daemon_client::internal::RequestServiceController::SetUpPromise,
 *                ::amsr::someip_daemon_client::internal::RequestServiceController::operator()
 *
 * \length        LONG
 *                Serializing request message, sending the message to the message_sender_, deserializing response
 *                message and setting value for request_service_rc_promise_
 * \endexclusivearea
 */
