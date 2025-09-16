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
 *        \brief  OS interface for memory abstraction on Linux.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/memory/internal/os_interface/mapped_memory.h"
#include "amsr/memory/internal/os_interface/memory_os_api_common.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief           Creates anonymous shared memory.
 *
 * \details         The shared memory is always created with read and write permission (the returned file descriptor was
 *                  opened with read/write mode). The file descriptor is opened with the close-on-exec flag set.
 *                  Will always fail if incompatible glibc version is used.
 *
 * \param[in]       size                Size in bytes that the shared memory segment shall have.
 *
 * \return          File descriptor for the created shared memory object.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kSize                    Requested shared memory size is invalid.
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
 */
auto AllocateAnonymousShm(SizeBytes size) noexcept -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

/*!
 * \brief           Seal a created anonymous shared memory.
 *
 * \details         The created shared memory is sealed which means that the shared memory object layout (e.g. its size)
 *                  cannot be changed anymore.
 *                  Will always fail if incompatible glibc version is used.
 *
 * \param[in]       shm_fd              Valid file descriptor of the anonymous shared memory object that shall be
 *                                      opened. The anonymous shared memory object must allow sealing and must not
 *                                      already be sealed.
 * \param[in]       shared_access_mode  kReadOnly if other processes with whom the SHM object is shared shall not get
 *                                      write access to the SHM object, kReadWrite otherwise. Note that this parameter
 *                                      is ignored if the AMSR_MEMORY_READONLY_SHM compile definition is not set and
 *                                      other processes will always have write access to the SHM object.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto SealAnonymousShm(osabstraction::io::NativeHandle shm_fd, AccessMode shared_access_mode) noexcept
    -> ::amsr::core::Result<void>;

/*!
 * \brief           Opens anonymous shared memory.
 *
 * \details         It is checked that the shared memory object is correctly sealed.
 *                  Will always fail if incompatible glibc version is used.
 *
 * \param[in]       shm_fd              Valid file descriptor of the anonymous shared memory object that shall be
 *                                      opened.
 * \param[in]       access_mode         kReadWrite if the shared memory object shall be opened for reading and writing,
 *                                      kReadOnly if the shared memory object shall be opened for reading only.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kProtocolError           Shared memory object was created invalid.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto OpenAnonymousShm(osabstraction::io::NativeHandle shm_fd, AccessMode access_mode) noexcept
    -> ::amsr::core::Result<void>;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_H_
