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
 *        \brief Trace sink interface for tracing methods and field methods.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_INTERFACE_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief   Trace sink interface for tracing methods.
 * \details Supported trace points:
 *          - Send/Receive MethodRequest.
 *          - Send/Receive MethodRequestNoReturn.
 *          - Send/Receive MethodResponse.
 *          - Send/Receive ApplicationError.
 *
 * \unit IpcBinding::IpcBindingCore::TraceMethodSink
 */
class TraceMethodSinkInterface {
 public:
  /*!
   * \brief Type-alias for RequestMessage.
   */
  using RequestMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessage;

  /*!
   * \brief Type-alias for RequestNoReturnMessage.
   */
  using RequestNoReturnMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage;

  /*!
   * \brief Type-alias for RemoteRequestMessage.
   */
  using RemoteRequestMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::RemoteRequestMessage;

  /*!
   * \brief Type-alias for ResponseMessage.
   */
  using ResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage;

  /*!
   * \brief Type-alias for ErrorCode.
   */
  using ErrorCode = ::ara::core::ErrorCode;

  /*!
   * \brief Type-alias for ApplicationErrorMessage.
   */
  using ApplicationErrorMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage;

  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for ClientId.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  TraceMethodSinkInterface(TraceMethodSinkInterface const&) noexcept = delete;
  TraceMethodSinkInterface(TraceMethodSinkInterface&&) noexcept = delete;
  auto operator=(TraceMethodSinkInterface const&) noexcept -> TraceMethodSinkInterface& = delete;
  auto operator=(TraceMethodSinkInterface&&) noexcept -> TraceMethodSinkInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~TraceMethodSinkInterface() noexcept = default;

  /*!
   * \brief Trace sending a method request in a proxy.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method request.
   * \param[in] client_id                     The client id of the proxy sending the method request.
   * \param[in] request_message               The request message that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceRequestSend(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                                RequestMessage request_message) const noexcept -> void = 0;

  /*!
   * \brief Trace sending a fire-and-forget method request in a proxy.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method request.
   * \param[in] client_id                     The client id of the proxy sending the method request.
   * \param[in] request_message               The request message that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceRequestNoReturnSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                                        ClientId client_id, RequestNoReturnMessage request_message) const noexcept
      -> void = 0;

  /*!
   * \brief Trace receiving a method request in a skeleton.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method request.
   * \param[in] request_message               The request message that was received.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceRequestReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                                    RemoteRequestMessage request_message) const noexcept -> void = 0;

  /*!
   * \brief Trace receiving a fire-and-forget method request in a skeleton.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method request.
   * \param[in] request_message               The request message that was received.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceRequestNoReturnReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                                            RequestNoReturnMessage request_message) const noexcept -> void = 0;

  /*!
   * \brief Trace sending a method response in a skeleton.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method response.
   * \param[in] response_message              The response message that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceResponseSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                                 ResponseMessage response_message) const noexcept -> void = 0;

  /*!
   * \brief Trace sending an application error response in a skeleton.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method response.
   * \param[in] response_message              The application error message that was sent.
   * \param[in] error_code                    The application error code.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceApplicationErrorResponseSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                                                 ApplicationErrorMessage response_message,
                                                 ErrorCode const& error_code) const noexcept -> void = 0;

  /*!
   * \brief Trace receiving a method response in a proxy.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method response.
   * \param[in] client_id                     The client id of the proxy receiving the method response.
   * \param[in] response_message              The response message that was received.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceResponseReceived(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                                     ResponseMessage response_message) const noexcept -> void = 0;

  /*!
   * \brief Trace receiving a application error response in a proxy.
   *
   * \param[in] provided_service_instance_id  The provided service instance of the method response.
   * \param[in] client_id                     The client id of the proxy receiving the method response.
   * \param[in] response_message              The application error message that was received.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceApplicationErrorResponseReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                                                     ClientId client_id,
                                                     ApplicationErrorMessage response_message) const noexcept
      -> void = 0;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceMethodSinkInterface() noexcept = default;
};
}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_INTERFACE_H_
