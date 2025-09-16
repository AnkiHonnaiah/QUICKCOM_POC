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
 *          \unit LogAndTrace::ConfigParser::SinksParser
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINKS_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINKS_PARSER_H_

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
#include "amsr/log/internal/config/v1/sink_parser.h"
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
class SinksParser final {
  /*! \brief The config to be parsed. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  SinkConfigMap config_{};
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
  explicit SinksParser(json::JsonParser& parser) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SinksParser() noexcept;
  /*!
   * \brief Allow moving of the SinksParser.
   * \spec
   *   requires true;
   * \endspec
   */
  SinksParser(SinksParser&&) noexcept;
  /*! \brief Do not allow copying of the SinksParser. */
  SinksParser(SinksParser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the SinksParser.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NL AutosarC++17_10-A15.5.1: MD_LOG_AutosarC++17_10-A15.5.1_explicit_noexcept
  auto operator=(SinksParser&&) & -> SinksParser&;
  /*! \brief Do not allow copying of the SinksParser. */
  auto operator=(SinksParser const&) & noexcept -> SinksParser& = delete;

  /*!
   * \brief Parses the complete file.
   * \return The parser result.
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
  auto GetConfig() && noexcept -> SinkConfigMap;
};
}  // namespace v1
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINKS_PARSER_H_
