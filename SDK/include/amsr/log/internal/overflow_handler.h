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
/**     \file
 *      \brief      Contains the declaration for the overflow messaging.
 *      \details
 *
 *      \unit       LogAndTrace::Common::OverflowHandler
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OVERFLOW_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OVERFLOW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <cstddef>
#include "amsr/core/string.h"
namespace vac {
namespace language {
// VECTOR NC AutosarC++17_10-A12.4.2: MD_LOG_AutosarC++17_10-A12.4.2_external_API
template <typename Signature>
class FunctionRef;
}  // namespace language
}  // namespace vac

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Handles messages for queue overflows.
 */
class OverflowHandler final {
  /*! \brief The application ID to use. */
  core::String app_id_{};
  /*! \brief The number of overflows that happened since last report. */
  std::atomic<std::size_t> count_{0};
  /*! \brief The last time the overflow has been reported. */
  std::chrono::time_point<std::chrono::steady_clock> last_time_{};
  /*! \brief True in case reporting is currently underway. */
  // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_external_API
  std::atomic_flag report_active_ = ATOMIC_FLAG_INIT;

 public:
  /*!
   * \brief Default constructs the overflow handler.
   * \spec
   *   requires true;
   * \endspec
   */
  OverflowHandler() noexcept {};
  /*!
   * \brief Define the default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~OverflowHandler() noexcept = default;
  /*! \brief Delete the copy constructor. */
  OverflowHandler(OverflowHandler const &) noexcept = delete;
  /*! \brief Delete the copy assignment. */
  auto operator=(OverflowHandler const &) &noexcept -> OverflowHandler & = delete;
  /*! \brief Delete the move constructor. */
  OverflowHandler(OverflowHandler &&) noexcept = delete;
  /*! \brief Delete the move assignment. */
  auto operator=(OverflowHandler &&) &noexcept -> OverflowHandler & = delete;

  /*!
   * \brief Sets the application ID for the message.
   *
   * \param[in] The application ID to used
   * \spec
   *   requires true;
   * \endspec
   */
  void SetAppId(core::String app_id) noexcept;
  /*!
   * \brief Reports a queue overflow.
   *
   * \param[in] interval The interval to wait between reporting the overflows. Defaults to 2s.
   * \spec
   *   requires true;
   * \endspec
   */
  void Report(std::chrono::milliseconds interval = std::chrono::milliseconds{2000}) noexcept;
  /*!
   * \brief Reports a queue overflow.
   *
   * \param[in] action   The action to perform in case the overflow should be reported.
   * \param[in] interval The interval to wait between reporting the overflows. Defaults to 2s.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M2.10.1: MD_LOG_AutosarC++17_10-M2.10.1
  void Report(vac::language::FunctionRef<void(std::size_t)> action,
              std::chrono::milliseconds interval = std::chrono::milliseconds{2000}) noexcept;

  /*!
   * \brief Reports a queue overflow in case the handler is unlocked.
   *
   * \param[in] action   The action to perform in case the overflow should be reported.
   * \param[in] interval The interval to wait between reporting the overflows. Defaults to 2s.
   * \param[in] locked   Report can only be generated if it's not locked
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M2.10.1: MD_LOG_AutosarC++17_10-M2.10.1
  void ReportIfUnlocked(vac::language::FunctionRef<void(std::size_t)> action, std::chrono::milliseconds interval,
                        bool locked) noexcept;

  /*!
   * \brief Reset the overflow counter.
   * \spec
   *   requires true;
   * \endspec
   */
  void Reset() noexcept {
    this->count_ = 0;
    this->last_time_ = std::chrono::time_point<std::chrono::steady_clock>{};
  }
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OVERFLOW_H_
