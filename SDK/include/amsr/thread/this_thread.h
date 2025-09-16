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
 *        \brief  Provides functionality to use within a running thread.
 *        \unit   amsr::thread::ThisThreadAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THIS_THREAD_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THIS_THREAD_H_

#include <chrono>
#include <thread>

#include "amsr/core/result.h"
#include "amsr/thread/common.h"
#include "amsr/thread/thread_id.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace thread {

/*!
 * \brief Sleep for the specified duration.
 * \details Delegate calls to std::this_thread::sleep_for implementation.
 * \tparam Rep          Arithmetic type representing the number of ticks of the duration.
 * \tparam Period       Type representing the number of seconds per tick.
 * \param[in] rtime     The time duration to sleep for.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Rep, typename Period>
inline void SleepFor(std::chrono::duration<Rep, Period> const& rtime) noexcept {
  std::this_thread::sleep_for(rtime);  // VCA_THREAD_STL_CALL_PASS_PTR_R
}

/*!
 * \brief Sleep until the given point in time.
 * \details Delegate calls to std::this_thread::sleep_until implementation.
 * \tparam Clock        The clock to use for time measurement. Must implement the named requirement Clock.
 * \tparam Duration     std::chrono::duration type to measure the time since epoch against.
 * \param[in] atime     Sleep until this point in time is reached.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Clock, typename Duration>
inline void SleepUntil(std::chrono::time_point<Clock, Duration> const& atime) noexcept {
  std::this_thread::sleep_until(atime);  // VCA_THREAD_STL_CALL_PASS_PTR_R
}

/*!
 * \brief Get the name for the current thread.
 * \return The name as ThreadNameStorage.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \error ThreadErrc::kNonPosixError Getting the thread name failed with unknown non POSIX error. Support data with
 * exact error code is provided.
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
ara::core::Result<ThreadNameStorage> GetName() noexcept;

/*!
 * \brief Set the name for the current thread.
 * \param[in] thread_name The name for the thread.
 * \return An empty result if successful.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \error ThreadErrc::kThreadNameOutOfRange       The given name exceeds the defined limit (kMaxSizeThreadName).
 * \error ThreadErrc::kNonPosixError              Setting the name caused a non POSIX specified error. Support data with
 * exact error code is provided.
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vpublic
 */
ara::core::Result<void> SetName(vac::container::CStringView const& thread_name) noexcept;

/*!
 * \brief   Get the Thread ID of the calling thread.
 * \details Always returns a ThreadId. Once the calling thread has finished, the value of ThreadId may be reused by
 *          another thread. Therefore, the user should not use the returned ThreadId after the identified thread has
 *          finished.
 * \return  ThreadId of the calling thread.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vpublic
 */
ThreadId GetCurrentThreadId() noexcept;

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THIS_THREAD_H_
