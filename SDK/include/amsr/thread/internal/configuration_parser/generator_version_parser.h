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
/*!        \file  configuration_parser/generator_version_parser.h
 *        \brief  Parser for generator version JSON objects in the thread configuration file.
 *        \unit   amsr::thread::ConfigurableThreads::ComponentLifecycleAPI
 *
 *********************************************************************************************************************/

#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_GENERATOR_VERSION_PARSER_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_GENERATOR_VERSION_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"

namespace amsr {
namespace thread {
namespace internal {
namespace configuration_parser {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_THREAD_M3.4.1_CanBeDeclaredLocally
/*!
 * \brief   Class for parsing of generator version JSON objects in the executable configuration.
 *
 * \details Ignores and does not return the generator version currently.
 */
class GeneratorVersionParser final : public amsr::json::Parser<GeneratorVersionParser> {
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
   * \brief Inheriting base class constructor.
   */
  using amsr::json::Parser<GeneratorVersionParser>::Parser;

  /*!
   * \brief Destructs the object.
   */
  ~GeneratorVersionParser() noexcept final = default;  // VCA_THREAD_MOLE_1298

  /*!
   * \brief               Copy constructor deleted.
   */
  GeneratorVersionParser(GeneratorVersionParser&) = delete;

  /*!
   * \brief               Move constructor deleted.
   */
  GeneratorVersionParser(GeneratorVersionParser&&) = delete;

  /*!
   * \brief               Copy assignment operator deleted.
   */
  GeneratorVersionParser& operator=(GeneratorVersionParser&) = delete;

  /*!
   * \brief               Move assignment operator deleted.
   */
  GeneratorVersionParser& operator=(GeneratorVersionParser&&) = delete;

  /*!
   * \brief           Event callback for the end of a JSON object.
   *
   * \return          State of parsing: amsr::json::ParserState::kFinished
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  static ParserResult OnEndObject(std::size_t) noexcept;

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
};

}  // namespace configuration_parser
}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_GENERATOR_VERSION_PARSER_H_
