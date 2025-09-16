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
 *        \brief  Provides helper functions to build the log streams for the different datatypes used in IPC-binding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_LOG_BUILDER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_LOG_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/integrity_level.h"
#include "ara/log/logging.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace logging {

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Helper class for Ipc-Binding values logging.
 *
 * \unit IpcBinding::IpcBindingCore::IpcBindingLogging
 */
class IpcBindingLogBuilder final {
 public:
  /*!
   * \brief Alias for LogStream provided by AraComLogger.
   */
  using LogStream = AraComLogger::LogStream;

  /*!
   * \brief Alias for LogLocation provided by AraComLogger.
   */
  using LogLocation = AraComLogger::LogLocation;

  // ---- Construction -------------------------------------------------------------------------------------------------

  IpcBindingLogBuilder() noexcept = delete;
  IpcBindingLogBuilder(IpcBindingLogBuilder const&) noexcept = delete;
  auto operator=(IpcBindingLogBuilder const&) noexcept -> IpcBindingLogBuilder& = delete;
  IpcBindingLogBuilder(IpcBindingLogBuilder&&) noexcept = delete;
  auto operator=(IpcBindingLogBuilder&&) noexcept -> IpcBindingLogBuilder& = delete;

  /*!
   * \brief   Destroy the log builder.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcBindingLogBuilder() noexcept = default;

  // ---- APIs ---------------------------------------------------------------------------------------------------------

  /*!
   * \brief Builds a log stream for the given IPC binding service ID.
   *
   * \param[in,out] s           The log stream to write to.
   * \param[in]     service_id  The service ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogServiceId(LogStream& s, ipc_protocol::ServiceId const service_id) noexcept;
  /*!
   * \brief Builds a log stream for the given IPC binding instance ID.
   *
   * \param[in,out] s            The log stream to write to.
   * \param[in]     instance_id  The instance ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogInstanceId(LogStream& s, ipc_protocol::InstanceId const instance_id) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding major version.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     major_version  The major version.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogMajorVersion(LogStream& s, ipc_protocol::MajorVersion const major_version) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding minor version.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     minor_version  The minor version.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogMinorVersion(LogStream& s, ipc_protocol::MinorVersion const minor_version) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding event id.
   *
   * \param[in,out] s         The log stream to write to.
   * \param[in]     event_id  The event ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogEventId(LogStream& s, ipc_protocol::EventId const event_id) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding method id.
   *
   * \param[in,out] s          The log stream to write to.
   * \param[in]     method_id  The method ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogMethodId(LogStream& s, ipc_protocol::MethodId const method_id) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding client id.
   *
   * \param[in,out] s          The log stream to write to.
   * \param[in]     client_id  The client ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogClientId(LogStream& s, ipc_protocol::ClientId const client_id) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding session id.
   *
   * \param[in,out] s           The log stream to write to.
   * \param[in]     session_id  The session ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogSessionId(LogStream& s, ipc_protocol::SessionId const session_id) noexcept;

  /*!
   * \brief Builds a log stream for the given ServiceInstanceIdentifier.
   *
   * \param[in,out] s                    The log stream to write to.
   * \param[in]     service_instance_id  The ServiceInstanceIdentifier.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogServiceInstanceId(LogStream& s,
                                   ipc_protocol::ServiceInstanceIdentifier const& service_instance_id) noexcept;

  /*!
   * \brief Builds a log stream for the given RequiredServiceInstanceId.
   *
   * \param[in,out] s                             The log stream to write to.
   * \param[in]     required_service_instance_id  The RequiredServiceInstanceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogServiceInstanceId(LogStream& s,
                                   RequiredServiceInstanceId const& required_service_instance_id) noexcept;

  /*!
   * \brief Builds a log stream for the given ProvidedServiceInstanceId.
   *
   * \param[in,out] s                             The log stream to write to.
   * \param[in]     provided_service_instance_id  The ProvidedServiceInstanceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogServiceInstanceId(LogStream& s,
                                   ProvidedServiceInstanceId const& provided_service_instance_id) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding service config.
   *
   * \param[in,out] s               The log stream to write to.
   * \param[in]     service_config  The service config object.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceConfig(
      LogStream& s,
      ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig const& service_config) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding service id, major version, and minor version.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     service_id     The service ID.
   * \param[in]     major_version  The major version.
   * \param[in]     minor_version  The minor version.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceId(LogStream& s, ipc_protocol::ServiceId const service_id,
                                   ipc_protocol::MajorVersion const major_version,
                                   ipc_protocol::MinorVersion const minor_version) noexcept;

  /*!
   * \brief Builds a log stream for the given IPC binding service id, major version, and instance id.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     service_id     The service ID.
   * \param[in]     major_version  The major version.
   * \param[in]     instance_id    The instance ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceInstanceId(LogStream& s, ipc_protocol::ServiceId const service_id,
                                           ipc_protocol::MajorVersion const major_version,
                                           ipc_protocol::InstanceId const instance_id) noexcept;
  /*!
   * \brief Builds a log stream for the given service id, major version, minor version and instance id.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     service_id     The service ID.
   * \param[in]     major_version  The major version.
   * \param[in]     minor_version  The minor version.
   * \param[in]     instance_id    The instance ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceInstanceId(LogStream& s, ipc_protocol::ServiceId const service_id,
                                           ipc_protocol::MajorVersion const major_version,
                                           ipc_protocol::MinorVersion const minor_version,
                                           ipc_protocol::InstanceId const instance_id) noexcept;

  /*!
   * \brief Builds a log stream for the given service id, major version, instance id and event id.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     service_id     The service ID.
   * \param[in]     major_version  The major version.
   * \param[in]     instance_id    The instance ID.
   * \param[in]     event_id       The event ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceInstanceEventId(LogStream& s, ipc_protocol::ServiceId const service_id,
                                                ipc_protocol::MajorVersion const major_version,
                                                ipc_protocol::InstanceId const instance_id,
                                                ipc_protocol::EventId const event_id) noexcept;

  /*!
   * \brief Builds a log stream for the given service id, major version, instance id and method id.
   *
   * \param[in,out] s              The log stream to write to.
   * \param[in]     service_id     The service ID.
   * \param[in]     major_version  The major version.
   * \param[in]     instance_id    The instance ID.
   * \param[in]     method_id      The method ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteServiceInstanceMethodId(LogStream& s, ipc_protocol::ServiceId const service_id,
                                                 ipc_protocol::MajorVersion const major_version,
                                                 ipc_protocol::InstanceId const instance_id,
                                                 ipc_protocol::MethodId const method_id) noexcept;

  /*!
   * \brief Builds a log stream for the given BasicIpc connection unicast address.
   *
   * \param[in,out] s        The log stream to write to.
   * \param[in]     address  The unicast address to be logged.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogConnectionUnicastAddress(LogStream& s, ipc_protocol::IpcUnicastAddress const& address) noexcept;

  /*!
   * \brief Builds a log stream for the given Integrity level.
   *
   * \param[in,out] s                The log stream to write to.
   * \param[in]     integrity_level  The integrity level to be logged.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogIntegrityLevel(LogStream& s, amsr::ipc::IntegrityLevel const integrity_level) noexcept;
};

}  // namespace logging
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_LOG_BUILDER_H_
