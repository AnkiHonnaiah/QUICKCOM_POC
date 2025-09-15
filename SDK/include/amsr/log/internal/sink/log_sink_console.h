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
 *        \brief  Provides an implementation for logging into the terminal.
 *      \details
 *
 *         \unit  LogAndTrace::LogSinks::LogSinkConsole
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_CONSOLE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_CONSOLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/iostream/filestream/std_output_stream.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/borrowed_reactor.h"
#include "amsr/log/internal/config/common_compile_time_config.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/sync/primitive.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "amsr/log_fwd.h"                       // IWYU pragma: export
#include "amsr/timer/timer.h"

namespace amsr {
namespace log {
namespace internal {
namespace sink {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC AutosarC++17_10-A10.1.1: MD_LOG_AutosarC++17_10-A10.1.1_external_API
// VECTOR NC Compiler-#611: MD_LOG_Compiler-#611_no_total_override_needed_for_impl
/*!
 * \brief Console stream sink of logging.
 *
 * \details Console sink prints log messages on console
 *
 * \trace DSGN-LogAndTrace-Sinks
 *
 * \vprivate Component private.
 */
class LogSinkConsole final : public ILogSink, public timer::Timer {
  /*! \brief The guard used to synchronize the pool access. */
  using Guard = sync::Primitive<config::kConsoleSyncStrategy>;

  // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_false_positive_braces
  /*! \brief Writer for logging to standard out. */
  ::amsr::stream::filestream::StdOutputStream std_out_{};
  /*! \brief Guard to synchronize flushing of log message. */
  Guard flush_guard_{};
  /*! \brief The log level that forces flushing the message directly. */
  LogLevel flush_threshold_{LogLevel::kDebug};

 public:
  /*!
   * \brief Constructs a new Log Sink Console object using a reactor.
   *
   * \param reactor The reactor to handle all async operations.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LogSinkConsole(internal::BorrowedReactor reactor) noexcept;
  /*! \brief Deleted move constructor. */
  LogSinkConsole(LogSinkConsole&&) noexcept = delete;
  /*! \brief Deleted copy constructor. */
  LogSinkConsole(LogSinkConsole const&) noexcept = delete;
  /*! \brief Deleted move assignment. */
  auto operator=(LogSinkConsole&&) & noexcept -> LogSinkConsole& = delete;
  /*! \brief Deleted copy assignment. */
  auto operator=(LogSinkConsole const&) & noexcept -> LogSinkConsole& = delete;
  /*!
   * \brief Destroys the Log Sink Console object.
   * \spec
   *   requires true;
   * \endspec
   */
  ~LogSinkConsole() noexcept final;

  /*!
   * \copydoc amsr::log::internal::sink::ILogSink::OnInitializeComponent()
   * \trace DSGN-LogAndTrace-FlushConfiguration
   */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnLog() */
  auto OnLog(MessageMetaData meta_data, Message message) noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::Name() */
  auto Name() const noexcept -> core::StringView final { return kConsoleSink; }

 private:
  /*!
   * \brief Handle the timer callback.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \return      True in case the sink is enabled, false if not.
   */
  auto HandleTimer() noexcept -> bool final;
};
}  // namespace sink
}  // namespace internal
}  // namespace log
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::log::internal::sink::LogSinkConsole::flush_guard_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::sink::LogSinkConsole::std_out_
 * \usedin          ::amsr::log::internal::sink::LogSinkConsole::Log
 *                  ::amsr::log::internal::sink::LogSinkConsole::HandleTimer
 * \exclude         All other functions of LogSinkConsole
 * \length          LONG
 *                  Lock active during all write access to the stdoutput.
 *                  Locking time depends on the length of the message & the output device.
 *                  Using e.g. TTY via SSH can even block other applications.
 * \endexclusivearea
 */

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_CONSOLE_H_
