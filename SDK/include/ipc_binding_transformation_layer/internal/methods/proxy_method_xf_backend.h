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
 *        \brief  Method XF to handle common logic for method requests/responses on proxy side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_XF_BACKEND_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/promise.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/methods/proxy_method_backend_interface.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_message_builder.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/methods/pending_request_map.h"
#include "ipc_binding_core/internal/methods/proxy_request_handler.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_transformation_layer/internal/methods/application_error_builder.h"
#include "ipc_binding_transformation_layer/internal/methods/proxy_application_error_response_xf.h"
#include "ipc_binding_transformation_layer/internal/methods/proxy_method_error_response_xf.h"
#include "ipc_binding_transformation_layer/internal/methods/proxy_method_response_xf.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

// VECTOR Next Construct AutosarC++17_10-A10.1.1: MD_IPCBINDING_AutosarC++17_10-A10.1.1_multiple_inheritance
/*!
 * \brief Method manager to handle method request/response on proxy side.
 *
 * \tparam Output                Struct type containing all method output arguments.
 * \tparam RequestSerializer     The serializer type used to serialize the method request packet.
 * \tparam ResponseDeserializer  Deserializer type for the method response.
 * \tparam InputArgsT            Types of the method input arguments.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
class ProxyMethodXfBackend final
    : public ::amsr::ipc_binding_core::internal::ProxyMethodBackendInterface,
      public ::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output, InputArgsT...> {
 public:
  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for TraceMethodSinkInterface.
   */
  using TraceMethodSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface;

  /*!
   * \brief Type-alias for MethodId.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type-alias for ClientId.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Type-alias for ProxyRouterInterface.
   */
  using ProxyRouterInterface = ipc_binding_core::internal::ProxyRouterInterface;

  /*!
   * \brief Type-alias for PendingRequestMap.
   */
  using PendingRequestMap = ipc_binding_core::internal::methods::PendingRequestMap<Output>;

  /*!
   * \brief Constructor of ProxyMethodXfBackend.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] method_id                     Method deployment ID.
   * \param[in] client_id                     The client ID.
   * \param[in] trace_sink                    Trace sink.
   * \param[in] proxy_router                  Reference to the ProxyRouter.
   * \param[in] application_errors            Map of all application errors and their error domains. (The map will not
   *                                          have any entries for field getter and setter).
   * \param[in] max_limit_session             Maximum value of session ID. After reaching this value, the session ID
   *                                          must be reset. If the value is not set explicitly then the wrap around of
   *                                          session will happen when usual max limit of the SessionId::type is
   *                                          reached.
   *
   * \pre     Proxy router pointer must be valid for the whole lifetime of the ProxyMethodXfBackend.
   * \context App
   */
  ProxyMethodXfBackend(ProvidedServiceInstanceId const provided_service_instance_id, MethodId const method_id,
                       ClientId const client_id,
                       std::unique_ptr<TraceMethodSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
                       ProxyRouterInterface& proxy_router,
                       ApplicationErrorBuilder::ApplicationErrorMap application_errors,
                       ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId::type max_limit_session = 0) noexcept
      : ::amsr::ipc_binding_core::internal::ProxyMethodBackendInterface{},
        // () is used to initialize the base class to suppress doxygen issue.
        ::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output, InputArgsT...>(),
        // () is used to initialize the base class to suppress doxygen issue.
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyMethodXfBackend"}},
        provided_service_instance_id_{provided_service_instance_id},
        client_id_{client_id},
        method_id_{method_id},
        trace_sink_{std::move(trace_sink)},                                   // VCA_IPCB_VALID_TRACE_SINK
        pending_request_map_{amsr::MakeShared<PendingRequestMap>().Value()},  // VCA_IPCB_MAKE_SHARED_SAFE_FUNCTION
        proxy_router_{proxy_router},
        // VCA_IPCB_VALID_TRACE_SINK
        request_handler_{logger_,      pending_request_map_, provided_service_instance_id,
                         method_id,    client_id_,           proxy_router_,
                         *trace_sink_, max_limit_session},
        // VCA_IPCB_PROXY_METHOD_MANAGER_APPLICATION_ERROR_LIST_INVALID
        application_errors_{std::move(application_errors)} {
    if (!trace_sink_) {
      logger_.TerminateOnViolation("Tried to construct a ProxyMethodXfBackend with an invalid (empty) trace sink.",
                                   Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    // VCA_IPCB_PROXYMETHOD_VALID_PROXY_ROUTER
    proxy_router_.AddMethodXf(method_id, *this);
  }

  ProxyMethodXfBackend(ProxyMethodXfBackend&&) noexcept = delete;
  auto operator=(ProxyMethodXfBackend&&) noexcept -> ProxyMethodXfBackend& = delete;
  ProxyMethodXfBackend(ProxyMethodXfBackend const&) noexcept = delete;
  auto operator=(ProxyMethodXfBackend const&) noexcept -> ProxyMethodXfBackend& = delete;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_MOLE_1298
  ~ProxyMethodXfBackend() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    // VCA_IPCB_PROXYMETHOD_VALID_PROXY_ROUTER
    proxy_router_.ReleaseMethodXf(method_id_);
  }  // VCA_IPCB_STD_MAP_DESTRUCTOR

  /*!
   * \brief Carry out the method request.
   *
   * \param[in] args  References to the method input arguments.
   *
   * \return A future object to hold the result of the method call.
   *
   * \error Method-specific  Errors will be returned in the result Future.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto HandleRequest(InputArgsT const&... args) noexcept -> ::ara::core::Future<Output> final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyMethodBackendInterface::OnResponseReceived
   *
   * \spec requires true; \endspec
   */
  void OnResponseReceived(::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage&& response) noexcept final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyMethodBackendInterface::OnApplicationErrorReceived
   *
   * \spec requires true; \endspec
   */
  void OnApplicationErrorReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage&& message) noexcept final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyMethodBackendInterface::OnErrorResponseReceived
   *
   * \spec requires true; \endspec
   */
  void OnErrorResponseReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage&& message) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::SetConnectionState
   */
  void SetConnectionState(bool connection_state) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::SetServiceState
   */
  void SetServiceState(bool service_state) noexcept final;

 private:
  /*!
   * \brief Type alias for logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for the ProxyRequestHandler.
   */
  using ProxyRequestHandler =
      ipc_binding_core::internal::methods::ProxyRequestHandler<Output, RequestSerializer, InputArgsT...>;

  /*!
   * \brief Cancel all pending method request promises with an artificial ErrorResponse using return code
   *        'ServiceNotAvailable'.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     TRUE
   */
  auto CancelAllPendingRequests() const noexcept -> void;

  /*!
   * \brief Handles application error message when it is received.
   *
   * \param[in,out] message  Application error message to be forwarded.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void HandleApplicationErrorReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage&& message) const;

  /*!
   * \brief Handles error response message when it is received.
   *
   * \param[in,out] message  Error message to be forwarded.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void HandleErrorResponseReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage&& message) const;

  /*!
   * \brief   Method logger for printing debug and error messages.
   * \details The concrete method will set the prefix for identification.
   */
  Logger logger_;

  /*!
   * \brief The provided service instance ID.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief The proxy instance client ID.
   */
  ClientId const client_id_;

  /*!
   * \brief The method deployment ID.
   */
  MethodId const method_id_;

  /*!
   * \brief Sink for tracing of methods.
   */
  std::unique_ptr<TraceMethodSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief Pending request map object to store method requests.
   */
  amsr::SharedPtr<PendingRequestMap> pending_request_map_;

  /*!
   * \brief   Pointer to the ProxyRouter.
   * \details The shared pointer will be used to create weak pointers that can later be stored and used in the request
   *          queue. By doing so we ensure that the router will still be valid when it is used later in time.
   */
  ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Proxy request handler.
   */
  ProxyRequestHandler request_handler_;

  /*!
   * \brief Possible application errors and their error domains referenced by this method.
   */
  ApplicationErrorBuilder::ApplicationErrorMap application_errors_;

  /*!
   * \brief Mutex for protection against parallel requests.
   */
  std::mutex service_up_lock_{};

  /*!
   * \brief Holds the currently known service instance state.
   */
  bool service_state_{true};
};

