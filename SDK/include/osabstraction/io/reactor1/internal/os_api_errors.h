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
 *        \brief  Provides error mappings for Linux operating system APIs used in Reactor1.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_OS_API_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_OS_API_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"

namespace osabstraction {
namespace io {
namespace reactor1 {
namespace internal {

/*!
 * \brief Handles errors during insertion of epoll entry.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to add file descriptor to
 *                                                            epoll instance.
 * \retval osabstraction::OsabErrc::kApiError                 Wrong file descriptor usage while adding it to the epoll
 *                                                            instance.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto HandleEpollCtlAddError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Handles errors during modification of epoll entry.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kApiError                 Wrong file descriptor usage while changing epoll settings.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto HandleEpollCtlModifyError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Handles the error during removal of epoll entry.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kApiError         Wrong file descriptor usage while changing epoll settings.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto HandleEpollCtlDeleteError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps close error numbers to different error messages.
 *
 * \param[in] number The error number
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
void HandleCloseError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace internal
}  // namespace reactor1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_OS_API_ERRORS_H_
