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
 *        \brief  Provides functions to create, open and map different kinds of memory on Linux.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/memory/internal/exchange_handle_types.h"
#include "amsr/memory/internal/os_interface/mapped_memory.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief   Properties that an allocated shared memory should have.
 */
enum class ShmProperties : std::uint32_t {
  /*!
   * \brief   Allocated shared memory segment does not need to be physically contiguous.
   */
  kPhysicallyDiscontiguousShm,
  /*!
   * \brief   Allocated shared memory segment must be physically contiguous.
   */
  kPhysicallyContiguousShm
};

/*!
 * \brief   Result of a AllocateShm() or OpenShm() call.
 */
struct ShmResult {
  /*!
   * \brief   Mapped memory.
   */
  MappedMemory shm;

  /*!
   * \brief   Shared pointer to SHM object file descriptor.
   * \details Never nullptr.
   */
  std::shared_ptr<osabstraction::io::FileDescriptor> shm_fd;
};

/*!
 * \brief           Allocates a new shared memory object and maps (part of it).
 *
 * \param[in]       shm_size              Size in bytes that the shared memory segment shall have.
 * \param[in]       map_offset            Offset into the shared memory object where the mapping shall start. The call
 *                                        fails if this is not a multiple of the page size.
 * \param[in]       map_size              Size of the memory mapping in bytes.
 * \param[in]       shm_properties        Properties that the shared memory object shall have.
 * \param[in]       shared_access_mode    kReadOnly if other processes with whom the SHM object is shared shall not get
 *                                        write access to the SHM object, kReadWrite otherwise. Note that this parameter
 *                                        is ignored if the AMSR_MEMORY_READONLY_SHM compile definition is not set and
 *                                        other processes will always have write access to the SHM object.
 *
 * \return          Mapped memory and SHM object file descriptor.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kApiError                Requested physically contiguous shared memory but
 *                                                                    extension point to allocate it is not implemented.
 * \error           osabstraction::OsabErrc::kSize                    Cannot create or map shared memory because
 *                                                                    shm_size and/or map_offset and/or map_size is
 *                                                                    invalid / not supported.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create or map
 *                                                                    shared memory.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create anonymous shared memory
 *                                                                    object.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto AllocateShm(SizeBytes shm_size, Offset map_offset, SizeBytes map_size, ShmProperties shm_properties,
                 AccessMode shared_access_mode) noexcept -> ::amsr::core::Result<ShmResult>;

/*!
 * \brief           Opens a shared memory exchange handle and maps (part of it).
 *
 * \details         The type of mapping (read-only / read-write) is determined by the shared access mode of the passed
 *                  exchange handle.
 *
 * \param[in]       exchange_handle       Native shared memory exchange handle to open.
 * \param[in]       map_offset            Offset into the shared memory object where the mapping shall start. The call
 *                                        fails if this is not a multiple of the page size.
 * \param[in]       map_size              Size of the memory mapping in bytes.
 *
 * \return          Mapped memory and SHM object file descriptor.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kSize                    Cannot map shared memory because offset and/or
 *                                                                    size of the requested mapping is invalid for the
 *                                                                    shared memory object.
 * \error           osabstraction::OsabErrc::kProtocolError           Shared access mode of exchange handle was
 *                                                                    read/write but memory is not allowed to be mapped
 *                                                                    as writable or shared memory object was created
 *                                                                    invalid.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to map shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto OpenShm(NativeSharedMemoryExchangeHandle const& exchange_handle, Offset map_offset, SizeBytes map_size) noexcept
    -> ::amsr::core::Result<ShmResult>;

/*!
 * \brief           Queries the shared memory's size.
 *
 * \details         Converts the exchange handle into a file descriptor if it has not been converted, yet.
 *
 * \param[in,out]   exchange_handle       Native shared memory exchange handle to open.
 *
 * \return          Size of the shared memory object.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kInvalidHandle           Failed to open shared memory handle because the
 *                                                                    handle is not valid (anymore). The handle may also
 *                                                                    become invalid due to a previously failed
 *                                                                    OpenShm() call for the handle.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to open shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE for different exchange handles
 */
auto GetShmSize(NativeSharedMemoryExchangeHandle const& exchange_handle) noexcept -> ::amsr::core::Result<SizeBytes>;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_API_H_
