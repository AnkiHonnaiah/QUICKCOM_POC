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
/**        \file  service_discovery_client_state_context.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "amsr/steady_timer/types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

enum class ServiceDiscoveryClientStateHandle : std::uint8_t;

/*!
 * \brief A Service Discovery Client state context.
 */
class ServiceDiscoveryClientStateContext {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientStateContext() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryClientStateContext() noexcept = default;

  ServiceDiscoveryClientStateContext(ServiceDiscoveryClientStateContext const &) = delete;
  ServiceDiscoveryClientStateContext(ServiceDiscoveryClientStateContext &&) = delete;
  ServiceDiscoveryClientStateContext &operator=(ServiceDiscoveryClientStateContext const &) & = delete;
  ServiceDiscoveryClientStateContext &operator=(ServiceDiscoveryClientStateContext &&) & = delete;

  /*!
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   * \pre     -
   * \context App, Timer
   * \steady FALSE
   */
  virtual bool IsServiceAvailable() const = 0;

  /*!
   * \brief Indicates whether the network is active or not.
   *
   * \return true if the network is active and false otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  virtual bool IsNetworkUp() const = 0;

  /*!
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  virtual bool IsServiceRequested() const = 0;

  /*!
   * \brief Transmits a FindService entry.
   * \pre     -
   * \context App, Timer
   * \steady FALSE
   */
  virtual void SendFindService() = 0;

  /*!
   * \brief Initiates a state change.
   *
   * \param[in] handle The handle of a new state.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  virtual void ChangeState(ServiceDiscoveryClientStateHandle const handle) = 0;

  /*!
   * \brief Starts the timer with a given delay.
   *
   * \param[in] delay A timer delay.
   * \pre     -
   * \context App | Timer
   * \steady FALSE
   */
  virtual void StartTimer(std::chrono::nanoseconds const delay) noexcept = 0;

  /*!
   * \brief Stops the timer.
   * \pre     -
   * \context Timer
   * \steady FALSE
   */
  virtual void StopTimer() = 0;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_CONTEXT_H_
