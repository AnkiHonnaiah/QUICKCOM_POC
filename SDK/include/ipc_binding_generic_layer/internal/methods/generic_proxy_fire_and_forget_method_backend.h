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
/*!        \file
 *        \brief  Generic proxy method backend class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/session_handler.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_fire_and_forget_method_backend_interface.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Generic Proxy Fire and Forget Method backend.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyMethod
 */
class AMSR_LG_BETA GenericProxyFireAndForgetMethodBackend final
    : public GenericProxyFireAndForgetMethodBackendInterface {
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
   * \brief Type-alias for SessionId.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type-alias for RequestNoReturnMessage.
   */
  using RequestNoReturnMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage;

  /*!
   * \brief Constructor of ProxyFireAndForgetMethodBackend.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] method_id                     Method deployment ID.
   * \param[in] client_id                     The client ID.
   * \param[in] trace_sink                    Trace sink. Must not be nullptr.
   * \param[in] proxy_router                  Reference to the ProxyRouter.
   *
   * \pre     Proxy router pointer must be valid for the whole lifetime of the ProxyMethodBackend.
   * \context App
   * \steady  FALSE
   */
  GenericProxyFireAndForgetMethodBackend(
      ProvidedServiceInstanceId provided_service_instance_id, MethodId method_id, ClientId client_id,
      std::unique_ptr<TraceMethodSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
      ProxyRouterInterface& proxy_router) noexcept;

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
  ~GenericProxyFireAndForgetMethodBackend() noexcept final = default;

  GenericProxyFireAndForgetMethodBackend(GenericProxyFireAndForgetMethodBackend const&) = delete;
  GenericProxyFireAndForgetMethodBackend(GenericProxyFireAndForgetMethodBackend&&) = delete;
  auto operator=(GenericProxyFireAndForgetMethodBackend const&) -> GenericProxyFireAndForgetMethodBackend& = delete;
  auto operator=(GenericProxyFireAndForgetMethodBackend&&) -> GenericProxyFireAndForgetMethodBackend& = delete;

  /*!
   * \brief Carry out the method request.
   *
   * \param[in] args  Serialized method arguments.
   *
   * \return A result indicating the success of the operation.
   *
   * \error Method-specific  Errors will be returned in the result.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto HandleRequest(socal::GenericDataType const& args) noexcept -> core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyFireAndForgetMethodBackendInterface::SetServiceState
   *
   * \spec requires true; \endspec
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
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::ipc_binding_core::internal::SessionHandler;

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
   * \brief Pointer to the ProxyRouter.
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
   * \brief Mutex for protection against parallel requests.
   */
  std::mutex service_up_lock_{};

  /*!
   * \brief Holds the currently known service instance state.
   */
  bool service_state_{true};
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_H_
