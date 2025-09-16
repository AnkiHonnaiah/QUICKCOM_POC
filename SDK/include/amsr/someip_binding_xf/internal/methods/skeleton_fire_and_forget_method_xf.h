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
/**       \file  someip_binding_xf/internal/methods/skeleton_fire_and_forget_method_xf.h
 *        \brief  Handle common logic for fire and forget method requests on skeleton side.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonFireAndForgetMethodXf
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_backend_interface.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_interface.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_method_request_handler.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_method_request_no_response_handler.h"
#include "amsr/someip_binding_xf/internal/methods/skeleton_method_request_xf.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_deserializer_interface.h"
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
 * \brief Handle skeleton fire and forget method request.
 *
 * \tparam InputArgs A variadic list of data types that correspond to the types of the elements of InputType.
 */
template <typename... InputArgs>
class SkeletonFireAndForgetMethodXf final
    : public ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestNoReturnHandlerInterface,
      public ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodBackendInterface<InputArgs...> {
 public:
  /*!
   * \brief Deserializer pointer which is used to create an InputType object from a provided Packet
   */
  using DeserializerUniquePtr =
      std::unique_ptr<::amsr::someip_binding_xf::internal::ser_deser::MethodRequestDeserializerInterface<InputArgs...>>;

  /*!
   * \brief Type alias for SkeletonMethodRequestXf.
   */
  using SkeletonMethodRequestXfType = SkeletonMethodRequestXf<InputArgs...>;

  /*!
   * \brief Type alias for SkeletonFireAndForgetMethodBackendInterface.
   */
  using SkeletonFireAndForgetMethodBackendInterface =
      ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodBackendInterface<InputArgs...>;

  SkeletonFireAndForgetMethodXf() = delete;
  SkeletonFireAndForgetMethodXf(SkeletonFireAndForgetMethodXf const&) = delete;
  SkeletonFireAndForgetMethodXf(SkeletonFireAndForgetMethodXf&&) = delete;
  SkeletonFireAndForgetMethodXf& operator=(SkeletonFireAndForgetMethodXf const&) & = delete;
  SkeletonFireAndForgetMethodXf& operator=(SkeletonFireAndForgetMethodXf&&) & = delete;

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
  ~SkeletonFireAndForgetMethodXf() noexcept final { server_.UnregisterMethodRequestNoReturnHandler(method_id_); };

  /*!
   * \brief Constructor.
   * \param[in] deserializer                    The Deserializer which deserializes the method arguments. Cannot be
   *                                            nullptr.
   *
   * \param[in] server                          Reference to the server where this handler is being registered such that
   *                                            the server can use this object to call HandleMethodRequest later.
   * \param[in] id                              Id of this method.
   * \pre -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \spec
   *   requires deserializer != nullptr;
   * \endspec
   */
  SkeletonFireAndForgetMethodXf(DeserializerUniquePtr deserializer,
                                ::amsr::someip_binding_core::internal::LocalServerInterface& server,
                                ::amsr::someip_protocol::internal::MethodId const id)
      : SkeletonMethodRequestNoReturnHandlerInterface{},
        SkeletonFireAndForgetMethodBackendInterface{},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        deserializer_{std::move(deserializer)},
        server_{server},
        method_id_{id} {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    server_.RegisterMethodRequestNoReturnHandler(method_id_, request_handler_);
  }

  /*!
   * \brief         Forward method request to SOCAL.
   *
   * \param[in,out] request Incoming method request to be processed.
   * \return Always return positive result.
   *
   * \pre           RegisterSkeletonFireAndForgetMethodInterface has been called.
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   *
   * \internal
   * - Verify that a method interface for forwarding the method requests is registered.
   *  - If not, log fatal and abort, since this can be considered a violation.
   * - Instantiate a Request, and if successful:
   *   - Forward the request to SOCAL SkeletonMethod for further processing.
   * - Otherwise log fatal and abort.
   * \endinternal
   */
  ::amsr::core::Result<void> HandleMethodRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept final {
    if (skeleton_fire_and_forget_method_ != nullptr) {
      ::ara::core::PolymorphicAllocator<SkeletonMethodRequestXfType> const allocator{};
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      ::ara::core::Result<amsr::UniquePtr<SkeletonMethodRequestXfType>> method_request_result{
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
          amsr::AllocateUnique<SkeletonMethodRequestXfType>(allocator, std::move(request.ReleaseBuffer()),
                                                            *deserializer_)};
      logger_.LogVerbose(
          [&request](::ara::log::LogStream& s) {
            s << "Method request received for further processing.";
            // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
            ::amsr::someip_binding_core::internal::logging::LogBuilder::LogSomeIpHeader(s, request.GetSomeIpHeader());
          },
          static_cast<char const*>(__func__), __LINE__);

      if (not method_request_result.HasValue()) {
        ::ara::core::String error_msg{"Failed to allocate memory for method request: "};
        error_msg.append(method_request_result.Error().Message());
        logger_.LogFatalAndAbort(
            [&error_msg](::amsr::someip_binding_core::internal::logging::StringStream& s) { s << error_msg; },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        skeleton_fire_and_forget_method_->HandleRequest(std::move(method_request_result.Value()));
      }
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Method request received but either no forward method has been registered or it has already been "
                 "deregistered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Always return positive result, no other check than skeleton fire and forget method
    return amsr::core::Result<void>{};
  }

  /*!
   * \brief       Registers a skeleton fire-and-forget method to its respective backend.
   *
   * \details     The skeleton fire-and-forget method frontend interface is received as a reference and
   *              its address is stored locally as a raw pointer.
   *              The provided reference must remain valid until DeregisterSkeletonFireAndForgetMethodInterface
   *              is called.
   *
   * \param[in]   skeleton_fnf_method A reference to the skeleton fire-and-forget method frontend interface.
   *
   * \pre         A skeleton fire-and-forget method frontend interface shall not have been previously registered.
   *              The related service must not have been offered yet.
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterSkeletonFireAndForgetMethodInterface(
      ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodInterface<InputArgs...>&
          skeleton_fnf_method) noexcept final {
    if (skeleton_fire_and_forget_method_ != nullptr) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Attempt to re-register skeleton fire-and-forget method frontend interface without de-registering "
                 "first.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    skeleton_fire_and_forget_method_ = &skeleton_fnf_method;
  }

  /*!
   * \brief       De-registers a skeleton fire-and-forget method from the method backend.
   *
   * \details     After this call, the registered skeleton fire-and-forget method is considered to be invalid.
   *
   * \pre         Deregister can only be called after the offering of the related service
   *              has been completely stopped.
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances
   *              Any calls to the registered skeleton f&f method must be synchronized against this function.
   * \reentrant   FALSE for same class instance, FALSE for different instances
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeregisterSkeletonFireAndForgetMethodInterface() noexcept final { skeleton_fire_and_forget_method_ = nullptr; }

 private:
  /*!
   * \brief Method request handler to be registered in the server interface.
   */

  std::shared_ptr<SkeletonMethodRequestNoResponseHandler> request_handler_{
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      std::make_shared<SkeletonMethodRequestNoResponseHandler>(*this)};

  /*!
   * \brief Deserializer pointer.
   */
  DeserializerUniquePtr deserializer_;

  /*!
   * \brief SOCAL method interface for forwarding the method requests.
   */
  ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodInterface<InputArgs...>*
      skeleton_fire_and_forget_method_{nullptr};

  /*!
   * \brief Server interface. The referenced object is constructed during initialization phase and destructed
   * during shutdown, therefore it is guaranteed that it outlives this object which is constructed and destructed during
   * runtime.
   */
  ::amsr::someip_binding_core::internal::LocalServerInterface& server_;

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
      ::amsr::someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SkeletonFireAndForgetMethodXf"_sv, method_id_)};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_H_
