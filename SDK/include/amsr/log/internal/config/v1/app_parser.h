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
 *        \brief  Contains the application configuration parser.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_APP_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_APP_PARSER_H_

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
#include "amsr/log/clock_type.h"
#include "amsr/log/internal/config/logging_config.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {
namespace v1 {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_LOG_AutosarC++17_10-M7.3.4_using_namespace
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_namespace
/*!
 * Import time literals
 */
using namespace std::chrono_literals;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
/*!
 * \brief Parse the application config data.
 *
 * \unit LogAndTrace::ConfigParser::AppParser
 *
 * \vprivate Component private.
 */
class AppParser final {
  /*! \brief The config to be parsed. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  AppConfig config_{};
  /*! \brief The generic JSON parser. */
  Ref<json::JsonParser> parser_;
  /*! \brief Validation for the application name element. */
  std::uint8_t name_found_{0};
  /*! \brief Validation for the application id element. */
  std::uint8_t id_found_{0};
  /*! \brief Lower bound length for valid application ids. */
  std::uint8_t lower_bound_app_id_{1};
  /*! \brief Upper bound length for valid application ids. */
  std::uint8_t upper_bound_app_id_{4};
  /*! \brief Validation for the application description element. */
  std::uint8_t desc_found_{0};
  /*! \brief Validation for the LogLevel element. */
  std::uint8_t log_level_found_{0};
  /*! \brief Validation for the ipc port element. */
  std::uint8_t ipc_port_found_{0};
  /*! \brief Validation for the clock element. */
  std::uint8_t timestamp_clock_found_{0};

 public:
  /*!
   * \brief Constructor from a LoggingConfig reference.
   * \param data The JSON data to parse
   */
  explicit AppParser(json::JsonParser& parser) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~AppParser() noexcept;
  /*!
   * \brief Allow moving of the AppParser.
   * \spec
   *   requires true;
   * \endspec
   */
  AppParser(AppParser&&) noexcept;
  /*! \brief Do not allow copying of the AppParser. */
  AppParser(AppParser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the AppParser.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(AppParser&&) & noexcept -> AppParser&;
  /*! \brief Do not allow copying of the AppParser. */
  auto operator=(AppParser const&) & noexcept -> AppParser& = delete;

  /*!
   * \brief Parse the whole app configuration.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Parse() & noexcept -> Result<void>;
  /*!
   * \brief Extracts the parsed configuration.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetConfig() && noexcept -> AppConfig;

  /*!
   * \brief Parses the process name.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseProcessName() noexcept -> Result<void>;

  /*!
   * \brief Parses the application ID.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseAppId() noexcept -> Result<void>;

  /*!
   * \brief Parses the application description.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseApplicationDescription() noexcept -> Result<void>;

  /*!
   * \brief Parses the version.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseVersion() noexcept -> Result<void>;

  /*!
   * \brief Parses the log level.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLogLevel() noexcept -> Result<void>;

  /*!
   * \brief Parses the maximum number of loggers.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseMaxLoggers() noexcept -> Result<void>;

  /*!
   * \brief Parses the ipc port to use.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseIpcPort() noexcept -> Result<void>;

  /*!
   * \brief Parses the timestamp clock to use.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseTimestampClock() noexcept -> Result<void>;

  /*!
   * \brief Parses the setting for the enable-log-config-requests.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseEnableLogConfigRequests() noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed configuration.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Validate() const noexcept -> Result<void>;
};
}  // namespace v1
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_APP_PARSER_H_
