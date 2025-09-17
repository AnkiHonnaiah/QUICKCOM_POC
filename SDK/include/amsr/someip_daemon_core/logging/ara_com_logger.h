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
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_ARA_COM_LOGGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_ARA_COM_LOGGER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/core/abort.h"
#include "ara/core/string.h"

#include "ara/core/error_code.h"
#include "ara/core/string_view.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "osabstraction/process/process.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace logging {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
using vac::container::literals::operator""_sv;

/*!
 * \brief Logger context id for SOME/IP specific libraries
 */
static constexpr ara::core::StringView kSomeIpLoggerContextId{"vcso"_sv};

/*!
 * \brief Logger description for SOME/IP specific libraries
 */
static constexpr ara::core::StringView kSomeIpLoggerContextDescription{"Logger for SOME/IP libraries"_sv};

/*!
 * \brief Logger context id for SOME/IP Daemon POSIX (not specific component)
 */
static constexpr ara::core::StringView kSomeIpDLoggerContextId{"vsom"_sv};

/*!
 * \brief Logger description for SOME/IP Daemon POSIX (not specific component)
 */
static constexpr ara::core::StringView kSomeIpDLoggerContextDescription{"Logger for SOME/IP Daemon Posix"_sv};

/*!
 * \brief Logger context id for Application
 */
static constexpr ara::core::StringView kApplicationLoggerContextId{"vsap"_sv};

/*!
 * \brief Logger description for Application Manager
 */
static constexpr ara::core::StringView kApplicationLoggerContextDescription{"Logger for Application Manager"_sv};

/*!
 * \brief Logger description for Application Manager
 */
static constexpr ara::core::StringView kApplicationHandlerLoggerContextDescription{"Logger for Application"_sv};

/*!
 * \brief Logger context id for Configuration
 */
static constexpr ara::core::StringView kConfigurationLoggerContextId{"vscf"_sv};

/*!
 * \brief Logger description for Configuration
 */
static constexpr ara::core::StringView kConfigurationLoggerContextDescription{"Logger for Configuration"_sv};

/*!
 * \brief Logger context id for Packet Router
 */
static constexpr ara::core::StringView kPacketRouterLoggerContextId{"vspr"_sv};

/*!
 * \brief Logger description for Packet Router
 */
static constexpr ara::core::StringView kPacketRouterLoggerContextDescription{"Logger for Packet Router"_sv};

/*!
 * \brief Logger context id for ServiceDiscovery
 */
static constexpr ara::core::StringView kServiceDiscoveryLoggerContextId{"vssd"_sv};

/*!
 * \brief Logger description for ServiceDiscovery
 */
static constexpr ara::core::StringView kServiceDiscoveryLoggerContextDescription{"Logger for Service Discovery"_sv};

/*!
 * \brief Logger context id for RemoteClientManager
 */
static constexpr ara::core::StringView kRemoteClientManagerLoggerContextId{"vscm"_sv};

/*!
 * \brief Logger description for RemoteClientManager
 */
static constexpr ara::core::StringView kRemoteClientManagerLoggerContextDescription{
    "Logger for Remote Client Manager"_sv};

/*!
 * \brief Logger context id for RemoteServerManager
 */
static constexpr ara::core::StringView kRemoteServerManagerLoggerContextId{"vssm"_sv};

/*!
 * \brief Logger description for RemoteServerManager
 */
static constexpr ara::core::StringView kRemoteServerManagerLoggerContextDescription{
    "Logger for Remote Server Manager"_sv};

/*!
 * \brief Logger context id for RequiredServiceInstanceManager
 */
static constexpr ara::core::StringView kRequiredServiceInstanceManagerLoggerContextId{"vsrm"_sv};

/*!
 * \brief Logger description for RequiredServiceInstanceManager
 */
static constexpr ara::core::StringView kRequiredServiceInstanceManagerLoggerContextDescription{
    "Logger for Required Service Instance Manager"_sv};

/*!
 * \brief Logger context id for InterfaceCardMonitor
 */
static constexpr ara::core::StringView kNetworkLoggerContextId{"vsnm"_sv};

/*!
 * \brief Logger description for InterfaceCardMonitor
 */
