/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  compile_time_config.h
 *        \brief  Configuration file for logging.
 *
 *      \details  File should be generated in future
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMPILE_TIME_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMPILE_TIME_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/log_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;
/*!
 * \brief Sets the logging configuration file to this location
 *
 * \vprivate Component private.
 */
static constexpr vac::container::CStringView kLoggingConfigFile{"./etc/logging_config.json"_sv};

/*!
 * \brief   Sets the number of sinks that can be added during runtime
 *
 * \vprivate Component private.
 */
static constexpr std::size_t kMaxNumberOfLogSinks{16u};

/*!
 * \brief Sets the maximum log level at compile time
 *
 * \vprivate Component private.
 */
static constexpr LogLevel kMaximumLogLevel{LogLevel::kVerbose};

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMPILE_TIME_CONFIG_H_
