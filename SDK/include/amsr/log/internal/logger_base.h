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
/*!        \file  logger_base.h
 *        \brief  The LoggerBase class and free functions contained in this file
 *                are used as a centralized handler for active Loggers, Sinks and
 *                the LoggingConfig used during the lifetime of an application.
 *
 *         \unit  LogAndTrace::Logging::LoggerBase
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOGGER_BASE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOGGER_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <unordered_map>
#include "amsr/core/string.h"
#include "amsr/log/i_log_sink_base.h"
#include "amsr/log/internal/array_vec.h"
#include "amsr/log/internal/config/compile_time_config.h"
#include "amsr/log/internal/config/log_sink_register.h"
#include "amsr/log/internal/config/logging_config.h"
#include "amsr/log/internal/ipc_client.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/reactor_fwd.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/sync/primitive.h"
#include "amsr/log/internal/sync/strategy.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "amsr/log/internal/utils.h"
#include "amsr/log/log_types.h"
#include "amsr/log/logger.h"
#include "amsr/trace/tracing_config.h"

namespace amsr {
namespace trace {
/*!
 * \brief Forward declaration of test fixture declared friend in LoggerBase.
 */
class TraceFixture;
}  // namespace trace
namespace log {
namespace internal {
/*!
 * \brief Returns a reference to the static owned reactor.
 * \spec
 *   requires true;
 * \endspec
 */
auto GetOwnedReactor() noexcept -> Box<Reactor>&;

/*!
 * \brief Gets the state of logging.
 *
 * \vprivate Component private
 * \spec
 *   requires true;
 * \endspec
 */
auto IsInitialized() noexcept -> bool;

// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric-OO.WMC.One_external_API
/*!
 * \brief Singleton managing all loggers & sinks.
 */
class LoggerBase final {
  /*! \brief The guard used to synchronize the pool access. */
  using Guard = sync::Primitive<sync::Strategy::kMutex>;

 public:
  /*! \brief Map type for storing logger instances. */
  using LoggerMap = Map<core::String, LoggerContainer>;

 private:
  /*! \brief Guard for thread synchronization. */
  Guard base_logger_guard_{};
  /*! \brief Default log Level used when no specific context log level was set.*/
  std::atomic<LogLevel> default_log_level_{LogLevel::kOff};
  /*!
   * \brief Configuration that may be modified
   * \details Once reverting to file config is needed we need a copy or reread.
   */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_LOG_AutosarC++17_10-M8.5.2_false_positive_braces
  config::LoggingConfig config_{};
  /*! \brief Storage container for logger instances. */
  LoggerMap logger_storage_{};
  /*! \brief Registered log sinks. */
  config::Sinks registered_sinks_;
  /*! \brief Shows if the logger has been initialized successfully. */
  bool is_initialized_{false};
  /*! \brief IPC client. */
  Optional<IpcClient> client_{};
  /*! \brief Tracing Config. */
  amsr::trace::config::TracingConfig tracing_config{};

 public:
  /*!
   * \brief Default constructor.
   *
   * \param sinks The already initialized sinks.
   *
   * \details Initializes a global (for the whole application) logger instance without a specific context.
   *
   * \vprivate Component private
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LoggerBase(config::Sinks&& sinks) noexcept;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Friend declaration for testing.
   * \spec
   *   requires true;
   * \endspec
   */
  friend class amsr::trace::TraceFixture;
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Forward declaration for initialization.
   * \spec
   *   requires true;
   * \endspec
   */
  friend auto InitializeComponent(BorrowedReactor reactor, core::StringView) noexcept -> Result<void>;
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Forward declaration for initialization.
   * \spec
   *   requires true;
   * \endspec
   */
  friend auto InitializeComponent(BorrowedReactor reactor, config::LoggingConfig&&) noexcept -> Result<void>;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Forward declaration for deinitialization.
   * \spec
   *   requires true;
   * \endspec
   */
  friend auto DeinitializeComponent() noexcept -> Result<void>;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Global function as friend
   *
   * \param log_sink Sink to register
   * \return         The empty Result, or the error that occurred
   * \spec
   *   requires true;
   * \endspec
   */
  friend auto RegisterNewLogSink(ILogSinkBase& log_sink) noexcept -> Result<void>;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief Global function as friend
   *
   * \param log_sink Pointer to log sink to unregister
   * \return         The empty Result, or the error that occurred
   * \spec
   *   requires true;
   * \endspec
   */
  friend auto UnregisterLogSink(ILogSinkBase const& log_sink) noexcept -> Result<void>;

