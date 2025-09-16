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
 *        \brief  Provides common types used in Safe IPC implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_DATA_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_DATA_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Null-terminated file path for OS system calls.
 */
struct FilePath {
  /*!
   * \brief   File path.
   */
  char const* value;
};

/*!
 * \brief Size of a file.
 */
struct FileSize {
  /*!
   * \brief     Size in bytes.
   * \details   FileSize is used for memory mapped files. Thus files that cannot be entirely mapped into the memory
   *            are not supported.
   */
  std::size_t value;
};

/*!
 * \brief Type for file mode.
 */
struct FileMode {
  /*!
   * \brief File mode.
   */
  mode_t value;
};

/*!
 * \brief The supported access modes needed to open a file.
 */
enum class OpenAccessMode : std::int32_t {
  kReadOnly = O_RDONLY, /*!< Read only access mode. */
  kReadWrite = O_RDWR   /*!< Read/Write access mode. */
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_DATA_TYPES_H_
