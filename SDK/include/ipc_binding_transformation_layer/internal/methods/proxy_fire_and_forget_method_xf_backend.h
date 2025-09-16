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
 *        \brief  Method XF to handle common logic for fire and forget method requests on proxy side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/methods/proxy_fire_and_forget_method_backend_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/ipc_protocol/ser_ipc_headers.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/session_handler.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief   Method manager to handle fire and forget method request on proxy side.
 * \details This is the base class for the generated Proxy<ClientServerOperation.shortName> classes and maintains the
 *          logger and pending request map for specific methods.
 *
 * \tparam RequestSerializer  The serializer type used to serialize method request.
 * \tparam InputArgsT         Types of the method input arguments.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
template <typename RequestSerializer, typename... InputArgsT>
class ProxyFireAndForgetMethodXfBackend final
    : public ::amsr::ipc_binding_core::internal::ProxyFireAndForgetMethodBackendInterface,
      public ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<InputArgsT...> {
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
   * \brief Constructor of ProxyFireAndForgetMethodManager.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] method_id                     Method deployment ID.
   * \param[in] client_id                     The client ID.
   * \param[in] trace_sink                    Trace sink.
   * \param[in] proxy_router                  Reference to the ProxyRouter.
   *
   * \pre     -
   * \context App
   */
  ProxyFireAndForgetMethodXfBackend(
      ProvidedServiceInstanceId const provided_service_instance_id, MethodId const method_id, ClientId const client_id,
      std::unique_ptr<TraceMethodSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
      ProxyRouterInterface& proxy_router) noexcept
      : ::amsr::ipc_binding_core::internal::ProxyFireAndForgetMethodBackendInterface{},
        // () is used to initialize the base class to suppress doxygen issue.
        ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<InputArgsT...>(),
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyFireAndForgetMethodXfBackend"}},
        provided_service_instance_id_{provided_service_instance_id},
        method_id_{method_id},
        client_id_{client_id},
        trace_sink_{std::move(trace_sink)},  // VCA_IPCB_VALID_TRACE_SINK
        proxy_router_{proxy_router} {
    if (!trace_sink_) {
      logger_.TerminateOnViolation(
          "Tried to construct a ProxyFireAndForgetMethodXfBackend with an invalid (empty) trace sink.",
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    // VCA_IPCB_PROXYMETHOD_VALID_PROXY_ROUTER
    proxy_router_.AddFireAndForgetMethodXf(method_id, *this);
  }

  ProxyFireAndForgetMethodXfBackend(ProxyFireAndForgetMethodXfBackend&&) noexcept = delete;
  auto operator=(ProxyFireAndForgetMethodXfBackend&&) noexcept -> ProxyFireAndForgetMethodXfBackend& = delete;
  ProxyFireAndForgetMethodXfBackend(ProxyFireAndForgetMethodXfBackend const&) noexcept = delete;
  auto operator=(ProxyFireAndForgetMethodXfBackend const&) noexcept -> ProxyFireAndForgetMethodXfBackend& = delete;

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
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~ProxyFireAndForgetMethodXfBackend() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    // VCA_IPCB_PROXYMETHOD_VALID_PROXY_ROUTER
    proxy_router_.ReleaseFireAndForgetMethodXf(method_id_);
  }

  /*!
   * \brief Carry out the method request.
   *
   * \param[in] args  References to the method input arguments.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   */
  auto HandleRequest(InputArgsT const&... args) noexcept -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyFireAndForgetMethodBackendInterface::SetServiceState
   */
  auto SetServiceState(bool service_state) noexcept -> void final;

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
   * \brief Type alias for session ID.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::ipc_binding_core::internal::SessionHandler;

  /*!
   * \brief Type alias for RequestNoReturnMessage.
   */
  using RequestNoReturnMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage;

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
   */
  auto GetNextSessionId() noexcept -> SessionId;

  /*!
   * \brief Send the RequestNoReturnMessage via ConnectionProxy.
   *
   * \param[in] message  RequestNoReturnMessage to be sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if ConnectionProxy is idle and packet fits into remaining buffer).
   */
  auto SendMessage(RequestNoReturnMessage&& message) const noexcept -> void;

  /*!
   * \brief Method logger for printing debug and error messages.
   */
  Logger logger_;

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
   * \brief Sink for tracing of methods.
   */
  std::unique_ptr<TraceMethodSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief The ProxyRouter.
   */
  ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Provides functionality to create unique IDs for each request (session).
   */
  Session session_{};

  /*!
   * \brief Mutex to protect session id.
   */
  std::mutex session_id_mutex_{};

  /*!
   * \brief Holds the currently known service instance state.
   */
  bool service_state_{true};

  /*!
   * \brief Mutex for protection against parallel requests.
   */
  std::mutex service_up_lock_{};
};

