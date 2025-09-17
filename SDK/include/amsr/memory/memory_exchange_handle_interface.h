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
 *        \brief  Defines interface for memory exchange handles.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_EXCHANGE_HANDLE_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_EXCHANGE_HANDLE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/memory/internal/exchange_handle_types.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {

/*!
 * \brief   Forward declaration of ReadableMemoryInterface.
 */
class ReadableMemoryInterface;

/*!
 * \brief   Forward declaration of ReadWritableMemoryInterface.
 */
class ReadWritableMemoryInterface;

/*!
 * \brief       Interface for memory exchange handles.
 *
 * \details     Memory exchange handles can be used to share access to memory with other processes. A memory exchange
 *              handle may be a weak or strong reference to the underlying memory it references. This means that the
 *              underlying memory may or may not be already freed if no memory object refers to it anymore.
 *
 * \vprivate    Vector product internal API
 */
class MemoryExchangeHandleInterface {
 public:
  /*!
   * \brief       Constructs a MemoryExchangeHandleInterface.
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \steady      TRUE
   *
   * \vprivate    Vector component internal API
   */
  MemoryExchangeHandleInterface() noexcept = default;

  /*!
   * \brief       Destroys the MemoryExchangeHandleInterface.
   *
   * \details     The strong or weak reference of this exchange handle to the underlying memory is released. The
   *              underlying memory is freed if it was previously allocated and all strong references to it are
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
  virtual ~MemoryExchangeHandleInterface() noexcept = default;

  /*!
   * \brief       Creates a memory mapping that is readable from the exchange handle.
   *
   * \details     Depending on the exchange handle, the memory mapping may also be writable. This call will fail if the
   *              passed offset and/or size is invalid for the memory object that this exchange handle refers to. This
   *              call may fail if the memory object this exchange handle was created from was already destroyed.
   *              This function can be called multiple times for the same memory exchange handle object to create
   *              multiple memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the memory object that this exchange handle refers to where the memory
   *                      mapping shall be started. Specific implementations of the MemoryExchangeHandleInterface may
   *                      have requirements on the value of offset.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                (Currently unused) Type of memory exchange handle is
   *                                                                not supported by OsAbstraction on this OS / in this
   *                                                                OsAbstraction configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Cannot map memory because offset and/or size of the
   *                                                                requested mapping is invalid for the memory object
   *                                                                that this exchange handle refers to.
   * \error       osabstraction::OsabErrc::kInvalidHandle           This exchange handle is invalid because the memory
   *                                                                object that it refers to does not exist (anymore)
   *                                                                or because a previous call to Get*Memory() or
   *                                                                GetMemorySize() failed.
   * \error       osabstraction::OsabErrc::kProtocolError           This exchange handle or the memory object it refers
   *                                                                to was created invalid.
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
  virtual auto GetReadableMemory(Offset offset, SizeBytes size) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> = 0;

  /*!
   * \brief       Creates a memory mapping that is readable and writable from the exchange handle.
   *
   * \details     This call will fail if this exchange handle does not allow writable memory mappings or if the
   *              passed offset and/or size is invalid for the memory object that this exchange handle refers to. This
   *              call may fail if the memory object this exchange handle was created from was already destroyed.
   *              This function can be called multiple times for the same memory exchange handle object to create
   *              multiple memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the memory object that this exchange handle refers to where the memory
   *                      mapping shall be started. Specific implementations of the MemoryExchangeHandleInterface may
   *                      have requirements on the value of offset.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                (Currently unused) Type of memory exchange handle is
   *                                                                not supported by OsAbstraction on this OS / in this
   *                                                                OsAbstraction configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Cannot map memory because offset and/or size of the
   *                                                                requested mapping is invalid for the memory object
   *                                                                that this exchange handle refers to.
   * \error       osabstraction::OsabErrc::kInvalidHandle           This exchange handle is invalid because the memory
   *                                                                object that it refers to does not exist (anymore)
   *                                                                or because a previous call to Get*Memory() or
   *                                                                GetMemorySize() failed.
   * \error       osabstraction::OsabErrc::kProtocolError           This exchange handle or the memory object it refers
   *                                                                to was created invalid or this exchange handle does
   *                                                                not allow write access to the memory object.
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
  virtual auto GetReadWritableMemory(Offset offset, SizeBytes size) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> = 0;

  /*!
   * \brief   Returns the size of the memory object that this exchange handle refers to.
   * \details This call may fail if the memory object this exchange handle was created from was already destroyed.
   *
   * \return Total size of memory in bytes.
   *
   * \error osabstraction::OsabErrc::kApiError       (Currently unused) Type of memory exchange handle is not supported
   *                                                 by OsAbstraction on this OS / in this OsAbstraction configuration.
   * \error osabstraction::OsabErrc::kUnexpected     Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInvalidHandle  This exchange handle is invalid because the memory object that it
   *                                                 refers to does not exist (anymore) or because a previous call to
   *                                                 Get*Memory() or GetMemorySize() failed.
   * \error osabstraction::OsabErrc::kProtocolError  This exchange handle or the memory object it refers to was created
   *                                                 invalid.
   * \error osabstraction::OsabErrc::kResource       Not enough system resources to open memory exchange handle.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \steady      TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual auto GetMemorySize() noexcept -> ::amsr::core::Result<SizeBytes> = 0;

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
  virtual auto AccessNativeExchangeHandle() noexcept -> internal::NativeExchangeHandle& = 0;

 protected:
  /*!
   * \brief       Copy constructs a MemoryExchangeHandleInterface.
   */
  MemoryExchangeHandleInterface(MemoryExchangeHandleInterface const& other) noexcept = default;

  /*!
   * \brief       Move constructs a MemoryExchangeHandleInterface.
   */
  MemoryExchangeHandleInterface(MemoryExchangeHandleInterface&& other) noexcept = default;

  /*!
   * \brief       Copy assigns a MemoryExchangeHandleInterface.
   */
  MemoryExchangeHandleInterface& operator=(MemoryExchangeHandleInterface const& other) & noexcept = default;

  /*!
   * \brief       Move assigns a MemoryExchangeHandleInterface.
   */
  MemoryExchangeHandleInterface& operator=(MemoryExchangeHandleInterface&& other) & noexcept = default;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_EXCHANGE_HANDLE_INTERFACE_H_
