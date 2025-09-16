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
/**        \file  someip_binding_core/internal/methods/method_interface.h
 *        \brief Interface class for defining methods API for the Proxy side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/future.h"
#include "amsr/someip_binding_core/internal/methods/method_response.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for defining methods API for the Proxy side.
 */
class MethodInterface {
 public:
  /*!
   * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse).
   */
  using MethodResponseType = ::amsr::someip_binding_core::internal::methods::MethodResponse;

  /*!
   * \brief Future type for the response value.
   */
  using ResponseValueFuture = ::amsr::core::Future<MethodResponseType>;

  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  MethodInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  virtual ~MethodInterface() noexcept = default;

  MethodInterface(MethodInterface const&) = delete;
  MethodInterface(MethodInterface&&) = delete;
  MethodInterface& operator=(MethodInterface const&) & = delete;
  MethodInterface& operator=(MethodInterface&&) & = delete;

  /*!
   * \brief Generates SOME/IP header for the method request and sends the complete SOME/IP message to the server.
   *
   * \param[in] packet The packet to be sent (containing the memory for the serialized header + the serialized
   *                    payload).
   * \return      Future is the return value of the method request.
   *
   * \pre         Packet must contains sufficient size(atleast header size).
   * \pre         If Future::then is used, the callable must be able to be executed in both the App and Reactor context.
                  In case of a send error, the callable may be executed immediately when calling Future::then.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ResponseValueFuture SendMethodRequest(MemoryBufferPtr packet) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_INTERFACE_H_
