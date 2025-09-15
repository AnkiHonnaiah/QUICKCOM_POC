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
 *        \brief  Shared memory object that is mapped as readable.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_SHARED_READABLE_MEMORY_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_SHARED_READABLE_MEMORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/memory/internal/os_interface/mapped_memory.h"
#include "amsr/memory/memory_exchange_handle_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/readable_memory_interface.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Shared memory object that is mapped as readable.
 *
 * \details     Manages the lifetime of the memory mapping and the underlying shared memory.
 *
 * \vprivate    Vector product internal API
 */
class SharedReadableMemory final : public ReadableMemoryInterface {
 public:
  /*!
   * \brief       Creates a shared memory object that is mapped as readable.
   *
   * \details     The shared memory object holds ownership of the memory mapping and holds a strong reference to the
   *              underlying physical memory.
   *
   * \param[in]   shm                  Mapped shared memory.
   * \param[in]   shm_fd               File descriptor of the shared memory object. Must be not be nullptr and a valid
   *                                   file descriptor.
   * \param[in]   shared_access_mode   kReadOnly if memory exchange handles created from this object should be read-only
   *                                   handles that cannot be opened with write access, kReadWrite otherwise.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector component internal API
   */
  SharedReadableMemory(internal::os_interface::MappedMemory&& shm,
                       std::shared_ptr<osabstraction::io::FileDescriptor> shm_fd,
                       AccessMode shared_access_mode) noexcept;

  /*!
   * \brief       SharedReadableMemory cannot be copied.
   */
  SharedReadableMemory(SharedReadableMemory const& other) noexcept = delete;

  /*!
   * \brief       Move constructs a SharedReadableMemory.
   * \param[in]   other   SharedReadableMemory to move construct from.
   * \steady      TRUE
   */
  SharedReadableMemory(SharedReadableMemory&& other) noexcept = default;

  /*!
   * \brief       SharedReadableMemory cannot be copied.
   */
  SharedReadableMemory& operator=(SharedReadableMemory const& other) & noexcept = delete;

  /*!
   * \brief       Move assigns a SharedReadableMemory.
   * \param[in]   other   SharedReadableMemory to move assign.
   * \return      Reference to this object.
   * \steady      FALSE
   */
  SharedReadableMemory& operator=(SharedReadableMemory&& other) & noexcept = default;

  /*!
   * \brief       Destroys the shared memory object.
   *
   * \details     This leads to:
   *
   *              - Unmapping of the shared memory. No more access must be done to the previously mapped addresses.
   *              - The strong reference of this shared memory object to the underlying physical memory is released. The
   *                underlying physical memory is freed if all strong references to it are released.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   */
  ~SharedReadableMemory() noexcept final = default;

  /*!
   * \brief       Create a shared memory exchange handle for this shared memory object.
   *
   * \details     The created exchange handle can be used to share access to the underlying physical shared memory of
   *              this object with other processes by sending the created exchange handle via a SafeIPC connection to
   *              the other process. Multiple exchange handles can be created from the same memory object.
   *
   * \return      Pointer to the created memory exchange handle.
   *
   * \error       -
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   * \trace       DSGN-Osab-SharedMemoryExchangeHandleLinux
   */
  auto CreateExchangeHandle() noexcept -> ::amsr::core::Result<std::unique_ptr<MemoryExchangeHandleInterface>> final;

  /*!
   * \brief       Get underlying operating system handle (SHM object file descriptor) for this memory object.
   *
   * \details     Be aware that the meaning of the returned integer value may vary between OS and/or memory object
   *              types. For some operating systems and/or memory object types, there may not even be a notion of an
   *              integer memory object handle. As such, code using this function may not be very portable.
   *              The user of OsAbstraction shall only perform operations in the returned handle which do not change the
   *              state of the underlying operating system memory object.
   *
   *              GetMemoryHandle() returns a SHM object file descriptor if called on a SharedReadableMemory object.
   *
   * \return      Operating system SHM object file descriptor (never amsr::memory::kInvalidMemoryHandle).
   *              The returned handle is only valid as long as this object still exists.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   * \trace       DSGN-Osab-GetInternalMemoryHandle
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetMemoryHandle() const noexcept -> MemoryHandle final { return shm_fd_->Get(); };

 private:
  /*!
   * \brief   Mapped shared memory.
   */
  internal::os_interface::MappedMemory shm_;

  /*!
   * \brief   File descriptor of the shared memory object, never nullptr.
   */
  std::shared_ptr<osabstraction::io::FileDescriptor> shm_fd_;

  /*!
   * \brief   kReadOnly if exchange handles created from this object should be read-only handles that cannot be opened
   *          with write access, kReadWrite otherwise.
   */
  AccessMode shared_access_mode_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_SHARED_READABLE_MEMORY_H_
