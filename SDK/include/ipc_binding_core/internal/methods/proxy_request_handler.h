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
/**        \file
 *        \brief  Proxy method request handler.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PROXY_REQUEST_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PROXY_REQUEST_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <limits>
#include <memory>
#include <unordered_map>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/promise.h"
#include "amsr/shared_ptr.h"
#include "ara/com/com_error_domain.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_message_builder.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/ipc_protocol/ser_ipc_headers.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/methods/destruction_action.h"
#include "ipc_binding_core/internal/methods/pending_request_map.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/session_handler.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   Handles the transmission of method requests and responses.
 * \details The handler is responsible to provide future objects that can be returned to the application calling a
 *          method request.
 *
 * \tparam Output             Struct type containing all method output arguments.
 * \tparam RequestSerializer  The serializer type used to serialize the method request packet.
 * \tparam InputArgsT         Types of the method input arguments.
 *
 * \unit IpcBinding::IpcBindingCore::ProxyMethod
 */
template <typename Output, typename RequestSerializer, typename... InputArgsT>
class ProxyRequestHandler final {
 public:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for session ID.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type-alias for MethodId.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type-alias for ClientId.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Type-alias for TraceMethodSinkInterface.
   */
  using TraceMethodSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface;

  /*!
   * \brief Type-alias for ProxyRouterInterface.
   */
  using ProxyRouterInterface = ipc_binding_core::internal::ProxyRouterInterface;

  /*!
   * \brief Type-alias for DestructionAction.
   */
  using DestructionAction = ipc_binding_core::internal::methods::DestructionAction<Output>;

  /*!
   * \brief Type-alias for DestructionAction.
   */
  using PendingRequestMap = ipc_binding_core::internal::methods::PendingRequestMap<Output>;

  /*!
   * \brief Constructor of ProxyRequestHandler.
   *
   * \param[in,out] logger                        The logger reference.
   * \param[in,out] pending_request_map           The pending request map reference.
   * \param[in]     provided_service_instance_id  The provided service instance ID.
   * \param[in]     method_id                     Method deployment ID.
   * \param[in]     client_id                     The client ID.
   * \param[in,out] proxy_router                  Reference to the ProxyXf ProxyRouter.
   * \param[in]     trace_sink                    Reference to the trace sink.
   * \param[in]     max_limit_session             Maximum value of session ID. After reaching this value, the session ID
   *                                              must be reset. If the value is not set explicitly then the wrap around
   *                                              of session will happen when usual max limit of the SessionId::type is
   *                                              reached. (A value other than default is only used for testing).
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ProxyRequestHandler(Logger& logger, amsr::SharedPtr<PendingRequestMap> pending_request_map,
                      ProvidedServiceInstanceId const provided_service_instance_id, MethodId const method_id,
                      ClientId const client_id, ProxyRouterInterface& proxy_router,
                      TraceMethodSinkInterface& trace_sink,
                      SessionId::type max_limit_session = std::numeric_limits<SessionId::type>::max()) noexcept
      : logger_{logger},
        pending_request_map_{std::move(pending_request_map)},
        provided_service_instance_id_{provided_service_instance_id},
        method_id_{method_id},
        client_id_{client_id},
        proxy_router_{proxy_router},
        trace_sink_{trace_sink},
        max_limit_session_{max_limit_session} {}

  ProxyRequestHandler(ProxyRequestHandler&&) noexcept = delete;
  auto operator=(ProxyRequestHandler&&) noexcept -> ProxyRequestHandler& = delete;
  ProxyRequestHandler(ProxyRequestHandler const&) noexcept = delete;
  auto operator=(ProxyRequestHandler const&) noexcept -> ProxyRequestHandler& = delete;

  /*!
   * \brief Destructor.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   */
  ~ProxyRequestHandler() noexcept = default;

