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
/**        \file  someip_binding_core/internal/methods/method_accessor_interface.h
 *        \brief  Method Accessor Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_ACCESSOR_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_ACCESSOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/methods/method_interface.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class that combines MethodInterface and ProxyMethodResponseHandlerInterface.
 */
// VECTOR NC AutosarC++17_10-M10.2.1: MD_SOMEIPBINDING_AutosarC++17_10-M10.2.1_entity_names_in_multiple_inheritance
class MethodAccessorInterface : public MethodInterface, public ProxyMethodResponseHandlerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  MethodAccessorInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ~MethodAccessorInterface() noexcept override = default;

  MethodAccessorInterface(MethodAccessorInterface const&) = delete;
  MethodAccessorInterface(MethodAccessorInterface&&) = delete;
  MethodAccessorInterface& operator=(MethodAccessorInterface const&) & = delete;
  MethodAccessorInterface& operator=(MethodAccessorInterface&&) & = delete;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_ACCESSOR_INTERFACE_H_
