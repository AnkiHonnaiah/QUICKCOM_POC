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
/*!        \file  socal/internal/thread_pool.h
 *        \brief  Defines class ThreadPool.
 *      \details  The ThreadPool class contains a number of threads that processes incoming requests.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <condition_variable>
#include <memory>
#include <string>

#include "amsr/core/string_view.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/thread_pool_task.h"
#include "amsr/socal/internal/types.h"
#include "amsr/thread/thread.h"
#include "vac/container/static_list.h"
#include "vac/container/static_vector.h"

namespace amsr {
namespace socal {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief The ThreadPool contains a number of threads that processes incoming requests.
 *
 * \complexity Multiple inter module calls to fs-thread are required during the construction of WorkerThread. Metric is
 *             slightly above the threshold, no risk identified.
 * \unit       Socal::ThreadPool::ThreadPool
 */
class ThreadPool final {
 public:
  /*!
   * \brief State to control worker thread execution.
   */
  enum class State : std::uint8_t { kStopped, kRunning };

  /*!
   * \brief ThreadPool configuration type.
   */
  using ThreadPoolConfig = configuration::ThreadPoolConfig;

  /*!
   * \brief Type for pool id.
   */
  using PoolId = configuration::PoolId;

  /*!
   * \brief Type for the number of worker threads.
   */
  using ThreadCount = configuration::ThreadCount;

  /*!
   * \brief Type for the number of tasks.
   */
  using TaskCount = configuration::TaskCount;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ThreadPool() noexcept;

  /*!
   * \brief If the ThreadPool object goes out of scope / gets destroyed, shut down all worker threads and join all
   *        threads contained for proper termination.
   * \pre         -
   * \context     Shutdown
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~ThreadPool() noexcept;

  /*!
   * \brief   Initializes the ThreadPool by spawning the worker threads.
   * \details This will return immediately if the thread pool has running threads already.
   * \param[in] config  The thread-pool's config including:
   *                    - Pool id,
   *                    - Number of worker threads.
   *                    - Number of tasks.
   *                    - (optional) scheduler priority.
   *                    - (optional) scheduler policy.
   * \pre         State of threadpool must be State::kStopped.
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  void Initialize(ThreadPoolConfig const& config) noexcept;

  /*!
   * \brief Add a generic task to the queue for the worker thread to process
   * \param[in] task  Task to add to the queue for later processing within a worker thread.
   * \return true if the task could be enqueued, false if the maximum number of queued tasks has been exceeded.
   * \pre         State of thread pool must be State::kRunning.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  bool EnqueueTask(ThreadPoolTask&& task) noexcept;

  /*!
   * \brief   Remove all tasks that have this key from the queue.
   * \details This function will block until all currently running tasks with the given key are done.
   * \param[in] key  Task key representing the creator the task. The given key must not be nullptr.
   * \pre         The given key must not be nullptr
   * \context     ANY context other than Callback.
   * \threadsafe  TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  void RemoveTasks(ThreadPoolTask::CreatorKey const key) noexcept;

  /*!
   * \brief Getter for the pool id.
   * \return The configured id of the Threadpool.
   * \pre         ThreadPool must be initialized.
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  PoolId GetPoolId() const noexcept;

  /*!
   * \brief Get the prefix for the name of this thread pool.
   * \return A string_view to the prefix of the thread pool.
   * \pre         Thread pool must be initialized.
   * \context     ANY
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  ::amsr::core::StringView GetPoolPrefix() const noexcept;

  /*!
   * \brief Type-alias that specifies a queue for the tasks to process.
   */
  using TaskQueue = vac::container::StaticList<ThreadPoolTask>;

