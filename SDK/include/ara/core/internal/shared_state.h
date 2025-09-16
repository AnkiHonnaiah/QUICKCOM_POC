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
/**        \file ara/core/internal/shared_state.h
 *        \brief Contains SharedState to be used by ara::core::Future and ara::core::Promise as a means to communicate.
 *
 *      \details Is thread-safe and exceptionless.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_SHARED_STATE_H_
#define LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_SHARED_STATE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <utility>
#include "vac/language/unique_function.h"

namespace ara {
namespace core {
namespace internal {

/*!
 * \brief  A threadsafe state to be used by ara::core::Promise and ara::core::Future to share data.
 * \tparam R  the Result type containing the same value/error types as the connected Future/Promise pair has.
 * \tparam Ec Future error code.
 * \vprivate
 */
template <typename R, typename Ec>
class SharedState final {
 public:
  /*!
   * \brief   Default constructor.
   * \details Called only by Promise.
   * \spec
   *   requires true;
   * \endspec
   */
  SharedState()
      : data_{Ec::no_state},
        is_ready_{false},
        is_valid_{false},
        cv_{},  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
        mutex_{},
        waiting_cnt_{0} {}

  /*! \brief Deleted copy constructor. */
  SharedState(SharedState const&) = delete;

  /*! \brief Deleted move constructor. */
  SharedState(SharedState&&) = delete;

  /*! \brief Deleted copy assignment operator. */
  SharedState& operator=(SharedState const&) & = delete;

