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
/**        \file  service_discovery_server_state_owner.h
 *        \brief  Service Discovery Server State Owner
 *
 *      \details  This class is the owner of the State Pool.
 *                It receives events and forward them to the current state.
 *                It also holds more variables that are not represented as separate states.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_pool.h"
#include "vac/statemachine/state_owner.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief ServiceDiscoveryServerStateOwner Server State Owner
 */
class ServiceDiscoveryServerStateOwner final
    : public vac::statemachine::StateOwner<ServiceDiscoveryServerStateInterface> {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] state_pool           State pool.
   * \param[in] service_instance_id  The service instance id.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ServiceDiscoveryServerStateOwner(ServiceDiscoveryServerStatePool& state_pool,
                                   configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryServerStateOwner() noexcept override = default;

  ServiceDiscoveryServerStateOwner(ServiceDiscoveryServerStateOwner const&) = delete;
  ServiceDiscoveryServerStateOwner(ServiceDiscoveryServerStateOwner&&) = delete;
  ServiceDiscoveryServerStateOwner& operator=(ServiceDiscoveryServerStateOwner const&) & = delete;
  ServiceDiscoveryServerStateOwner& operator=(ServiceDiscoveryServerStateOwner&&) & = delete;

  /*!
   * \brief Sets the Context.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void SetContext(ServiceDiscoveryServerContextInterface* context);

  // Current State Variables

  /*!
   * \brief Checks the current network state
   *
   * \return "true" if network is up and "false" otherwise.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsNetworkUp() const;

  /*!
   * \brief Check the current service state.
   *
   * \return "true" if service is requested and "false" if released.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsServiceUp() const;

  // State Control

  /*!
   * \brief Change current state to requested_next_state.
   *
   * \pre          -
   * \context      App, Timer, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void UpdateState();

  /*!
   * \brief Called just before a multicast offer service message is sent.
   *
   * \pre -
   * \context     Timer
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady       FALSE
   */
  void OnOfferScheduled();

  /*!
   * \brief Called after an offer service message has been sent.
   * \pre -
   * \context     Timer
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady       FALSE
   */
  void OnOfferSent();

  // Events

  /*!
   * \brief Called when network is brought up.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnNetworkUp();

  /*!
   * \brief Called when a provided service instance is brought up.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnServiceUp();

  /*!
   * \brief Called when a provided service instance is brought down.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnServiceDown();

  /*!
   * \brief Called on reception of a SOME/IP SD FindService entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnFindServiceEntry(someip_daemon_core::IpAddress const& from_address,
                          amsr::net::ip::Port const& from_port) noexcept;

 private:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns the context used by context depending State methods.
   *
   * \return SD server state machine context.
   * \pre          -
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ServiceDiscoveryServerContextInterface& GetContext() override;

  /*!
   * \brief State machine context.
   */
  ServiceDiscoveryServerContextInterface* context_{nullptr};

  /*!
   * \brief Network status; true = connected, false = disconnected.
   */
  bool network_up_{false};

  /*!
   * \brief Server service status; true = available, false = not available.
   */
  bool service_up_{false};

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_
