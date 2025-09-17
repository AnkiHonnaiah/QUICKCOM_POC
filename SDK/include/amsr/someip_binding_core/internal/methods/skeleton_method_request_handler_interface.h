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
/*!        \file  skeleton_method_request_handler_interface.h
 *        \brief
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for handling SOME/IP client callbacks.
 */
class SkeletonMethodRequestHandlerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SkeletonMethodRequestHandlerInterface() = default;
  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SkeletonMethodRequestHandlerInterface() = default;

  SkeletonMethodRequestHandlerInterface(SkeletonMethodRequestHandlerInterface const&) = delete;
  SkeletonMethodRequestHandlerInterface(SkeletonMethodRequestHandlerInterface&&) = delete;
  SkeletonMethodRequestHandlerInterface& operator=(SkeletonMethodRequestHandlerInterface const&) & = delete;
  SkeletonMethodRequestHandlerInterface& operator=(SkeletonMethodRequestHandlerInterface&&) & = delete;

  /*!
   * \brief       Receive handler for method requests.
   * \param[in]   request  Request class holding the serialized method request [SOME/IP Header + payload].
   * \return Empty result in case of success or an error code otherwise.
   * \error  CommunicationErrc::wrong_method_id  If wrong method id is given.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> HandleMethodRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_HANDLER_INTERFACE_H_
