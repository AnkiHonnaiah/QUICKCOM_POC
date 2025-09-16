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
 *        \brief  Maps the QNX OS interface errors.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_LINUX_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_LINUX_H_

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
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to bind address.
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable        The address is not available on the local machine.
 * \retval osabstraction::OsabErrc::kAddressError               The address is already in use on the local machine.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapBindSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error for recvmsg() call.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \retval osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapReceiveFromSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kBusy                       No message in socket error queue.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapRecvMsgFromErrorQueueError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 * \retval osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g. network interface down.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapSendToSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \details
 * Aborts on fatal errors and maps non-fatal errors. Not all operating system return all errors.
 *
 * \param[in] number Error number
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   The system settings do not support the passed option.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapGetSocketOptionError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \details
 * Aborts on fatal errors and maps non-fatal errors. Not all operating system return all errors.
 *
 * \param[in] number Error number
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   The system settings do not support the passed option.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable      The address is not available on the local machine.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapSetSocketOptionError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_ERRORS_LINUX_H_
