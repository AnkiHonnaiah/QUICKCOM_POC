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
/*!        \file   someip_binding_generic_layer/internal/methods/generic_proxy_method.h
 *         \brief  Method manager to handle common logic for method requests and return responses on proxy side within
 *                 SOME/IP binding.
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::Methods::GenericProxyMethod
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/future.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/methods/generic_proxy_method_backend_interface.h"
#include "amsr/someip_binding_core/internal/methods/client_method.h"
#include "amsr/someip_binding_core/internal/methods/method_response.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Method manager to handle common logic for method requests and return responses on proxy side within
 *        SOME/IP binding.
 */
class GenericProxyMethod final : public ::amsr::socal::internal::methods::GenericProxyMethodBackendInterface {
 public:
  /*!
   * \brief Type alias for the concrete memory buffer type.
   */
  using BufferPtrType = ::vac::memory::allocator::MemoryBufferPtr;

  /*!
   * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse).
   */
  using MethodResponseType = ::amsr::someip_binding_core::internal::methods::MethodResponse;

  /*!
   * \brief Future type for the method response type.
   */
  using ResponseValueFuture = ::amsr::core::Future<MethodResponseType>;

  /*!
   * \brief Generic data future type.
   */
  using GenericDataFuture = ::amsr::core::Future<::amsr::socal::GenericDataPtr>;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~GenericProxyMethod() = default;

  GenericProxyMethod(GenericProxyMethod&&) = delete;
  GenericProxyMethod(GenericProxyMethod const&) = delete;
  auto operator=(GenericProxyMethod const&) -> GenericProxyMethod& = delete;
  auto operator=(GenericProxyMethod&&) -> GenericProxyMethod& = delete;

  /*!
   * \brief  Constructor.
   * \param[in] client_method   The client method.
   * \param[in] allocator       Allocator for memory used to serialize/send method request.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericProxyMethod(std::unique_ptr<amsr::someip_binding_core::internal::methods::ClientMethod> client_method,
                     amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> allocator) noexcept;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Serialized method arguments (buffer can be empty in case of no input arguments).
   * \return A future object that can be used to access the result of the method call.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \error <ApplicationError>               The configured application error, if any.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \spec requires true; \endspec
   */
  auto HandleRequest(::amsr::socal::GenericDataType const& args) noexcept -> GenericDataFuture final;

 private:
  /*!
   * \brief Return error code based on the internal error code retrieved from the negative response.
   *
   * \param[in] internal_error_code  Internal error code.
   *
   * \return  Error code
   * \pre     -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::ErrorCode CreateErrorCode(amsr::core::ErrorDomain::CodeType const internal_error_code) noexcept;

  /*!
   * \brief Client method unique ptr.
   */
  std::unique_ptr<::amsr::someip_binding_core::internal::methods::ClientMethod> client_method_;

  /*!
   * \brief Memory buffer allocator.
   */
  amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> tx_buffer_allocator_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_H_
