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
/**        \file  service_discovery_message_payload.h
 *        \brief  Payload of a SD message (entries and options)
 *
 *      \details  This structure wraps a container of entries and options for a single Service Discovery message
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_PAYLOAD_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_PAYLOAD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * \brief SOME/IP SD Message.
 */
struct ServiceDiscoveryMessagePayload {
  /*!
   * \brief Service discovery entries.
   */
  entries::ServiceDiscoveryEntryContainer entries{};
  /*!
   * \brief Service discovery options.
   */
  options::ServiceDiscoveryOptionContainer options{};
};

/*!
 * \brief A container for ServiceDiscoveryMessagePayload
 */
using ServiceDiscoveryMessagePayloadContainer = ara::core::Vector<ServiceDiscoveryMessagePayload>;

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_PAYLOAD_H_
