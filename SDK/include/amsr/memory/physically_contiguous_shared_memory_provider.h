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
 *        \brief  Memory provider for physically contiguous shared memory.
 *
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_PHYSICALLY_CONTIGUOUS_SHARED_MEMORY_PROVIDER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_PHYSICALLY_CONTIGUOUS_SHARED_MEMORY_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_interface.h"
#include "amsr/memory/readable_memory_interface.h"
#include "amsr/memory/shared_memory_provider_interface.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Memory provider for physically contiguous shared memory.
 *
 * \details     Allocates anonymous shared memory objects that are not visible in the file system.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-SharedMemoryQNX
 * \trace       DSGN-Osab-SharedMemoryLinux
 */
class PhysicallyContiguousSharedMemoryProvider final : public SharedMemoryProviderInterface {  // VCA_OSA_MOLE_1298
 public:
  /*!
   * \brief       Allocates a new physically contiguous shared memory object and memory maps it.
   *
   * \details     The whole created shared memory object is memory mapped. The memory is mapped as readable and writable
   *              even though a ReadableMemoryInterface pointer is returned.
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              shared memory objects.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Customer did not implement OsAbstraction extension
   *                                                                point to allocate physically contiguous SHM.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured shared memory object size is invalid /
   *                                                                not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map shared
   *                                                                memory object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory object.
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
  auto GetReadableMemory() const noexcept -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> final;

  /*!
   * \brief       Allocates a new physically contiguous shared memory object and memory maps a part of it.
   *
   * \details     The memory is mapped as readable and writable even though a ReadableMemoryInterface pointer is
   *              returned. This call will fail if the passed offset and/or size is invalid for the created memory
   *              object.
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              shared memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the created memory object where the memory mapping shall be started.
   *                      This call will fail if the offset is not a multiple of the page size.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Customer did not implement OsAbstraction extension
   *                                                                point to allocate physically contiguous SHM.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured shared memory object size and/or passed
   *                                                                offset and/or passed size is invalid/not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map shared
   *                                                                memory object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory object.
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
  auto GetReadableMemory(Offset offset, SizeBytes size) const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadableMemoryInterface>> final;

  /*!
   * \brief       Allocates a new physically contiguous shared memory object and memory maps it.
   *
   * \details     The whole created shared memory object is memory mapped. The memory is mapped as readable and
   *              writable. This function can be called multiple times for the same memory provider object to create
   *              multiple shared memory objects.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Customer did not implement OsAbstraction extension
   *                                                                point to allocate physically contiguous SHM.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured shared memory object size is invalid /
   *                                                                not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map shared
   *                                                                memory object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory object.
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
  auto GetReadWritableMemory() const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> final;

  /*!
   * \brief       Allocates a new physically contiguous shared memory object and memory maps a part of it.
   *
   * \details     This call will fail if the passed offset and/or size is invalid for the created memory object.
   *              This function can be called multiple times for the same memory provider object to create multiple
   *              shared memory objects.
   *
   * \param[in]   offset  Offset (in bytes) into the created memory object where the memory mapping shall be started.
   *                      This call will fail if the offset is not a multiple of the page size.
   * \param[in]   size    Size (in bytes) that the memory mapping shall have.
   *
   * \return      Pointer to created memory object.
   *
   * \error       osabstraction::OsabErrc::kApiError                Customer did not implement OsAbstraction extension
   *                                                                point to allocate physically contiguous SHM.
   * \error       osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error       osabstraction::OsabErrc::kSize                    Configured shared memory object size and/or passed
   *                                                                offset and/or passed size is invalid/not supported.
   * \error       osabstraction::OsabErrc::kResource                Not enough system resources to create or map shared
   *                                                                memory object.
   * \error       osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory object.
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
  auto GetReadWritableMemory(Offset offset, SizeBytes size) const noexcept
      -> ::amsr::core::Result<std::unique_ptr<ReadWritableMemoryInterface>> final;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_PHYSICALLY_CONTIGUOUS_SHARED_MEMORY_PROVIDER_H_
