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
 *        \brief  Provide basic thread functionality.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_H_

#include <pthread.h>
#include <sys/types.h>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/thread/thread_config_types.h"
#include "vac/container/c_string_view.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace thread {
namespace internal {

// forward declaration for testing
namespace tests {
// VECTOR Disable AutosarC++17_10-M3.2.4: MD_THREAD_M3.2.4_FriendTestDeclaration
class UT__BASETHREAD_MoveCTor__OtherEmpty_Test;
class UT__BASETHREAD_MoveCTor__OtherJoinable_Test;
class UT__BASETHREAD_MoveCTor__OtherJoined_Test;
class UT__BASETHREAD_MoveAssignment__ThisEmpty__OtherEmpty_Test;
class UT__BASETHREAD_MoveAssignment__ThisEmpty__OtherJoined_Test;
class UT__BASETHREAD_MoveAssignment__ThisEmpty__OtherJoinable_Test;
class UT__BASETHREAD_MoveAssignment__ThisJoined__OtherEmpty_Test;
class UT__BASETHREAD_MoveAssignment__ThisJoined__OtherJoined_Test;
class UT__BASETHREAD_MoveAssignment__ThisJoined__OtherJoinable_Test;
class UT__BASETHREAD__DeathTest_DTor__ThisJoinable__Abort_Test;
// VECTOR Enable AutosarC++17_10-M3.2.4: MD_THREAD_M3.2.4_FriendTestDeclaration
}  // namespace tests

/*!
 * \brief The base thread class.
 * \unit amsr::thread::ConfigurableThreads::BaseThread
 * \trace DSGN-VectorThreadLibrary-BaseThread
 * \vprivate Vector component internal API
 */
class BaseThread final {
 public:
  /*!
   * \brief Construct an empty BaseThread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  BaseThread() noexcept = default;

  BaseThread(BaseThread const&) noexcept = delete;

  BaseThread& operator=(BaseThread const&) noexcept = delete;

  /*!
   * \brief Move construct a BaseThread instance.
   * \details Swaps the thread handles of the current BaseThread instance and the passed BaseThread instance.
   * \param[in,out] t   The BaseThread instance to move from.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE for different t
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  BaseThread(BaseThread&& t) noexcept;

  /*!
   * \brief Move assign a BaseThread instance.
   * \param[in,out] t   The thread instance to move the thread_handle_ from.
   * \return A pointer to the thread instance the thread_handle_ was moved to.
   * \context           ANY
   * \pre               Target BaseThread instance must be joined.
   * \threadsafe        TRUE for different t
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  BaseThread& operator=(BaseThread&& t) & noexcept;

  /*!
   * \brief Destruct a BaseThread instance.
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  ~BaseThread() noexcept;

  /*!
   * \brief Static object construction of a BaseThread with configuration config and function func as callable.
   * \param[in] thread_config Input thread configuration.
   * \param[in] func The function the thread shall execute. The referenced function must exist for the entire lifetime
   * of the executing thread (i.e. not the BaseThread instance). Only after joining, the reference may be invalidated.
   * \param[in,out] args The arguments to run func with. The referenced arguments must exist for the entire lifetime of
   * the executing thread (i.e. not the BaseThread instance). Only after joining, the reference may be invalidated.
   * \return The constructed BaseThread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \error ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit or not a
   * multiple of the system page size.
   * \error ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or the policy.
   * \error ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with exact error code
   * is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  // VECTOR NC AutosarC++17_10-M2.10.1: MD_THREAD_AutosarC++17_10-M2.10.1
  static amsr::core::Result<BaseThread> Create(ThreadConfig const& thread_config, void* (*func)(void*),
                                               void* args) noexcept;

  /*!
   * \brief Join the given thread.
   * \details If a deadlock would occur due to calling Join() on a thread that is already joining the calling thread
   * program execution is aborted.
   * \return A void result on success.
   * \context           ANY|!Self   May be called only from other threads.
   * \pre               -
   * \threadsafe        FALSE
   * \error ThreadErrc::kJoinThreadFailed The thread is not joinable.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-BaseThread
   * \vprivate Vector component internal API
   */
  amsr::core::Result<void> Join() noexcept;

  /*!
   * \brief Set the name for the associated thread.
   * \param[in] name The name for the thread.
   * \return A void result on success.
   * \context           ANY
   * \pre               -
   * \threadsafe        FALSE
   * \error ThreadErrc::kInvalidThreadHandle The associated thread is not joinable, name cannot be set.
   * \error ThreadErrc::kThreadNameOutOfRange The given name exceeds the defined limit (kMaxSizeThreadName).
   * \error ThreadErrc::kNonPosixError Setting the name caused a non POSIX specified error. Support data with exact
   * error code is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  amsr::core::Result<void> SetName(vac::container::CStringView name) const noexcept;

  /*!
   * \brief Return whether the given thread is joinable.
   * \return Joinable state as bool.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vprivate Vector component internal API
   */
  bool Joinable() const noexcept { return thread_handle_ != ThreadHandle{}; };

 private:
  /*!
   * \brief The thread handle to use.
   */
  // VECTOR Next Line AutosarC++17_10-M18.7.1: MD_THREAD_M18.7.1_PthreadApi
  using ThreadHandle = pthread_t;

  /*!
   * \brief The handle for the underlying thread object.
   */
  ThreadHandle thread_handle_{};

  /*!
   * \brief Swap the thread handles of the current BaseThread instance with the given instance.
   * \param[in,out] rhs The value to swap with.
   * \context           ANY
   * \pre               -
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  void Swap(BaseThread& rhs) noexcept;

  /*!
   * \brief Construct a BaseThread instance from a ThreadHandle (i.e. thread_handle_).
   * \param[in] thread_handle The thread handle for the new BaseThread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  explicit BaseThread(ThreadHandle&& thread_handle) noexcept
      : thread_handle_{std::forward<ThreadHandle>(thread_handle)} {}

  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveCTor__OtherEmpty);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveCTor__OtherJoinable);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveCTor__OtherJoined);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisEmpty__OtherEmpty);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisEmpty__OtherJoined);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisEmpty__OtherJoinable);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisJoined__OtherEmpty);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisJoined__OtherJoined);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD, MoveAssignment__ThisJoined__OtherJoinable);
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(tests::UT__BASETHREAD__DeathTest, DTor__ThisJoinable__Abort);
};

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_H_
