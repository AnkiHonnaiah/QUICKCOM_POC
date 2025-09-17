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
/*!        \file   someip_binding_generic_layer/internal/methods/generic_proxy_fire_and_forget_method.h
 *         \brief  Method manager class for handle and send method request on proxy side within SOME/IP binding.
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::Methods::GenericProxyFireAndForgetMethod
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/methods/generic_proxy_fire_and_forget_method_backend_interface.h"
#include "amsr/someip_binding_core/internal/methods/client_method_no_return.h"
#include "someip-protocol/internal/message.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Method manager class for handle and send method request on proxy side within SOME/IP binding.
 */
class GenericProxyFireAndForgetMethod final
    : public ::amsr::socal::internal::methods::GenericProxyFireAndForgetMethodBackendInterface {
 public:
  /*!
   * \brief Type alias for GenericDataFragment.
   */
  using GenericDataFragmentType = ::amsr::socal::GenericDataFragment;

  /*!
   * \brief Type alias for the concrete memory buffer type.
   */
  using BufferPtrType = ::vac::memory::allocator::MemoryBufferPtr;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~GenericProxyFireAndForgetMethod() = default;

  GenericProxyFireAndForgetMethod(GenericProxyFireAndForgetMethod&&) = delete;
  GenericProxyFireAndForgetMethod(GenericProxyFireAndForgetMethod const&) = delete;
  GenericProxyFireAndForgetMethod& operator=(GenericProxyFireAndForgetMethod const&) = delete;
  GenericProxyFireAndForgetMethod& operator=(GenericProxyFireAndForgetMethod&&) = delete;

  /*!
   * \brief  Constructor.
   * \param[in] client_method_no_return   The client method no return.
   * \param[in] allocator                 Allocator for memory used to serialize/send method request.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericProxyFireAndForgetMethod(
      std::unique_ptr<::amsr::someip_binding_core::internal::methods::ClientMethodNoReturn> client_method_no_return,
      amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> allocator) noexcept;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Serialized method arguments (buffer can be empty in case of no input arguments).
   * \return Result with no value if the request was successful or an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> HandleRequest(::amsr::socal::GenericDataType const& args) noexcept final;

 private:
  /*!
   * \brief Client method no return unique ptr.
   */
  std::unique_ptr<::amsr::someip_binding_core::internal::methods::ClientMethodNoReturn> client_method_no_return_;

  /*!
   * \brief Memory buffer allocator.
   */
  amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> tx_buffer_allocator_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_
