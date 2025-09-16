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
 *        \brief  Common types for memory abstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace memory {

/*!
 * \brief   Type for a memory address.
 */
struct Address {
  /*!
   * \brief   Address value.
   */
  std::uintptr_t value;
};

/*!
 * \brief   Type for an offset (in bytes) into a memory segment.
 */
struct Offset {
  /*!
   * \brief   Offset value.
   */
  std::size_t value;
};

/*!
 * \brief   Type for a size in bytes.
 */
struct SizeBytes {
  /*!
   * \brief   Size.
   */
  std::size_t value;
};

/*!
 * \brief   Type for a memory alignment.
 * \details Valid memory alignments are a power of 2.
 */
struct Alignment {
  /*!
   * \brief   Alignment value.
   */
  std::size_t value;
};

/*!
 * \brief   Type for the number of elements in an array (not the size of the array in bytes).
 */
struct Size {
  /*!
   * \brief   Size.
   */
  std::size_t value;
};

/*!
 * \brief   Type for access rights to memory.
 */
enum class AccessMode : std::int32_t {
  /*!
   * \brief   Memory can only be read, not written.
   */
  kReadOnly = O_RDONLY,
  /*!
   * \brief   Memory can be read and written.
   */
  kReadWrite = O_RDWR
};

/*!
 * \brief   Type for handles to operating system memory objects.
 */
using MemoryHandle = std::int32_t;

/*!
 * \brief   Invalid handle to operating system memory objects.
 * \details Indicates that there exists no operating system handle for the memory object.
 */
constexpr MemoryHandle kInvalidMemoryHandle{-1};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_TYPES_H_
