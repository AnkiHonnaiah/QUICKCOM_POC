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
/*!        \file  logger.h
 *        \brief  Class wrapping the logging functionalities for convenience and consistency.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/string.h"
#include "amsr/log/internal/lifecycle.h"
#include "ara/log/logging.h"
#include "crypto/common/util/logger_stream.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace common {
namespace util {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

constexpr vac::container::CStringView const kCommonLoggingContext{"CCOM"_sv}; /*!< Context Id for crypto common */
constexpr vac::container::CStringView const kServerLoggingContext{"CSER"_sv}; /*!< Context Id for crypto server */
constexpr vac::container::CStringView const kClientLoggingContext{"CCLI"_sv}; /*!< Context Id for crypto client */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
constexpr vac::container::CStringView const kX509LoggingContext{"C509"_sv}; /*!< Context Id for crypto X509 */
constexpr vac::container::CStringView const kCustomExtensionParserLoggingContext{
    "XCEP"_sv}; /*!< Context Id for crypto ASN1Parser */

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief The logger provides unified access to formatted logging functions.
 * \vprivate Product Private
 */
class Logger final {
 public:
  /*! A null terminated character sequence */
  using CString = char const*;
  /*! The type for line numbers */
  using LineNumber = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_CRYPTO_AutosarC++17_10-A3.9.1_basic_type

  /*!
   * \brief Constructs a logger for the given class with Additional context information.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] ctx_info Additional context information
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Logger(amsr::core::StringView const ctx_id, amsr::core::StringView const ctx_info)
      : ctx_id_{ctx_id}, ctx_info_{ctx_info} {}

  /*!
   * \brief Copy-constructs a logger.
   * \param[in] logger Logger to copy
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Logger(Logger const& logger) = default;

  /*!
   * \brief Move-constructs a logger.
   * \param[in,out] logger Logger to move
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Logger(Logger&& logger) noexcept = default;

  /*!
   * \brief Copy-assigns a logger.
   * \param[in] logger Logger to copy
   * \return Logger.
   * \vprivate Product Private
   */
  Logger& operator=(Logger const& logger) & = default;

  /*!
   * \brief Move-assigns a logger.
   * \param[in] logger Logger to move
   * \return Logger.
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Logger& operator=(Logger&& logger) & noexcept = default;

  /*!
   * \brief Destructs the logger.
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~Logger() noexcept = default;

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on verbose level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogVerbose(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogVerbose());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on verbose level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogVerbose(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogVerbose(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on verbose level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogDebug(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogDebug());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on debug level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogDebug(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogDebug(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on information level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogInfo(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogInfo());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on information level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogInfo(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogInfo(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on warning level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogWarn(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogWarn());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on warning level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogWarn(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogWarn(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on error level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogError(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogError());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on error level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name Function name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogError(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogError(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on fatal level.
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  LoggerStream LogFatal(CString file_name, LineNumber line_num) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogFatal());
    }

    return Log(std::move(stream), file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on fatal level.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream LogFatal(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num) noexcept {
    return Logger{ctx_id, ""_sv}.LogFatal(file_name, line_num);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs an error code at error level.
   * \details Given message should not be empty and not terminated by any kind of punctuation.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] error_code Error code to log
   * \param[in] message Addition error message
   * \param[in] file_name Function name
   * \param[in] line_num Line number
   * \return -
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void LogErrorCode(amsr::core::StringView const ctx_id, amsr::core::ErrorCode error_code,
                           amsr::core::StringView const message, CString file_name, LineNumber line_num) noexcept {
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const error_message = error_code.Message();
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const user_message = error_code.UserMessage();

    if (user_message.empty()) {
      LogError(ctx_id, file_name, line_num) << message << ": " << error_message;
    } else {
      LogError(ctx_id, file_name, line_num) << message << ": " << error_message << " (" << user_message << ")";
    }
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on fatal level and aborts.
   * \param[in] file_name Function name
   * \param[in] line_num Line number
   * \param[in] message Log message
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  [[noreturn]] void LogFatalAndAbort(CString file_name, LineNumber line_num,
                                     amsr::core::StringView const message) const noexcept {
    LoggerStream stream{};

    if (amsr::log::internal::IsComponentInitialized()) {
      stream.SetStream(ara::log::CreateLogger(ctx_id_, ctx_info_).LogFatal());
    }
    Log(std::move(stream), file_name, line_num) << message;
    amsr::core::Abort(message.data());
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Logs a message on fatal level and aborts.
   * \param[in] ctx_id Context ID for the logging instance
   * \param[in] file_name Function name
   * \param[in] line_num Line number
   * \param[in] message Log message
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  [[noreturn]] static void LogFatalAndAbort(amsr::core::StringView const ctx_id, CString file_name, LineNumber line_num,
                                            amsr::core::StringView const message) noexcept {
    Logger const logger{ctx_id, ""_sv};
    logger.LogFatalAndAbort(file_name, line_num, message);
  }

 private:
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Formats and logs the provided context information.
   * \param[in,out] stream Stream to write to
   * \param[in] file_name File name
   * \param[in] line_num Line number
   * \return The logging stream
   * \context ANY
   * \pre Logging is initialized
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static LoggerStream Log(LoggerStream stream, CString file_name, LineNumber line_num) noexcept {
    stream << file_name << " [" << static_cast<std::uint64_t>(line_num) << "]: ";
    return stream;
  }

  amsr::core::String ctx_id_;   /*!< Context ID, e.g. the name of the sub-component */
  amsr::core::String ctx_info_; /*!< Some Additional context information */
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_H_
