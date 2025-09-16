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
/*!        \file
 *        \brief  Lock aware of threads using it, protecting against wrong usage.
 *        \unit   osabstraction::net_utils
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_THREAD_AWARE_LOCK_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_THREAD_AWARE_LOCK_H_

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#include "amsr/core/abort.h"

namespace amsr {
namespace net_utils {
namespace internal {

/*!
 * \brief           A lock that is aware of the thread that holds it.
 *
 * \details         The information which thread holds the lock is used to prevent that
 *                  - a thread that holds the lock acquires it again.
 *                  - a thread that does not hold the lock releases it.
 *                  This information is protected by the lock as well.
 */
class ThreadAwareLock final {
 public:
  /*!
   * \brief           Create lock that is currently not locked.
   */
  ThreadAwareLock() noexcept = default;

  // Not copyable or movable as contained mutex is not copyable or movable.
  ThreadAwareLock(ThreadAwareLock const&) = delete;
  ThreadAwareLock(ThreadAwareLock&&) = delete;
  ThreadAwareLock& operator=(ThreadAwareLock const&) = delete;
  ThreadAwareLock& operator=(ThreadAwareLock&&) = delete;

  /*!
   * \brief           Destroys the lock.
   *
   * \pre             The lock is not held by any thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ThreadAwareLock() noexcept {
    // Destroying an std::mutex while it is held is UB.
    // Actually checking if a lock is held or not is always a race condition, this check is the best possible effort
    // to detect the precondition violation.
    if (holding_thread_id_.load() != std::thread::id{}) {
      ::amsr::core::Abort("Fatal error, lock destroyed while held.");
    }
  }  // VCA_OSA_STL_CALL_OBJ_DTOR

  /*!
   * \brief           Makes the calling thread acquire the lock.
   *
   * \details         If the lock is held by some other thread, the calling thread is blocked until the lock can be
   *                  acquired.
   *
   * \pre             The calling thread does not hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  void Lock() noexcept {
    // The ID of this thread does not change so it does not need to be protected by the lock.
    std::thread::id const this_thread_id{std::this_thread::get_id()};

    if (this_thread_id == holding_thread_id_.load()) {
      ::amsr::core::Abort("Fatal error, a thread tried to acquire a lock it already holds.");
    }

    mutex_.lock();  // VCA_OSA_STL_CALL_OBJ_PASS_VOID

    // The lock is already held at this point, this write operation is protected. Only one thread can execute this at
    // each point in time due to the lock. This means, there is no other write operation going on in another thread at
    // the same time this write operation takes place.
    // A thread that reaches this point can not call this function again before the write operation is complete because
    // it is only one thread of execution which must complete one function call before it can do another function call.
    holding_thread_id_.store(this_thread_id);
  }

  /*!
   * \brief           Makes the calling thread release the lock.
   *
   * \pre             The calling thread must hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  void Unlock() noexcept {
    std::thread::id const this_thread_id{std::this_thread::get_id()};

    if (this_thread_id != holding_thread_id_.load()) {
      ::amsr::core::Abort("Fatal error, a thread tried to release a lock it does not hold.");
    }

    // The lock is still held at this point, the write operation is protected. Only one thread can execute this at each
    // each point in time due to the lock. This means, there is no other write operation going on in another thread at
    // the same time this write operation takes place.
    // The default-constructed thread ID is a value that is different from the ID of any thread.
    holding_thread_id_.store(std::thread::id{});

    mutex_.unlock();  // VCA_OSA_STL_CALL_OBJ_PASS_VOID
  }

  /*!
   * \brief           Asserts that the calling thread holds the lock.
   *
   * \pre             The calling thread must hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  void AssertThreadHasLocked() const noexcept {
    // All changes to holding_thread_id_ that can happen concurrently are irrelevant because it can only change
    // regarding the relevant value (ID of the calling thread) if the calling thread acquires or releases the lock. But
    // the calling thread can only do one thing at a time, thus no relevant change can happen concurrently to this
    // function.

    // The ID of this thread does not change so it does not need to be protected by the lock.
    std::thread::id const this_thread_id{std::this_thread::get_id()};
    if (this_thread_id != holding_thread_id_.load()) {
      ::amsr::core::Abort("Fatal error, assertion that a thread must hold a lock failed.");
    }
  }

 private:
  /*!
   * \brief   The thread ID of the thread that currently holds the lock.
   *
   * \details If the lock is not held, this has the value that does not identify any thread. This is protected by the
   *          lock so only one write operation to it can occur at each point in time.
   *          An atomic is used to prevent reading a value that is only partly written.
   */
  std::atomic<std::thread::id> holding_thread_id_{};

  /*!
   * \brief   The mutex of the lock.
   */
  std::mutex mutex_{};
};

}  // namespace internal
}  // namespace net_utils
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_THREAD_AWARE_LOCK_H_
