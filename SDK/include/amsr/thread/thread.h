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
 *        \brief  Thread implementation with dynamic memory.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/common.h"
#include "amsr/thread/error_domain.h"
#include "amsr/thread/internal/base_thread.h"
#include "amsr/thread/internal/dynamic_work.h"
#include "amsr/thread/thread_config_types.h"
#include "ara/core/error_domain.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace thread {
/*!
 * \brief Thread class.
 * \details The thread created by this class should only be modified with the API of this library.
 * \unit amsr::thread::ConfigurableThreads::Thread
 * \trace DSGN-VectorThreadLibrary-Thread
 * \vpublic
 */
class Thread final {
 public:
  /*!
   * \brief Default construct a Thread.
   * \details Creates an empty Thread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  Thread() noexcept = default;
  /*!
   * \brief Copy constructor deleted.
   */
  Thread(Thread const&) noexcept = delete;
  /*!
   * \brief Copy assignment deleted.
   */
  Thread& operator=(Thread const&) noexcept = delete;
  /*!
   * \brief Move construct a Thread.
   * \param[in,out] t The Thread instance to move from.
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
  Thread(Thread&& t) noexcept = default;  // VCA_THREAD_MOLE_1298
  /*!
   * \brief Move assign a Thread.
   * \param[in,out] t The Thread instance to move from.
   * \return            A reference to Thread.
   * \context           ANY
   * \pre               Target Thread instance must not be joinable.
   * \threadsafe        TRUE for different t
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  Thread& operator=(Thread&& t) & noexcept = default;  // VCA_THREAD_MOLE_1298

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_THREAD_M0.1.8_ThreadDestructor
  /*!
   * \brief Destruct a Thread.
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~Thread() noexcept = default;  // VCA_THREAD_MOLE_1298

  /*!
   * \brief Static object construction of a Thread with no specified configuration (use the default).
   * \tparam Callable   The callable type.
   * \tparam Args       The argument type.
   * \param[in] func    The callable to run within the thread.
   * \param[in,out] args The arguments to invoke the callable with within a thread (zero or more).
   * \return            A Thread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \error ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit, not a
   * multiple of the system page size (support data with passed stack size is provided), or thread name is longer than
   * the maximum thread name length kMaxSizeThreadName (support data with length of the passed name is provided).
   * \error ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or the policy.
   * \error ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with exact error code
   * is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \details A static_assert ensures that the passed arguments fit to the function signature.
   * \trace             DSGN-VectorThreadLibrary-ThreadConfiguration
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Callable,
            std::enable_if_t<!std::is_same<std::decay_t<Callable>, thread::ThreadConfig>::value>* = nullptr,
            typename... Args>
  // VECTOR NC AutosarC++17_10-M2.10.1: MD_THREAD_AutosarC++17_10-M2.10.1
  // VECTOR Next Construct VectorC++-V3.9.2: MD_THREAD_V3.9.2_builtin_types
  static ara::core::Result<Thread> Create(Callable&& func, Args&&... args) noexcept {
    // VECTOR Next Construct AutosarC++17_10-A5.0.1: MD_THREAD_A5.0.1_OrderOfThreadConfigAndStringConstruction
    // VECTOR Next Construct VectorC++-V5.0.1: MD_THREAD_V5.0.1_OrderOfThreadConfigAndStringConstruction
    return Create(ThreadConfig{}, amsr::core::String{}, std::forward<Callable>(func), std::forward<Args>(args)...);
  }

  /*!
   * \brief Static object construction of a Thread with specified configuration.
   * \tparam Callable   The callable type.
   * \tparam Args       The argument type.
   * \param[in] config  The configuration to start a thread with.
   * \param[in] thread_name        Name of the thread.
   * \param[in] func    The callable to run within the thread.
   * \param[in,out] args The arguments to invoke the callable with within a thread (zero or more).
   * \return A Thread instance.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \error ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit, not a
   * multiple of the system page size (support data with passed stack size is provided), or thread name is longer than
   * the maximum thread name length kMaxSizeThreadName (support data with length of the passed name is provided).
   * \error ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or the policy.
   * \error ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with exact error code
   * is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \details A static_assert ensures that the passed arguments fit to the function signature.
   * \trace             DSGN-VectorThreadLibrary-ThreadConfiguration, DSGN-VectorThreadLibrary-ThreadName
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Callable, typename... Args>
  // VECTOR NC AutosarC++17_10-M2.10.1: MD_THREAD_AutosarC++17_10-M2.10.1
  // VECTOR Next Construct VectorC++-V3.9.2: MD_THREAD_V3.9.2_builtin_types
  static ara::core::Result<Thread> Create(ThreadConfig const& config, ara::core::StringView const& thread_name,
                                          Callable&& func, Args&&... args) noexcept {
    amsr::core::Result<Thread> result{ThreadErrc::kThreadCreationFailed};

    if (thread_name.length() > thread::kMaxSizeThreadName) {
      result.EmplaceError(thread::ThreadErrc::kInvalidConfig,
                          static_cast<ara::core::ErrorDomain::SupportDataType::CodeType>(thread_name.length()),
                          "Thread name out of bounds.");
    } else {
      // VECTOR Next Construct AutosarC++17_10-A18.9.1: MD_THREAD_A18.9.1
      // VCA_THREAD_CALL_PASS_FUNC_OBJ, VCA_THREAD_CALL_PASS_PTR_R
      result = StartThread(MakeDynamicWork((std::bind(std::forward<Callable>(func), std::forward<Args>(args)...)),
                                           thread_name, config.GetNiceValue()),
                           config, thread_name);
    }

    return result;
  }

  /*!
   * \brief Join the given thread.
   * \details   If a deadlock would occur due to calling Join() on a thread that is already joining the
   *            calling thread program execution is aborted.
   * \return A void result on success.
   * \context           ANY|!Self   May be called only from other threads.
   * \pre               -
   * \threadsafe        FALSE
   * \error ThreadErrc::kJoinThreadFailed The thread is not joinable.
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-Thread
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Result<void> Join() noexcept {
    return thread_obj_.Join();  // VCA_THREAD_CALL_OBJ_PASS_VOID
  }

  /*!
   * \brief Get the name of the associated thread.
   * \return A StringView of the thread name.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-ThreadName
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::StringView GetName() const noexcept {
    return amsr::core::StringView{thread_name_.data(), thread_name_length_};
  }

  /*!
   * \brief Return whether the given thread is joinable.
   * \return Joinable state as bool.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-Thread
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  bool Joinable() const noexcept { return thread_obj_.Joinable(); };

  /*!
   * \brief Gets the thread running state.
   * \return Thread running state.
   * \context           ANY
   * \pre               Must not be an empty thread instance.
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-Thread
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadRunningState GetRunningState() const noexcept {
    if (dynamic_work_ == nullptr) {
      amsr::core::Abort("Thread::GetRunningState was called on an invalid Thread object");
    }
    return dynamic_work_->GetRunningState();  // VCA_THREAD_CALL_BY_PTR_PASS_VALUE
  }

 private:
  /*!
   * \brief Shared pointer to a base class of the dynamic work.
   */
  using DynamicWorkPtr = std::shared_ptr<internal::DynamicWorkBase>;

