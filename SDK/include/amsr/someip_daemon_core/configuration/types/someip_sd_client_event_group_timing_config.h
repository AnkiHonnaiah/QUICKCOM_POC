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
/**        \file  someip_sd_client_event_group_timing_config.h
 *        \brief  SOME/IP SD Client eventgroup timing config
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_EVENT_GROUP_TIMING_CONFIG_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_EVENT_GROUP_TIMING_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/request_response_delay.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Type-alias for the SOME/IP delay of retrying subscription of an eventgroup.
 */
using SubscribeEventgroupRetryDelay = ::amsr::someip_protocol::internal::SubscribeEventgroupRetryDelay;

/*!
 * \brief Type-alias for the SOME/IP subscribe eventgroup maximum retries.
 */
using SubscribeEventgroupRetryMax = ::amsr::someip_protocol::internal::SubscribeEventgroupRetryMax;

/*!
 * This meta-class is used to specify configuration related to service discovery in the context of eventgroup
 * re-subscription
 */
struct SomeIpSdClientEventGroupSubscriptionRetrialConfig {
  /*!
   * \brief Defines the interval in milliseconds to re-trigger a subscription to an Eventgroup, if a retry to subscribe
   * to an Eventgroup is configured.
   */
  SubscribeEventgroupRetryDelay subscribe_eventgroup_retry_delay_ms;

  /*!
   * \brief Defines the maximum counts of retries to subscribe to an eventgroup.
   */
  SubscribeEventgroupRetryMax subscribe_eventgroup_retry_max;
};

/*!
 * This meta-class is used to specify configuration related to service discovery in the context of an eventgroup on
 * SOME/IP.
 */
struct SomeipSdClientEventGroupTimingConfig {
  /*!
   * \brief The Service Discovery shall delay answers to unicast messages triggered by multicast messages (e.g.
   *        Subscribe Eventgroup after Offer Service).
   *        If not present, the answers shall be immediately sent.
   */
  ::ara::core::Optional<RequestResponseDelay> request_response_delay;

  /*!
   * \brief Defines an optional configuration for eventgroup subscription retrial.
   */
  ::ara::core::Optional<SomeIpSdClientEventGroupSubscriptionRetrialConfig> subscribe_eventgroup_retry_config;

  /*!
   * \brief Defines the time in seconds the subscription of this event is expected by the client.
   *        This value is sent from the client to the server in the SD-subscribeEvent message.
   */
  ::amsr::someip_protocol::internal::Ttl ttl{};
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_EVENT_GROUP_TIMING_CONFIG_H_
