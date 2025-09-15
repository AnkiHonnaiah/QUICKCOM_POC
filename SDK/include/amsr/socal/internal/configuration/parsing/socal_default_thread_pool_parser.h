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
/*!        \file  socal_default_thread_pool_parser.h
 *        \brief  JSON parser for Socal default ThreadPools configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_DEFAULT_THREAD_POOL_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_DEFAULT_THREAD_POOL_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief   Parser for the Socal P-Port DefaultThreadPool and R-Port DefaultThreadPool JSON configurations.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration object.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class SocalDefaultThreadPoolParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief Enumeration type to indicate the type of default thread-pool for which the configuration is being parsed.
   */
  enum class DefaultThreadPoolType : std::uint8_t {
    kPPortDefaultThreadPool,  // Default thread pool type is P-Port.
    kRPortDefaultThreadPool   // Default thread pool type is R-Port.
  };

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   * \param[in]  json              JSON document to parse.
   * \param[out] config            A reference to the configuration object to be filled by the parser.
   * \param[in]  thread_pool_type  Type of the default thread pool (P-Port or R-Port).
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit SocalDefaultThreadPoolParser(::amsr::json::JsonData& json, ThreadPoolConfig& config,
                                        DefaultThreadPoolType const thread_pool_type) noexcept;

  SocalDefaultThreadPoolParser(SocalDefaultThreadPoolParser const&) noexcept = delete;
  SocalDefaultThreadPoolParser& operator=(SocalDefaultThreadPoolParser const&) & = delete;
  SocalDefaultThreadPoolParser(SocalDefaultThreadPoolParser&&) noexcept = delete;
  SocalDefaultThreadPoolParser& operator=(SocalDefaultThreadPoolParser&&) & = delete;

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
  ~SocalDefaultThreadPoolParser() noexcept final = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return either nothing or the error.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> Finalize() noexcept final;

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   * \param[in] json_key  Parsed key string.
   * \return ParserResult with ParserState::kRunning or an error code is returned.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final;

  /*!
   * \brief Callback if an unexpected event happened during JSON parsing.
   * \return Always a parser result with related error code.
   * \error ComErrc::kJsonParsingFailed  In any case.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ParserResult OnUnexpectedEvent() noexcept final;

 private:
  /*!
   * \brief JSON key for the maximum number of tasks.
   */
  static constexpr vac::container::CStringView kMaxNumberOfTasksKey{"max_number_of_tasks"_sv};

  /*!
   * \brief JSON key for the referred thread configuration identifier.
   */
  static constexpr vac::container::CStringView kThreadConfigKey{"bsw_thread_configuration"_sv};

  /*!
   * \brief Parse the maximum number of tasks.
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
  ParserResult ParseMaxNumberOfTasks() noexcept;

  /*!
   * \brief Parse the bsw thread configuration short-name.
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
  ParserResult ParseBswThreadConfiguration() noexcept;

  /*!
   * \brief Initialize the attributes of the default thread-pool which are not configurable.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void InitializeNonConfigurableAttributes() noexcept;

  /*!
   * \brief Map error code to Socal specific JSON parsing failure error code.
   * \param[in] error_code  ErrorCode to be mapped.
   * \return Mapped ErrorCode
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static ::amsr::core::ErrorCode MapToSocalError(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Maximum number of tasks type found.
   */
  std::size_t max_number_of_tasks_found_{0};

  /*!
   * \brief Number of threads configurations found.
   */
  std::size_t number_of_bsw_thread_configuration_found_{0};

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  ThreadPoolConfig& config_;

  /*!
   * \brief The type of the default thread-pool for which the configuration is parsed.
   */
  DefaultThreadPoolType const default_thread_pool_type_{};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_DEFAULT_THREAD_POOL_PARSER_H_