  /*! \brief Delete copy Constructor. */
  LoggerBase(LoggerBase const&) = delete;
  /*! \brief Delete copy assignment operator. */
  auto operator=(LoggerBase const&) -> LoggerBase& = delete;
  /*! \brief Delete move Constructor. */
  LoggerBase(LoggerBase&&) = delete;
  /*! \brief Delete move assignment operator. */
  auto operator=(LoggerBase&&) -> LoggerBase& = delete;
  /*!
   * \brief Use default Destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~LoggerBase() noexcept = default;

  /*!
   * \brief Return singleton instance of the logger
   *
   * \returns singleton instance of the logger
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Get() noexcept -> LoggerBase&;

  /*!
   * \brief Sets the default log level for a logger with the given context id.
   *
   * \param ctx_id    context ID
   * \param log_level new log level
   *
   * \details An empty context id indicates that all contexts on the application shall be updated, aligned with
   *          [PRS_Dlt_00194].
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLogLevelForContext(core::StringView ctx_id, LogLevel log_level) noexcept;

  /*!
   * \brief Sets the log level for a logger with the given context id and log mode.
   *
   * \param ctx_id    context ID
   * \param log_level new log level
   * \param log_mode  new log mode
   *
   * \return          The empty Result, or the error that occurred
   *
   * \details An empty context id indicates that all contexts on the application shall be updated, aligned with
   *          [PRS_Dlt_00194].
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto SetLogLevelForMode(core::StringView ctx_id, LogLevel log_level, core::StringView log_mode) noexcept
      -> Result<void>;

  /*!
   * \brief Determines if the provided log sink is currently available.
   *
   * \param sink The log sink that is to be checked.
   * \return     True if enabled, else false.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsLogSinkEnabled(core::StringView sink) const noexcept -> bool;

  /*!
   * \brief Member function to log message.
   *
   * \param meta_data Meta data for the function.
   * \param message   Log message.
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  void Log(MetaData meta_data, Message message) noexcept;

  /*!
   * \brief Get constant configuration structure
   *
   * \return Constant configuration structure
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetConfig() const noexcept -> config::LoggingConfig const& { return this->config_; }

  /*!
   * \brief Search for the context ID for the logger instance
   * if not found it will insert it's Context ID
   * If found it will return with out duplication
   *
   * Note: The call of create logger context shall be done before
   * logging message to classify the messages inside the logging stream
   *
   * \param ctx_id          The context ID :shall be unique
   * \param ctx_description Context description
   * \return                Logger reference
   *
   * \vprivate Component internal.
   */
  auto CreateLogContext(core::StringView ctx_id, core::StringView ctx_description) noexcept -> Logger&;

  /*!
   * \brief Executes the given callable on the list of registered log contexts.
   *
   * \param fn Callable to execute.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details The callable must not call any function that accesses the LogContexts again. Doing so may result in a
   *          deadlock.
   *
   * \vprivate Component internal.
   */
  void InspectLogContexts(FunctionRef<void(Logger const&)> fn) noexcept;

