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
/*!        \file  skeleton_method_request_no_return_handler_interface.h
 *        \brief
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RETURN_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RETURN_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for handling SOME/IP client callbacks.
 */
class SkeletonMethodRequestNoReturnHandlerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SkeletonMethodRequestNoReturnHandlerInterface() = default;
  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SkeletonMethodRequestNoReturnHandlerInterface() = default;

  SkeletonMethodRequestNoReturnHandlerInterface(SkeletonMethodRequestNoReturnHandlerInterface const&) = delete;
  SkeletonMethodRequestNoReturnHandlerInterface(SkeletonMethodRequestNoReturnHandlerInterface&&) = delete;
  SkeletonMethodRequestNoReturnHandlerInterface& operator=(SkeletonMethodRequestNoReturnHandlerInterface const&) & =
      delete;
  SkeletonMethodRequestNoReturnHandlerInterface& operator=(SkeletonMethodRequestNoReturnHandlerInterface&&) & = delete;

  /*!
   * \brief       Receive handler for method requests.
   * \param[in]   request  request class holding the serialized method request [SOME/IP Header + payload].
   * \return Return positive result or an error.
   * \error  CommunicationErrc::wrong_method_id  If wrong method id is given.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \steady      TRUE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> HandleMethodRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_NO_RETURN_HANDLER_INTERFACE_H_
