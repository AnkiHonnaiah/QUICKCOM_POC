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
/*!        \file
 *        \brief  Component specific wrapper class using the logging API (singleton).
 *        \unit   SomeIpBinding::SomeIpBindingCore::AraComLogger
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_SINGLETON_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_SINGLETON_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>
#include <string>
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "ara/core/abort.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "osabstraction/process/processinterface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Singleton logger.
 * \details Wrapper for logging API whose singleton instance is to be used for logging.
 */
class AraComLoggerSingleton final {
 public:
  /*!
   * \brief Get singleton instance of the logger.
   * \return A singleton instance.
   * \context ANY
   * \pre -
   * \reentrant  FALSE
   * \threadsafe TRUE
   * \steady     TRUE
   */
  static amsr::generic::Singleton<AraComLoggerSingleton>& GetInstance() noexcept {
    // VECTOR NC AutosarC++17_10-A3.3.2: MD_SOMEIPBINDING_AutosarC++17_10-A3.3.2_Container_static_objects
    static amsr::generic::Singleton<AraComLoggerSingleton> instance;
    return instance;
  }

  /*!
   * \brief Constructor.
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady    FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  AraComLoggerSingleton() noexcept
      : process_id_string_{std::to_string(osabstraction::process::GetProcessId())},
        logger_{ara::log::CreateLogger(kSomeIpLoggerContextId, kSomeIpLoggerContextDescription)} {}

  /*!
   * \brief Destructor.
   */
  ~AraComLoggerSingleton() noexcept = default;

  AraComLoggerSingleton(AraComLoggerSingleton const&) = delete;
  AraComLoggerSingleton(AraComLoggerSingleton&&) = delete;
  AraComLoggerSingleton& operator=(AraComLoggerSingleton const&) = delete;
  AraComLoggerSingleton& operator=(AraComLoggerSingleton&&) = delete;

  /*!
   * \brief Wrapper function for "ara::log::LogError()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogError(amsr::core::StringView const& custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                std::uint32_t const line_number) const noexcept {
    logger_.LogError([this, &custom_prefix, &function_name, &line_number,
                      &func_log_msg](ara::log::LogStream& log_stream) noexcept -> void {
      // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogWarn()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogWarn(amsr::core::StringView const& custom_prefix, LogMessage&& func_log_msg, char const* function_name,
               std::uint32_t const line_number) const noexcept {
    logger_.LogWarn([this, &custom_prefix, &function_name, &line_number,
                     &func_log_msg](ara::log::LogStream& log_stream) noexcept -> void {
      // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogInfo()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogInfo(amsr::core::StringView const& custom_prefix, LogMessage&& func_log_msg, char const* function_name,
               std::uint32_t const line_number) const noexcept {
    logger_.LogInfo([this, &custom_prefix, &function_name, &line_number,
                     &func_log_msg](ara::log::LogStream& log_stream) noexcept -> void {
      // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogDebug()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogDebug(amsr::core::StringView const& custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                std::uint32_t const line_number) const noexcept {
    logger_.LogDebug([this, &custom_prefix, &function_name, &line_number,
                      &func_log_msg](ara::log::LogStream& log_stream) noexcept -> void {
      // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogDebug()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  void LogDebug(amsr::core::StringView const& custom_prefix, char const* function_name,
                std::uint32_t const line_number) const noexcept {
    logger_.LogDebug(
        [this, &custom_prefix, &function_name, &line_number](ara::log::LogStream& log_stream) noexcept -> void {
          // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
          LogCommon(custom_prefix, function_name, line_number, log_stream);
        });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogVerbose()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Log printing function to call.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogVerbose(amsr::core::StringView const& custom_prefix, LogMessage&& func_log_msg, char const* function_name,
                  std::uint32_t const line_number) const noexcept {
    logger_.LogVerbose([this, &custom_prefix, &function_name, &line_number,
                        &func_log_msg](ara::log::LogStream& log_stream) noexcept -> void {
      // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
      LogCommon(custom_prefix, function_name, line_number, log_stream);
      func_log_msg(log_stream);
    });
  }

  /*!
   * \brief Wrapper function for "ara::log::LogVerbose()" to prepend the log message with prefix, function name
   *        and line number.
   * \details Log message is printed only if the log level is supported.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] function_name Function's name to prepend the log message with.
   * \param[in] line_number Line number to prepend the log message with.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  void LogVerbose(amsr::core::StringView const& custom_prefix, char const* function_name,
                  std::uint32_t const line_number) const noexcept {
    logger_.LogVerbose(
        [this, &custom_prefix, &function_name, &line_number](ara::log::LogStream& log_stream) noexcept -> void {
          // VCA_SOMEIPBINDING_ACCESSING_LOGGER_MEMBER_IN_LAMBDA
          LogCommon(custom_prefix, function_name, line_number, log_stream);
        });
  }

  /*!
   * \brief Writes a fatal log message and then abort the process
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] func_log_msg Printing function to call.
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \steady      FALSE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  [[noreturn]] void LogFatalAndAbort(ara::core::StringView const& custom_prefix, LogMessage&& func_log_msg,
                                     char const* function_name, std::uint32_t const line_number) noexcept {
    // clang-format off
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.2.1_outside_range_of_enumerator_string_stream_FP
    // clang-format on
    StringStream message_stream{};
    message_stream << "[";
    message_stream << process_id_string_;
    message_stream << ":";

    if (!custom_prefix.empty()) {
      message_stream << custom_prefix;
      message_stream << ":";
    }
    message_stream << function_name;
    message_stream << ":";
    message_stream << line_number;
    message_stream << "][";
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    func_log_msg(message_stream);
    message_stream << "]";

    logger_.LogFatal([&message_stream](ara::log::LogStream& stream) noexcept {
      stream << ara::core::StringView{message_stream.str()};
    });
    ara::core::Abort(message_stream.str().c_str());
  }

 private:
  /*!
   * \brief Log the common part of a log message.
   * \param[in] custom_prefix Custom prefix to use in the main prefix.
   * \param[in] function_name Function's name of the log message.
   * \param[in] line_number Line's number of the log message.
   * \param[in] log_stream Log stream to use for logging.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   */
  void LogCommon(amsr::core::StringView const& custom_prefix, char const* function_name,
                 std::uint32_t const line_number, ara::log::LogStream& log_stream) const noexcept {
    using vac::container::literals::operator""_sv;

    log_stream << "["_sv;

    log_stream << process_id_string_;
    log_stream << ":"_sv;

    if (!custom_prefix.empty()) {
      log_stream << custom_prefix;
      log_stream << ":"_sv;
    }

    log_stream << function_name;
    log_stream << ":"_sv;

    log_stream << line_number;

    log_stream << "] "_sv;
  }

  /*!
   * \brief OS process ID as a string for use in the prefix of logs.
   */
  amsr::core::String const process_id_string_;

  /*!
   * \brief Wrapped logger to access logging APIs.
   */
  ara::log::Logger& logger_;
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_SINGLETON_H_
