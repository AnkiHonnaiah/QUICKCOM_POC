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
/**        \file  amsr/someip_binding_core/internal/client_interface.h
 *        \brief  Interface class for handling SOME/IP client callbacks.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/events/client_event_interface.h"
#include "amsr/someip_binding_core/internal/methods/client_method.h"
#include "amsr/someip_binding_core/internal/methods/client_method_no_return.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_event_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip_daemon_client/internal/someip_daemon_client.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for handling SOME/IP client public calls.
 */
class ClientInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ClientInterface() = default;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientInterface() noexcept = default;

  ClientInterface(ClientInterface const&) = delete;
  ClientInterface(ClientInterface&&) = delete;
  ClientInterface& operator=(ClientInterface const&) & = delete;
  ClientInterface& operator=(ClientInterface&&) & = delete;

  /*!
   * \brief       Starts the client to listen to communication and service status updates.
   *
   * \pre         The binding must not have been previously started or it must have been stopped first.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void Start() noexcept = 0;

  /*!
   * \brief       Stops the client from listening to communication and service status updates.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void Stop() noexcept = 0;

  /*!
   * \brief       Access to the event information that this client provides.
   * \return      Event information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual ServiceEventInformation const& GetEventConfig() const noexcept = 0;

  /*!
   * \brief       Access to the method information that this client provides.
   * \return      Method information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual ServiceMethodInformation const& GetMethodConfig() const noexcept = 0;

  /*!
   * \brief       Access to the field information that this client provides.
   * \return      Field information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual ServiceFieldInformation const& GetFieldConfig() const noexcept = 0;

  /*!
   * \brief       Getter for client method impl.
   * \param[in]   method_id  Method ID.
   * \return      Method interface result.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \steady      FALSE
   */
  virtual ::amsr::core::Result<std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodInterface>>
  GetMethod(::amsr::someip_protocol::internal::MethodId const method_id) noexcept = 0;

  /*!
   * \brief       Getter for client method no return impl.
   * \param[in]   method_id  Method ID.
   * \return      Method no response interface result.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \steady      FALSE
   */
  virtual ::amsr::core::Result<
      std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodNoResponseInterface>>
  GetMethodNoReturn(::amsr::someip_protocol::internal::MethodId const method_id) noexcept = 0;

  /*!
   * \brief       Creates and provides access to the ClientEvent that will be filled by its implementation that is
   *              stored in the LocalClient.
   * \param[in]   event_id      Event ID
   * \return      A shared pointer result to access the registered event.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \steady      FALSE
   */
  virtual amsr::core::Result<std::shared_ptr<amsr::someip_binding_core::internal::events::ClientEventInterface>>
  GetEvent(::amsr::someip_protocol::internal::EventId event_id) const noexcept = 0;

  /*!
   * \brief       Creates and registers client event implementation.
   * \param[in]   event_information      Event information.
   * \return      A shared pointer result to access the newly registered event.
   *
   * \context     Any (SomeIpBindingLite)
   * \pre         Calling this method is only allowed in context of SomeIpBindingLite.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual amsr::core::Result<std::shared_ptr<amsr::someip_binding_core::internal::events::ClientEventInterface>>
  RegisterEvent(::amsr::someip_binding_core::internal::ServiceEventInformation const& event_information) noexcept = 0;

  /*!
   * \brief       Unregisters client event implementation.
   * \param[in]   event_id      Event ID.
   *
   * \context     Any (SomeIpBindingLite)
   * \pre         Calling this method is only allowed in context of SomeIpBindingLite.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void UnregisterEvent(::amsr::someip_protocol::internal::EventId event_id) noexcept = 0;

  /*!
   * \brief       Register Method Response handler in order to receive responses for this specific method.
   *
   * \param[in]   id       Method ID.
   * \param[in]   handler  Method response handler.
   *
   * \pre         The client has stopped or not yet started.
   * \pre         No other handler has been registered for the same id.
   * \pre         No method requests have been called yet.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void RegisterMethodResponseHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface* handler) noexcept = 0;

  /*!
   * \brief       Unregister Method Response handler.
   * \details     After this call is returned, no method responses shall be received by the registered handler.
   *
   * \param[in]   id  Method ID.
   *
   * \pre         The client has stopped or not yet started.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void UnregisterMethodResponseHandler(::amsr::someip_protocol::internal::MethodId const id) noexcept = 0;

  /*!
   * \brief       An instantiated proxy will send a method request to a server.
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   * \return      amsr::core::Result with no error in case of successful transmission or error code otherwise.
   * \error       ClientError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ClientError::kSystemConfigurationNotAvailable if this required service instance is not configured.
   * \error       ClientError::kSystemPrivilegesNoAccess if access is denied for this service.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) noexcept = 0;

  /*!
   * \brief       Lets the SOME/IP binding know that a proxy wishes to receive an event of a service instance.
   * \details     This function will abort in case
   *              - the connection to the SOME/IP daemon has not been established.
   *              - the event has already been subscribed to
   * \param[in]   event_id    SOME/IP event id of the service.
   * \param[in]   event_handler  Pointer to the event handler, used for event notification and subscription state
   *                             updates.
   * \return      An empty result if subscribe request is successfully done, error otherwise.
   *
   * \error ::amsr::someip_binding_core::internal::ClientError::kRuntimeConnectionTransmissionFailed
   * No connection to the SOME/IP Daemon has been established.
   *
   * \pre         - Connection to the SOME/IP daemon is established.
   *              - The event is currently not subscribed to.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  virtual amsr::core::Result<void> SubscribeEvent(
      ::amsr::someip_protocol::internal::EventId const event_id,
      ::amsr::someip_binding_core::internal::SomeipBindingEventHandlerInterface* event_handler) noexcept = 0;

  /*!
   * \brief       Lets the SOME/IP binding know that an proxy does not wish to receive an event of a service instance.
   * \param[in]   event_id    SOME/IP event id of the service.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void UnsubscribeEvent(::amsr::someip_protocol::internal::EventId const event_id) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_INTERFACE_H_
