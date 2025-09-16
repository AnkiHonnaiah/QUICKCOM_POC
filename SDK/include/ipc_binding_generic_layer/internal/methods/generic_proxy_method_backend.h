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
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/generic_data_type.h"
#include "ara/core/future.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/methods/proxy_request_handler.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_method_backend_interface.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_request_serializer.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Generic Proxy Method backend.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyMethod
 */
class AMSR_LG_BETA GenericProxyMethodBackend final : public GenericProxyMethodBackendInterface {
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
   * \brief Type-alias for ClientId.
   */
  using ProxyRouterInterface = ipc_binding_core::internal::ProxyRouterInterface;

  /*!
   * \brief Type-alias for SessionId.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type-alias for SessionId.
   */
  using ResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage;

  /*!
   * \brief Type-alias for SessionId.
   */
  using ErrorResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage;

  /*!
   * \brief Type-alias for SessionId.
   */
  using ApplicationErrorMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage;

  /*!
   * \brief Constructor of ProxyMethodBackend.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] method_id                     Method deployment ID.
   * \param[in] client_id                     The client ID.
   * \param[in] trace_sink                    Trace sink. Must not be nullptr.
   * \param[in] proxy_router                  Reference to the ProxyRouter.
   * \param[in] max_limit_session             Maximum value of session ID. After reaching this value, the session ID
   *                                          must be reset. If the value is not set explicitly then the wrap around of
   *                                          session will happen when usual max limit of the SessionId::type is
   *                                          reached.
   *
   * \pre        Proxy router pointer must be valid for the whole lifetime of the ProxyMethodBackend.
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  GenericProxyMethodBackend(ProvidedServiceInstanceId provided_service_instance_id, MethodId method_id,
                            ClientId client_id,
                            std::unique_ptr<TraceMethodSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
                            ProxyRouterInterface& proxy_router, SessionId::type max_limit_session = 0) noexcept;

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
  ~GenericProxyMethodBackend() noexcept final = default;

  GenericProxyMethodBackend(GenericProxyMethodBackend const&) = delete;
  GenericProxyMethodBackend(GenericProxyMethodBackend&&) = delete;
  auto operator=(GenericProxyMethodBackend const&) -> GenericProxyMethodBackend& = delete;
  auto operator=(GenericProxyMethodBackend&&) -> GenericProxyMethodBackend& = delete;

  /*!
   * \brief Carry out the method request.
   *
   * \param[in] args  Serialized method arguments.
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
  auto HandleRequest(socal::GenericDataType const& args) noexcept -> core::Future<socal::GenericDataPtr> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::SetConnectionState
   *
   * \spec requires true; \endspec
   */
  void SetConnectionState(bool connection_state) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::SetServiceState
   *
   * \spec requires true; \endspec
   */
  void SetServiceState(bool service_state) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::OnResponseReceived
   *
   * \spec requires true; \endspec
   */
  void OnResponseReceived(ResponseMessage&& response) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::OnApplicationErrorReceived
   *
   * \spec requires true; \endspec
   */
  void OnApplicationErrorReceived(ApplicationErrorMessage&& message) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::ProxyMethodBackendInterface::OnErrorResponseReceived
   *
   * \spec requires true; \endspec
   */
  void OnErrorResponseReceived(ErrorResponseMessage&& message) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_generic_layer::internal::methods::GenericProxyMethodBackendInterface::GetMethodId
   *
   * \spec requires true; \endspec
   */
  auto GetMethodId() const noexcept -> MethodId final;

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
   * \brief Type alias for the used PendingRequestMap.
   */
  using PendingRequestMap = ipc_binding_core::internal::methods::PendingRequestMap<socal::GenericDataPtr>;

  /*!
   * \brief Type alias for the used ProxyRequestHandler.
   */
  using ProxyRequestHandler =
      ipc_binding_core::internal::methods::ProxyRequestHandler<socal::GenericDataPtr, GenericProxyRequestSerializer,
                                                               socal::GenericDataType>;
  /*!
   * \brief Cancel all pending method request promises with kServiceNotAvailable error.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     TRUE
   */
  void CancelAllPendingRequests() noexcept;

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
   * \brief Pointer to the ProxyRouter.
   */
  ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Proxy request handler.
   */
  ProxyRequestHandler request_handler_;

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

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_H_
