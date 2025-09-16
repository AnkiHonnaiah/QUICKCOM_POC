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
/*!        \file  configuration_parser/thread_config_parser.h
 *        \brief  Parser for thread config JSON objects in the thread configurations.
 *        \unit   amsr::thread::ConfigurableThreads::ComponentLifecycleAPI
 *
 *********************************************************************************************************************/

#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_PARSER_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/json/reader.h"
#include "amsr/thread/thread_config_types.h"

namespace amsr {
namespace thread {
namespace internal {
namespace configuration_parser {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_THREAD_M3.4.1_CanBeDeclaredLocally
/*!
 * \brief Class for parsing of thread config JSON objects in the thread configuration.
 */
class ThreadConfigParser final : public amsr::json::Parser<ThreadConfigParser> {
 public:
  /*!
   * \brief vajson JSON document type
   */
  using VaDocument = amsr::json::JsonData;

  /*!
   * \brief Parser result type
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Error code result type
   */
  using ErrorCode = amsr::json::JsonErrc;

  /*!
   * \brief         Constructs a ThreadConfigParser.
   *
   * \param[in]     doc              The JSON document to parse.
   * \param[out]    thread_config    Output of the parser: A thread configuration of a process.
   *
   * \context       InitPhase
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \trace         DSGN-VectorThreadLibrary-ThreadConfigurationParsing
   */
  ThreadConfigParser(VaDocument& doc, ThreadConfig& thread_config) noexcept
      : Parser{doc}, thread_config_{thread_config} {}

  /*!
   * \brief Destructs the object.
   */
  ~ThreadConfigParser() noexcept final = default;  // VCA_THREAD_MOLE_1298

  /*!
   * \brief               Copy constructor deleted.
   */
  ThreadConfigParser(ThreadConfigParser&) = delete;

  /*!
   * \brief               Move constructor deleted.
   */
  ThreadConfigParser(ThreadConfigParser&&) = delete;

  /*!
   * \brief               Copy assignment operator deleted.
   */
  ThreadConfigParser& operator=(ThreadConfigParser&) = delete;

  /*!
   * \brief               Move assignment operator deleted.
   */
  ThreadConfigParser& operator=(ThreadConfigParser&&) = delete;

  /*!
   * \brief           Event callback for a string.
   *
   * \param[in]       str   The encountered string.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Unexpected string encountered or invalid resource
   *                                                                group referenced.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnString(vac::container::CStringView str) noexcept;

  /*!
   * \brief           Event callback for a number.
   *
   * \param[in]       number   The encountered number.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Unexpected number encountered.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber number) noexcept;

  /*!
   * \brief           Event callback for the end of a JSON object.
   *
   * \return          State of parsing: amsr::json::ParserState::kFinished
   *
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Parsed configuration is invalid.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnEndObject(std::size_t) const noexcept;

  /*!
   * \brief           Event callback for an unexpected type.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning to continue parsing
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  static ParserResult OnUnexpectedEvent() noexcept;

 private:
  /*!
   * \brief Output of the parser: A thread configuration of a process.
   */
  ThreadConfig& thread_config_;

  /*!
   * \brief The identifier of parsed thread configuration.
   */
  amsr::core::String parsed_identifier_{};

  /*!
   * \brief The parsed thread scheduling priority.
   */
  amsr::core::Optional<SchedulingPriority> parsed_sched_priority_{};

  /*!
   * \brief The parsed thread scheduling policy.
   */
  amsr::core::Optional<SchedulingPolicy> parsed_sched_policy_{};

  /*!
   * \brief The parsed thread stack size.
   */
  amsr::core::Optional<std::size_t> stack_size_{};

  /*!
   * \brief The parsed thread nice value.
   */
  amsr::core::Optional<NiceValue> nice_value_{};
};

}  // namespace configuration_parser
}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_PARSER_H_
