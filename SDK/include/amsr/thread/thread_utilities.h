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
/*!        \file  thread_utilities.h
 *         \brief
 *         \unit amsr::thread::ThreadUtilities
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_UTILITIES_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_UTILITIES_H_

#include "ara/core/result.h"
#include "osabstraction/thread/thread.h"

namespace amsr {
namespace thread {

/*!
 * \brief           Get the number of processors (currently) available in the system.
 * \details         Depending on the operating system, the output might represent either the static number of processors
 *                  configured by the system or the count of currently available processors. In certain cases, the count
 *                  of available processors might be lower than the total number of processors configured by the
 *                  operating system. This can occur due to certain processors being offline, especially in systems that
 *                  support hotplugging, where processors can be dynamically added or removed. It is important to
 *                  consider that the return value should be treated as a hint due to potential variations based on the
 *                  operating system's settings and dynamic system changes.
 *                  Calling this routine is quite expensive. Therefore, it should not be called repeatedly.
 * \return          number of processors available in the system.
 * \error           osabstraction::OsabErrc::kUnexpected Unexpected error occurred.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 * \trace            DSGN-VectorThreadLibrary-GetNumberOfAvailableProcessors
 */
/*!
 * \internal
 *  - Get and return the result of the available processors.
 * \endinternal
 */
inline ara::core::Result<std::int64_t> GetNumberOfAvailableProcessors() noexcept {
  return osabstraction::thread::GetNumberOfAvailableProcessors();
}

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_UTILITIES_H_
