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
 *        \brief  Logger
 *      \details  Convenience wrapper around ara::log.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/array.h"
#include "amsr/core/string_view.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "vac/container/string_literals.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Type-alias for StringView.
 */
using StringView = ::amsr::core::StringView;

/*!
 * \brief Logger context id for IPC specific libraries.
 */
static constexpr StringView kIpcLoggerContextId{"vcip"_sv};

/*!
 * \brief Logger description for IPC specific libraries.
 */
static constexpr StringView kIpcLoggerContextDescription{"Logger for IpcBinding"_sv};

/*!
 * \brief   IpcBinding logging wrapper.
 * \details This is a wrapper for ara::log::Logger that behaves exactly the same except that it inserts a pre-defined
 *          prefix in front of the logging message.
 *
 * \unit IpcBinding::IpcBindingCore::IpcBindingLogging
 */
class AraComLogger final {
 public:
  /*!
   * \brief Type-alias for LogStream.
   */
  using LogStream = ::ara::log::LogStream;

  /*!
   * \brief Function type producing log outputs.
   *
   * \param[in,out] s  Log stream.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  using LogMessageFunction = vac::language::FunctionRef<void(LogStream&)>;

  /*!
   * \brief Location info for the origin of a log statement.
   */
  struct LogLocation {
    /*!
     * \brief Type alias for function name string.
     */
    using FunctionName = char const*;

    /*!
     * \brief Type alias for line number.
     */
    using LineNumber = std::uint32_t const;

    /*!
     * \brief Function name.
     */
    FunctionName function_name;

    /*!
     * \brief Line number.
     */
    LineNumber line_number;
  };

  /*!
   * \brief Max logged size of the custom prefix string passed to the ctor. Longer prefixes will be cropped.
   */
  static constexpr std::size_t kCustomPrefixMaxSize{80};

  // ---- Construction -------------------------------------------------------------------------------------------------

  /*!
   * \brief Construct a logger.
   *
   * \param[in] context_id           Context ID.
   * \param[in] context_description  Context description.
   * \param[in] prefix               The prefix which should be printed in front of the logging message.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  AraComLogger(StringView context_id, StringView context_description, StringView prefix) noexcept;

  /*!
   * \brief Destroy the logger.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~AraComLogger() noexcept = default;

  /*!
   * \brief Copy-construct the logger.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  AraComLogger(AraComLogger const&) noexcept = default;

  /*!
   * \brief Do not copy-assign the logger.
   */
  auto operator=(AraComLogger const&) noexcept -> AraComLogger& = delete;

  /*!
   * \brief Move-construct the logger.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  AraComLogger(AraComLogger&&) noexcept = default;

  /*!
   * \brief Do not move-assign the logger.
   */
  auto operator=(AraComLogger&&) noexcept -> AraComLogger& = delete;

  // ---- APIs ---------------------------------------------------------------------------------------------------------

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_IPCBINDING_AutosarC++17_10-A13.3.1_OverloadingWithForwardingReference
  /*!
   * \brief   Handle a violation: Log fatal error and abort further process execution.
   * \details Format of logged messages:
   *          - Fatal error log: "Violation: <static message><dynamic message>".
   *          - Abort: "<static message>".
   *
   * \param[in] static_log_message   Static message part.
   * \param[in] dynamic_log_message  Log lambda function creating dynamic message part.
   * \param[in] location             Origin of the violation.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  [[noreturn]] auto TerminateOnViolation(StringView static_log_message, LogMessageFunction dynamic_log_message,
                                         LogLocation const& location) const noexcept -> void;

  /*!
   * \brief   Handle a violation: Log fatal error and abort further process execution.
   * \details Format of logged messages:
   *          - Fatal error log: "<static message>".
   *          - Abort: "<static message>".
   *
   * \param[in] static_log_message  Static message part.
   * \param[in] location            Origin of the violation.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  [[noreturn]] void TerminateOnViolation(StringView static_log_message, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a fatal message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogFatal(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogFatal(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a error message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogError(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogError(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a warning message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogWarn(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogWarn(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a info message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogInfo(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogInfo(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a debug message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogDebug(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogDebug(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

  /*!
   * \brief   Logs a verbose message along with a prepended prefix, function name and line number.
   * \details Forwards the log message to ara::log::LogVerbose(), where the log arguments are evaluated only if the log
   *          level is sufficient.
   *
   * \param[in] log_msg   Function producing the actual log message. Output of this function will be logged right after
   *                      the assembled prefix.
   * \param[in] location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void LogVerbose(LogMessageFunction log_msg, LogLocation const& location) const noexcept;

 private:
  /*!
   * \brief   Size of the string buffer containing the logger prefix.
   * \details Contains:
   *          - 20 characters for process ID (uint64 value).
   *          - 4 characters for '[|] '.
   *          - N characters for custom prefix string.
   */
  static constexpr std::size_t kPrefixBufferSize{20 + 4 + kCustomPrefixMaxSize};

  /*!
   * \brief Constructs the logger prefix string.
   *
   * \param[in] prefix  Custom free-text prefix string.
   *
   * \return Number of valid characters in the prefix buffer.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto ConstructPrefix(StringView prefix) noexcept -> std::size_t;

  /*!
   * \brief Print the prefix string to the LogStream.
   *
   * \param[in,out] s         Target LogStream.
   * \param[in]     location  Origin of the log output.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void PrintPrefix(LogStream& s, LogLocation const& location) const noexcept;

  /*!
   * \brief The actual wrapped ara::log::Logger used for logging.
   */
  ara::log::Logger& logger_;

  /*!
   * \brief Buffer for the logger prefix string.
   */
  ::amsr::core::Array<char, kPrefixBufferSize> prefix_buffer_{};

  /*!
   * \brief Length of the prefix string.
   */
  std::size_t const prefix_length_;
};

}  // namespace logging
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_ARA_COM_LOGGER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
