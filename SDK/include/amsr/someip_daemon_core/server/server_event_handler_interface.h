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
/**        \file  server_event_handler_interface.h
 *        \brief  ServerEventHandler Interface.
 *
 *      \details  Interface used by the network to communicate with a remote client
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_EVENT_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/generic/generic_error_domain.h"
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief ServerEventHandlerInterface.
 */
class ServerEventHandlerInterface {
 public:
  /*!
   * \brief Delete default constructor.
   * \steady FALSE
   */
  ServerEventHandlerInterface() = default;

  /*!
   * \brief Define default destructor.
   * \steady FALSE
   */
  virtual ~ServerEventHandlerInterface() noexcept = default;

  ServerEventHandlerInterface(ServerEventHandlerInterface const&) = delete;
  ServerEventHandlerInterface(ServerEventHandlerInterface&&) = delete;
  ServerEventHandlerInterface& operator=(ServerEventHandlerInterface const&) & = delete;
  ServerEventHandlerInterface& operator=(ServerEventHandlerInterface&&) & = delete;

  /*!
   * \brief Add a new subscriber for a given event group.
   *
   * \pre A TCP connection must have been opened by the client before calling this function.
   *
   * \param[in] udp_endpoint An optional containing IP address and port using UDP from a provided service instance.
   * \param[in] tcp_endpoint An optional containing IP address and port using TCP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration if the corresponding
   *         TCP connection was not found.
   *
   * \context   Init, Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<void> SubscribeEventgroup(
      ara::core::Optional<amsr::someip_daemon_core::service_discovery::message ::options::IpEndpointOption> const&
          udp_endpoint,
      ara::core::Optional<amsr::someip_daemon_core::service_discovery::message ::options::IpEndpointOption> const&
          tcp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id) noexcept = 0;

  /*!
   * \brief Remove a subscriber for a given event group.
   *
   * \param[in] udp_endpoint An optional containing IP address and port using UDP from a provided service instance.
   * \param[in] tcp_endpoint An optional containing IP address and port using TCP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   *
   * \context   Init, Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void UnsubscribeEventgroup(
      ara::core::Optional<amsr::someip_daemon_core::service_discovery::message ::options::IpEndpointOption> const&
          udp_endpoint,
      ara::core::Optional<amsr::someip_daemon_core::service_discovery::message ::options::IpEndpointOption> const&
          tcp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id) noexcept = 0;

  /*!
   * \brief Test whether a TCP connection with the specified remote IP address and remote port exist or not.
   *
   * \param[in] address A remote IP address.
   * \param[in] port A remote port number.
   *
   * \return "true" if the specified connection exists and "false" otherwise.
   *
   * \context   Init, Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual bool HasTcpConnection(someip_daemon_core::IpAddress const& address,
                                amsr::net::ip::Port const port) const noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_EVENT_HANDLER_INTERFACE_H_
