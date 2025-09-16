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
/**        \file  someip_sd_client_service_instance_config.h
 *        \brief  SOME/IP SD client service instance config
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_SERVICE_INSTANCE_CONFIG_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_SERVICE_INSTANCE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Client specific settings that are relevant for the configuration of SOME/IP Service-Discovery.
 */
struct SomeipSdClientServiceInstanceConfig {
  /*!
   * \brief Controls initial find behavior of clients.
   *        If not present (Default), initial delay shall be set to 0 and repetition phase shall be skipped.
   */
  ::ara::core::Optional<InitialSdDelayConfig> sd_config{};

  /*!
   * \brief The time to live (in seconds) in the FindService entry.
   *        Note: This value is only written in FindService entries (as required by the protocol),
   *        but has really no usage on the server side. It might be removed in the future PRS versions.
   */
  ::amsr::someip_protocol::internal::Ttl ttl{};
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SD_CLIENT_SERVICE_INSTANCE_CONFIG_H_
