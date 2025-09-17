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
/*!        \file  socal/internal/logging/ara_com_logger.h
 *        \brief  Helper class for logging that wraps the ara::log::Logger.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LOGGING_ARA_COM_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <sstream>
#include <string>

#include "amsr/core/string_view.h"
#include "amsr/loguti/stream.h"  // IWYU pragma: keep, required as every other unit in socal uses AraComLogger to access logutil
#include "ara/core/abort.h"
#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "osabstraction/process/process.h"
#include "vac/container/string_literals.h"
#include "vac/language/cpp17_backport.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace socal {
namespace internal {
namespace logging {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief Logger context id for ara::com libraries.
 */
static constexpr ::amsr::core::StringView kAraComLoggerContextId{"vcom"_sv};
/*!
 * \brief Logger description for ara::com libraries.
 */
static constexpr ::amsr::core::StringView kAraComLoggerContextDescription{"Logger for ara::com"_sv};
/*!
 * \brief Logger context id for Configuration.
 */
static constexpr ::amsr::core::StringView kConfigurationLoggerContextId{"vscf"_sv};
/*!
 * \brief Logger description for Configuration.
 */
static constexpr ::amsr::core::StringView kConfigurationLoggerContextDescription{"Logger for Configuration"_sv};
/*!
 * \brief Type alias for line numbers.
 */
using LineNumber = std::int32_t;

/*!
 * \brief Type alias for external log stream.
 */
using LogStream = ::ara::log::LogStream;

