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
/**        \file  reboot_management_types.h
 *        \brief  Types for Reboot management
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_MANAGEMENT_TYPES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_MANAGEMENT_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/net/ip/address.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace reboot_management {

/*!
 * \brief structure to store session_id and reboot flag for a unicast or multicast message.
 */
struct RebootParam {
  /*!
   * \brief Session id identifier.
   */
  amsr::someip_protocol::internal::SessionId session_id_{amsr::someip_protocol::internal::kSomeIpSdMinimumSessionId};

  /*!
   * \brief Reboot flag must be set to 1 after starting the controller and when the
   * session ID changes from 0xFFFF to 0x0001, the reboot flag must be set to 0.
   */
  bool reboot_flag_{true};
};

/*!
 * \brief A key to identify if a reboot has happened. It is composed by the string representation of an IP address and
 * port.
 */
struct RebootDetectionKey {
  /*!
   * \brief   A tuple with an IP address and port.
   * \details It is allocated in a fixed-size array.
   */
  std::pair<amsr::net::ip::Address::StringBuffer, amsr::someip_protocol::internal::Port> key;

  /*!
   * \brief Compare two objects of RebootDetectionKey.
   * \param[in] other The other object to compare to this object.
   * \return true if the key is less than the other; false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  bool operator<(RebootDetectionKey const& other) const noexcept { return key < other.key; }
};

/*!
 * \brief Maps the information of AddressPair to the session_id and reboot flag
 */
using RebootManagementMap = ara::core::Map<RebootDetectionKey, RebootParam>;

}  // namespace reboot_management
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_MANAGEMENT_TYPES_H_
