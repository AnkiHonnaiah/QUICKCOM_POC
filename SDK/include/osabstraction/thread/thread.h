/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  This file implements an abstraction for some POSIX thread API's.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "vac/container/c_string_view.h"

#include "osabstraction/osab_error_domain.h"
#include "osabstraction/thread/internal/thread_internal.h"
#include "osabstraction/thread/thread_types.h"

namespace osabstraction {
namespace thread {

/*!
 * \brief           Sets name for the given thread.
 *
 * \param[in]       thread_handle Native handle of the thread to whom a name should be set.
 * \param[in]       thread_name C-string representing the thread's name.
 *
 * \return          An empty result if successful.
 *
 *
 * \error           osabstraction::OsabErrc::kApiError  The length of thread_name exceeds the allowed limit.
 *
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The process information pseudo-filesystem could
 *                                                                    not be opened. The specific system error code is
 *                                                                    provided in the support data.
 *
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Lacking privileges to set name.
 *
 * \context         ANY
 *
 * \pre             Length of thread_name is restricted to 16 characters, including the terminating null byte ('\0').
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace           DSGN-Osab-PthreadSetName
 */
Result SetNameOfThread(ThreadNativeHandle thread_handle, vac::container::CStringView const& thread_name) noexcept;

/*!
 * \brief           Sets name for the given thread.
 *
 * \param[in]       thread_handle Native handle of the thread to whom a name should be set.
 * \param[in]       thread_name C-string representing the thread's name.
 *
 * \return          An empty result if successful.
 *
 *
 * \error           osabstraction::OsabErrc::kApiError  The length of thread_name exceeds the allowed limit.
 *
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The process information pseudo-filesystem could
 *                                                                    not be opened. The specific system error code is
 *                                                                    provided in the support data.
 *
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Lacking privileges to set name.
 *
 * \context         ANY
 *
 * \pre             Length of thread_name is restricted to 16 characters, including the terminating null byte ('\0').
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace           DSGN-Osab-PthreadSetName
 */
Result SetNameOfThread(ThreadNativeHandle thread_handle, amsr::core::String const& thread_name) noexcept;

/*!
 * \brief           Get the number of processors (currently) available in the system.
 *
 * \details         Depending on the operating system, the output might represent either the static number of processors
 *                  configured by the system or the count of currently available processors. In certain cases, the count
 *                  of available processors might be lower than the total number of processors configured by the
 *                  operating system. This can occur due to certain processors being offline, especially in systems that
 *                  support hotplugging, where processors can be dynamically added or removed. It is important to
 *                  consider that the return value should be treated as a hint due to potential variations based on the
 *                  operating system's settings and dynamic system changes.
 *                  Calling this routine is quite expensive. Therefore, it should not be called repeatedly.
 *
 * \return          The number of processors (currently) available in the system.
 *
 * \error           osabstraction::OsabErrc::kUnexpected  Unexpected error has occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace           DSGN-Osab-ThreadGetAvailableProcessors
 */
amsr::core::Result<std::int64_t> GetNumberOfAvailableProcessors() noexcept;

/*!
 * \brief           Determines the thread name visible in the kernal and its interfaces.
 *
 * \details         This function is a wrapper of pthread_getname_np().
 *
 * \return          Returns the thread name visible in the kernal and its interfaces.
 *
 * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error.
 * \error           osabstraction::OsabErrc::kSize                     The buffer length is too small to store the
 *                                                                     thread name.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges   (QNX only) Not allowed to get the thread name.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-PthreadGetName
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<ThreadNameStorage> GetThreadName() noexcept;

/*!
 * \brief           Updates the nice value for the calling thread.
 * \details         Sets the nice value if supported by the underlying OS.
 * \param[in]       nice_value       Nice value to set for this thread.
 * \return          -
 * \error           OsabErrc::kApiError    The nice value is not set due to validation error.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-Thread-NiceValue
 *
 * \vprivate        Vector product internal API
 */
::amsr::core::Result<void> SetNiceValue(NiceValue nice_value) noexcept;

/*!
 * \brief           Validate the nice value against given scheduling settings.
 * \param[in]       nice_value       nice value to check.
 * \param[in]       sched_policy     Optional Scheduling Policy to check.
 * \param[in]       sched_priority   Optional Scheduling Priority to check.
 * \return          empty result if valid scheduling settings are provided, error otherwise
 * \error           OsabErrc::kApiError      Nice value is not valid with respect to the scheduling parameters.
 *
 * \context         InitPhase
 *
 * \pre             -
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-Thread-NiceValue
 *
 * \vprivate        Vector product internal API
 */
::amsr::core::Result<void> ValidateNiceValue(NiceValue nice_value, OptionalSchedulingPolicies sched_policy,
                                             OptionalSchedulingPriority sched_priority) noexcept;

}  // namespace thread
}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_THREAD_H_
