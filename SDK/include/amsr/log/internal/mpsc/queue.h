/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       queue.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::MPSC::Queue
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_QUEUE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <deque>
#include <functional>
#include <type_traits>
#include <utility>

#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/sync/primitive.h"
#include "amsr/log/internal/utils.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace log {
namespace internal {
namespace mpsc {
/*!
 * \brief Internal queue with mutex
 *
 * \tparam T             The value type of the queue.
 *                       Must implement the MoveInsertable, MoveConstructible and Destructible named requirements.
 *                       Must also meet the type requirements of amsr::core::Optional<T>::Optional()
 *                       and amsr::core::Optional<T>::Emplace.
 * \tparam kSyncStrategy Switches between synchronization strategies.
 */
template <typename T, sync::Strategy kSyncStrategy>
class Queue final {
  /*!
   * \brief The guard used in this queue.
   * \note Maps kThread to the Spinlock implementation, kMutex to the std::mutex & kUnsafe to an empty lock
   */
  using Guard = sync::Primitive<kSyncStrategy>;
  /*! \brief Internal queue type. */
  using Q = std::deque<T>;

 public:
  /*! \brief Type for the on direct message callback */
  using OnMessage = vac::language::UniqueFunction<bool(T* const) const>;

 private:
  /*! \brief The queue containing the data. */
  Q queue_{};  // VECTOR SL AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_false_positive_braces
  /*! \brief The on message callback. */
  OnMessage mutable on_message_{[](T const* const) { return false; }};
  /*! \brief The guard protecting the queue. */
  Guard queue_guard_{};
  /*! \brief The guard protecting the callbacks. */
  Guard message_guard_{};
  /*! \brief The capacity of the queue. */
  std::uint32_t capacity_;

 public:
  /*! \brief Reference type of the contained value. */
  using reference = typename Q::reference;

  /*!
   * \brief Constructor
   * \param capacity The capacity for the queue
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Queue(std::uint32_t capacity) noexcept : capacity_(capacity) {}

  /*! \brief Disallow move construction. */
  Queue(Queue&&) = delete;
  /*! \brief Disallow copy construction. */
  Queue(Queue const&) = delete;
  /*! \brief Disallow move assignment. */
  auto operator=(Queue&&) -> Queue& = delete;
  /*! \brief Disallow copy assignment. */
  auto operator=(Queue const&) -> Queue& = delete;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_CHECKED_CONTRACT
  ~Queue() noexcept = default;

  /*!
   * \brief Add a value to the queue
   *
   * \param value The value to add
   * \return      Nothing or the error that occurred.
   *
   * \internal
   *   - #10 Try to send data directly to the receiver.
   *   - #20 If sending directly failed, store the value in the queue.
   * \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  auto Push(T&& value) noexcept -> Result<void> {
    Result<void> result{};
    if (not this->PushToUser<kSyncStrategy>(value)) {
      result = this->PushToQueue(std::move(value));
    }
    return result;
  }

  /*!
   * \brief Pop from this queue.
   *
   * \return either the popped element or nothing.
   *
   * \internal
   *   - #10 Get exclusive access to the queue.
   *   - #20 If the queue is not empty
   *    - #30 move the first element to the result.
   *    - #40 Pop the first element from the queue.
   * \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  auto Pop() noexcept -> Optional<T> {
    Optional<T> result{};
    std::lock_guard<Guard> const guard{this->queue_guard_};
    if (not this->queue_.empty()) {
      // VCA_LOGAPI_CHECKED_CONTRACT
      T& ref{this->queue_.front()};
      result.emplace(std::move(ref));
      // VCA_LOGAPI_CHECKED_CONTRACT
      this->queue_.pop_front();
    }
    return result;
  }

  /*!
   * \brief Set the on message callback
   *
   * \param on_message The callback to call on new messages
   * \spec
   *   requires true;
   * \endspec
   */
  void SetOnMessage(OnMessage&& on_message) noexcept {
    std::lock_guard<Guard> const guard{this->message_guard_};
    this->on_message_ = std::move(on_message);
  }

