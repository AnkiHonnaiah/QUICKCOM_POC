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
/*!        \file
 *        \brief  ResourceUsage error handling.
 *        \unit   osabstraction::resource_usage
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace resource_usage {
namespace internal {
namespace os_interface {

/*!
 * \brief Converts an error number that occurred when opening path in filesystem to an ErrorCode.
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
auto MapOpenErrors(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when read from file to an ErrorCode.
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
auto MapReadErrors(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when trying to get clockid to an ErrorCode.
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
auto MapGetCpuClockIdErrors(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Converts an error number that occurred when trying to get clock time to an ErrorCode.
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
auto MapClockGetTimeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace resource_usage
}  // namespace amsr

#endif  // LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_OS_INTERFACE_ERRORS_H_
