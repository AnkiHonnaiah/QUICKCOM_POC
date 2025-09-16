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
/**        \file  service_discovery_client_interface.h
 *        \brief  A service discovery client interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/network/interface_card_state_observer_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/offer_service_listener.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_endpoint_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_observer.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief A Service Discovery client interface.
 */
class ServiceDiscoveryClientInterface : public network::InterfaceCardStateObserverInterface,
                                        public ServiceDiscoveryClientEndpointObserver {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientInterface() noexcept = default;

  /*!
   * \brief  Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryClientInterface() noexcept override = default;

  ServiceDiscoveryClientInterface(ServiceDiscoveryClientInterface const&) = delete;
  ServiceDiscoveryClientInterface(ServiceDiscoveryClientInterface&&) = delete;
  ServiceDiscoveryClientInterface& operator=(ServiceDiscoveryClientInterface const&) & = delete;
  ServiceDiscoveryClientInterface& operator=(ServiceDiscoveryClientInterface&&) & = delete;

  /*!
   * \brief Register an observer to the Client State Machine.
   *
   * \param[in] instance_id The instance id of the RemoteServer to register.
   * \param[in] observer    The observer to the Client State Machine.
   *
   * \note instance_id parameter is needed for the Instance Id All use case, where one SD client may have multiple
   *       observers
   *
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual void RegisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id,
                                    ServiceDiscoveryClientObserver* const observer) noexcept = 0;

  /*!
   * \brief Unregister an observer from the Client State Machine.
   *
   * \param[in] instance_id The instance id of the RemoteServer to register.
   *
   * \note instance_id parameter is needed for the Instance Id All use case, where one SD client may have multiple
   *       observers
   *
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual void UnregisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id) noexcept = 0;

  /*!
   * \brief Register a listener for service offer updates.
   *
   * \param[in] offer_service_listener Listener. It will be notified upon service status changes.
   *
   * \pre -
   * \context Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual void RegisterOfferServiceListener(OfferServiceListener* offer_service_listener) noexcept = 0;

  /*!
   * \brief Called on service instance shutdown.
   *
   * \details Called from ServiceDiscovery.
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \pre     -
   * \trace SPEC-4981668
   * \trace SPEC-4981669
   */
  virtual void StopServiceDiscoveryClient() noexcept = 0;

  /*!
   * \brief Unregister the listener.
   *
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \pre     -
   */
  virtual void UnregisterOfferServiceListener() = 0;

  // ---- Calls from Local applications ----------------------------------------------------------------------

  /*!
   * \brief Called when the service has been requested.
   *
   * \details Called from ServiceDiscovery.
   * \pre     Service has not been requested before.
   * \context App
   * \steady FALSE
   */
  virtual void RequestService() noexcept = 0;

  /*!
   * \brief Called when the service has been released.
   *
   * \details Called from ServiceDiscovery.
   * \pre     Service has been requested before.
   * \context App
   * \steady FALSE
   */
  virtual void ReleaseService() noexcept = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_INTERFACE_H_
