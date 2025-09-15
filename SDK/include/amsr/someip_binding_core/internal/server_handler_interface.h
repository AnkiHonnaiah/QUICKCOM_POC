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
/**        \file  amsr/someip_binding_core/internal/server_handler_interface.h
 *        \brief  ara::com SOME/IP Binding Server Handler Interface to be implemented in ServerManager.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for SOME/IP ServerManager implementations.
 */
class ServerHandlerInterface {
 public:
  /*!
   * \brief Default default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServerHandlerInterface() = default;
  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServerHandlerInterface() = default;

  ServerHandlerInterface(ServerHandlerInterface const&) = delete;
  ServerHandlerInterface(ServerHandlerInterface&&) = delete;
  ServerHandlerInterface& operator=(ServerHandlerInterface const&) & = delete;
  ServerHandlerInterface& operator=(ServerHandlerInterface&&) & = delete;

  /*!
   * \brief                  Receive method handler for SOME/IP requests.
   * \details                This API is called upon reception of a new method request.
   * \param[in] request      Method Request SOME/IP header + Payload
   * \return Return positive result or an error.
   * \error  CommunicationErrc::not_ready  If skeleton is not running.
   * \pre                    -
   * \context                Reactor
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \vprivate
   * \synchronous            TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal steady TRUE \endinternal
   */
  virtual ::amsr::core::Result<void> OnRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept = 0;

  /*!
   * \brief                  Receive method handler for SOME/IP fire and forget requests.
   * \details                This API is called upon reception of a new fire and forget method request.
   * \param[in] request      Method Request[SOME/IP header + Payload]
   * \return Return positive result or an error.
   * \error  CommunicationErrc::not_ready  If skeleton is not running.
   * \pre                    -
   * \context                Reactor
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \vprivate
   * \synchronous            TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal steady TRUE \endinternal
   */
  virtual ::amsr::core::Result<void> OnRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_HANDLER_INTERFACE_H_
