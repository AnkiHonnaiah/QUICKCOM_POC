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
/*!        \file  configuration_parser/configuration_keys.h
 *        \brief  Provide Constants for JSON keys in the thread configuration.
 *        \unit   amsr::thread::ConfigurableThreads::ComponentLifecycleAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_CONFIGURATION_KEYS_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_CONFIGURATION_KEYS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/string_literals.h"

namespace amsr {
namespace thread {
namespace internal {
namespace configuration_parser {

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_THREAD_M7.3.6_sv
using vac::container::literals::operator""_sv;

/*!
 * \brief Class containing JSON keys for the thread configuration parsers.
 */
class ConfigurationParserKeys final {
 public:
  /*!
   * \brief Key for generatorVersion
   */
  static constexpr vac::container::CStringView const kGeneratorVersion{"generatorVersion"_sv};

  /*!
   * \brief Key for the thread configs array
   */
  static constexpr vac::container::CStringView const kThreadConfigs{"threadsConfigs"_sv};

  /*!
   * \brief Key for the identifier attribute of thread config.
   */
  static constexpr vac::container::CStringView const kIdentifier{"identifier"_sv};

  /*!
   * \brief Key for the scheduling policy value
   */
  static constexpr vac::container::CStringView const kSchedulingPolicy{"schedulingPolicy"_sv};

  /*!
   * \brief Key for the scheduling priority value
   */
  static constexpr vac::container::CStringView const kSchedulingPriority{"schedulingPriority"_sv};

  /*!
   * \brief String representation of the 'Other' scheduling policy
   */
  static constexpr vac::container::CStringView const kSchedulingPolicyOther{"SCHED_OTHER"_sv};

  /*!
   * \brief String representation of the 'FIFO' scheduling policy
   */
  static constexpr vac::container::CStringView const kSchedulingPolicyFifo{"SCHED_FIFO"_sv};

  /*!
   * \brief String representation of the 'Round Robin' scheduling policy
   */
  static constexpr vac::container::CStringView const kSchedulingPolicyRoundRobin{"SCHED_RR"_sv};

  /*!
   * \brief Key for the thread stack size value
   */
  static constexpr vac::container::CStringView const kStackSize{"stackSize"_sv};

  /*!
   * \brief Key for the nice value
   */
  static constexpr vac::container::CStringView const kNiceValue{"niceValue"_sv};
};

}  // namespace configuration_parser
}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_CONFIGURATION_PARSER_CONFIGURATION_KEYS_H_
