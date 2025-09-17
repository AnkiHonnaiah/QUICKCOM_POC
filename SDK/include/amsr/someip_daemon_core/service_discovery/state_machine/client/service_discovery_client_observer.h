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
/**        \file  service_discovery_client_observer.h
 *        \brief  An interface class service discovery client observer
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OBSERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/active_offer_service_entry.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief An observer-pattern interface for the notifications of eventgroup subscription state updates.
 */
class ServiceDiscoveryClientObserver {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientObserver() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryClientObserver() noexcept = default;

  ServiceDiscoveryClientObserver(ServiceDiscoveryClientObserver const &) = delete;
  ServiceDiscoveryClientObserver(ServiceDiscoveryClientObserver &&) = delete;
  ServiceDiscoveryClientObserver &operator=(ServiceDiscoveryClientObserver const &) & = delete;
  ServiceDiscoveryClientObserver &operator=(ServiceDiscoveryClientObserver &&) & = delete;

  /*!
   * \brief Called upon reception of an ACK entry.
   *
   * \param[in] eventgroup_id The event group identifier.
   * \param[in] multicast_address Multicast address.
   * \param[in] multicast_port Multicast port.
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void OnSubscribeEventgroupAck(amsr::someip_protocol::internal::EventgroupId const eventgroup_id,
                                        ara::core::Optional<someip_daemon_core::IpAddress> const &multicast_address,
                                        amsr::someip_protocol::internal::Port const multicast_port) = 0;

  /*!
   * \brief Called upon reception of an NACK entry.
   *
   * \param[in] eventgroup_id The event group identifier.
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void OnSubscribeEventgroupNack(amsr::someip_protocol::internal::EventgroupId const eventgroup_id) = 0;

  /*!
   * \brief     Called when a service has been offered via dynamic SD.
   *
   * \param[in] entry        The offer service SD entry.
   *
   * \pre       The service instance must be known in the configuration.
   * \context   Network
   * \steady    FALSE
   */
  virtual void OnOfferRemoteService(service_discovery::state_machine::client::ActiveOfferServiceEntry const &entry) = 0;

  /*!
   * \brief Called when an offer renewal entry has been received (necessary to trigger Subscription messages).
   *
   * \param[in] is_multicast indicates whether the entry is received via multicast or unicast.
   *
   * \pre -
   * \context Network
   * \steady TRUE
   */
  virtual void OnOfferRenewal(bool const is_multicast) = 0;

  /*!
   * \brief     Called when a service is no longer offered offered
   *
   * \pre       -
   * \context   Network
   * \steady    FALSE
   */
  virtual void OnStopOfferRemoteService() = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OBSERVER_H_
