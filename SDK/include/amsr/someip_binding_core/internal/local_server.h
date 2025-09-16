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
/**        \file  amsr/someip_binding_core/internal/local_server.h
 *        \brief  ara::com SOME/IP Binding Local Server memory wrapper.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::LocalServer
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/local_server_manager_interface.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Manages memory ownership of LocalServerImpl to allow the object to be movable
 */
class LocalServer final : public LocalServerInterface {
 public:
  /*!
   * \brief       Constructor.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   local_server_impl     Unique pointer to this local server implementation.
   * \param[in]   local_server_manager  Reference to the local server manager.
   *
   * \pre         -
   *
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  LocalServer(ProvidedServiceInstanceId const service_instance_id,
              std::unique_ptr<LocalServerInterface> local_server_impl,
              std::weak_ptr<LocalServerManagerInterface> local_server_manager) noexcept;

  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ~LocalServer() noexcept final;

  /*!
   * \brief       Default move constructor.
   * \details     This will invalidate the owned resource. Any call to an instance of a
   *              moved-from object of this type will trigger an abort.
   * \param[in,out] other  Other LocalServer to be moved in.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  LocalServer(LocalServer&& other) noexcept;

  LocalServer() = delete;
  LocalServer(LocalServer const&) = delete;
  LocalServer& operator=(LocalServer const&) & = delete;
  LocalServer& operator=(LocalServer&&) & = delete;

  /*!
   * \brief           Offer a service over the SOME/IP binding from the skeleton by forwarding the call
   *                  to the corresponding server implementation.
   *
   * \return          amsr::core::Result<void, Error> that holds no value nor error upon success.
   *                  Otherwise, it contains an error.
   * \error           ServerError::kAlreadyOffered If the server was already offered.
   * \error           ServerError::kSystemPrivilegesNoAccess If the application lacking the proper permission
   *                  for offering server.
   * \error           All other possible errors mapped from someip_daemon_client to be reported to that caller, and
   *                  casted from 'ControlMessageReturnCode' type to 'ServerError' type.
   *
   * \context         App
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   * \exceptionsafety No side effects.
   *
   * \vprivate
   */
  amsr::core::Result<void> OfferService() noexcept final;

  /*!
   * \brief       Stop offering a specific service instance from the skeleton by forwarding the call to the
   *              corresponding server implementation.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  void StopOfferService() noexcept final;

  /*!
   * \brief       An instantiated skeleton implementation will send an event notification to the client.
   *              If the service is not yet offered, the event will be stored as initial value if it belongs
   *              to field notification.
   *
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   *
   * \pre         Connection to the SOME/IP Daemon is established
   *
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \vprivate
   */
  void SendEventNotification(MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief       An instantiated skeleton implementation will send a signal based event notification to the client.
   *
   * \param[in]   packet Contains the complete SomeIpPacket of header and payload.
   * \pre         Connection to the SOME/IP Daemon is established.
   * \pre         Service is offered
   *
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \vprivate
   */
  void SendPduEventNotification(MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief       An instantiated skeleton implementation will send a method response to the client.
   *
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   *
   * \pre         Connection to the SOME/IP Daemon is established
   * \pre         Service is offered
   *
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \vprivate
   */
  void SendMethodResponse(
      MemoryBufferPtr packet,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept final;

  /*!
   * \brief       Send a SOME/IP error response packet by forwarding the call to the corresponding server
   *              implementation.
   *
   * \details     Checks if service is on offer and delegates to DoSendErrorResponse. Used in case of:
   *              - Infrastructural checks failed (protocol version, return code, message type)
   *              - Deserialization failed
   *              - Serialization of method responses failed
   *              - Service / Method is not implemented.
   *
   * \param[in]   return_code    The return code to write into the SOME/IP header.
   * \param[in]   request_header The unmodified SOME/IP header.
   *                             This SOME/IP header will be used for assembling the error
   *                             response and is partially being reused.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   *
   * \pre         Connection to the SOME/IP Daemon is established
   * \pre         Service is offered
   *
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \vprivate
   */
  void SendErrorResponse(
      someip_protocol::internal::ReturnCode const return_code,
      someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::RegisterMethodRequestHandler
   */
  void RegisterMethodRequestHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      std::weak_ptr<methods::SkeletonMethodRequestHandlerInterface> handler) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::UnregisterMethodRequestHandler
   */
  void UnregisterMethodRequestHandler(::amsr::someip_protocol::internal::MethodId const id) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::RegisterMethodRequestNoReturnHandler
   */
  void RegisterMethodRequestNoReturnHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      std::weak_ptr<methods::SkeletonMethodRequestNoReturnHandlerInterface> handler) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::UnregisterMethodRequestNoReturnHandler
   */
  void UnregisterMethodRequestNoReturnHandler(::amsr::someip_protocol::internal::MethodId const id) noexcept final;

 private:
  /*!
   * \brief The service instance ID for this specific local server.
   */
  ProvidedServiceInstanceId service_instance_id_;

  /*!
   * \brief The concrete implementation for this local server. This resource will be released
   *        and returned to the local server manager once this specific local server gets
   *        destroyed.
   */
  std::unique_ptr<LocalServerInterface> local_server_impl_;

  /*!
   * \brief The local server manager used to return the acquired resources once not needed anymore.
   *        This local server manager is created during initialization inside the SomeIpBindingCore.
   */
  std::weak_ptr<LocalServerManagerInterface> local_server_manager_;
};
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_H_
