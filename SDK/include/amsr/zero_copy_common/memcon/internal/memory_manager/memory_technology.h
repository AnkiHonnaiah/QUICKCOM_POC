/*!********************************************************************************************************************
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
/*!        \file
 *        \brief Definition of the MemoryTechnology enum.
 *        \unit ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_TECHNOLOGY_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_TECHNOLOGY_H_

#include <cstdint>

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {

/*!
 * \brief    Enumeration to configure the memory technology used by MemCon.
 * \vprivate Component private
 */
enum class MemoryTechnology : std::uint8_t { kSharedMemory = 0, kPhysContigSharedMemory = 1 };

}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_TECHNOLOGY_H_
