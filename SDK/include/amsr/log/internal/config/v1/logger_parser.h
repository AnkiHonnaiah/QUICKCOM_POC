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
 *        \brief  Contains the definitions of the logger configuration parser.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_LOGGER_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_LOGGER_PARSER_H_

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
 * \vprivate Component private.
 */
class LoggerParser final {
  /*! \brief The config to be parsed. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  Ref<LoggerConfig> config_;
  /*! \brief The generic JSON parser. */
  Ref<json::JsonParser> parser_;

 public:
  /*!
   * \brief Constructor from a LoggingConfig reference.
   * \param data The JSON data to parse
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LoggerParser(json::JsonParser& parser, LoggerConfig& cfg) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~LoggerParser() noexcept;
  /*!
   * \brief Allow moving of the LoggerParser.
   * \spec
   *   requires true;
   * \endspec
   */
  LoggerParser(LoggerParser&&) noexcept;
  /*! \brief Do not allow copying of the LoggerParser. */
  LoggerParser(LoggerParser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the LoggerParser.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(LoggerParser&&) & noexcept -> LoggerParser&;
  /*! \brief Do not allow copying of the LoggerParser. */
  auto operator=(LoggerParser const&) & noexcept -> LoggerParser& = delete;

  /*!
   * \brief Parses the complete file.
   * \return The parser result.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Parse() & noexcept -> Result<void>;

 private:
  /*!
   * \brief Parses the context description.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseDescription() & noexcept -> Result<void>;
  /*!
   * \brief Parses the log level.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLevel() & noexcept -> Result<void>;
  /*!
   * \brief Parses the logging sink configs.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseSinks() & noexcept -> Result<void>;
  /*!
   * \brief Parses the log level of a logging sink config.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseSinkLevel(core::String const& sink_name) & noexcept -> Result<void>;
};
}  // namespace v1
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_LOGGER_PARSER_H_
