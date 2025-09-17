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
 *         \brief  Provides a wrapper around a pre-configured ara logging component.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_ZERO_COPY_LOGGER_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_ZERO_COPY_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>
#include <ostream>
#include <type_traits>

#include "amsr/core/array.h"
#include "amsr/core/string_view.h"
#include "amsr/loguti/stream.h"
#include "amsr/zero_copy_binding/core/internal/logging/stream_operators.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "osabstraction/process/process.h"
#include "vac/container/string_literals.h"
#include "vac/language/function_ref.h"
#include "vac/language/location.h"
#include "vac/testing/test_adapter.h"

/*!
 * \brief               Macro for verbose log.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_VERBOSE(LOGGER, MESSAGE)                                                                              \
  (LOGGER).Log(::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger::LogLevel::kVerbose, (MESSAGE), \
               AMSR_FILE, __LINE__)

/*!
 * \brief               Macro for debug log.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_DEBUG(LOGGER, MESSAGE)                                                                              \
  (LOGGER).Log(::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger::LogLevel::kDebug, (MESSAGE), \
               AMSR_FILE, __LINE__)

/*!
 * \brief               Macro for info log.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_INFO(LOGGER, MESSAGE)                                                                              \
  (LOGGER).Log(::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger::LogLevel::kInfo, (MESSAGE), \
               AMSR_FILE, __LINE__)

/*!
 * \brief               Macro for warn log.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_WARN(LOGGER, MESSAGE)                                                                              \
  (LOGGER).Log(::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger::LogLevel::kWarn, (MESSAGE), \
               AMSR_FILE, __LINE__)

/*!
 * \brief               Macro for error log.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_ERROR(LOGGER, MESSAGE)                                                                              \
  (LOGGER).Log(::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger::LogLevel::kError, (MESSAGE), \
               AMSR_FILE, __LINE__)

/*!
 * \brief               Macro for fatal log and abort.
 * \param[in] LOGGER    The ZeroCopyLogger instance that will be used for logging.
 * \param[in] MESSAGE   The message passed to the log function.
 *                      Can be a string(view) or a callable returning an ::amsr::core::String.
 */
// VECTOR NC AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_macroForLogging
#define LOG_FATAL_AND_ABORT(LOGGER, MESSAGE) (LOGGER).LogFatalAndAbort((MESSAGE), AMSR_FILE, __LINE__)

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

namespace logging {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_ZEROCOPYBINDING_M7.3.6_usingDirective
using ::vac::container::literals::operator""_sv;

/*!
 * \brief   ZeroCopyBinding logging wrapper.
 * \details An easy-to-use wrapper of the ::ara::log::Logger with pre-defined prefix in front of the logging messages.
 *
 * \unit    ZeroCopyBinding::ZeroCopyLogger
 */
class ZeroCopyLogger final {
  /*!
   * \brief   The type of the line_number parameter.
   * \details Not unsigned to avoid integer conversion warnings when LogStream::WithLocation() is called.
   */
  using LineNumber = std::int32_t;

 public:
  /*!
   * \brief Type alias for logger LogLevel.
   */
  using LogLevel = ::ara::log::LogLevel;

  /*!
   * \brief Maximum allowed user prefix size.
   */
  static constexpr std::size_t kUserPrefixMaxSize{80U};

  /*!
   * \brief     Construct a ZeroCopyLogger.
   * \details   If the passed user prefix exceeds kUserPrefixMaxSize it will be cropped.
   * \param[in] user_prefix         The user prefix that will be printed in front of the logging message.
   * \param[in] context_id          The context ID.
   * \param[in] context_description The context description.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  explicit ZeroCopyLogger(::amsr::core::StringView user_prefix = ""_sv,
                          ::amsr::core::StringView context_id = kZeroCopyLoggerContextId,
                          ::amsr::core::StringView context_description = kZeroCopyLoggerContextDescription) noexcept;

  ZeroCopyLogger(ZeroCopyLogger const&) = delete;
  ZeroCopyLogger(ZeroCopyLogger&&) = delete;
  auto operator=(ZeroCopyLogger const&) -> ZeroCopyLogger& = delete;
  auto operator=(ZeroCopyLogger&&) -> ZeroCopyLogger& = delete;

  /*!
   * \brief Destructor, use default.
   */
  ~ZeroCopyLogger() noexcept = default;