static constexpr ara::core::StringView kNetworkLoggerContextDescription{"Logger for Interface Card Monitor"_sv};

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

 public:
  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context ANY
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~AraComLogger() noexcept = default;

  AraComLogger(AraComLogger const&) = delete;
  AraComLogger(AraComLogger&&) = delete;
  AraComLogger& operator=(AraComLogger const&) & = delete;
  AraComLogger& operator=(AraComLogger&&) & = delete;

  /*!
   * \brief Constructor with ara::core::StringView parameter list
   * \param[in] context_id Context ID
   * \param[in] context_description Context description
   * \param[in] prefix The prefix is printed in front of the logging message.
   * \pre -
   * \context   ANY
   * \reentrant False
   * \steady    FALSE
   */
  AraComLogger(ara::core::StringView const context_id, ara::core::StringView const context_description,
               ara::core::StringView const prefix)
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
   * \steady    TRUE
   */
  template <typename LogMessage>
  void LogFatal(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogFatal([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
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
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady    TRUE
   */
  template <typename LogMessage>
  void LogError(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogError([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
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
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady    TRUE
   */
  template <typename LogMessage>
  void LogWarn(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogWarn([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
      print_log_msg(s);
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
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogInfo(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogInfo([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
      print_log_msg(s);
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
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogDebug(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogDebug([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
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
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  template <typename LogMessage>
  void LogVerbose(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    this->logger_.LogVerbose([&function_name, &line, &print_log_msg, this](ara::log::LogStream& s) {
      s << this->prefix_ << function_name << ":" << line << ": ";
      print_log_msg(s);
    });
  }

  /*!
   * \brief Writes a fatal log message and then abort the process
   * \details Log message is printed only if the log level is supported.
   * \tparam LogMessage Data type of the callback for logging message.
   * \param[in] print_log_msg Printing function to call.
   * \param[in] file_name Name of the file where creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   *
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \steady      TRUE
   * \spec
   *   requires function_name != nullptr;
   * \endspec
   */
  template <typename LogMessage>
  [[noreturn]] void LogFatalAndAbort(LogMessage&& print_log_msg, ara::core::StringView const file_name,
                                     char const* function_name, LineNumber const line_number) const noexcept {
    ara::core::String final_message{};
    final_message.append(prefix_);
    final_message.append("[");
    final_message.append(file_name);
    final_message.append(":");
    final_message.append(function_name);
    final_message.append(":");
    final_message.append(std::to_string(line_number));
    final_message.append("]");
    final_message.append("[");
    print_log_msg(final_message);
    final_message.append("]");

    logger_.LogFatal([&final_message](ara::log::LogStream& stream) noexcept {
      // VECTOR NL AutosarC++17_10-M8.5.1: MD_SomeIpDaemon_AutosarC++17_10_M8.5.1_false_positive
      stream << ara::core::StringView{final_message};
    });

    ara::core::Abort(final_message.c_str());
  }
  /*!
   * \brief Check if current configured log will pass desired log level.
   * \param[in] log_level Log level to verify
   * \return True if desired log level satisfies the configured reporting level, otherwise False.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool IsLogEnabled(ara::log::LogLevel const log_level) const noexcept { return logger_.IsEnabled(log_level); };

  /*!
   * \brief           Log the supplied error code at "error" logging level.
   * \param[in]       error Error in "ara::core::Result" type.
   * \param[in]       logger  A logger to use for logging.
   * \param[in]       function_name Name of the function where the error originates from.
   * \param[in]       line_number Line number in the source of the log.
   * \param[in]       context_message A message to explain the context of the logged error.
   * \context         INIT
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \internal
   * - Log error code information at "error" level of logging.
   * \endinternal
   */
  static void LogErrorCodeAsError(ara::core::ErrorCode const& error,
                                  someip_daemon_core::logging::AraComLogger const& logger, char const* function_name,
                                  uint32_t line_number, ara::core::String context_message) noexcept {
    logger.LogError(
        [&context_message, &error](ara::log::LogStream& s) noexcept {
          s << "Error Value: ";
          s << error.Value();
          s << ", User message: ";
          s << error.UserMessage();
          s << " error encountered. Context: ";
          s << context_message;
          s << ", Support Data: ";
          s << error.SupportData();
        },
        function_name, static_cast<std::int32_t>(line_number));
  }

  /*!
   * \brief Wrapper function overload for ara::log::LogVerbose() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogVerbose(char const* function_name, LineNumber const line) const noexcept {
    logger_.LogVerbose([&function_name, &line, this](ara::log::LogStream& s) {
      s << prefix_ << function_name << ":" << line << ": ";
    });
  }

  /*!
   * \brief Wrapper function overload for ara::log::LogDebug() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogDebug(char const* function_name, LineNumber const line) const noexcept {
    logger_.LogDebug([&function_name, &line, this](ara::log::LogStream& s) {
      s << prefix_ << function_name << ":" << line << ": ";
    });
  }

  /*!
   * \brief Wrapper function overload for ara::log::LogInfo() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name Printed as prefix to ease navigation to the particular code part.
   * \param[in] line Printed as prefix to ease navigation to the particular code part.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_SomeIpDaemon_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogInfo(char const* function_name, LineNumber const line) const noexcept {
    logger_.LogInfo([&function_name, &line, this](ara::log::LogStream& s) {
      s << prefix_ << function_name << ":" << line << ": ";
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
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  static ara::core::String const ConstructPrefix(ara::core::StringView const prefix) noexcept {
    ara::core::String const process_id_string{std::to_string(process_id_)};
    // Adding 4 for static part '[: ]' and 1 for null termination
    std::size_t const size{prefix.size() + process_id_string.size() + 5};
    ara::core::String constructed_prefix{};
    constructed_prefix.reserve(size);
    constructed_prefix += "[";
    constructed_prefix += process_id_string;
    if (!prefix.empty()) {
      constructed_prefix += ": ";
      constructed_prefix += prefix.ToString();
    }
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
  ara::core::String const prefix_;
};

}  // namespace logging
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_ARA_COM_LOGGER_H_
