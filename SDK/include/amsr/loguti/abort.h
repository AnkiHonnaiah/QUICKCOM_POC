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
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ABORT_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ABORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/utils.h"
#include "amsr/loguti/safe_logger.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace loguti {
/*!
 * \brief Abort with a logged message
 * \param ctx_id The context ID to use if logging is active.
 * \param message The message to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] void Abort(vac::container::CStringView ctx_id, vac::container::CStringView message) noexcept;

/*!
 * \brief Abort with a logged message
 * \param message The message to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] inline void Abort(vac::container::CStringView message) noexcept { Abort(internal::kVctrCtxId, message); }

/*!
 * \brief Abort with a logged message
 * \param ctx_id The context ID to use if logging is active.
 * \param message The message stream to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] void Abort(vac::container::CStringView ctx_id,
                        vac::language::FunctionRef<void(SafeLogger&)> message) noexcept;

/*!
 * \brief Abort with a logged message
 * \param message The message stream to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] inline void Abort(vac::language::FunctionRef<void(SafeLogger&)> message) noexcept {
  Abort(internal::kVctrCtxId, message);
}

/*!
 * \brief Abort on error codes
 * \param ctx_id The context ID to use if logging is active.
 * \param error_code The error code to print
 * \param message The message to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] void Abort(vac::container::CStringView ctx_id, amsr::core::ErrorCode error_code,
                        vac::container::CStringView message) noexcept;

/*!
 * \brief Abort on error codes
 * \param error_code The error code to print
 * \param message The message to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] inline void Abort(amsr::core::ErrorCode error_code, vac::container::CStringView message) noexcept {
  Abort(internal::kVctrCtxId, error_code, message);
}

/*!
 * \brief Abort on error codes
 * \param ctx_id The context ID to use if logging is active.
 * \param error_code The error code to print
 * \param message The message stream to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] void Abort(vac::container::CStringView ctx_id, amsr::core::ErrorCode error_code,
                        vac::language::FunctionRef<void(SafeLogger&)> message) noexcept;

/*!
 * \brief Abort on error codes
 * \param error_code The error code to print
 * \param message The message stream to print
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Abort
 */
[[noreturn]] inline void Abort(amsr::core::ErrorCode error_code,
                               vac::language::FunctionRef<void(SafeLogger&)> message) noexcept {
  Abort(internal::kVctrCtxId, error_code, message);
}

}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ABORT_H_
