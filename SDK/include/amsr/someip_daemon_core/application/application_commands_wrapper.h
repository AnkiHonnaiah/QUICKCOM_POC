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
/*!        \file  application_commands_wrapper.h
 *        \brief  Application commands wrapper
 *
 *      \details  This class wraps all commands that can be called by an application in order for them to be executed by
 *                the daemon, when a command is called by an application a corresponding function will be executed by
 *                the daemon and the result of that execution is then remapped to a different error domain before it is
 *                returned back to the requestor application.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_COMMANDS_WRAPPER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_COMMANDS_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <utility>
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/application/application_client_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_server_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_version.h"
#include "someipd_app_protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Shorting for namespace amsr::someipd_app_protocol::internal.
 */
namespace someipd_app_protocol = ::amsr::someipd_app_protocol::internal;

/*!
 * \brief Application Commands Wrapper.
 *
 */
class ApplicationCommandsWrapper final {
 public:
  /*!
   * \brief Type alias for "ApplicationServerHandler".
   */
  using ApplicationServerHandlerType = ApplicationServerHandlerInterface;

  /*!
   * \brief Type alias for "ApplicationClientHandler".
   */
  using ApplicationClientHandlerType = ApplicationClientHandlerInterface;

  /*!
   * \brief Type alias for "ControlMessageReturnCode".
   */
  using ControlMessageReturnCode = ::amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Type-alias for general payload for control messages
   */
  using ServiceDiscoveryControlMessagePayload = amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData;

  /*!
   * \brief Type alias for "ApplicationOperationResult".
   */
  using ApplicationOperationResult = OperationResult;

  /*!
   * \brief Type alias for "RequestLocalServer" operation return code.
   */
  using ApplicationRequestLocalServerOperationResult =
      typename ApplicationServerHandlerType::RequestLocalServerOperationResult;

  /*!
   * \brief Constructor.
   *
   * \param[in] application_client_handler A reference to the application client handler.
   * \param[in] application_server_handler A reference to the application server handler.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  ApplicationCommandsWrapper(ApplicationClientHandlerType& application_client_handler,
                             ApplicationServerHandlerType& application_server_handler)
      : application_client_handler_(application_client_handler),
        application_server_handler_(application_server_handler) {}

  /*!
   * \brief  Default destructor.
   * \steady FALSE
   */
  ~ApplicationCommandsWrapper() = default;

  ApplicationCommandsWrapper(ApplicationCommandsWrapper const&) = delete;
  ApplicationCommandsWrapper(ApplicationCommandsWrapper const&&) = delete;
  auto operator=(ApplicationCommandsWrapper const&) -> ApplicationCommandsWrapper& = delete;
  auto operator=(ApplicationCommandsWrapper const&&) -> ApplicationCommandsWrapper& = delete;

  // ---- Control logic ------------------------------------------------------------------------------------