// ---- Public APIs ----------------------------------------------------------------------------------------------------

/*!
 * \internal
 * - Get the next session ID.
 * - Build the fire and forget method request message header.
 * - Allocate the buffer of size header + payload.
 * - Serialize the message header.
 * - Serialize the payload.
 * - Create a request message to transmit the packet.
 * - Trace the request send with the trace sink.
 * - Transmit the packet.
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A2.11.1: MD_IPCBINDING_AutosarC++17_10_A2.11.1_args_parameter_shadowing
template <typename RequestSerializer, typename... InputArgsT>
auto ProxyFireAndForgetMethodXfBackend<RequestSerializer, InputArgsT...>::HandleRequest(
    InputArgsT const&... args) noexcept -> void {
  namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;

  // Obtain current offered state, guarded by locking mutex
  bool const service_up{[this]() {
    // VCA_IPCB_LOCK_GUARD_LIFETIME
    std::lock_guard<std::mutex> const lock{service_up_lock_};
    return service_state_;
  }()};

  if (service_up) {
    // PTP-B-IpcBinding-Xf-Proxy-Serialize-MethodRequest-FireAndForget
    SessionId const session_id{GetNextSessionId()};
    ipc_protocol::RequestNoReturnMessageHeader const header{provided_service_instance_id_.ServiceId(),
                                                            provided_service_instance_id_.InstanceId(),
                                                            provided_service_instance_id_.MajorVersion(),
                                                            method_id_,
                                                            client_id_,
                                                            session_id};

    logger_.LogVerbose(
        [&header](Logger::LogStream& s) {
          s << "Handling proxy fire and forget method request (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s, header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogSessionId(s, header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // Calculate the required size for the request arguments
    std::size_t const payload_size{RequestSerializer::GetRequiredBufferSize(args...)};
    // Allocate memory for packet serialization
    std::size_t const alloc_size{ipc_protocol::kProtocolMessageHeaderLength +
                                 ipc_protocol::kRequestNoReturnMessageHeaderLength + payload_size};
    // Creation of a buffer to serialize the generic protocol header, the request header and the payload.
    ipc_protocol::IpcPacketShared packet{ipc_protocol::IpcPacketSharedConstruct(alloc_size)};

    ::amsr::someip_protocol::internal::serialization::BufferView const body_view{*packet};
    ::amsr::someip_protocol::internal::serialization::Writer writer{body_view};

    // Serialize IPC fire&forget message request header
    ipc_protocol::serialization::SerializeRequestNoReturnMessageHeader(writer, header, payload_size);

    // Serialize method arguments
    RequestSerializer::Serialize(writer, args...);
    ipc_protocol::RequestNoReturnMessage message{header, std::move(packet)};

    // PTP-E-IpcBinding-Xf-Proxy-Serialize-MethodRequest-FireAndForget

    // VCA_IPCB_VALID_TRACE_SINK
    trace_sink_->TraceRequestNoReturnSend(provided_service_instance_id_, client_id_, message);

    SendMessage(std::move(message));
  } else {
    logger_.LogWarn(
        [this](Logger::LogStream& s) {
          s << "Unable to send fire and forget method request (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s,
              // VCA_IPCB_LOG_LAMBDA_PASS_CLASS_MEMBER_BY_REFERENCE
              provided_service_instance_id_.ServiceId(),  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              // VCA_IPCB_LOG_LAMBDA_PASS_CLASS_MEMBER_BY_REFERENCE
              provided_service_instance_id_.MajorVersion(),  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              // VCA_IPCB_LOG_LAMBDA_PASS_CLASS_MEMBER_BY_REFERENCE
              provided_service_instance_id_.InstanceId(),  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              method_id_);                                 // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s, client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ") as service is currently down. The fire and forget method request will be dropped";
        },
        Logger::LogLocation{static_cast<char const*>(__func__),
                            __LINE__});  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
  }
}

/*!
 * \internal
 * - Update stored service state.
 * \endinternal
 */
