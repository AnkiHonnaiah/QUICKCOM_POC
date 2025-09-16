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
 *        \brief  Dynamic work struct definition.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_DYNAMIC_WORK_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_DYNAMIC_WORK_H_

#include <algorithm>
#include <atomic>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/common.h"
#include "amsr/thread/thread_config_types.h"
#include "amsr/thread/thread_running_state.h"
#include "vac/language/cpp17_backport.h"

namespace amsr {
namespace thread {
namespace internal {

/*!
 * \brief A base struct for the work associated with a thread.
 * \unit   amsr::thread::ConfigurableThreads::Thread
 */
class DynamicWorkBase {
 public:
  /*!
   * \brief Constructor defaulted.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  DynamicWorkBase() noexcept = default;
  DynamicWorkBase(DynamicWorkBase const&) = delete;
  DynamicWorkBase& operator=(DynamicWorkBase const&) = delete;
  DynamicWorkBase(DynamicWorkBase&&) = delete;
  DynamicWorkBase& operator=(DynamicWorkBase&&) = delete;
  /*!
   * \brief Defaulted virtual destructor.
   */
  virtual ~DynamicWorkBase() noexcept = default;
  /*!
   * \brief Function to execute within a thread.
   * \context           Thread that owns this DynamicWork as resource.
   * \pre               -
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \internal
   *  - Updates the thread running state before and after invoking the callable.
   *  - Invokes the callable.
   *  - Resets the callable when the thread completes.
   * \endinternal
   */
  virtual void Run() = 0;
  /*!
   * \brief Retrieve the member variable for the thread name.
   * \return A StringView of the thread name.
   * \context           Thread owning this DynamicWork before jumping to user context.
   * \pre               -
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  virtual amsr::core::StringView GetName() const noexcept = 0;
  /*!
   * \brief Get the thread running state.
   * \return            Thread running state.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   */
  virtual amsr::thread::ThreadRunningState GetRunningState() const noexcept = 0;

  /*!
   * \brief Get the thread nice value.
   * \return            Thread nice value.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   */
  virtual amsr::core::Optional<NiceValue> GetNiceValue() const noexcept = 0;
};

/*!
 * \brief DynamicWork structure templated with the work (the callable).
 * \tparam Callable The type of work. Must be a callable with no arguments.
 * \unit   amsr::thread::ConfigurableThreads::Thread
 */
template <typename Callable>
class DynamicWork final : public DynamicWorkBase {  // VCA_THREAD_MOLE_1298
 public:
  /*!
   * \brief             Construct a DynamicWork instance.
   * \param[in] f       The callable to store in the dynamic work unit.
   * \param[in] name    The thread name to store in the dynamic work unit. The length of the name must not exceed the
   *                    the length of the name storage.
   * \param[in] nice_value       The nice value to store in the dynamic work unit.
   * \context           Instantiation of a Thread.
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   */
  DynamicWork(Callable&& f, amsr::core::StringView const name,
              amsr::core::Optional<NiceValue> const& nice_value) noexcept
      : DynamicWorkBase{}, name_length_{name.length()}, nice_value_{nice_value} {
    static_assert(vac::language::is_invocable_r<void, Callable>::value,
                  "The template parameter Callable must be a callable with no arguments.");

    func_.emplace(std::move(f));
    if (!func_.has_value()) {  // COV_THREAD_EMPTYCALLABLE
      amsr::core::Abort("Thread function must not be empty.");
    }

    static_cast<void>(std::copy(name.cbegin(), name.cend(), name_.begin()));
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_THREAD_M0.1.8_PublicAPI_Callback
  /*!
   * \copydoc DynamicWorkBase::Run()
   */
  void Run() final {
    running_state_ = amsr::thread::ThreadRunningState::kRunning;

    func_.value()();  // VCA_THREAD_EXT_CALL_CALLBACK_PASS_VOID
    func_.reset();

    running_state_ = amsr::thread::ThreadRunningState::kCompleted;
  }

  /*!
   * \copydoc DynamicWorkBase::GetName() const
   */
  amsr::core::StringView GetName() const noexcept final { return amsr::core::StringView{name_.cbegin(), name_length_}; }

  /*!
   * \copydoc DynamicWorkBase::GetRunningState() const
   */
  amsr::thread::ThreadRunningState GetRunningState() const noexcept final { return running_state_; }

  /*!
   * \copydoc DynamicWorkBase::GetNiceValue() const
   */
  amsr::core::Optional<NiceValue> GetNiceValue() const noexcept final { return nice_value_; }

 private:
  /*!
   * \brief The callable to run within the thread.
   */
  amsr::core::Optional<Callable const> func_{};
  /*!
   * \brief Buffer for the thread name.
   */
  ThreadNameStorage name_{};
  /*!
   * \brief Length of the stored string.
   */
  std::size_t const name_length_;

  /*!
   * \brief Keeps the thread running state.
   */
  std::atomic<amsr::thread::ThreadRunningState> running_state_{amsr::thread::ThreadRunningState::kUninitialized};

  /*!
   * \brief The thread nice value.
   * \details OS dependent value. The value is set if SchedulingPolicy is SCHED_OTHER.
   */
  amsr::core::Optional<NiceValue> nice_value_{};
};

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_DYNAMIC_WORK_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_THREAD_EMPTYCALLABLE
//   \ACCEPT XF
//   \REASON The condition cannot be tested because the current implementation of the Thread does not allow empty
//           callables.
// COV_JUSTIFICATION_END
