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
/*!        \file  ara_com_logger.h
 *        \brief  Contains the component specific logger class
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_LOGGING_ARA_COM_LOGGER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_LOGGING_ARA_COM_LOGGER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "ara/core/array.h"
#include "ara/core/string_view.h"
#include "ara/log/logging.h"
#include "osabstraction/process/process.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_tp {
namespace logging {

/*!
 * \brief Type-alias for StringView.
 */
using StringView = ::ara::core::StringView;

/*!
 * \brief AraCom Logging Wrapper.
 * \details This is a wrapper for ara::log::Logger that behaves exactly the same except that it inserts a pre-defined
 * prefix in front of the logging message.
 */
class AraComLogger final {
  /*!
   * \brief The type for line numbers
   */
  using LineNumber = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_SomeIpDaemon_AutosarC++17_10-A3.9.1_basic_type

  /*!
   * \brief Max logged size of the custom prefix string passed to the ctor. Longer prefixes will be cropped.
   */
  static constexpr std::size_t kCustomPrefixMaxSize{50};

  /*!
   * \brief Type-alias for LogStream.
   */
  using LogStream = ::ara::log::LogStream;

 public:
  /*!
   * \brief Construct a logger.
   * \param[in] prefix The prefix which should be printed in front of the logging message.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  explicit AraComLogger(StringView const prefix) noexcept;

  /*!
   * \brief     Wrapper function for ara::log::LogFatal() to put the prefix, function name and line in front.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Printed as prefix to ease navigation to the particular code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  template <typename LogMessage>
  void LogFatal(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogFatal([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Wrapper function for ara::log::LogError() to put the prefix, function name and line in front of each log
   *            message.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Printed as prefix to ease navigation to the particular code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    TRUE
   */

  template <typename LogMessage>
  void LogError(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    this->logger_.LogError([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Wrapper function for ara::log::LogWarn() to put the prefix, function name and line in front of each log
   *            message.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Printed as prefix to ease navigation to the particular code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    TRUE
   */
  template <typename LogMessage>
  void LogWarn(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    this->logger_.LogWarn([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Wrapper function for ara::log::LogInfo() to put the prefix, function name and line in front.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Printed as prefix to ease navigation to the particular code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  template <typename LogMessage>
  void LogInfo(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogInfo([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Wrapper function for ara::log::LogDebug() to put the prefix, function name and line in front.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Line number printed as prefix to ease navigation to the code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogDebug(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogDebug([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Wrapper function for ara::log::LogVerbose() to put the prefix, function name and line in front of each
   *            log message.
   * \details   Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line          Printed as prefix to ease navigation to the particular code part.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogVerbose(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    this->logger_.LogVerbose([&function_name, &line, &print_log_msg, this](LogStream& s) {
      this->AddPrefix(s, function_name, line);
      print_log_msg(s);
    });
  }

  /*!
   * \brief     Check if current configured log will pass desired log level.
   * \param[in] log_level Log level to verify
   * \return    True if desired log level satisfies the configured reporting level, otherwise False.
   * \context   ANY
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool IsLogEnabled(ara::log::LogLevel const log_level) const noexcept { return logger_.IsEnabled(log_level); };

  /*!
   * \brief       Wrapper function overload for ara::log::LogVerbose() for logging without a custom log message.
   * \details     Log arguments are evaluated only if log level is sufficient.
   * \param[in]   function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in]   line Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogVerbose(char const* function_name, LineNumber const line) const noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    logger_.LogVerbose([&function_name, &line, this](LogStream& s) { this->AddPrefix(s, function_name, line); });
  }

  /*!
   * \brief       Wrapper function overload for ara::log::LogDebug() for logging without a custom log message.
   * \details     Log arguments are evaluated only if log level is sufficient.
   * \param[in]   function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in]   line          Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogDebug(char const* function_name, LineNumber const line) const noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    logger_.LogDebug([&function_name, &line, this](LogStream& s) { this->AddPrefix(s, function_name, line); });
  }

 private:
  /*!
   * \brief   Size of the string buffer containing the logger prefix.
   * \details Contains:
   *          - 20 characters for process ID (uint64 value).
   *          - 4 characters for '[:] ' and null termination.
   *          - N characters for custom prefix string.
   *          - 1 byte null-termination.
   */
  static constexpr std::size_t kPrefixBufferSize{20 + 4 + kCustomPrefixMaxSize + 1};

  /*!
   * \brief       Constructs the logger prefix string.
   * \param[in]   prefix  Custom free-text prefix string.
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ConstructPrefix(StringView prefix) noexcept -> void;

  /*!
   * \brief         Adds a prefix to a provided stream containing the function name and line.
   * \param[in,out] stream        An ara::log::Logstream on which the prefix will be added.
   * \param[in]     function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in]     line          Printed as prefix to ease navigation to the particular code part.
   * \pre           -
   * \context       ANY
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   * \internal
   * - Add the prefix, the function name and the line number with delimiters to the log stream.
   * \endinternal
   */
  void AddPrefix(LogStream& stream, char const* function_name, LineNumber const line) const noexcept {
    stream << prefix_ << function_name << ":" << line << ": ";
  }

  /*!
   * \brief The actual wrapped ara::log::Logger used for logging.
   */
  ara::log::Logger& logger_;

  /*!
   * \brief Buffer for the logger prefix string.
   */
  ::ara::core::Array<char, kPrefixBufferSize> prefix_{};
};

}  // namespace logging
}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_LOGGING_ARA_COM_LOGGER_H_