template <typename RequestSerializer, typename... InputArgsT>
auto ProxyFireAndForgetMethodXfBackend<RequestSerializer, InputArgsT...>::SetServiceState(bool service_state) noexcept
    -> void {
  std::lock_guard<std::mutex> const lock{service_up_lock_};
  service_state_ = service_state;
}

// ---- Private APIs ---------------------------------------------------------------------------------------------------

/*!
 * \internal
 * - Lock the session_id_mutex_ in order to increment session id safely.
 *   - Get the session id for this handler.
 *   - Store the current session ID.
 *   - Increment the session ID for the next method request.
 *   - Return stored session ID.
 * \endinternal
 */
template <typename RequestSerializer, typename... InputArgsT>
auto ProxyFireAndForgetMethodXfBackend<RequestSerializer, InputArgsT...>::GetNextSessionId() noexcept -> SessionId {
  std::lock_guard<std::mutex> const lock{session_id_mutex_};
  SessionId const session_id{session_.GetSessionId()};
  ++session_;

  return session_id;
}

/*!
 * \internal
 * - Try to access the ProxyRouter instance:
 *   - Try to access the related ConnectionProxy instance:
 *     - Send method request via ConnectionProxy.
 *   - Otherwise log a warning and drop the method request message transmission.
 * - Otherwise log a warning and drop the method request message transmission.
 * \endinternal
 */
template <typename RequestSerializer, typename... InputArgsT>
auto ProxyFireAndForgetMethodXfBackend<RequestSerializer, InputArgsT...>::SendMessage(
    RequestNoReturnMessage&& message) const noexcept -> void {
  // PTP-B-IpcBinding-Xf-Proxy-Send-MethodRequest-FireAndForget

  // VCA_IPCB_VALID_PROXY_ROUTER
  amsr::SharedPtr<ConnectionProxyInterface> const connection_proxy{proxy_router_.GetConnectionProxy()};
  if (connection_proxy != nullptr) {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    // VCA_IPCB_PROXYMETHOD_CONNECTION_PROXY_VALID
    TransmitMessageHandlerInterface& transmit_message_handler{connection_proxy->GetTransmitMessageHandler()};
    // VCA_IPCB_PROXYMETHOD_CONNECTION_PROXY_TRANSMIT_MESSAGE_HANDLER_VALID
    transmit_message_handler.SendRequestNoReturn(std::move(message));
  } else {
    logger_.LogWarn(
        [&message](Logger::LogStream& s) {
          ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessageHeader const& header{
              message.GetMessageHeader()};  // COV_IpcBinding_CoverageComponentOverall_trivial_utility

          s << "Unable to access ConnectionProxy for transmission of method request (";
          LogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s, header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogSessionId(s, header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << "). The request message will be dropped.";
        },
        Logger::LogLocation{static_cast<char const*>(__func__),
                            __LINE__});  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
  }
  // PTP-E-IpcBinding-Xf-Proxy-Send-MethodRequest-FireAndForget
}

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_

// clang-format off
/*!
 * \exclusivearea
 * amsr::ipc_binding_transformation_layer::internal::methods::ProxyFireAndForgetMethodXfBackend::session_id_mutex_
 *                Protects from parallel transmission of fire & forget method requests.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal:::ProxyFireAndForgetMethodXfBackend::session_
 *           is the protected resource from an ProxyFireAndForgetMethodXfBackend object.
 * \usedin   amsr::ipc_binding_transformation_layer::internal::methods::ProxyFireAndForgetMethodXfBackend::GetNextSessionId
 * \exclude  All other methods of ProxyFireAndForgetMethodXfBackend.
 * \length   SHORT Increment of numeric session ID.
 * \endexclusivearea
 */

/*!
 * \exclusivearea amsr::ipc_binding_transformation_layer::internal::ProxyFireAndForgetMethodXfBackend::service_up_lock_
 *                Used for protecting against parallel requests and service instance state callbacks from reactor.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal::ProxyFireAndForgetMethodXfBackend::service_state_,
 *
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyFireAndForgetMethodXfBackend::SetServiceState
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyFireAndForgetMethodXfBackend::HandleRequest
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length SHORT Only for read and write operation of numeric service state.
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
