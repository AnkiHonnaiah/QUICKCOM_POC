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
 *        \brief  Define the worker function to execute within the threads.
 *        \unit   amsr::thread::ConfigurableThreads::ThreadPool
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_WORKER_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_WORKER_H_

#include <condition_variable>
#include <mutex>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/thread/internal/thread_pool_data.h"

namespace amsr {
namespace thread {
namespace internal {

/*!
 * \brief Process a single work unit from the queue when notified.
 * \tparam W Type of the work unit.
 * \param[in,out] work_ptr A pointer to the data shared between the threads.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \internal
 *  - Lock the mutex.
 *  - Check if work is available in the queue, else wait for condition variable.
 *  - Once work is available, copy element at front of the work queue.
 *  - Remove element from work queue.
 *  - Unlock mutex and execute callable.
 * \endinternal
 */
template <typename W>
inline void WorkOne(ThreadPoolData<W>* work_ptr) noexcept {
  std::unique_lock<std::mutex> lock{work_ptr->work_queue_mutex};  // VCA_THREAD_STL_CALL_OBJ_PASS_PTR_RW
  while (work_ptr->running && work_ptr->work_queue.empty()) {
    work_ptr->work_queue_cv.wait(lock);  // VCA_THREAD_STL_CALL_BY_PTR_PASS_VALUE
  }
  if (work_ptr->running) {
    // VCA_THREAD_EXT_CALL_BY_PTR_PASS_VOID
    if (work_ptr->work_queue.size() <= 0) {
      amsr::core::Abort("Size of queue to store work units has to be greater than zero.");
    }
    // VCA_THREAD_EXT_CALL_BY_PTR_PASS_VOID, VCA_THREAD_CALL_OBJ_MOVE
    W work_unit{std::move(work_ptr->work_queue.front())};
    work_ptr->work_queue.pop_front();  // VCA_THREAD_EXT_CALL_BY_PTR_PASS_VOID
    lock.unlock();
    // VCA_THREAD_EXT_CALL_CALLBACK_PASS_VOID
    work_unit.Run();  // execute the callable
  }
}

/*!
 * \brief The function to execute within the threads of the thread pool.
 * \tparam W            Type of the work unit.
 * \param[in,out] args  Void* argument providing a pointer to the data shared between the threads. Must be of type
 *                      ThreadPoolData.
 * \return nullptr.
 * \context             Thread of the thread pool.
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThreadPoolWorker
 */
template <typename W>
inline void* Worker(void* args) noexcept {
  // VECTOR NL AutosarC++17_10-M5.2.8: MD_THREAD_M5.2.8
  ThreadPoolData<W>* const work_ptr{static_cast<ThreadPoolData<W>*>(args)};
  while (work_ptr->running) {
    WorkOne(work_ptr);
  }
  return nullptr;
}

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_WORKER_H_
