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
/*!        \file  someip_binding_core/internal/local_client_impl.h
 *        \brief  Local client Impl
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::LocalClient
 *
 *      \details  This class shall be used for communication with the binding core.
 *                The life cycle is controller by its user.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_IMPL_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <mutex>
#include <utility>
#include "amsr/someip_binding_core/internal/client_interface.h"
#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/events/client_event_interface.h"
#include "amsr/someip_binding_core/internal/events/event_accessor_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/methods/client_method.h"
#include "amsr/someip_binding_core/internal/methods/client_method_no_return.h"
#include "amsr/someip_binding_core/internal/methods/method_accessor_interface.h"
#include "amsr/someip_binding_core/internal/methods/method_no_response_interface.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/proxy_handler_interface.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/service_listener_registry_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

// VECTOR Next Construct Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief   This class shall be used for communication with the binding core.
 *          It shall be connected/disconnected to/from the binding consistently by the user.
 * \details To use this class, the following sequence shall be done:
 *          - Construction & Preparation: (single threaded)
 *            - RequiredServiceInstanceImpl::CreateClient() // triggers SomeIpDaemon to generate client ID.
 *            - LocalClientImpl::LocalClientImpl()
 *            - LocalClientImpl::RegisterMethodResponseHandler()
 *          - Runtime: (Multi-threaded App + Single-threaded Reactor contexts)
 *            - LocalClientImpl::Start() // This allows reactor context APIs
 *            - [App] LocalClientImpl::SubscribeEvent
 *            - [App] LocalClientImpl::UnsubscribeEvent
 *            - [App] LocalClientImpl::SendMethodRequest
 *            - [Reactor] LocalClientImpl::HandleEventSubscriptionStateUpdate
 *            - [Reactor] LocalClientImpl::HandleSomeIpEvent
 *            - [Reactor] LocalClientImpl::HandleSignalBasedEvent
 *            - [Reactor] LocalClientImpl::OnServiceInstanceUp
 *            - [Reactor] LocalClientImpl::OnServiceInstanceDown
 *            - [Reactor] LocalClientImpl::HandleMethodResponse
 *            - LocalClientImpl::Stop() // Once stopped, no more reactor context APIs are triggered.
 *          - Destruction:
 *            - LocalClientImpl::UnregisterMethodResponseHandler()
 *            - LocalClientImpl::ReleaseService() // triggers SomeIpDaemon to release client ID.
 *            - LocalClientImpl::~LocalClientImpl()
 */
