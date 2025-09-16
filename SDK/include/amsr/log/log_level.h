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
/**     \file       log_level.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::LogLevel
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_LOG_LEVEL_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_LOG_LEVEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/log/internal/log_error_domain.h"
#include "vac/container/string_literals.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {
/*!
 * \brief  List of possible severity levels.
 *
 * \details As defined in SWS_LOG_00018
 * \details As defined in PRS_Dlt_00619
 *
 * \vpublic
 */
enum class LogLevel : std::uint8_t { kOff = 0, kFatal = 1, kError = 2, kWarn = 3, kInfo = 4, kDebug = 5, kVerbose = 6 };

namespace internal {
/*!
 * \brief Converts a string to a log level
 * \param view The string to convert
 * \return The converted log level
 * \spec
 *   requires true;
 * \endspec
 */
inline auto ConvertStringToLogLevel(core::StringView view) noexcept -> core::Result<LogLevel> {
  using vac::language::Switch;
  using vac::container::literals::operator""_sv;

  return Switch<core::Result<LogLevel>>(view)
      .Case("off"_sv, Ok(LogLevel::kOff))
      .Case("fatal"_sv, Ok(LogLevel::kFatal))
      .Case("error"_sv, Ok(LogLevel::kError))
      .Case("warn"_sv, Ok(LogLevel::kWarn))
      .Case("info"_sv, Ok(LogLevel::kInfo))
      .Case("debug"_sv, Ok(LogLevel::kDebug))
      .Case("verbose"_sv, Ok(LogLevel::kVerbose))
      .Default(Err(LogErrc::kInitializationFailed, "Invalid Log Level."));
}
/*!
 * \brief Converts a string to a log level
 * \param view The string to convert
 * \return The converted log level
 * \spec
 *   requires true;
 * \endspec
 */
inline auto ConvertLegacyStringToLogLevel(core::StringView view) noexcept -> core::Result<LogLevel> {
  using vac::language::Switch;
  using vac::container::literals::operator""_sv;

  return Switch<core::Result<LogLevel>>(view)
      .Case("kOff"_sv, Ok(LogLevel::kOff))
      .Case("kFatal"_sv, Ok(LogLevel::kFatal))
      .Case("kError"_sv, Ok(LogLevel::kError))
      .Case("kWarn"_sv, Ok(LogLevel::kWarn))
      .Case("kInfo"_sv, Ok(LogLevel::kInfo))
      .Case("kDebug"_sv, Ok(LogLevel::kDebug))
      .Case("kVerbose"_sv, Ok(LogLevel::kVerbose))
      .Default(Err(LogErrc::kInitializationFailed, "Invalid Log Level."));
}
/*!
 * \brief Converts an integer to a log level
 * \param value The integer to convert
 * \return The converted log level
 */
inline auto ConvertIntegerToLogLevel(std::uint8_t value) noexcept -> core::Result<LogLevel> {
  using vac::language::Switch;

  return Switch<core::Result<LogLevel>>(value)
      .Case(0, Ok(LogLevel::kOff))
      .Case(1, Ok(LogLevel::kFatal))
      .Case(2, Ok(LogLevel::kError))
      .Case(3, Ok(LogLevel::kWarn))
      .Case(4, Ok(LogLevel::kInfo))
      .Case(5, Ok(LogLevel::kDebug))
      .Case(6, Ok(LogLevel::kVerbose))
      .Default(Err(LogErrc::kRuntimeError, "Value cannot be interpreted as LogLevel."));
}
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_LOG_LEVEL_H_