  /*!
   * \brief Create a dynamic work instance.
   * \tparam Callable The type of work.
   * \param[in] f       The work to create a work instance for.
   * \param[in] name    The name of the work instance. The length of the name must not exceed the length of the name
   *                    storage in DynamicWork.
   * \param[in] nice_value      Nice Value to set for thread.
   * \return            A unique pointer to the dynamic work instance.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \trace             DSGN-VectorThreadLibrary-ThreadMemoryAllocation
   */
  template <typename Callable>
  static DynamicWorkPtr MakeDynamicWork(Callable&& f, amsr::core::StringView const name,
                                        amsr::core::Optional<NiceValue> nice_value) noexcept {
    // VCA_THREAD_EXT_CALL_PASS_VALUE
    return std::make_shared<internal::DynamicWork<Callable>>(std::forward<Callable>(f), name, nice_value);
  }

  /*!
   * \brief Start a thread with the given work (dynamic work instance) and configuration.
   * \param[in] ptr     A pointer to the dynamic work instance.
   * \param[in] config  The configuration to start the thread with. Configuration values must be valid. Otherwise, no
   *                    thread will be created.
   * \param[in] thread_name        Name of the thread.
   * \return            A Thread instance.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \error ThreadErrc::kInsufficientResources Insufficient memory, or maximum number of threads reached.
   * \error ThreadErrc::kInvalidConfig Stack size less than minimum stack size, greater than system limit, not a
   * multiple of the system page size (support data with passed stack size is provided), or thread name is longer than
   * the maximum thread name length (support data with length of the passed name is provided).
   * \error ThreadErrc::kInsufficientPermissions Insufficient permissions to set scheduling parameters or the policy.
   * \error ThreadErrc::kNonPosixError All other errors that are non POSIX specified. Support data with exact error code
   * is provided.
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  static amsr::core::Result<Thread> StartThread(DynamicWorkPtr ptr, ThreadConfig const& config,
                                                amsr::core::StringView const& thread_name);

  /*!
   * \brief             Constructor from a BaseThread instance.
   * \param[in] thr     A BaseThread instance to construct a Thread from.
   * \param[in,out] name A name for the thread stored in Thread instance. The length of name must not exceed
   *                    kMaxSizeThreadName.
   * \param[in] dynamic_work_ptr DynamicWorkData shared pointer.
   * \context           Static object construction.
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  Thread(thread::internal::BaseThread&& thr, amsr::core::StringView name, DynamicWorkPtr&& dynamic_work_ptr);

  /*!
   * \brief Store the name of the associated thread.
   */
  ThreadNameStorage thread_name_{};

  /*!
   * \brief Store the actual length of the name contained in thread_name_.
   */
  std::size_t thread_name_length_{0};

  /*!
   * \brief BaseThread member for low-level thread management.
   */
  internal::BaseThread thread_obj_;

  /*!
   * \brief shared_ptr to DynamicWork.
   * \details dynamic_work_ is shared with the thread function(ThreadWrapper()).
   */
  DynamicWorkPtr dynamic_work_;
};

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_H_