  /*!
   * \brief Getter for the number of worker threads. This number will not change during runtime.
   * \return The number of worker threads.
   * \pre         -
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  typename TaskQueue::size_type GetNumberOfWorkerThreads() const noexcept;

  /*!
   * \brief Getter for the maximum number of task configured. This number will not change during runtime.
   * \return The maximum number of tasks configured.
   * \pre         -
   * \context     App
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  TaskCount GetMaxNumberOfTasks() const noexcept;

 private:
  /*!
   * \brief   Shuts down the complete ThreadPool including all contained threads.
   * \details This will terminate threads even if there are pending tasks in the queue. To ensure the condition_var
   *          works as intended refer to: http://en.cppreference.com/w/cpp/thread/condition_variable Even if the shared
   *          variable is atomic, it must be modified under the mutex in order to correctly publish the modification to
   *          the waiting thread.
   * \pre         -
   * \context     Shutdown
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void Shutdown() noexcept;

  /*!
   * \brief Condition variable used to do a blocking wait until a tasked is pending for a worker thread or the thread-
   *        pool has been shut down.
   */
  std::condition_variable cv_;

  /*!
   * \brief Queue of tasks which are processed by worker threads.
   */
  TaskQueue queue_;

  /*!
   * \brief Mutex for synchronizing access to the task queue.
   */
  std::mutex pool_mutex_;

  /*!
   * \brief State of this thread pool for threads to access and terminate if the state_ is kStopped.
   */
  std::atomic<State> state_;

  /*!
   * \brief Prefix of this thread pool's name.
   */
  std::string pool_prefix_{};

  /*!
   * \brief Thread pools may not be copied.
   */
  ThreadPool(ThreadPool const& other) = delete;

  /*!
   * \brief Thread pools may not be moved.
   */
  ThreadPool(ThreadPool const&& other) = delete;

  /*!
   * \brief Thread pools may not be copied.
   */
  ThreadPool& operator=(ThreadPool const& other) = delete;

  /*!
   * \brief Thread pools may not be moved.
   */
  ThreadPool& operator=(ThreadPool&& other) = delete;

  /*!
   * \brief Called by the initialization routine to spawn the worker threads.
   * \param[in] thread_config_name  Name of the bsw thread configuration used to create the worker thread.
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void SpawnWorkerThreads(std::string const& thread_config_name) noexcept;

  /*!
   * \brief   Constructs the worker thread name for the provided arguments.
   * \details The name will look like this: [prefix][pool ID]-[thread count].
   *
   *          [prefix] has at most 9 characters [pool ID] is 1 byte, i.e. it has at most 3 characters '-' is 1 character
   *          [thread count] is 1 byte, i.e. it has at most 3 characters.
   *
   *          In total this will result in the largest possible constructed name to consist of 16 characters (guideline
   *          is documented in AMSR-12703).
   * \param[in] thread_id  ID of the worker thread.
   * \return Name of the worker thread
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  std::string GetWorkerThreadName(std::size_t const thread_id) const noexcept;

  /*!
   * \brief Configuration for this thread-pool.
   */
  ThreadPoolConfig cfg_;

  /*!
   * \brief Worker Thread manages single thread of the ThreadPool.
   */
  class WorkerThread final {
   public:
    /*!
     * \brief Creates and starts worker thread.
     * \param[in] worker_thread_name  Name of the worker thread, used for logging.
     * \param[in] thread_config_name  Name of the thread configuration.
     * \param[in] queue               Task queue, where the tasks shall be picked up.
     * \param[in] state               Threadpool state, used for shutting down this thread.
     * \param[in] pool_mutex          Mutex used to protect accesses to the queue.
     * \param[in] cv                  Condition variable used to signal worker threads of newly incoming tasks, or
     *                                threadpool state change.
     * \pre         -
     * \context     Init
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady FALSE
     */
    WorkerThread(std::string const& worker_thread_name, std::string const& thread_config_name,
                 ThreadPool::TaskQueue& queue, std::atomic<State>& state, std::mutex& pool_mutex,
                 std::condition_variable& cv) noexcept;

    WorkerThread(WorkerThread const&) noexcept = delete;
    WorkerThread(WorkerThread&&) noexcept = delete;
    WorkerThread& operator=(WorkerThread const&) & noexcept = delete;
    WorkerThread& operator=(WorkerThread&&) & noexcept = delete;

