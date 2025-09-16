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
 *        \brief  Defines OS specific constants used in Safe IPC.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_OS_SPECIFIC_CONSTANTS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_OS_SPECIFIC_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace ipc {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief  Additional OS specific flags that shall be passed to an mmap() call used to map shared memory.
 */
constexpr std::int32_t kOsSpecificMmapFlags{0};
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_OS_SPECIFIC_CONSTANTS_H_
