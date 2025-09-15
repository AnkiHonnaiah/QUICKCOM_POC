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
 *        \brief  Provides interface for POSIX/UNIX operating system APIs used in memory.
 *         \unit  osabstraction::memory::SharedMemory_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief Get the size of a shared memory object
 *
 * \param[in] native_handle  Valid file descriptor of the SHM object whose size shall be queried.
 *
 * \return Size of the file in bytes.
 *
 * \error osabstraction::OsabErrc::kUnexpected              Unexpected error while determining the SHM object size.
 * \error osabstraction::OsabErrc::kResource                Not enough memory to get object size.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError  SHM object size is too big for this process or the  system
 *                                                          does not support the fstat call or a file system I/O error
 *                                                          occurred.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto GetFileSize(osabstraction::io::NativeHandle native_handle) noexcept -> ::amsr::core::Result<SizeBytes>;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_H_
