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
/*!        \file  someip_binding_xf/internal/methods/skeleton_method_request_handler.h
 *         \brief SkeletonMethodRequestHandler represents a SOME/IP method request handler.
 *         \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonMethodRequestHandler
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief RequestHandler class for handle method request.
 */
class SkeletonMethodRequestHandler final
    : public ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestHandlerInterface {
 public:
  /*!
   * \brief Type alias for SkeletonMethodRequestHandlerInterface.
   */
  using SkeletonMethodRequestHandlerInterface =
      ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestHandlerInterface;

  /*!
   * \brief       Constructor
   *
   * \param[in]   method_xf   Reference to the method xf handler to delegate the calls.
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  explicit SkeletonMethodRequestHandler(SkeletonMethodRequestHandlerInterface& method_xf)
      : SkeletonMethodRequestHandlerInterface{}, method_xf_{method_xf} {}

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SkeletonMethodRequestHandler() noexcept final = default;

  SkeletonMethodRequestHandler(SkeletonMethodRequestHandler const&) = delete;
  SkeletonMethodRequestHandler(SkeletonMethodRequestHandler&&) = delete;
  SkeletonMethodRequestHandler& operator=(SkeletonMethodRequestHandler const&) & = delete;
  SkeletonMethodRequestHandler& operator=(SkeletonMethodRequestHandler&&) & = delete;

  /*!
   * \brief         Forward method request to actual MethodXf implementation.
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
  ::amsr::core::Result<void> HandleMethodRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return method_xf_.HandleMethodRequest(std::move(request));
  }

 private:
  /*!
   * \brief Reference to the method xf handler to delegate the calls.
   */
  ::amsr::someip_binding_core::internal::methods::SkeletonMethodRequestHandlerInterface& method_xf_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_H_
