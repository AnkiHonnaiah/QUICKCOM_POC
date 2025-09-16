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
/**       \file  someip_binding_xf/internal/methods/skeleton_method_xf.h
 *        \brief  Method manager to handle common logic for method requests/responses on skeleton side.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonMethodXf
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/socal/internal/methods/skeleton_method_backend_interface.h"
#include "amsr/socal/internal/methods/skeleton_method_interface.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_method_request_handler.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_method_request_xf.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_response_handler.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_deserializer_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_serializer_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/someip_message.h"

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Method manager to handle skeleton method request/response.
 *
 * \tparam Output    The output struct type.
 * \tparam InputArgs A variadic list of data types that correspond to the types of the elements of InputType.
 */
template <typename Output, typename... InputArgs>
class SkeletonMethodXf final
    : public ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestHandlerInterface,
      public ::amsr::socal::internal::methods::SkeletonMethodBackendInterface<Output, InputArgs...> {
 public:
  /*!
   * \brief Deserializer pointer which is used to create an InputType object from a provided Packet
   */
  using DeserializerUniquePtr =
      std::unique_ptr<::amsr::someip_binding_xf::internal::ser_deser::MethodRequestDeserializerInterface<InputArgs...>>;

  /*!
   * \brief Application Error.
   */
  using ApApplicationError = ::amsr::someip_protocol::internal::serialization::ApApplicationError;

  /*!
   * \brief Serializer interface pointer with ResponseDataType.
   */
  using ResponseSerializerUniquePtr =
      std::unique_ptr<::amsr::someip_binding_xf::internal::ser_deser::MethodResponseSerializerInterface<Output>>;

  /*!
   * \brief Serializer interface pointer with application error.
   */
  using ApplicationErrorSerializerUniquePtr = std::unique_ptr<
      ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseSerializerInterface<ApApplicationError>>;

  /*!
   * \brief An alias to the map of errordomain.
   */
  using ApplicationErrorMap =
      std::map<amsr::core::ErrorDomain::IdType, ::amsr::someip_binding_xf::internal::methods::ApplicationErrorDomain>;

  /*!
   * \brief Type alias for the SkeletonMethodRequestXf.
   */
  using SkeletonMethodRequestXfType = SkeletonMethodRequestXf<InputArgs...>;

  /*!
   * \brief Type alias for the SkeletonResponseHandler.
   */
  using SkeletonResponseHandlerType = SkeletonResponseHandler<Output>;

  /*!
   * \brief Type alias for SkeletonMethodBackendInterface.
   */
  using SkeletonMethodBackendInterface =
      ::amsr::socal::internal::methods::SkeletonMethodBackendInterface<Output, InputArgs...>;

  SkeletonMethodXf() = delete;
  SkeletonMethodXf(SkeletonMethodXf const&) = delete;
  SkeletonMethodXf(SkeletonMethodXf&&) = delete;
  SkeletonMethodXf& operator=(SkeletonMethodXf const&) & = delete;
  SkeletonMethodXf& operator=(SkeletonMethodXf&&) & = delete;

  /*!
   * \brief Destructor.
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Cancel the registration for this method ID.
   * \endinternal
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SkeletonMethodXf() noexcept final { server_.UnregisterMethodRequestHandler(method_id_); };

  /*!
   * \brief Constructor.
   * \param[in] deserializer          The Deserializer which deserializes the method arguments. Cannot be nullptr.
   * \param[in] instance_id           Instance ID.
   * \param[in] server                Reference to the server where this handler is being registered such that
   *                                  the server can use this object to call HandleMethodRequest later.
   * \param[in] serializer            Serializer for the method response payload. Cannot be nullptr.
   * \param[in] app_error_serializer  Serializer for application error response. Cannot be nullptr.
   * \param[in] allocator             Allocator for memory used to serialize/send positive response or application.
   *                                  Cannot be nullptr.
   * \param[in] application_error_map Application errors referenced by this method.
   * \param[in] id                    Id of this method.
   *
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \spec
   *   requires deserializer != nullptr;
   *   requires serializer != nullptr;
   *   requires allocator != nullptr;
   *   requires app_error_serializer != nullptr;
   * \endspec
   *
   * \internal
   * - Register this method ID at the server.
   * \endinternal
   */
  SkeletonMethodXf(DeserializerUniquePtr deserializer, ::amsr::someip_protocol::internal::InstanceId const instance_id,
                   ::amsr::someip_binding_core::internal::LocalServerInterface& server,
                   ResponseSerializerUniquePtr serializer, ApplicationErrorSerializerUniquePtr app_error_serializer,
                   std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> allocator,
                   ApplicationErrorMap const& application_error_map,
                   ::amsr::someip_protocol::internal::MethodId const id)
      : SkeletonMethodRequestHandlerInterface{},
        SkeletonMethodBackendInterface{},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        deserializer_{std::move(deserializer)},
        instance_id_{instance_id},
        server_{server},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        response_serializer_{std::move(serializer)},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        app_error_serializer_{std::move(app_error_serializer)},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        allocator_{std::move(allocator)},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        application_error_map_{application_error_map},
        method_id_{id} {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    server_.RegisterMethodRequestHandler(method_id_, request_handler_);
  }

  /*!
   * \brief         Forward method request to SOCAL.
   *
   * \param[in,out] request  Incoming method request to be processed.
   * \return Always return positive result.
   *
   * \pre           RegisterSkeletonMethodInterface has been called.
   *
   * \context       Reactor
   *
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   *
   * \internal
   * - Verify that a method interface for forwarding the method requests is registered.
   *  - If not, log fatal and abort, since this can be considered a violation.
   * - Instantiate a ResponseHandler to process the responses received for this method request later.
   * - Instantiate a Request, if both ResponseHandler and Request instantiation are successful:
   *   - Forward the request to SOCAL SkeletonMethod for further processing.
   * \endinternal
   */
  ::amsr::core::Result<void> HandleMethodRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept final {
    if (skeleton_method_ != nullptr) {
      someip_protocol::internal::SomeIpMessageHeader const& someip_message_header{request.GetSomeIpHeader()};
      logger_.LogVerbose(
          [&someip_message_header](::ara::log::LogStream& s) {
            s << "Method request received for further processing.";
            ::amsr::someip_binding_core::internal::logging::LogBuilder::LogSomeIpHeader(s, someip_message_header);
          },
          static_cast<char const*>(__func__), __LINE__);
      ara::core::PolymorphicAllocator<SkeletonMethodRequestXfType> const request_allocator{};
      ara::core::PolymorphicAllocator<SkeletonResponseHandlerType> const response_allocator{};

      // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      amsr::UniquePtr<SkeletonMethodRequestXfType> method_request_unique_ptr{
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
          amsr::AllocateUnique<SkeletonMethodRequestXfType>(request_allocator, std::move(request.ReleaseBuffer()),
                                                            *deserializer_)
              .Value()};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
      amsr::UniquePtr<SkeletonResponseHandlerType> response_handler_unique_ptr{
          amsr::AllocateUnique<SkeletonResponseHandlerType>(  // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
              response_allocator, instance_id_, someip_message_header, server_, *response_serializer_,
              *app_error_serializer_, *allocator_,
              // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
              application_error_map_, request.GetRequesterAddress())
              .Value()};

      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      skeleton_method_->HandleRequest(std::move(method_request_unique_ptr), std::move(response_handler_unique_ptr));

    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Method request received but either no forward method has been registered or it has already been "
                 "deregistered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Always return positive result, no other check than skeleton method
    return amsr::core::Result<void>{};
  }

 public:
  /*!
   * \brief       Registers a SkeletonMethodInterface to its respective backend.
   *
   * \details     The skeleton method frontend interface is received as a reference and
   *              its address is stored locally as a raw pointer.
   *              The provided reference must remain valid until DeregisterSkeletonMethodInterface
   *              is called.
   *
   * \param[in]   skeleton_method A reference to the skeleton method frontend interface.
   *
   * \pre         A skeleton method frontend interface shall not have been previously registered.
   *              The related service must not have been offered yet.
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances
   * \reentrant   FALSE for same class instance, FALSE for different instances
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterSkeletonMethodInterface(
      ::amsr::socal::internal::methods::SkeletonMethodInterface<Output, InputArgs...>& skeleton_method) noexcept final {
    if (skeleton_method_ != nullptr) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Attempt to re-register skeleton method frontend interface without de-registering "
                 "first.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    skeleton_method_ = &skeleton_method;
  }

  /*!
   * \brief       De-registers a skeleton method from the method backend.
   *
   * \details     After this call, the registered reference to the skeleton method is considered to be invalid.
   *
   * \pre         Deregister can only be called after the offering of the related service
   *              has been completely stopped.
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances
   *              Any calls to the registered skeleton method must be synchronized against this function.
   * \reentrant   FALSE for same class instance, FALSE for different instances
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeregisterSkeletonMethodInterface() noexcept final { skeleton_method_ = nullptr; }

 private:
  /*!
   * \brief Method request handler to be registered in the server interface.
   *        SomeIpBindingCore (LocalServer) will hold a weak pointer to this object,
   *        in order to forward incoming method requests.
   *        It will be registered into SomeIpBindingCore within Ctor of this class, and
   *        deregistered during destruction.
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  std::shared_ptr<SkeletonMethodRequestHandler> request_handler_{std::make_shared<SkeletonMethodRequestHandler>(*this)};

  /*!
   * \brief Deserializer pointer.
   */
  DeserializerUniquePtr deserializer_;

  /*!
   * \brief Instance ID.
   */
  ::amsr::someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief Server interface. The referenced object is constructed during initialization phase and destructed
   * during shutdown, therefore it is guaranteed that it outlives this object which is constructed and destructed during
   * runtime.
   */
  ::amsr::someip_binding_core::internal::LocalServerInterface& server_;

  /*!
   * \brief Response serializer pointer.
   */
  ResponseSerializerUniquePtr response_serializer_;

  /*!
   * \brief Application error serializer pointer.
   */
  ApplicationErrorSerializerUniquePtr app_error_serializer_;

  /*!
   * \brief Used memory buffer allocator.
   */
  std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> allocator_;

  /*!
   * \brief Application error map.
   */
  ApplicationErrorMap const application_error_map_;

  /*!
   * \brief SOCAL method interface for forwarding the method requests.
   */
  ::amsr::socal::internal::methods::SkeletonMethodInterface<Output, InputArgs...>* skeleton_method_{nullptr};

  /*!
   * \brief Id of the associated method.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief Method logger for printing debug and error messages.
   * \details The concrete method will set the prefix for identification.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription,
      ::amsr::someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix("SkeletonMethodXf"_sv,
                                                                                             method_id_)};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_XF_H_
