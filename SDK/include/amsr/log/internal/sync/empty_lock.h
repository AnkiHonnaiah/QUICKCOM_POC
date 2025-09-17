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
 *      \brief      Contains an implementation of a dummy lock for single thread purposes.
 *      \details
 *
 *      \unit       LogAndTrace::Common::SyncStrategy::EmptyLock
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_EMPTY_LOCK_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_EMPTY_LOCK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace log {
namespace internal {
namespace sync {
/*!
 * \brief An empty mutex that replaces std::mutex in single threaded use cases
 *
 * \vprivate Component internal.
 */
class EmptyLock final {
 public:
  /*!
   * \brief Implement the try_lock interface for a lock_guard
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto try_lock() noexcept -> bool { return true; }  // NOLINT(readability-identifier-naming)
  /*!
   * \brief Implement the lock interface for a lock_guard.
   * \details Is a dummy implementation that can be used only for single threaded applications.
   *
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_AutosarC++17_10-M0.1.8_no_effect
  constexpr static void lock() noexcept {}  // NOLINT(readability-identifier-naming)
  /*!
   * \brief Implement the unlock interface for a lock_guard
   * \details Is a dummy implementation that can be used only for single threaded applications.
   *
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_AutosarC++17_10-M0.1.8_no_effect
  constexpr static void unlock() noexcept {}  // NOLINT(readability-identifier-naming)
};
}  // namespace sync
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_EMPTY_LOCK_H_
