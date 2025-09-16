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
 *        \brief  Type definitions for memory exchange handles.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_EXCHANGE_HANDLE_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_EXCHANGE_HANDLE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/variant.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace memory {
namespace internal {

/*!
 * \brief   Linux specific shared memory exchange handle type.
 */
struct NativeSharedMemoryExchangeHandle {
  /*!
   * \brief   Shared pointer to the SHM object file descriptor.
   * \details Never nullptr.
   */
  std::shared_ptr<osabstraction::io::FileDescriptor> shm_fd;

  /*!
   * \brief   Shared access mode of the SHM object.
   * \details The exchange handle should be opened with this mode.
   */
  AccessMode shared_access_mode{AccessMode::kReadOnly};
};

/*!
 * \brief   Union of all implemented exchange handle types on Linux.
 */
using NativeExchangeHandle = ::amsr::core::Variant<NativeSharedMemoryExchangeHandle>;

}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_EXCHANGE_HANDLE_TYPES_H_
