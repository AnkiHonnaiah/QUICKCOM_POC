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
/*!        \file  someip_daemon_client/include/someip_daemon_client/internal/logging/ara_com_logger.h
 *        \brief  Contains the component specific logger class
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_LOGGING_ARA_COM_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sstream>
#include "amsr/core/string_view.h"
#include "ara/core/abort.h"
#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "osabstraction/process/process_native_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {
namespace logging {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M7.3.6_using-directives_in_header_file
using vac::container::literals::operator""_sv;

/*!
 * \brief Contains the function name.
 * \vprivate This struct is component private.
 */
struct FunctionName {
  /*!
   * \brief Function name.
   */
  char const* value;
};

/*!
 * \brief Contains the line number.
 * \vprivate This enum class is component private.
 */
struct LineNumber {
  /*!
   * \brief line number.
   */
  std::int32_t const value;
};

/*!
 * \brief Logger context id for SOME/IP specific libraries
 */
static constexpr amsr::core::StringView kSomeIpLoggerContextId{"vcso"_sv};

/*!
 * \brief Logger description for SOME/IP specific libraries
 */
static constexpr amsr::core::StringView kSomeIpLoggerContextDescription{"Logger for SOME/IP libraries"_sv};

/*!
 * \brief AraCom Logging Wrapper.
 * \details This is a wrapper for ara::log::Logger that behaves exactly the same except that it inserts a pre-defined
 * prefix in front of the logging message.
 */
class AraComLogger final {  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
 public:
  /*!
   * \brief StringStream type alias.
   */
  using StringStream = std::ostringstream;

  /*!
   * \brief Constructor with amsr::core::StringView parameter list
   * \param[in] context_id Context ID
   * \param[in] context_description Context description
   * \param[in] prefix The prefix is printed in front of the logging message.
   * \pre -
   * \context Init
   * \reentrant False
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
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
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogFatal(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    this->logger_.LogFatal([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGGER_LOGLOCATION_VALID
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogError() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogError(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    this->logger_.LogError([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGGER_LOGLOCATION_VALID
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogWarn() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogWarn(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    this->logger_.LogWarn([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGGER_LOGLOCATION_VALID
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogInfo() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogInfo(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    // VCA_SDCL_CALLING_LOGGER_FUNCTION_IS_SAFE
    this->logger_.LogInfo([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGSTREAM_CONCATENATION_IS_SILENT
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogDebug() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Line number printed as prefix to ease navigation to the code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogDebug(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    this->logger_.LogDebug([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGGER_LOGLOCATION_VALID
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief Wrapper function for ara::log::LogVerbose() to put the prefix, function name and line in front of each log
   * message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam LogMessage Type of log message.
   * \param[in] print_log_msg Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogMessage>
  void LogVerbose(LogMessage&& print_log_msg, FunctionName function_name, LineNumber line) const noexcept {
    this->logger_.LogVerbose([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      // VCA_SDCL_LOGGER_LOGLOCATION_VALID
      s << this->prefix_ << function_name.value << ":" << line.value << ": ";
      print_log_msg(s);  // VCA_SDCL_VALID_PARAMETER_IN_LAMBDA_FUNCTION
    });
  }

  /*!
   * \brief   Writes a fatal log message and then abort the process
   * \details Log message is printed only if the log level is supported.
   *
   * \param[in] func_log_msg   Printing function to call.
   * \param[in] function_name  Name of the function creating the log entry (const char pointer). Cannot be nullptr.
   * \param[in] line_number    The line number of the log message.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   * \spec requires function_name != nullptr; \endspec
   */
  template <typename LogMessage>
  [[noreturn]] void LogFatalAndAbort(LogMessage&& func_log_msg, char const* function_name,
                                     std::uint32_t const line_number) const noexcept {
    ara::core::String const line_number_str{std::to_string(line_number)};

    StringStream message_stream{};                 // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << "[";                         // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << std::to_string(kProcessId);  // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << ":";                         // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << this->prefix_;               // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << ":";                         // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << function_name;               // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << ":";                         // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << line_number_str;             // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << "]";                         // VCA_SDCL_STRING_STREAM_USAGE
    message_stream << "[";                         // VCA_SDCL_STRING_STREAM_USAGE
    // VCA_SDCL_EXTERNAL_FUNCTION_CONTRACT
    func_log_msg(message_stream);
    message_stream << "]";  // VCA_SDCL_STRING_STREAM_USAGE

    logger_.LogFatal([&message_stream](ara::log::LogStream& stream) noexcept {
      // VCA_SDCL_STRING_STREAM_USAGE
      stream << ara::core::StringView{message_stream.str()};
    });

    // VCA_SDCL_STRING_STREAM_USAGE
    ara::core::Abort(message_stream.str().c_str());
  }

 private:
  /*!
   * \brief The OS process ID.
   * \details Stored as static member to improve logger instantiation performance. Determination of the process ID
   *          requires a significant runtime.
   */
  static osabstraction::process::ProcessId const kProcessId;

  /*!
   * \brief Assemble logger prefix string.
   * \param[in] prefix User prefix string
   * \return Logger prefix string.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto ConstructPrefix(amsr::core::StringView const prefix) noexcept -> ara::core::String {
    ara::core::String const process_id_string{std::to_string(kProcessId)};  // VCA_SDCL_VALID_STRING_COPY_CONSTRUCTOR
    // Adding 4 for static part '[: ]' and 1 for null termination
    std::size_t const size{prefix.size() + process_id_string.size() + 5};
    ara::core::String constructed_prefix{};
    constructed_prefix.reserve(size);         // VCA_SDCL_VALID_PARAMETER_TO_RESERVE
    constructed_prefix += "[";                // VCA_SDCL_VALID_STRING_CONCATENATION
    constructed_prefix += process_id_string;  // VCA_SDCL_VALID_STRING_CONCATENATION
    if (!prefix.empty()) {
      constructed_prefix += ": ";               // VCA_SDCL_VALID_STRING_CONCATENATION
      constructed_prefix += prefix.ToString();  // VCA_SDCL_VALID_STRING_CONCATENATION
    }
    constructed_prefix += "]";  // VCA_SDCL_VALID_STRING_CONCATENATION
    return constructed_prefix;
  }  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

  /*!
   * \brief The actual wrapped ara::log::Logger used for logging.
   */
  ara::log::Logger& logger_;

  /*!
   * \brief The prefix printed before the log message.
   */
  ara::core::String const prefix_;
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
