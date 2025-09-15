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
/*!        \file
 *        \brief  Contains all configuration elements that are dependent on a specific logger.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGER_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGER_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/log/internal/map.h"
#include "amsr/log/log_level.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

/*! \brief Contains the log level information per sink. */
using LoggerSinkMap = Map<core::String, LogLevel>;

/*! \brief Contains all logger specifict information. */
// VCA_LOGAPI_MOLE_1298
struct LoggerConfig {
  /*! \brief A description of the logger. */
  core::String description{};
  /*! \brief The log level for that logger. */
  core::Optional<LogLevel> log_level{};
  /*! \brief Contains the log levels per sink. */
  LoggerSinkMap sinks{};
};

/*!
 * \brief   The container for all logger configurations.
 * \details Maps a context name/id to its config.
 */
using LoggerConfigMap = Map<core::String, LoggerConfig>;

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGER_CONFIG_H_
