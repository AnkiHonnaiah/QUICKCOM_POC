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
/*!        \file  someip_binding_xf/internal/methods/skeleton_method_request_no_response_handler.h
 *         \brief SkeletonRequestNoResponseHandler represents a SOME/IP method request handler.
 *         \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonMethodRequestNoResponseHandler
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RESPONSE_HANDLER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RESPONSE_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief RequestNoResponseHandler class for handle fire and forget method request.
 */
class SkeletonMethodRequestNoResponseHandler final
    : public ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestNoReturnHandlerInterface {
 public:
  /*!
   * \brief Type alias for SkeletonMethodRequestNoReturnHandlerInterface.
   */
  using SkeletonMethodRequestNoReturnHandlerInterface =
      ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestNoReturnHandlerInterface;

  /*!
   * \brief       Constructor
   *
   * \param[in]   fire_and_forget_method_xf Reference to the fire and forget method xf handler to delegate the calls.
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  explicit SkeletonMethodRequestNoResponseHandler(
      SkeletonMethodRequestNoReturnHandlerInterface& fire_and_forget_method_xf)
      : SkeletonMethodRequestNoReturnHandlerInterface{}, fire_and_forget_method_xf_{fire_and_forget_method_xf} {}

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SkeletonMethodRequestNoResponseHandler() noexcept final = default;

  SkeletonMethodRequestNoResponseHandler(SkeletonMethodRequestNoResponseHandler const&) = delete;
  SkeletonMethodRequestNoResponseHandler(SkeletonMethodRequestNoResponseHandler&&) = delete;
  SkeletonMethodRequestNoResponseHandler& operator=(SkeletonMethodRequestNoResponseHandler const&) & = delete;
  SkeletonMethodRequestNoResponseHandler& operator=(SkeletonMethodRequestNoResponseHandler&&) & = delete;

  /*!
   * \brief         Forward method request to actual FireAndForgetMethodXf implementation.
   *
   * \param[in,out] request  Incoming method request to be processed.
   * \return Always return positive result.
   *
   * \pre -
   *
   * \context       Reactor
   *
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady TRUE \endinternal
   */
  ::amsr::core::Result<void> HandleMethodRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return fire_and_forget_method_xf_.HandleMethodRequestNoReturn(std::move(request));
  }

 private:
  /*!
   * \brief Reference to the fire and forget method xf handler to delegate the calls.
   */
  ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestNoReturnHandlerInterface&
      fire_and_forget_method_xf_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RESPONSE_HANDLER_H_
