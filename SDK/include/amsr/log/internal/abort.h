/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       abort.h
 *      \brief      Provide functionality to abort with a log message
 *      \details    Provides conditional & unconditional ways to abort
 *
 *      \unit       LogAndTrace::Common::Abort
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ABORT_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ABORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/log/internal/ext_fwd.h"  // IWYU pragma: keep
#include "amsr/log/internal/fallback_logger.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/utils.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Log in case normal logging is not available
 *
 * \param fn The logging function
 *
 * \threadsafe TRUE
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
void LogFallback(FunctionRef<void(FallbackLogger&)> fn) noexcept;

/*!
 * \brief Log in case normal logging is not available
 *
 * \param ec  The error code to log
 * \param msg The message to log
 *
 * \threadsafe TRUE
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
void LogFallback(ErrorCode const& ec, vac::container::CStringView msg) noexcept;

/*!
 * \brief Abort with a logged message
 *
 * \param message The message to print
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
[[noreturn]] void Abort(vac::container::CStringView message) noexcept;

/*!
 * \brief Abort on error codes
 *
 * \param error_code The error code to print
 * \param message    The message to print
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
[[noreturn]] void Abort(core::ErrorCode error_code, vac::container::CStringView message) noexcept;

/*!
 * \brief Asserts that a condition holds and aborts in case it doesn't
 *
 * \param condition The condition to check
 * \param message   The message to log on failure
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
inline void Assert(bool condition, vac::container::CStringView message) noexcept {
  if (not condition) {
    Abort(message);
  }
}

/*!
 * \brief Unwraps the result and logs on error before aborting
 *
 * \param result  The result to unwrap
 * \param message The message to write in case of an error
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
inline void UnwrapOrAbort(core::Result<void> result, vac::container::CStringView message) noexcept {
  if (not result) {
    Abort(result.Error(), message);
  }
}

/*!
 * \brief Unwraps the result and logs on error before aborting
 *
 * \tparam T      The type in the result
 *                T must meet the template parameter requirements of ara::core::Result<T>
 * \param result  The result to unwrap
 * \param message The message to write in case of an error
 * \return        The unwrapped value
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
template <typename T>
auto UnwrapOrAbort(core::Result<T>&& result, vac::container::CStringView message) noexcept -> T {
  UnwrapOrAbort(result.Drop(), message);
  return std::move(result).Value();
}

/*!
 * \brief Unwraps the result and logs on error before aborting
 *
 * \tparam T      The type in the result
 *                T must meet the template parameter requirements of ara::core::Result<T>
 * \param result  The result to unwrap
 * \param message The message to write in case of an error
 * \return        The unwrapped value
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
template <typename T>
auto UnwrapOrAbort(core::Result<T>& result, vac::container::CStringView message) noexcept -> T& {
  UnwrapOrAbort(result.Drop(), message);
  return result.Value();
}

/*!
 * \brief Unwraps the result and logs on error before aborting
 *
 * \tparam T      The type in the result
 *                T must meet the template parameter requirements of ara::core::Result<T>
 * \param result  The result to unwrap
 * \param message The message to write in case of an error
 * \return        The unwrapped value
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
template <typename T>
auto UnwrapOrAbort(core::Result<T> const& result, vac::container::CStringView message) noexcept -> T const& {
  UnwrapOrAbort(result.Drop(), message);
  return result.Value();
}

// VECTOR Disable AutosarC++17_10-A16.0.1: MD_LOG_AutosarC++17_10-A16.0.1_ifdef
/*!
 * \brief Asserts in debug mode that a condition holds and aborts in case it doesn't
 *
 * \param condition The condition to check
 * \param message   The message to log on failure
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
inline void AssertDebug(bool condition, vac::container::CStringView message) noexcept {
#ifndef NDEBUG
  Assert(condition, message);
#else
  static_cast<void>(condition);
  static_cast<void>(message);
#endif
}
// VECTOR Enable AutosarC++17_10-A16.0.1

/*!
 * \brief Check that checks that ara::log::Abort does not recurse.
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component internal.
 */
void SetAbortCalled() noexcept;

/*!
 * \brief Reports the error in case the result is negative.
 *
 * \param result The result to check.
 * \param msg    The message to print.
 * \spec
 *   requires true;
 * \endspec
 */
inline void ReportOnError(Result<void> const& result, vac::container::CStringView msg) {
  if (not result) {
    LogFallback(result.Error(), msg);
  }
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ABORT_H_
