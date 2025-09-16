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
/**        \file  multicast_eventgroup_config.h
 *        \brief  Multicast configuration of an eventgroup.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_MULTICAST_EVENTGROUP_CONFIG_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_MULTICAST_EVENTGROUP_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/service_address.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Multicast eventgroup configuration
 */
struct MulticastEventgroupConfig {
  /*!
   * \brief Subscribers threshold to the eventgroup.
   */
  std::uint32_t multicast_threshold;

  /*!
   * \brief Multicast address.
   */
  amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress multicast_address;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_MULTICAST_EVENTGROUP_CONFIG_H_
