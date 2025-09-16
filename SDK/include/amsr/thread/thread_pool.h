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
 *        \brief  Define a pool of threads.
 *        \unit   amsr::thread::ConfigurableThreads::ThreadPool
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_POOL_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_POOL_H_

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/common.h"
#include "amsr/thread/error_domain.h"
#include "amsr/thread/internal/base_thread.h"
#include "amsr/thread/internal/thread_pool_data.h"
#include "amsr/thread/internal/worker.h"
#include "amsr/thread/thread_config_types.h"
#include "amsr/thread/work_unit.h"
#include "ara/core/vector.h"
#include "vac/container/c_string_view.h"
#include "vac/container/static_list.h"
#include "vac/testing/test_adapter.h"

namespace amsr {

namespace thread {

// forward declaration for friend test
namespace tests {
// VECTOR Disable AutosarC++17_10-M3.2.4: MD_THREAD_M3.2.4_FriendTestDeclaration
class UT__THREADPOOL_Create_Test;
class UT__THREADPOOL_ExecuteWork_Test;
class UT__THREADPOOL_SubmitWork_Test;
class UT__THREADPOOL_Configuration_Test;
class UT__THREADPOOL_WorkQueueFifoOrder_Test;
class UT__THREADPOOL__NWorker_Lifecycle_Test;
class UT__THREADPOOL_Stop__EmptyPool_Test;
class UT__THREADPOOL_Stop__RunningPool_Test;
class UT__THREADPOOL_SubmitWork__Order_Test;
class UT__THREADPOOL__ZeroWorker_SubmitWork_Test;
class UT__THREADPOOL__NWorker_WorkQueueCapacity_Test;
class UT__THREADPOOL__NWorker_ThreadsInPool_Test;
class UT__THREADPOOL__NWorker_NoInitialWork_Test;
// VECTOR Enable AutosarC++17_10-M3.2.4: MD_THREAD_M3.2.4_FriendTestDeclaration
}  // namespace tests

namespace internal {
/*!
 * \brief               Determine the number of digits required to print the given number in decimal format.
 * \param[in] number    The number to get the digit count for.
 * \return              Number of digits required.
 * \details             The mapping for the naming for n started threads with n: 1-10 -> 0-9, i.e. 1 digit, 11-100 ->
 *                      10-99, i.e. 2 digits etc.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           TRUE
 * \synchronous         TRUE
 */
std::size_t DigitCount(std::size_t number) noexcept;

/*!
 * \brief               Check that the thread names fulfill the size constraints.
 * \param[in]           base_name The name to use as base.
 * \param[in]           number_threads The number of threads to start.
 * \return              A void result if all names fulfill the size constraints (i.e. name.size() <= kMaxSizeThreadName)
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \error ThreadErrc::kThreadNameOutOfRange The given name exceeds the defined limit (kMaxSizeThreadName) or is too
 * long to provide proper enumeration (i.e. base_name0..., base_name11).
 * \reentrant           FALSE
 * \synchronous         TRUE
 */
amsr::core::Result<void> SizeCheck(amsr::core::StringView base_name, std::size_t number_threads) noexcept;

}  // namespace internal

/*!
 * \brief Implement a thread pool with a given capacity.
 * \details Threads created by this class should only be modified with the API of this library.
 * \tparam W The work unit to use/process in the thread pool. Must be derived from thread::WorkUnit.
 * \trace DSGN-VectorThreadLibrary-ThreadPool
 * \vpublic
 */
template <class W>
class ThreadPool final {
  static_assert(std::is_base_of<WorkUnit, W>::value, "W must inherit from WorkUnit");

 public:
  /*!
   * \brief Deleted empty constructor.
   */
  ThreadPool() noexcept = delete;
  /*!
   * \brief Copy constructor deleted.
   */
  ThreadPool(ThreadPool const&) = delete;
  /*!
   * \brief Copy assignment deleted.
   */
  ThreadPool& operator=(ThreadPool const&) = delete;

