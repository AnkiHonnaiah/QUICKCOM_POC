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
/*!        \file
 *        \brief  Helper functions for parsed Thread Configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_UTILITIES_THREAD_CONFIG_UTILITIES_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_UTILITIES_THREAD_CONFIG_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/thread/thread_config_types.h"

namespace amsr {
namespace thread {
namespace internal {
namespace utilities {

/*!
 * \brief           Definition of AMSR_THREAD_CONFIGURABLE_SETTINGS env variable states.
 */
enum class ThreadConfigurableSettingsEnvVarStates : std::uint8_t {
  /*!
   * \brief           Enabled
   */
  kEnabled = 1,
  /*!
   * \brief           Disabled
   */
  kDisabled = 2,
  /*!
   * \brief           Invalid
   */
  kInvalid = 3,
};

/*!
 * \brief           Parsing logic for thread_config.json file.
 *
 * \return          Empty result if parsing is successful, error otherwise.
 *
 * \error           ThreadErrc::kThreadConfigValidationFailed if integrity check is enabled
 *                  and validation of thread_config.json failed.
 * \error           ThreadErrc::kThreadConfigJsonFileOpenFailed if openeing
 *                  thread_config.json failed.
 * \error           ThreadErrc::kThreadConfigJsonFileParsingFailed if parsing
 *                  thread_config.json failed.
 * \error           ThreadErrc::kInsufficientResources if configured instances inside
 *                  thread_config.json are more than kMaxThreadConfigs(128).
 *
 * \context         InitPhase
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 *
 * \trace           DSGN-VectorThreadLibrary-ThreadConfigurationParsing
 * \trace           DSGN-VectorThreadLibrary-ThreadConfiguration
 */
amsr::core::Result<void> ParseThreadConfiguration() noexcept;

/*!
 * \brief           Searches the environment list to find the state of AMSR_THREAD_CONFIGURABLE_SETTINGS.
 *
 * \return          State of AMSR_THREAD_CONFIGURABLE_SETTINGS env variable.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
ThreadConfigurableSettingsEnvVarStates GetThreadConfigurableSettingsEnvVar() noexcept;

}  // namespace utilities
}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_UTILITIES_THREAD_CONFIG_UTILITIES_H_
