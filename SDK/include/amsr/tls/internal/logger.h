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
/*!        \file
 *         \brief  Logger.
 *         \unit   SecCom::LibTLS::TlsProvider
 *
 *********************************************************************************************************************/
#ifndef LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_INTERNAL_LOGGER_H_
#define LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_INTERNAL_LOGGER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string_view.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace tls {
namespace internal {

/*!
 * \brief Logger.
 * \details Wrapper for "ara::log::Logger" for customized logging.
 */
class Logger final {
 public:
  Logger() noexcept = delete;
  ~Logger() noexcept = delete;
  Logger(Logger const&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger const&) = delete;
  Logger& operator=(Logger&&) = delete;

  /*!
   * \brief Static wrapper function for "ara::log::LogFatal()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "fatal" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogFatal(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                       std::uint32_t const line_number) noexcept {
    GetInstance().LogFatal([&custom_prefix, &func_log_msg, &function_name,
                            &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogError()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "error" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogError(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                       std::uint32_t const line_number) noexcept {
    GetInstance().LogError([&custom_prefix, &func_log_msg, &function_name,
                            &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogWarn()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "warn" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogWarn(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                      std::uint32_t const line_number) noexcept {
    GetInstance().LogWarn([&custom_prefix, &func_log_msg, &function_name,
                           &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogInfo()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "info" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogInfo(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                      std::uint32_t const line_number) noexcept {
    GetInstance().LogInfo([&custom_prefix, &func_log_msg, &function_name,
                           &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogDebug()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "debug" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogDebug(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                       std::uint32_t const line_number) noexcept {
    GetInstance().LogDebug([&custom_prefix, &func_log_msg, &function_name,
                            &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogDebug()" to prepend the log message with prefix, function name
   *        and line number (does not support user provided logging function).
   * \details Log message is printed only if the log level is supported.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "debug" level with the user specific logging function as missing.
   * \endinternal
   */
  static void LogDebug(amsr::core::StringView const custom_prefix, char const* function_name,
                       std::uint32_t const line_number) noexcept {
    GetInstance().LogDebug(
        [&custom_prefix, &function_name, &line_number](ara::log::LogStream& log_stream) noexcept -> void {
          LogCommon(custom_prefix, function_name, line_number, log_stream);
        });
  }

  /*!
   * \brief Static wrapper function for "ara::log::LogVerbose()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log at "verbose" level.
   * \endinternal
   */
  template <typename LogMessage>
  static void LogVerbose(amsr::core::StringView const custom_prefix, LogMessage&& func_log_msg,
                         char const* function_name, std::uint32_t const line_number) noexcept {
    GetInstance().LogVerbose([&custom_prefix, &func_log_msg, &function_name,
                              &line_number](ara::log::LogStream& log_stream) noexcept -> void {
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

 private:
  /*!
   * \brief Get singleton instance of the logger.
   * \return A singleton instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \threadsafe TRUE
   * \internal
   * - Get the static instance.
   * \endinternal
   */
  static ara::log::Logger& GetInstance() noexcept {
    using vac::container::literals::operator""_sv;

    static ara::log::Logger& logger{ara::log::CreateLogger("vcsc"_sv, "Logger for TLS provider"_sv)};
    return logger;
  }

  /*!
   * \brief Log the common part of the log message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \param[in] log_stream Log stream to use for logging.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \internal
   * - Log the common part of log messages.
   * \endinternal
   */
  static void LogCommon(amsr::core::StringView const& custom_prefix, char const* function_name,
                        std::uint32_t const line_number, ara::log::LogStream& log_stream) {
    using vac::container::literals::operator""_sv;

    log_stream << "["_sv;
    log_stream << custom_prefix;
    log_stream << "] "_sv;

    log_stream << function_name;
    log_stream << ":"_sv;
    log_stream << line_number;
    log_stream << ": "_sv;
  }
};

}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_INTERNAL_LOGGER_H_
