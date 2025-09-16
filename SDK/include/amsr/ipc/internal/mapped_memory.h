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
 *        \unit   osabstraction::ipc::IpcConnection_QNX
 *        \unit   osabstraction::ipc::IpcConnection_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_IPC_INTERNAL_MAPPED_MEMORY_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_IPC_INTERNAL_MAPPED_MEMORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/span.h"

namespace amsr {
namespace ipc {
namespace internal {

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
   */
  MappedMemory() noexcept = default;

  /*!
   * \brief           Creates a MappedMemory object that manages a section of mapped memory.
   *
   * \param[in]       address   Base address of the memory mapping.
   * \param[in]       size      Length of the memory mapping.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit MappedMemory(std::uint8_t* address, std::size_t size) noexcept : address_{address}, size_{size} {}

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
  ~MappedMemory() noexcept {
    Release();  // VCA_OSA_CALL_OBJ_PASS_VOID
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
   */
  auto GetView() const noexcept -> ::amsr::core::Span<std::uint8_t> {
    return ::amsr::core::Span<std::uint8_t>{address_, size_};
  }

 private:
  /*!
   * \brief           Unmaps the managed mapped memory and resets address_ and size_ if this object owns any memory.
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
   * \brief   Base address of the mapped memory.
   */
  std::uint8_t* address_{nullptr};

  /*!
   * \brief   Size of the mapped memory.
   */
  std::size_t size_{0};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_IPC_INTERNAL_MAPPED_MEMORY_H_
