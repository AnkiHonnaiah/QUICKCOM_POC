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
 *        \brief  Provides utilities to work with shared memory files.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SHM_UTILITIES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SHM_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/internal/data_types.h"
#include "amsr/ipc/internal/mapped_memory.h"
#include "amsr/ipc/internal/posix_os_api.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief  Result of a CreateShm() call.
 */
struct CreateShmResult {
  /*!
   * \brief  Mapped memory created from the shared memory file.
   */
  MappedMemory shared_memory{};

  /*!
   * \brief  ID of the created shared memory file.
   */
  std::uint64_t shm_id{0};
};

/*!
 * \brief           Generates a unique shared memory ID that can be used when creating a shared memory file.
 * \details         This ID is unique across processes if every process behaves correctly.
 *
 * \return          Generated unique ID.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GenerateUniqueShmId() noexcept -> std::uint64_t;

/*!
 * \brief           Creates a null-terminated file path that refers to a shared memory file with the passed ID.
 *
 * \param[in]       shm_id           ID number that identifies the shared memory file.
 * \param[out]      shm_path_buffer  Buffer where the generated file path can be stored. Has to be large enough for the
 *                                   generated file path (+ null-termination).
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
void ResolveShmPath(std::uint64_t shm_id, ::amsr::core::Span<char> shm_path_buffer) noexcept;

/*!
 * \brief           Creates a new shared memory file and memory maps it.
 * \details         The file is created in the directory configured by PathConfiguration::kIpcConnectionWorkingDir.
 *                  A newly created shared memory file will be mapped into this process with read and write permissions.
 *
 * \param[in]       file_mode                       File mode for the created shared memory file. This specifies the
 *                                                  Unix permissions that the created file will have. This mode is
 *                                                  explicitly set with ChangeFileMode() after the file was created so
 *                                                  that the current umask does not influence on the Unix permissions of
 *                                                  the created file.
 * \param[in]       file_size                       Size that the created shared memory file should have.
 * \param[in]       max_filename_collision_retries  Maximum number of times that this function should try to create the
 *                                                  shared memory file if creation failed because of a file name
 *                                                  collision.
 *
 * \return          Mapped memory created from the shared memory file and ID of the shared memory file.
 *
 * \error           osabstraction::OsabErrc::kUnexpected             Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kProtocolError          Filename collisions occurred and
 *                                                                   max_filename_collision_retries was exceeded
 *                                                                   or the system environment is in an unexpected
 *                                                                   state or a file system I/O error occurred
 *                                                                   or the connection establishment protocol was
 *                                                                   violated in another way, possibly by an outside
 *                                                                   process.
 * \error           osabstraction::OsabErrc::kResource               Not enough system resources to create shared
 *                                                                   memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto CreateShm(FileMode file_mode, FileSize file_size, std::size_t max_filename_collision_retries) noexcept
    -> ::amsr::core::Result<CreateShmResult>;

/*!
 * \brief           Signature of a function that validates a shared memory size.
 *
 * \details         Such a function pointer is passed to OpenShm() which then calls this function to verify that the
 *                  size of the opened shared memory is valid.
 *                  Different types of shared memories have different requirements on the shared memory size and it is
 *                  up to the user of OpenShm() to pass a function that can correctly verify the size for the type of
 *                  shared memory that is opened.
 *
 * \param[in]       shared_memory_size        Size of the shared memory that shall be validated.
 *
 * \return          True if the shared memory size is a valid size, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
using ValidateShmSizeFunction = bool (*)(std::size_t shared_memory_size);

/*!
 * \brief           Opens an existing shared memory file and memory maps it.
 *
 * \param[in]       shm_id                          ID of the shared memory file that should be opened. The file path
 *                                                  of the shared memory file is generated from this ID.
 * \param[in]       open_and_protection_mode        Access mode with which the file shall be opened with. The file is
 *                                                  then memory mapped with the corresponding memory protection mode.
 * \param[in]       shared_memory_size_validator    Pointer to a function that is called by OpenShm() to verify that the
 *                                                  size of the opened file is a valid size
 *                                                  (see ValidateShmSizeFunction).
 *
 * \return          Mapped memory created from the shared memory file.
 *
 * \error           osabstraction::OsabErrc::kUnexpected             Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kDisconnected           Shared memory file does not exist anymore.
 * \error           osabstraction::OsabErrc::kProtocolError          The system environment is in an unexpected
 *                                                                   state or a file system I/O error occurred
 *                                                                   or the connection establishment protocol was
 *                                                                   violated in another way, possibly by an outside
 *                                                                   process.
 * \error           osabstraction::OsabErrc::kResource               Not enough system resources to open shared
 *                                                                   memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto OpenShm(std::uint64_t shm_id, OpenAccessMode open_and_protection_mode,
             ValidateShmSizeFunction shared_memory_size_validator) noexcept -> ::amsr::core::Result<MappedMemory>;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SHM_UTILITIES_H_
