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
 *        \brief  Provides compile-time SafeIPC file path configuration.
 *        \unit   osabstraction::ipc::IpcUtils_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_PATH_CONFIGURATION_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_PATH_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <climits>
#include <cstddef>
#include <cstdint>
#include "vac/container/c_string_view.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Provides compile-time SafeIPC file path configuration.
 */
class PathConfiguration {
 public:
  /*!
   * \brief   Maximum allowed length for all file paths used in SafeIPC, does not include null-terminator.
   *
   * \details It is checked during compile-time with a static_assert that this maximum value is not exceeded. Note that
   *          ENAMETOOLONG might still be returned by a system call, e.g. when the configured path contains links and
   *          the expanded path would be too long or a component of the configured file path is too long or the file
   *          system does not support file paths with PATH_MAX length.
   */
  static constexpr std::size_t kFilePathMaxLength{PATH_MAX};

  /*!
   * \brief   Path to the directory where all SafeIPC server files shall be placed.
   *
   * \return Path to the directory where all SafeIPC server files shall be placed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  static auto GetIpcServerWorkingDir() noexcept -> vac::container::CStringView;

  /*!
   * \brief  Path to the directory where all SafeIPC shared memory files shall be placed.
   *
   * \return Path to the directory where all SafeIPC shared memory files shall be placed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  static auto GetIpcConnectionWorkingDir() noexcept -> vac::container::CStringView;

  /*!
   * \brief   printf-like format string that can be used to resolve a server file path.
   *
   * \details Contains two std::uint32_t format specifiers for the server address domain and the server address port.
   *
   * \return printf-like format string that can be used to resolve a server file path.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */

  static auto GetIpcServerFileFormat() noexcept -> vac::container::CStringView;

  /*!
   * \brief   printf-like format string that can be used to resolve a shared memory file path.
   *
   * \details Contains one std::uint64_t format specifier for the shared memory ID.
   *
   * \return printf-like format string that can be used to resolve a shared memory file path.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  static auto GetIpcShmFileFormat() noexcept -> vac::container::CStringView;
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_PATH_CONFIGURATION_H_
