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
/**        \file  service_discovery_stop_subscribe_eventgroup_entry.h
 *        \brief  Data structure for SubscribeEventgroup Service Discovery entries
 *
 *      \details  This data structure contains the parameters of a Service Discovery Stop Subscribe Eventgroup entry
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_STOP_SUBSCRIBE_EVENTGROUP_ENTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_STOP_SUBSCRIBE_EVENTGROUP_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_eventgroup_entry_id.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace entries {

/*!
 * \brief Represents a SOME/IP SD StopSubscribeEventgroup entry.
 * \trace SPEC-4981624
 * \trace SPEC-4981625
 */
struct StopSubscribeEventgroupEntry {
  /*!
   * \brief Entry identifier
   */
  entries::EventgroupEntryId entry_id{};

  /*!
   * \brief The unicast address of the client, this shall be the same as the one used in
   * entries::SubscribeEventgroupEntry
   */
  ara::core::Optional<options::IpEndpointOption> udp_endpoint;

  /*!
   * \brief The unicast address of the client, this shall be the same as the one used in
   * entries::SubscribeEventgroupEntry
   */
  ara::core::Optional<options::IpEndpointOption> tcp_endpoint;
};

/*!
 * \brief A container for StopSubscribeEventgroupEntry
 */
using StopSubscribeEventgroupEntryContainer = ara::core::Vector<StopSubscribeEventgroupEntry>;

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_STOP_SUBSCRIBE_EVENTGROUP_ENTRY_H_
