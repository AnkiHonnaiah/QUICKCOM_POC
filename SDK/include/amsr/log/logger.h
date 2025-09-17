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
/*!        \file  amsr/log/logger.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include "amsr/core/string.h"
#include "amsr/log/internal/config/compile_time_config.h"
#include "amsr/log/internal/log_levels.h"
#include "amsr/log/internal/memory_pool.h"
#include "amsr/log/internal/types/typed_message_builder.h"
#include "amsr/log/internal/utils.h"
#include "amsr/log/log_mode.h"
#include "amsr/log/log_types.h"
#include "amsr/log/logstream.h"
#include "amsr/log_fwd.h"  // IWYU pragma: export
#include "vac/language/unique_function.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace log {
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric-OO.WMC.One_external_API
/*!
 * \brief The Class Logger represents a logger context.
 * \details A context can be seen as a logger instance within one application or process scope.
 * \trace DSGN-LogAndTrace-Logger
 * \unit LogAndTrace::Logging::Logger
 * \vpublic
 */
class Logger final {
  /*! \brief Friend declaration for streams. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  friend class LogStream;
  /*! \brief Friend declaration for the container who's able to destroy the logger. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  friend class internal::LoggerContainer;

  /*! \brief A null terminated character sequence. */
  using CString = char const*;
  /*! \brief The type for line numbers. */
  using LineNumber = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_LOG_AutosarC++17_10-A3.9.1_basic_type

  /*! Context ID */
  core::String ctx_id_;
  /*! Context Description */
  core::String ctx_description_;
  /*! Log Level */
  internal::LogLevels log_levels_;
  /*! Proxy object providing access to application wide context */
  internal::Ref<internal::LoggerBase> logger_base_;
  /*! Prefix that will be prepended to all log messages */
  core::StringView prefix_;
  /*! \brief Container for all messages. */
  internal::MemoryPool<internal::TypedMessageBuilder> message_pool_{
      1, [](internal::TypedMessageBuilder& m) { m.Clear(); }};

 public:
  /*!
   * \brief Constructor.
   *
   * \param ctx_id          Context ID
   * \param ctx_description Context description
   * \param log_level       Initial log level
   * \param logger_base     Proxy for accessing application configuration
   *
   * \vprivate Vector component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  Logger(core::StringView ctx_id, core::String ctx_description, LogLevel log_level,
         internal::LoggerBase& logger_base) noexcept;

  /*! \brief Use default destructor.
   * \vprivate Vector component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  ~Logger() noexcept = default;
  /*! \brief Delete move Constructor.
   * \vprivate Vector component internal.
   */
  Logger(Logger&&) = delete;
  /*! \brief Delete move assignment operator.
   * \vprivate Vector component internal.
   */
  auto operator=(Logger&&) -> Logger& = delete;
  /*! \brief Delete copy constructor.
   * \vprivate Vector component internal.
   */
  Logger(Logger const&) = delete;
  /*! \brief Delete copy assignment operator.
   * \vprivate Vector component internal.
   */
  auto operator=(Logger const&) -> Logger& = delete;