  /*!
   * \brief Check if the logger has been initialized successfully.
   *
   * \return True if the init process has been sucessful.
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInitialized() const noexcept -> bool;

  /*!
   * \brief Get the IPC client.
   *
   * \return Reference to the optional holding the IPC client.
   *
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetIpcClient() noexcept -> Optional<IpcClient>& { return this->client_; }

  /*!
   * \brief   Sets the enabled trace contexts to the provided vector.
   * \pre     Can only be used before TracingConfig is Initialized.
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetEnabledTraceContexts(trace::config::TracingConfig::ContextVector&& ctxs) noexcept {
    // VCA_LOGAPI_READONLY_FUNCTION
    this->tracing_config.SetEnabledContexts(std::move(ctxs));
  }

  /*!
   * \brief  Returns vector with the enabled trace contexts.
   * \return Vector with the enabled trace contexts.
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetEnabledTraceContexts() const noexcept -> trace::config::TracingConfig::ContextVector const& {
    return this->tracing_config.GetEnabledContexts();
  }

  /*!
   * \brief   Returns vector with active trace sinks.
   * \return  Vector with custom trace sinks.
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetEnabledTraceSinks() const noexcept -> trace::config::TracingConfig::TraceSinkVector const& {
    return this->tracing_config.GetTraceSinks();
  }

  /*!
   * \brief   Add custom trace sink.
   * \pre     Can only be used before TracingConfig is Initialized.
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  void AddTraceSink(std::unique_ptr<trace::TraceSink> trace_sink) noexcept {
    // VCA_LOGAPI_READONLY_FUNCTION
    this->tracing_config.AddTraceSink(std::move(trace_sink));
  }

  /*!
   * \brief   Checks if time point acquisistion is enabled.
   * \return  Boolean of whether time point acquisition is enabled.
   * \details
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsTraceTimePointAcquisitionEnabled() const noexcept -> bool {
    return this->tracing_config.IsTimePointAcquisitionEnabled();
  }

  /*!
   * \brief   Enables or disables trace time point acquisition.
   * \param   new_value boolean which enables or disables time point acquisition.
   * \details Can only be used before TracingConfig is Initialized.
   * \vprivate Component internal.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTraceTimePointAcquisition(bool new_value) noexcept {
    // VCA_LOGAPI_READONLY_FUNCTION
    this->tracing_config.SetTimePointAcquisition(new_value);
  }

 private:
  /*!
   * \brief Initializes the loggerbase singleton
   *
   * \param reactor The passed reactor
   * \return        The singleton instance of the logger
   * \spec
   *   requires true;
   * \endspec
   */
  static auto InitializeSingleton(BorrowedReactor reactor) noexcept -> Result<LoggerBase&>;

  /*!
   * \brief Deinitializes the 'LoggerBase' singleton
   *
   * \return Nothing on success
   *
   * \error{LogErrorDomain, LogErrc::kNotInitialized, In case the singleton is not initialized.}
   * \spec
   *   requires true;
   * \endspec
   */
  static auto DeinitializeSingleton() noexcept -> Result<void>;

  /*!
   * \brief Resets the log levels back to the settings read from the config file.
   * \spec
   *   requires true;
   * \endspec
   */
  void ResetConfigFromFile() noexcept;

  /*!
   * \brief Sets the timestamp clock type from the configuration.
   *
   * \trace DSGN-LogAndTrace-ConfigurableTimestampClock
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetClockTypeFromConfig() const noexcept;

  /*!
   * \brief Register log sink for logging messages
   * \details Register should be usually used at single threaded initialization phase. Therefore method is
   *          not thread safe.
   *
   * \param log_sink Sink to register
   * \return         The positive result if successful, otherwise an error.
   * \spec
   *   requires true;
   * \endspec
   *
   * \threadsafe FALSE
   */
  auto RegisterLogSink(ILogSinkBase& log_sink) noexcept -> Result<void>;

  /*!
   * \brief Unregister log sink for logging messages
   * \details Unregister should be usually used at single threaded shutdown phase. Therefore method is not thread safe.
   *
   * \param log_sink Pointer to log sink to deregister
   * \return         The positive result if the sink was found and unregistered, otherwise an error.
   * \spec
   *   requires true;
   * \endspec
   *
   * \threadsafe FALSE
   */
  auto UnregisterLogSink(ILogSinkBase const& log_sink) noexcept -> Result<void>;

  /*!
   * \brief Creates and registers a new Logger instance in the logger storage.
   *
   * \param ctx_id          The context ID :shall be unique
   * \param ctx_description Context description
   * \param logger_base     Reference to base logger
   * \return                Logger reference to the newly instantiated logger
   * \spec
   *   requires true;
   * \endspec
   */
  auto RegisterNewLogger(core::StringView ctx_id, core::String&& ctx_description, LoggerBase& logger_base) noexcept
      -> Logger&;

