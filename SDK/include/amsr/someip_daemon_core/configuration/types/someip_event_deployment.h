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
/**        \file  someip_event_deployment.h
 *        \brief  SomeIp Event Deployment
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_EVENT_DEPLOYMENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_EVENT_DEPLOYMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/transport_protocol.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief SOME/IP configuration settings for an Event.
 */
struct SomeIpEventDeployment {
  /*!
   * \brief Unique Identifier within a ServiceInterface that identifies the Event in SOME/IP.
   *        This Identifier is sent as part of the Message ID in SOME/IP messages.
   */
  ::amsr::someip_protocol::internal::EventId event_id;

  /*!
   * \brief This attribute defines over which Transport Layer Protocol this event is intended to be sent.
   */
  TransportProtocol transport_protocol;

  /*!
   * \brief Indication of whether this event is a field notifier
   */
  bool is_field;

  /*!
   * \brief Indication of whether this event is a PDU (signal based)
   */
  bool is_signal_based;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_EVENT_DEPLOYMENT_H_