    /*!
     * \brief Joins the running thread.
     * \pre         -
     * \context     Shutdown
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady FALSE
     */
    ~WorkerThread() noexcept;

    /*!
     * \brief Wait until the current running task is finished if it is associated with the given key.
     * \param[in] key  The key to wait for.
     * \pre         -
     * \context     App.
     * \threadsafe  TRUE
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     */
    void Wait(ThreadPoolTask::CreatorKey const key) noexcept;

   private:
    /*!
     * \brief  The main function of the thread.
     * \remark Called by the worker thread.
     * \pre        -
     * \context    Init | Callback | Shutdown.
     * \threadsafe TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     */
    void Run() noexcept;

    /*!
     * \brief Logger.
     */
    ::amsr::socal::internal::logging::AraComLogger logger_;

    /*!
     * \brief The current running task key.
     */
    ThreadPoolTask::CreatorKey running_task_key_{nullptr};

    /*!
     * \brief Mutex to protect running_task_key_ from being accessed/modified from multiple threads.
     */
    std::mutex task_mutex_;

    /*!
     * \brief Condition variable used to do a blocking wait until an already scheduled task is done.
     */
    std::condition_variable task_cv_;

    /*!
     * \brief Reference of the task queue.
     */
    TaskQueue& queue_;

    /*!
     * \brief Reference of the threadpool state.
     */
    std::atomic<State>& state_;

    /*!
     * \brief Reference of the mutex to protect accesses to queue_.
     */
    std::mutex& pool_mutex_;

    /*!
     * \brief Reference of the condition variable used for queue_ changes.
     */
    std::condition_variable& cv_;

    /*!
     * \brief Name of the worker thread. Needed for debugging purposes.
     */
    std::string const worker_thread_name_;

    /*!
     * \brief The worker thread.
     */
    ::amsr::thread::Thread worker_thread_{};
  };

  /*!
   * \brief Pool Id.
   */
  PoolId pool_id_{0U};

  /*!
   * \brief Type-alias for the worker thread container.
   */
  using WorkerThreadContainer = vac::container::StaticVector<WorkerThread>;

  /*!
   * \brief Worker threads contained within this thread-pool.
   */
  WorkerThreadContainer workers_;

  /*!
   * \brief Reference to the logger for this thread-pool.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::ThreadPool::pool_mutex_
 * Used for synchronization by using a condition variable and protecting the read/write access to the task queue.
 *
 * \protects ::amsr::socal::internal::ThreadPool::queue_ and ::amsr::socal::internal::ThreadPool::state_ are the
 * protected resource of a ThreadPool object.
 * \usedin ::amsr::socal::internal::ThreadPool::EnqueueTask
 * \usedin ::amsr::socal::internal::ThreadPool::RemoveTasks
 * \usedin ::amsr::socal::internal::ThreadPool::Shutdown
 * \usedin ::amsr::socal::internal::ThreadPool::SpawnWorkerThreads
 * \usedin ::amsr::socal::internal::ThreadPool::WorkerThread::Run
 * \exclude All other methods of a ThreadPool and WorkerThread object.
 * \length SHORT Pushing tasks to the queue.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::amsr::socal::internal::ThreadPool::WorkerThread::task_mutex_
 * Used to protect the running worker task key from being accessed/modified from multiple threads.
 *
 * \protects ::amsr::socal::internal::ThreadPool::WorkerThread::running_task_key_ is the protected resource of a
 * WorkerThread object.
 * \usedin ::amsr::socal::internal::ThreadPool::WorkerThread::Run
 * \usedin ::amsr::socal::internal::ThreadPool::WorkerThread::Wait
 * \exclude All other methods of a ThreadPool::WorkerThread object.
 * \length SHORT scheduling the running task by manipulating the threadpool queue
 * \endexclusivearea
 */
// clang-format on
