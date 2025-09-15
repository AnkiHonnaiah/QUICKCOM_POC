/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  JSON parser of services configurations.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_array_parser.h"
#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config.h"
#include "amsr/comtrace/internal/config/field_config.h"
#include "amsr/comtrace/internal/config/method_config.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"
#include "amsr/comtrace/internal/config/service_config.h"
#include "amsr/comtrace/internal/config/trace_config.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief Parser for the services element inside the trace configuration.
 *
 * \unit       ComTrace::TraceConfig
 * \complexity Interaction with outer parsers required.
 */
class ServicesJsonParser final : public ::amsr::json::v2::SingleArrayParser {
 public:
  /*!
   * \brief Name alias for TraceConfig.
   */
  using TraceConfig = ::amsr::comtrace::internal::config::TraceConfig;

  /*!
   * \brief Constructor for ServicesJsonParser.
   *
   * \param[in,out] json    The loaded JSON data from the configuration file.
   * \param[in,out] config  The object holding the parsed configuration.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ServicesJsonParser(JsonData& json, TraceConfig& config) noexcept;

  ServicesJsonParser(ServicesJsonParser const&) noexcept = delete;
  auto operator=(ServicesJsonParser const&) noexcept -> ServicesJsonParser& = delete;
  ServicesJsonParser(ServicesJsonParser&&) noexcept = delete;
  auto operator=(ServicesJsonParser&&) noexcept -> ServicesJsonParser& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServicesJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

 protected:
  /*!
   * \brief Callback on a JSON array element encounter from the parsed JSON object.
   *
   * \return Parser result.
   *
   * \error ComTraceErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnElement() noexcept -> ParserResult final;

 private:
  /*!
   * \brief Parser for a single service interface configuration.
   */
  class ServiceJsonParser final : public ::amsr::json::v2::SingleObjectParser {
   public:
    /*!  Name alias for ServiceConfig. */
    using ServiceConfig = ::amsr::comtrace::internal::config::ServiceConfig;
    /*!  Name alias for EventConfig. */
    using EventConfig = ::amsr::comtrace::internal::config::EventConfig;
    /*!  Name alias for FieldConfig. */
    using FieldConfig = ::amsr::comtrace::internal::config::FieldConfig;
    /*!  Name alias for MethodConfig. */
    using MethodConfig = ::amsr::comtrace::internal::config::MethodConfig;

    /*!
     * \brief Constructor for ServiceJsonParser.
     *
     * \param[in,out] json    The loaded JSON data from the configuration file.
     * \param[in,out] config  Reference to the service interface config.
     *
     * \pre     -
     * \context Init
     * \steady  FALSE
     */
    ServiceJsonParser(JsonData& json, ServiceConfig& config) noexcept;

    ServiceJsonParser(ServiceJsonParser const&) noexcept = delete;
    auto operator=(ServiceJsonParser const&) noexcept -> ServiceJsonParser& = delete;
    ServiceJsonParser(ServiceJsonParser&&) noexcept = delete;
    auto operator=(ServiceJsonParser&&) noexcept -> ServiceJsonParser& = delete;

    /*!
     * \brief Default destructor.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    ~ServiceJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

   protected:
    /*!
     * \brief Callback on a new key parsed from the JSON object.
     *
     * \param[in] json_key  Parsed key string.
     *
     * \return Parser result.
     *
     * \error ComTraceErrc::json_parsing_failure  If JSON parsing failed.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnKey(StringView json_key) noexcept -> ParserResult final;

    /*!
     * \brief Callback to validate that all configuration elements have been accepted.
     *
     * \return Either nothing or the error.
     *
     * \error ComTraceErrc::json_parsing_failure  If validation fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto Finalize() noexcept -> ara::core::Result<void> final;

   private:
    /*! JSON key for service identifier. */
    static constexpr StringView kServiceIdentifierKey{"service_id"_sv};
    /*! JSON key for shortname path. */
    static constexpr StringView kShortnamePathKey{"shortname_path"_sv};
    /*! JSON key for event deployments. */
    static constexpr StringView kEventsKey{"events"_sv};
    /*! JSON key for method deployments. */
    static constexpr StringView kMethodsKey{"methods"_sv};
    /*! JSON key for field deployments. */
    static constexpr StringView kFieldsKey{"fields"_sv};

    std::size_t number_of_shortname_path_found_{0}; /*!< Number of key 'shortname_path' found. */
    std::size_t number_of_events_found_{0};         /*!< Number of key 'events' found. */
    std::size_t number_of_methods_found_{0};        /*!< Number of key 'methods' found. */
    std::size_t number_of_fields_found_{0};         /*!< Number of key 'fields' found. */

    /*! Reference to the service interface config. */
    ServiceConfig& config_;
  };

  /*! Reference for the TraceConfig configuration object. */
  TraceConfig& trace_config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_
