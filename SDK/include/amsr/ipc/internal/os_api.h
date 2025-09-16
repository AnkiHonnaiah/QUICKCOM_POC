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
 *        \brief  Provides interface for common operating system APIs used in Safe IPC.
 *         \unit  osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *         \unit  osabstraction::ipc::IpcOsInterface_QNX
 *   \complexity  IpcOsInterface_QNX is a collection of free wrapper functions around operating system services. The
 *                functions can be tested independently because they perform independent tasks. The SUM complexity
 *                metric is slightly exceeded because of the number of methods, however splitting the unit into multiple
 *                units is not performed because does not provide any benefit for both testing and safety analysis.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/mman.h>
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/ipc/internal/mapped_memory.h"
#include "osabstraction/io/native_types.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief   Memory protection for mapped memory.
 */
enum class MemoryProtection : std::int32_t {
  /*!
   * Memory is read only.
   */
  kReadable = PROT_READ,

  /*!
   * Memory is readable and writable.
   */
  kReadableAndWritable = std::int32_t{std::uint32_t{PROT_READ} | std::uint32_t{PROT_WRITE}}
};

/*!
 * \brief   Length of mapped memory.
 */
struct MapMemoryLength {
  /*!
   * \brief   Length in bytes.
   */
  std::size_t value;
};

/*!
 * \brief           Create shared memory mapping of a file.
 *
 * \details         Always starts the mapping at offset 0.
 *
 * \param[in]       length              Length of the memory mapping. Must be larger than 0.
 * \param[in]       memory_protection   Desired memory protection of the mapping. Must not conflict with the open mode
 *                                      of the file.
 * \param[in]       native_handle       File descriptor of the file that shall be mapped.
 *
 * \return          Mapped memory.
 *
 * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error during mapping of shared memory.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to map shared memory.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError   Cannot map shared memory because the file or the
 *                                                                     file system does not support memory mapping or
 *                                                                     the specified range is invalid for the file or
 *                                                                     the file cannot be accessed.
 * \error           osabstraction::OsabErrc::kBusy                     Cannot map shared memory because the underlying
 *                                                                     file is busy or because the mapping could not be
 *                                                                     locked into memory.
 * \error           osabstraction::OsabErrc::kResource                 Not enough system resources to map shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapMemory(MapMemoryLength length, MemoryProtection memory_protection,
               osabstraction::io::NativeHandle native_handle) noexcept -> ::amsr::core::Result<MappedMemory>;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_H_