  /*!
   * \brief Request a service with the given ID.
   *
   * \param[in] request Struct containing service/instance ID and major/minor version of the requested service.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return The newly allocated SOME/IP client identifier if the operation is successful and error otherwise.
   * \error SomeIpDaemonErrc::access_denied If the IAM does not grant the permission to request a service.
   * \error SomeIpDaemonErrc::client_ids_overflow Local client Id generator reaches the maximum number of simultaneous
   *        clients.
   * \error SomeIpDaemonErrc::remote_server_not_found If no remote server is found.
   * \error SomeIpDaemonErrc::required_service_instance_not_found If no required service instance is found.
   *
   */
  ara::core::Result<someip_protocol::internal::ClientId> RequestService(
      amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData const& request) noexcept {
    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};
    return application_client_handler_.RequestService(service_instance_id);
  }

  /*!
   * \brief Release the requested client.
   *
   * \param[in] request Struct containing client ID to be released.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if successfully released, otherwise "kNotOk".
   *
   * \internal
   * - Release the client with the given ID.
   * - If the client could not be released,
   *   - Set the return code to indicate failure.
   * - Return the result.
   * \endinternal
   */
  ControlMessageReturnCode ReleaseService(
      amsr::someipd_app_protocol::internal::ControlMessageReleaseServicePayload const& request) {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};
    configuration::types::SomeIpServiceInstanceId const& service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};
    bool const success{application_client_handler_.ReleaseService(service_instance_id, request.client_id)};
    if (!success) {
      result = ControlMessageReturnCode::kNotOk;
    }

    return result;
  }

  /*!
   * \brief Request the local server.
   *
   * \param[in] request Struct containing service instance ID to be requested.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if the service is successfully offered, otherwise an error code.
   *
   * \internal
   * - Request the local server.
   * - Map the return error to a control message return code
   * - Return the status code.
   * \endinternal
   */
  ControlMessageReturnCode RequestLocalServer(ServiceDiscoveryControlMessagePayload const& request) noexcept {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        {request.service_id, configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    ApplicationRequestLocalServerOperationResult const operation_result{
        application_server_handler_.RequestLocalServer(service_instance_id)};

    switch (operation_result.result_) {
      case ApplicationOperationResult::kOk: {
        result = ControlMessageReturnCode::kOk;
        break;
      }
      case ApplicationOperationResult::kFailure: {
        result = ControlMessageReturnCode::kLocalServerNotAvailable;
        break;
      }
      case ApplicationOperationResult::kAccessDenied: {
        result = ControlMessageReturnCode::kOfferServiceAccessDenied;
        break;
      }
      case ApplicationOperationResult::kConfigurationFailure: {
        result = ControlMessageReturnCode::kNotOk;
        break;
      }
      default:
        // Not valid
        break;
    }

    return result;
  }

  /*!
   * \brief Release the local server.
   *
   * \param[in] request Struct containing service instance ID of the server to be released.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Release the local server.
   * \endinternal
   */
  void ReleaseLocalServer(ServiceDiscoveryControlMessagePayload const& request) noexcept {
    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        {request.service_id, configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    application_server_handler_.ReleaseLocalServer(service_instance_id);
  }

  /*!
   * \brief Offer a service.
   *
   * \param[in] request Struct containing service instance ID to be offered.
   *
   * \pre -
   * \context  App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Offer the service.
   * \endinternal
   */
  void OfferService(ServiceDiscoveryControlMessagePayload const& request) noexcept {
    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        {request.service_id, configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    application_server_handler_.OfferService(service_instance_id);
  }

  /*!
   * \brief Stop offering a service.
   *
   * \param[in] request Struct containing service instance ID of the offer to be stopped.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Stop offering the service.
   * \endinternal
   */
  void StopOfferService(ServiceDiscoveryControlMessagePayload const& request) noexcept {
    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        {request.service_id, configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    application_server_handler_.StopOfferService(service_instance_id);
  }

  /*!
   * \brief Subscribe to an event.
   *
   * \param[in] request Struct containing service/instance/event ID of the event to be subscribed.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if the "SubscribeEvent" request is successfully handled, otherwise an error code.
   *
   * \internal
   * - Subscribe to the requested event.
   * - If it is not possible to subscribe,
   *   - Set the return code to indicate failure.
   * - Return the result.
   * \endinternal
   */
  ControlMessageReturnCode SubscribeEvent(
      amsr::someipd_app_protocol::internal::EventControlMessagePayload const& request) {
    ControlMessageReturnCode result{ControlMessageReturnCode::kNotOk};

    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    ApplicationOperationResult const application_operation_result{
        application_client_handler_.SubscribeEvent(service_instance_id, request.event_id, request.client_id)};

    if (application_operation_result == ApplicationOperationResult::kOk) {
      result = ControlMessageReturnCode::kOk;
    }

    return result;
  }

  /*!
   * \brief UnsubscribeEvent control message handler.
   *
   * \param[in] request Struct containing service/instance/event ID of the event to be unsubscribed.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if the "UnsubscribeEvent" request is successfully handled, otherwise an error code.
   *
   * \internal
   * - Unsubscribe to the requested event.
   * - If it is not possible to unsubscribe,
   *   - Set the return code to indicate failure.
   * - Return the result.
   * \endinternal
   */
  ControlMessageReturnCode UnsubscribeEvent(
      amsr::someipd_app_protocol::internal::EventControlMessagePayload const& request) {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    ApplicationOperationResult const unsubscribe_event_result{
        application_client_handler_.UnsubscribeEvent(service_instance_id, request.event_id, request.client_id)};
    if (unsubscribe_event_result != ApplicationOperationResult::kOk) {
      result = ControlMessageReturnCode::kNotOk;
    }

    return result;
  }

  /*!
   * \brief Start service discovery of the given service with the given ID.
   *
   * \param[in] request Struct containing service/instance ID of the requested service.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if the "StartServiceDiscovery" request is successfully handled, otherwise an error code.
   *
   * \internal
   * - Start service discovery of the given service.
   * - If the request fails,
   *   - Set the return code to indicate failure.
   * - Return the result.
   * \endinternal
   */
  ControlMessageReturnCode StartServiceDiscovery(ServiceDiscoveryControlMessagePayload const& request) {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    bool const success{application_client_handler_.StartServiceDiscovery(service_instance_id)};
    if (!success) {
      result = ControlMessageReturnCode::kNotOk;
    }

    return result;
  }

  /*!
   * \brief Stop service discovery of the given service.
   *
   * \param[in] request Struct containing service/instance ID of the service.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return "kOk" if the "StopServiceDiscovery" request is successfully handled, otherwise an error code.
   *
   * \internal
   * - Stop service discovery of the given service.
   * - If the stop fails,
   *   - Set the return code to indicate failure.
   * - Return the result.
   * \endinternal
   */
  ControlMessageReturnCode StopServiceDiscovery(ServiceDiscoveryControlMessagePayload const& request) noexcept {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    configuration::types::SomeIpServiceInstanceId const service_instance_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            request.service_id,
            configuration::types::SomeIpServiceVersion{request.major_version, request.minor_version}},
        request.instance_id};

    bool const success{application_client_handler_.StopServiceDiscovery(service_instance_id)};
    if (!success) {
      result = ControlMessageReturnCode::kNotOk;
    }

    return result;
  }

 private:
  /*!
   * \brief Application client handler.
   */
  ApplicationClientHandlerType& application_client_handler_;

  /*!
   * \brief Application server handler.
   */
  ApplicationServerHandlerType& application_server_handler_;
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_COMMANDS_WRAPPER_H_
