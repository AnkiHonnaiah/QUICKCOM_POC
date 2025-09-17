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
/**        \file  service_discovery_server_interface.h
 *        \brief  A service discovery server interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief A Service Discovery server interface.
 */
class ServiceDiscoveryServerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryServerInterface() noexcept = default;

  /*!
   * \brief  Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryServerInterface() noexcept = default;

  ServiceDiscoveryServerInterface(ServiceDiscoveryServerInterface const&) = delete;
  ServiceDiscoveryServerInterface(ServiceDiscoveryServerInterface&&) = delete;
  ServiceDiscoveryServerInterface& operator=(ServiceDiscoveryServerInterface const&) & = delete;
  ServiceDiscoveryServerInterface& operator=(ServiceDiscoveryServerInterface&&) & = delete;

  /*!
   * \brief Called when a provided service instance is brought up.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \steady       FALSE
   * \synchronous  TRUE
   */
  virtual void OnServiceUp() noexcept = 0;

  /*!
   * \brief Called when a provided service instance is brought down.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void OnServiceDown() noexcept = 0;

  /*!
   * \brief Called when network is brought up.
   *
   * \pre          -
   * \context      Init, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void OnNetworkUp() noexcept = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_INTERFACE_H_