  /*!
   * \brief Reads in the configuration values from the file.
   *
   * \param logging_config_file Path to logging config file.
   * \param reactor             The passed reactor.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Initialize(core::StringView logging_config_file, BorrowedReactor& reactor) noexcept -> Result<void>;

  /*!
   * \brief Reads in the configuration values from a runtime config.
   *
   * \param config  The already created configuration.
   * \param reactor The passed reactor.
   * \spec
   *   requires true;
   * \endspec
   */
  auto InitializeFromConfig(config::LoggingConfig&& config, BorrowedReactor& reactor) noexcept -> Result<void>;

  /*!
   * \brief Initializes the registered log sinks.
   *
   * \param reactor The passed reactor.
   *
   * \note Can be folded into Initialize once 'InitLogging' is removed.
   * \spec
   *   requires true;
   * \endspec
   */
  auto InitializeSinks(BorrowedReactor& reactor) noexcept -> Result<void>;

  /*!
   * \brief Executes the given callable on the list of registered log sinks.
   *
   * \param fn Callable to execute. Must return Result<void>.
   * \details  The function stops if the callable returns an error.
   * \return   The empty result or the first error that occurred.
   * \spec
   *   requires true;
   * \endspec
   */
  auto InspectLogSinks(FunctionRef<Result<void>(ILogSinkBase&)> fn) noexcept -> Result<void> {
    Result<void> result{};
    for (ILogSinkBase& log_sink : this->registered_sinks_) {
      // VCA_LOGAPI_VALID_OBJECT
      result = result.And(fn(log_sink));
    }
    return result;
  }

  /*!
   * \brief Filter a message by logger & sink.
   *
   * \param meta_data The logger meta data.
   * \param cfg The configuration containing the sink data.
   * \param sink_name The name of the sink.
   * \return True if the message is filtered.
   *
   * \trace DSGN-LogAndTrace-FilterMessage
   */
  // VECTOR NC AutosarC++17_10-A3.3.1: MD_LOG_AutosarC++17_10-A3.3.1
  static auto FilterMessage(MetaData meta_data, config::SinkConfigMap const& cfg, core::StringView sink_name) noexcept
      -> bool;

  /*!
   * \brief Initialize the IpcClient and connect to LogDaemon.
   *
   * \param reactor          The passed reactor.
   * \param should_receive   Flag to indicate if IPC client enable functionality to receive messages.
   * \return                 The empty result or the first error that occurred.
   * \spec
   *   requires true;
   * \endspec
   *
   * \trace DSGN-LogAndTrace-Ipc
   */
  auto StartIpcClient(BorrowedReactor& reactor, bool should_receive) noexcept -> Result<void>;

  FRIEND_TEST(UT__Utility, MessageMetaDataUtils); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase,
              InitializeFromConfigShouldNotStartIpcClient);              /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, InitializeFromConfigShouldStartIpcClient); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase,
              StartIpcClientReturnsErrorOnMissingIpcPort);           /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, RegisterNewLoggerAddsLoggerToStorage); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase,
              InspectLogContextsInvokesCallbackIfLoggerIsPresent);    /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, MessagesCanBeSentWithoutSinkConfig);    /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, MessagesCanBeSentWithoutSinkThreshold); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase,
              MessageIsAllowedThroughFilterWithSinkThresholdSet);        /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, MessageIsFilteredOutBasedOnThresholds);    /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase, MessageIsFilteredIfLoggerOverrideIsSetup); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LoggerBase,
              MessageIsFilteredOutIfMessageAboveSinkThreshold); /*!< \brief Friend declaration for testing. */
};

/*!
 * \brief Gets the raw loggerbase singleton.
 *
 * \vprivate Component internal.
 * \spec
 *   requires true;
 * \endspec
 */
auto GetRawLoggerBaseSingleton() noexcept -> Box<LoggerBase>&;
}  // namespace internal
}  // namespace log
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::log::internal::LoggerBase::base_logger_guard_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::LoggerBase::logger_storage_
 * \usedin          ::amsr::log::internal::LoggerBase::InspectLogContexts
 *                  ::amsr::log::internal::LoggerBase::CreateLogContext
 *                  ::amsr::log::internal::LoggerBase::SetLogLevelForContext
 *                  ::amsr::log::internal::LoggerBase::SetLogLevelForMode
 * \exclude         All other functions of LoggerBase
 * \length          MEDIUM
 *                  Lock active during all updates to the contained loggers.
 * \endexclusivearea
 */

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOGGER_BASE_H_
