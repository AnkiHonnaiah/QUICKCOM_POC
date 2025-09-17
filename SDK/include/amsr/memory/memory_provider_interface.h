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
 *        \brief  Defines interface for memory providers.
 *         \unit  osabstraction::memory::SharedMemory_Linux
 *         \unit  osabstraction::memory::SharedMemory_QNX
 *
 *   \complexity  Only minor violation, no action required.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_PROVIDER_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_PROVIDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_interface.h"
#include "amsr/memory/readable_memory_interface.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Interface for memory providers.
 *
 * \details     Memory providers allow creation and mapping of different kinds of memory.
 *
 * \vprivate    Vector product internal API
 */
class MemoryProviderInterface {
 public:
  /*!
   * \brief       Constructs a MemoryProviderInterface.
   *
   * \spec
   *   requires true;
   * \endspec
   * \steady      TRUE
   *
   * \vprivate    Vector component internal API
   */
  MemoryProviderInterface() noexcept = default;

  /*!
   * \brief       Virtual destructor for interface.
   *
   * \spec
   *   requires true;
   * \endspec
   * \steady      TRUE
   */
  virtual ~MemoryProviderInterface() noexcept = default;

  /*!
   * \brief       Creates memory object and a memory mapping that is readable from the object.
   *
   * \details     The whole created memory object is memory mapped.
   *              Depending on the type of memory:
   *
   *              - The underlying physical memory may already be allocated or will be allocated by this call.
   *              - The memory mapping may also be writable.
   *
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              memory objects.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Requested type of memory is not supported by
   *                                                                OsAbstraction on this OS / in this OsAbstraction
   *                                                                configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured memory object size is invalid /
   *                                                                not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map memory
   *                                                                object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create memory object.
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
  virtual auto GetReadableMemory() const noexcept -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> = 0;

  /*!
   * \brief       Creates memory object and a memory mapping that is readable from the object.
   *
   * \details     Only the part of the memory object, specified by the passed offset and size, is memory mapped. This
   *              call will fail if the passed offset and/or size is invalid for the created memory object.
   *              Depending on the type of memory:
   *
   *              - The underlying physical memory may already be allocated or will be allocated by this call.
   *              - The memory mapping may also be writable.
   *
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the created memory object where the memory mapping shall be started.
   *                      Specific implementations of the ReadableMemoryInterface may have requirements on the value of
   *                      offset.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Requested type of memory is not supported by
   *                                                                OsAbstraction on this OS / in this OsAbstraction
   *                                                                configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured memory object size and/or passed offset
   *                                                                and/or passed size is invalid / not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map memory
   *                                                                object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create memory object.
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
  virtual auto GetReadableMemory(Offset offset, SizeBytes size) const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> = 0;

  /*!
   * \brief       Creates memory object and a memory mapping that is readable and writable from the object.
   *
   * \details     The whole created memory object is memory mapped.
   *              Depending on the type of memory:
   *
   *              - The underlying physical memory may already be allocated or will be allocated by this call.
   *
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              memory objects.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Requested type of memory is not supported by
   *                                                                OsAbstraction on this OS / in this OsAbstraction
   *                                                                configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured memory object size is invalid /
   *                                                                not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map memory
   *                                                                object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create memory object.
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
  virtual auto GetReadWritableMemory() const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> = 0;

  /*!
   * \brief       Creates memory object and a memory mapping that is readable and writable from the object.
   *
   * \details     Only the part of the memory object, specified by the passed offset and size, is memory mapped. This
   *              call will fail if the passed offset and/or size is invalid for the created memory object.
   *              Depending on the type of memory:
   *
   *              - The underlying physical memory may already be allocated or will be allocated by this call.
   *
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the created memory object where the memory mapping shall be started.
   *                      Specific implementations of the ReadableMemoryInterface may have requirements on the value of
   *                      offset.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Requested type of memory is not supported by
   *                                                                OsAbstraction on this OS / in this OsAbstraction
   *                                                                configuration.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured memory object size and/or passed offset
   *                                                                and/or passed size is invalid / not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map memory
   *                                                                object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create memory object.
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
  virtual auto GetReadWritableMemory(Offset offset, SizeBytes size) const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> = 0;

  /*!
   * \brief       Sets the size (in bytes) that the memory object shall have.
   *
   * \details     This is not necessarily equal to the size of the created memory mapping.
   *              The Get*Memory() functions will fail if this function was not called for the object.
   *              Depending on the type of memory this may determine the amount of memory that will be allocated for
   *              created memory objects.
   *
   * \param[in]   size    Size (in bytes) that memory objects created from this memory provider shall have.
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
   */
  void SetSize(SizeBytes size) noexcept { size_.emplace(size); }

  /*!
   * \brief       Get the configured memory object size.
   *
   * \return      Configured memory object size or an empty optional if no size was configured.
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
   */
  auto GetSize() const noexcept -> ::amsr::core::Optional<SizeBytes> { return size_; }

 protected:
  /*!
   * \brief       Copy constructs a MemoryProviderInterface.
   */
  MemoryProviderInterface(MemoryProviderInterface const& other) noexcept = default;

  /*!
   * \brief       Move constructs a MemoryProviderInterface.
   */
  MemoryProviderInterface(MemoryProviderInterface&& other) noexcept = default;

  /*!
   * \brief       Copy assigns a MemoryProviderInterface.
   */
  MemoryProviderInterface& operator=(MemoryProviderInterface const& other) & noexcept = default;

  /*!
   * \brief       Move assigns a MemoryProviderInterface.
   */
  MemoryProviderInterface& operator=(MemoryProviderInterface&& other) & noexcept = default;

 private:
  /*!
   * \brief       Configured memory object size. Optional is empty if no size was configured.
   */
  ::amsr::core::Optional<SizeBytes> size_{};
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_PROVIDER_INTERFACE_H_
