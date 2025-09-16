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
 *        \brief Method response handler at skeleton side.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_H_

#include <memory>
#include <utility>

#include "amsr/core/string_view.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/methods/skeleton_response_handler_interface.h"
#include "someip-protocol/internal/serialization/writer.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ser_ipc_headers.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_method_response_handler_xf_base.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief   Method response handler at skeleton side.
 * \details The class contains response handler functions which serialize the results of a method request and route the
 *          results to proxy.
 *
 * \tparam Output              The output type of method request.
 * \tparam ResponseSerializer  The serializer type for the response.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
template <typename Output, typename ResponseSerializer>
class SkeletonMethodResponseHandlerXf final
    : public ::amsr::socal::internal::methods::SkeletonResponseHandlerInterface<Output>,
      public SkeletonMethodResponseHandlerXfBase {
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
   * \brief The alias for RequestMessageHeader.
   */
  using RequestMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessageHeader;

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
   */
  SkeletonMethodResponseHandlerXf(ProvidedServiceInstanceId const provided_service_instance_id,
                                  RequestMessageHeader const& request_header, ConnectionSkeletonWeakPtr connection_ptr,
                                  TraceMethodSinkInterface& trace_sink) noexcept
      : ::amsr::socal::internal::methods::SkeletonResponseHandlerInterface<Output>(),
        // VCA_IPCB_STD_WEAK_POINTER_DESTRUCTOR
        SkeletonMethodResponseHandlerXfBase(provided_service_instance_id, request_header, connection_ptr, trace_sink),
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"SkeletonMethodResponseHandlerXf"}},
        provided_service_instance_id_{provided_service_instance_id},
        request_header_{request_header} {}

  SkeletonMethodResponseHandlerXf(SkeletonMethodResponseHandlerXf&&) noexcept = delete;
  auto operator=(SkeletonMethodResponseHandlerXf&&) noexcept -> SkeletonMethodResponseHandlerXf& = delete;
  SkeletonMethodResponseHandlerXf(SkeletonMethodResponseHandlerXf const&) noexcept = delete;
  auto operator=(SkeletonMethodResponseHandlerXf const&) noexcept -> SkeletonMethodResponseHandlerXf& = delete;

  /*!
   * \brief Destructor of SkeletonMethodResponseHandlerXf.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VCA_IPCB_MOLE_1298
  ~SkeletonMethodResponseHandlerXf() noexcept final = default;

  /*!
   * \brief Create and route a serialized method response for a processed method request.
   *
   * \param[in] response  The method result received.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Prepare the serialized response message from the given header and payload.
   * - Serialize the Header.
   * - Serialize the payload.
   * - Assemble the response message and route response to corresponding method request.
   * \endinternal
   */
  void HandleMethodResponse(Output const&& response) noexcept final {
    // PTP-B-IpcBinding-Xf-Skeleton-Serialize-MethodResponse

    logger_.LogDebug(
        [this](::ara::log::LogStream& s) {
          s << "Response received for method request: ";
          LogBuilder::LogCompleteServiceInstanceMethodId(s, request_header_.service_id_, request_header_.major_version_,
                                                         request_header_.instance_id_, request_header_.method_id_);
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;
    namespace ipc_marshalling = ipc_protocol::serialization;
    namespace someip_marshalling = ::amsr::someip_protocol::internal::serialization;

    // Calculate the required size for the response data
    std::size_t const payload_size{ResponseSerializer::GetRequiredBufferSize(response)};
    // Allocate memory for packet serialization
    std::size_t const alloc_size{ipc_protocol::kProtocolMessageHeaderLength +
                                 ipc_protocol::kResponseMessageHeaderLength + payload_size};

    // Creation of a buffer to serialize the generic protocol header, the request header and the payload.
    ipc_protocol::IpcPacketShared packet{ipc_protocol::IpcPacketSharedConstruct(alloc_size)};
    someip_marshalling::Writer writer{someip_marshalling::BufferView{*packet}};

    // Serialize IPC response message header
    ipc_marshalling::SerializeResponseMessageHeader(writer, request_header_, payload_size);
    // Serialize method response
    ResponseSerializer::Serialize(writer, response);

    // PTP-E-IpcBinding-Xf-Skeleton-Serialize-MethodResponse

    // Assemble the complete response message for transmission.
    ipc_protocol::ResponseMessage response_msg{request_header_, std::move(packet)};

    SendResponse(std::move(response_msg));
  }

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
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Build the ApplicationErrorMessage.
   * - Send the application error response message for the corresponding method request.
   * \endinternal
   */
  void HandleApplicationError(::amsr::core::ErrorCode const& error_code) noexcept final {
    SkeletonMethodResponseHandlerXfBase::HandleApplicationErrorBase(error_code);
  };

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
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Serialize and transmit a error response if a deserialization error occurred.
   * - Serialize and transmit a error response if method scheduling fails or service is not available.
   * - Abort in case of an unexpected error is received.
   * \endinternal
   */
  void HandleErrorResponse(::amsr::core::ErrorCode const& error_code) noexcept final {
    SkeletonMethodResponseHandlerXfBase::HandleErrorResponseBase(error_code);
  }

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
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_RESPONSE_HANDLER_XF_H_