// VECTOR NC AutosarC++17_10-A10.2.1: MD_SOMEIPBINDING_AutosarC++17_10-A10.2.1_entity_names_in_multiple_inheritance
// VECTOR NC AutosarC++17_10-M10.2.1: MD_SOMEIPBINDING_AutosarC++17_10-M10.2.1_entity_names_in_multiple_inheritance
class LocalClientImpl final : public ::amsr::someip_binding_core::internal::ClientInterface,
                              public ::amsr::someip_binding_core::internal::ProxyHandlerInterface,
                              public ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface,
                              public ::std::enable_shared_from_this<LocalClientImpl> {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] proxy_identity                      Unique ID of the client.
   * \param[in] service_information                 Service interface information used in the service instance.
   * \param[in] client_manager                      Client Manager object that shall be used for message reception.
   * \param[in] client_transport_to_daemon          The client-transport-to-daemon.
   * \param[in] service_listener_registry_handle    A reference to the service status update handler to register and
   *                                                unregister from notifications upon request and releasing of this
   *                                                binding client.
   *
   * \context App
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  LocalClientImpl(::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
                  ::amsr::someip_binding_core::internal::ServiceInformation const& service_information,
                  ::amsr::someip_binding_core::internal::ClientManagerInterface& client_manager,
                  ::amsr::someip_binding_core::internal::ClientTransportToDaemonInterface& client_transport_to_daemon,
                  ::amsr::someip_binding_core::internal::ServiceListenerRegistryInterface&
                      service_listener_registry_handle) noexcept;

  /*!
   * \brief Destructor
   *
   * \pre     All registered method response handlers have been already unregistered.
   * \pre     The client is stopped.
   * \context App
   * \steady  FALSE
   */
  ~LocalClientImpl() noexcept final;

  LocalClientImpl(LocalClientImpl const&) = delete;
  LocalClientImpl(LocalClientImpl&&) = delete;
  LocalClientImpl& operator=(LocalClientImpl const&) & = delete;
  LocalClientImpl& operator=(LocalClientImpl&&) & = delete;

 private:
  /*!
   * \brief Map between method id and client method impl.
   */
  using ClientMethodMap =
      std::map<::amsr::someip_protocol::internal::MethodId,
               std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodAccessorInterface>>;

  /*!
   * \brief Map between method id and client method no return impl.
   */
  using ClientMethodNoReturnMap =
      std::map<::amsr::someip_protocol::internal::MethodId,
               std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodNoResponseInterface>>;

  /*!
   * \brief Map between method ID and method response handler.
   */
  using MethodHandlersType =
      std::map<::amsr::someip_protocol::internal::MethodId, methods::ProxyMethodResponseHandlerInterface*>;

  /*!
   * \brief Map of event notification and subscription state update handlers
   */
  using EventHandlerMap = std::map<::amsr::someip_protocol::internal::EventId,
                                   someip_binding_core::internal::SomeipBindingEventHandlerInterface*>;

  /*!
   * \brief Map of event implementations.
   */
  using ClientEventMap = std::map<::amsr::someip_protocol::internal::EventId,
                                  std::shared_ptr<someip_binding_core::internal::events::EventAccessorInterface>>;

  /*!
   * \brief         Callback function to be called from binding whenever one service instance gets offered.
   * \param[in]     offered_service The provided service instance ID of the offered service.
   * \pre           -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

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
  void OnServiceInstanceDown(ProvidedServiceInstanceId const& stopped_service) noexcept final;

  /*!
   * \brief       Lets the SOME/IP binding know that a proxy wishes to receive an event of a service instance.
   * \details     This function will abort in case the connection to the SOME/IP daemon has not been established.
   * \param[in]   event_id       SOME/IP event id of the service.
   * \param[in]   event_handler  Pointer to the event handler, used for event notification and subscription state
   *                             updates.
   * \return      A result which always has a value.
   *
   * \pre         Connection to the SOME/IP daemon is established
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   */
  amsr::core::Result<void> SubscribeEvent(
      ::amsr::someip_protocol::internal::EventId const event_id,
      ::amsr::someip_binding_core::internal::SomeipBindingEventHandlerInterface* event_handler) noexcept final;

  /*!
   * \brief       Handle reception of a PDU event message.
   * \param[in]   event The PDU event message.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   */
  void HandleSignalBasedEvent(events::PduMessage const& event) noexcept final;

  /*!
   * \brief       Handle reception of an event subscription state update.
   * \param[in]   event_subscription_state_entry The event subscription state.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleEventSubscriptionStateUpdate(
      someip_protocol::internal::EventSubscriptionState const& event_subscription_state_entry) noexcept final;

  /*!
   * \brief       Handle reception of an event message.
   * \param[in]   event The event message.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleSomeIpEvent(internal::events::EventMessage const& event) noexcept final;

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
  void UnsubscribeEvent(::amsr::someip_protocol::internal::EventId const event_id) noexcept final;

  /*!
   * \brief       Starts the client to listen to communication and service status updates.
   * \details     After this call, we are allowed to receive callbacks.
   *              i.e. calls from context Reactor.
   *
   * \pre         The binding must not have been previously started or it must have been stopped first.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Start() noexcept final;

  /*!
   * \brief       Stops the client from listening to communication and service status updates.
   * \details     After this call, it is guaranteed that no more callbacks are received.
   *              i.e. no more calls from context Reactor.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Stop() noexcept final;

  /*!
   * \brief       Access to the event information that this client provides.
   * \return      event information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  ServiceEventInformation const& GetEventConfig() const noexcept final;

  /*!
   * \brief       Access to the method information that this client provides.
   * \return      method information reference.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  ServiceMethodInformation const& GetMethodConfig() const noexcept final;

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
  ServiceFieldInformation const& GetFieldConfig() const noexcept final;

  /*!
   * \brief       Getter for client method impl.
   * \param[in]   method_id  Method ID.
   * \return      Method interface result.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested method is not configured.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ::amsr::core::Result<std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodInterface>> GetMethod(
      ::amsr::someip_protocol::internal::MethodId const method_id) noexcept final;

  /*!
   * \brief       Getter for client method no return impl.
   * \param[in]   method_id  Method ID.
   * \return      Method no response interface result.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested method is not configured.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ::amsr::core::Result<std::shared_ptr<::amsr::someip_binding_core::internal::methods::MethodNoResponseInterface>>
  GetMethodNoReturn(::amsr::someip_protocol::internal::MethodId const method_id) noexcept final;

  /*!
   * \brief       Getter for client event implementation.
   * \param[in]   event_id      Event ID
   * \return      A shared pointer result to access the registered event.
   * \error       ClientError::kSystemConfigurationNotAvailable if the requested event is not configured.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::core::Result<std::shared_ptr<amsr::someip_binding_core::internal::events::ClientEventInterface>> GetEvent(
      ::amsr::someip_protocol::internal::EventId event_id) const noexcept final;

  /*!
   * \brief       Creates and registers client event implementation.
   * \param[in]   event_information      Event information.
   * \return      A shared pointer result to access the newly registered event.
   *
   * \context     Any (SomeIpBindingLite)
   * \pre         Calling this method is only allowed in context of SomeIpBindingLite.
   * \pre         It needs to be ensured that the event is not already registered.
   * \pre         Its only allowed to register one event at a time.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::core::Result<std::shared_ptr<amsr::someip_binding_core::internal::events::ClientEventInterface>> RegisterEvent(
      ::amsr::someip_binding_core::internal::ServiceEventInformation const& event_information) noexcept final;

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
  void UnregisterEvent(::amsr::someip_protocol::internal::EventId event_id) noexcept final;

  /*!
   * \brief       Register Method Response handler into Client object in order to receive responses
   *              for this specific method.
   *
   * \param[in]   id       Method ID.
   * \param[in]   handler  Method response handler.
   *
   * \pre         The client has stopped or not yet started.
   * \pre         No other handler has been registered for the same id.
   *
   * \context     App (During Proxy ctor)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterMethodResponseHandler(
      ::amsr::someip_protocol::internal::MethodId const id,
      amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface* handler) noexcept final;

  /*!
   * \brief       Unregister Method Response handler.
   * \details     After this call is returned, no method responses shall be received
   *              by the registered handler.
   *
   * \param[in]   id  Method ID.
   *
   * \pre         The client has stopped or not yet started.
   * \pre         A handler with the given method ID has been already registered.
   *
   * \context     App (During Proxy dtor)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  void UnregisterMethodResponseHandler(::amsr::someip_protocol::internal::MethodId const id) noexcept final;

  /*!
   * \brief       Receive handler for method responses.
   *
   * \param[in]   method_response Variant of the method response.
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
  void HandleMethodResponse(
      amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface::MethodResponseType&&
          method_response) noexcept final;

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
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief       Access to the method information and create the client method implementation instances.
   * \param[in]   method_information  Struct that stores the method information
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void CreateMethodInstances(ServiceMethodInformation const& method_information) noexcept;

  /*!
   * \brief       Access to the event information and create the client event implementation instances.
   * \param[in]   event_information      Struct that stores the event information
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void CreateEventInstances(ServiceEventInformation const& event_information) noexcept;

  /*!
   * \brief       Access to the field information and create the client event implementation instances.
   * \param[in]   field_information      Struct that has the field information
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void CreateFieldInstances(ServiceFieldInformation const& field_information) noexcept;

  /*!
   * \brief       Store a new method instance in the method map.
   * \param[in]   method_id Method ID.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void EmplaceMethod(someip_protocol::internal::MethodId method_id) noexcept;

  /*!
   * \brief       Verify unregistered method response handler.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void VerifyUnregisteredMethodResponseHandler() noexcept;

  /*!
   * \brief Lock for avoiding concurrent access to event_handlers_map_
   */
  std::mutex event_handlers_lock_{};

  /*!
   * \brief List of event notification and event subscription state handlers.
   * \details The map is filled during SomeipBinding initialization, and cleaned during deinitialization. No
   *          modification is done during Runtime.
   */
  EventHandlerMap event_handlers_map_{};

  /*!
   * \brief List of client event implementations
   * \details The map is filled during initialization, and cleaned during deinitialization. No
   *          modification is done during Runtime.
   */
  ClientEventMap client_event_map_{};

  /*!
   * \brief Unique identifier for this client
   */
  ::amsr::someip_binding_core::internal::ProxyBindingIdentity const proxy_identity_;

  /*!
   * \brief Service interface information this client implements. This is a shared immutable reference to the
   * information stored in the required service instance impl shared among all created clients.
   */
  ::amsr::someip_binding_core::internal::ServiceInformation const& service_information_;

  /*!
   * \brief Reference for ClientManager.
   */
  ::amsr::someip_binding_core::internal::ClientManagerInterface& client_manager_;

  /*!
   * \brief The client-transport-to-daemon.
   */
  ::amsr::someip_binding_core::internal::ClientTransportToDaemonInterface& client_transport_to_daemon_;

  /*!
   * \brief A reference to the service status update handler to register and unregister from notifications upon
   *        releasing this binding client.
   */
  ::amsr::someip_binding_core::internal::ServiceListenerRegistryInterface& service_listener_registry_handle_;

  /*!
   * \brief A map which includes the methods handlers associated to the corresponding method ID.
   */
  MethodHandlersType method_handlers_{};

  /*!
   * \brief Map between method id and client method.
   */
  ClientMethodMap client_method_map_{};

  /*!
   * \brief Map between method id and client method no return.
   */
  ClientMethodNoReturnMap client_method_no_return_map_{};

  /*!
   * \brief Flag used to indicate whether the binding has been started and registered into the ClientManager.
   *        Only accessed from App context (single threaded).
   */
  bool is_started_{false};

  /*!
   * \brief Logger for tracing and debugging.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_CLIENT_IMPL_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::LocalClientImpl::event_handlers_lock_
 *                Ensures consistency while read and write access to the container of registered event handlers
 *                (subscribers).
 *
 * \protects ::amsr::someip_binding_core::internal::LocalClientImpl::event_handlers_map_
 *           is the protected resource and contains all registered event handlers.
 *
 * \usedin SubscribeEvent(write), UnsubscribeEvent(write), HandleSignalBasedEvent(read), HandleSomeIpEvent      (read),
 *         HandleEventSubscriptionStateUpdate(read)
 *
 * \length MEDIUM Limited to a map lookup, an emplacement if a event handler is added to the map,
 *                a removal of a event handler entry from the map,
 *                a subscription state update callback or
 *                a notification callback.
 *                The exclusive area starts and ends always in the smallest possible scope within the protected methods.
 *                This exclusive area covers calls to at most one method. The complexity is logarithmic in the size of
 *                the container.
 * \endexclusivearea
 */
