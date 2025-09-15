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
/**        \file  socal/internal/thread_pools_manager_interface.h
 *        \brief  Interface for the ThreadpoolsManager.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/thread_pool_task.h"
#include "amsr/socal/internal/types.h"
#include "vac/container/static_list.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class interface for handling ThreadPoolsManager.
 *
 * \unit Socal::ThreadPool::ThreadPoolsManager
 */
class ThreadPoolsManagerInterface {
 public:
  /*!
   * \brief Type for pool id.
   */
  using PoolId = configuration::PoolId;

  /*!
   * \brief Type-alias that specifies a queue for the tasks to process.
   */
  using TaskQueue = vac::container::StaticList<std::unique_ptr<Task>>;

  /*!
   * \brief Trigger the EnqueueTask on the assigned thread pool.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \param[in] task                   ThreadPoolTask to add to the queue for later processing within a worker thread.
   * \return true if the task could be enqueued, false if the maximum number of queued tasks has exceeded.
   * \pre         State of threadpool must be State::kRunning.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  virtual bool EnqueueTask(::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier,
                           ThreadPoolTask&& task) noexcept = 0;

  /*!
   * \brief Trigger Remove tasks on the assigned threadpool.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \param[in] key                    Key identifying the creator of the thread-pool task. The given key must not be
   *                                   nullptr.
   * \pre         ThreadPool must be initialized.
   * \pre         The given key must not be nullptr.
   * \context     ANY context other than Callback.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires key != nullptr; \endspec
   * \steady FALSE
   */
  virtual void RemoveTasks(::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier,
                           ThreadPoolTask::CreatorKey const key) noexcept = 0;

  /*!
   * \brief Trigger the GetNumberOfWorkerThreads on the configured threadpool. This number will not change during
   *        runtime.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \return The number of worker threads.
   * \pre         ThreadPool must be initialized.
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual typename TaskQueue::size_type GetNumberOfWorkerThreads(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept = 0;

  /*!
   * \brief Trigger the GetPoolId on the configured threadpool.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \return The configured id of the Threadpool.
   * \pre         ThreadPool must be initialized.
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual PoolId GetPoolId(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept = 0;

  /*!
   * \brief Trigger the GetMaxNumberOfTasks on the configured threadpool. This number will not change during runtime.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \return The number of tasks configured.
   * \pre         ThreadPool must be initialized.
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual typename configuration::TaskCount GetMaxNumberOfTasks(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept = 0;

  /*!
   * \brief Check whether user-defined thread pool is assigned to the provided identifier.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \return TRUE if the a user-defined threadpool is mapped. FALSE otherwise.
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual bool IsThreadPoolAssigned(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) const noexcept = 0;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ThreadPoolsManagerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual ~ThreadPoolsManagerInterface() noexcept = default;

  ThreadPoolsManagerInterface(ThreadPoolsManagerInterface const& other) = delete;
  ThreadPoolsManagerInterface(ThreadPoolsManagerInterface const&& other) = delete;
  ThreadPoolsManagerInterface& operator=(ThreadPoolsManagerInterface const& other) = delete;
  ThreadPoolsManagerInterface& operator=(ThreadPoolsManagerInterface&& other) = delete;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_INTERFACE_H_
