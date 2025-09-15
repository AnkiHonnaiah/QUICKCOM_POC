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
/*!        \file  someip_binding_core/internal/local_client.h
 *        \brief  Local client
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::LocalClient
 *
 *      \details  This class shall be used for communication with the binding core.
 *                The life cycle is controller by its user.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/client_interface.h"
#include "amsr/someip_binding_core/internal/events/client_event.h"
#include "amsr/someip_binding_core/internal/methods/client_method.h"
#include "amsr/someip_binding_core/internal/methods/client_method_no_return.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief This class shall be used for communication with the binding core.
 *        It shall be connected/disconnected to/from the binding consistently by the user.
 */
class LocalClient final {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] implementation  A shared pointer to the binding client implementation.
   * \param[in] client_id       The client ID.
   *
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires implementation!= nullptr;
   * \endspec
   */
  LocalClient(std::shared_ptr<ClientInterface> implementation,
              someip_protocol::internal::ClientId const client_id) noexcept;

  ~LocalClient() noexcept = default;

  /*!
   * \brief     Default move constructor
   * \param[in] other The moved-from object
   *
   * \context ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  LocalClient(LocalClient&& other) noexcept = default;

  LocalClient(LocalClient const&) = delete;
  LocalClient& operator=(LocalClient const&) & = delete;
  LocalClient& operator=(LocalClient&&) & = delete;

  /*!
   * \brief       Register Method Response handler into Client object in order to receive responses
   *              for this specific method.
   * \param[in]   id       Method ID.
   * \param[in]   handler  Method response handler.
   *
   * \pre         The binding must not haven been previously requested.
   * \pre         No other handler has been registered for the same id.
   * \pre         No method requests have been called yet.
   * \pre         The API must not be called on a moved from LocalClient's object.
   *
   * \context     App (During Proxy ctor)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterMethodResponseHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface* handler) noexcept;

  /*!
   * \brief       Unregister Method Response handler.
   * \details     After this call is returned, no method responses shall be received
   *              by the registered handler.
   * \param[in]   id  Method ID.
   *
   * \pre         The binding must haven been previously released.
   * \pre         A handler with the given method ID has been already registered.
   * \pre         The API must not be called on a moved from LocalClient's object.
   *
   * \context     App (During Proxy dtor)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void UnregisterMethodResponseHandler(::amsr::someip_protocol::internal::MethodId const id) noexcept;

  /*!
   * \brief       Return the client ID.
   * \return      The client ID.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  someip_protocol::internal::ClientId GetClientId() const noexcept;

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
   * \pre         The API must not be called on a moved from LocalClient's object.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) const noexcept;

  /*!
   * \brief       Lets the SOME/IP binding know that a proxy wishes to receive an event of a service instance.
   * \details     This function will abort in case the connection to the SOME/IP daemon has not been established.
   * \param[in]   event_id    SOME/IP event id of the service.
   * \param[in]   event_handler  Pointer to the event handler, used for event notification and subscription state
   *                             updates.
   * \return      A result which always has a value.
   *
   * \pre         Connection to the SOME/IP daemon is established
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      FALSE
   * \synchronous TRUE
   * \vprivate
   */
  amsr::core::Result<void> SubscribeEvent(
      ::amsr::someip_protocol::internal::EventId const event_id,
      ::amsr::someip_binding_core::internal::SomeipBindingEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief       Lets the SOME/IP binding know that an proxy does not wish to receive an event of a service instance.
   * \details     The function will abort if:
   *              - a subscription to the event cannot be retrieved
   *              - a handler to deal with event notifications cannot be retrieved
   *              - an error is returned from the attempt to forward the unsubscription to the Some/IP daemon
   * \param[in]   event_id    SOME/IP event id of the service.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  void UnsubscribeEvent(::amsr::someip_protocol::internal::EventId const event_id) noexcept;

  /*!
   * \brief       Starts the binding by registering into the client manager and into listening the service status
   *              updates.
   * \details     After this call, we are allowed to receive callbacks.
   *
   * \pre         The binding must not have been previously started or it must have been stopped first.
   * \pre         The API must not be called on a moved from LocalClient's object.
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Start() const noexcept;

  /*!
   * \brief       Stops the binding by unregistering from the client manager and from the service status updates.
   * \details     After this call, it is guaranteed that no more callbacks are received.
   *
   * \pre         The API must not be called on a moved from LocalClient's object.
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Stop() const noexcept;

  /*!
   * \brief       Access to the event information that this client provides.
   * \return      Event information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceEventInformation const& GetEventConfig() const noexcept;

  /*!
   * \brief       Access to the method information that this client provides.
   * \return      Method information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceMethodInformation const& GetMethodConfig() const noexcept;

  /*!
   * \brief       Access to the field information that this client provides.
   * \return      Field information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceFieldInformation const& GetFieldConfig() const noexcept;

  /*!
   * \brief       Getter for client method.
   * \param[in]   method_id  Method ID.
   * \return      Client method result.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested method is not configured.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ::amsr::core::Result<std::unique_ptr<::amsr::someip_binding_core::internal::methods::ClientMethod>> GetMethod(
      ::amsr::someip_protocol::internal::MethodId const method_id) const noexcept;

  /*!
   * \brief       Getter for client method no return.
   * \param[in]   method_id  Method ID.
   * \return      Client method no response result.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested method is not configured.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ::amsr::core::Result<std::unique_ptr<::amsr::someip_binding_core::internal::methods::ClientMethodNoReturn>>
  GetMethodNoReturn(::amsr::someip_protocol::internal::MethodId const method_id) const noexcept;

  /*!
   * \brief       Creates and provides access to the ClientEvent that will be filled by its implementation that is
   *              stored in the LocalClient.
   * \param[in]   event_id    SOME/IP event id of the service.
   * \return      A Unqiue pointer result to access the client event.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested event is not configured.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::core::Result<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>> GetEvent(
      ::amsr::someip_protocol::internal::EventId event_id) const noexcept;

  /*!
   * \brief       Creates and provides access to the ClientEvent that will be filled by its implementation that will be
   *              stored in the LocalClient.
   * \details     This method is only needed in context of SomeIpBindingLite. Before it can be subscribed to an event,
   *              the event needs to be created and registered into the LocalClient.
   *
   * \param[in]   event_information      Eventinformation.
   * \return      A unqiue pointer result to access the newly registered event. The returned result is always valid.
   *
   * \context     Any (SomeIpBindingLite)
   * \pre         Calling this method is only allowed in context of SomeIpBindingLite.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::core::Result<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>> RegisterEvent(
      ::amsr::someip_binding_core::internal::ServiceEventInformation const& event_information) noexcept;

  /*!
   * \brief       Unregisters client event implementation.
   * \details     This method is only needed in context of SomeIpBindingLite. After the event was successfully
   *              unsubscribed, it needs to be deregistered from the LocalClient.
   *
   * \param[in]   event_id      Event ID.
   *
   * \context     Any (SomeIpBindingLite)
   * \pre         Calling this method is only allowed in context of SomeIpBindingLite.
   * \pre         The event was successfully unsubscribed.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void UnregisterEvent(::amsr::someip_protocol::internal::EventId event_id) noexcept;

 private:
  /*!
   * \brief A shared pointer to the binding client implementation.
   */
  std::shared_ptr<ClientInterface> implementation_;

  /*!
   * \brief The client ID.
   */
  someip_protocol::internal::ClientId const client_id_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_H_
