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
/**        \file  session_generator.h
 *        \brief  Session ID / Reboot Flag Generator
 *
 *      \details  Generates the session id and reboot flag for unicast and multicast SD messages.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_SESSION_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_SESSION_GENERATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/service_discovery/reboot_management/reboot_management_types.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace reboot_management {

/*!
 * \brief Maintains the session id and reboot flag for unicast and multicast messages for reboot detection.
 */
class SessionGenerator {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  SessionGenerator() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~SessionGenerator() = default;
  SessionGenerator(SessionGenerator const &) = delete;
  SessionGenerator(SessionGenerator &&) = delete;
  SessionGenerator &operator=(SessionGenerator const &) & = delete;
  SessionGenerator &operator=(SessionGenerator &&) & = delete;

  /*!
   * \brief Adds a new entry or updates an existing entry in the sd_unicast_sender_reboot_management_map and returns the
   * next session id and reboot flag.
   * \param[in] address_pair structure to store ip address and port for message.
   * \return RebootParam next session id and reboot flag.
   *
   * \context Timer
   * \steady TRUE
   * \trace SPEC-4981504
   * \trace SPEC-4981505
   * \trace SPEC-4981494
   * \trace SPEC-4981495
   * \trace SPEC-4981496
   */
  RebootParam GetNextRebootParamOnUnicastMessageTransmission(RebootDetectionKey const &reboot_key);

  /*!
   * \brief Returns the next session id and reboot flag for the multicast message and increments the session id for a
   * new multicast message being sent.
   * \return RebootParam last session id and reboot flag.
   *
   * \context Timer
   * \steady FALSE
   * \trace SPEC-4981504
   * \trace SPEC-4981505
   * \trace SPEC-4981494
   * \trace SPEC-4981495
   * \trace SPEC-4981496
   */
  RebootParam GetNextRebootParamOnMulticastMessageTransmission();

 private:
  /*!
   * \brief Maps the information of unicast message sender to the session_id and reboot flag.
   * \trace SPEC-4981506
   */
  RebootManagementMap sd_unicast_sender_reboot_management_map_;

  /*!
   * \brief Maintains the information of multicast message sender session_id and reboot flag.
   * \trace SPEC-4981506
   */
  RebootParam sd_multicast_sender_reboot_param_;

  /*!
   * \brief Friend test to access the private member sd_unicast_sender_reboot_management_map_
   */
  FRIEND_TEST(UT__SomeIpd__SessionGenerator, someipd_posix__service_discovery__reboot_management__WrapUnicastSessionId);

  /*!
   * \brief Friend test to access the private member sd_multicast_sender_reboot_param_
   */
  FRIEND_TEST(UT__SomeIpd__SessionGenerator,
              someipd_posix__service_discovery__reboot_management__WrapMulticastSessionId);
};

}  // namespace reboot_management
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_SESSION_GENERATOR_H_
