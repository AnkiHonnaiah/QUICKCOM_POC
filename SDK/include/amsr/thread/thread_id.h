/***********************************************************************************************************************
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
 *        \brief  Define the ThreadId class.
 *        \unit   amsr::thread::ThreadId
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_ID_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_ID_H_

#include <pthread.h>
#include <sys/types.h>
#include <ostream>
#include "amsr/core/optional.h"

namespace amsr {
namespace thread {

// VECTOR NL AutosarC++17_10-A2.2.1: MD_THREAD_A2.2.1_forbiddenCharacter_false_positive
/*!
 * \brief   The class serves as a unique identifier of the current thread.
 * \details Thread IDs can be reused by the system. After the thread that this ThreadId identifies has finished, the
 *          value of this ThreadId can be reused by another thread. A previously valid ThreadId may represent a
 *          different thread in the future. Therefore, the user should not use this ThreadId after the thread that this
 *          object identifies has finished.
 *          Generally, Thread ID usage for any practical purpose other than logging and debugging is discouraged.
 * \unit    amsr::thread::ThreadId
 * \trace   DSGN-VectorThreadLibrary-ThreadId
 * \vpublic
 */
class ThreadId final {
 private:
  /*!
   * \brief The native thread ID to use.
   */
  // VECTOR Next Line AutosarC++17_10-M18.7.1: MD_THREAD_M18.7.1_PthreadApi
  using NativeThreadId = pthread_t;

 public:
  /*!
   * \brief               Get the Thread ID of the calling thread.
   * \return              ThreadId of the calling thread.
   * \context             ANY
   * \pre                 -
   * \threadsafe          TRUE
   * \reentrant           FALSE
   * \synchronous         TRUE
   * \trace               DSGN-VectorThreadLibrary-ThreadId
   * \vprivate Component private
   */
  static ThreadId CreateCurrentThreadId() noexcept;

  /*!
   * \brief Default-constructs a new thread identifier. The identifier does not represent a thread.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadId() noexcept = default;

  /*!
   * \brief Default Copy constructor.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadId(ThreadId const&) noexcept = default;

  /*!
   * \brief Default Copy assignment.
   * \return Reference to itself.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadId& operator=(ThreadId const&) & noexcept = default;

  /*!
   * \brief Default move constructor.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadId(ThreadId&&) noexcept = default;

  /*!
   * \brief Default move assignment operator.
   * \return Reference to itself.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadId& operator=(ThreadId&&) & noexcept = default;

  /*!
   * \brief Default Destructor.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~ThreadId() noexcept = default;

  /*!
   * \brief           Compare two ThreadIds for equality.
   * \param[in]       rhs ThreadIds to compare to.
   * \return          True if both ThreadID objects either have no thread identifiers or have
   *                  identical native thread IDs. Otherwise, false.
   * \pre  -
   * \context ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vpublic
   * \trace DSGN-VectorThreadLibrary-ThreadId
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(ThreadId const& rhs) const&;

  /*!
   * \brief           Checks for inequality of two ThreadIds.
   * \param[in]       rhs ThreadIds to compare to.
   * \return          True if both ThreadIds identify different threads.
   * \pre  -
   * \context ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vpublic
   * \trace DSGN-VectorThreadLibrary-ThreadId
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator!=(ThreadId const& rhs) const&;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_THREAD_A11.3.1_friend
  /*!
   * \brief           Outputs the thread identifier details to an output stream.
   * \param[in,out]   os Output stream.
   * \param[in]       id ThreadId to output.
   * \returns         A reference to the same output stream.
   * \pre  -
   * \context ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vpublic
   * \trace DSGN-VectorThreadLibrary-ThreadId
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - Outputs the thread ID if there is a value. Otherwise, a message with no thread identifier
   *    to an output stream.
   * \endinternal
   */
  friend std::ostream& operator<<(std::ostream& os, ThreadId const& id) noexcept {
    if (id.native_thread_id_.has_value()) {
      os << id.native_thread_id_.value();
    } else {
      os << "No native thread id associated with this object.";  // VCA_THREAD_STL_CALL_PASS_PTR_R
    }
    return os;
  }

 private:
  /*!
   * \brief           Constructs a ThreadId from the OS native thread ID.
   * \param[in]       native_id The native thread ID representing the thread.
   * \pre -
   * \context ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate Component private
   * \trace DSGN-VectorThreadLibrary-ThreadId
   */
  explicit ThreadId(NativeThreadId native_id) noexcept;

  /*!
   * \brief The native thread ID that identifies the thread.
   */
  ara::core::Optional<NativeThreadId> native_thread_id_{};
};

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_ID_H_