  /*! \brief Deleted move assignment operator. */
  SharedState& operator=(SharedState&&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Defaulted destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SharedState() noexcept = default;  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief Get the number of threads waiting.
   * \pre   -
   * \spec requires true; \endspec
   * \return Number of threads waiting.
   */
  auto GetWaitingCnt() -> std::uint8_t {
    std::lock_guard<std::mutex> const lock{mutex_};
    return waiting_cnt_;
  }

  /*!
   * \brief   Called by the attached Promise when destructed, sets state to ready. If state is not ready, set
   *          broken_promise.
   * \details Called only by ~Promise() and Promise::operator=(Promise&&)
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void BreakPromise() noexcept {
    bool should_notify{false};
    {
      std::lock_guard<std::mutex> const lock{mutex_};
      if (!is_ready_) {
        data_.EmplaceError(Ec::broken_promise);
        is_ready_ = true;
      }
      should_notify = waiting_cnt_ > 0;
    }
    // if the other thread would at this moment call wait() is_ready_ is already true so it will not need a notification
    // so we can use the already out of date should_notify value here.
    if (should_notify) {
      cv_.notify_all();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  /*!
   * \brief   Wait until a value has been set and the state is ready.
   * \details Called only by Future.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void Wait() noexcept {
    // VECTOR Next Line AutosarC++17_10-M8.5.2: MD_VAC_M8.5.2_provideExplicitInitializer
    std::unique_lock<std::mutex> lock{mutex_};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_++;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    cv_.wait(lock, [this]() -> bool { return is_ready_; });  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_--;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
  }

  /*!
   * \brief   Waits until the state is ready or until the given time is reached, whichever comes first.
   * \details Called only by Future.
   * \tparam  Clock The clock on which this time point is measured.
   * \tparam  Duration A std::chrono::duration type used to measure the time since epoch.
   * \param   abs_time maximum time point to block until.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  bool True if the state is ready and false if it is not.
   */
  template <typename Clock, typename Duration>
  bool WaitUntil(std::chrono::time_point<Clock, Duration> const& abs_time) noexcept(false) {
    // VECTOR Next Line AutosarC++17_10-M8.5.2: MD_VAC_M8.5.2_provideExplicitInitializer
    std::unique_lock<std::mutex> lock{mutex_};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_++;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    bool const ret{cv_.wait_until(lock, abs_time, [this]() -> bool { return is_ready_; })};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_--;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    return ret;
  }

  /*!
   * \brief   Waits until the state is ready or until the given time has passed, whichever comes first.
   * \details Called only by Future.
   * \tparam  Rep An arithmetic type representing the number of ticks.
   * \tparam  Period Type representing the tick period.
   * \param   timeout_duration Maximum duration to wait for.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  bool True if the state is ready and false if it is not.
   */
  template <typename Rep, typename Period>
  bool WaitFor(std::chrono::duration<Rep, Period> const& timeout_duration) noexcept(false) {
    // VECTOR Next Line AutosarC++17_10-M8.5.2: MD_VAC_M8.5.2_provideExplicitInitializer
    std::unique_lock<std::mutex> lock{mutex_};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_++;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    bool const ret{cv_.wait_for(lock, timeout_duration, [this]() -> bool { return is_ready_; })};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_--;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    return ret;
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief   Sets the given lvalue to the contained data_.
   * \details If the data has already been set then promise_already_satisfied will be set instead. If trying to set
   *          after the data has been retrieved then no_state will be set instead.
   *          Called only by Promise.
   * \param   data lvalue to be copied to data_.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetData(R const& data) noexcept {
    bool should_notify{false};
    {
      std::lock_guard<std::mutex> const lock{mutex_};
      if (!SetError()) {
        data_ = data;
        is_ready_ = true;  // If SetError() returns true, then is_ready_ is already true.
      }
      should_notify = waiting_cnt_ > 0;
    }
    if (should_notify) {
      cv_.notify_all();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Sets the given rvalue to the contained data_.
   * \details   If the data has already been set then promise_already_satisfied will be set instead. If trying to set
   *            after the data has been retrieved then no_state will be set instead.
   *            Called only by Promise.
   * \param[in] data rvalue to be moved to data_.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetData(R&& data) noexcept {
    bool should_notify{false};
    {
      std::lock_guard<std::mutex> const lock{mutex_};
      if (!SetError()) {
        data_ = std::move(data);
        is_ready_ = true;  // If SetError() returns true, then is_ready_ is already true.
      }
      should_notify = waiting_cnt_ > 0;
    }
    if (should_notify) {
      cv_.notify_all();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Assigns the given executable to the func_.
   * \details   If the executable has already been executed then promise_already_satisfied will be set instead. If
   *            trying to set after the data has been retrieved then no_state will be set instead. Called only by
   *            Promise.
   * \tparam    F type of given executable.
   * \param[in] exec executable assigned to func_.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename F>
  void SetData(F&& exec) noexcept {
    bool should_notify{false};
    {
      std::lock_guard<std::mutex> const lock{mutex_};
      if (!SetError()) {
        // VECTOR NC AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
        func_ = std::forward<F>(exec);
        is_ready_ = true;
      }
      should_notify = waiting_cnt_ > 0;
    }
    if (should_notify) {
      cv_.notify_all();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  /*!
   * \brief   Retrieves the contained data_.
   * \details If the state is not ready, it will wait until it is ready.
   *          Called only by Future.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  data The contained data.
   */
  R GetData() noexcept {
    // VECTOR Next Line AutosarC++17_10-M8.5.2: MD_VAC_M8.5.2_provideExplicitInitializer
    std::unique_lock<std::mutex> lock{mutex_};
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_++;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    cv_.wait(lock, [this]() -> bool { return is_ready_; });  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    // VECTOR NL AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAround
    waiting_cnt_--;  // VECTOR SL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAround
    if (func_ != nullptr) {
      data_ = func_();
    }
    is_valid_ = false;
    return std::move(data_);
  }

  /*!
   * \brief   Makes state valid, called by Promise::get_future() to indicate that a connection has been made.
   * \details If the data has already been retrieved, it will set a future_already_retrieved error.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void MakeValid() noexcept {
    bool should_notify{false};
    {
      std::lock_guard<std::mutex> const lock{mutex_};
      if (is_valid_) {
        data_.EmplaceError(Ec::future_already_retrieved);
        is_ready_ = true;
        should_notify = waiting_cnt_ > 0;
      }
      is_valid_ = true;
    }

    if (should_notify) {
      cv_.notify_all();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  /*!
   * \brief   Checks if the state is valid.
   * \details Called only by Future.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  bool True if valid, false if not.
   */
  bool Valid() noexcept {
    std::lock_guard<std::mutex> const lock{mutex_};
    return is_valid_;
  }

  /*!
   * \brief   Checks if the state is ready.
   * \details Called only by Future.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  bool True if ready, false if not.
   */
  bool Ready() noexcept {
    std::lock_guard<std::mutex> const lock{mutex_};
    return is_ready_;
  }

 private:
  /*!
   * \brief  Sets error if trying to write to data_ during an unallowed state.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return true if an error has been set, false otherwise.
   */
  bool SetError() noexcept {
    if (is_ready_) {
      if (!is_valid_) {
        data_.EmplaceError(Ec::no_state);  // Occurs when trying to set data after reading data.
      } else {
        data_.EmplaceError(Ec::promise_already_satisfied);  // Occurs when setting data multiple times.
      }
      func_ = nullptr;
    }
    return is_ready_;
  }

  /*! \brief The contained data as a Result<T, E>. */
  R data_{};

  /*! \brief Bool to check if a value has been set to data_. */
  bool is_ready_{};

  /*! \brief Bool to check if a connection between Promise and Future has been made. */
  bool is_valid_{};

  /*! \brief Condition variable to be used when waiting. */
  std::condition_variable cv_{};

  /*! \brief Mutex to ensure thread-safety. */
  std::mutex mutex_{};

  /*! \brief UniqueFunction to hold an executable. */
  vac::language::UniqueFunction<R()> func_{nullptr};

  /*! \brief Counter to see how many are waiting for a notification. */
  std::uint8_t waiting_cnt_{};
};

}  // namespace internal
}  // namespace core
}  // namespace ara

/*!
 * \exclusivearea   ara::core::internal::SharedState::mutex_
 *                  Protect all private data members of SharedState object
 * \protects        ara::core::internal::SharedState::data_
 *                  ara::core::internal::SharedState::is_ready_
 *                  ara::core::internal::SharedState::is_valid_
 *                  ara::core::internal::SharedState::cv_
 *                  ara::core::internal::SharedState::func_
 *                  ara::core::internal::SharedState::waiting_cnt_
 * \usedin          ara::core::SharedState::BreakPromise
 *                  ara::core::SharedState::Wait
 *                  ara::core::SharedState::WaitUntil
 *                  ara::core::SharedState::WaitFor
 *                  ara::core::SharedState::SetData
 *                  ara::core::SharedState::GetData
 *                  ara::core::SharedState::MakeValid
 *                  ara::core::SharedState::Valid
 *                  ara::core::SharedState::Ready
 *                  ara::core::SharedState::GetWaitingCnt
 * \exclude         SetError of a SharedState object
 * \length          LONG
 *                  This exclusive area may be blocked long time by "waiting" functionality
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_SHARED_STATE_H_
