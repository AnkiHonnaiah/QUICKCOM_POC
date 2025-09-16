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
/*!        \file  skeleton_method_handler_interface.h
 *        \brief
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for handling SOME/IP client callbacks.
 */
class SkeletonMethodHandlerInterface : public SkeletonMethodRequestHandlerInterface,
                                       public SkeletonMethodRequestNoReturnHandlerInterface {
 public:
  /*!
   * \brief Set of skeleton method request handlers for which request handlers have to be registered.
   */
  using RequiredSkeletonMethodHandlerSet = ::amsr::core::Vector<RequiredSkeletonMethodHandler>;

  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SkeletonMethodHandlerInterface() = default;

  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SkeletonMethodHandlerInterface() override = default;

  SkeletonMethodHandlerInterface(SkeletonMethodHandlerInterface const&) = delete;
  SkeletonMethodHandlerInterface(SkeletonMethodHandlerInterface&&) = delete;
  SkeletonMethodHandlerInterface& operator=(SkeletonMethodHandlerInterface const&) & = delete;
  SkeletonMethodHandlerInterface& operator=(SkeletonMethodHandlerInterface&&) & = delete;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_HANDLER_INTERFACE_H_
