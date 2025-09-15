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
/**        \file  service_discovery_client_endpoint_observer.h
 *
 *        \brief  This interface provides pure virtual methods to handle actions on client reception of a SOME/IP SD
 *message.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ENDPOINT_OBSERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ENDPOINT_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief This interface provides pure virtual methods to handle actions on client reception of a SOME/IP SD message.
 */
class ServiceDiscoveryClientEndpointObserver {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientEndpointObserver() noexcept = default;

  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryClientEndpointObserver() noexcept = default;

  ServiceDiscoveryClientEndpointObserver(ServiceDiscoveryClientEndpointObserver const&) = delete;
  ServiceDiscoveryClientEndpointObserver(ServiceDiscoveryClientEndpointObserver&&) = delete;
  ServiceDiscoveryClientEndpointObserver& operator=(ServiceDiscoveryClientEndpointObserver const&) & = delete;
  ServiceDiscoveryClientEndpointObserver& operator=(ServiceDiscoveryClientEndpointObserver&&) & = delete;

  /*!
   * \brief Function to handle reboot detection for unicast and multicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void OnRebootDetected(someip_daemon_core::IpAddress const& from_address,
                                amsr::net::ip::Port const& from_port) = 0;

  /*!
   * \brief Function to handle reception of an OfferService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The OfferService entry
   * \param[in] is_multicast  A flag indicating whether the received SD message came from a multicast channel
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual bool OnOfferServiceEntry(someip_daemon_core::IpAddress const& from_address,
                                   amsr::net::ip::Port const& from_port,
                                   message::entries::OfferServiceEntry const& entry, bool const is_multicast) = 0;

  /*!
   * \brief Function to handle reception of a StopOfferService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The StopOfferService entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual bool OnStopOfferServiceEntry(someip_daemon_core::IpAddress const& from_address,
                                       amsr::net::ip::Port const& from_port,
                                       message::entries::StopOfferServiceEntry const& entry) = 0;

  /*!
   * \brief Function to handle reception of an SubscribeEventgroupAck SOME/IP SD entry.
   *
   * \param[in] entry         The SubscribeEventgroupAck entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual bool OnSubscribeEventgroupAckEntry(message::entries::SubscribeEventgroupAckEntry const& entry) = 0;

  /*!
   * \brief Function to handle reception of an SubscribeEventgroupNAck SOME/IP SD entry.
   *
   * \param[in] entry         The SubscribeEventgroupNAck entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual bool OnSubscribeEventgroupNackEntry(message::entries::SubscribeEventgroupNAckEntry const& entry) = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ENDPOINT_OBSERVER_H_
