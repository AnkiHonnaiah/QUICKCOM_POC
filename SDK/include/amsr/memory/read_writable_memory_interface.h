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
 *        \brief  Defines interface for memory objects that are mapped as readable and writable.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/span.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_interface.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Interface for memory objects that are mapped as readable and writable.
 *
 * \details     Memory objects manage the lifetime of the memory mapping and the underlying memory.
 *
 * \vprivate    Vector product internal API
 */
class ReadWritableMemoryInterface : public ReadableMemoryInterface {
 public:
  /*!
   * \brief       Inherited constructor of ReadableMemoryInterface
   */
  using ReadableMemoryInterface::ReadableMemoryInterface;

  /*!
   * \brief       Destroys the memory object.
   *
   * \details     This leads to:
   *
   *              - Unmapping of the memory. No more access must be done to the previously mapped addresses.
   *              - The strong reference of this memory object to the underlying memory is released. The underlying
   *                memory is freed if it was previously allocated and all strong references to it are released.
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
  ~ReadWritableMemoryInterface() noexcept override = default;

  /*!
   * \brief       Get readable and writable view to the memory mapping of this object.
   *
   * \return      Readable and writable view to the memory mapping of this object.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   * \trace       DSGN-Osab-MemoryView
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Return view to mapped memory.
   * \endinternal
   */
  auto GetReadWritableMemoryView() const noexcept -> ReadWritableMemoryView {
    return ReadWritableMemoryView{this->GetMemoryView()};
  }

 protected:
  /*!
   * \brief       Copy constructs a ReadWritableMemoryInterface.
   */
  ReadWritableMemoryInterface(ReadWritableMemoryInterface const& other) noexcept = default;

  /*!
   * \brief       Move constructs a ReadWritableMemoryInterface.
   */
  ReadWritableMemoryInterface(ReadWritableMemoryInterface&& other) noexcept = default;

  /*!
   * \brief       Copy assigns a ReadWritableMemoryInterface.
   */
  ReadWritableMemoryInterface& operator=(ReadWritableMemoryInterface const& other) & noexcept = default;

  /*!
   * \brief       Move assigns a ReadWritableMemoryInterface.
   */
  ReadWritableMemoryInterface& operator=(ReadWritableMemoryInterface&& other) & noexcept = default;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_INTERFACE_H_
