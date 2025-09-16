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
/*!        \file  native_log_message.h
 *        \brief  NativeLogMessage struct
 *        \unit   osabstraction::log_source
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LOGSOURCE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_NATIVE_LOG_MESSAGE_H_
#define LIB_LOGSOURCE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_NATIVE_LOG_MESSAGE_H_

#include "amsr/core/string.h"
#include "osabstraction/process/process_types.h"

namespace amsr {
namespace syslog {

/*!
 * \brief The severity of a log message.
 * \vprivate Vector product internal API
 */
enum class Severity : std::uint8_t {
  /*!
   * \brief A fatal unrecoverable error.
   */
  kFatal,
  /*!
   * \brief A recoverable error condition.
   */
  kError,
  /*!
   * \brief A warning condition.
   */
  kWarn,
  /*!
   * \brief An informational message.
   */
  kInfo,
  /*!
   * \brief Debugging information.
   */
  kDebug,
  /*!
   * \brief Very detailed debugging information.
   */
  kVerbose
};

/*!
 * \brief A log message from the native logging system.
 * \vprivate Vector product internal API
 */
struct NativeLogMessage {
  /*!
   * \brief A timestamp at which the message was logged.
   * \details The timestamp is given in an unspecified format that depends on the operating system. The timestamp
   *          might only be precise to whole seconds.
   */
  ::amsr::core::String timestamp;
  /*!
   * \brief The severity of the logged message.
   */
  Severity severity{Severity::kInfo};
  /*!
   * \brief The process ID from which the message originated from.
   * \details If this information is not available the value will be osabstraction::process::kInvalidProcessId.
   */
  osabstraction::process::ProcessId pid{osabstraction::process::kInvalidProcessId};
  /*!
   * \brief A string identifying the process from which the message originated from.
   */
  ::amsr::core::String process_name;
  /*!
   * \brief The contents of the message.
   */
  ::amsr::core::String payload;
};

}  // namespace syslog
}  // namespace amsr

#endif  // LIB_LOGSOURCE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_NATIVE_LOG_MESSAGE_H_
