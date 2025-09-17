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
 *        \brief  Contains all configuration elements that are generic.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_APP_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_APP_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/log/clock_type.h"
#include "amsr/log/log_level.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

/*! \brief Contains all application configuration. */
// VCA_LOGAPI_MOLE_1298
struct AppConfig {
  /*! \brief The name of the application. */
  core::String name{};
  /*! \brief The id of the application. */
  core::String id{};
  /*! \brief A description of the application. */
  core::String description{};
  /*! \brief The version of the application. */
  core::String version{};
  /*! \brief The global default log level. */
  LogLevel log_level{LogLevel::kOff};
  /*! \brief Max number of loggers. */
  std::uint32_t max_loggers{0};
  /*! \brief The IPC port to communicate with LogDaemon. */
  core::Optional<ipc::Port> ipc_port{};
  /*! \brief The clock type to be used for timestamps. */
  ClockType clock_type{ClockType::kDefault};
  /*! \brief Enables configurability & starts connection to LogDaemon. */
  bool enable_log_config_requests{false};
};

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_APP_CONFIG_H_
