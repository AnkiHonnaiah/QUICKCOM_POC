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
/**        \file  service_discovery_subscribe_eventgroup_ack_entry.h
 *        \brief  Data structure for SubscribeEventgroupAck Service Discovery entries
 *
 *      \details  This data structure contains the parameters of a Service Discovery Subscribe Eventgroup Ack entry
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_SUBSCRIBE_EVENTGROUP_ACK_ENTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_SUBSCRIBE_EVENTGROUP_ACK_ENTRY_H_

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
 * \brief Represents a SOME/IP SD SubscribeEventgroupAck entry.
 *
 * \trace SPEC-4981561
 * \trace SPEC-4981570
 * \trace SPEC-4981614
 */
struct SubscribeEventgroupAckEntry {
  /*!
   * \brief Entry identifier
   */
  entries::EventgroupEntryId entry_id{};

  /*!
   * \brief Time to live
   */
  amsr::someip_protocol::internal::Ttl ttl{};

  /*!
   * \brief The Multicast address to which the server will send the multicast events
   */
  ara::core::Optional<options::IpEndpointOption> multicast_endpoint;
};

/*!
 * \brief A container for SubscribeEventgroupAckEntry
 */
using SubscribeEventgroupAckEntryContainer = ara::core::Vector<SubscribeEventgroupAckEntry>;

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_SUBSCRIBE_EVENTGROUP_ACK_ENTRY_H_
