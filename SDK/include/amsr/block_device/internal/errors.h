/**********************************************************************************************************************
 *  COPYRIGHT
 *
 -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *
 -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  errors.h
 *        \brief  BlockDevice error handling.
 *        \unit   osabstraction::block_device
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_ERRORS_H_
#define LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_ERRORS_H_

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace block_device {
namespace internal {

/*!
 * \brief Converts an error number that occurred when opening a block device to an ErrorCode.
 *
 * \param[in]       number        The error number set by the operating system.
 *
 * \return          An error code corresponding to the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapOpenErrors(osabstraction::internal::OsErrorNumber number) noexcept -> amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when calling fstat on a block device to an ErrorCode.
 *
 * \param[in]       number        The error number set by the operating system.
 *
 * \return          An error code corresponding to the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapStatErrors(osabstraction::internal::OsErrorNumber number) noexcept -> amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when read from a block device to an ErrorCode.
 *
 * \param[in]       number        The error number set by the operating system.
 *
 * \return          An error code corresponding to the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapReadErrors(osabstraction::internal::OsErrorNumber number) noexcept -> amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when writing to a block device to an ErrorCode.
 *
 * \param[in]       number        The error number set by the operating system.
 *
 * \return          An error code corresponding to the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapWriteErrors(osabstraction::internal::OsErrorNumber number) noexcept -> amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when flushing a block device to an ErrorCode.
 *
 * \param[in]       number        The error number set by the operating system.
 *
 * \return          An error code corresponding to the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapFlushErrors(osabstraction::internal::OsErrorNumber number) noexcept -> amsr::core::ErrorCode;

}  // namespace internal
}  // namespace block_device
}  // namespace amsr

#endif  // LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_ERRORS_H_