  /*!
   * \brief Move construct a ThreadPool.
   * \param[in] t       The ThreadPool instance to move from.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE for different t
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadPool(ThreadPool&& t) noexcept = default;

  /*!
   * \brief Move assign a ThreadPool.
   * \param[in] t       The ThreadPool instance to move from.
   * \return            Reference to itself.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE for different t
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   */
  ThreadPool& operator=(ThreadPool&& t) & noexcept = default;  // VCA_THREAD_MOLE_1298

  /*!
   * \brief             Destruct a ThreadPool.
   * \context           Destruction of ThreadPool.
   * \pre               -
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~ThreadPool() noexcept { Stop(); }

  /*!
   * \brief             Static object construction of a ThreadPool.
   * \param[in]         config  The configuration to start the threads of the ThreadPool with. The name as specified in
   *                    the configuration must follow the constraint laid out in the details section.
   * \param[in]         thread_name        Name of the thread.
   * \param[in]         number_threads The number of threads to start.
   * \param[in]         length_list The length of the work queue.
   * \return            A ThreadPool<W>.
   * \details The configuration provides both the thread parameters (e.g. stack size) and a basis for the naming of
   * the individual threads of the thread pool. The member 'name' of the config is used as following for the thread
   * naming: name0, name1, ... name[number_threads-1]. The name must therefore be shorter than: kMaxSizeThreadName -
   * [number of digits required to display number_threads - 1]. Ex.: number_threads = 10 -> enumeration from 0 - 9,
   * hence one digit required -> max size of name: kMaxSizeThreadName - 1.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \error             ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error             ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit or
   *                    not a multiple of the system page size.
   * \error             ThreadErrc::kThreadNameOutOfRange The given name exceeds the defined limit (kMaxSizeThreadName)
   *                    or is too long to provide proper enumeration (i.e. base_name0..., base_name11).
   * \error             ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or
   *                    the policy.
   * \error             ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with
   *                    exact error code is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-ThreadPoolInstantiation,
   *                    DSGN-VectorThreadLibrary-ThreadPool
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Check required length for thread name enumeration.
   *  - Instantiate a ThreadPool. This includes heap memory allocation.
   *  - Start the threads of the ThreadPool.
   *  - Check that the threads were started successfully.
   *  - Name the threads of the ThreadPool.
   *  - On error, stop the threads and return the error.
   * \endinternal
   */
  static ara::core::Result<ThreadPool<W>> Create(thread::ThreadConfig const& config,
                                                 ara::core::StringView const& thread_name, std::size_t number_threads,
                                                 std::size_t length_list) {
    /*!
     * \brief Shorthand for static object construction result of ThreadPool.
     */
    using R = amsr::core::Result<ThreadPool<W>>;
    // VCA_THREAD_CALL_PASS_PTR_R
    return internal::SizeCheck(thread_name, number_threads)
        .Map([number_threads, length_list]() -> ThreadPool<W> {
          return ThreadPool<W>{number_threads, length_list};
        })
        .AndThen([&config, &thread_name](ThreadPool<W>&& thr) -> R {
          R result{std::move(thr)};
          amsr::core::Result<void> res{result.Value().StartThread(config)};
          if (res.HasValue()) {
            res = result.Value().NameThread(thread_name);
          }
          if (!res.HasValue()) {
            result.Value().Stop();
            result.EmplaceError(res.Error());
          }
          return result;
        });  // COV_THREAD_TOOLINGISSUE
             // Covered by: UT__THREADPOOL, Create
             // Covered by: UT__THREADPOOL, Create__InvalidStackSize
  }

