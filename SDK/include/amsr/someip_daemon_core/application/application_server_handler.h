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
/*!        \file  application_server_handler.h
 *        \brief  Represent a connected instance of a local application server handler.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/application/application_server_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/application/sending_routing_controller.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/server/local_server_error.h"
#include "amsr/someip_daemon_core/server/local_server_manager.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/log/logging.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/language/unique_function.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Handles the server of a local application.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP Daemon and the application.
 */
template <typename ApplicationConnectionType>
class ApplicationServerHandler : public ApplicationServerHandlerInterface {
 public:
  /*!
   * \brief A type alias for "ApplicationConnection".
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief A type alias for "SendingRoutingController".
   */
  using SendingRoutingControllerType = SendingRoutingController<ApplicationConnection>;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief Constructor.
   *
   * \param[in] identity_access_manager    A reference to the identity access manager.
   * \param[in] application_id             A unique application ID.
   * \param[in] config                     A configuration.
   * \param[in] sending_routing_controller A share pointer of sending_routing_controller.
   * \param[in] local_server_manager       A reference to the LocalServerManager.
   * \param[in] local_servers              A reference to the container of offered local servers.
   *
   * \pre -
   * \context App
   * \steady  FALSE
   */
  ApplicationServerHandler(iam::IamInterface& identity_access_manager, amsr::ipc::Credentials const application_id,
                           configuration::Configuration const& config,
                           std::shared_ptr<SendingRoutingControllerType> const& sending_routing_controller,
                           server::LocalServerManager& local_server_manager, LocalServerMap& local_servers)
      : ApplicationServerHandlerInterface(),
        identity_access_manager_{identity_access_manager},
        application_identifier_{application_id},
        config_{config},
        sending_routing_controller_{sending_routing_controller},
        local_server_manager_{local_server_manager},
        local_servers_{local_servers} {}

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Destructor.
   *
   * \pre -
   * \context Shutdown, Timer
   * \steady  FALSE
   */
  ~ApplicationServerHandler() noexcept override = default;

  ApplicationServerHandler(ApplicationServerHandler const&) = delete;
  ApplicationServerHandler(ApplicationServerHandler const&&) = delete;
  auto operator=(ApplicationServerHandler const&) -> ApplicationServerHandler& = delete;
  auto operator=(ApplicationServerHandler const&&) -> ApplicationServerHandler& = delete;

