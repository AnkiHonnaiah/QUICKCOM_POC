/*********************************************************************************************************************
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
/*!       \file
 *        \brief Base class of SkeletonMethodResponseHandlerXfBase providing non-templated functionality.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_BASE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_BASE_H_

#include <memory>

#include "amsr/core/error_code.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief Method response handler at skeleton side.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
class SkeletonMethodResponseHandlerXfBase {
 public:
  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief The alias for RequestMessageHeader.
   */
  using RequestMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessageHeader;

  /*!
   * \brief Type-alias for TraceMethodSinkInterface.
   */
  using TraceMethodSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface;

  /*! Alias to the weak ptr of a ConnectionSkeleton */
  using ConnectionSkeletonWeakPtr =
      amsr::WeakPtr<amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface>;

  /*! Type alias for ConnectionSkeletonPtr. */
  using ConnectionSkeletonPtr = ipc_binding_core::internal::ConnectionSkeletonPtr;

  /*!
   * \brief Constructor
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] request_header                Message header of the corresponding request message.
   * \param[in] connection_ptr                Weak pointer to the connection skeleton where this message should be
   *                                          routed.
   * \param[in] trace_sink                    Reference to the trace sink.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  SkeletonMethodResponseHandlerXfBase(ProvidedServiceInstanceId provided_service_instance_id,
                                      RequestMessageHeader const& request_header,
                                      ConnectionSkeletonWeakPtr connection_ptr,
                                      TraceMethodSinkInterface& trace_sink) noexcept;

  SkeletonMethodResponseHandlerXfBase(SkeletonMethodResponseHandlerXfBase&&) noexcept = delete;
  auto operator=(SkeletonMethodResponseHandlerXfBase&&) noexcept -> SkeletonMethodResponseHandlerXfBase& = delete;
  SkeletonMethodResponseHandlerXfBase(SkeletonMethodResponseHandlerXfBase const&) noexcept = delete;
  auto operator=(SkeletonMethodResponseHandlerXfBase const&) noexcept -> SkeletonMethodResponseHandlerXfBase& = delete;

  /*!
   * \brief   Create and send an ApplicationError message.
   * \details In case of ApApplicationErrors provided which are not configured, the application will abort when the
   *          forwarded error is received on client application.
   *
   * \param[in] error_code  The ApApplicationError error code returned by the skeleton implementation. Only configured
   *                        ApApplicationErrors must be provided.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Build the ApplicationErrorMessage.
   * - Send the application error response message for the corresponding method request.
   * \endinternal
   */
  void HandleApplicationErrorBase(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Serialize and transmit an error response.
   *
   * \param[in] error_code  The error code returned due to failure in method request processing.
   *                        ara::com::ComErrc::kMethodArgumentsNotRetrieved (deserialization error) or
   *                        ara::com::ComErrc::kErrorNotOk (service was stopped or method scheduling error).
   *
   * \pre         -
   * \context     App (service not available/DeserializationError) | Reactor (scheduling error/service not available).
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Serialize and transmit a error response if a deserialization error occurred.
   * - Serialize and transmit a error response if method scheduling fails or service is not available.
   * - Abort in case of an unexpected error is received.
   * \endinternal
   */
  void HandleErrorResponseBase(::amsr::core::ErrorCode const& error_code) noexcept;

 protected:
  /*!
   * \brief Type alias for ResponseMessage.
   */
  using ResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage;

  /*!
   * \brief Destructor of SkeletonMethodResponseHandlerXfBase.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VCA_IPCB_MOLE_1298
  ~SkeletonMethodResponseHandlerXfBase() noexcept = default;

  /*!
   * \brief Send the serialized response message to the related connection.
   *
   * \param[in] response_msg  The serialized response message.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SendResponse(ResponseMessage&& response_msg) const noexcept;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for IpcProtocol return code.
   */
  using ReturnCode = ::amsr::ipc_binding_core::internal::ipc_protocol::ReturnCode;

  /*!
   * \brief Build and send a error response message to the corresponding connection.
   *
   * \param[in] return_code  Return Code.
   *
   * \pre
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void SendErrorResponse(ReturnCode return_code) const noexcept;

  /*!
   * \brief Logger of the method.
   */
  Logger logger_;

  /*!
   * \brief The provided service instance ID.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief Message header of the corresponding request message.
   */
  RequestMessageHeader const request_header_;

  /*!
   * \brief Weak pointer to the connection skeleton where this message should be routed.
   */
  ConnectionSkeletonWeakPtr const connection_ptr_;

  /*!
   * \brief Sink for tracing of methods.
   */
  TraceMethodSinkInterface& trace_sink_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_BASE_H_
