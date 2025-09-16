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
/**        \file  service_discovery_server.h
 *        \brief  Service Discovery Server Service Instance State Machine implementation
 *
 *      \details  This class creates and contains the SD server state machine. It receives all events and forwards them
 *to the state machine owner.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/server/eventgroup/event_manager.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/find_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/stop_subscribe_eventgroup_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_context.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_endpoint_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief ServiceDiscoveryServer.
 *
 * \trace SPEC-4981670
 */
class ServiceDiscoveryServer : public ServiceDiscoveryServerEndpointObserver, public ServiceDiscoveryServerInterface {
 public:
  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryServer() noexcept override = default;

  ServiceDiscoveryServer() = delete;
  ServiceDiscoveryServer(ServiceDiscoveryServer const&) = delete;
  ServiceDiscoveryServer(ServiceDiscoveryServer&&) = delete;
  ServiceDiscoveryServer& operator=(ServiceDiscoveryServer const&) & = delete;
  ServiceDiscoveryServer& operator=(ServiceDiscoveryServer&&) & = delete;

  /*!
   * \brief Constructor of ServiceDiscoveryServer.
   *
   * \param[in] service_id            A SOME/IP service identifier.
   * \param[in] instance_id           A SOME/IP service instance identifier.
   * \param[in] major_version         The major version of SOME/IP service instance.
   * \param[in] minor_version         The minor version of SOME/IP service instance.
   * \param[in] communication_type    The communication type.
   * \param[in] local_network_endpoint Network endpoint.
   * \param[in] timer_manager         A timer manager.
   * \param[in] message_scheduler     A message scheduler.
   * \param[in] config                A configuration.
   *
   * \pre          -
   * \context      Init
   * \steady       FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   *
   */
  ServiceDiscoveryServer(
      amsr::someip_protocol::internal::ServiceId const service_id,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someip_protocol::internal::MajorVersion const major_version,
      amsr::someip_protocol::internal::MinorVersion const minor_version,
      ServiceInstanceCommunicationType const communication_type,
      someip_daemon_core::configuration::types::RequiredNetworkEndpoint const& local_network_endpoint,
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      message::scheduler::SchedulerInterface* message_scheduler, configuration::Configuration const* config)
      : ServiceDiscoveryServerEndpointObserver{},
        ServiceDiscoveryServerInterface{},
        message_builder_{local_network_endpoint.address, config->GetIpcChannel().GetProvidedServiceInstance(
                                                             service_id, instance_id, major_version, minor_version)},
        event_manager_{
            timer_manager,
            local_network_endpoint.network,
            config,
            config->GetIpcChannel().GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version),
            communication_type,
            message_builder_,
            message_scheduler},
        service_instance_id_{{service_id, {major_version, minor_version}}, instance_id},
        context_{state_owner_,
                 config->GetIpcChannel()
                     .GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version)
                     ->service_discovery_,
                 message_builder_, message_scheduler} {
    state_owner_.SetContext(&context_);
  }

  /*!
   * \brief Called when network is brought up.
   *
   * \pre          -
   * \context      Init, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \internal
   * - Notify the state owner about the network being up.
   * \endinternal
   */
  void OnNetworkUp() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    state_owner_.OnNetworkUp();
  }

  /*!
   * \brief Called when a provided service instance is brought up.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \internal
   * - Notify the state owner about the service being up.
   * - Set service_up_ flag to true.
   * \endinternal
   */
  void OnServiceUp() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    state_owner_.OnServiceUp();
    service_up_ = true;
  }

  /*!
   * \brief Called when a provided service instance is brought down.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \internal
   * - Unsubscribe all eventgroup entries.
   * - Notify the state owner about the service being down.
   * - Set service_up_ flag to false.
   * \endinternal
   */
  void OnServiceDown() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    event_manager_.UnsubscribeAllEventgroupEntries();
    state_owner_.OnServiceDown();
    service_up_ = false;
  }

  /*!
   * \brief Get the event manager.
   *
   * \return A reference to the associated EventManager Instance.
   *
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::someip_daemon_core::server::eventgroup::EventManager& GetEventManager() noexcept { return event_manager_; }

  // ---- Calls from Remote services ----

  /*!
   * \brief Function to handle reboot detection for unicast and multicast SOME/IP SD message.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A port identifying sender's port.
   *
   * \context Network
   * \steady TRUE
   */
  void OnRebootDetected(someip_daemon_core::IpAddress const& from_address,
                        amsr::net::ip::Port const& from_port) noexcept override {
    // host at from_address was rebooted: discard from_port and undo all
    // event group subscriptions from this host
    static_cast<void>(from_port);
    event_manager_.UnsubscribeAllEventgroupsOnReboot(from_address);
  }

  /*!
   * \brief Function to handle reception of an FindService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The FindService entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady FALSE
   * \trace SPEC-10144713
   */
  bool OnFindServiceEntry(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                          message::entries::FindServiceEntry const& entry) noexcept override {
    bool entry_accepted{false};
    // Skip foreign entries
    if (IsMatchingFindServiceEntry(entry.entry_id)) {
      entry_accepted = true;
      state_owner_.OnFindServiceEntry(from_address, from_port);
    }
    return entry_accepted;
  }

  /*!
   * \brief Function to handle reception of an SubscribeEventgroup SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The SubscribeEventgroup entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady TRUE
   */
  bool OnSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& from_address,
                                  amsr::net::ip::Port const& from_port,
                                  message::entries::SubscribeEventgroupEntry const& entry) override {
    bool entry_accepted{false};
    // Skip foreign entries
    if (IsMatchingEventgroupEntry(entry.entry_id)) {
      entry_accepted = true;
      if (service_up_) {
        event_manager_.SubscribeEventgroupEntry(from_address, from_port, entry);
      }
    }
    return entry_accepted;
  }

  /*!
   * \brief Function to handle reception of an StopSubscribeEventgroup SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The StopSubscribeEventgroup entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \context Network
   * \steady FALSE
   */
  bool OnStopSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& from_address,
                                      amsr::net::ip::Port const& from_port,
                                      message::entries::StopSubscribeEventgroupEntry const& entry) override {
    bool entry_accepted{false};
    // Skip foreign entries
    if (IsMatchingEventgroupEntry(entry.entry_id)) {
      entry_accepted = true;
      if (service_up_) {
        event_manager_.UnSubscribeEventgroupEntry(from_address, from_port, entry);
      }
    }
    return entry_accepted;
  }

 protected:
  /*!
   * \brief Checks if the find service entry targets this state machine or not.
   * The following checks are done:
   *  - entry service id matches that of the state machine,
   *  - entry instance id matches that of the state machine or is set to kInstanceIdAll,
   * -  entry major version matches that of the state machine or is set to kMajorVersionAny
   *    Note: this MajorVersion check is required due to PRS_SOMEIPSD_00351
   *  - entry minor version matches that of the state machine or is set to kMinorVersionAny
   *
   * \param[in] entry_id An SD Entry ID.
   *
   * \pre          -
   * \context     Network
   * \steady      FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \return True if the entry matches this state machine; false otherwise.
   * \trace SPEC-10144600
   * \trace SPEC-4981675, SPEC-4981676, SPEC-4981677
   */
  bool IsMatchingFindServiceEntry(message::entries::ServiceEntryId const& entry_id) const {
    return (entry_id.service_id == service_instance_id_.deployment_id.service_interface_id) &&
           ((entry_id.instance_id == amsr::someip_protocol::internal::kInstanceIdAll) ||
            (entry_id.instance_id == service_instance_id_.instance_id)) &&
           // A FindService message with kMajorVersionAny is allowed
           ((entry_id.major_version == amsr::someip_protocol::internal::kMajorVersionAny) ||
            (entry_id.major_version == service_instance_id_.deployment_id.service_interface_version.major_version)) &&
           ((entry_id.minor_version == amsr::someip_protocol::internal::kMinorVersionAny) ||
            (entry_id.minor_version == service_instance_id_.deployment_id.service_interface_version.minor_version));
  }

  /*!
   * \brief Checks if the find service entry targets this state machine or not.
   * The following checks are done:
   *  - entry service id matches that of the state machine,
   *  - entry instance id matches that of the state machine or is set to kInstanceIdAll
   *
   * \param[in] entry_id An entries::EventgroupEntryId.
   *
   * \pre          -
   * \context     Network
   * \steady      TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \return True if the entry matches this state machine; false otherwise.
   * \trace SPEC-10144600
   * \trace SPEC-4981675, SPEC-4981676, SPEC-4981677
   */
  bool IsMatchingEventgroupEntry(message::entries::EventgroupEntryId const& entry_id) const {
    return (entry_id.service_id == service_instance_id_.deployment_id.service_interface_id) &&
           (entry_id.instance_id == service_instance_id_.instance_id) &&
           (entry_id.major_version == service_instance_id_.deployment_id.service_interface_version.major_version);
  }

  /*!
   * \brief     Get the state owner object reference.
   * \return    The state owner object reference
   * \pre       -
   * \reentrant False
   * \context   Testing
   * \steady    FALSE
   */
  ServiceDiscoveryServerStateOwner& GetStateOwner() { return state_owner_; }

 private:
  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] service_id     A SOME/IP service identifier.
   * \param[in] instance_id    A SOME/IP service instance identifier.
   * \param[in] major_version  A SOME/IP service major version.
   * \param[in] minor_version  A SOME/IP service minor version.
   *
   * \return A string representing the logger context identifier for given SOME/IP service parameters.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  static ara::core::String GetLoggerContextId(amsr::someip_protocol::internal::ServiceId const service_id,
                                              amsr::someip_protocol::internal::InstanceId const instance_id,
                                              amsr::someip_protocol::internal::MajorVersion const major_version,
                                              amsr::someip_protocol::internal::MinorVersion const minor_version) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};
    // VECTOR NL AutosarC++17_10-M8.4.4: MD_SomeIpDaemon_M8.4.4_FalsePositive
    prefix_builder << ::amsr::stream::SetShowBase << ::amsr::stream::Hex;
    prefix_builder << ara::core::StringView{"ServiceDiscoveryServer "};
    prefix_builder << ara::core::StringView{"(ServiceId: "} << service_id;
    prefix_builder << ara::core::StringView{", InstanceId: "} << instance_id;
    prefix_builder << ara::core::StringView{", MajorVersion: "} << major_version;
    prefix_builder << ara::core::StringView{", MinorVersion: "} << minor_version << ')';
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder message_builder_;
  /*!
   * \brief Event Subscription Manager.
   */
  amsr::someip_daemon_core::server::eventgroup::EventManager event_manager_;

  /*!
   * \brief A SOME/IP service interface deployment id.
   */
  configuration::types::SomeIpServiceInstanceId const service_instance_id_;

  /*!
   * \brief A container of available actions for ServiceDiscoveryServer.
   */
  ServiceDiscoveryServerContext context_;

  /*!
   * \brief State Pool.
   */
  ServiceDiscoveryServerStatePool state_pool_{};

  /*!
   * \brief State Machine Owner.
   */
  ServiceDiscoveryServerStateOwner state_owner_{state_pool_, service_instance_id_};

  /*!
   * \brief The status of the service
   */
  bool service_up_{false};

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      (GetLoggerContextId(service_instance_id_.deployment_id.service_interface_id, service_instance_id_.instance_id,
                          service_instance_id_.deployment_id.service_interface_version.major_version,
                          service_instance_id_.deployment_id.service_interface_version.minor_version))};
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_
