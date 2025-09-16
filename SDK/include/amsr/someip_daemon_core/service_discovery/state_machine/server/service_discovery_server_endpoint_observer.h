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
/**        \file  service_discovery_server_endpoint_observer.h
 *
 *        \brief  This interface provides pure virtual methods to handle actions on server reception of a SOME/IP SD
 *message.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_ENDPOINT_OBSERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_ENDPOINT_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief This interface provides pure virtual methods to handle actions on server reception of a SOME/IP SD message.
 */
class ServiceDiscoveryServerEndpointObserver {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryServerEndpointObserver() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryServerEndpointObserver() noexcept = default;

  ServiceDiscoveryServerEndpointObserver(ServiceDiscoveryServerEndpointObserver const&) = delete;
  ServiceDiscoveryServerEndpointObserver(ServiceDiscoveryServerEndpointObserver&&) = delete;
  ServiceDiscoveryServerEndpointObserver& operator=(ServiceDiscoveryServerEndpointObserver const&) & = delete;
  ServiceDiscoveryServerEndpointObserver& operator=(ServiceDiscoveryServerEndpointObserver&&) & = delete;

  /*!
   * \brief Function to handle reboot detection for unicast and multicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   *
   * \context Network
   * \steady TRUE
   */
  virtual void OnRebootDetected(someip_daemon_core::IpAddress const& from_address,
                                amsr::net::ip::Port const& from_port) noexcept = 0;

  /*!
   * \brief Function to handle reception of an FindService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The FindService entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady FALSE
   */
  virtual bool OnFindServiceEntry(someip_daemon_core::IpAddress const& from_address,
                                  amsr::net::ip::Port const& from_port,
                                  message::entries::FindServiceEntry const& entry) noexcept = 0;

  /*!
   * \brief Function to handle reception of an SubscribeEventgroup SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The SubscribeEventgroup entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady TRUE
   */
  virtual bool OnSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& from_address,
                                          amsr::net::ip::Port const& from_port,
                                          message::entries::SubscribeEventgroupEntry const& entry) = 0;

  /*!
   * \brief Function to handle reception of an StopSubscribeEventgroup SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The StopSubscribeEventgroup entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady FALSE
   */
  virtual bool OnStopSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& from_address,
                                              amsr::net::ip::Port const& from_port,
                                              message::entries::StopSubscribeEventgroupEntry const& entry) = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_ENDPOINT_OBSERVER_H_
