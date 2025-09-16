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
/**        \file  service_discovery_offer_service_entry.h
 *        \brief  Data structure for OfferService Service Discovery entries
 *
 *      \details  This data structure contains the parameters of a Service Discovery Offer Service entry
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_OFFER_SERVICE_ENTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_OFFER_SERVICE_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_service_entry_id.h"
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
 * \brief Represents a SOME/IP SD OfferService entry.
 *
 * \trace SPEC-4981552
 * \trace SPEC-4981558
 * \trace SPEC-4981596
 * \trace SPEC-4981597
 * \trace SPEC-4981598
 * \trace SPEC-4981599
 */
struct OfferServiceEntry {
  /*!
   * \brief Entry identifier
   */
  entries::ServiceEntryId entry_id{};

  /*!
   * \brief Time to live
   */
  amsr::someip_protocol::internal::Ttl ttl{};

  /*!
   * \brief The unicast address of the server, method requests shall be sent to this address if configured to be sent
   * via UDP
   */
  ara::core::Optional<options::IpEndpointOption> udp_endpoint;

  /*!
   * \brief The unicast address of the server, method requests shall be sent to this address if configured to be sent
   * via TCP
   */
  ara::core::Optional<options::IpEndpointOption> tcp_endpoint;
};

/*!
 * \brief A container for OfferServiceEntry
 */
using OfferServiceEntryContainer = ara::core::Vector<OfferServiceEntry>;

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_OFFER_SERVICE_ENTRY_H_
