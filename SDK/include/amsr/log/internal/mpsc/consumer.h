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
/**     \file       consumer.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::MPSC::Consumer
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_CONSUMER_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_CONSUMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/log/internal/mpsc/mpsc_fwd.h"
#include "amsr/log/internal/mpsc/queue.h"
#include "amsr/log/internal/utils.h"

namespace amsr {
namespace log {
namespace internal {
namespace mpsc {

/*!
 * \brief A Consumer of a "Multiple Producer, Single Consumer" queue
 *
 * \tparam T      The contained value type.
 *                Must comply to the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 * \tparam thread Thread safety setting.
 *                Must comply to the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 */
template <typename T, sync::Strategy thread>
class Consumer final {
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Friend declaration for factory function
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T1, sync::Strategy thread1>
  friend auto MakeMPSC(std::uint32_t buf_size) noexcept -> std::pair<Producer<T1, thread1>, Consumer<T1, thread1>>;

  /*!
   * \brief The internally used queue
   */
  using Q = Queue<T, thread>;

  /*!
   * \brief Shared queue between producer and consumer
   */
  std::shared_ptr<Q> queue_;

 public:
  /*!
   * \brief Unqualified access to the optional
   */
  using ValueOption = Optional<T>;

  /*!
   * \brief Move allowed
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOVE_CONSTRUCTOR, VCA_LOGAPI_CHECKED_CONTRACT
  Consumer(Consumer&&) noexcept = default;
  /*!
   * \brief Move allowed
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  auto operator=(Consumer&&) & noexcept -> Consumer& = default;
  /*! \brief Copy prohibited because it's "single consumer" */
  Consumer(Consumer const&) = delete;
  /*! \brief Copy prohibited because it's "single consumer" */
  auto operator=(Consumer const&) -> Consumer& = delete;
  /*!
   * \brief Default destructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~Consumer() noexcept = default;

  /*!
   * \brief Returns the next value from the queue if it exists.
   *
   * \return A value from the queue or nothing.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_OBJECT_IS_VALID
  auto Pop() const noexcept -> ValueOption { return this->queue_->Pop(); }

  /*!
   * \brief Set the on message callback
   *
   * \param on_message The callback to call on new messages
   * \spec
   *   requires true;
   * \endspec
   */
  void SetOnMessage(typename Q::OnMessage on_message) const noexcept {
    // VCA_LOGAPI_VALID_OBJECT
    this->queue_->SetOnMessage(std::move(on_message));
  }

  /*!
   * \brief Removes the currently set on message callback
   * \spec
   *   requires true;
   * \endspec
   */
  void ClearOnMessage() const noexcept {
    // VCA_LOGAPI_VALID_OBJECT
    this->queue_->ClearOnMessage();
  }

 private:
  /*!
   * \brief Value constructor that is called by MakeMPSC
   *
   * \param queue The internal queue which is shared between Producer & Consumer
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Consumer(std::shared_ptr<Q> queue) noexcept : queue_{queue} {}
};
}  // namespace mpsc
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_CONSUMER_H_