  /*!
   * \brief Get a LogStream for the specified severity.
   *
   * \param[in] log_level The log level to use.
   * \return              Log stream
   * \details As defined in SWS_LOG_00131
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto WithLevel(LogLevel log_level) noexcept -> LogStream {
    // VCA_LOGAPI_CHECKED_WRITES
    return LogStream{*this, log_level};
  }

  /*!
   * \brief Get a LogStream for severity LogLevel::kFatal
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00008, SWS_LOG_00064
   * \note if LogLevel::kFatal is not enabled, nothing will be logged from this function
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogFatal() noexcept -> LogStream { return this->WithLevel(LogLevel::kFatal); }

  /*!
   * \brief Get a LogStream for severity LogLevel::kError
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00009, SWS_LOG_00065
   * \note if LogLevel::kError is not enabled, nothing will be logged from this function
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogError() noexcept -> LogStream { return this->WithLevel(LogLevel::kError); }

  /*!
   * \brief Get a LogStream for severity LogLevel::kWarn
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00010, SWS_LOG_00066
   * \note if LogLevel::kWarn is not enabled, nothing will be logged from this function
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogWarn() noexcept -> LogStream { return this->WithLevel(LogLevel::kWarn); }

  /*!
   * \brief Get a LogStream for severity LogLevel::kInfo
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00011, SWS_LOG_00067
   * \note if LogLevel::kInfo is not enabled, nothing will be logged from this function
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogInfo() noexcept -> LogStream { return this->WithLevel(LogLevel::kInfo); }

  /*!
   * \brief Get a LogStream for severity LogLevel::kDebug
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00012, SWS_LOG_00068
   * \note if LogLevel::kDebug is not enabled, nothing will be logged from this function
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogDebug() noexcept -> LogStream { return this->WithLevel(LogLevel::kDebug); }

  /*!
   * \brief Get a LogStream for severity LogLevel::kVerbose
   *
   * \return Log stream
   *
   * \details As defined in SWS_LOG_00013, SWS_LOG_00069
   * \note if LogLevel::kVerbose is not enabled, nothing will be logged from this function
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto LogVerbose() noexcept -> LogStream { return this->WithLevel(LogLevel::kVerbose); }

  /*!
   * \brief Log messages for severity LogLevel::kFatal
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogFatal(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kFatal, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kFatal); });
  }

  /*!
   * \brief Log messages for severity LogLevel::kError
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogError(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kError, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kError); });
  }

  /*!
   * \brief Log messages for severity LogLevel::kWarn
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogWarn(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kWarn, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kWarn); });
  }

  /*!
   * \brief Log messages for severity LogLevel::kInfo
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogInfo(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kInfo, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kInfo); });
  }

  /*!
   * \brief Log messages for severity LogLevel::kDebug
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogDebug(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kDebug, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kDebug); });
  }

  /*!
   * \brief Log messages for severity LogLevel::kVerbose
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn The log message
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogVerbose(FunctionRef<void(LogStream&)> fn) noexcept {
    internal::WithFilteredSeverity<LogLevel::kVerbose, internal::config::kMaximumLogLevel>(
        [this, &fn]() { this->Log(fn, LogLevel::kVerbose); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kFatal
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogFatal(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kFatal, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kFatal, func_name, line_number); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kError
   *
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogError(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kError, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kError, func_name, line_number); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kWarn
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogWarn(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kWarn, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kWarn, func_name, line_number); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kInfo
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogInfo(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kInfo, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kInfo, func_name, line_number); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kDebug
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogDebug(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kDebug, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kDebug, func_name, line_number); });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log messages for severity LogLevel::kVerbose
   * \details Log arguments are evaluated only if log is enabled
   *
   * \param fn          The log message
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   *
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Vector component internal.
   */
  void LogVerbose(FunctionRef<void(LogStream&)> fn, CString func_name, LineNumber line_number) noexcept {
    internal::WithFilteredSeverity<LogLevel::kVerbose, internal::config::kMaximumLogLevel>(
        [this, &fn, func_name, line_number]() { this->Log(fn, LogLevel::kVerbose, func_name, line_number); });
  }

  /*!
   * \brief Set log level of logger instance
   *
   * \param log_level Requested log level
   *
   * \trace DSGN-LogAndTrace-FilterMessage
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLogLevel(LogLevel log_level) noexcept;

  /*!
   * \brief Set log level of logger instance
   *
   * \param sink      The sink to set the log level for.
   * \param log_level Requested log level
   *
   * \trace DSGN-LogAndTrace-FilterMessage
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLogLevel(core::StringView sink, LogLevel log_level) noexcept;

  /*!
   * \brief Getter context ID.
   *
   * \return context id
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContext() const noexcept -> core::StringView { return ctx_id_; }

  /*!
   * \brief Getter context description.
   *
   * \return context description
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextDescription() const noexcept -> core::StringView;

  /*!
   * \brief The Application can check if current configured log will pass desired log level.
   *
   * \param log_level Log level to verify
   * \return          True if desired log level satisfies the configuration, otherwise False.
   *
   * \details As defined in SWS_LOG_00007, SWS_LOG_00070
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   *
   * \vpublic
   */
  auto IsEnabled(LogLevel log_level) const noexcept -> bool;

  /*!
   * \brief The Application can check if current configured log message shall be sent.
   *
   * \param sink      The sink to check for.
   * \param log_level Log level to verify
   * \return          True if desired log sink & level satisfy the configuration, otherwise False.
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Component private.
   */
  auto IsEnabled(core::StringView sink, LogLevel log_level) const noexcept -> internal::FilterResult;

  /*!
   * \brief Returns the proxy to the LoggerBase for accessing application ID, remote logger etc.
   *
   * \returns A reference to the logger base
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Component private.
   */
  auto GetLoggerBaseProxy() noexcept -> internal::LoggerBase& { return logger_base_; }

  /*!
   * \brief Returns the proxy to the LoggerBase for accessing application ID, remote logger etc.
   *
   * \returns A reference to the logger base
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate Component private.
   */
  auto GetLoggerBaseProxy() const noexcept -> internal::LoggerBase const& { return logger_base_; }

  /*!
   * \brief Sets a prefix that will be prepended before every log statement
   *
   * \param sv The prefix to use
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   *
   * \vprivate Vector component internal.
   */
  void SetPrefix(core::StringView sv) noexcept { this->prefix_ = sv; }

 private:
  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Log lazily for the specified log level
   *
   * \param fn          The log message wrapped in a lambda
   * \param log_level   The log level for which this is meant
   * \param func_name   The name of the function which contains the log statement
   * \param line_number The line of the log statement
   * \spec
   *   requires true;
   * \endspec
   */
  void Log(FunctionRef<void(LogStream&)> fn, LogLevel log_level, CString func_name, LineNumber line_number) noexcept {
    auto prefixed_message = [func_name, line_number, fn](LogStream& s) {
      s.WithLocation(func_name, line_number);
      fn(s);
    };
    this->Log(std::move(prefixed_message), log_level);
  }

