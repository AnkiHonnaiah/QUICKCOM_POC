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
/*!        \file  log_source_errors.h
 *        \brief  Error mapping for Linux.
 *        \unit   osabstraction::log_source
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_INTERNAL_LOG_SOURCE_ERRORS_H_
#define LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_INTERNAL_LOG_SOURCE_ERRORS_H_

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace syslog {
namespace internal {

/*!
 * \brief Handles errors returned by close or fclose.
 *
 * \param[in]       error      The error number set by close or fclose.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
void HandleCloseError(osabstraction::internal::OsErrorNumber error) noexcept;

/*!
 * \brief Maps errors returned by socket to a result.
 *
 * \param[in]       error      The error number set by socket.
 *
 * \return          An error code for the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapSocketErrors(osabstraction::internal::OsErrorNumber error) noexcept -> amsr::core::ErrorCode;

/*!
 * \brief Maps errors returned by bind to a result.
 *
 * \param[in]       error      The error number set by bind.
 *
 * \return          An error code for the given error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
auto MapBindErrors(osabstraction::internal::OsErrorNumber error) noexcept -> amsr::core::ErrorCode;

}  // namespace internal
}  // namespace syslog
}  // namespace amsr

#endif  // LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_INTERNAL_LOG_SOURCE_ERRORS_H_
