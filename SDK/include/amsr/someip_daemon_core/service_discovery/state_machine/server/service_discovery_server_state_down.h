/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights redown with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_server_state_down.h
 *        \brief  Down Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the down phase
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_DOWN_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_DOWN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief This class handles the events happening during the Down phase of the SD server state machine.
 * \trace SPEC-10144674
 * \trace SPEC-10144676
 */
class ServiceDiscoveryServerStateDown final : public ServiceDiscoveryServerStateInterface {
 public:
  /*!
   * \brief ServiceDiscoveryServerStateDown
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ServiceDiscoveryServerStateDown() noexcept;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryServerStateDown() noexcept override = default;

  ServiceDiscoveryServerStateDown(ServiceDiscoveryServerStateDown const&) = delete;
  ServiceDiscoveryServerStateDown(ServiceDiscoveryServerStateDown&&) = delete;
  ServiceDiscoveryServerStateDown& operator=(ServiceDiscoveryServerStateDown const&) & = delete;
  ServiceDiscoveryServerStateDown& operator=(ServiceDiscoveryServerStateDown&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Context sensitive method called when state is entered.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      Init, App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnEnter(ServiceDiscoveryServerContextInterface& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Context sensitive method called when state is left.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnLeave(ServiceDiscoveryServerContextInterface& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks if change from current state to state with given handle is valid.
   *
   * \param[in] handle The handle of the requested state.
   *
   * \return true if change is valid, otherwise returns false.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsValidChange(ServiceDiscoveryServerStateHandle const handle) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the provided service instance is brought up.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \steady       FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \trace SPEC-4980065, SPEC-4981637, SPEC-10144677
   */
  void OnServiceUp(ServiceDiscoveryServerContextInterface& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when network is brought up.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnNetworkUp(ServiceDiscoveryServerContextInterface& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the provided service instance is brought down.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnServiceDown(ServiceDiscoveryServerContextInterface& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when network is brought down.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnNetworkDown(ServiceDiscoveryServerContextInterface& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called once OfferService message is sent.
   *
   * \param[in] context The server state machine context.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   * \trace SPEC-10144680
   */
  void OnOfferSent(ServiceDiscoveryServerContextInterface& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc ServiceDiscoveryServerStateInterface::OnFindServiceEntry
   * \steady FALSE
   * \trace SPEC-10144680
   */
  void OnFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                          someip_daemon_core::IpAddress const& from_address,
                          amsr::net::ip::Port const& from_port) const noexcept override;

 private:
  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoveryServerStateDown"}};
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_DOWN_H_