  /*!
   * \brief             Submit a work unit to the thread pool.
   * \tparam Args       Type of the arguments.
   * \param[in] args    The arguments to instantiate a new work unit with. The provided arguments must exist until a
   *                    worker thread processed the submitted work (finished the execution of the Run() method).
   *                    However, this information might not be known, depending on the work unit implementation. In such
   *                    cases it is advisable to ensure, that the arguments exist for the entire lifetime of the
   *                    ThreadPool. Only after destruction, the arguments may be invalidated.
   * \return            True if the work unit was submitted successfully.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-WorkUnit
   * \vpublic
   * \internal
   *  - Lock the mutex for the work queue.
   *  - Check that the work queue is not full.
   *   - If the queue is not full, emplace the given arguments into the work queue.
   *   - Notify a single thread about new work.
   * \endinternal
   */
  template <typename... Args>
  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_THREAD_M9.3.3_no_logical_constness
  bool SubmitWork(Args&&... args) noexcept {
    if (data_ == nullptr) {
      amsr::core::Abort("There is no shared data in unique pointer.");
    }
    bool ret_val{false};
    std::lock_guard<std::mutex> const lock{data_->work_queue_mutex};  // VCA_THREAD_STL_CALL_OBJ_PASS_PTR_RW
    if (!data_->work_queue.full()) {
      data_->work_queue.emplace_back(std::forward<Args>(args)...);  // VCA_THREAD_EXT_CALL_BY_PTR_MOVE
      data_->work_queue_cv.notify_one();                            // VCA_THREAD_STL_CALL_BY_PTR_PASS_VOID
      ret_val = true;
    }
    return ret_val;
  }

  /*!
   * \brief             Signal all threads to exit.
   * \details           Threads will exit once they complete their current work unit.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Set running flag to false, lock mutex, notify all threads.
   * \endinternal
   */
  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_THREAD_M9.3.3_no_logical_constness
  void Stop() noexcept {
    if (data_ != nullptr) {
      std::lock_guard<std::mutex> const lock{data_->work_queue_mutex};  // VCA_THREAD_STL_CALL_OBJ_PASS_PTR_RW
      data_->running = false;                                           // VCA_THREAD_STL_CALL_BY_PTR_PASS_VALUE
      data_->work_queue_cv.notify_all();                                // VCA_THREAD_STL_CALL_BY_PTR_PASS_VOID
    }
  }

  /*!
   * \brief             Check if the queue is full.
   * \return            True if the queue is full.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-WorkUnit
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Lock mutex, return queue state.
   * \endinternal
   */
  bool IsQueueFull() const noexcept {
    if (data_ == nullptr) {
      amsr::core::Abort("There is no shared data in unique pointer.");
    }
    std::lock_guard<std::mutex> const lock{data_->work_queue_mutex};  // VCA_THREAD_STL_CALL_OBJ_PASS_PTR_RW
    return data_->work_queue.full();
  }

 private:
  /*!
   * \brief             Construct a ThreadPool instance.
   * \param[in]         number_threads The number of threads.
   * \param[in]         length_list The length of the work_queue.
   * \details           Includes heap memory allocation.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_THREAD_A15.4.2_threads_more_than_max_size_exception
  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_THREAD_A15.5.3_threads_more_than_max_size_exception
  ThreadPool(std::size_t number_threads, std::size_t length_list) noexcept {
    // heap allocation
    data_ = std::make_unique<internal::ThreadPoolData<W>>();
    data_->work_queue.reserve(length_list);  // VCA_THREAD_EXT_CALL_BY_PTR_PASS_VALUE
    threads_.reserve(number_threads);        // VCA_THREAD_EXT_CALL_PASS_VALUE
  }

  /*!
   * \brief             Start threads with the given config.
   * \param[in]         config  The configuration to start the threads with.
   * \return            A void result on success.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \error             ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error             ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit or
   *                    not a multiple of the system page size.
   * \error             ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or
   *                    the policy.
   * \error             ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with
   *                    exact error code is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-ThreadPoolInstantiation
   *
   * \internal
   * - Loop for all threads.
   *   - Check that no error occurred in the previous step.
   *     - Create a thread.
   *     - Check whether thread construction was successful.
   * \endinternal
   */
  ara::core::Result<void> StartThread(thread::ThreadConfig const& config) noexcept {
    amsr::core::Result<void> result{};
    std::size_t i{0};

    while ((++i <= threads_.capacity()) && (result.HasValue())) {
      amsr::core::Result<internal::BaseThread> thr_result{
          thread::internal::BaseThread::Create(  // VCA_THREAD_CALL_PASS_FUNC_OBJ
              config, &internal::Worker<W>, data_.get())};
      if (thr_result.HasValue()) {
        threads_.emplace_back(std::move(thr_result).Value());  // VCA_THREAD_EXT_CALL_BY_PTR_MOVE
      } else {
        result.EmplaceError(thr_result.Error());
      }
    }
    return result;
  }

