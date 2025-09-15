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
 *        \brief  Contains the configuration for logging.
 *
 *      \details
 *
 *         \unit  LogAndTrace::Logging::LoggingConfig
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGING_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGING_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader_fwd.h"
#include "amsr/log/internal/config/app_config.h"
#include "amsr/log/internal/config/logger_config.h"
#include "amsr/log/internal/config/sink_config.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_LOG_AutosarC++17_10-M7.3.4_using_namespace
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_namespace
/*! \brief Import time literals. */
using namespace std::chrono_literals;

/*!
 * \brief POD object for access to config fields
 *
 * \vprivate Component private.
 */
struct LoggingConfig {
  /*! \brief Contains general information like versions etc. */
  AppConfig general_config{};
  /*! \brief Contains per log sink configuration. */
  SinkConfigMap sink_config{};
  /*! \brief Contains per log configuration. */
  LoggerConfigMap logger_config{};
};

/*!
 * \brief Creates a Logging Config object from the passed JSON data.
 *
 * \param json The content of the JSON data.
 * \return     Either the constructed logging configuration object or the error that occurred.
 * \spec
 *   requires true;
 * \endspec
 */
auto CreateLoggingConfig(::amsr::json::JsonData&& json) noexcept -> Result<LoggingConfig>;

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOGGING_CONFIG_H_
