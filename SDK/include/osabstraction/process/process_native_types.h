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
/*!        \file
 *        \brief  Defines operating system specific process types.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_NATIVE_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_NATIVE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sched.h>
#include <sys/types.h>
#include <cstdint>

namespace osabstraction {
namespace process {

/*!
 * \brief User identifier.
 */
using UserId = uid_t;  // VECTOR Same Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling

/*!
 * \brief   Operating system process identifier.
 * \details Some operating systems may reuse process IDs in a way it cannot be guaranteed that all created OsProcess
 *          objects have unique process IDs.
 */
using ProcessId = pid_t;  // VECTOR Same Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling

/*!
 * \brief Group identifier.
 */
using GroupId = gid_t;

/*!
 * \brief Type to store exit code of a process.
 */
using ExitCodeType = int;  // VECTOR Same Line AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_NATIVE_TYPES_H_
