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
 *        \brief  OS interface for memory abstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_COMMON_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_COMMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/memory/internal/os_interface/mapped_memory.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief           Map a shared memory object.
 *
 * \param[in]       offset              Offset into the shared memory object where the mapping shall start. The call
 *                                      fails if this is not a multiple of the page size.
 * \param[in]       size                Size of the memory mapping in bytes.
 * \param[in]       access_mode         Desired memory protection of the mapping. The call fails if this conflicts with
 *                                      the open mode of the file.
 * \param[in]       shm_fd              Valid file descriptor of the file that shall be mapped.
 *
 * \return          Mapped memory.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error during mapping of shared memory.
 * \error           osabstraction::OsabErrc::kSize                    Cannot map shared memory because offset and/or
 *                                                                    size of the requested mapping is invalid for the
 *                                                                    shared memory object.
 * \error           osabstraction::OsabErrc::kProtocolError           (Linux only) Not allowed to map shared memory with
 *                                                                    the specified memory protection or file descriptor
 *                                                                    does not refer to a shared memory object.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to map shared memory.
 * \error           osabstraction::OsabErrc::kBusy                    (QNX only) Cannot map shared memory because the
 *                                                                    underlying resource is busy.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto MapSharedMemory(Offset offset, SizeBytes size, AccessMode access_mode,
                     osabstraction::io::NativeHandle shm_fd) noexcept -> ::amsr::core::Result<MappedMemory>;

/*!
 * \brief           Unmaps a previously created memory mapping.
 *
 * \details         Aborts if unmapping fails.
 *
 * \param[in]       memory_to_unmap     View to the memory that shall be unmapped. Must be a valid range.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
void UnmapMemory(::amsr::core::Span<std::uint8_t> memory_to_unmap) noexcept;

/*!
 * \brief           Creates physically contiguous anonymous shared memory.
 *
 * \details         The shared memory is always created with read and write permission (the returned file descriptor was
 *                  opened with read/write mode). The created shared memory is sealed which means that the shared memory
 *                  object layout cannot be changed anymore. The file descriptor is opened with the
 *                  close-on-exec flag set.
 *
 * \param[in]       size                Size in bytes that the shared memory segment shall at least have.
 *
 * \return          File descriptor for the created shared memory object.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kApiError                Extension point to allocate physically contiguous
 *                                                                    shared memory is not implemented.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create physically contiguous
 *                                                                    anonymous shared memory object.
 * \error           osabstraction::OsabErrc::kSize                    Requested shared memory size is invalid or not
 *                                                                    supported.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create anonymous
 *                                                                    shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto AllocatePhysContigAnonymousShm(SizeBytes size) noexcept -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_COMMON_H_
