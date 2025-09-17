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
/**     \file       asr.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_ASR_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_ASR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/instance_specifier.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/loguti/stream/ns.h"
#include "ara/log/logstream.h"
#include "vac/container/c_string_view.h"

namespace AMSR_LOGUTI_LOG_NS {
namespace log {
/*!
 * \brief   Appends a result to the given log stream.
 *
 * \tparam  LoggableT   The value type of the result.
 *                      Must be a type supported by ara::log::LogStream as specified
 *                      by the AUTOSAR standard (19-03), or a type specified in CREQ-LogUtility-StreamExtensions.
 *                      LoggableT must also comply with the template parameter requirements of
 *                      amsr::core::Result<LoggableT, LoggableE>.
 * \tparam  LoggableE   The error type of the result.
 *                      Must be a type supported by ara::log::LogStream as specified
 *                      by the AUTOSAR standard (19-03), or a type specified in the CREQ-LogUtility-StreamExtensions.
 *                      LoggableE must also comply with the template parameter requirements of
 *                      amsr::core::Result<LoggableT, LoggableE>.
 *
 * \param   stream      The log stream to append to.
 * \param   value       The value to be logged.
 *
 * \return  The passed log stream.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 * \unit Stream
 *
 * \internal
 * - If the given Result contains a value:
 *   - Log the value.
 * - Otherwise:
 *   - Log the error.
 * \endinternal
 */
template <typename LoggableT, typename LoggableE>
auto operator<<(ara::log::LogStream& stream, amsr::core::Result<LoggableT, LoggableE> const& value) noexcept
    -> ara::log::LogStream& {
  if (value.HasValue()) {
    // VECTOR NC AutosarC++17_10-A4.5.1: MD_LOGU_AutosarC++17_10-A4.5.1_shift_op_false_positive
    stream << value.Value();  // VCA_LOGUTI_CHECKED_WRITES
  } else {
    stream << value.Error();
  }
  return stream;
}

/*!
 * \brief   Appends an optional value to the given log stream.
 *
 * \tparam  Loggable  The value type of the Optional.
 *                    Must be a type supported by ara::log::LogStream as specified
 *                    by the AUTOSAR standard (19-03), or a type specified in CREQ-LogUtility-StreamExtensions.
 *                    Loggable must also comply with the template parameter requirements of
 *                    amsr::core::Optional<Loggable>.
 *
 * \param   stream    The log stream to append to.
 * \param   value     The value to be logged.
 *
 * \return  The passed log stream.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 * \unit Stream
 *
 * \internal
 * - If the given optional contains a value:
 *   - Log the value.
 * - Otherwise:
 *   - Log an empty value.
 * \endinternal
 */
template <typename Loggable>
auto operator<<(ara::log::LogStream& stream, amsr::core::Optional<Loggable> const& value) noexcept
    -> ara::log::LogStream& {
  if (value.has_value()) {
    // VECTOR NC AutosarC++17_10-A4.5.1: MD_LOGU_AutosarC++17_10-A4.5.1_shift_op_false_positive
    stream << *value;  // VCA_LOGUTI_CHECKED_WRITES
  } else {
    stream << vac::container::CStringView::FromLiteral("'None'", 6);
  }
  return stream;
}

/*!
 * \brief Appends an InstanceSpecifier to the log stream.
 * \param stream The log stream to append to.
 * \param value The value to be logged.
 * \return The passed log stream.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 * \unit Stream
 */
auto operator<<(ara::log::LogStream& stream, amsr::core::InstanceSpecifier const& value) noexcept
    -> ara::log::LogStream&;
}  // namespace log
}  // namespace AMSR_LOGUTI_LOG_NS
#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_ASR_H_