/*!
 * \brief AraCom Logging Wrapper. This is a wrapper for ara::log::Logger that behaves exactly the same except that it
 *        inserts a pre-defined prefix in front of the logging message.
 *
 * \unit Socal::AraComLogger
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class AraComLogger final {
 public:
  /*!
   * \brief StringStream type alias.
   */
  using StringStream = std::ostringstream;

  /*!
   * \brief StringStream Callback type alias.
   */
  using StringStreamCallback = ::vac::language::FunctionRef<void(StringStream&)>;

  /*!
   * \brief Constructs the logger with the given context id, description and prefix.
   * \param[in] context_id           Context ID.
   * \param[in] context_description  Context description.
   * \param[in] prefix               The prefix is printed in front of the logging message.
   * \pre     -
   * \context ANY
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit AraComLogger(::amsr::core::StringView const& context_id, ::amsr::core::StringView const& context_description,
                        std::string const& prefix) noexcept
      // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR
      : AraComLogger{context_id, context_description, ::amsr::core::StringView{prefix}} {}

  /*!
   * \brief Constructs the logger with the given context id, description and prefix (amsr::core::StringView overload).
   * \param[in] context_id           Context ID.
   * \param[in] context_description  Context description.
   * \param[in] prefix               The prefix is printed in front of the logging message.
   * \pre     -
   * \context ANY
   * \steady FALSE
   * \spec requires true; \endspec
   */

  explicit AraComLogger(::amsr::core::StringView const context_id, ::amsr::core::StringView const context_description,
                        ::amsr::core::StringView const prefix) noexcept
      // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR
      : logger_{::ara::log::CreateLogger(context_id, context_description)}, prefix_{ConstructPrefix(prefix)} {}

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function for ara::log::LogFatal() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogFatal of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogFatal(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogFatal([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function for logging with Log-level Fatal and abort.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] func_log_msg   Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady FALSE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogFatal of the logger with a callback that will.
   *   - Add a prefix to the message.
   *   - Invoke the given log message callable.
   * - Abort the process.
   * \endinternal
   */
  template <typename StringStreamCallback>
  void LogFatalAndAbort(StringStreamCallback func_log_msg, char const* function_name,
                        LineNumber const line) const noexcept {
    ara::core::String const line_number_str{std::to_string(line)};

    StringStream final_message{};      // VCA_SOCAL_CALLING_STL_APIS
    final_message << prefix_;          // VCA_SOCAL_CALLING_STL_APIS
    final_message << function_name;    // VCA_SOCAL_CALLING_STL_APIS
    final_message << ":";              // VCA_SOCAL_CALLING_STL_APIS
    final_message << line_number_str;  //
    final_message << ": ";             // VCA_SOCAL_CALLING_STL_APIS

    // VCA_SOCAL_CALLING_STL_APIS
    func_log_msg(final_message);

    // VCA_SOCAL_CALLING_STL_APIS
    ara::core::String final_msg_str{final_message.str()};

    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogFatal(
        [&final_msg_str](ara::log::LogStream& stream) { stream << ara::core::StringView{final_msg_str}; });

    ara::core::Abort(final_msg_str.c_str());
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function for ara::log::LogError() to put the prefix, function name and line in front of each log
   *          message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogError of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogError(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogError([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function for ara::log::LogWarn() to put the prefix, function name and line in front of each log
   *          message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogWarn of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogWarn(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogWarn([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  /*!
   * \brief   Wrapper function for ara::log::LogInfo() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogInfo of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogInfo(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogInfo([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  /*!
   * \brief   Wrapper function for ara::log::LogDebug() to put the prefix, function name and line in front.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Line number printed as prefix to ease navigation to the code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogDebug of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogDebug(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogDebug([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  /*!
   * \brief   Wrapper function for ara::log::LogVerbose() to put the prefix, function name and line in front of each
   *          log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \tparam    LogMessage     Functor for dumping the log message to LogStream.
   * \param[in] print_log_msg  Log message object that shall be printed right after the assembled prefix.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogVerbose of the logger with a callback that will.
   *   - Add a prefix to the message, and.
   *   - Invoke the given log message callable.
   * \endinternal
   */
  template <typename LogMessage>
  void LogVerbose(LogMessage&& print_log_msg, char const* function_name, LineNumber const line) const noexcept {
    static_assert(!vac::language::is_invocable_r<void, LogMessage, ::ara::log::LogStream const&>::value,
                  "Passing an empty lambda function is not allowed");
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogVerbose([function_name, line, &print_log_msg, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
      print_log_msg(s);
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function overload for ara::log::LogVerbose() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogVerbose of the logger with a callback that will.
   *   - Add a prefix to the message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogVerbose(char const* function_name, LineNumber const line) const noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogVerbose([function_name, line, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function overload for ara::log::LogDebug() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogDebug of the logger with a callback that will.
   *   - Add a prefix to the message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogDebug(char const* function_name, LineNumber const line) const noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogDebug([function_name, line, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
    });
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief   Wrapper function overload for ara::log::LogInfo() for logging without a custom log message.
   * \details Log arguments are evaluated only if log level is sufficient.
   * \param[in] function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in] line           Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires function_name != nullptr; \endspec
   * \internal
   * - Call LogInfo of the logger with a callback that will.
   *   - Add a prefix to the message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_Socal_AutosarC++17_10-A13.3.1_overload_forwarding_reference
  void LogInfo(char const* function_name, LineNumber const line) const noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    logger_.get().LogInfo([function_name, line, this](::ara::log::LogStream& s) {
      this->AddPrefix(s, function_name, line);  // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
    });
  }

  /*!
   * \brief   Get the logger prefix string.
   * \details The returned StringView is only valid until the lifetime of *this object.
   * \return Logger prefix string.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ara::core::StringView GetLoggerPrefix() const noexcept { return ara::core::StringView{prefix_}; }

 private:
  /*!
   * \brief   The OS process ID.
   * \details Stored as static member to improve logger instantiation performance. Determination of the process ID
   *          requires a significant runtime.
   */
  static osabstraction::process::ProcessId const process_id_;

  /*!
   * \brief Assemble logger prefix string.
   * \param[in] prefix  User prefix string.
   * \return Logger prefix string.
   * \pre     -
   * \context ANY
   * \steady FALSE
   * \internal
   * - Create an empty string and reserve size accommodating the whole prefix.
   * - Add the prefix' elements to the string.
   * - Return the constructed prefix string.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  static std::string const ConstructPrefix(::amsr::core::StringView prefix) noexcept {
    std::string const process_id_string{std::to_string(process_id_)};
    // Adding 4 for static part '[: ]' and 1 for null termination
    // VCA_SOCAL_CALLING_STL_APIS
    std::size_t const size{prefix.size() + process_id_string.size() + 5};
    // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR, VCA_SOCAL_CALLING_STL_APIS
    std::string constructed_prefix{};
    // VCA_SOCAL_UNDEFINED_EXTERNAL_LIBCPP_FUNCTION
    constructed_prefix.reserve(size);
    // VCA_SOCAL_CALLING_STL_APIS
    constructed_prefix += "[";
    // VCA_SOCAL_CALLING_STL_APIS
    constructed_prefix += process_id_string;
    if (!prefix.empty()) {
      // VCA_SOCAL_CALLING_STL_APIS
      constructed_prefix += ": ";
      // VCA_SOCAL_CALLING_STL_APIS
      constructed_prefix += prefix.ToString();
    }
    // VCA_SOCAL_CALLING_STL_APIS
    constructed_prefix += "]";
    return constructed_prefix;
  }

  // VECTOR NC VectorC++-V3.9.2: MD_SOCAL_VectorC++-V3.9.2_builtin_types_false_positive
  /*!
   * \brief Adds a prefix to a provided stream containing the function name and line.
   * \param[in,out] stream         An ara::log::Logstream on which the prefix will be added.
   * \param[in]     function_name  Printed as prefix to ease navigation to the particular code part.
   * \param[in]     line           Printed as prefix to ease navigation to the particular code part.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \internal
   * - Add the prefix, the function name and the line number with delimiters to the log stream.
   * \endinternal
   */
  void AddPrefix(::ara::log::LogStream& stream, char const* function_name, LineNumber const line) const noexcept {
    stream << prefix_ << function_name << ":" << line << ": ";
  }

  /*!
   * \brief The actual wrapped ara::log::Logger used for logging.
   */
  std::reference_wrapper<::ara::log::Logger> logger_;

  /*!
   * \brief The prefix printed before the log message.
   */
  std::string prefix_;
};

}  // namespace logging
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