/*!
 * \internal
 * - If service is up.
 *   - Forward the method request to request handler.
 * - Else log a warning and respond immediately with error.
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A15.4.2: MD_IPCBINDING_AutosarC++17_10-A15.4.2_STL_exceptions
// VECTOR NC AutosarC++17_10-A15.5.3: MD_IPCBINDING_AutosarC++17_10-A15.5.3_STL_exceptions
// VECTOR NC AutosarC++17_10-A2.11.1: MD_IPCBINDING_AutosarC++17_10_A2.11.1_args_parameter_shadowing
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
auto ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::HandleRequest(
    InputArgsT const&... args) noexcept -> ::amsr::core::Future<Output> {
  ::amsr::core::Future<Output> result{};

  // Obtain current offered state, guarded by locking mutex
  bool const service_up{[this]() {
    // VCA_IPCB_LOCK_GUARD_LIFETIME
    std::lock_guard<std::mutex> const lock{service_up_lock_};
    return service_state_;
  }()};

  if (service_up) {
    result = std::move(request_handler_.HandleMethodRequest(args...));
  } else {
    logger_.LogWarn([](::ara::log::LogStream& s) { s << "Proxy method request invoked, but service is down."; },
                    Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    ::amsr::core::Promise<Output> promise{};
    result = std::move(promise.get_future());
    promise.SetError({::ara::com::ComErrc::kServiceNotAvailable, "Proxy method request invoked, but service is down."});
  }

  return result;
}

/*!
 * \internal
 * - Extract message header from the response.
 * - Find the matching promise for the response by using its session id and remove it from the pending requests list.
 * - Check if the promise was found.
 *   - If application errors are defined:
 *     - Trace the response received with the trace sink.
 *     - Create and move response functor into promise.
 *   - Else log warning and resolve promise with kNetworkBindingFailure.
 * - Otherwise, log an error message.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::OnApplicationErrorReceived(
    ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage&& message) noexcept {
  logger_.LogDebug(
      [&message](::ara::log::LogStream& s) {
        ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessageHeader const header{
            message.GetMessageHeader()};
        s << "Application error received (";
        LogBuilder::LogCompleteServiceInstanceMethodId(s, header.service_id_, header.major_version_,
                                                       header.instance_id_, header.method_id_);
        s << ", ";
        LogBuilder::LogSessionId(s, header.session_id_);
        s << ")";
      },
      Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessageHeader const& header{
      message.GetMessageHeader()};
  ::amsr::core::Optional<::amsr::core::Promise<Output>> request_optional{
      pending_request_map_->MoveOutRequest(header.session_id_)};

  if (request_optional.has_value()) {
    ::amsr::core::Promise<Output> pending_request{std::move(request_optional.value())};

    if (!application_errors_.empty()) {
      // VCA_IPCB_VALID_TRACE_SINK
      trace_sink_->TraceApplicationErrorResponseReceived(provided_service_instance_id_, client_id_, message);

      // Create ProxyApplicationErrorResponseXf object.
      ProxyApplicationErrorResponseXf<Output> method_res_xf{std::move(message), application_errors_};
      // Response is stored in promise.
      pending_request.SetExecutable(std::move(method_res_xf));
    } else {
      // No application errors configured.
      logger_.LogWarn(
          [&header](::ara::log::LogStream& s) {
            s << "ApplicationError response received although no ApplicationErrors are configured for the method (";
            LogBuilder::LogCompleteServiceInstanceMethodId(s, header.service_id_, header.major_version_,
                                                           header.instance_id_, header.method_id_);
            s << ")";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

      pending_request.SetError(
          {::ara::com::ComErrc::kNetworkBindingFailure,
           "ApplicationError response received although no ApplicationErrors are configured for the method."});
    }

  } else {
    logger_.LogError(
        [&header](::ara::log::LogStream& s) {
          s << "No request found for the received ApApplicationError response with (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogSessionId(s,
                                   header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << "). Routing failed!";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
  }
}

/*!
 * \internal
 * - Process the error response message.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::OnErrorResponseReceived(
    ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage&& message) noexcept {
  HandleErrorResponseReceived(std::move(message));
}

/*!
 * \internal
 * - If connection is disconnected:
 *   - Cancel all pending method requests.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::SetConnectionState(
    bool connection_state) noexcept {
  if (!connection_state) {
    CancelAllPendingRequests();
  }
}

/*!
 * \internal
 * - Update stored service state.
 * - Cancel all pending method requests.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::SetServiceState(
    bool service_state) noexcept {
  {
    std::lock_guard<std::mutex> const lock{service_up_lock_};
    service_state_ = service_state;
  }

  if (!service_state) {
    CancelAllPendingRequests();
  }
}

/*!
 * \internal
 * - Loop over all pending method requests:
 *   - Move promise out of pending requests map (deletes the map entry).
 *   - Create an artificial error response with return code 'ServiceNotAvailable'.
 *   - Resolve the promise with an artificial ErrorResponse message using return code 'ServiceNotAvailable'
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::CancelAllPendingRequests()
    const noexcept {
  namespace ipc_protocol = ipc_binding_core::internal::ipc_protocol;
  ipc_protocol::ErrorResponseMessageHeader error_header{provided_service_instance_id_.ServiceId(),
                                                        provided_service_instance_id_.InstanceId(),
                                                        provided_service_instance_id_.MajorVersion(),
                                                        method_id_,
                                                        client_id_,
                                                        {0},  // To be set by the returned session id
                                                        ipc_protocol::ReturnCode::kServiceNotAvailable};

  logger_.LogDebug(
      [&error_header](::ara::log::LogStream& s) {
        s << "Resolving all pending method requests with error 'ServiceNotAvailable' (";

        LogBuilder::LogCompleteServiceInstanceMethodId(s, error_header.service_id_, error_header.major_version_,
                                                       error_header.instance_id_, error_header.method_id_);
        s << ", ";
        LogBuilder::LogClientId(s, error_header.client_id_);
        s << ")";
      },
      Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  typename PendingRequestMap::OptionalMapEntry request{pending_request_map_->MoveOutNextRequest()};
  // VECTOR NL AutosarC++17_10-A12.8.3: MD_IPCBINDING_AutosarC++17_10_A12.8.3_false_positive_moved_from
  while (request.has_value()) {
    error_header.session_id_.value = request.value().first;
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_IPCBINDING_AutosarC++17_10_A12.8.3_false_positive_moved_from
    ::amsr::core::Promise<Output> pending_request{std::move(request.value().second)};
    ipc_protocol::ErrorResponseMessage error_message{
        ipc_protocol::IpcMessageBuilder::CreateErrorResponseMessage(error_header)};

    ProxyMethodErrorResponseXf<Output> method_res_xf{std::move(error_message)};
    // Response is stored in promise.
    pending_request.SetExecutable(std::move(method_res_xf));

    // VCA_IPCB_PROXY_METHOD_PENDING_REQUEST_MAP_SHARED_PTR
    request = pending_request_map_->MoveOutNextRequest();
  }
}

/*!
 * \internal
 * - Extract message header from the response.
 * - Find the matching promise for the response by using its session id and remove it from the pending requests list.
 * - Check if the promise was found.
 *   - Trace the response received with the trace sink.
 *   - Create and move response functor into promise.
 * - Otherwise, log an error message.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::OnResponseReceived(
    ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage&& response) noexcept {
  logger_.LogDebug(
      [&response](::ara::log::LogStream& s) {
        ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessageHeader const header{
            response.GetMessageHeader()};
        s << "Proxy method response received (";
        LogBuilder::LogCompleteServiceInstanceMethodId(s, header.service_id_, header.major_version_,
                                                       header.instance_id_, header.method_id_);
        s << ", ";
        LogBuilder::LogSessionId(s, header.session_id_);
        s << ")";
      },
      Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessageHeader const& header{response.GetMessageHeader()};
  ::amsr::core::Optional<::amsr::core::Promise<Output>> request_optional{
      pending_request_map_->MoveOutRequest(header.session_id_)};

  if (request_optional.has_value()) {
    // VCA_IPCB_VALID_TRACE_SINK
    trace_sink_->TraceResponseReceived(provided_service_instance_id_, client_id_, response);

    ::amsr::core::Promise<Output> pending_request{std::move(request_optional.value())};
    // Create ProxyMethodResponseXf object.
    ProxyMethodResponseXf<ResponseDeserializer, Output> method_res_xf{std::move(response)};
    // Response is stored in promise.
    pending_request.SetExecutable(std::move(method_res_xf));
  } else {
    logger_.LogError(
        [&header](::ara::log::LogStream& s) {
          s << "No request found for the received response with ";
          LogBuilder::LogSessionId(s,
                                   header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ResponseMessage: ";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ". Routing failed!";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
  }
}

/*!
 * \internal
 * - Extract message header from the response.
 * - Find the matching promise for the response by using its session id and remove it from the pending requests list.
 * - Check if the promise was found.
 *   - Create and move response functor into promise.
 * - Otherwise, log an error message.
 * \endinternal
 */
