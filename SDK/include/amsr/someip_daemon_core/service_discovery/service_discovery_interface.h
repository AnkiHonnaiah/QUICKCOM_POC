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
/**        \file  service_discovery_interface.h
 *        \brief  Service discovery interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/service_discovery/service_discovery_endpoint.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief ServiceDiscovery.
 */
class ServiceDiscoveryInterface {
 public:
  /*!
   * \brief Constructor of ServiceDiscoveryInterface.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryInterface() = default;
  /*!
   * \brief Destructor of ServiceDiscoveryInterface.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryInterface() noexcept = default;

  ServiceDiscoveryInterface(ServiceDiscoveryInterface const&) = delete;
  ServiceDiscoveryInterface(ServiceDiscoveryInterface&&) = delete;
  ServiceDiscoveryInterface& operator=(ServiceDiscoveryInterface const&) & = delete;
  ServiceDiscoveryInterface& operator=(ServiceDiscoveryInterface&&) & = delete;

  /*!
   * \brief Initializes connection manager, endpoints and state machines.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void Initialize() noexcept = 0;

  /*!
   * \brief Get the SD endpoint corresponding to the given address.
   *
   * \param[in] address Address of the endpoint.
   * \return The SD endpoint corresponding to the given address, or nullptr if not existing.
   *
   * \pre -
   * \context Init, Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual ServiceDiscoveryEndpoint* GetSdEndpoint(
      amsr::someip_protocol::internal::IpAddress const& address) noexcept = 0;
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_INTERFACE_H_
