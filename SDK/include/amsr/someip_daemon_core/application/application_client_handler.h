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
/*!        \file  application_client_handler.h
 *        \brief  Represent a connected instance of a local application client handler.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <sstream>
#include <tuple>
#include <utility>
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/application/application_client_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/application/sending_routing_controller.h"
#include "amsr/someip_daemon_core/client/required_service_instance_manager.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logging_utilities.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/log/logging.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Handles the server of a local application.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP Daemon and the application.
 */
template <typename ApplicationConnectionType>
class ApplicationClientHandler : public service_discovery::ServiceDiscoveryOfferServiceObserver,
                                 public ApplicationClientHandlerInterface {
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
   * \brief A SOME/IP service instance/address.
   */
  using ServiceInstanceAddressMap = ara::core::Map<someip_protocol::internal::InstanceId,
                                                   amsr::someip_daemon_core::configuration::types::ServiceAddress>;

  /*!
   * \brief Type alias for a unique pointer to the local client interface.
   */
  using LocalClientInterfaceUniquePtr = amsr::UniquePtr<client::LocalClientInterface>;

  /*!
   * \brief Constructor.
   *
   * \param[in] identity_access_manager A reference to the identity access manager.
   * \param[in] application_id A unique application ID.
   * \param[in] sending_routing_controller A share pointer of sending_routing_controller.
   * \param[in] required_service_instance_manager  A reference to the RequiredServiceInstanceManager.
   * \param[in] local_clients  A reference to the container of local clients.
   *
   * \pre -
   * \context App
   * \steady  FALSE
   */
  ApplicationClientHandler(iam::IamInterface& identity_access_manager, amsr::ipc::Credentials const application_id,
                           std::shared_ptr<SendingRoutingControllerType> const& sending_routing_controller,
                           client::RequiredServiceInstanceManager& required_service_instance_manager,
                           LocalClientMap& local_clients) noexcept
      : service_discovery::ServiceDiscoveryOfferServiceObserver(),
        ApplicationClientHandlerInterface(),
        identity_access_manager_{identity_access_manager},
        application_identifier_{application_id},
        sending_routing_controller_{sending_routing_controller},
        required_service_instance_manager_{required_service_instance_manager},
        local_clients_{local_clients} {}

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
  ~ApplicationClientHandler() noexcept override = default;

  ApplicationClientHandler(ApplicationClientHandler const&) = delete;

  ApplicationClientHandler(ApplicationClientHandler const&&) = delete;

  auto operator=(ApplicationClientHandler const&) -> ApplicationClientHandler& = delete;
  auto operator=(ApplicationClientHandler const&&) -> ApplicationClientHandler& = delete;

  /*!
   * \brief Called when the connected application requests the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return The newly allocated SOME/IP client identifier if the operation is successful and error otherwise.
   * \error SomeIpDaemonErrc::access_denied If the IAM does not grant the permission to request a service.
   * \error SomeIpDaemonErrc::client_ids_overflow Local client Id generator reaches the maximum number of simultaneous
   *        clients.
   * \error SomeIpDaemonErrc::remote_server_not_found If no remote server is found.
   * \error SomeIpDaemonErrc::required_service_instance_not_found If no required service instance is found.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the service, instance IDs, major version and minor version.
   * - Set the return code to indicate access denied error.
   * - If the identity access manager grants access to the FindService operation,
   *   - Get the required service instance.
   *   - If the required service instance is not a nullptr.
   *    - Request the service from required service instance.
   *    - If request service result has value, emplace the local client in the local clients map.
   *     - If emplaced successfully,
   *      - Register the response handler.
   *      - Emplace the client Id to be returned.
   *     - Else, log an error and abort.
   *    - Otherwise, set the return value to the request service result.
   *   - Otherwise, set the return value to indicate that required service instance is not exist.
   * - Otherwise, log a warn that IAM access denied.
   * - Return the return value.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  ara::core::Result<someip_protocol::internal::ClientId> RequestService(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept override {
    logger_.LogDebug(
        [&service_instance_id](ara::log::LogStream& s) {
          s << "(ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", MinorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.minor_version);
          s << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    ara::core::Result<someip_protocol::internal::ClientId> result{
        ara::core::Result<someip_protocol::internal::ClientId>::FromError(SomeIpDaemonErrc::access_denied)};

    // CheckFindService API is called in the context of proxy creation during the 'RequestService' process.
    bool const access_granted{identity_access_manager_.CheckFindService(
        service_instance_id.deployment_id.service_interface_id,
        service_instance_id.deployment_id.service_interface_version.major_version,
        service_instance_id.deployment_id.service_interface_version.minor_version, service_instance_id.instance_id,
        application_identifier_)};

    // Check the access rights.
    if (access_granted) {
      // Create SomeIpServiceInstanceIdCommunication for require service instance (with no minor version)
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id_communication{
          configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication{
              service_instance_id.deployment_id.service_interface_id,
              service_instance_id.deployment_id.service_interface_version.major_version},
          service_instance_id.instance_id};

      std::shared_ptr<client::RequiredServiceInstanceInterface> const required_service_instance{
          required_service_instance_manager_.GetRequiredServiceInstance(service_instance_id_communication)};

      if (required_service_instance != nullptr) {
        ara::core::Result<LocalClientInterfaceUniquePtr> request_service_result{
            required_service_instance->RequestService(service_instance_id.instance_id)};
        if (request_service_result.HasValue()) {
          // Create a local SomeIpServiceInstanceId copy with dummy minor version
          configuration::types::SomeIpServiceInstanceId const service_instance_id_dummy_minor_version{
              configuration::types::SomeIpServiceInterfaceDeploymentId{
                  service_instance_id.deployment_id.service_interface_id,
                  configuration::types::SomeIpServiceVersion{
                      service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
              service_instance_id.instance_id};
          amsr::someip_protocol::internal::ClientId const client_id{request_service_result.Value()->GetClientId()};
          configuration::types::LocalClientId const local_client_id{service_instance_id_dummy_minor_version, client_id};

          std::pair<typename LocalClientMap::iterator, bool> const emplace_result{
              local_clients_.emplace(local_client_id, std::move(request_service_result.Value()))};
          if (emplace_result.second) {
            emplace_result.first->second->RegisterResponseHandler(sending_routing_controller_.get());
            result.EmplaceValue(local_client_id.client_id);
          } else {
            logger_.LogFatalAndAbort(
                [&service_instance_id](ara::core::String& abort_msg) noexcept {
                  std::ostringstream string_stream{};
                  string_stream << "Failed to emplace a local client in the local clients map (ServiceId: 0x";
                  string_stream << std::hex << service_instance_id.deployment_id.service_interface_id;
                  string_stream << ", InstanceId: 0x";
                  string_stream << std::hex << service_instance_id.instance_id;
                  string_stream << ", MajorVersion: 0x";
                  string_stream << std::hex
                                << static_cast<std::uint16_t>(
                                       service_instance_id.deployment_id.service_interface_version.major_version);
                  string_stream << ", MinorVersion: 0x";
                  string_stream << std::hex
                                << static_cast<std::uint16_t>(
                                       service_instance_id.deployment_id.service_interface_version.minor_version);
                  string_stream << "). Local client already exists";
                  abort_msg.append(string_stream.str());
                },
                AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
          }
        } else {
          result.EmplaceError(request_service_result.Error());
        }
      } else {
        result.EmplaceError(SomeIpDaemonErrc::required_service_instance_not_found);
      }
    } else {
      logger_.LogWarn(
          [&service_instance_id](ara::log::LogStream& s) noexcept {
            s << "Can't request service. IAM access denied. (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", MinorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.minor_version);
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Called when the connected application request a start of service discovery for the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return "true" if the service discovery was started successfully and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the service and instance IDs.
   * - Find the required service instance.
   *   - Start service discovery for the required service instance.
   *   - Register this service as requested in the container of started service discoveries.
   *   - Get all already-offered services.
   *     - For each offered service, send initial snapshot to the application
   *     - Log debug if no service is offered.
   * - If required service instance could not be found, log error
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  bool StartServiceDiscovery(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept override {
    bool result{false};

    logger_.LogDebug(
        [&service_instance_id](ara::log::LogStream& s) {
          s << "Starting service discovery of service (ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Create SomeIpServiceInstanceIdCommunication for require service instance (with no minor version)
    configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id_communication{
        configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication{
            service_instance_id.deployment_id.service_interface_id,
            service_instance_id.deployment_id.service_interface_version.major_version},
        service_instance_id.instance_id};

    std::shared_ptr<client::RequiredServiceInstanceInterface> const required_service_instance{
        required_service_instance_manager_.GetRequiredServiceInstance(service_instance_id_communication)};

    // Check if the service instance is configured
    if (required_service_instance != nullptr) {
      // Create a local SomeIpServiceInstanceId copy with dummy minor version
      configuration::types::SomeIpServiceInstanceId const requested_service_instance{
          configuration::types::SomeIpServiceInterfaceDeploymentId{
              service_instance_id.deployment_id.service_interface_id,
              configuration::types::SomeIpServiceVersion{
                  service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
          service_instance_id.instance_id};
      required_service_instance->StartServiceDiscovery(this);

      // A service instance might be requested multiple times.
      started_discoveries_of_requested_service_instances_.push_back(requested_service_instance);

      // Send initial snapshot if service was already found.
      ServiceInstanceAddressMap const& offered_service_instances{required_service_instance->GetOfferedServices()};

      // Send initial snapshot with all offered service instances
      typename ServiceInstanceAddressMap::const_iterator const iter_begin{offered_service_instances.cbegin()};
      typename ServiceInstanceAddressMap::const_iterator const iter_end{offered_service_instances.cend()};

      // Loop over all found instance IDs. This loop may contain more than one iteration only in case of InstaceId ALL
      for (ServiceInstanceAddressMap::const_iterator it{iter_begin}; it != iter_end; ++it) {
        // The exact found service instance (instance ID must not be ALL)
        configuration::types::SomeIpServiceInstanceId const found_service_instance{
            configuration::types::SomeIpServiceInterfaceDeploymentId{
                service_instance_id.deployment_id.service_interface_id,
                configuration::types::SomeIpServiceVersion{
                    service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
            it->first};

        sending_routing_controller_->OnStartOfferServiceInstance(found_service_instance);
      }

      // Log amount of services found during initial snapshot
      logger_.LogInfo(
          [&service_instance_id, &offered_service_instances](ara::log::LogStream& s) {
            s << "Initial snapshot (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Number of services found: ";
            s << offered_service_instances.size();
          },
          static_cast<char const*>(__func__), __LINE__);

      result = true;
    } else {
      // Required service instance not found. Configuration mismatch
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) {
            s << "Error starting service discovery of service (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Possible configuration mismatch between Daemon and Application.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Called when the connected application request to stop service discovery for the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return "true" if the service discovery is stopped successfully and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Log the service and instance IDs.
   * - Set the return value to indicate failure.
   * - Search the service in the list of started service discoveries.
   * - If a service is found in the list,
   *   - Use the service discovery to release the service.
   *   - If the release of the service is successful,
   *     - Remove the service from the list of started service discoveries.
   *     - Set the return value to indicate success.
   *   - Else
   *     - Log an error about the failed stop.
   * - Return the return value.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  bool StopServiceDiscovery(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept override {
    logger_.LogDebug(
        [&service_instance_id](ara::log::LogStream& s) {
          s << "Stopping service discovery of a service (ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    bool result{false};

    // Create a local SomeIpServiceInstanceId copy with dummy minor version
    configuration::types::SomeIpServiceInstanceId const service_instance_id_dummy_minor_version{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
        service_instance_id.instance_id};

    ServiceInstancesContainer::const_iterator const it{
        std::find(started_discoveries_of_requested_service_instances_.cbegin(),
                  started_discoveries_of_requested_service_instances_.cend(), service_instance_id_dummy_minor_version)};

    if (it != started_discoveries_of_requested_service_instances_.cend()) {
      // Create a local copy SomeIpServiceInstanceIdCommunication for require service instance (with no minor version)
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id_communication{
          configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication{
              service_instance_id.deployment_id.service_interface_id,
              service_instance_id.deployment_id.service_interface_version.major_version},
          service_instance_id.instance_id};
      std::shared_ptr<client::RequiredServiceInstanceInterface> const required_service_instance{
          required_service_instance_manager_.GetRequiredServiceInstance(service_instance_id_communication)};

      if (required_service_instance != nullptr) {
        required_service_instance->StopServiceDiscovery(this);
        static_cast<void>(started_discoveries_of_requested_service_instances_.erase(it));
        result = true;
      } else {
        logger_.LogError(
            [&service_instance_id](ara::log::LogStream& s) {
              s << "Service discovery of service (ServiceId: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
              s << ", InstanceId: 0x";
              s << ara::log::HexFormat(service_instance_id.instance_id);
              s << ") couldn't be stopped";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [&service_instance_id](ara::log::LogStream& s) {
            s << "Trying to stop a service discovery without calling StartServiceDiscovery (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << "). Possible configuration mismatch between Daemon and Application.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Request an event subscription as a connected application.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id            A SOME/IP event identifier.
   * \param[in] client_id           The subscriber unique client identifier.
   *
   * \return "OperationResult::kOk" if the service is successfully offered, an error code otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the service, instance and event IDs.
   * - Set the return value to indicate access denied.
   * - Check if identity access manager allows access for the event.
   * - If access is allowed,
   *  - Get local client from the local clients map.
   *   - If local client is exist,
   *    - If subscription result doesn't have value, log an error and set kFailure as a return result.
   *    - Else, set KOk as a return result.
   *  - Else, log an error and set kFailure as a return result.
   * - Else, log an error.
   * - Return the return value.
   * \endinternal
   * \trace SPEC-10144839
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  OperationResult SubscribeEvent(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                 amsr::someip_protocol::internal::EventId const event_id,
                                 amsr::someip_protocol::internal::ClientId const client_id) noexcept override {
    logger_.LogDebug(
        [&service_instance_id, &event_id, &client_id](ara::log::LogStream& s) {
          s << "Subscribing to event (ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << ", EventId: 0x";
          // Append the event Id in the correct format
          logging::LoggingUtilities::LogEventId(s, event_id);
          s << ", ClientId: 0x";
          s << ara::log::HexFormat(client_id);
          s << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    OperationResult result{OperationResult::kAccessDenied};

    // Create a local SomeIpServiceInstanceId copy with dummy minor version
    configuration::types::SomeIpServiceInstanceId const service_instance_id_dummy_minor_version{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
        service_instance_id.instance_id};

    if (identity_access_manager_.CheckEventSubscription(
            service_instance_id_dummy_minor_version.deployment_id.service_interface_id,
            service_instance_id_dummy_minor_version.deployment_id.service_interface_version.major_version,
            service_instance_id_dummy_minor_version.instance_id, event_id, application_identifier_)) {
      configuration::types::LocalClientId const local_client_id{service_instance_id_dummy_minor_version, client_id};

      LocalClientMap const& local_clients_const_ref{local_clients_};
      typename LocalClientMap::const_iterator const local_client_it{local_clients_const_ref.find(local_client_id)};
      if (local_client_it != local_clients_const_ref.cend()) {
        ara::core::Result<void> const subscribe_result{
            local_client_it->second->SubscribeEvent(event_id, sending_routing_controller_)};
        if (!subscribe_result.HasValue()) {
          logger_.LogError(
              [&service_instance_id, &client_id, &event_id, &subscribe_result](ara::log::LogStream& s) noexcept {
                ara::core::StringView const error_msg{subscribe_result.Error().Message()};
                s << "Can't subscribe event with (ServiceId: 0x";
                s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(service_instance_id.instance_id);
                s << ", EventId: 0x";
                // Append the event Id in the correct format
                logging::LoggingUtilities::LogEventId(s, event_id);
                s << ", ClientId: 0x";
                s << ara::log::HexFormat(client_id);
                s << ", Error code: ";
                s << error_msg;
                s << ").";
              },
              static_cast<char const*>(__func__), __LINE__);
          result = OperationResult::kFailure;
        } else {
          result = OperationResult::kOk;
        }
      } else {
        logger_.LogError(
            [&service_instance_id, &client_id, &event_id](ara::log::LogStream& s) noexcept {
              s << "Can't subscribe event with (ServiceId: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
              s << ", InstanceId: 0x";
              s << ara::log::HexFormat(service_instance_id.instance_id);
              s << ", EventId: 0x";
              // Append the event Id in the correct format
              logging::LoggingUtilities::LogEventId(s, event_id);
              s << ", ClientId: 0x";
              s << ara::log::HexFormat(client_id);
              s << "). Local client not found.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = OperationResult::kFailure;
      }
    } else {
      logger_.LogError(
          [&service_instance_id, &client_id, &event_id](ara::log::LogStream& s) noexcept {
            s << "Can't subscribe event. IAM access denied. (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << ", EventId: 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
            s << ", ClientId: 0x";
            s << ara::log::HexFormat(client_id);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Cancel an event subscription as a connected application.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id            A SOME/IP event identifier.
   * \param[in] client_id           The subscriber unique client identifier.
   *
   * \return "OperationResult::kOk" if the event is successfully unsubscribe, an error code otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the service, instance and event IDs.
   * - Set the return value to indicate access denied.
   * - Check if identity access manager allows access for the event.
   * - If access is allowed,
   *  - Get local client from the local clients map.
   *  - If local client is exist,
   *   - Unsubscribe to the event and set kOk as a return result.
   *  - Else, log an error indicates that local client is not exist and set kFailure as a return result.
   * - Otherwise, log an error indicates that IAM access denied.
   * - Return the return value.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  OperationResult UnsubscribeEvent(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                   someip_protocol::internal::EventId const event_id,
                                   someip_protocol::internal::ClientId const client_id) noexcept override {
    logger_.LogDebug(
        [&service_instance_id, &client_id, &event_id](ara::log::LogStream& s) noexcept {
          s << "Unsubscribing to event (ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << ", EventId: 0x";
          // Append the event Id in the correct format
          logging::LoggingUtilities::LogEventId(s, event_id);
          s << ", ClientId: 0x";
          s << ara::log::HexFormat(client_id);
          s << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    OperationResult result{OperationResult::kAccessDenied};

    // Create a local SomeIpServiceInstanceId copy with dummy minor version
    configuration::types::SomeIpServiceInstanceId const service_instance_id_dummy_minor_version{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
        service_instance_id.instance_id};

    // A proxy may get a subscription rejection due to IAM but it will not be notified, so it may try to unsubscribe.
    // Therefore, we must only process unsubscription requests for subscriptions that are not denied by IAM.
    if (identity_access_manager_.CheckEventSubscription(
            service_instance_id_dummy_minor_version.deployment_id.service_interface_id,
            service_instance_id_dummy_minor_version.deployment_id.service_interface_version.major_version,
            service_instance_id_dummy_minor_version.instance_id, event_id, application_identifier_)) {
      configuration::types::LocalClientId const local_client_id{service_instance_id_dummy_minor_version, client_id};
      LocalClientMap const& local_clients_const_ref{local_clients_};
      typename LocalClientMap::const_iterator const local_client_it{local_clients_const_ref.find(local_client_id)};
      if (local_client_it != local_clients_const_ref.cend()) {
        local_client_it->second->UnsubscribeEvent(event_id, sending_routing_controller_);
        result = OperationResult::kOk;
      } else {
        logger_.LogError(
            [&service_instance_id, &client_id, &event_id](ara::log::LogStream& s) noexcept {
              s << "Can't unsubscribe event with (ServiceId: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
              s << ", InstanceId: 0x";
              s << ara::log::HexFormat(service_instance_id.instance_id);
              s << ", EventId: 0x";
              // Append the event Id in the correct format
              logging::LoggingUtilities::LogEventId(s, event_id);
              s << ", ClientId: 0x";
              s << ara::log::HexFormat(client_id);
              s << "). Local client not found.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = OperationResult::kFailure;
      }
    } else {
      logger_.LogWarn(
          [&service_instance_id, &client_id, &event_id](ara::log::LogStream& s) noexcept {
            s << "Can't unsubscribe to an event whose subscription is rejected by IAM. (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << ", EventId: 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
            s << ", ClientId: 0x";
            s << ara::log::HexFormat(client_id);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief Releases a previously allocated SOME/IP client identifier.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] client_id           A SOME/IP client identifier.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Set the return value to indicate failure.
   * - Erase the local client from the local clients map.
   * - If the local client erasing failed, log an error.
   * - Else, set the result to true.
   * - Return the return value.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  bool ReleaseService(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                      amsr::someip_protocol::internal::ClientId const client_id) noexcept override {
    logger_.LogDebug(
        [&service_instance_id, &client_id](ara::log::LogStream& s) {
          s << "ServiceId: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(service_instance_id.instance_id);
          s << "ClientId: 0x";
          s << ara::log::HexFormat(client_id);
        },
        static_cast<char const*>(__func__), __LINE__);
    bool result{false};

    // Create a local SomeIpServiceInstanceId copy with dummy minor version
    configuration::types::SomeIpServiceInstanceId const service_instance_id_dummy_minor_version{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            service_instance_id.deployment_id.service_interface_id,
            configuration::types::SomeIpServiceVersion{
                service_instance_id.deployment_id.service_interface_version.major_version, 0U}},
        service_instance_id.instance_id};
    configuration::types::LocalClientId const local_client_id{service_instance_id_dummy_minor_version, client_id};

    // Erase the local client with the given key.
    // The destruction of the LocalClient object will remove the response
    // route, free the client ID and release the service
    // (e.g. close the TCP connection if no more clients use same service)
    std::size_t const elem_erased{local_clients_.erase(local_client_id)};
    if (elem_erased != 1U) {
      logger_.LogError(
          [&service_instance_id, &client_id](ara::log::LogStream& s) {
            s << "Request to release a service with (ServiceId: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_id);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(service_instance_id.instance_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(service_instance_id.deployment_id.service_interface_version.major_version);
            s << ", ClientId: 0x";
            s << ara::log::HexFormat(client_id);
            s << "). Service is not provided by this client.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      result = true;
    }
    return result;
  }

  /*!
   * \brief Inform the application about start of a service instance offer.
   *
   * \details Implement the pure virtual method of the observer interface from the service discovery.
   *
   * \param[in] service_instance_id    The SOME/IP service instance id.
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the function call.
   * - Notify the routing controller about start of offer of the service instance.
   * \endinternal
   */
  void OnStartOfferServiceInstance(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const& remote_server_address) noexcept override {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    static_cast<void>(remote_server_address);
    sending_routing_controller_->OnStartOfferServiceInstance(service_instance_id);
  }

  /*!
   * \brief Inform the application about stoppage of a service instance offer.
   *
   * \details Implement the pure virtual method of the observer interface from the service discovery.
   *
   * \param[in] service_instance_id    The SOME/IP service instance id.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the function call.
   * - Notify the routing controller about stoppage of offer of the service instance.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void OnStopOfferServiceInstance(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept override {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    sending_routing_controller_->OnStopOfferServiceInstance(service_instance_id);
  }

  /*!
   * \brief Stop service discovery for all started but not yet stopped service discoveries of requested service
   * instances.
   *
   * \pre -
   * \context   Shutdown, Timer, App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Make a local copy of the container of started service discoveries
   * - For each entry within the container
   *   - Trigger StopServiceDiscovery (this leads to deletion of elements in the original container)
   * - Clear the original container
   * \endinternal
   */
  void StopAllServiceDiscoveries() {
    logger_.LogDebug(
        [this](ara::log::LogStream& s) noexcept {
          ServiceInstancesContainer::size_type const size{started_discoveries_of_requested_service_instances_.size()};

          s << "Number of requested services " << size;
        },
        static_cast<char const*>(__func__), __LINE__);

    ServiceInstancesContainer started_discoveries_of_requested_service_instances_copy{
        started_discoveries_of_requested_service_instances_};

    for (ServiceInstancesContainer::const_reference service_instance_id :
         started_discoveries_of_requested_service_instances_copy) {
      static_cast<void>(StopServiceDiscovery(service_instance_id));
    }

    started_discoveries_of_requested_service_instances_.clear();
  }

  /*!
   * \brief Release all allocated client resources.
   *
   * \pre -
   * \context   Shutdown, App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Erase all remaining local clients
   * \endinternal
   */
  void CleanUpClientResources() noexcept {
    logger_.LogDebug([](ara::log::LogStream& s) noexcept { s << "Cleaning up client resources"; },
                     static_cast<char const*>(__func__), __LINE__);

    local_clients_.clear();
  }

 private:
  /*!
   * \brief Container type for service instances.
   */
  using ServiceInstancesContainer = ara::core::Vector<configuration::types::SomeIpServiceInstanceId>;

  /*!
   * \brief Reference to identity and access management.
   */
  iam::IamInterface& identity_access_manager_;

  /*!
   * \brief The unique identity of the application.
   */
  amsr::ipc::Credentials application_identifier_;

  /*!
   * \brief The routing controller.
   * \details It must be a shared pointer because the packet router accepts only shared pointers.
   */
  std::shared_ptr<SendingRoutingControllerType> sending_routing_controller_;

  /*!
   * \brief Reference to the Required Service Instance Manager that manages remote offers.
   */
  client::RequiredServiceInstanceManager& required_service_instance_manager_;

  /*!
   * \brief Contains the list of currently started service discoveries of requested service instances.
   */
  ServiceInstancesContainer started_discoveries_of_requested_service_instances_{};

  /*!
   * \brief Map of local clients.
   */
  LocalClientMap& local_clients_;

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
    prefix_builder.Write(ara::core::StringView{"ApplicationClientHandler: "});
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_H_