template <typename Output, typename RequestSerializer, typename ResponseDeserializer, typename... InputArgsT>
void ProxyMethodXfBackend<Output, RequestSerializer, ResponseDeserializer, InputArgsT...>::HandleErrorResponseReceived(
    ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage&& message) const {
  logger_.LogDebug(
      [&message](::ara::log::LogStream& s) {
        ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessageHeader const header{
            message.GetMessageHeader()};
        s << "Proxy method error response received (";
        LogBuilder::LogCompleteServiceInstanceMethodId(s, header.service_id_, header.major_version_,
                                                       header.instance_id_, header.method_id_);
        s << ", ";
        LogBuilder::LogSessionId(s, header.session_id_);
        s << ")";
      },
      Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessageHeader const& header{
      message.GetMessageHeader()};
  ::amsr::core::Optional<::amsr::core::Promise<Output>> request_optional{
      // VCA_IPCB_PROXY_METHOD_PENDING_REQUEST_MAP_SHARED_PTR
      pending_request_map_->MoveOutRequest(header.session_id_)};

  if (request_optional.has_value()) {
    ::amsr::core::Promise<Output> pending_request{std::move(request_optional.value())};
    // Create ProxyMethodErrorResponseXf object.
    ProxyMethodErrorResponseXf<Output> method_res_xf{std::move(message)};
    // Response is stored in promise.
    pending_request.SetExecutable(std::move(method_res_xf));
  } else {
    logger_.LogError(
        [&header](::ara::log::LogStream& s) {
          s << "No request found for the received error response with ";
          LogBuilder::LogSessionId(s,
                                   header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ErrorResponseMessage: ";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ". Routing failed!";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
  }
}

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_XF_BACKEND_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_transformation_layer::internal::ProxyMethodXfBackend::service_up_lock_
 *                Used for protecting against parallel requests and service instance state callbacks from reactor.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal::ProxyMethodXfBackend::service_state_,
 *
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyMethodXfBackend::SetServiceState
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyMethodXfBackend::HandleMethodRequest
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length MEDIUM Only for read and write operation on the service state.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
