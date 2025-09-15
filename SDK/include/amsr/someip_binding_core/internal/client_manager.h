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
/*!        \file  amsr/someip_binding_core/internal/client_manager.h
 *        \brief  ara::com SOME/IP Binding Client Manager
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientManager
 *
 *      \details  ClientManager serves as an anchor between the SOME/IP protocol via POSIX and the
 *                ara::com related high-level layers.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/someip_binding_core/internal/client_handler_interface.h"
#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/events/event_message.h"
#include "amsr/someip_binding_core/internal/events/pdu_message.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip_daemon_client/internal/config_model.h"
#include "someip_daemon_client/internal/default_template_configuration.h"
#include "someip_daemon_client/internal/proxy_someip_daemon_client.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
/*!
 * \brief Realizes a concrete Binding for the proxy communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class ClientManager final : public ClientManagerInterface, public ClientHandlerInterface {
 public:
  ClientManager(ClientManager const&) = delete;
  ClientManager(ClientManager&&) = delete;
  ClientManager& operator=(ClientManager const&) & = delete;
  ClientManager& operator=(ClientManager&&) & = delete;

  /*!
   * \brief Type alias for SOME/IP message type
   */
  using SomeIpMessageType = amsr::someip_protocol::internal::SomeIpMessageType;

  /*!
   * \brief        Constructor.
   * \pre          -
   * \context      Init
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  ClientManager();

  /*!
   * \brief       Use default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ClientManager() noexcept final = default;

  /*!
   * \copydoc   ::amsr::someip_binding_core::internal::ClientHandlerInterface::HandleSomeIpEvent
   */
  void HandleSomeIpEvent(::amsr::someip_protocol::internal::InstanceId const instance_id,
                         ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept final;

  /*!
   * \copydoc   ::amsr::someip_binding_core::internal::ClientHandlerInterface::HandleSomeIpInitialFieldNotification
   */
  void HandleSomeIpInitialFieldNotification(
      ::amsr::someip_protocol::internal::InstanceId const instance_id,
      ::amsr::someip_protocol::internal::ClientId const client_id,
      ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept final;

  /*!
   * \brief       Receive handler for SOME/IP method response.
   * \details     This API is called upon respond to a message.
   * \param[in]   instance_id The SOME/IP instance id of the requested service.
   * \param[in]   packet      Serialized SOME/IP Method Response [SOME/IP message].
   *                          SOME/IP Message type must kResponse or kError.  Any other
   *                          type would be considered as violation and would lead to abort.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-10144318
   */
  void HandleMethodResponse(::amsr::someip_protocol::internal::InstanceId const instance_id,
                            ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept final;

  /*!
   * \brief       Handles signal based EventNotifications.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet      The PDU message.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-4980079, SPEC-4980114
   */
  void HandleSignalBasedEvent(::amsr::someip_protocol::internal::InstanceId const instance_id,
                              ::amsr::someipd_app_protocol::internal::PduMessage&& packet) noexcept final;

  /*!
   * \brief       Called on asynchronous notification about event subscription state update.
   * \details     HandleEventSubscriptionStateUpdate is called every time an event subscription state has been
   * updated.
   * \param[in]   event_subscription_state_entry The entry containing the event details with the current
   * subscription state.
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
  void HandleEventSubscriptionStateUpdate(
      ::amsr::someip_protocol::internal::EventSubscriptionState const& event_subscription_state_entry) noexcept final;

  /*!
   * \brief       Registers one proxy binding on construction of a ServiceProxySomeIpBinding.
   * \details     Called from user thread(s)
   * \param[in]   proxy_identity Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
   * \param[in]   proxy_handler  A weak pointer to the constructed element to store in the map proxy_handlers_. On
   *                             method response a look-up through the registered proxy bindings occurs, to delegate
   *                             the response.
   *                             Proxy binding shall not be already registered.
   *                             Proxy binding shall not be nullptr.
   *
   * \pre         No client with the same identity has been registered.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void RegisterProxyBinding(
      ProxyBindingIdentity const proxy_identity,
      std::weak_ptr<::amsr::someip_binding_core::internal::ProxyHandlerInterface> proxy_handler) noexcept final;

  /*!
   * \brief       De-register one proxy binding from the map (e.g. a proxy service instance gets destructed).
   * \details     Called from user thread(s)
   * \param[in]   proxy_identity The key to search for and the associated value to erase from the map.
   * \pre         Proxy binding has already been registered.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void DeRegisterProxyBinding(ProxyBindingIdentity const proxy_identity) noexcept final;

 private:
  /*!
   * \brief Registered proxy binding objects. This back-link is needed for the routing of method responses,
   * event notifications. No ownership of the bindings through use of weak_ptr.
   */
  using ServiceProxySomeIpBindingMap =
      std::map<ProxyBindingIdentity, std::weak_ptr<::amsr::someip_binding_core::internal::ProxyHandlerInterface>>;

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * Lock for avoiding concurrent read/write of proxy_handlers_
   */
  std::mutex proxy_handlers_lock_{};

  /*!
   * Container of all proxy bindings registered.
   */
  ServiceProxySomeIpBindingMap proxy_handlers_{};
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::ClientManager::proxy_handlers_lock_
 *                Ensures consistency while read and write access to the container of registered proxy bindings.
 *
 * \protects ::amsr::someip_binding_core::internal::ClientManager::proxy_handlers_
 *           is the protected resource and contains all registered proxy bindings.
 *
 * \usedin RegisterProxyBinding(write), DeRegisterProxyBinding(write), OnServiceStopped(read), RouteMethodResponse(read)
 *
 * \length MEDIUM Limited to a map lookup, an emplacement if a proxy binding is added to the map,
 *                a removal of a proxy binding entry from the map,
 *                a reception of a method response or
 *                a service state update callback.
 *                The exclusive area starts and ends always in the smallest possible scope within the protected methods.
 *                This exclusive area covers calls to at most one method. The complexity is logarithmic in the size of
 *                the container.
 * \endexclusivearea
 */
