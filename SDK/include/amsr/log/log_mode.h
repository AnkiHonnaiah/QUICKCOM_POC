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
/**     \file       log_mode.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::LogMode
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_LOG_MODE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_LOG_MODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <type_traits>
#include "amsr/core/abort.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {

/*!
 * \brief Log mode. Flags, used to configure the sink for log messages.
 *
 * \note In order to combine flags, at least the OR and AND operators needs to be provided for this type.
 *
 * \details As defined in SWS_LOG_00019
 *
 * \vpublic
 */
enum class LogMode : std::uint8_t {
  kNone = 0x00,
  kRemote = 0x01,
  kFile = 0x02,
  kConsole = 0x04,
  kSysLog = 0x08,
  kCustom = 0x10,
  kAll = kRemote | kFile | kConsole | kSysLog | kCustom
};

namespace internal {
/*! \brief Enable _sv operator. */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::operator""_sv;
/*! \brief Literal for the console sink. */
constexpr static core::StringView kConsoleSink{"console"_sv};
/*! \brief Literal for the file sink. */
constexpr static core::StringView kFileSink{"file"_sv};
/*! \brief Literal for the remote sink. */
constexpr static core::StringView kRemoteSink{"remote"_sv};
/*! \brief Literal for the syslog sink. */
constexpr static core::StringView kSysLogSink{"syslog"_sv};
/*! \brief Literal for the custom sink. */
constexpr static core::StringView kCustomSink{"custom"_sv};
/*! \brief Literal for no sink. */
constexpr static core::StringView kNoneSink{"kNone"_sv};
/*! \brief Literal for all sinks. */
constexpr static core::StringView kAllSink{"all"_sv};

/*!
 * \brief      Converts a string to a log mode.
 * \note       Conversion limited to configurable sink variants at application startup,
 *             meaning kAllSink is excluded.
 *
 * \param view The string to convert.
 * \return     The converted log mode.
 * \spec
 *   requires true;
 * \endspec
 */
inline auto ConvertStringToLogMode(core::StringView view) noexcept -> core::Result<LogMode> {
  return vac::language::Switch<core::Result<LogMode>>(view)
      .Case(kConsoleSink, Ok(LogMode::kConsole))
      .Case("kConsole"_sv, Ok(LogMode::kConsole))
      .Case(kFileSink, Ok(LogMode::kFile))
      .Case("kFile"_sv, Ok(LogMode::kFile))
      .Case(kRemoteSink, Ok(LogMode::kRemote))
      .Case("kRemote"_sv, Ok(LogMode::kRemote))
      .Case(kSysLogSink, Ok(LogMode::kSysLog))
      .Case("kSysLog"_sv, Ok(LogMode::kSysLog))
      .Case(kCustomSink, Ok(LogMode::kCustom))
      .Case("kCustom"_sv, Ok(LogMode::kCustom))
      .Case(kNoneSink, Ok(LogMode::kNone))
      .Default(Err(LogErrc::kInitializationFailed, "Invalid Log Mode."));
}

/*!
 * \brief      Converts a log mode to a string.
 * \note       Conversion limited to sink variants susceptible to configuration changes
 *             during runtime, meaning LogMode::kNone is excluded.
 *
 * \param sink The log mode to convert.
 * \return     The converted string.
 *
 */
inline auto ConvertLogModeToString(LogMode sink) noexcept -> core::Result<core::StringView> {
  return vac::language::Switch<core::Result<core::StringView>>(sink)
      .Case(LogMode::kConsole, Ok(core::StringView{kConsoleSink}))
      .Case(LogMode::kFile, Ok(core::StringView{kFileSink}))
      .Case(LogMode::kRemote, Ok(core::StringView{kRemoteSink}))
      .Case(LogMode::kSysLog, Ok(core::StringView{kSysLogSink}))
      .Case(LogMode::kCustom, Ok(core::StringView{kCustomSink}))
      .Case(LogMode::kAll, Ok(core::StringView{kAllSink}))
      .Default(Err(LogErrc::kInitializationFailed, "Invalid Log Mode."));
}
}  // namespace internal

/*!
 * \brief Alias for underlying type of LogMode
 *
 * \vprivate Component private.
 */
using UnderlyingLogModeType = std::underlying_type<LogMode>::type;

/*!
 * \brief Operator overload of bitwise OR for enum class
 * \param a First parameter
 * \param b Second parameter
 * \return Returns bitwise OR operated result of both input arguments
 * \spec
 *   requires true;
 * \endspec
 *
 * \details As defined in SWS_LOG_00019
 * \vpublic
 */
inline auto operator|(LogMode a, LogMode b) noexcept -> LogMode {
  // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_external_API
  return static_cast<LogMode>(static_cast<UnderlyingLogModeType>(a) | static_cast<UnderlyingLogModeType>(b));
}

/*!
 * \brief Operator overload of bitwise AND for enum class
 * \param a First parameter
 * \param b Second parameter
 * \return Returns bitwise AND operated result of both input arguments
 * \spec
 *   requires true;
 * \endspec
 *
 * \details As defined in SWS_LOG_00019
 * \vpublic
 */
inline auto operator&(LogMode a, LogMode b) noexcept -> LogMode {
  // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_external_API
  return static_cast<LogMode>(static_cast<UnderlyingLogModeType>(a) & static_cast<UnderlyingLogModeType>(b));
}

// VECTOR NC AutosarC++17_10-A8.4.4: MD_LOG_AutosarC++17_10-A8.4.4_external_API
/*!
 * \brief Operator overload of bitwise OR and assign for enum class
 * \param a First parameter
 * \param b Second parameter
 * \return Operate bitwise OR and assign result
 * \spec
 *   requires true;
 * \endspec
 *
 * \details As defined in SWS_LOG_00019
 * \vpublic
 */
inline auto operator|=(LogMode& a, LogMode b) noexcept -> LogMode& {
  // VECTOR NC AutosarC++17_10-A4.5.1: MD_LOG_AutosarC++17_10-A4.5.1_bit_mask
  a = a | b;
  return a;
}

// VECTOR NC AutosarC++17_10-A8.4.4: MD_LOG_AutosarC++17_10-A8.4.4_external_API
/*!
 * \brief Operator overload of bitwise AND and assign for enum class
 * \param a First parameter
 * \param b Second parameter
 * \return Operate bitwise AND and assign result
 * \spec
 *   requires true;
 * \endspec
 *
 * \details As defined in SWS_LOG_00019
 * \vpublic
 */
inline auto operator&=(LogMode& a, LogMode b) noexcept -> LogMode& {
  // VECTOR NC AutosarC++17_10-A4.5.1: MD_LOG_AutosarC++17_10-A4.5.1_bit_mask
  a = a & b;
  return a;
}
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_LOG_MODE_H_
