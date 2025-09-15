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
/*! \file
 *  \brief Logger for the IpcServiceDiscovery component.
 *  \unit IpcServiceDiscovery::Common::Logger
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_LOGGER_LOGGER_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_LOGGER_LOGGER_H_

// VECTOR NC AutosarC++17_10-A16.0.1: MD_IPCSERVICEDISCOVERY_A16.0.1_macroForLogging
/*! Macro for creating an instance of the Location struct with the current filename and line number. */
#define LOCATION \
  ::amsr::ipc_service_discovery::common::internal::Logger::Location { __FILE__, __LINE__ }

#include <string>
#include "amsr/ipc_service_discovery/common/internal/types/constants.h"
#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/log/logging.h"
#include "vac/container/string_literals.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Logger for the IpcServiceDiscovery component.
 * \vprivate Component Private
 */
class Logger final {
 public:
  /*!
   * \brief Log level for the Logger.
   */
  using LogLevel = ::ara::log::LogLevel;

  /*!
   * \brief Interface of the callback function that logs a message.
   * \param[in] log_stream A reference to the LogStream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  using LogCallback = vac::language::UniqueFunction<void(::ara::log::LogStream& log_stream)>;

  /*!
   * \brief Location struct for location information.
   * \details This struct is intended to be used with the LOCATION macro for all the APIs of Logger.
   * \vprivate Component Private
   */
  struct Location {
    ::ara::core::StringView file_name{};
    std::uint32_t line_number{0};
  };

  /*!
   * \brief Constructor with ::ara::core::StringView parameter list.
   * \param[in] prefix The prefix that will be printed in front of the logging message.
   * \param[in] context_id The context ID.
   * \param[in] context_description The context description.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  explicit Logger(::ara::core::StringView prefix = ""_sv,
                  ::ara::core::StringView context_id = Constants::kIpcServiceDiscoveryLoggerContextId,
                  ::ara::core::StringView context_description = "Logger for IpcServiceDiscovery component") noexcept;

  /*!
   * \brief Log a message with LogLevel verbose.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogVerbose(::ara::core::StringView log_message,
                  ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel verbose.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogVerbose(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel debug.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogDebug(::ara::core::StringView log_message,
                ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel debug.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogDebug(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel info.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogInfo(::ara::core::StringView log_message,
               ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel info.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogInfo(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel warn.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogWarn(::ara::core::StringView log_message,
               ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel warn.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogWarn(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel error.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogError(::ara::core::StringView log_message,
                ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel error.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogError(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel fatal.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogFatal(::ara::core::StringView log_message,
                ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with LogLevel fatal.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogFatal(LogCallback log_message, ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with severity fatal if an asserted condition fails.
   * \param[in] condition The condition that needs to hold.
   * \param[in] logger A const reference to the Logger.
   * \param[in] condition_name A StringView to the condition name.
   * \param[in] log_message A StringView to the message to log with severity error.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void LogAssert(bool condition, ::ara::core::StringView condition_name,
                 ::ara::core::Optional<common::internal::Logger::Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with specified LogLevel.
   * \param[in] log_level LogLevel of the message to be logged.
   * \param[in] log_message StingView to the message to be logged.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec requires true; \endspec
   */
  void Log(LogLevel log_level, ::ara::core::StringView log_message,
           ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

  /*!
   * \brief Log a message with specified LogLevel.
   * \param[in] log_level LogLevel of the message to be logged.
   * \param[in] log_message LogCallback for streaming the message to be logged.
   *                        The callable shall not throw any exception and has to be silent.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed by
   *                         the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Log(LogLevel log_level, LogCallback log_message,
           ::ara::core::Optional<Location> const& location_opt = {}) const noexcept;

 private:
  /*!
   * \brief Log the prefix specified by the user.
   * \param[in] log_stream LogStream reference to log the prefix to.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LogPrefix(::ara::log::LogStream& log_stream) const noexcept;

  /*!
   * \brief Preparing a log message by adding prefix and evaluating location.
   * \param[in] log_stream LogStream reference to log the prefix to.
   * \param[in] location_opt Optional containing a Location for location information. This is intended to be passed
   *                         by the LOCATION macro.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void PrepareLog(::ara::log::LogStream& log_stream,
                  ::ara::core::Optional<Location> const& location_opt) const noexcept;

  /*! Reference to the logger. */
  std::reference_wrapper<::ara::log::Logger> logger_;

  /*! Prefix string of the Logger. */
  ::ara::core::StringView prefix_;

  /*! Friend declarations for testing purposes. */
  FRIEND_TEST(UT__Logger__Logger, Constructor);
  FRIEND_TEST(UT__Logger__Logger, LogStringView);
  FRIEND_TEST(UT__Logger__Logger, LogLogCallback);
};

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_LOGGER_LOGGER_H_
