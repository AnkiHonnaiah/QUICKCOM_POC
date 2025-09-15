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
/**        \file  service_discovery_client_state_machine_context.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief A Service Discovery Client state machine context.
 */
class ServiceDiscoveryClientStateMachineContext {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientStateMachineContext() noexcept = default;

  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryClientStateMachineContext() noexcept = default;

  ServiceDiscoveryClientStateMachineContext(ServiceDiscoveryClientStateMachineContext const &) = delete;
  ServiceDiscoveryClientStateMachineContext(ServiceDiscoveryClientStateMachineContext &&) = delete;
  ServiceDiscoveryClientStateMachineContext &operator=(ServiceDiscoveryClientStateMachineContext const &) & = delete;
  ServiceDiscoveryClientStateMachineContext &operator=(ServiceDiscoveryClientStateMachineContext &&) & = delete;

  /*!
   * \brief Checks the availability of the service.
   *
   * \return true if the service is available (offered, and TTL is not expired), and false otherwise.
   *
   * \context App, Timer
   * \steady FALSE
   */
  virtual bool IsServiceAvailable() const = 0;

  /*!
   * \brief Indicates whether the network is active or not.
   *
   * \return true if the network is active and false otherwise.
   * \context App
   * \steady FALSE
   */
  virtual bool IsNetworkUp() const = 0;

  /*!
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   * \context App
   * \steady FALSE
   */
  virtual bool IsServiceRequested() const = 0;

  /*!
   * \brief Transmits a FindService entry.
   * \context App
   * \steady FALSE
   */
  virtual void SendFindService() = 0;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_