  /*!
   * \brief   Serialize and transmit the method request. Create and return the future to the application.
   * \details An attempt is being made to transmit the serialized method request. Therefore a future-promise pair is
   *          created to forward the result of the message request to the application. The promise is used to forward
   *          the result of the asynchronous message call to the application. In case of an error a ComErrc is set into
   *          the promise immediately.
   *
   * \param[in] args  The method request input arguments.
   *
   * \return A Future that allows the caller to retrieve the result of the method request.
   *
   * \error ara::com::ComErrc::kNetworkBindingFailure  Method request couldn't be transmitted.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE Received response notified asynchronously using future-promise pattern.
   * \steady      TRUE
   *
   * \trace SPEC-4980140
   *
   * \internal
   * - Get the next session ID.
   * - Build the method request message header.
   * - Allocate the buffer of size header + payload.
   * - Serialize the request message header.
   * - Serialize the payload.
   * - Add an entry in the pending request map using the current session id.
   * - Check if a future was created, while adding an entry in the pending request map.
   *   - Use the future to be returned to the application.
   *   - Create a request message to transmit the packet.
   *   - Trace the request send with the trace sink.
   *   - Send the message.
   * - Otherwise, create a future and set a ComError as error.
   *   - Return a future to the application.
   * \endinternal
   */
  auto HandleMethodRequest(InputArgsT const&... args) noexcept -> ::amsr::core::Future<Output> {
    namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;

    // PTP-B-IpcBinding-Xf-Proxy-Serialize-MethodRequest

    SessionId const session_id{GetNextSessionId()};
    RequestMessageHeader const header{provided_service_instance_id_.ServiceId(),
                                      provided_service_instance_id_.InstanceId(),
                                      provided_service_instance_id_.MajorVersion(),
                                      method_id_,
                                      client_id_,
                                      session_id};

    logger_.LogVerbose(
        [&header](Logger::LogStream& s) {
          s << "Handling proxy method request (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s,
                                  header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogSessionId(s,
                                   header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // Calculate the required size for the request arguments
    std::size_t const payload_size{RequestSerializer::GetRequiredBufferSize(args...)};
    // Allocate memory for packet serialization
    std::size_t const alloc_size{ipc_protocol::kProtocolMessageHeaderLength +
                                 ipc_protocol::kRequestMessageHeaderLength + payload_size};

    // Creation of a buffer to serialize the generic protocol header, the request header and the payload.
    ipc_protocol::IpcPacketShared packet{ipc_protocol::IpcPacketSharedConstruct(alloc_size)};
    ::amsr::someip_protocol::internal::serialization::BufferView const body_view{*packet};
    ::amsr::someip_protocol::internal::serialization::Writer writer{body_view};
    // Serialize IPC request message header
    ipc_protocol::serialization::SerializeRequestMessageHeader(writer, header, payload_size);

    // Serialize method arguments
    RequestSerializer::Serialize(writer, args...);

    // PTP-E-IpcBinding-Xf-Proxy-Serialize-MethodRequest

    logger_.LogVerbose(
        [&header](Logger::LogStream& s) {
          s << "Emplacing promise for proxy method request in pending requests map (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s,
                                  header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogSessionId(s,
                                   header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    ::amsr::core::Optional<::amsr::core::Future<Output>> added_request{
        pending_request_map_->StoreRequest(header.session_id_)};

    ::amsr::core::Future<Output> result{};
    if (added_request.has_value()) {
      result = std::move(added_request.value());
      RequestMessage message{header, std::move(packet)};

      // VCA_IPCB_VALID_TRACE_SINK
      trace_sink_.TraceRequestSend(provided_service_instance_id_, client_id_, message);

      if (!SendMessage(std::move(message))) {
        result = CreateErrorFuture(ara::com::ComErrc::kServiceNotAvailable, "Failed to send method request.");
      }
    } else {
      logger_.LogError(
          [&header](Logger::LogStream& s) {
            s << "Failed to emplace promise object for proxy method request in pending requests map. Request will be "
                 "dropped and is not sent (";
            LogBuilder::LogCompleteServiceInstanceMethodId(
                s, header.service_id_, header.major_version_, header.instance_id_,
                header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogClientId(s,
                                    header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogSessionId(s,
                                     header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ")";
          },
          Logger::LogLocation{static_cast<char const*>(__func__),
                              __LINE__});  // COV_IpcBinding_CoverageComponentOverall_trivial_utility

      result = CreateErrorFuture(ara::com::ComErrc::kNetworkBindingFailure,
                                 "Session ID already in use: Method request was not sent.");
    }

    return result;
  }

 private:
  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::ipc_binding_core::internal::SessionHandler;

  /*!
   * \brief Type alias for RequestMessageHeader.
   */
  using RequestMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessageHeader;

  /*!
   * \brief Type alias for RequestMessage.
   */
  using RequestMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessage;

  /*!
   * \brief Type alias for ConnectionProxyInterface.
   */
  using ConnectionProxyInterface = ::amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface;

  /*!
   * \brief Type alias for TransmitMessageHandlerInterface.
   */
  using TransmitMessageHandlerInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface;

  /*!
   * \brief Get the next session ID.
   *
   * \return The next session ID value.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Lock the session_id_mutex_ in order to increment session ID safely.
   *   - Store the current session ID.
   *   - If the max limit for session is set and the max limit has reached,.
   *     - Reset the session ID.
   *   - Otherwise, increment the session ID for the next method request.
   *   - Return the stored session ID.
   * \endinternal
   */
  auto GetNextSessionId() noexcept -> SessionId {
    std::lock_guard<std::mutex> const lock{session_id_mutex_};
    SessionId const session_id{session_.GetSessionId()};

    // condition check is needed only for testing: it allows the testing of an overflow/rollover of the session-id to
    // test the use case "session-id already in use". Without the max_limit_session_, a test would need to create
    // '2^(sizeof(session_id)*8)' method requests, which would cause an out-of-memory error.
    if ((max_limit_session_ > 0) && (session_id.value == max_limit_session_)) {
      session_.Reset();  //  COV_IpcBinding_FunctionCallCoverage_BSOCIAL_8246
    } else {
      ++session_;
    }

    return session_id;
  };

  /*!
   * \brief Send the RequestMessage via ConnectionProxy.
   *
   * \param[in] message  RequestMessage to be sent.
   *
   * \return Boolean indicating whether the message has been handled.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if ConnectionProxy is idle and packet fits into remaining buffer).
   * \steady      TRUE
   *
   * \internal
   * - Try to access the related ConnectionProxy instance:
   *   - Send method request via ConnectionProxy.
   *   - If send operation failed:
   *     - Simulate reception of an artificial error response with error code 'ServiceNotAvailable'
   * - Otherwise log a warning and drop the method request message transmission.
   * \endinternal
   */
  auto SendMessage(RequestMessage&& message) noexcept -> bool {
    // PTP-B-IpcBinding-Xf-Proxy-Send-MethodRequest

    namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;
    RequestMessageHeader const& request_header{message.GetMessageHeader()};

    bool message_handled{false};

    // VCA_IPCB_VALID_PROXY_ROUTER
    amsr::SharedPtr<ConnectionProxyInterface> const connection_proxy{proxy_router_.GetConnectionProxy()};
    if (connection_proxy != nullptr) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
      // VCA_IPCB_PROXYMETHOD_CONNECTION_PROXY_VALID
      TransmitMessageHandlerInterface& transmit_message_handler{connection_proxy->GetTransmitMessageHandler()};
      // VCA_IPCB_PROXYMETHOD_CONNECTION_PROXY_TRANSMIT_MESSAGE_HANDLER_VALID
      bool const send_result{transmit_message_handler.SendRequest(std::move(message))};
      if (!send_result) {
        ReceiveErrorResponseSendFailed(request_header);
      }
      message_handled = true;
    } else {
      logger_.LogWarn(
          [&request_header](Logger::LogStream& s) {
            s << "Unable to access ConnectionProxy for transmission of method request (";
            LogBuilder::LogCompleteServiceInstanceMethodId(
                s, request_header.service_id_, request_header.major_version_, request_header.instance_id_,
                request_header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogClientId(
                s, request_header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogSessionId(
                s, request_header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << "). The request message will be dropped.";
          },
          Logger::LogLocation{static_cast<char const*>(__func__),
                              __LINE__});  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
    }

    // PTP-E-IpcBinding-Xf-Proxy-Send-MethodRequest
    return message_handled;
  };

  /*!
   * \brief Send the RequestMessage via ConnectionProxy.
   *
   * \param[in] request_header  Header for the request message.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if ConnectionProxy is idle and packet fits into remaining buffer).
   * \steady      TRUE
   *
   * \internal
   * - Construct an ErrorResponse message with error code 'ServiceNotAvailable'.
   * - Try to access the related MethodXf:
   *   - Notify the ProxyMethodXf about the artificial (message not actually received via IPC) ErrorResponse.
   * - Otherwise log a warning the MethodXf cannot be accessed and drop the error response.
   * \endinternal
   */
  auto ReceiveErrorResponseSendFailed(RequestMessageHeader const& request_header) noexcept -> void {
    namespace ipc_protocol = ipc_binding_core::internal::ipc_protocol;

    ipc_protocol::ErrorResponseMessageHeader const error_response_header{
        request_header.service_id_,
        request_header.instance_id_,
        request_header.major_version_,
        request_header.method_id_,
        request_header.client_id_,
        request_header.session_id_,
        ipc_protocol::ReturnCode::kServiceNotAvailable};

    ipc_binding_core::internal::ProxyMethodBackendInterface* const method_xf{
        proxy_router_.GetMethodXf(request_header.method_id_)};  // VCA_IPCB_VALID_PROXY_ROUTER
    if (method_xf != nullptr) {
      ipc_protocol::ErrorResponseMessage error_response{ipc_protocol::IpcMessageBuilder::CreateErrorResponseMessage(
          error_response_header)};  // COV_IpcBinding_CoverageComponentOverall_trivial_utility

      // VCA_IPCB_PROXYMETHOD_PROXY_ROUTER_METHOD_XF_VALID
      method_xf->OnErrorResponseReceived(std::move(error_response));
    } else {
      logger_.LogWarn(
          [&error_response_header](::ara::log::LogStream& s) {
            s << "Failed to access MethodXf for error response reception (";
            LogBuilder::LogCompleteServiceInstanceMethodId(
                s, error_response_header.service_id_, error_response_header.major_version_,
                error_response_header.instance_id_,
                error_response_header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogClientId(
                s, error_response_header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ", ";
            LogBuilder::LogSessionId(
                s, error_response_header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility

            s << ") Message will be dropped.";
          },
          Logger::LogLocation{static_cast<char const*>(__func__),
                              __LINE__});  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
    }
  }

  /*!
   * \brief Create a future that contains an error.
   *
   * \param[in] error_code     Error code.
   * \param[in] error_message  Error message.
   *
   * \return Future with an error.
   *
   * \pre     -
   * \context App
   * \steady  TRUE
   *
   * \internal
   * - Create a future-promise pair.
   * - Store an error in the promise.
   * - Return the future.
   * \endinternal
   */
  static auto CreateErrorFuture(ara::com::ComErrc error_code, char const* error_message) noexcept
      -> ::amsr::core::Future<Output> {
    ::amsr::core::Promise<Output> promise{};
    ::amsr::core::Future<Output> future{promise.get_future()};
    promise.SetError({error_code, error_message});

    return future;
  }

  /*!
   * \brief Reference to the logger of the method.
   */
  Logger& logger_;

  /*!
   * \brief Pending request map object to store method requests.
   */
  amsr::SharedPtr<PendingRequestMap> pending_request_map_;

  /*!
   * \brief The provided service instance ID.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief The method deployment ID.
   */
  MethodId const method_id_;

  /*!
   * \brief The client ID.
   */
  ClientId const client_id_;

  /*!
   * \brief Reference to the ProxyRouter.
   */
  ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Sink for tracing of methods.
   */
  TraceMethodSinkInterface& trace_sink_;

  /*!
   * \brief   Maximum value of session ID. After reaching this value, the session ID must be reset.
   * \details Currently only used for testing, But later can be used for limiting the size of the request map as well.
   */
  SessionId::type const max_limit_session_;

  /*!
   * \brief Provides functionality to create unique IDs for each request (session ID).
   */
  Session session_{};

  /*!
   * \brief Mutex to protect session id.
   */
  std::mutex session_id_mutex_{};
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PROXY_REQUEST_HANDLER_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_transformation_layer::internal::methods::ProxyRequestHandler::session_id_mutex_
 *                Protects from parallel transmission of method requests.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal:::ProxyRequestHandler::session_
 * \usedin   amsr::ipc_binding_transformation_layer::internal:::ProxyRequestHandler::GetNextSessionId
 * \exclude  All other methods of ProxyRequestHandler.
 * \length   SHORT Increment of numeric session ID.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
//
// \ID COV_IpcBinding_FunctionCallCoverage_BSOCIAL_8246
//   \ACCEPT  XX
//   \REASON  FunctionCallCoverage gap is reported for the API.
//            Since the gap is reported wrong implementation the story BSOCIAL-8246 is created the fix the gap.
// COV_JUSTIFICATION_END
