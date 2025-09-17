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
/**     \file       producer.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::MPSC::Producer
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_PRODUCER_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_PRODUCER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/log/internal/mpsc/mpsc_fwd.h"
#include "amsr/log/internal/mpsc/queue.h"

namespace amsr {
namespace log {
namespace internal {
namespace mpsc {

/*!
 * \brief A Producer of a "Multiple Producer, Single Consumer" queue
 *
 * \tparam T      The contained value type.
 *                Must comply to the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 * \tparam thread Thread safety setting.
 *                Must comply to the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 */
template <typename T, sync::Strategy thread>
class Producer final {
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
   * \brief Move allowed
   * \spec
   *   requires true;
   * \endspec
   */
  Producer(Producer&&) noexcept = default;
  /*!
   * \brief Move allowed
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  auto operator=(Producer&&) & noexcept -> Producer& = default;
  /*!
   * \brief Copy allowed because it's "multiple producer"
   * \spec
   *   requires true;
   * \endspec
   */
  Producer(Producer const&) noexcept = default;
  /*!
   * \brief Copy allowed because it's "multiple producer"
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Producer const&) & noexcept -> Producer& = default;
  /*!
   * \brief Default destructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~Producer() noexcept = default;

  /*!
   * \brief Add a value to the function
   *
   * \param value A value to push into the queue
   * \spec
   *   requires true;
   * \endspec
   */
  auto Push(T&& value) const noexcept -> Result<void> {
    // VCA_LOGAPI_VALID_OBJECT
    return this->queue_->Push(std::move(value));
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
  explicit Producer(std::shared_ptr<Q> queue) noexcept : queue_{queue} {}
};
}  // namespace mpsc
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_PRODUCER_H_