  /*!
   * \brief             Name all threads in vector threads_.
   * \param[in]         base_name The string to use as basis for the naming of the threads. Length/Size of base_name +
   *                    digit count(threads_.capacity() - 1) <= kMaxSizeThreadName.
   * \return            A void result on success.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \error             ThreadErrc::kInvalidThreadHandle The associated thread is not joinable, name cannot be set.
   * \error             ThreadErrc::kThreadNameOutOfRange The given name is too long to append the enumeration values
   *                    for the threads.
   * \error             ThreadErrc::kNonPosixError Setting the name caused a non POSIX specified error. Support data
   *                    with the error code is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-ThreadPool, DSGN-VectorThreadLibrary-ThreadPoolInstantiation
   *
   * \internal
   * - Empty the thread name storage.
   * - Copy the contents of the base_name.
   * - Loop over all threads.
   *   - Check that no error occurred in the previous step.
   *     - Copy the final name into the temporary name storage.
   *     - Set the name.
   * \endinternal
   */
  ara::core::Result<void> NameThread(ara::core::StringView const base_name) noexcept {
    if ((base_name.size() + internal::DigitCount(threads_.capacity())) > kMaxSizeThreadName) {
      amsr::core::Abort("Capacity of storing all threads instances is more than kMaxSizeThreadName.");
    }
    amsr::core::Result<void> result{};
    std::size_t i{0};
    ThreadNameStorage thread_name_storage{0};

    static_cast<void>(std::copy(base_name.cbegin(), base_name.cend(), thread_name_storage.begin()));
    for (thread::internal::BaseThread& thr : threads_) {
      if (result.HasValue()) {
        std::string const counter{std::to_string(i)};  // VCA_THREAD_STL_CALL_PASS_VALUE
        ThreadNameStorage::iterator thread_name_storage_it{thread_name_storage.begin()};
        std::advance(thread_name_storage_it, base_name.length());
        static_cast<void>(std::copy(counter.cbegin(), counter.cend(), thread_name_storage_it));
        // Calculated length (base_name.length() + counter.length()) is w/o zero term, +1 required for CStringView.
        // VCA_THREAD_CALL_PASS_PTR_R
        result = thr.SetName(
            vac::container::CStringView{thread_name_storage.data(), base_name.length() + counter.length() + 1});
        ++i;
      }
    }
    return result;
  }

  /*!
   * \brief Store all shared data with a unique_ptr.
   */
  std::unique_ptr<internal::ThreadPoolData<W>> data_{nullptr};
  /*!
   * \brief Store all thread instances.
   */
  ara::core::Vector<thread::internal::BaseThread> threads_{};

  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, Create);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, ExecuteWork);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, SubmitWork);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, Configuration);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, WorkQueueFifoOrder);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL__NWorker, Lifecycle);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, Stop__EmptyPool);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, Stop__RunningPool);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL, SubmitWork__Order);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL__ZeroWorker, SubmitWork);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL__NWorker, WorkQueueCapacity);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL__NWorker, ThreadsInPool);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__THREADPOOL__NWorker, NoInitialWork);
};

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_POOL_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_THREAD_TOOLINGISSUE
//   \ACCEPT XX
//   \REASON The branch is already covered by a test case but VCast does not show correct coverage measurements.
// COV_JUSTIFICATION_END
