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
/*!        \file  processinterface.h
 *        \brief  Process API - Abstraction of Process management.
 *        \unit osabstraction::Process_Linux
 *        \unit osabstraction::Process_PikeOS
 *        \unit osabstraction::Process_QNX
 *
 *      \details  Intended for use by other components.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/result.h"

#include "osabstraction/process/process_types.h"

namespace osabstraction {
namespace process {

/*!
 * \brief           Initializes the OS Abstraction process subcomponent.
 *
 * \details         This function has to be called before the process API can be used. It does nothing and always
 *                  returns success on operating systems where no initialization is necessary. This initialization
 *                  cannot be retried if it failed.
 *
 * \param[in]       max_num_processes   Maximum number of processes that will be created.
 *
 * \return          true if the initialization succeeded or this function was called before, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-Process-Initialization
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
bool Initialize(std::size_t max_num_processes) noexcept;

/*!
 * \brief           Returns the operating system process ID of the calling process.
 *
 * \return          Process ID of the calling process.
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
 * \trace           DSGN-Osab-Process-GetPid
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
ProcessId GetProcessId() noexcept;

/*!
 * \brief           Determines the integrity level of the current process.
 *
 * \details         The integrity level of a process cannot be configured on Linux. As such, every process is treated as
 *                  a QM process on Linux and this function then always returns IntegrityLevel::kQm.
 *
 * \return          The integrity level of the current process.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error.
 * \error           osabstraction::OsabErrc::kResource                Insufficient resources.
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
 * \trace           DSGN-Osab-Process-GetOwnIntegrityLevel
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<IntegrityLevel> GetOwnIntegrityLevel() noexcept;

/*!
 * \brief           Returns the process handle of the calling process.
 *
 * \details         The process handle is an unique identifier for all processes created by OsProcess::CreateProcess().
 *
 * \return          Process handle of the calling process, kInvalidProcessHandle if the process handle is not available.
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
 * \trace           DSGN-Osab-Process-RetrieveProcessHandle
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
ProcessHandle RetrieveProcessHandle() noexcept;

/*!
 * \brief Options for parameter of WaitForChildTermination().
 */
enum class WaitForChildTerminationOption : std::uint8_t {
  /*!
   * \brief Wait for child termination in blocking manner.
   */
  kBlockingWait,
  /*!
   * \brief Wait for child termination in non-blocking manner.
   */
  kNonBlockingWait
};

/*!
 * \brief           Checks if an up to now unchecked child process has terminated and returns information about this
 *                  terminated child.
 *
 * \details         The function can either wait for the termination of a child process if no child process is to be
 *                  processed (blocking) or return immediately (non-blocking).
 *
 * \note            The calling process must have at least one child process when calling this function.
 *
 * \param[in]       options                   Selects if the function shall be blocking or unblocking.
 *
 * \return          Process handle, OS process ID and exit status of the child process that terminated. If the function
 *                  was selected to be non-blocking and there is no child process to be processed,
 *                  kInvalidProcessHandle as process handle, kInvalidProcessId as process ID and 0 as exit status are
 *                  returned.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error.
 * \error           osabstraction::OsabErrc::kDoesNotExist            Calling process does not have child processes.
 * \error           osabstraction::OsabErrc::kBusy                    Function was interrupted by a signal.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  QNX only: Caller lacks required permissions.
 *
 * \context         ANY
 *
 * \pre             Calling process must have one or more child processes.
 * \pre             On QNX: process must have ability PROCMGR_AID_WAIT enabled.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \trace           DSGN-Osab-Process-AwaitChildTermination
 *
 * \vprivate        Vector product internal API
 */
::amsr::core::Result<osabstraction::process::ProcessStatus> WaitForChildTermination(
    WaitForChildTerminationOption options = WaitForChildTerminationOption::kBlockingWait) noexcept;

/*!
 * \brief           Checks if a process exit code of a process indicates successful termination.
 *
 * \param[in]       exit_code                 The exit code to check.
 *
 * \return          true if the exit code indicates that the process terminated successfully.
 *                  false if the exit code indicates that the process has not terminated or terminated unsuccessfully.
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
 *
 * \spec
 *   requires true;
 * \endspec
 */
bool TerminatedSuccessfully(ExitCodeType exit_code) noexcept;

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_
