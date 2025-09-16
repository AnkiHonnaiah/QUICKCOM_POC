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
/**        \file  service_discovery_message_header_builder.h
 *        \brief  SD Message Header Builder
 *
 *      \details  Builder for SD Message Header. Unicast flag and reboot parameters are managed by this class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_HEADER_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_HEADER_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "amsr/someip_daemon_core/service_discovery/reboot_management/session_generator.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * \brief SD Message Header Builder.
 *        The class sets the following header parameters:
 *         - Unicast flag
 *         - Reboot flag
 *         - Session
 *        Each ServiceDiscoveryEndpoint has one corresponding Header Builder.
 */
class ServiceDiscoveryMessageHeaderBuilder final {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryMessageHeaderBuilder() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryMessageHeaderBuilder() noexcept = default;

  ServiceDiscoveryMessageHeaderBuilder(ServiceDiscoveryMessageHeaderBuilder const&) = delete;
  ServiceDiscoveryMessageHeaderBuilder(ServiceDiscoveryMessageHeaderBuilder&&) = delete;
  ServiceDiscoveryMessageHeaderBuilder& operator=(ServiceDiscoveryMessageHeaderBuilder const&) = delete;
  ServiceDiscoveryMessageHeaderBuilder& operator=(ServiceDiscoveryMessageHeaderBuilder&&) = delete;

  /*!
   * \brief Determine the SD flags for the next outgoing multicast SOME/IP SD message.
   *
   * \remark The Unicast Flag of the SOME/IP-SD Header is set to Unicast (that means 1) for all SD Messages since this
   *         means that receiving using unicast is supported.
   * \remark The Unicast Flag is left over from historical SOME/IP versions and is only kept for compatibility reasons.
   *         Its use besides this is very limited.
   *
   * \param[in,out] sd_message The SOME/IP SD message to be sent.
   * \pre -
   * \context Timer, Network, App
   * \reentrant FALSE
   * \steady FALSE
   * \trace SPEC-4981509
   */
  static void SetUnicastFlag(ServiceDiscoveryMessage& sd_message);

  /*!
   * \brief Determine the SD reboot flag and the session id for the next outgoing multicast SOME/IP SD message and sets
   *        them to the passed SOME/IP SD message.
   *
   * \param[in,out] sd_message The SOME/IP SD message to be sent.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-4981493
   */
  void SetMulticastRebootParams(ServiceDiscoveryMessage& sd_message);

  /*!
   * \brief Determine the SD reboot flag and the session id for the next outgoing unicast SOME/IP SD message and sets
   *        them to the passed SOME/IP SD message.
   *
   * \param[in,out] sd_message The SOME/IP SD message to be sent.
   * \param[in] to_address An IP address identifying the destination.
   * \param[in] to_port A UDP port identifying the destination.
   * \pre -
   * \context Timer
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-4981493
   */
  void SetUnicastRebootParams(ServiceDiscoveryMessage& sd_message, someip_daemon_core::IpAddress const& to_address,
                              amsr::net::ip::Port const to_port);

 private:
  /*!
   * \brief Reboot flag & Session Generator.
   *
   * \details For each SD endpoint, one reboot manager must exist.
   *
   * \trace SPEC-4981506
   */
  reboot_management::SessionGenerator session_generator_{};
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_HEADER_BUILDER_H_
