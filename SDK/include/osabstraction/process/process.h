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
/*!        \file  libosabstraction-posix/include/osabstraction/process/process.h
 *        \brief  Process API - Abstraction of Process management for POSIX.
 *
 *      \details  Intended for use by other components.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unistd.h>
#include "amsr/core/result.h"
#include "osabstraction/process/os_process_settings.h"
#include "osabstraction/process/process_types.h"
#include "osabstraction/process/processinterface.h"

namespace osabstraction {
namespace process {

/*!
 * \brief   Representation of one Process in a POSIX OS.
 *
 * \details A process is an instance of an executable image that is being executed. It has its own address space and a
 *          set of system resources. An object of this class represents one running process. It allows to stop the
 *          process and acquire some information about it.
 */
class OsProcess final {
 public:
  /*!
   * \brief           Starts a process.
   *
   * \details         Makes all required preparations, starts a new process and makes it execute the desired executable.
   *
   *                  Note that CreateProcess() is in general only safe to be called from a single threaded process or a
   *                  process that has only a main thread and additional OsAbstraction internal threads.
   *                  Contact CT Exec before if you intend to call CreateProcess() from a process with additional
   *                  threads (ESCAN00111278).
   *
   * \note            A successful return of this function does not guarantee that the execution of the specified
   *                  executable in the process will start. In case the execution can not be started, the created
   *                  process will abnormally terminate.
   *
   * \param[in]       executable_path           The absolute path of the executable that shall be executed. This must
   *                                            not be changed for the lifetime of this class.
   * \param[in]       name                      Name that ends up in argv[0]. This must not be changed for the
   *                                            lifetime of this class.
   * \param[in]       working_dir               The absolute path of the working directory. This must not be changed
   *                                            for the lifetime of this class.
   * \param[in, out]  settings                  The settings to create a process. This must not be changed for the
   *                                            lifetime of this class.
   *
   * \return          OsProcess with the started process handle.
   *
   * \error           OsabErrc::kProcessCreationFailed               Process could not be started. Check support data
   *                                                                 for more information.
   * \error           OsabErrc::kProcessCreationFailedFatal          Process could not be started. The system is
   *                                                                 irreversibly corrupted, end execution. Check
   *                                                                 support data for more information.
   * \error           OsabErrc::kProcessCreationFailedMultithreaded  (QNX only) Process could not be started because of
   *                                                                 a conflicting operation in another thread.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreation-Linux
   * \trace           DSGN-Osab-Process-ProcessCreation-QNX
   * \trace           DSGN-Osab-Process-ProcessCreationMandatorySettings
   * \trace           DSGN-Osab-Process-MachineInitApp
   * \trace           DSGN-Osab-Process-RetrieveProcessHandle
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<OsProcess> CreateProcess(PathToExecutable const& executable_path,
                                                       ExecutableName const& name, WorkingDirectory const& working_dir,
                                                       OsProcessSettings& settings);

  /*!
   * \brief           Kills the process.
   *
   * \details         If the process is running, it is killed.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessForcefulTermination
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~OsProcess() noexcept {
    Kill();  // VCA_OSA_CALL_BY_PTR_PASS_VOID
  }

  /*!
   * \brief  Move Constructor.
   *
   * \param           other     OsProcess to move-construct from.
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
   * \vprivate        Vector product internal API
   */
  OsProcess(OsProcess&& other) noexcept {
    std::swap(is_running_, other.is_running_);
    std::swap(pid_, other.pid_);
  }

  OsProcess() = delete;
  OsProcess(OsProcess const& other) = delete;
  OsProcess& operator=(OsProcess const& other) = delete;
  OsProcess& operator=(OsProcess&& other) = delete;

  /*!
   * \brief           Send a SIGTERM signal to the process.
   *
   * \details         SIGTERM does not forcefully terminate the process, the reaction is dependent on the program which
   *                  is executed by the process.
   *                  Should the process not be running, this function does nothing.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessTermination
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SendTerminationRequest() const;

  /*!
   * \brief           Returns the operating system process ID.
   *
   * \return          The process ID.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-GetPid
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ProcessId GetId() const noexcept { return pid_; }

  /*!
   * \brief           Returns the process handle.
   *
   * \details         The process handle is identical to the process ID in this implementation.
   *
   * \return          The process handle.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-RetrieveProcessHandle
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ProcessHandle GetHandle() const noexcept { return pid_; }

  /*!
   * \brief           Marks this process as no longer running.
   *
   * \details         SendTerminationRequest() and Kill() do not have an effect on processes that are not running.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessTermination
   * \trace           DSGN-Osab-Process-ProcessForcefulTermination
   * \trace           DSGN-Osab-Process-ProcessStateMonitoring
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void OnTerminated() noexcept { is_running_ = false; }

  /*!
   * \brief           Send a SIGKILL signal to the process.
   *
   * \details         SIGKILL terminates the process ungracefully.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             Process must be running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessForcefulTermination
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void Kill() noexcept;

  /*!
   * \brief           Sets the passed running abilities to this process.
   * \details         Implemented only on QNX.
   * \param[in, out]  settings                  The settings where the running abilities have been stored for the target
   *                                            process.
   * \return          empty result or an error.
   * \error           OsabErrc::kApiError                error during ability creation/lookup, or invalid pid passed
   *                                                     to procmgr_ability().
   * \error           OsabErrc::kInsufficientPrivileges  Not enough privileges to execute procmgr_ability()
   *
   * \context         ANY|!InitPhase
   *
   * \pre             Process must be running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessRunningQNXAbility
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<void> SetRunningAbilities(OsProcessSettings const& settings) const noexcept;

 private:
  /*!
   * \brief           Creates an object representing an already started process.
   *
   * \param[in]       process_handle   The process_handle of the started process.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreation-Linux
   * \trace           DSGN-Osab-Process-ProcessCreation-QNX
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  explicit OsProcess(ProcessHandle process_handle) : pid_(process_handle), is_running_(true) {}

  /*!
   * \brief   Process ID (also process handle).
   */
  ProcessId pid_{kInvalidProcessId};

  /*!
   * \brief   True if the process is running. Otherwise false.
   * \details Default value is given as 'false' because inside the move constructor the values of the default
   *          initialized object is swapped with the incoming object and if the default initialized value is 'true',
   *          during destruction, kill() with the uninitialized pid will be called.
   */
  bool is_running_{false};
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
