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
/*!        \file  configuration_parser/thread_config_file_parser.h
 *        \brief  Parser for executable's thread configuration file
 *        \unit   amsr::thread::ConfigurableThreads::ComponentLifecycleAPI
 *
 *********************************************************************************************************************/

#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_FILE_PARSER_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_FILE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/thread/thread_config_types.h"

namespace amsr {
namespace thread {
namespace internal {
namespace configuration_parser {

/*!
 * \brief Class used for parsing configuration parameters from the executable configuration.
 */
class ThreadConfigFileParser final : public amsr::json::Parser<ThreadConfigFileParser> {
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
   * \brief       Constructs an ThreadConfigFileParser.
   *
   * \param[in]   doc             The JSON document to parse.
   * \param[out]  configuration   Output of the parser: Parsed executable's thread configuration.
   *
   * \context     InitPhase
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ThreadConfigFileParser(VaDocument& doc, ThreadConfigList& configuration) noexcept;

  /*!
   * \brief Destructs the object.
   */
  ~ThreadConfigFileParser() noexcept final = default;  // VCA_THREAD_MOLE_1298

  /*!
   * \brief               Copy constructor deleted.
   */
  ThreadConfigFileParser(ThreadConfigFileParser&) = delete;

  /*!
   * \brief               Move constructor deleted.
   */
  ThreadConfigFileParser(ThreadConfigFileParser&&) = delete;

  /*!
   * \brief               Copy assignment operator deleted.
   */
  ThreadConfigFileParser& operator=(ThreadConfigFileParser&) = delete;

  /*!
   * \brief               Move assignment operator deleted.
   */
  ThreadConfigFileParser& operator=(ThreadConfigFileParser&&) = delete;

  /*!
   * \brief           Event callback for the start of a JSON object.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning
   *
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Unexpected object encountered or parsing of object
   *                                                                failed.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnStartObject() noexcept;

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
   * \brief           Event callback for the start of an array.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning
   *
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Unexpected array encountered.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnStartArray() noexcept;

  /*!
   * \brief           Event callback for the end of an array.
   *
   * \return          State of parsing: amsr::json::ParserState::kRunning
   *
   * \error           amsr::json::JsonErrc::kUserValidationFailed   Unexpected end of array encountered.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ParserResult OnEndArray(std::size_t) noexcept;

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
   * \brief Output of the parser: Parsed executable's thread configuration.
   */
  ThreadConfigList& configuration_;

  /*!
   * \brief Parser state flag that shows if the parser is currently in the thread configs array.
   */
  bool in_thread_configs_array_{false};
};

}  // namespace configuration_parser
}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_THREAD_CONFIG_FILE_PARSER_H_
