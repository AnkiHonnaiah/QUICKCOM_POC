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
/**        \file  someip_binding_core/internal/methods/proxy_method_response_handler_interface.h
 *        \brief  Method manager to handle common logic for methods on proxy side within SOME/IP
 *                binding.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>

#include "amsr/core/variant.h"
#include "amsr/someip_binding_core/internal/methods/application_error_response.h"
#include "amsr/someip_binding_core/internal/methods/error_response.h"
#include "amsr/someip_binding_core/internal/methods/method_response.h"
#include "amsr/someip_binding_core/internal/methods/positive_response.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface defining methods API for the Proxy side.
 */
class ProxyMethodResponseHandlerInterface {
 public:
  /*!
   * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse, ErrorResponse).
   */
  using MethodResponseType = ::amsr::someip_binding_core::internal::methods::MethodResponseVariant;

  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ProxyMethodResponseHandlerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  virtual ~ProxyMethodResponseHandlerInterface() noexcept = default;

  ProxyMethodResponseHandlerInterface(ProxyMethodResponseHandlerInterface const&) = delete;
  ProxyMethodResponseHandlerInterface(ProxyMethodResponseHandlerInterface&&) = delete;
  ProxyMethodResponseHandlerInterface& operator=(ProxyMethodResponseHandlerInterface const&) & = delete;
  ProxyMethodResponseHandlerInterface& operator=(ProxyMethodResponseHandlerInterface&&) & = delete;

  /*!
   * \brief Receive handler for method responses.
   *
   * \param[in] method_response Variant of the method response.
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
  virtual void HandleMethodResponse(MethodResponseType&& method_response) noexcept = 0;

  /*!
   * \brief       Callback triggered when the service is down.
   * \param[in]   stopped_service The stopped service instance.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnServiceInstanceDown(ProvidedServiceInstanceId const& stopped_service) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_HANDLER_INTERFACE_H_
