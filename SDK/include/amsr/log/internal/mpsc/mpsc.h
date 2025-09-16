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
/**     \file       mpsc.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/log/internal/mpsc/consumer.h"  // IWYU pragma: export
#include "amsr/log/internal/mpsc/mpsc_fwd.h"  // IWYU pragma: keep
#include "amsr/log/internal/mpsc/producer.h"  // IWYU pragma: export
#include "amsr/log/internal/mpsc/queue.h"     // IWYU pragma: keep

namespace amsr {
namespace log {
namespace internal {
namespace mpsc {

/*!
 * \brief   Factory method to create a "Multiple Producer, Single Consumer" queue
 *
 * \tparam  T         The value type to store.
 *                    Must meet the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 * \tparam  Thread    Thread safety setting.
 *                    Must meet the requirements of amsr::log::internal::mpsc::Queue<T, thread>.
 *
 * \param   buf_size  The size of the buffer queue
 *
 * \return  A pair of producer & consumer
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, sync::Strategy thread>
auto MakeMPSC(std::uint32_t buf_size) noexcept -> std::pair<Producer<T, thread>, Consumer<T, thread>> {
  // VECTOR NC AutosarC++17_10-A18.5.8: MD_LOG_AutosarC++17_10-A18.5.8_false_positive_local_heap
  // VCA_LOGAPI_CHECKED_CONTRACT
  std::shared_ptr<Queue<T, thread>> queue{std::make_shared<Queue<T, thread>>(buf_size)};

  // VCA_LOGAPI_CHECKED_CONTRACT
  Producer<T, thread> producer{queue};
  // VCA_LOGAPI_CHECKED_CONTRACT
  Consumer<T, thread> consumer{std::move(queue)};
  return std::make_pair(std::move(producer), std::move(consumer));
  // VCA_LOGAPI_CHECKED_CONTRACT
}

}  // namespace mpsc
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_H_
