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
/**        \file  service_discovery_message.h
 *        \brief  SD message in wire format
 *
 *      \details  This data structure contains a Service Discovery message in wire format
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * \brief SOME/IP SD Message.
 *
 * \trace SPEC-4981485
 * \trace SPEC-4981486
 */
struct ServiceDiscoveryMessage {
  /*!
   * \brief SOME/IP message header
   */
  amsr::someip_protocol::internal::SomeIpMessageHeader someip_header_{};

  // SD header
  /*!
   * \brief Service discovery flags
   */
  amsr::someip_protocol::internal::SdFlag flags_{};

  /*!
   * \brief Service discovery entries.
   */
  entries::ServiceDiscoveryEntryContainer entries_{};

  /*!
   * \brief Service discovery options.
   */
  options::ServiceDiscoveryOptionContainer options_{};

  /*!
   * \brief Service discovery payload.
   */
  message::ServiceDiscoveryMessagePayload payload_{};
};

/*!
 * \brief A container for ServiceDiscoveryMessage
 */
using ServiceDiscoveryMessageContainer = ara::core::Vector<ServiceDiscoveryMessage>;

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_H_
