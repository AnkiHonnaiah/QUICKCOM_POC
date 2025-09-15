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
 *        \brief  Defines interface for memory objects that are mapped as readable.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_MEMORY_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_MEMORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/memory/memory_exchange_handle_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/readable_memory_view.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Interface for memory objects that are mapped as readable.
 *
 * \details     Memory objects manage the lifetime of the memory mapping and the underlying memory.
 *
 * \vprivate    Vector product internal API
 */
class ReadableMemoryInterface {
 public:
  /*!
   * \brief       Creates an interface for a memory object that is mapped as readable.
   *
   * \details     The memory object holds ownership of the memory mapping and holds a strong reference to the underlying
   *              memory.
   *
   * \param[in]   view_to_memory  View to the mapped memory.
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
  explicit ReadableMemoryInterface(::amsr::core::Span<std::uint8_t> view_to_memory) noexcept : view_{view_to_memory} {}

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
  virtual ~ReadableMemoryInterface() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_OS_specific_implementation_constness_or_staticness
  /*!
   * \brief       Create a memory exchange handle for this memory object.
   *
   * \details     The created memory exchange handle can be used to share access to the underlying memory of this memory
   *              object with other processes by sending the created exchange handle via a SafeIPC connection to the
   *              other process. Multiple memory exchange handles can be created from the same memory object.
   *
   * \return      Pointer to the created memory exchange handle.
   *
   * \error       osabstraction::OsabErrc::kApiError                (PikeOS only) amsr::memory is not
   *                                                                implemented for this OS.
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
  virtual auto CreateExchangeHandle() noexcept
      -> ::amsr::core::Result<std::unique_ptr<MemoryExchangeHandleInterface>> = 0;

  /*!
   * \brief       Get readable view to the memory mapping of this object.
   *
   * \return      Readable view to the memory mapping of this object.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   * \trace       DSGN-Osab-MemoryView
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReadableMemoryView() const noexcept -> ReadableMemoryView { return ReadableMemoryView{GetMemoryView()}; }

  /*!
   * \brief       Get underlying operating system handle for this memory object.
   *
   * \details     Be aware that the meaning of the returned integer value may vary between OS and/or memory object
   *              types. For some operating systems and/or memory object types, there may not even be a notion of an
   *              integer memory object handle. As such, code using this function may not be very portable.
   *              The user of OsAbstraction shall only perform operations in the returned handle which do not change the
   *              state of the underlying operating system memory object.
   *              See CDD of memory object implementations for concrete meaning of this handle for a specific type of
   *              memory.
   *
   * \return      Underlying operating system handle for this memory object or amsr::memory::kInvalidMemoryHandle if
   *              there is no notion of an integer memory object handle for the type of this memory object and/or this
   *              OS.
   *              The returned handle is only valid as long as this object still exists.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   * \trace       DSGN-Osab-GetInternalMemoryHandle
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual auto GetMemoryHandle() const noexcept -> MemoryHandle = 0;

 protected:
  /*!
   * \brief       Copy constructs a ReadableMemoryInterface.
   */
  ReadableMemoryInterface(ReadableMemoryInterface const& other) noexcept = default;

  /*!
   * \brief       Move constructs a ReadableMemoryInterface.
   */
  ReadableMemoryInterface(ReadableMemoryInterface&& other) noexcept = default;

  /*!
   * \brief       Copy assigns a ReadableMemoryInterface.
   */
  ReadableMemoryInterface& operator=(ReadableMemoryInterface const& other) & noexcept = default;

  /*!
   * \brief       Move assigns a ReadableMemoryInterface.
   */
  ReadableMemoryInterface& operator=(ReadableMemoryInterface&& other) & noexcept = default;

  /*!
   * \brief       Get view to the mapped memory of this object.
   *
   * \return      View to the mapped memory of this object.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto GetMemoryView() const noexcept -> ::amsr::core::Span<std::uint8_t> { return view_; }

 private:
  /*!
   * \brief       View to the mapped memory of this object.
   */
  ::amsr::core::Span<std::uint8_t> view_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_MEMORY_INTERFACE_H_
