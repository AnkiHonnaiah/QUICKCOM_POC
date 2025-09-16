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
/*!        \file  process_types.h
 *        \brief  Public data types which are used by Process API.
 *
 *      \details  Intended for use by other components.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <map>
#include "amsr/core/span.h"

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"

#include "amsr/ipc/integrity_level.h"
#include "osabstraction/process/cpu_core_control_interface.h"
#include "osabstraction/process/process_native_types.h"
#include "osabstraction/process/resource_group.h"
#include "osabstraction/thread/thread_types.h"

namespace osabstraction {
namespace process {

/*!
 * \brief   Handle for a process created by OsProcess::CreateProcess().
 * \details All child processes get a process handle assigned when they are created with OsProcess::CreateProcess().
 *          This handle is guaranteed to be unique for all created child processes.
 */
using ProcessHandle = pid_t;  // VECTOR Same Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief Invalid process handle.
 */
constexpr ProcessHandle kInvalidProcessHandle{-1};

/*!
 * \brief   Operating system process identifier.
 * \details Some operating systems may reuse process IDs in a way it cannot be guaranteed that all created OsProcess
 *          objects have unique process IDs.
 */
using ProcessId = pid_t;  // VECTOR Same Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling

/*!
 * \brief Invalid operating system process identifier.
 */
constexpr ProcessId kInvalidProcessId{-1};

/*!
 * \brief Data type to specify the path of the executable which shall be executed in a process.
 */
using PathToExecutable = amsr::core::String;

/*!
 * \brief Data type to specify the name that should be passed as argv[0] to the process.
 */
using ExecutableName = amsr::core::String;

/*!
 * \brief Data type to specify the name that should be passed as argv[0] to the process.
 */
using AbsoluteBinaryPath = ::amsr::core::Span<char>;

/*!
 * \brief Data type to specify the current working directory of a process.
 */
using WorkingDirectory = amsr::core::String;

/*!
 * \brief Data type to specify a command line argument which can be passed to a process.
 */
using Argument = amsr::core::String;

/*!
 * \brief A list of command line arguments.
 */
using Arguments = ara::core::Vector<Argument>;

/*!
 * \brief   The Scheduling settings available for processes
 *
 * \details Contains scheduling policy and priority
 */
struct SchedulingSettings {
  /*!
   * \brief Scheduling policies
   */
  osabstraction::thread::SchedulingPolicies policies;

  /*!
   * \brief Scheduling priority
   */
  osabstraction::thread::SchedulingPriority priority;
};

/*!
 * \brief Optional process scheduling settings.
 */
using OptionalSchedulingSettings = ::amsr::core::Optional<SchedulingSettings>;

static_assert(sizeof(UserId) == 4, "User identifier is expected to be 32 bit.");
static_assert(sizeof(GroupId) == 4, "Group identifier is expected to be 32 bit.");

/*!
 * \brief Optional CPU affinity
 */
using OptionalCpuAffinity = ::amsr::core::Optional<CpuAffinity>;

/*!
 * \brief Type for the optional user identifier.
 */
using OptionalUserId = ::amsr::core::Optional<UserId>;
/*!
 * \brief Type for the optional group identifier.
 */
using OptionalGroupId = ::amsr::core::Optional<GroupId>;
/*!
 * \brief Type for the list of group identifiers.
 */
using GroupIdList = ara::core::Vector<GroupId>;
/*!
 * \brief Type for the optional list of group identifiers.
 */
using OptionalGroupIdList = ::amsr::core::Optional<GroupIdList>;

/*!
 * \brief Type for the optional resource group.
 */
using OptionalResourceGroup = ::amsr::core::Optional<ResourceGroup>;

/*!
 * \brief   Process Status
 *
 * \details This type provides information about Process termination.
 */
struct ProcessStatus {
  /*!
   * \brief Handle of the Process which terminated.
   */
  ProcessHandle process_handle;
  /*!
   * \brief Operating system ID of the Process which terminated.
   */
  ProcessId pid;
  /*!
   * \brief Status which encodes termination details (e.g. signal or exit code).
   */
  ExitCodeType status;
};

/*!
 * \brief   Environment Variable
 */
using EnvironmentVar = amsr::core::String;

/*!
 * \brief   A map of environment variables.
 *
 * \details Contains the environment variables with the environment variable name as key and a string that contains
 *          the environment variable name followed by an equals character followed by the environment variable value
 *          as value
 */
using EnvironmentVars = std::map<amsr::core::String, EnvironmentVar>;

/*!
 * \brief   Data type to specify the name of the resource limit.
 * \details Implemented only on QNX, Linux.
 */
using ResourceLimitName = ::amsr::core::String;

/*!
 * \brief   Data type to specify the value of the resource limit.
 * \details Negative values have special meaning (i.e. RLIM_INFINITY defines "no limit"
 *          and is mapped to -1 on Linux and to -3 on QNX-64bit).
 *          Implemented only on QNX, Linux
 */
using ResourceLimitValue = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief   Type for security policy identifier.
 */
using SecurityPolicyId = amsr::core::String;

/*!
 * \brief Type for the optional security policy identifier.
 */
using OptionalSecurityPolicyId = ::amsr::core::Optional<SecurityPolicyId>;

/*!
 * \brief Integrity level of a process.
 */
using IntegrityLevel = amsr::ipc::IntegrityLevel;

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_TYPES_H_
