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
 *        \brief  Contains the declaration of the top level configuration parser.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONFIG_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONFIG_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/json/reader_fwd.h"
#include "amsr/log/internal/config/logging_config.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_LOG_AutosarC++17_10-M7.3.4_using_namespace
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_namespace
/*! \brief Import time literals. */
using namespace std::chrono_literals;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
/*!
 * \brief SAX parser class implementing the actual logging.
 *
 * \vprivate Component private.
 */
class ConfigParser final {
  /*! \brief The parser implementation. */
  std::unique_ptr<json::JsonParser> parser_;

 public:
  /*!
   * \brief Constructor from a JSON data reference.
   * \param data The JSON data to parse
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ConfigParser(json::JsonData& data) noexcept;
  /*!
   * \brief Explicit destructor needed to use forward declaration.
   * \spec
   *   requires true;
   * \endspec
   */
  ~ConfigParser() noexcept;
  /*!
   * \brief Allow moving of the ConfigParser.
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigParser(ConfigParser&&) noexcept;
  /*!
   * \brief Allow moving of the ConfigParser.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(ConfigParser&&) & noexcept -> ConfigParser&;
  /*! \brief Do not allow copying of the ConfigParser. */
  ConfigParser(ConfigParser const&) noexcept = delete;
  /*! \brief Do not allow copying of the ConfigParser. */
  auto operator=(ConfigParser const&) & noexcept -> ConfigParser& = delete;

  /*!
   * \brief Gets the completely parsed configuration object.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Parse() && noexcept -> Result<LoggingConfig>;
};
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONFIG_PARSER_H_
