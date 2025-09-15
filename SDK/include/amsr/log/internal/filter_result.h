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
/**     \file       log_levels.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FILTER_RESULT_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FILTER_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>

namespace amsr {
namespace log {
namespace internal {

/*! \brief The result of the two-stage message filter. */
class FilterResult {
  /*!
   * \brief The bits that are used for the result.
   *
   * \details bit 0: Filter enabled (true) /disabled
   * bit 1: Filtered by sink (true) /logger
   */
  std::bitset<2> set_{};
  /*!
   * \brief Create the result from a bitset.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit FilterResult(std::bitset<2> set) : set_{set} {}

 public:
  /*!
   * \brief Create the result from a logger-specific filter.
   *
   * \param enabled If true the logger will remain enabled.
   * \return        The constructed result.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto FromLoggerFilter(bool enabled) noexcept -> FilterResult {
    std::bitset<2> set{};
    set.set(0, enabled);
    return FilterResult{set};
  }

  /*!
   * \brief Create the result from a sink-specific filter.
   *
   * \param enabled If true the logger will remain enabled.
   * \return        The constructed result.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto FromSinkFilter(bool enabled) noexcept -> FilterResult {
    std::bitset<2> set{};
    set.set(0, enabled);
    set.set(1);
    return FilterResult{set};
  }

  /*!
   * \brief Check if the message should stay enabled.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
  auto IsEnabled() const noexcept -> bool { return this->set_.test(0); }
  /*!
   * \brief Check if the message has already passed a sink filter.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
  auto HasPassedSink() const noexcept -> bool { return this->set_.test(1); }
  /*!
   * \brief Check if the message has only passed a log filter.
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasPassedLogger() const noexcept -> bool { return not this->HasPassedSink(); }
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FILTER_RESULT_H_
