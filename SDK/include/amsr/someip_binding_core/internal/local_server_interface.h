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
/**        \file  amsr/someip_binding_core/internal/local_server_interface.h
 *        \brief  Interface for calls from the backend on the Local Server class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for SOME/IP Server implementations.
 */
class LocalServerInterface {
 public:
  /*!
   * \brief Default default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  LocalServerInterface() = default;

  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~LocalServerInterface() = default;

  LocalServerInterface(LocalServerInterface const&) = delete;
  LocalServerInterface& operator=(LocalServerInterface const&) & = delete;
  LocalServerInterface& operator=(LocalServerInterface&&) & = delete;
  LocalServerInterface(LocalServerInterface&&) = delete;

  /*!
   * \brief             Register a new method request handler.
   * \param[in] id      Method id.
   * \param[in] handler Handler for this skeleton method.
   * \pre               No method request handler has been registered before for this method id.
   * \context           App
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \exceptionsafety   No side effects.
   * \synchronous       TRUE
   * \steady            FALSE
   */
  virtual void RegisterMethodRequestHandler(::amsr::someip_protocol::internal::MethodId const id,
                                            std::weak_ptr<methods::SkeletonMethodRequestHandlerInterface> handler) = 0;

  /*!
   * \brief             Unregister a method request handler.
   * \param[in] id      Method id.
   * \pre               A method request handler has been registered before for this method id.
   * \pre               Server is not offered.
   * \context           App
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \exceptionsafety   No side effects.
   * \synchronous       TRUE
   * \steady            FALSE
   */
  virtual void UnregisterMethodRequestHandler(::amsr::someip_protocol::internal::MethodId const id) = 0;

  /*!
   * \brief             Register a new method request no return handler.
   * \param[in] id      Method id.
   * \param[in] handler Handler for this skeleton fire and forget method.
   * \pre               No method request handler has been registered before for this method id.
   * \context           App
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \exceptionsafety   No side effects.
   * \synchronous       TRUE
   * \steady            FALSE
   */
  virtual void RegisterMethodRequestNoReturnHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      std::weak_ptr<methods::SkeletonMethodRequestNoReturnHandlerInterface> handler) = 0;

  /*!
   * \brief             Unregister a method request handler.
   * \param[in] id      Method id.
   * \pre               A method request handler has been registered before for this method id.
   * \pre               Server is not offered.
   * \context           App
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \exceptionsafety   No side effects.
   * \synchronous       TRUE
   * \steady            FALSE
   */
  virtual void UnregisterMethodRequestNoReturnHandler(::amsr::someip_protocol::internal::MethodId const id) = 0;

  /*!
   * \brief           Offer a service over the SOME/IP binding from the skeleton.
   * \return          amsr::core::Result<void, Error> that holds no value nor error upon success.
   *                  Otherwise, it contains an error.
   * \pre             RegisterMethodResponseHandler() has been called and UnregisterMethodResponseHandler()
   *                  has not been called.
   * \error           ServerError::kNotAvailable If the server is already owned by another provider local application.
   * \error           ServerError::kSystemPrivilegesNoAccess If the application lacking the proper permission
   *                  for offering server.
   * \error           All other possible errors mapped from someip_daemon_client to be reported to that caller, and
   *                  casted from 'ControlMessageReturnCode' type to 'ServerError' type.
   * \context         App
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \exceptionsafety No side effects.
   * \synchronous     TRUE
   * \steady          FALSE
   */
  virtual amsr::core::Result<void> OfferService() = 0;

  /*!
   * \brief       Unoffer a specific service instance from the skeleton.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void StopOfferService() = 0;

  /*!
   * \brief       An instantiated skeleton implementation will send an event notification to the client.
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   * \pre         Connection to the SOME/IP Daemon is established
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void SendEventNotification(MemoryBufferPtr packet) = 0;

  /*!
   * \brief An instantiated skeleton implementation will send a signal based event notification to the client.
   * \param[in] packet Contains the complete SomeIpPacket of header and payload.
   * \pre Connection to the SOME/IP Daemon is established.
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void SendPduEventNotification(MemoryBufferPtr packet) = 0;

  /*!
   * \brief       An instantiated skeleton implementation will send a method response to the client.
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void SendMethodResponse(
      MemoryBufferPtr packet,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) = 0;

  /*!
   * \brief       Send a SOME/IP error response packet.
   * \details     Used in case of:
   *              - Infrastructural checks failed (protocol version, return code, message type)
   *              - Deserialization failed
   *              - Serialization of method responses failed
   *              - Service / Method is not implemented.
   * \param[in]   return_code    The return code to write into the SOME/IP header.
   * \param[in]   request_header The unmodified SOME/IP header.
   *                             This SOME/IP header will be used for assembling the error
   *                             response and is partially being reused.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void SendErrorResponse(
      someip_protocol::internal::ReturnCode const return_code,
      someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_INTERFACE_H_
