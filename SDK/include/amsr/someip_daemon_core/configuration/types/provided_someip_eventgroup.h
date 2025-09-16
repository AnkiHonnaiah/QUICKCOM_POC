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
/**        \file  provided_someip_eventgroup.h
 *         \brief Datatype containing all events of an eventgroup, an optional multicast configuration for the
 *                eventgroup, and information containing whether the eventgroup has one field event, tcp event or udp
 *                event.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_SOMEIP_EVENTGROUP_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_SOMEIP_EVENTGROUP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/multicast_eventgroup_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 *\brief Alias of a map of SOME/IP event deployments by event id.
 */
using ProvidedSomeIpEventMap =
    ara::core::Map<someip_protocol::internal::EventId, configuration::types::SomeIpEventDeployment>;

struct ProvidedSomeIpEventgroup {
  /*!
   * \brief A map to all the events within the eventgroup by Event Id.
   */
  ProvidedSomeIpEventMap const events;

  /*!
   * \brief True if there is at least one event within the eventgroup that uses TCP. Used to reduce amount of map
   *        iterations during runtime.
   */
  bool const contains_tcp_event;

  /*!
   * \brief True if there is at least one event within the eventgroup that uses UDP. Used to reduce amount of map
   *        iterations during runtime.
   */
  bool const contains_udp_event;

  /*!
   * \brief True if there is at least one field within the eventgroup. Used to reduce amount of map iterations during
   *        runtime.
   */
  bool const contains_field;

  /*!
   * \brief Multicast eventgroup configuration.
   */
  ara::core::Optional<MulticastEventgroupConfig> multicast_eventgroup_config;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_SOMEIP_EVENTGROUP_H_
