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
 *        \brief  Contains the declarations of v0 configuration parser.
 *
 *      \details
 *
 *         \unit  LogAndTrace::ConfigParser::V0Parser
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V0_V0_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V0_V0_PARSER_H_

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
namespace v0 {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_LOG_AutosarC++17_10-M7.3.4_using_namespace
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_namespace
/*! Import time literals */
using namespace std::chrono_literals;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
/*!
 * \brief SAX parser class implementing the actual logging.
 *
 * \vprivate Component private.
 */
class V0Parser final {
  /*! \brief The config to be parsed. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  LoggingConfig config_{};
  /*! \brief The parser implementation. */
  Ref<json::JsonParser> parser_;
  /*! \brief Validation for the application name element. */
  std::uint8_t name_found_{0};
  /*! \brief Validation for the application description element. */
  std::uint8_t desc_found_{0};
  /*! \brief Validation for the LogMode element. */
  std::uint8_t log_mode_found_{0};
  /*! \brief Validation for the LogLevel element. */
  std::uint8_t log_level_found_{0};
  /*! \brief Validation for the log dir element. */
  std::uint8_t log_dir_found_{0};
  /*! \brief Validation for the log file element. */
  std::uint8_t log_file_found_{0};
  /*! \brief Validation for the application ID element. */
  std::uint8_t app_id_found_{0};
  /*! \brief Validation for the IPC port element. */
  std::uint8_t ipc_port_found_{0};
  /*! \brief Validation for the message buffer size element. */
  std::uint8_t msg_buf_size_found_{0};
  /*! \brief Validation for the max loggers element. */
  std::uint8_t max_loggers_found_{0};

  /*! \brief path to directory of log files (legacy). */
  core::String log_dir_path_{};

 public:
  /*!
   * \brief Constructor from a JSON data reference.
   * \param parser The JSON parser to use.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit V0Parser(json::JsonParser& parser) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~V0Parser() noexcept;
  /*!
   * \brief Allow moving of the V0Parser.
   * \spec
   *   requires true;
   * \endspec
   */
  V0Parser(V0Parser&&) noexcept;
  /*! \brief Do not allow copying of the V0Parser. */
  V0Parser(V0Parser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the V0Parser.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NL AutosarC++17_10-A15.5.1: MD_LOG_AutosarC++17_10-A15.5.1_explicit_noexcept
  auto operator=(V0Parser&&) & -> V0Parser&;
  /*! \brief Do not allow copying of the V0Parser. */
  auto operator=(V0Parser const&) & noexcept -> V0Parser& = delete;

  /*!
   * \brief Gets the completely parsed configuration object.
   * \param initial_key The first key of the object that has already been parsed.
   * \return The constructed logging config or an error.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseConfig(core::StringView initial_key) && noexcept -> Result<LoggingConfig>;

 private:
  /*!
   * \brief Parses the complete file.
   * \param initial_key The first key of the object that has already been parsed.
   * \return The parser result.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Parse(core::StringView initial_key) noexcept -> Result<void>;

  /*!
   * \brief Sets the sink defaults.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSinkDefaults() noexcept;

  /*!
   * \brief Parses the process name.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseProcessName() noexcept -> Result<void>;

  /*!
   * \brief Parses the application description.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseApplicationDescription() noexcept -> Result<void>;

  /*!
   * \brief Parses the DLT application id.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseApplicationId() noexcept -> Result<void>;

  /*!
   * \brief Parses the log modes.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLogMode() noexcept -> Result<void>;

  /*!
   * \brief Parses the log level.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLogLevel() noexcept -> Result<void>;

  /*!
   * \brief Parses the logging directory.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLogDirPath() noexcept -> Result<void>;

  /*!
   * \brief Parses the log file path.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseLogFilePath() noexcept -> Result<void>;

  /*!
   * \brief Parses the IPC port to connect to.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseIpcPort() noexcept -> Result<void>;

  /*!
   * \brief Parses the reconnection retry offset.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseReconnectRetryOffset() noexcept -> Result<void>;

  /*!
   * \brief Parses the IPC message buffer.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseMessageBufferSize() noexcept -> Result<void>;

  /*!
   * \brief Parses the maximum number of loggers.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseMaxLoggers() noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed configuration.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Validate() noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed global configuration elements.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ValidateGlobal() const noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed console configuration elements.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ValidateConsole() const noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed file configuration elements.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ValidateFile() noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed remote/DLT configuration elements.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ValidateRemote() const noexcept -> Result<void>;

  /*!
   * \brief Validates the parsed Syslog configuration elements.
   * \spec
   *   requires true;
   * \endspec
   */
  auto ValidateSyslog() const noexcept -> Result<void>;
};
}  // namespace v0
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V0_V0_PARSER_H_
