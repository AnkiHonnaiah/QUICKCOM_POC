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
/*!        \file  someip_binding_core/internal/logging/ara_com_logger.h
 *        \brief  Contains the component specific logger class
 *        \unit   SomeIpBinding::SomeIpBindingCore::AraComLogger
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "ara/core/abort.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "osabstraction/process/process.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Logger context id for SOME/IP specific libraries
 */
static amsr::core::StringView constexpr kSomeIpLoggerContextId{"vcso"_sv};

/*!
 * \brief Logger description for SOME/IP specific libraries
 */
static amsr::core::StringView constexpr kSomeIpLoggerContextDescription{"Logger for SOME/IP libraries"_sv};

/*!
 * \brief AraCom Logging Wrapper.
 * This is a wrapper for ara::log::Logger that behaves exactly the same except that it inserts a pre-defined prefix in
 * front of the logging message.
 */
class AraComLogger final {
  /*!
   * \brief The type for line numbers
   */
  using LineNumber = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_SOMEIPBINDING_AutosarC++17_10-A3.9.1_basic_type

 public:
  /*!
   * \brief Constructor with amsr::core::StringView parameterlist
   *
   * \param[in] context_id Context ID
   * \param[in] context_description Context description
   * \param[in] prefix The prefix is printed in front of the logging message.
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal steady FALSE \endinternal
   */
  AraComLogger(amsr::core::StringView const context_id, amsr::core::StringView const context_description,
               amsr::core::StringView const prefix)
      : logger_{ara::log::CreateLogger(context_id, context_description)}, prefix_{ConstructPrefix(prefix)} {}

  /*!
   * \brief Wrapper function for ara::log::LogFatal() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  void LogFatal(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogFatal([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogError() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  void LogError(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogError([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      print_log_msg(s);
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogWarn() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  void LogWarn(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogWarn([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogInfo() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  void LogInfo(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogInfo([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogDebug() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Line number printed as prefix to ease navigation to the code part.
   *
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
  void LogDebug(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogDebug([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogVerbose() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
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
  void LogVerbose(LogMessage&& print_log_msg, char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogVerbose([&function_name, line_number, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Writes a fatal log message and then abort the process
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] func_log_msg Printing function to call.
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   *
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
  [[noreturn]] void LogFatalAndAbort(LogMessage&& func_log_msg, char const* function_name,
                                     std::uint32_t const line_number) const noexcept {
    // clang-format off
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.2.1_outside_range_of_enumerator_string_stream_FP
    // clang-format on
    StringStream message_stream{};
    message_stream << prefix_;
    message_stream << "[";
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

  /*!
   * \brief Check if current configured log will pass desired log level.
   * \param[in] log_level Log level to verify
   * \return True if desired log level satisfies the configured reporting level, otherwise False.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsLogEnabled(ara::log::LogLevel const log_level) const noexcept { return logger_.IsEnabled(log_level); };

  /*!
   * \brief Get a reference to the encapsulated ara::log::Logger
   * \return A reference to the logger
   *
   *  \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR Next Line AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_returns_nonconst
  ara::log::Logger& GetLogger() noexcept { return logger_; }

  /*!
   * \brief Wrapper function overload for ara::log::LogVerbose() for logging prefix, function name and line without a
   * custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogVerbose(char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogVerbose([&function_name, line_number, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
    });
  }

  /*!
   * \brief Wrapper function overload for ara::log::LogDebug() for logging prefix, function name and line without a
   * custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line_number Printed as prefix to ease navigation to the particular code part.
   * \pre -
   * \context ANY
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SOMEIPBINDING_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogDebug(char const* function_name, LineNumber const line_number) const noexcept {
    this->logger_.LogDebug([&function_name, line_number, this](ara::log::LogStream& s) {
      // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
      s << this->prefix_ << function_name << ":" << line_number << ": ";
    });
  }

 private:
  /*!
   * \brief The OS process ID.
   * \details Stored as static member to improve logger instantiation performance. Determination of the process ID
   *          requires a significant runtime.
   */
  static osabstraction::process::ProcessId const process_id_;

  /*!
   * \brief Assemble logger prefix string.
   * \param[in] prefix User prefix string
   * \return Logger prefix string.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  static amsr::core::String const ConstructPrefix(amsr::core::StringView const prefix) noexcept {
    amsr::core::String const process_id_string{std::to_string(process_id_)};
    // Adding 4 for static part '[: ]' and 1 for null termination
    std::size_t const size{prefix.length() + process_id_string.size() + 5};
    amsr::core::String constructed_prefix{};
    constructed_prefix.reserve(size);
    // VCA_SOMEIPBINDING_STRING_INSUFFICIENT_SIZE
    constructed_prefix += "[";
    // VCA_SOMEIPBINDING_STRING_INSUFFICIENT_SIZE
    constructed_prefix += process_id_string;
    if (!prefix.empty()) {
      // VCA_SOMEIPBINDING_STRING_INSUFFICIENT_SIZE
      constructed_prefix += ": ";
      // VCA_SOMEIPBINDING_STRING_INSUFFICIENT_SIZE
      constructed_prefix += prefix;
    }
    // VCA_SOMEIPBINDING_STRING_INSUFFICIENT_SIZE
    constructed_prefix += "]";
    return constructed_prefix;
  }

  /*!
   * \brief The actual wrapped ara::log::Logger used for logging.
   */
  ara::log::Logger& logger_;

  /*!
   * \brief The prefix printed before the log message.
   */
  amsr::core::String const prefix_;
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