  /*!
   * \brief       Logs a message along with a prepended prefix, file name and line number.
   * \details     If the log level is sufficient, stream the log message to ::ara::log::LogStream.
   * \param[in]   log_level     The desired log level.
   * \param[in]   log_message   A StringView object containing the actual log message.
   * \param[in]   file_name     The file name of the log output.
   * \param[in]   line_number   The line number of the log output.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Component private
   */
  void Log(LogLevel log_level, ::amsr::core::StringView log_message, ::amsr::core::StringView file_name,
           LineNumber line_number) const noexcept;

  /*!
   * \brief       Logs the message created by a given callable with a prepended prefix, file name and line number.
   * \details     If the log level is sufficient, stream the log message to ::ara::log::LogStream.
   * \param[in]   log_level       The desired log level.
   * \param[in]   log_message_fn  A callable accepting a logstream to write its own output to.
   *                              The callable shall not throw any exception and has to be silent.
   * \param[in]   file_name       The file name of the log output.
   * \param[in]   line_number     The line number of the log output.
   * \pre         The passed callable shall only modify the log stream object via operator<<().
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void Log(LogLevel log_level, ::vac::language::FunctionRef<void(::ara::log::LogStream&)> log_message_fn,
           ::amsr::core::StringView file_name, LineNumber line_number) const noexcept;

  /*!
   * \brief       Logs a fatal message along with a prepended prefix, file name and line number and aborts execution.
   * \details     If the log level is sufficient, stream the log message to ::ara::log::LogStream.
   * \param[in]   log_message   A StringView object containing the actual log message.
   * \param[in]   file_name     The file name of the log output.
   * \param[in]   line_number   The line number of the log output.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Component private
   */
  [[noreturn]] void LogFatalAndAbort(::amsr::core::StringView log_message, ::amsr::core::StringView file_name,
                                     LineNumber line_number) const noexcept;

  /*!
   * \brief       Logs a fatal message along with a prepended prefix, file name and line number and aborts execution.
   * \details     If the log level is sufficient, stream the log message to a StringStream.
   * \param[in]   log_message_fn  A callable accepting a ostringstream to write its own output to.
   *                              The callable shall not throw any exception and has to be silent.
   * \param[in]   file_name       The file name of the log output.
   * \param[in]   line_number     The line number of the log output.
   * \pre         The passed callable shall only modify the ostringstream object via operator<<().
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  [[noreturn]] void LogFatalAndAbort(::vac::language::FunctionRef<void(std::ostream&)> log_message_fn,
                                     ::amsr::core::StringView file_name, LineNumber line_number) const noexcept;

 private:
  /*!
   * \brief The wrapped ::ara::log::Logger used for logging.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief The logger context ID for ZeroCopyBinding specific libraries.
   */
  static constexpr ::amsr::core::StringView kZeroCopyLoggerContextId{"vczc"_sv};

  /*!
   * \brief The logger description for ZeroCopyBinding specific libraries.
   */
  static constexpr ::amsr::core::StringView kZeroCopyLoggerContextDescription{
      "Logger for ZeroCopyBinding libraries"_sv};

  /*!
   * \brief   The OS process ID.
   * \details Process ID stored as static member to improve logger instantiation performance.
   *          Determination of the process ID requires a significant amount of runtime.
   */
  static ::osabstraction::process::ProcessId const kProcessId;

  /*!
   * \brief Maximal amount of digits of the used integer type of the process id in decimal system.
   */
  static constexpr std::size_t kMaxDigitsProcessId{
      std::numeric_limits<std::enable_if_t<std::numeric_limits<::osabstraction::process::ProcessId>::is_integer,
                                           ::osabstraction::process::ProcessId>>::digits10 +
      1U};

  /*!
   * \brief Size of the string buffer containing the process ID and the user prefix.
   */
  static constexpr std::size_t kPrefixBufferSize{kMaxDigitsProcessId + "[: ] "_sv.length() + kUserPrefixMaxSize};

  /*!
   * \brief The buffer where the prefix is constructed.
   */
  ::amsr::core::Array<char, kPrefixBufferSize> prefix_buffer_{};

  /*!
   * \brief The prefix printed before the log message.
   */
  ::amsr::core::StringView prefix_{};

  /*!
   * \brief     Constructs the logger prefix string by prepending the process ID.
   * \details   If the passed user prefix exceeds kUserPrefixMaxSize it will be cropped.
   * \param[in] user_prefix User prefix string.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  void ConstructPrefix(::amsr::core::StringView user_prefix) noexcept;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ZeroCopyLogger, DefaultConstructor);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ZeroCopyLogger, LogLambdaLogLevelNotEnabled);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ZeroCopyLogger, LogStreamingLambdaLogLevelNotEnabled);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ZeroCopyLogger, LogStringViewLogLevelNotEnabled);
};

}  // namespace logging

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_ZERO_COPY_LOGGER_H_
