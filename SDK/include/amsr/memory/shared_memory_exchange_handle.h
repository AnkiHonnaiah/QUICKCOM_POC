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
 *        \brief  Memory exchange handle for shared memory.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_EXCHANGE_HANDLE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_EXCHANGE_HANDLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/memory/internal/exchange_handle_types.h"
#include "amsr/memory/memory_exchange_handle_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_interface.h"
#include "amsr/memory/readable_memory_interface.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Memory exchange handle for shared memory.
 *
 * \details     Shared memory exchange handles can be used to share access to shared memory with other processes. A
 *              memory exchange handle may be a weak or strong reference to the underlying memory it references. This
 *              means that the underlying memory may or may not be already freed if no memory object refers to it
 *              anymore.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-SharedMemoryExchangeHandleQNX
 * \trace       DSGN-Osab-SharedMemoryExchangeHandleLinux
 */
class SharedMemoryExchangeHandle final : public MemoryExchangeHandleInterface {
 public:
  /*!
   * \brief       Constructs a SharedMemoryExchangeHandle.
   *
   * \param[in]   native_shm_handle  OS specific shared memory exchange handle for this exchange handle.
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
  explicit SharedMemoryExchangeHandle(internal::NativeSharedMemoryExchangeHandle&& native_shm_handle) noexcept
      : MemoryExchangeHandleInterface{}, native_exchange_handle_{std::move(native_shm_handle)} {}

  /*!
   * \brief       Exchange handles cannot be copied.
   */
  SharedMemoryExchangeHandle(SharedMemoryExchangeHandle const& other) noexcept = delete;

  /*!
   * \brief       Move constructs a SharedMemoryExchangeHandle.
   * \param[in]   other   SharedMemoryExchangeHandle to move construct from.
   *
   * \steady      TRUE
   */
  SharedMemoryExchangeHandle(SharedMemoryExchangeHandle&& other) noexcept = default;

  /*!
   * \brief       Exchange handles cannot be copied.
   */
  SharedMemoryExchangeHandle& operator=(SharedMemoryExchangeHandle const& other) & noexcept = delete;

  /*!
   * \brief       Move assigns a SharedMemoryExchangeHandle.
   * \param[in]   other   SharedMemoryExchangeHandle to move assign.
   * \return      Reference to this object.
   *
   * \steady      TRUE
   */
  SharedMemoryExchangeHandle& operator=(SharedMemoryExchangeHandle&& other) & noexcept = default;

  /*!
   * \brief       Destroys the SharedMemoryExchangeHandle.
   *
   * \details     The strong or weak reference of this exchange handle to the underlying shared memory is released. The
   *              underlying shared memory is freed if it was previously allocated and all strong references to it are
   *              released.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~SharedMemoryExchangeHandle() noexcept final = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief       Creates a memory mapping that is readable from the shared memory exchange handle.
   *
   * \details     The memory mapping will also be mapped as writable if this exchange handle is a read/write handle.
   *              This call will fail if the passed offset and/or size is invalid for the memory object that this
   *              exchange handle refers to. This call may fail if the memory object this exchange handle was created
   *              from was already destroyed. This function can be called multiple times for the same memory exchange
   *              handle object to create multiple shared memory memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the memory object that this exchange handle refers to where the memory
   *                      mapping shall be started. This call will fail if the offset is not a multiple of the page
   *                      size.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Cannot map shared memory because offset and/or
   *                                                                size of the requested mapping is invalid for the
   *                                                                shared memory object that this exchange handle
   *                                                                refers to.
   * \error       osabstraction::OsabErrc::kInvalidHandle           This exchange handle is invalid because the memory
   *                                                                object that it refers to does not exist (anymore)
   *                                                                or because a previous call to Get*Memory() failed.
   * \error       osabstraction::OsabErrc::kProtocolError           This exchange handle or the shared memory object it
   *                                                                refers to was created invalid.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to open shared memory
   *                                                                exchange handle.
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
  auto GetReadableMemory(Offset offset, SizeBytes size) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> final;

  /*!
   * \brief       Creates a memory mapping that is readable and writable from the shared memory exchange handle.
   *
   * \details     This call will fail if this exchange handle does not allow writable memory mappings or if the
   *              passed offset and/or size is invalid for the memory object that this exchange handle refers to. This
   *              call may fail if the memory object this exchange handle was created from was already destroyed. This
   *              function can be called multiple times for the same memory exchange handle object to create multiple
   *              shared memory memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the memory object that this exchange handle refers to where the memory
   *                      mapping shall be started. This call will fail if the offset is not a multiple of the page
   *                      size.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Cannot map shared memory because offset and/or
   *                                                                size of the requested mapping is invalid for the
   *                                                                shared memory object that this exchange handle
   *                                                                refers to.
   * \error       osabstraction::OsabErrc::kInvalidHandle           This exchange handle is invalid because the memory
   *                                                                object that it refers to does not exist (anymore)
   *                                                                or because a previous call to Get*Memory() failed.
   * \error       osabstraction::OsabErrc::kProtocolError           This exchange handle or the shared memory object it
   *                                                                refers to was created invalid or this exchange
   *                                                                handle does not allow write access to the shared
   *                                                                memory object.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to open shared memory
   *                                                                exchange handle.
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
  auto GetReadWritableMemory(Offset offset, SizeBytes size) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> final;

  /*!
   * \brief   Returns the size of the shared memory object that this exchange handle refers to.
   * \details This call may fail if the memory object this exchange handle was created from was already destroyed.
   *
   * \return Total size of shared memory in bytes.
   *
   * \error osabstraction::OsabErrc::kUnexpected     Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInvalidHandle  This exchange handle is invalid because the memory object that it
   *                                                 refers to does not exist (anymore) or because a previous call to
   *                                                 Get*Memory() or GetMemorySize() failed.
   * \error osabstraction::OsabErrc::kProtocolError  This exchange handle or the memory object it refers to was created
   *                                                 invalid.
   * \error osabstraction::OsabErrc::kResource       Not enough system resources to open shared memory exchange handle.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  auto GetMemorySize() noexcept -> ::amsr::core::Result<SizeBytes> final;

  /*!
   * \brief       Gets access to the internal OS specific exchange handle.
   *
   * \return      Reference to the internal OS specific exchange handle.
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
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate    Vector component internal API
   */
  auto AccessNativeExchangeHandle() noexcept -> internal::NativeExchangeHandle& final {
    return native_exchange_handle_;
  }

 private:
  /*!
   * \brief       OS specific exchange handle.
   */
  internal::NativeExchangeHandle native_exchange_handle_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_EXCHANGE_HANDLE_H_
