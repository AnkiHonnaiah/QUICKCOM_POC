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
/*!        \file  socal_parser.h
 *        \brief  JSON parser for Socal configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <set>

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader/v2/single_object_parser.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/configuration/generator_version_config.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief   Parser for the Socal JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration object.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class SocalParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   * \param[in]  json                      JSON document to parse.
   * \param[out] config                    A reference to the configuration object to be filled by the parser.
   * \param[out] generator_version_config  A reference to the generator version object to be filled by the parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  SocalParser(::amsr::json::JsonData& json, Configuration& config,
              GeneratorVersionConfig& generator_version_config) noexcept;

  SocalParser(SocalParser const&) noexcept = delete;
  SocalParser& operator=(SocalParser const&) & = delete;
  SocalParser(SocalParser&&) noexcept = delete;
  SocalParser& operator=(SocalParser&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SOCAL_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~SocalParser() noexcept override = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   * \param[in] json_key  Parsed key string.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final;

 private:
  /*!
   * \brief JSON key of the user-defined thread pools.
   */
  static constexpr ::amsr::core::StringView kThreadPoolsKey{"thread_pools"_sv};

  /*!
   * \brief JSON key of the P-Port default thread pool.
   */
  static constexpr ::amsr::core::StringView kPPortDefaultThreadPoolKey{"p_port_default_thread_pool"_sv};

  /*!
   * \brief JSON key of the R-Port default thread pool.
   */
  static constexpr ::amsr::core::StringView kRPortDefaultThreadPoolKey{"r_port_default_thread_pool"_sv};

  /*!
   * \brief JSON key for the generator version.
   */
  static constexpr ::amsr::core::StringView kGeneratorVersionKey{"generatorVersion"_sv};

  /*!
   * \brief JSON key for the service interfaces object.
   */
  static constexpr ::amsr::core::StringView kServiceInterfacesKey{"service_interfaces"_sv};

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return Result containing no value or an error.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ::amsr::core::Result<void> Finalize() noexcept final;

  /*!
   * \brief Parse user-defined thread pools.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult ParseThreadPools() noexcept;

  /*!
   * \brief Parse P-Port default thread pool.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult ParsePPortDefaultThreadPool() noexcept;

  /*!
   * \brief Parse R-Port default thread pool.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult ParseRPortDefaultThreadPool() noexcept;

  /*!
   * \brief Parse the generator version.
   * \return Parser result.
   * \error SomeIpBindingErrc::json_parsing_failure  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult ParseGeneratorVersion() noexcept;

  /*!
   * \brief Parse a single thread pool object.
   * \param[in,out] thread_pool_config  The to-be-parsed thread pool config object.
   * \return The empty result or an error.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> ParseThreadPoolObject(ThreadPoolConfig& thread_pool_config) noexcept;

  /*!
   * \brief Parse service interfaces object.
   * \return Parse result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ParserResult ParseServiceInterfaces() noexcept;

  /*!
   * \brief Parse service interface object.
   * \param[in,out] service_interfaces_config  The service interfaces config object to be filled by the parser.
   * \return Parse result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  SocalParser::ParserResult ParseServiceInterfaceObject(ServiceInterfacesConfig& service_interfaces_config) noexcept;

  /*!
   * \brief Map error code to Socal specific JSON parsing failure error code.
   * \param[in] error_code  ErrorCode to be mapped.
   * \return Mapped ErrorCode
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static ::amsr::core::ErrorCode MapToSocalError(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  Configuration& config_;

  /*!
   * \brief Reference to generator version struct filled by the parser.
   */
  GeneratorVersionConfig& generator_version_config_;

  /*!
   * \brief Thread pool id counter. Increased for each user defined thread pool.
   */
  PoolId user_defined_pool_id_counter_{1};

  /*!
   * \brief Maximum number of P-Port default thread pool containers found.
   */
  std::size_t number_of_p_port_default_thread_pool_found_{0};

  /*!
   * \brief Maximum number of R-Port default thread pool containers found.
   */
  std::size_t number_of_r_port_default_thread_pool_found_{0};

  /*!
   * \brief Maximum number of user-defined thread pool containers found.
   */
  std::size_t number_of_user_defined_thread_pool_found_{0};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PARSER_H_
