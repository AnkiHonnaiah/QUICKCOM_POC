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
/**        \file
 *        \brief  Provides an implementation for logging via the log daemon.
 *      \details
 *
 *         \unit  LogAndTrace::LogSinks::LogSinkDaemon
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_DAEMON_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_DAEMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/borrowed_reactor.h"
#include "amsr/log/internal/config/common_compile_time_config.h"
#include "amsr/log/internal/config/compile_time_config.h"
#include "amsr/log/internal/config/sink_config.h"
#include "amsr/log/internal/ipc_client.h"
#include "amsr/log/internal/mpsc/mpsc.h"
#include "amsr/log/internal/overflow_handler.h"
#include "amsr/log/internal/reactor_fwd.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace log {
/*! \brief Forward declaration of the logging config. */
namespace internal {
namespace sink {
/*! \brief Using for Producer. */
using Producer = mpsc::Producer<IpcClient::Data, config::kRemoteSyncStrategy>;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
// VECTOR NC Compiler-#611: MD_LOG_Compiler-#611_no_total_override_needed_for_impl
/*!
 * \brief Class for logging to daemon.
 *
 * \trace DSGN-LogAndTrace-Sinks
 *
 * \vprivate Component private.
 */
// VCA_LOGAPI_MOLE_1298
class LogSinkDaemon final : public ILogSink {
  /*! \brief Ipc reactor. */
  BorrowedReactor reactor_;
  // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_false_positive_braces
  /*! \brief Producer to store incoming log messages into the queue. */
  Optional<Producer> data_producer_{};
  /*! \brief Producer to store incoming control messages into the queue. */
  Optional<Producer> ctrl_producer_{};
  /*! \brief Handles message queue overflows. */
  OverflowHandler overflow_handler_{};
  /*! \brief Flag to indicate if initialization has completed. */
  bool is_initialized_{false};

 public:
  /*!
   * \brief Constructor from a reactor.
   * \param reactor Takes the reactor as input.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  explicit LogSinkDaemon(BorrowedReactor reactor) noexcept;

  /*! \brief Explicitly delete the default constructor. */
  LogSinkDaemon() = delete;

  /*! \brief Inherit constructors. */
  using ILogSink::ILogSink;
  /*! \brief Inherit assignment operator. */
  using ILogSink::operator=;

  auto OnCreateLogger(core::String const ctx_id, core::String const ctx_desc) noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnInitializeComponent() */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> final;

  auto OnDeinitializeComponent() noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnLog() */
  auto OnLog(MessageMetaData meta_data, Message message) noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::Name() */
  auto Name() const noexcept -> core::StringView final { return kRemoteSink; }

 private:
  /*!
   * \brief Validates the configuration for the daemon sink.
   *
   * \param cfg      The startup configuration for the logging framework.
   * \param sink_cfg The sink configuration for the remote sink.
   * \return         An error if the config is invalid.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  static auto ValidateConfig(config::LoggingConfig const& cfg, config::SinkConfig const& sink_cfg) noexcept
      -> core::Result<void>;

  /*!
   * \brief Register a new application.
   *
   * \param logger_base The application's LoggerBase.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  void RegisterNewApplication(core::String app_id, core::String app_desc) noexcept;
};

/*!
 * \brief Send a message to the daemon.
 *
 * \param producer The queue to write to.
 * \param ll       Contains the default log level.
 * \param app_id   Contains the app 'id' as a string.
 * \param ctx_id   Contains the context 'id' as a string.
 * \param message  The message.
 * \spec
 *   requires true;
 * \endspec
 */
void SendMessage(
    Producer& producer, LogLevel ll, core::String&& app_id, core::String&& ctx_id, core::StringView message,
    vac::language::FunctionRef<void(void)> overflow =
        // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_M0.1.8_EmptyCallback
    []() {}) noexcept;

/*!
 * \brief Register a application by the daemon.
 * \param producer    The queue to write to.
 * \param id          Contains the app 'id' as a string.
 * \param description Contains the application description.
 * \spec
 *   requires true;
 * \endspec
 */
void RegisterApplication(Producer& producer, core::String&& id, core::String&& description) noexcept;

/*!
 * \brief Register a context by the daemon.
 * \param producer    The queue to write to.
 * \param app_id      Contains the app 'id' as a string.
 * \param ctx_id      Contains the context 'id' as a string.
 * \param description Contains the context description.
 * \spec
 *   requires true;
 * \endspec
 */
void RegisterContext(Producer& producer, core::String&& app_id, core::String&& ctx_id,
                     core::String&& description) noexcept;
}  // namespace sink
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_DAEMON_H_
