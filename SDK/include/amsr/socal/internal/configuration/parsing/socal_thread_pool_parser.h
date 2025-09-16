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
/*!        \file  socal_thread_pool_parser.h
 *        \brief  JSON parser for Socal ThreadPool configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_THREAD_POOL_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_THREAD_POOL_PARSER_H_

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
 * \brief   Parser for the Socal user-defined ThreadPool JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration object.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class SocalThreadPoolParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   * \param[in]  json    JSON document to parse.
   * \param[out] config  A reference to the configuration object to be filled by the parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit SocalThreadPoolParser(::amsr::json::JsonData& json, ThreadPoolConfig& config) noexcept;

  SocalThreadPoolParser(SocalThreadPoolParser const&) noexcept = delete;
  SocalThreadPoolParser& operator=(SocalThreadPoolParser const&) & = delete;
  SocalThreadPoolParser(SocalThreadPoolParser&&) noexcept = delete;
  SocalThreadPoolParser& operator=(SocalThreadPoolParser&&) & = delete;

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
  ~SocalThreadPoolParser() noexcept final = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

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

 private:
  /*!
   * \brief JSON key for the number of threads.
   */
  static constexpr vac::container::CStringView kNumberOfThreadsKey{"number_of_threads"_sv};

  /*!
   * \brief JSON key for the maximum number of tasks.
   */
  static constexpr vac::container::CStringView kMaxNumberOfTasksKey{"max_number_of_tasks"_sv};

  /*!
   * \brief JSON key for the referred thread configuration identifier.
   */
  static constexpr vac::container::CStringView kThreadConfigKey{"bsw_thread_configuration"_sv};

  /*!
   * \brief JSON key for the assigned p-port prototypes array.
   */
  static constexpr ::amsr::core::StringView kAssignedPportPrototypesContainerKey{"assigned_pport_prototypes"_sv};

  /*!
   * \brief Parse the number of threads.
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
  ParserResult ParseNumberOfThreads() noexcept;

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
   * \brief Parse the assigned pport prototype container.
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
  ParserResult ParseAssignedPPortPrototypeContainer() noexcept;

  /*!
   * \brief Parse the assigned pport prototype object.
   * \param[out] name  Reference to the assigned p-port.
   * \return The empty result or an error.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> ParseAssignedPPortPrototypeObject(AssignmentName& name) noexcept;

  /*!
   * \brief Map error code to Socal specific JSON parsing failure error code.
   * \param[in] error_code  ErrorCode to be mapped.
   * \return Mapped ErrorCode
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static ::amsr::core::ErrorCode MapToSocalError(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Number of threads type found.
   */
  std::size_t number_of_threads_found_{0};

  /*!
   * \brief Maximum number of tasks type found.
   */
  std::size_t max_number_of_tasks_found_{0};

  /*!
   * \brief Assigned p-port prototype type found.
   */
  std::size_t assigned_pport_prototypes_found_{0};

  /*!
   * \brief P-port type found.
   */
  std::size_t pport_found_{0};

  /*!
   * \brief Number of threads configurations found.
   */
  std::size_t number_of_bsw_thread_configuration_found_{0};

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  ThreadPoolConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_THREAD_POOL_PARSER_H_
