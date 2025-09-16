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
 *        \brief  Interface for Proxy FnF Method XF
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/methods/proxy_fire_and_forget_method_backend_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for Proxy Fire and Forget Method Xf.
 *
 * \tparam ArgsT The Input arguments for the method.
 */
template <typename... ArgsT>
class ProxyFireAndForgetMethodXfInterface
    : public ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<ArgsT...> {
 public:
  /*!
   * \brief default ctor
   */
  ProxyFireAndForgetMethodXfInterface() = default;

  /*!
   * \brief default dtor
   */
  virtual ~ProxyFireAndForgetMethodXfInterface() = default;

  ProxyFireAndForgetMethodXfInterface(ProxyFireAndForgetMethodXfInterface const&) = delete;
  ProxyFireAndForgetMethodXfInterface(ProxyFireAndForgetMethodXfInterface&&) = delete;
  auto operator=(ProxyFireAndForgetMethodXfInterface const&) -> ProxyFireAndForgetMethodXfInterface& = delete;
  auto operator=(ProxyFireAndForgetMethodXfInterface&&) -> ProxyFireAndForgetMethodXfInterface& = delete;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_INTERFACE_H_
