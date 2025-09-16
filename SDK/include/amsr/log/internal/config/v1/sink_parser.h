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
 *        \brief  Contains the parser for a single sink.
 *
 *      \details
 *
 *         \unit  LogAndTrace::ConfigParser::SinkParser
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINK_PARSER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINK_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader/v2.h"
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
/*! \brief Import time literals. */
using namespace std::chrono_literals;
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
/*!
 * \brief Parse the application config data.
 *
 * \vprivate Component private.
 */
class SinkParser final : public json::v2::SingleObjectParser {
  /*! \brief The config to be parsed. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  Ref<SinkConfig> config_;

 public:
  /*!
   * \brief Constructor from a LoggingConfig reference.
   * \param data The JSON data to parse
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SinkParser(json::JsonParser& parser, SinkConfig& cfg) noexcept;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~SinkParser() noexcept final = default;
  /*!
   * \brief Allow moving of the SinkParser.
   * \spec
   *   requires true;
   * \endspec
   */
  SinkParser(SinkParser&&) noexcept;
  /*! \brief Do not allow copying of the SinkParser. */
  SinkParser(SinkParser const&) noexcept = delete;
  /*!
   * \brief Allow moving of the SinkParser.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(SinkParser&&) & noexcept -> SinkParser&;
  /*! \brief Do not allow copying of the SinkParser. */
  auto operator=(SinkParser const&) & noexcept -> SinkParser& = delete;

  /*!
   * \brief Parse bools in the JSON file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnBool(bool value) noexcept -> json::ParserResult final;

  /*!
   * \brief Parse numbers in the JSON file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnNumber(json::JsonNumber num) noexcept -> json::ParserResult final;

  /*!
   * \brief Dummy implementation so that parser does not break down.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnKey(core::StringView) noexcept -> json::ParserResult final {
    return json::ParserResult{json::ParserState::kRunning};
  }

  /*!
   * \brief Parse strings in the JSON file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnString(core::StringView view) noexcept -> json::ParserResult final;

 private:
  /*!
   * \brief Parse normal strings in the JSON file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnRegularString(core::StringView view) noexcept -> json::ParserResult;

  /*!
   * \brief Parse log level strings in the JSON file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnLogLevel(core::StringView view) noexcept -> json::ParserResult;

  /*!
   * \brief   Tries to add a key value pair.
   *
   * \tparam  T  The type to construct a ContentData.
   *             Must be a type that ContentData is constructible from.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  template <typename T>
  auto Emplace(core::StringView key, T&& value, char const* message) noexcept -> json::ParserResult {
    // VCA_LOGAPI_VALID_OBJECT
    std::pair<IterMut<SinkConfig>, bool> it{this->config_->emplace(std::piecewise_construct,
                                                                   std::forward_as_tuple(core::String{key}),
                                                                   std::forward_as_tuple(std::forward<T>(value)))};
    return json::ParserResult{json::ParserState::kRunning}.Filter([&it](json::ParserState) { return it.second; },
                                                                  Err(LogErrc::kInitializationFailed, message));
  }
};
}  // namespace v1
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_V1_SINK_PARSER_H_
