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
 *        \brief  Maps the internal errors.
 *
 *      \details  Maps the errors for every basic socket service request.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ptp {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps the SetBlockingModeError to an error message and aborts process execution.
 * \param[in] number Error number
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \spec
 *   requires true;
 * \endspec
 */
void MapSetBlockingModeError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   The process has insufficient privileges to create a
 *                                                            socket.
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to create a socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::ErrorCode MapCreateSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error numbers to the appropriate abort messages.
 *
 * \details This function always aborts on a close error.
 *
 * \param[in] number The error number
 *
 * \spec
 *   requires true;
 * \endspec
 */
void MapCloseSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category. The
 *                                                              error may be ioctl command specific.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation..
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     Native handle is not associated  with character-special
 *                                                              device or device does not support the operation.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     (Linux Only) Not allowed to perform the operation.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::ErrorCode MapIoctlError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_H_
