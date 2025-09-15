/**********************************************************************************************************************
 *  COPYRIGHT
 *
 -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *
 -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  resource_usage.h
 *        \brief  ResourceUsageAPI
 *         \unit  osabstraction::resource_usage
 *   \complexity  resource_usage unit is QM, no action required.
 *
 *********************************************************************************************************************/
#ifndef LIB_RESOURCEUSAGE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_RESOURCE_USAGE_RESOURCE_USAGE_H_
#define LIB_RESOURCEUSAGE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_RESOURCE_USAGE_RESOURCE_USAGE_H_

#include "amsr/core/result.h"
#include "amsr/core/vector.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/process/process_native_types.h"

namespace amsr {
namespace resource_usage {

/*!
 * \brief Type for the running processes list returned by GetAllPIDs().
 * \vpublic
 */
using RunningProcessList = ::amsr::core::Vector<osabstraction::process::ProcessId>;

/*!
 * \brief Type representing cpu time in nanoseconds.
 * \vpublic
 */
using CpuTime = std::uint64_t;

/*!
 * \brief           Returns list of running PIDs.
 *
 * \return          List of running processes identifiers.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetAllProcessesIds
 *
 * \vpublic
 */
::amsr::core::Result<RunningProcessList> GetAllPIDs() noexcept;

/*!
 * \brief           Returns number of Cpu cores in the machine.
 *
 * \return          Number of available Cpu cores.
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetCpuCoresNumber
 *
 * \vpublic
 */
::amsr::core::Result<std::uint16_t> GetCpuCoresNumber() noexcept;

/*!
 * \brief           Returns the current idle time of all cores in the machine.
 *
 * \return          Current idle time of all cores in nanoseconds.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     Requested operation is invalid or not supported
 *                                                                       or operation failed because of a hardware error
 * \error           osabstraction::OsabErrc::kBusy                       Device is temporarily unavailable
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory
 * \error           osabstraction::OsabErrc::kSize                       (QNX Only) Overflow of addition of system space
 *                                                                       and user space nanoseconds of total
 *                                                                       Cpu idle time
 *                                                                       (Linux Only) Overflow while converting
 *                                                                       clock ticks to nano seconds
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetMachineCpuIdleTime
 *
 * \vpublic
 */
::amsr::core::Result<CpuTime> GetMachineCpuIdleTime() noexcept;

/*!
 * \brief           Returns the current idle time of a specific core in the machine.
 *
 * \param[in]       core_number  Cpu core number. Core numbers start at 0.
 *
 * \return          Current idle time of a specific core in nanoseconds.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     Requested operation is invalid or not supported
 *                                                                       or operation failed because of a hardware error
 * \error           osabstraction::OsabErrc::kBusy                       Device is temporarily unavailable
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory or ClockId is invalid
 * \error           osabstraction::OsabErrc::kApiError                   (Linux Only) Input core number is invalid
 * \error           osabstraction::OsabErrc::kSize                       (Linux Only) Overflow while converting
 *                                                                       clock ticks to nano seconds
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetCoreCpuIdleTime
 *
 * \vpublic
 */
::amsr::core::Result<CpuTime> GetCoreCpuIdleTime(std::uint16_t core_number) noexcept;

/*!
 * \brief           Returns the time the process spent in user space + in system space.
 *
 * \param[in]       process_pid  Process identifier.
 *
 * \return          Time the process spent in user and system spaces in nanoseconds.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     (Linux Only) Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     (Linux Only) Kernel does not support
 *                                                                       obtaining the per-process CPU-time
 *                                                                       clock of another process.
 * \error           osabstraction::OsabErrc::kDoesNotExist               Process is not running.
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetProcessCpuUsage
 *
 * \vpublic
 */
::amsr::core::Result<CpuTime> GetProcessCpuUsage(osabstraction::process::ProcessId process_pid) noexcept;

/*!
 * \brief Type representing memory unit in kilobytes.
 * \vpublic
 */
using MemoryUnit = std::uint64_t;

/*!
 * \brief           Returns the total memory in the machine.
 *
 * \return          Total memory in the machine.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 (Linux Only) Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     (Linux Only) Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     (Linux Only) Requested operation is invalid or
 *                                                                        not supported or operation failed because of a
 *                                                                        hardware error
 * \error           osabstraction::OsabErrc::kResource                   (Linux Only) Insufficient memory
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetMachineTotalMemory
 *
 * \vpublic
 */
::amsr::core::Result<MemoryUnit> GetMachineTotalMemory() noexcept;

/*!
 * \brief           Returns the free memory in the machine.
 *
 * \return          Free memory in the machine.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     Requested operation is invalid or not supported
 *                                                                       or operation failed because of a hardware error
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetMachineFreeMemory
 *
 * \vpublic
 */
::amsr::core::Result<MemoryUnit> GetMachineFreeMemory() noexcept;

/*!
 * \brief           Returns the consumed memory by specific process.
 *
 * \param[in]       process_pid  Process identifier.
 *
 * \return          Consumed memory by specific process.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     Requested operation is invalid or not supported
 *                                                                       or operation failed because of a hardware error
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory
 * \error           osabstraction::OsabErrc::kBusy                       (QNX Only) Device is temporarily unavailable
 * \error           osabstraction::OsabErrc::kSize                       Overflow of addition of different memory
 *                                                                       segments which the process consumed
 * \error           osabstraction::OsabErrc::kInvalidHandle              PID is incorrect. Its size is larger than max
 *                                                                       allowed path length.
 * \error           osabstraction::OsabErrc::kDoesNotExist               (Linux Only) Process is not running.
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-ResourceUsage-GetProcessMemoryUsage
 *
 * \vpublic
 */
::amsr::core::Result<MemoryUnit> GetProcessMemoryUsage(osabstraction::process::ProcessId process_pid) noexcept;

}  // namespace resource_usage
}  // namespace amsr

#endif  // LIB_RESOURCEUSAGE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_RESOURCE_USAGE_RESOURCE_USAGE_H_
