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
 *        \brief  Contains the v1 application configuration parser.
 *
 *      \details
 *
 *         \unit  LogAndTrace::ConfigParser::V1Parser
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_V1_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_V1_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/json/reader_fwd.h"
#include "amsr/log/internal/config/logging_config.h"
#include "amsr/log/internal/config/v1/app_parser.h"
#include "amsr/log/internal/config/v1/loggers_parser.h"
#include "amsr/log/internal/config/v1/sinks_parser.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/ref.h"

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
class V1Parser final {
  /*! \brief The generic JSON parser. */
  Ref<json::JsonParser> parser_;
  /*! \brief The 'app' parser. */
  AppParser app_parser_;
  /*! \brief The 'sinks' parser. */
  SinksParser sinks_parser_;
  /*! \brief The 'loggers' parser. */
  LoggersParser logger_parser_;

 public:
  /*!
   * \brief Constructor from a LoggingConfig reference.
   * \param data The JSON data to parse
   * \spec
   *   requires true;
   * \endspec
   */
  explicit V1Parser(json::JsonParser& parser) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~V1Parser() noexcept;
  /*!
   * \brief Allow moving of the V1Parser.
   * \spec
   *   requires true;
   * \endspec
   */
  V1Parser(V1Parser&&) noexcept;
  /*! \brief Do not allow copying of the V1Parser. */
  V1Parser(V1Parser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the V1Parser.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NL AutosarC++17_10-A15.5.1: MD_LOG_AutosarC++17_10-A15.5.1_explicit_noexcept
  auto operator=(V1Parser&&) & -> V1Parser&;
  /*! \brief Do not allow copying of the V1Parser. */
  auto operator=(V1Parser const&) & noexcept -> V1Parser& = delete;

  /*!
   * \brief Parse the whole configuration object.
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
  auto GetConfig() && noexcept -> LoggingConfig;
};
}  // namespace v1
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_V1_PARSER_H_
