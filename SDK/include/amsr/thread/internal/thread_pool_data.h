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
 *        \brief  Define the data structure to use for the thread pool.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_POOL_DATA_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_POOL_DATA_H_

#include <atomic>
#include <condition_variable>
#include <mutex>

#include "vac/container/static_list.h"

namespace amsr {
namespace thread {
namespace internal {
/*!
 * \brief Structure for data shared between the threads.
 * \tparam W Type of the work unit.
 */
template <class W>
struct ThreadPoolData {  // VCA_THREAD_MOLE_1298, VCA_THREAD_STL_CALL_OBJ_DTOR
  /*!
   * \brief A queue to store work units before being processed by a thread pool worker.
   */
  vac::container::StaticList<W> work_queue{};
  /*!
   * \brief Synchronize access to the work queue.
   */
  std::mutex work_queue_mutex{};
  /*!
   * \brief Notify thread pool workers about new work or request them to exit.
   */
  std::condition_variable work_queue_cv{};  // VCA_THREAD_STL_CTOR
  /*!
   * \brief Notify workers to finish their execution.
   */
  std::atomic_bool running{true};
};

/*!
 * \exclusivearea   ::thread::internal::ThreadPoolData::work_queue_mutex
 *                  Used for synchronized access to the work queue from different worker threads.
 * \protects        ::thread::internal::ThreadPoolData::work_queue
 * \usedin          ::thread::internal::WorkOne, ::thread::ThreadPool::Stop, ::thread::ThreadPool::SubmitWork,
 *                  ::thread::ThreadPool::IsQueueFull
 * \exclude         All other methods of a ThreadPool object.
 * \length          SHORT Put/get work units from the queue.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_POOL_DATA_H_
