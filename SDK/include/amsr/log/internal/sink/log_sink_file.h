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
 *        \brief  Provides an implementation for logging into files.
 *      \details
 *
 *         \unit  LogAndTrace::LogSinks::LogSinkFile
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_FILE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_FILE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/iostream/filestream/output_file_stream.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/borrowed_reactor.h"
#include "amsr/log/internal/config/common_compile_time_config.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/sync/primitive.h"
#include "amsr/log/internal/types/types_fwd.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "amsr/log/log_level.h"
#include "amsr/log_fwd.h"
#include "amsr/timer/timer.h"

namespace amsr {
namespace log {
namespace internal {
namespace sink {
// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC AutosarC++17_10-A10.1.1: MD_LOG_AutosarC++17_10-A10.1.1_external_API
// VECTOR NC Compiler-#611: MD_LOG_Compiler-#611_no_total_override_needed_for_impl
/*!
 * \brief Class to prepare logging into file.
 *
 * \details Preparation to stream into file by creating writer instance and log file.
 *
 * \trace DSGN-LogAndTrace-Sinks
 *
 * \vprivate Component private.
 */
class LogSinkFile final : public ILogSink, public timer::Timer {
  /*! \brief The guard used to synchronize the pool access. */
  using Guard = sync::Primitive<config::kFileSyncStrategy>;

  // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_false_positive_braces
  /*! \brief Writer for logging to a file. */
  ::amsr::stream::filestream::OutputFileStream file_{};
  /*! \brief Guard to synchronize flushing of log message. */
  Guard flush_guard_{};
  /*! \brief The log level that forces flushing the message directly. */
  LogLevel flush_threshold_{LogLevel::kDebug};

 public:
  /*!
   * \brief Constructs a new Log Sink File object.
   *
   * \param reactor The reactor to handle all async operations.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LogSinkFile(internal::BorrowedReactor reactor) noexcept;
  /*! \brief Deleted move constructor. */
  LogSinkFile(LogSinkFile&&) noexcept = delete;
  /*! \brief Deleted copy constructor. */
  LogSinkFile(LogSinkFile const&) noexcept = delete;
  /*! \brief Deleted move assignment. */
  auto operator=(LogSinkFile&&) & noexcept -> LogSinkFile& = delete;
  /*! \brief Deleted copy assignment. */
  auto operator=(LogSinkFile const&) & noexcept -> LogSinkFile& = delete;
  /*!
   * \brief Destroys the Log Sink File object.
   * \spec
   *   requires true;
   * \endspec
   */
  ~LogSinkFile() noexcept final;

  /*!
   * \copydoc amsr::log::internal::sink::ILogSink::OnInitializeComponent()
   * \trace DSGN-LogAndTrace-FlushConfiguration
   */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnLog() */
  auto OnLog(MessageMetaData meta_data, Message message) noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::Name() */
  auto Name() const noexcept -> core::StringView final { return kFileSink; }

 private:
  /*!
   * \brief Validates the configuration for the file sink.
   *
   * \param cfg The startup configuration for the logging framework.
   * \return    True in case file sink is active. Returns an error if the config is invalid.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto ValidateConfig(config::LoggingConfig const& cfg) const noexcept -> core::Result<bool>;

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
 * \exclusivearea   ::amsr::log::internal::sink::LogSinkFile::flush_guard_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::sink::LogSinkFile::file_
 * \usedin          ::amsr::log::internal::sink::LogSinkFile::Log
 *                  ::amsr::log::internal::sink::LogSinkFile::HandleTimer
 * \exclude         All other functions of LogSinkFile
 * \length          LONG
 *                  Lock active during all write access to the file.
 *                  Locking time depends on the length of the message & the file.
 *                  Using a nonregular file that blocks can even block other applications.
 * \endexclusivearea
 */

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_FILE_H_
