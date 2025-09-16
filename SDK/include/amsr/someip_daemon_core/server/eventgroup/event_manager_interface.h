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
/**        \file  event_manager_interface.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/********************************************************************************************************************/

#include "amsr/net/ip/address.h"
#include "someip-protocol/internal/someip_posix_types.h"
namespace amsr {
namespace someip_daemon_core {
namespace server {
namespace eventgroup {

/*!
 * \brief Internal class within the server statemachine to manage un/subscription interface for eventgroups.
 */

class EventManagerInterface {
 public:
  /*!
   * \brief Delete default constructor.
   * \steady FALSE
   */
  EventManagerInterface() = default;
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier

  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  virtual ~EventManagerInterface() noexcept = default;

  EventManagerInterface(EventManagerInterface&&) = delete;
  EventManagerInterface& operator=(EventManagerInterface const&) & = delete;
  EventManagerInterface& operator=(EventManagerInterface&&) & = delete;
  EventManagerInterface(EventManagerInterface const&) = delete;

  /*!
   * \brief Cancels eventgroup subscription if the TCP connection broke unexpectedly.
   *
   * \param[in] tcp_address           TCP address of the subscription.
   * \param[in] tcp_port              TCP port of the subscription.
   * \param[in] eventgroup_id         Eventgroup Id of the new subscription
   *
   * \steady FALSE
   */
  virtual void CancelEventgroupSubscriptionUponTcpDisconnection(
      someip_daemon_core::IpAddress const& tcp_address, amsr::net::ip::Port const tcp_port,
      someip_protocol::internal::EventgroupId const eventgroup_id) noexcept = 0;
};

}  // namespace eventgroup
}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_INTERFACE_H_
