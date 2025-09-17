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
 *      \brief      Contains a synchronization primitive that switches implementation based on a strategy.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_PRIMITIVE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_PRIMITIVE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>

#include "amsr/log/internal/sync/empty_lock.h"  // IWYU pragma: export
#include "amsr/log/internal/sync/strategy.h"    // IWYU pragma: export

namespace amsr {
namespace log {
namespace internal {
namespace sync {
/*!
 * \brief A class that tries to aquire exclusive access to a lock.
 *
 * \tparam Guard The synchronization primitive to use. Must be either a type implementing BasicLockable or EmptyLock.
 *
 * \details Will not block, but simply continue with a nonaquired state.
 *
 * \threadsafe TRUE in case the Guard itself is threadsafe.
 *
 * \unit LogAndTrace::Common::SyncStrategy::TryLock
 *
 * \vprivate Component internal.
 */
template <typename Guard>
class TryLock final {
  /*! \brief The synchronization primitive to lock. */
  Guard& guard_;
  /*! \brief The state of the guard. */
  bool locked_;

 public:
  /*!
   * \brief Tries to aquire the lock of the passed guard without blocking.
   *
   * \param guard The guard to lock.
   *
   * \threadsafe TRUE in case the Guard itself is threadsafe.
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit TryLock(Guard& guard) noexcept
      : guard_{guard},
        // VCA_LOGAPI_CHECKED_CONTRACT
        locked_{guard_.try_lock()} {}

  /*! \brief Deleted move constructor. */
  TryLock(TryLock&&) = delete;
  /*! \brief Deleted move assignment. */
  auto operator=(TryLock&&) & -> TryLock& = delete;
  /*! \brief Deleted copy constructor. */
  TryLock(TryLock const&) = delete;
  /*! \brief Deleted copy assignment. */
  auto operator=(TryLock const&) & -> TryLock& = delete;

  /*!
   * \brief On destruction unlock the guard in case we have locked it.
   *
   * \threadsafe TRUE in case the Guard itself is threadsafe.
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_AutosarC++17_10-M0.1.8_no_effect
  ~TryLock() noexcept {
    if (locked_) {
      // VCA_LOGAPI_VALID_OBJECT
      this->guard_.unlock();
    }
  }

  /*!
   * \brief Check if the guard was locked.
   *
   * \return True if the guard was locked.
   *
   * \threadsafe TRUE
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit operator bool() const noexcept { return this->locked_; }
};

/*!
 * \brief A switchable synchronization primitive.
 *
 * \tparam kStrategy The strategy to use for the synchronization primitive.
 *
 * \details Supports Mutexes, Spinlocks & a dummy implemenation (single threaded usecase).
 */
template <Strategy kStrategy>
using Primitive = std::conditional_t<kStrategy == Strategy::kMutex, std::mutex, EmptyLock>;

}  // namespace sync
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_PRIMITIVE_H_
