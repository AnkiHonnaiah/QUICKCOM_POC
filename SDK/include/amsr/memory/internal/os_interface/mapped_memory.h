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
 *        \brief  Provides a class that manages a mapped memory section.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MAPPED_MEMORY_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MAPPED_MEMORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/span.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief  Represents and owns a section of mapped memory.
 */
class MappedMemory final {
 public:
  /*!
   * \brief           Creates a MappedMemory object that manages no mapped memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  MappedMemory() noexcept = default;

  /*!
   * \brief           Creates a MappedMemory object that manages a section of mapped memory.
   *
   * \param[in]       mapped_memory_view  View to the mapped memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  explicit MappedMemory(::amsr::core::Span<std::uint8_t> mapped_memory_view) noexcept : view_{mapped_memory_view} {}

  /*!
   * \brief           Move constructs a MappedMemory object.
   *
   * \details         This object takes over the ownership of the mapped memory, the other object will not manage any
   *                  mapped memory anymore.
   *
   * \param[in]       other   MappedMemory object to move construct from.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  MappedMemory(MappedMemory&& other) noexcept;

  /*!
   * \brief           Move assigns a MappedMemory object.
   *
   * \details         This object takes over the ownership of the mapped memory, the other object will not manage any
   *                  mapped memory anymore.
   *
   * \param[in]       other   MappedMemory object to move assign to this object.
   *
   * \return          Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  MappedMemory& operator=(MappedMemory&& other) & noexcept;

  MappedMemory(MappedMemory const&) noexcept = delete;
  MappedMemory& operator=(MappedMemory const&) & noexcept = delete;

  /*!
   * \brief           Releases the mapped memory managed by this object (if this object owns any).
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Release the memory managed by this object (if this object manages any memory).
   * \endinternal
   */
  ~MappedMemory() noexcept {  // COV_OSAB_TOOLINGISSUE
    Release();                // VCA_OSA_CALL_PASS_VOID
  }

  /*!
   * \brief           Returns a span to the managed memory.
   *
   * \return          Returns a span to the mapped memory or empty span if the object does not own any memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetView() const noexcept -> ::amsr::core::Span<std::uint8_t> { return view_; }

 private:
  /*!
   * \brief           Unmaps the managed mapped memory and resets view_ if this object owns any memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void Release() noexcept;

  /*!
   * \brief   View to the mapped memory.
   */
  ::amsr::core::Span<std::uint8_t> view_{};
};

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MAPPED_MEMORY_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_OSAB_TOOLINGISSUE
//   \ACCEPT XX
//   \REASON The destructor is already called by a test case but tooling does not show correct call coverage of
//           destructor.
// COV_JUSTIFICATION_END
