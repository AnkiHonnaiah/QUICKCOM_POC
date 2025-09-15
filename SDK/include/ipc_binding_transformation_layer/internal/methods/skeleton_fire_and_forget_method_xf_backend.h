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
 *        \brief  Fire and forget methodxf provides logic to handle method requests on skeleton side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/string_view.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_backend_interface.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "ara/core/string.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_fire_and_forget_method_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_method_request_xf.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief Handles method request on skeleton side.
 *
 * \tparam RequestDeserializer  The deserializer type for the input arguments.
 * \tparam InputArgs            The types of method arguments.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
template <typename RequestDeserializer, typename... InputArgs>
class SkeletonFireAndForgetMethodXfBackend final
    : public SkeletonFireAndForgetMethodXfBackendInterface,
      public ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodBackendInterface<InputArgs...> {
 public:
  /*!
   * \brief Type alias for templated SkeletonFireAndForgetMethodInterfaceType.
   */
  using SkeletonFireAndForgetMethodInterfaceType =
      ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodInterface<InputArgs...>;

  /*!
   * \brief Type alias for templated SkeletonFireAndForgetMethodBackendInterface.
   */
  using SkeletonFireAndForgetMethodBackendInterface =
      ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodBackendInterface<InputArgs...>;

  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for TraceMethodSinkInterface.
   */
  using TraceMethodSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface;

  /*!
   * \brief Constructor of SkeletonFireAndForgetMethodXfBackend.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] trace_sink                    Trace sink.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit SkeletonFireAndForgetMethodXfBackend(
      ProvidedServiceInstanceId const provided_service_instance_id,
      std::unique_ptr<TraceMethodSinkInterface> trace_sink) noexcept  // IGNORE_BLACKLIST BSOCIAL-8805
      : SkeletonFireAndForgetMethodXfBackendInterface{},
        SkeletonFireAndForgetMethodBackendInterface{},
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"SkeletonFireAndForgetMethodXfBackend"}},
        provided_service_instance_id_{provided_service_instance_id},
        trace_sink_{std::move(trace_sink)}  // VCA_IPCB_VALID_TRACE_SINK
  {
    if (!trace_sink_) {
      logger_.TerminateOnViolation(
          "Tried to construct a SkeletonFireAndForgetMethodXfBackend with an invalid (empty) trace sink.",
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }

  SkeletonFireAndForgetMethodXfBackend(SkeletonFireAndForgetMethodXfBackend&&) noexcept = delete;
  auto operator=(SkeletonFireAndForgetMethodXfBackend&&) noexcept -> SkeletonFireAndForgetMethodXfBackend& = delete;
  SkeletonFireAndForgetMethodXfBackend(SkeletonFireAndForgetMethodXfBackend const&) noexcept = delete;
  auto operator=(SkeletonFireAndForgetMethodXfBackend const&) noexcept
      -> SkeletonFireAndForgetMethodXfBackend& = delete;

  /*!
   * \brief Destructor of SkeletonMethodXfBackend.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~SkeletonFireAndForgetMethodXfBackend() noexcept final = default;  // VCA_IPCB_MOLE_1298

  /*!
   * \brief   Forwards the fire and forget method request to the request handler.
   * \details HandleMethodRequest can only be called if the route to LocalServerSink has been created in OfferService
   *          Call. And it cannot be called any further if the route to LocalServerSink has been deleted in
   *          StopOfferService Call. And since StopOfferService would be blocked in app context until it can be
   *          performed via reactor sync task its not possible that socal_method_ gets dangling in HandleMethodRequest
   *          function.
   *
   * \param[in,out] request  Incoming method request to be deserialized and processed.
   *
   * \pre         RegisterSkeletonFireAndForgetMethodInterface should have been called before.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Verify that a valid socal skeleton method is available to forward the method request.
   *   - Instantiate a Request, and if successful:
   *     - Forward the request to SOCAL SkeletonFireAndForgetMethod for further processing.
   *   - Otherwise, log fatal and abort.
   * - Log an error and abort if valid socal skeleton method is not available.
   * \endinternal
   */
  void HandleMethodRequest(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage&& request) noexcept final {
    if (socal_method_ != nullptr) {
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10-A18.5.8_Large_packets_allocated_on_stack
      ::ara::core::Result<amsr::UniquePtr<SkeletonMethodRequest>> method_request_result{
          amsr::tmp::MakeUnique<SkeletonMethodRequest>(request.GetPacket())};

      LogHandleMethodRequest(request);

      // VCA_IPCB_VALID_TRACE_SINK
      trace_sink_->TraceRequestNoReturnReceived(provided_service_instance_id_, request);

      if (!method_request_result.HasValue()) {
        ::amsr::core::ErrorCode const error{method_request_result.Error()};

        logger_.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
            "Failed to allocate memory for method request: ",
            [&error](::ara::log::LogStream& s) { s << error.Message() << "."; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      } else {
        // VCA_IPCB_VALID_POINTER_AFTER_CHECK
        socal_method_->HandleRequest(std::move(method_request_result.Value()));
      }
    } else {
      LogHandleMethodRequest(request);

      logger_.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
          "Method request cannot be processed.",
          [&request](::ara::log::LogStream& s) {
            s << "No valid Skeleton method is registered, method request cannot be processed.";
            RequestNoReturnMessageHeader const& header{
                request.GetMessageHeader()};  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder::LogCompleteServiceInstanceMethodId(
                s, header.service_id_, header.major_version_, header.instance_id_,
                header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }

  /*!
   * \brief Registers a skeleton fire-and-forget method for method communication.
   *
   * \param[in] skeleton_fnf_method  A reference to the skeleton fire and forget method interface. The reference should
   *                                 be valid until DeregisterSkeletonMethodInterface is called.
   *
   * \pre         A skeleton method interface must not have been previously registered.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - In case of Re-registration.
   *   - Abort.
   * - Register skeleton method.
   * \endinternal
   */
  void RegisterSkeletonFireAndForgetMethodInterface(
      SkeletonFireAndForgetMethodInterfaceType& skeleton_fnf_method) noexcept final {
    if (socal_method_ != nullptr) {
      logger_.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
          "Attempt to re-register skeleton method interface without de-registering first.",
          [](ara::log::LogStream& s) {
            s << "Attempt to re-register skeleton method interface without de-registering first.";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
    socal_method_ = &skeleton_fnf_method;
  }

  /*!
   * \brief   De-registers a skeleton fire-and-forget method.
   * \details After this call, the registered skeleton fire-and-forget method is considered to be invalid.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances Any calls to the registered socal method
   *              object must be synchronized against this function.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - DeRegister skeleton method.
   * \endinternal
   */
  void DeregisterSkeletonFireAndForgetMethodInterface() noexcept final { socal_method_ = nullptr; }

 private:
  /*!
   * \brief Log call to HandleMethodRequest.
   *
   * \param[in] request  The request argument of HandleMethodRequest.
   *
   * \pre    -
   * \steady TRUE
   */
  void LogHandleMethodRequest(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage& request) const noexcept {
    logger_.LogVerbose(
        [&request](::ara::log::LogStream& s) {
          s << "Fire and Forget method request received for further processing.";
          RequestNoReturnMessageHeader const& header{
              request.GetMessageHeader()};  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder::LogCompleteServiceInstanceMethodId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.method_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
  }

  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief The alias for RequestMessageHeader.
   */
  using RequestNoReturnMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessageHeader;

  /*!
   * \brief Type alias for method request.
   */
  using SkeletonMethodRequest = SkeletonMethodRequestXf<RequestDeserializer, InputArgs...>;

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
   * \brief SOCAL method interface for forwarding the method requests.
   */
  ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodInterface<InputArgs...>* socal_method_{nullptr};

  /*!
   * \brief Sink for tracing of methods.
   */
  std::unique_ptr<TraceMethodSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
//
// \ID COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
//   \ACCEPT  XX
//   \REASON  CallCoverage gap reported as logger is a separate unit.
//            As the error scenario is handled as violation no further unit interactions are possible.
//            Therefore no separate test of CT or MUT level necessary.
// COV_JUSTIFICATION_END