  /*!
   * \brief Log lazily for the specified log level
   *
   * \param fn        The log message wrapped in a lambda
   * \param log_level The log level for which this is meant
   * \spec
   *   requires true;
   * \endspec
   */
  void Log(FunctionRef<void(LogStream&)> fn, LogLevel log_level) noexcept {
    static_assert(std::is_void<decltype(fn(std::declval<LogStream&>()))>::value,
                  "Return type of message must be void!");
    // VCA_LOGAPI_CHECKED_WRITES
    if (this->IsEnabled(log_level)) {
      // VCA_LOGAPI_CHECKED_WRITES
      LogStream stream{*this, log_level};
      fn(stream);
      // VCA_LOGAPI_CHECKED_WRITES
    }
  }

  FRIEND_TEST(UT__BufferPool, LogStreamsReuseMemoryPool);        /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__BufferPool, LogStreamsCreatesNewMessagePools); /*!< \brief Friend declaration for testing. */
};

/*!
 * \brief Logs integer numbers in hexadecimal format.
 *
 * Negatives are represented in 2's complement.
 *
 * \tparam T        The unsigned integer type.
 *                  T shall be of an unsigned integer type for which std::numeric_limits<T>::is_integer returns true
 *                  and std::numeric_limits<T>::is_signed returns false.
 * \param[in] value integer number to be converted into hexadecimal number system
 * \return          The type that has a built-in stream handler
 *
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename std::enable_if_t<
                          std::numeric_limits<T>::is_integer && (!std::numeric_limits<T>::is_signed), std::uint8_t> = 0>
constexpr auto HexFormat(T value) noexcept -> IntLogWrapper<T, internal::NumericBase::kHexBase> {
  return {value};
}

/*!
 * \brief Logs integer numbers in hexadecimal format.
 *
 * \tparam T        The signed integer type
 *                  T shall be of a signed integer type for which std::numeric_limits<T>::is_integer returns true
 *                  and std::numeric_limits<T>::is_signed returns true.
 * \param[in] value Integer number to be converted into hexadecimal number system
 * \return          The type that has a built-in stream handler
 *
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename std::enable_if_t<std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed,
                                                std::uint8_t> = 0>
constexpr auto HexFormat(T value) noexcept -> IntLogWrapper<std::make_unsigned_t<T>, internal::NumericBase::kHexBase> {
  return {ConvertToTwosComplement(value)};
}

/*!
 * \brief Logs integer numbers in binary format.
 *
 * Negatives are represented in 2's complement.
 *
 * \tparam T        The signed integer type
 *                  T shall be of an unsigned integer type for which std::numeric_limits<T>::is_integer returns true
 *                  and std::numeric_limits<T>::is_signed returns false.
 * \param[in] value Integer number to be converted into a binary value
 * \return          The type that has a built-in stream handler
 *
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename std::enable_if_t<
                          std::numeric_limits<T>::is_integer && (!std::numeric_limits<T>::is_signed), std::uint8_t> = 0>
constexpr auto BinFormat(T value) noexcept -> IntLogWrapper<T, internal::NumericBase::kBinBase> {
  return {value};
}

/*!
 * \brief Logs integer numbers in binary format.
 *
 * \tparam T        The signed integer type
 *                  T shall be of a signed integer type for which std::numeric_limits<T>::is_integer returns true
 *                  and std::numeric_limits<T>::is_signed returns true.
 * \param[in] value Integer number to be converted into a binary value
 * \return          The type that has a built-in stream handler
 *
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename std::enable_if_t<std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed,
                                                std::uint8_t> = 0>
constexpr auto BinFormat(T value) noexcept -> IntLogWrapper<std::make_unsigned_t<T>, internal::NumericBase::kBinBase> {
  return {ConvertToTwosComplement(value)};
}

namespace internal {
/*!
 * \brief A container that is able to destroy and thus store the logger.
 *
 * \unit LogAndTrace::Logging::LoggerContainer
 */
class LoggerContainer final {
  /*! \brief The stored logger */
  Logger logger_;

 public:
  /*!
   * \brief Creates the logger with its constructor arguments.
   *
   * \param ctx_id          Context ID
   * \param ctx_description Context description
   * \param log_level       Initial log level
   * \param logger_base     Proxy for accessing application configuration
   * \spec
   *   requires true;
   * \endspec
   */
  LoggerContainer(core::StringView const ctx_id, core::String&& ctx_description, LogLevel log_level,
                  LoggerBase& logger_base) noexcept
      // VCA_LOGAPI_CHECKED_WRITES
      : logger_{ctx_id, std::move(ctx_description), log_level, logger_base} {}

  /*!
   * \brief Get the underlying logger.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() & noexcept -> Ref<Logger> {
    // VCA_LOGAPI_VALID_OBJECT
    return Ref<Logger>{this->logger_};
  }

  /*!
   * \brief Get the underlying logger.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() const& noexcept -> Ref<Logger const> { return Ref<Logger const>{this->logger_}; }
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGER_H_
