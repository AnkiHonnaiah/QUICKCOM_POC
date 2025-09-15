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
/**        \file  initial_sd_delay_config.h
 *        \brief  Initial SD Delay Config
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_INITIAL_SD_DELAY_CONFIG_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_INITIAL_SD_DELAY_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Type-alias for 'Maximum number of repetitions in Repetition Phase'
 */
using InitialRepetitionsMax = std::uint32_t;

/*!
 * \brief initial SD delay configuration.
 *        Used to define initial wait phase, repetition phase for server or client SM.
 */
struct InitialSdDelayConfig {
  /*!
   * \brief The minimum number of repetitions in the repetition phase.
   *        Default value: zero (no minimum delay)
   */
  std::chrono::nanoseconds initial_delay_min{std::chrono::nanoseconds::zero()};

  /*!
   * \brief The maximum delay in the FindService entry.
   *        Default value: zero (no initial delay, send first find service message immediately)
   */
  std::chrono::nanoseconds initial_delay_max{std::chrono::nanoseconds::zero()};

  /*!
   * \brief The maximum number of repetitions in the repetition phase.
   *        Default value: optional not set = no repetition phase.
   */
  ::ara::core::Optional<InitialRepetitionsMax> initial_repetitions_max{};

  /*!
   * \brief The base delay in the FindService entry during repetition phase.
   *        Default value: optional not set = no repetition phase.
   */
  ::ara::core::Optional<std::chrono::nanoseconds> initial_repetitions_base_delay{};
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_INITIAL_SD_DELAY_CONFIG_H_
