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
/*!        \file  thread_types.h
 *        \brief  Public thread data types.
 *
 *      \details  Intended for use by other components.
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_TYPES_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <pthread.h>
#include <array>
#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

namespace osabstraction {
namespace thread {

/*!
 * \brief           Definition of thread's native handle type for POSIX OS's.
 *
 * \vprivate        Vector product internal API
 */
using ThreadNativeHandle = pthread_t;  // VECTOR SL AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_PosixApi

/*!
 * \brief           Result type is used to represent return value of osabstraction::thread API's.
 *
 * \vprivate        Vector product internal API
 */
using Result = ::amsr::core::Result<void>;

/*!
 * \brief Provide a maximum length for the name of a thread.
 * \details The given size defines the number of usable characters excluding zero termination.
 *
 * \vprivate        Vector product internal API
 */
constexpr std::size_t kMaxSizeThreadName{15};

/*!
 * \brief Alias for the storage of the thread name.
 *
 * \vprivate        Vector product internal API
 */
using ThreadNameStorage = std::array<char, kMaxSizeThreadName + 1>;

/*!
 * \brief The supported scheduling policies.
 */
enum class SchedulingPolicies : std::int32_t {
  kOther = SCHED_OTHER,  /*!< No real-time: OS-specific policy. Might be the same as SCHED_FIFO or SCHED_RR. */
  kFifo = SCHED_FIFO,    /*!< Real-time: First in-first out (FIFO) scheduling policy. */
  kRoundRobin = SCHED_RR /*!< Real-time: Round robin scheduling policy. */
};

/*!
 * \brief Optional Scheduling policies
 */
using OptionalSchedulingPolicies = ::amsr::core::Optional<SchedulingPolicies>;

/*!
 * \brief Process scheduling priority.
 */
using SchedulingPriority = int;  // VECTOR Same Line AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief Process nice value.
 */
using NiceValue = int;  // VECTOR Same Line AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief Optional process scheduling priority.
 */
using OptionalSchedulingPriority = ::amsr::core::Optional<SchedulingPriority>;

/*!
 * \brief Optional process nice value.
 */
using OptionalNiceValue = ::amsr::core::Optional<NiceValue>;

}  // namespace thread
}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_TYPES_H_
