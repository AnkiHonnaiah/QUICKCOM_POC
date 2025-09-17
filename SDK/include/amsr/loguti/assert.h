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
/**     \file
 *      \brief      Provide functionality to assert with a log message
 *      \details    Provide assert functionalities that are available also in release builds.
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ASSERT_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ASSERT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

#include "amsr/log/internal/log_error_domain.h"
#include "amsr/loguti/abort.h"

namespace amsr {
namespace loguti {
/*!
 * \brief Asserts that a condition holds and aborts in case it doesn't
 * \param condition The condition to check
 * \param ctx_id The context ID to use if logging is active.
 * \param message The message to log on failure
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Assert
 */
inline void Assert(bool condition, vac::container::CStringView ctx_id, vac::container::CStringView message) noexcept {
  if (not condition) {
    Abort(ctx_id, message);
  }
}

/*!
 * \brief Asserts that a condition holds and aborts in case it doesn't
 * \param condition The condition to check
 * \param message The message to log on failure
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Assert
 */
inline void Assert(bool condition, vac::container::CStringView message) noexcept {
  Assert(condition, internal::kVctrCtxId, message);
}
}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_ASSERT_H_