  /*!
   * \brief     Called when the connected application requests ownership of a local server.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return    The operation's result.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If the identity access manager allows offering the service.
   *   Note: this check can be done once when a skeleton requests ownership of a local server, and, if allowed to offer,
   *   there is no need to check this every time the skeleton calls OfferService.
   * - Request local server implementation.
   * - If request succeeded.
   *   - Store the created local server in the local server map.
   *   - Set positive operation result.
   *   - Otherwise, log the appropriate error.
   * - Otherwise, local server not available (already requested or configuration error). Log error.
   * - In case of IAM lack of permissions, log error and return appropriate error code.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  RequestLocalServerOperationResult RequestLocalServer(
      configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept override {
    RequestLocalServerOperationResult operation_result{OperationResult::kFailure};

    logger_.LogInfo(
        [&service_instance_id](ara::log::LogStream& s) noexcept {
          s << "(ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << "). ";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Check access rights at first.
    if (identity_access_manager_.CheckOfferService(
            service_instance_id.deployment_id.service_interface_id,
            service_instance_id.deployment_id.service_interface_version.major_version,
            service_instance_id.deployment_id.service_interface_version.minor_version, service_instance_id.instance_id,
            application_identifier_)) {
      LocalServerId const local_server_id{
          configuration::types::SomeIpServiceInterfaceDeploymentId{
              service_instance_id.deployment_id.service_interface_id,
              configuration::types::SomeIpServiceVersion{
                  service_instance_id.deployment_id.service_interface_version.major_version,
                  0U /* Multiple MinorVersions not yet supported*/}},
          service_instance_id.instance_id};

      // Request ownership of the local server implementation
      ara::core::Result<server::LocalServerManager::LocalServerUniquePtr> local_server_result{
          local_server_manager_.RequestLocalServer(local_server_id)};

      if (local_server_result.HasValue()) {
        // Add local server to the list of owned local servers by this application
        static_cast<void>(local_servers_.emplace(local_server_id, std::move(local_server_result.Value())));
        // Set positive result
        operation_result.result_ = OperationResult::kOk;
      } else {
        // Log the error
        logger_.LogError(
            [&service_instance_id, &local_server_result](ara::log::LogStream& s) noexcept {
              ara::core::ErrorCode const error_code{local_server_result.Error()};
              ara::core::StringView const error_message{error_code.Message()};
              s << "Error requesting local server (ServiceId: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
              s << ", InstanceId: 0x";
              s << ara::log::HexFormat(service_instance_id.instance_id);
              s << "). ";
              s << error_message;
            },
            static_cast<char const*>(__func__), __LINE__);

        // Transform the error code in case of configuration failure
        if (local_server_result.Error() == server::LocalServerOperationError::kServiceNotConfigured) {
          operation_result.result_ = OperationResult::kConfigurationFailure;
        }
      }
    } else {
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) noexcept {
            s << "Error requesting local server (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Access denied by Identity and Access Management.";
          },
          static_cast<char const*>(__func__), __LINE__);
      operation_result.result_ = OperationResult::kAccessDenied;
    }

    return operation_result;
  }

  /*!
   * \brief     Called when the connected application releases ownership of the local server.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Attempt to erase the local server based on the given id
   *   This will internally trigger StopOfferService in the LocalServerImpl in case StopOfferService has not been
   *   explicitly called by the application.
   * - If no local servers could be erased based on that key
   *   - Log error
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void ReleaseLocalServer(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept override {
    LocalServerId const local_server_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version,
                0U /* Multiple MinorVersions not yet supported*/}},
        service_instance_id.instance_id};

    logger_.LogInfo(
        [&local_server_id](ara::log::LogStream& s) noexcept {
          s << "(ServiceId: 0x";
          s << ara::log::HexFormat(local_server_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(local_server_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(local_server_id.instance_id);
          s << "). ";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Attempt to erase the local server with the given key.
    // If successfully erased, the destruction of the LocalServerImpl object will
    // trigger LocalServerImpl::StopOfferService
    std::size_t const elem_erased{local_servers_.erase(local_server_id)};

    if (elem_erased != 1U) {
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) noexcept {
            s << "Error releasing local server (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Local Server not owned by this application.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief     Called when the connected application offers the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \pre       RequestLocalServer must have been called before and it must have returned success.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Try to find the local server in the local server map.
   * - If found (local server ownership already acquired).
   *   - Trigger OfferService on it.
   * - Otherwise, local server already owned by other application. Log error.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void OfferService(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept override {
    LocalServerId const local_server_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version,
                0U /* Multiple MinorVersions not yet supported*/}},
        service_instance_id.instance_id};

    logger_.LogDebug(
        [&local_server_id](ara::log::LogStream& s) noexcept {
          s << "(ServiceId: 0x";
          s << ara::log::HexFormat(local_server_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(local_server_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(local_server_id.instance_id);
          s << "). ";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Check if the local server is already owned by this application
    LocalServerMap const& local_servers_const_ref{local_servers_};
    typename LocalServerMap::const_iterator const it{local_servers_const_ref.find(local_server_id)};

    if (it != local_servers_const_ref.cend()) {
      // Offer the service
      it->second->OfferService(sending_routing_controller_);
    } else {
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) noexcept {
            s << "Error offering service (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). The local server has not been requested before.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief     Called when the connected application stops offering the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Attempt to find the local server in the local server map
   * - If the local server could be found, trigger StopOfferService
   * - Otherwise, log an error.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void StopOfferService(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept override {
    LocalServerId const local_server_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version,
                0U /* Multiple MinorVersions not yet supported*/}},
        service_instance_id.instance_id};

    logger_.LogDebug(
        [&service_instance_id](ara::log::LogStream& s) noexcept {
          s << "(ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << "). ";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Check if the service is already owned by any application
    LocalServerMap const& local_servers_const_ref{local_servers_};
    typename LocalServerMap::const_iterator const it{local_servers_const_ref.find(local_server_id)};

    if (it != local_servers_const_ref.cend()) {
      // Stop offering the service
      it->second->StopOfferService();
    } else {
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) noexcept {
            s << "Error stopping offer service (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Local Server not owned by this application.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Release all allocated resources.
   *
   * \pre -
   * \context   Shutdown, Timer, App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Stop offering services.
   * \endinternal
   */
  void CleanUpServerResources() {
    logger_.LogDebug([](ara::log::LogStream& s) noexcept { s << "Cleaning up server resources"; },
                     static_cast<char const*>(__func__), __LINE__);
    // PTP-B-ApplicationServerHandler_CleanUpServerResources
    // Clean up servers. This must be done after cleaning up the routes in the "PacketRouter",
    // so we don't send other error responses to the destroyed application.
    local_servers_.clear();
    // PTP-E-ApplicationServerHandler_CleanUpServerResources
  }

 private:
  /*!
   * \brief Reference to identity and access management.
   */
  iam::IamInterface& identity_access_manager_;

  /*!
   * \brief The unique identity of the application.
   */
  amsr::ipc::Credentials const application_identifier_;

  /*!
   * \brief A reference to a configuration instance.
   */
  configuration::Configuration const& config_;

  /*!
   * \brief The sending routing controller.
   * \details It must be a shared pointer because the packet router accepts only shared pointers.
   */
  std::shared_ptr<SendingRoutingControllerType> sending_routing_controller_;

  /*!
   * \brief Reference to local server management.
   */
  server::LocalServerManager& local_server_manager_;

  /*!
   * \brief Map of offered local servers.
   */
  LocalServerMap& local_servers_;

  /*!
   * \brief Generates a unique logger context identifier for a given application
   *
   * \param[in] application_identifier An application identifier
   * \return    A string representing the logger context identifier for the given application.
   *
   * \pre       -
   * \context   App
   * \steady    FALSE
   */
  static ara::core::String GetLoggerContextId(amsr::ipc::Credentials const application_identifier) {
    constexpr std::size_t kBufferSize{50};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};
    prefix_builder.Write(ara::core::StringView{"ApplicationServerHandler: "});
    prefix_builder.Write(ara::core::StringView{"User ID: "});
    // VECTOR NL AutosarC++17_10-M8.4.4: MD_SomeIpDaemon_M8.4.4_FalsePositive
    prefix_builder << ::amsr::stream::SetShowBase;
    prefix_builder.Write(application_identifier, ::amsr::stream::Base::kHex);
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    GetLoggerContextId(application_identifier_)};
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_H_