  /*!
   * \brief Removes the currently set on message callback.
   * \spec
   *   requires true;
   * \endspec
   */
  void ClearOnMessage() noexcept {
    std::lock_guard<Guard> const guard{this->message_guard_};
    this->on_message_ = [](T const* const) { return false; };
  }

 private:
  /*!
   * \brief Try to send the value to the user.
   *
   * \tparam S    The synchronization strategy.
   *              Must be sync::Strategy::kMutex.
   * \param value The value to push.
   * \return      True in case the circumvention was successful.
   *
   * \internal
   *   - #10 If we get exclusive access to the message callback.
   *    - #20 Then alert the user about this message.
   *   - #30 Return the result of the operation.
   * \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  template <sync::Strategy S = kSyncStrategy, std::enable_if_t<S == sync::Strategy::kMutex, std::uint8_t> = 0>
  auto PushToUser(T& value) noexcept -> bool {
    bool result{false};
    if (sync::TryLock<Guard> const guard{this->message_guard_}) {
      result = this->on_message_(&value);
    }
    return result;
  }

  /*!
   * \brief Try to send the value to the user.
   *
   * \tparam S    The synchronization strategy.
   *              Must be sync::Strategy::kEmpty.
   * \param value The value to push.
   * \return      True in case the circumvention was successful.
   *
   * \internal
   *   - #10 Alert the user about this message.
   *   - #20 Return the result of the operation.
   * \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  template <sync::Strategy S = kSyncStrategy, std::enable_if_t<S == sync::Strategy::kEmpty, std::uint8_t> = 0>
  auto PushToUser(T& value) const noexcept -> bool {
    return this->on_message_(&value);
  }

  /*!
   * \brief Add a value to the queue
   *
   * \param value The value to add
   * \return      Nothing or the error that occurred.
   *
   * \internal
   *   - #10 Get exclusive access to the queue.
   *    - #20 If there's space for more elements.
   *     - #30 Store the element and unlock the queue guard.
   *     - #40 Get exclusive access to the message callback.
   *     - #50 Notify the user about a new message in the queue.
   *   - #30 Return the result of the operation.
   * \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  auto PushToQueue(T&& value) noexcept -> core::Result<void> {
    core::Result<void> result{};

    this->queue_guard_.lock();
    if (this->queue_.size() < this->capacity_) {
      // TODO(visshj): Push may throw. Once a libVac alternative is ready, use its return value instead.
      // VCA_LOGAPI_CHECKED_CONTRACT
      this->queue_.push_back(std::move(value));
      this->queue_guard_.unlock();
      {
        std::lock_guard<Guard> const guard{this->message_guard_};
        this->CallOnMessage();
      }
    } else {
      this->queue_guard_.unlock();
      result.EmplaceError(Err(LogErrc::kBufferTooSmall, "Message could not be added to queue"));
    }

    return result;
  }

  /*!
   * \brief Calls the on message callback if it is set.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_M0.1.8_DefaultCallback
  void CallOnMessage() const noexcept { this->on_message_(nullptr); }
};

}  // namespace mpsc
}  // namespace internal
}  // namespace log
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::log::internal::sink::Queue::queue_guard_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::sink::Queue::queue_
 * \usedin          ::amsr::log::internal::sink::Queue::Pop
 *                  ::amsr::log::internal::sink::Queue::PushToQueue
 * \exclude         All other functions of Queue
 * \length          MEDIUM
 *                  Lock active during all access to the queue.
 * \endexclusivearea
 */
/*!
 * \exclusivearea   ::amsr::log::internal::sink::Queue::message_guard_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::sink::Queue::on_message_
 * \usedin          ::amsr::log::internal::sink::Queue::SetOnMessage
 *                  ::amsr::log::internal::sink::Queue::ClearOnMessage
 *                  ::amsr::log::internal::sink::Queue::PushToUser
 *                  ::amsr::log::internal::sink::Queue::PushToQueue
 * \exclude         All other functions of Queue
 * \length          MEDIUM
 *                  Lock active during all access to the on_message callback.
 * \endexclusivearea
 */

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_QUEUE_H_
