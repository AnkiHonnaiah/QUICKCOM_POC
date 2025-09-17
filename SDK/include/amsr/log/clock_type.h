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
/**     \file       clock_type.h
 *      \brief      Contains clock type information.
 *      \details
 *
 *      \unit       LogAndTrace::Common::Clock
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_CLOCK_TYPE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_CLOCK_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/log_error_domain.h"
#include "vac/container/string_literals.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief List of possible clock types.
 */
enum class ClockType : std::uint8_t { kSystem = 1, kSteady = 2, kHighResolution = 3, kDefault = kHighResolution };

/*!
 * \brief Converts a string to a ClockType enum
 * \param view The string to convert
 * \return The converted clock type
 * \spec
 *   requires true;
 * \endspec
 */
inline auto ConvertStringToClockType(core::StringView view) noexcept -> core::Result<ClockType> {
  using vac::language::Switch;
  using vac::container::literals::operator""_sv;

  return Switch<core::Result<ClockType>>(view)
      .Case("system"_sv, Ok(ClockType::kSystem))
      .Case("steady"_sv, Ok(ClockType::kSteady))
      .Default(Err(LogErrc::kInitializationFailed, "Invalid Clock Type."));
}
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_CLOCK_TYPE_H_
