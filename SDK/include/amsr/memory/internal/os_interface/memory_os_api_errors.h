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
 *        \brief  Maps memory abstraction system call errors to OsAbstraction errors.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps mapping shared memory error number to osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during mapping of shared memory.
 * \retval osabstraction::OsabErrc::kSize                    Cannot map shared memory because offset and/or size of the
 *                                                           requested mapping is invalid for the shared memory object.
 * \retval osabstraction::OsabErrc::kProtocolError           Not allowed to map shared memory with the specified memory
 *                                                           protection or file descriptor does not refer to a shared
 *                                                           memory object.
 * \retval osabstraction::OsabErrc::kResource                Not enough system resources to map shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto MapMapSharedMemoryError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps creating anonymous shared memory object error number to osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during creation of anonymous shared memory
 *                                                           object.
 * \retval osabstraction::OsabErrc::kResource                Not enough system resources to create anonymous shared
 *                                                           memory object.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapCreateAnonymousShmObjectError(osabstraction::internal::OsErrorNumber number) noexcept
    -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps configuring anonymous shared memory object error number to osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during shared memory object size
 *                                                           configuration.
 * \retval osabstraction::OsabErrc::kResource                Failed to configure size of shared memory object because of
 *                                                           an I/O error.
 * \retval osabstraction::OsabErrc::kSize                    Failed to configure size of shared memory because the
 *                                                           requested size is invalid.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapSetShmSizeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps opening shared memory handle error number to osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during sealing of shared memory object.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapSealShmObjectError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps getting shared memory object seals error number to osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error while getting seals of shared memory
 *                                                           object.
 * \retval osabstraction::OsabErrc::kProtocolError           Getting seals of shared memory object failed because the
 *                                                           object was created invalid.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapGetShmObjectSealsError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_MEMORY_OS_API_ERRORS_H_
