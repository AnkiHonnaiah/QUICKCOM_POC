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
/**        \file  service_discovery_client.h
 *        \brief  A service discovery client
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>

#include "amsr/iostream/io_format.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/address_validator/address_validator.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/configuration/util/configuration_types_utilities.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/entry_interpretation_error.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/stop_offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_ack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_nack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/remote_endpoints_validator_error.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/active_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine_context.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/offer_service_listener.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_observer.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief The StringView operator.
 */
using ::vac::container::literals::operator""_sv;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief A Service Discovery client.
 * \details Represents SD client. It processes and forwards SD messages to the client context state machines
 *          and maintains networks active status, service instance configuration,
 *          message scheduler, communication handling to provider service instance and current active offer.
 *
 * \tparam ServiceDiscoveryClientStateMachine A Service Discovery Client state machine.
 */
template <typename ServiceDiscoveryClientStateMachine = find_service::ServiceDiscoveryClientStateMachine>
class ServiceDiscoveryClient : public ServiceDiscoveryClientInterface,
                               public find_service::ServiceDiscoveryClientStateMachineContext {
 private:
  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

 public:
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  /*!
   * \brief Destructor.
   * \context Shutdown, Network
   * \steady FALSE
   */
  ~ServiceDiscoveryClient() noexcept { logger_.LogDebug(static_cast<char const*>(__func__), __LINE__); };

  ServiceDiscoveryClient() = delete;
  ServiceDiscoveryClient(ServiceDiscoveryClient const&) = delete;
  ServiceDiscoveryClient(ServiceDiscoveryClient&&) = delete;
  ServiceDiscoveryClient& operator=(ServiceDiscoveryClient const&) & = delete;
  ServiceDiscoveryClient& operator=(ServiceDiscoveryClient&&) & = delete;

  /*!
   * \brief Constructor of ServiceDiscoveryClient.
   *
   * \param[in] required_service_instance_deployment  A reference to the service interface deployment.
   * \param[in] instance_id                           A SOME/IP service instance identifier.
   * \param[in] sdclient_service_instance_config      A reference to the service discovery service instance config.
   * \param[in] network_endpoint                      A reference to the network endpoint.
   * \param[in] required_eventgroups                  Required eventgroups timing configuration. Used to access the list
   *                                                  of required eventgroups and find out what transport protocols are
   *                                                  required
   * \param[in] timer_manager                         A timer manager.
   * \param[in] message_scheduler                     A message scheduler.
   * \param[in] statistics_handler                    A reference to the statistics handler.
   *
   * \pre     -
   * \context Init, Network
   * \steady FALSE
   */
  ServiceDiscoveryClient(
      configuration::types::RequiredSomeIpServiceInstanceDeployment const& required_service_instance_deployment,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      configuration::types::SomeipSdClientServiceInstanceConfig const& sdclient_service_instance_config,
      configuration::types::RequiredNetworkEndpoint const& network_endpoint,
      RequiredEventgroupsContainer const& required_eventgroups,
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      message::scheduler::SchedulerInterface* message_scheduler,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept
      : ServiceDiscoveryClientInterface{},
        find_service::ServiceDiscoveryClientStateMachineContext{},
        required_service_instance_deployment_{required_service_instance_deployment},
        instance_id_{instance_id},
        sdclient_service_instance_config_{sdclient_service_instance_config},
        network_endpoint_{network_endpoint},
        timer_manager_{timer_manager},
        message_scheduler_{message_scheduler},
        statistics_handler_{statistics_handler},
        udp_endpoint_required_{configuration::util::ConfigurationTypesUtilities::IsTransportProtocolRequired(
            required_service_instance_deployment.communication_deployment, required_eventgroups,
            configuration::types::TransportProtocol::kUDP)},
        tcp_endpoint_required_{configuration::util::ConfigurationTypesUtilities::IsTransportProtocolRequired(
            required_service_instance_deployment.communication_deployment, required_eventgroups,
            configuration::types::TransportProtocol::kTCP)},
        offer_ttl_timer_{timer_manager->CreateTimer(
            [this]() {
              // Upon timer expiration, handle TTL expiry
              OnOfferTtlExpired();
            },
            amsr::steady_timer::MissedTimeoutBehavior::kDiscardMissedTimeouts)} {
    assert(offer_ttl_timer_ != nullptr);
  }

  /*!
   * \brief Register a listener for service offer updates.
   *
   * \param[in] offer_service_listener Listener. It will be notified upon service status changes.
   *
   * \pre -
   * \context Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterOfferServiceListener(OfferServiceListener* offer_service_listener) noexcept override {
    offer_service_listener_ = offer_service_listener;
  }

  /*!
   * \brief Unregister the listener.
   *
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterOfferServiceListener() noexcept override { offer_service_listener_ = nullptr; }

  /*!
   * \brief Register an observer to the Client State Machine.
   *
   * \param[in] instance_id  The ID of the RemoteServer.
   * \param[in] observer     The observer to the Client State Machine.
   *
   * \note The parameter instance_id is not necessary for this specific API implementation, but required by the
   *       interface for the Instance Id ALL use case implementation.
   *
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - Register the observer.
   * - If there is an active offer, notify the observer
   * \endinternal
   */
  void RegisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id,
                            ServiceDiscoveryClientObserver* const observer) noexcept override {
    // A specific instance ID must be used
    assert(instance_id != configuration::ConfigurationTypesAndDefs::kInstanceIdAll);
    // Double registration is not allowed
    assert(remote_server_ == nullptr);
    static_cast<void>(instance_id);
    remote_server_ = observer;

    // The remote server must get the initial state of the service
    if (active_offer_.has_value()) {
      remote_server_->OnOfferRemoteService(active_offer_.value());
    }
  }

  /*!
   * \brief Unregister the remote server.
   * \details This API is needed as RemoteServers are destroyed on-demand when no more proxies request the service,
   *          but SD will still be running, so we should avoid notifying an object that has been destroyed.
   *
   * \param[in] instance_id  The ID of the RemoteServer.
   *
   * \note The parameter instance_id is not necessary for this specific API implementation, but required by the
   *       interface for the Instance Id ALL use case implementation.
   *
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - Reset the observer pointer
   * \endinternal
   */
  void UnregisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id) noexcept override {
    assert(instance_id != configuration::ConfigurationTypesAndDefs::kInstanceIdAll);
    // Double unregistration is not allowed
    assert(remote_server_ != nullptr);
    static_cast<void>(instance_id);
    remote_server_ = nullptr;
  }

  // ---- Local Calls from SOME/IP Daemon --------------------------------------------------------------------

  /*!
   * \brief Called on service instance shutdown.
   *
   * \details Called from ServiceDiscovery.
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \pre     -
   *
   * \internal
   *  - Stop TTL expiry timer of OfferService.
   *  - Notify the state machine that we are no longer interested in the service.
   * \endinternal
   */
  void StopServiceDiscoveryClient() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);
    offer_ttl_timer_->Stop();
    state_machine_.OnServiceReleased();
  }

  /*!
   * \brief Called when network is brought up.
   *
   * \details Called from ServiceDiscovery.
   * \context   Network
   * \reentrant FALSE
   * \steady FALSE
   * \pre       -
   *
   * \internal
   *  - Set network status to active.
   *  - Activate client state machine network.
   * \endinternal
   */
  void OnNetworkUp() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);
    is_network_up_ = true;
    state_machine_.OnNetworkUp();
  }

  /*!
   * \brief Called when network is brought down.
   *
   * \details Called from ServiceDiscovery.
   * \context Network
   * \steady FALSE
   * \pre     -
   *
   * \internal
   *  - Set network status to deactive.
   *  - Invalidate the OfferService.
   *  - Stop OfferService TTL timer expiry.
   *  - Deactivate client state machine network.
   *  - Trigger OnNetworkDown on the state machine.
   *  - If offer service listener and active offer is valid
   *    - Trigger OnStopOfferService on the offer service listener.
   *  - Trigger OnStopOfferService on the remote server if registered.
   * \endinternal
   * \trace SPEC-10144857
   * \trace SPEC-10144720
   * \trace SPEC-10144751
   * \trace SPEC-10144761
   * \trace SPEC-10144763
   */
  void OnNetworkDown() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);
    is_network_up_ = false;
    offer_ttl_timer_->Stop();
    state_machine_.OnNetworkDown();

    if ((offer_service_listener_ != nullptr) && (active_offer_.has_value())) {
      offer_service_listener_->OnStopOfferService(instance_id_);
    }
    active_offer_.reset();
    if (remote_server_ != nullptr) {
      remote_server_->OnStopOfferRemoteService();
    }
  }

  // ---- Calls from Local applications ----------------------------------------------------------------------

  /*!
   * \brief Called when the service has been requested.
   *
   * \details Called from ServiceDiscovery.
   * \pre     Service has not been requested before.
   * \context App
   * \steady FALSE
   *
   * \internal
   *   - Forward the request to client state machine.
   * \endinternal
   */
  void RequestService() noexcept override {
    service_requested_ = true;
    state_machine_.OnServiceRequested();
  }

  /*!
   * \brief Called when the service has been released.
   *
   * \details Called from ServiceDiscovery.
   * \pre     Service has been requested before.
   * \context App
   * \steady FALSE
   *
   * \internal
   *   - Forward the service release to the client state machine.
   * \endinternal
   */
  void ReleaseService() noexcept override {
    service_requested_ = false;
    state_machine_.OnServiceReleased();
  }

  // ---- Calls from Remote services --------

  /*!
   * \brief Called on detection of reboot.
   *
   * \details Called from ServiceDiscovery to ALL state machines on reboot detection of a source.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A port identifying sender's port.
   * \pre     -
   * \context Network
   * \steady TRUE
   * \trace SPEC-10144488
   *
   * \internal
   * - If OfferService is valid,
   *   - Get the endpoint IP address and port of the active offer.
   *   - If endpoint IP address and port matching to the given IP address and port,
   *     - Invalidate the active offer.
   *     - Stop the TTL expiry timer.
   *     - Trigger StopOfferService on the state machine.
   *     - Trigger StopOfferService on the remote server if registered.
   * \endinternal
   */
  void OnRebootDetected(someip_daemon_core::IpAddress const& from_address,
                        amsr::net::ip::Port const& from_port) override {
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    logger_.LogVerbose(
        [&from_address, &from_port](ara::log::LogStream& s) {
          s << "Detected reboot from address :";
          s << from_address.MakeString().data();
          s << " Port :" << from_port.port;
        },
        static_cast<char const*>(__func__), __LINE__);

    // Reboot detection is only relevant if an offer is currently active
    if (active_offer_.has_value()) {
      someip_daemon_core::IpAddress const sd_address{active_offer_.value().sd_address};
      amsr::net::ip::Port const sd_port{active_offer_.value().sd_port};

      // Check if this state machine is connected to the rebooted device
      bool const is_same_address{sd_address == from_address};
      if ((sd_port.port == from_port.port) && (is_same_address)) {
        active_offer_.reset();
        offer_ttl_timer_->Stop();
        if (remote_server_ != nullptr) {
          remote_server_->OnStopOfferRemoteService();
        }
        if (offer_service_listener_ != nullptr) {
          offer_service_listener_->OnStopOfferService(instance_id_);
        }
      }
    }
  }

  /*!
   * \brief Function to handle reception of an OfferService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The OfferService entry
   * \param[in] is_multicast  A flag indicating whether the received SD message came from a multicast channel
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   * \pre     -
   * \context Network
   * \steady TRUE
   *
   * \internal
   * - Checks whether a service entry id matches to this client
   * - If new offer is received
   *   - Save offer
   *   - Trigger state machine
   *   - Trigger remote server if registered
   *   - If TTL is less than 0xFFFFFF (infinite)
   *     - Start TTL timer
   * - If offer was already received before
   *   - Check if offer matches with the previously received
   *   - Save offer
   *   - Trigger state machine
   *   - Trigger remote server if registered
   *   - If TTL is less than 0xFFFFFF (infinite)
   *   	 - Restart TTL timer
   *   - Otherwise, stop the current TTL timer
   * \endinternal
   *
   * \trace SPEC-10144879
   * \trace SPEC-10144880
   * \trace SPEC-10144884
   * \trace SPEC-10144718
   * \trace SPEC-10144719
   * \trace SPEC-10144721
   * \trace SPEC-10144715
   * \trace SPEC-10144721
   * \trace SPEC-10144726
   */
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  bool OnOfferServiceEntry(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                           message::entries::OfferServiceEntry const& entry, bool const is_multicast) override {
    bool entry_accepted{false};
    // Check if the received entry targets this state machine
    if (IsMatchingOfferServiceEntry(entry.entry_id)) {
      entry_accepted = true;

      // Check received endpoint options validity
      ara::core::Result<void> const endpoint_validity_check_result{
          ValidateTcp(entry.tcp_endpoint).AndThen([this, &entry]() { return ValidateUdp(entry.udp_endpoint); })};

      if (endpoint_validity_check_result.HasValue()) {
        if (!active_offer_.has_value()) {
          // New Offer is received
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          logger_.LogDebug(
              [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                s << "Accepted offer from <";
                s << from_address.MakeString().data();
                s << ", ";
                s << from_port.port;
                s << ">: (ServiceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.service_id);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.instance_id);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.major_version);
                s << ", MinorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.minor_version);
                s << ")";
              },
              static_cast<char const*>(__func__), __LINE__);

          active_offer_.emplace(ActiveOfferServiceEntry{from_address, from_port, entry, is_multicast});
          if (offer_service_listener_ != nullptr) {
            ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> tcp_endpoint{};
            ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> udp_endpoint{};

            if (entry.tcp_endpoint.has_value()) {
              configuration::types::Port const tcp_port{entry.tcp_endpoint.value().port.port};
              tcp_endpoint.emplace(amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress{
                  entry.tcp_endpoint.value().address, tcp_port});
            }
            if (entry.udp_endpoint.has_value()) {
              configuration::types::Port const udp_port{entry.udp_endpoint.value().port.port};
              udp_endpoint.emplace(amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress{
                  entry.udp_endpoint.value().address, udp_port});
            }
            amsr::someip_daemon_core::configuration::types::ServiceAddress const remote_server_address{tcp_endpoint,
                                                                                                       udp_endpoint};

            offer_service_listener_->OnOfferService(entry.entry_id.instance_id, remote_server_address);
          }
          state_machine_.OnOfferService();
          if (remote_server_ != nullptr) {
            remote_server_->OnOfferRemoteService(*active_offer_);
          }
          if (entry.ttl < configuration::ConfigurationTypesAndDefs::kTtlInfinite) {
            std::chrono::seconds const ttl_seconds{entry.ttl};
            amsr::steady_timer::Duration const ttl_duration{osabstraction::time::TimeStamp{ttl_seconds}};
            offer_ttl_timer_->Start(ara::core::Optional<amsr::steady_timer::Duration>(ttl_duration), {});
          }
        } else if (active_offer_.has_value()) {
          // Compare the received entry with the already registered one
          if (CompareOfferServiceEntries(from_address, from_port, entry.entry_id)) {
            // Offer renewal is received
            // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
            logger_.LogVerbose(
                [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                  s << "Offer renewal from <";
                  s << from_address.MakeString().data();
                  s << ", " << from_port.port;
                  s << ">: (ServiceId: 0x";
                  s << ara::log::HexFormat(entry.entry_id.service_id);
                  s << ", InstanceId: 0x";
                  s << ara::log::HexFormat(entry.entry_id.instance_id);
                  s << ", MajorVersion: 0x";
                  s << ara::log::HexFormat(entry.entry_id.major_version);
                  s << ", MinorVersion: 0x";
                  s << ara::log::HexFormat(entry.entry_id.minor_version);
                  s << ")";
                },
                static_cast<char const*>(__func__), __LINE__);

            active_offer_.emplace(ActiveOfferServiceEntry{from_address, from_port, entry, is_multicast});
            state_machine_.OnOfferService();
            if (remote_server_ != nullptr) {
              remote_server_->OnOfferRenewal(is_multicast);
            }

            if (entry.ttl < configuration::ConfigurationTypesAndDefs::kTtlInfinite) {
              // Re-start the timer
              std::chrono::seconds const ttl_seconds{entry.ttl};
              amsr::steady_timer::Duration const ttl_duration{osabstraction::time::TimeStamp{ttl_seconds}};
              offer_ttl_timer_->Start(ara::core::Optional<amsr::steady_timer::Duration>(ttl_duration), {});
            } else {
              // In case the renewed offer comes with an infinite TTL, we shall not re-start the timer but we need to
              // stop it in case the previous offer had a finite TTL and the timer is still running, to avoid it to
              // expire. In case the previous offer already had infinite TTL and no timer was running, stopping the
              // timer won't harm.
              offer_ttl_timer_->Stop();
            }
          } else {
            // Ignore non-matching offer
            // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
            logger_.LogVerbose(
                [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                  s << "Non-matching offer from <";
                  s << from_address.MakeString().data();
                  s << ", ";
                  s << from_port.port;
                  s << ">: (ServiceId: 0x";
                  s << ara::log::HexFormat(entry.entry_id.service_id);
                  s << ", InstanceId: 0x";
                  s << ara::log::HexFormat(entry.entry_id.instance_id);
                  s << ", MajorVersion: 0x";
                  s << ara::log::HexFormat(entry.entry_id.major_version);
                  s << ", MinorVersion: 0x";
                  s << ara::log::HexFormat(entry.entry_id.minor_version);
                  s << ") is ignored.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
        } else {
          // Ignore non-matching offer
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          logger_.LogVerbose(
              [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                s << "Non-matching offer from <";
                s << from_address.MakeString().data();
                s << ", ";
                s << from_port.port;
                s << ">: (ServiceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.service_id);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.instance_id);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.major_version);
                s << ", MinorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.minor_version);
                s << ") is ignored.";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      } else {
        // Ignore offer from different subnet
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
        logger_.LogDebug(
            [&from_address, &from_port, &entry, &endpoint_validity_check_result](ara::log::LogStream& s) {
              s << "IP endpoint option validation error in offer from <";
              s << from_address.MakeString().data();
              s << ", ";
              s << from_port.port;
              s << ">.";
              if (entry.tcp_endpoint.has_value()) {
                s << " TCP address: ";
                s << entry.tcp_endpoint.value().address.MakeString().data();
                s << ".";
              }
              if (entry.udp_endpoint.has_value()) {
                s << " UDP address: ";
                s << entry.udp_endpoint.value().address.MakeString().data();
              }
              s << ". ";
              s << endpoint_validity_check_result.Error().Message();
            },
            static_cast<char const*>(__func__), __LINE__);

        // Report invalid offer service entry
        ara::core::String const from_address_str{from_address.ToString()};
        statistics_handler_.ReportInvalidOfferServiceEntry(entry.entry_id.service_id, entry.entry_id.instance_id,
                                                           from_address_str, from_port.port);
      }
    }
    return entry_accepted;
  }

  /*!
   * \brief Function to handle reception of a StopOfferService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The StopOfferService entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   * \pre     -
   * \context Network
   * \steady FALSE
   *
   * \internal
   * - Checks whether a service entry id matches to this client
   * - If offer was already received before
   *   - Check if offer matches with the previously received
   *   - Delete local copy of active offer
   *   - Trigger state machine
   *   - Trigger remote server if registered
   *   - Stop TTL timer
   * \endinternal
   * \trace SPEC-10144857
   * \trace SPEC-10144720
   * \trace SPEC-10144751
   * \trace SPEC-10144761
   * \trace SPEC-10144763
   */
  bool OnStopOfferServiceEntry(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                               message::entries::StopOfferServiceEntry const& entry) override {
    bool entry_accepted{false};
    // Check if the received entry targets this state machine
    if (IsMatchingOfferServiceEntry(entry.entry_id)) {
      entry_accepted = true;
      if (active_offer_.has_value()) {
        // Compare the received entry with the already registered one
        if (CompareOfferServiceEntries(from_address, from_port, entry.entry_id)) {
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          logger_.LogDebug(
              [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                s << "Stopping offer from <";
                s << from_address.MakeString().data();
                s << ", ";
                s << from_port.port;
                s << ">: (ServiceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.service_id);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.instance_id);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.major_version);
                s << ", MinorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.minor_version);
                s << ")";
              },
              static_cast<char const*>(__func__), __LINE__);

          active_offer_.reset();
          if (offer_service_listener_ != nullptr) {
            offer_service_listener_->OnStopOfferService(entry.entry_id.instance_id);
          }
          if (remote_server_ != nullptr) {
            remote_server_->OnStopOfferRemoteService();
          }
          offer_ttl_timer_->Stop();
        } else {
          // Non-matching stop offer -> Ignore
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          logger_.LogVerbose(
              [&from_address, &from_port, &entry](ara::log::LogStream& s) {
                s << "Non-matching stop offer from <";
                s << from_address.MakeString().data();
                s << ", " << from_port.port;
                s << ">: (ServiceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.service_id);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(entry.entry_id.instance_id);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(entry.entry_id.major_version);
                s << ", MinorVersion: 0x" << ara::log::HexFormat(entry.entry_id.minor_version);
                s << ") is ignored.";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      }
    } else {
      // Ignore non-matching offer
      // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
      logger_.LogVerbose(
          [&from_address, &from_port, &entry](ara::log::LogStream& s) {
            s << "Non-matching offer from <";
            s << from_address.MakeString().data();
            s << ", ";
            s << from_port.port;
            s << ">: (ServiceId: 0x";
            s << ara::log::HexFormat(entry.entry_id.service_id);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(entry.entry_id.instance_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(entry.entry_id.major_version);
            s << ", MinorVersion: 0x";
            s << ara::log::HexFormat(entry.entry_id.minor_version);
            s << ") is ignored.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return entry_accepted;
  }

  /*!
   * \brief Function to handle reception of an SubscribeEventgroupAck SOME/IP SD entry.
   *
   * \param[in] entry  The SubscribeEventgroupAck entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   * \pre     -
   * \context Network
   * \steady TRUE
   *
   * \internal
   * - Checks whether a service entry id matches to this client
   * - If there is a registered remote server
   *   - Notify the remote server
   * \endinternal
   * \trace SPEC-10144863
   */
  bool OnSubscribeEventgroupAckEntry(message::entries::SubscribeEventgroupAckEntry const& entry) override {
    // Skip foreign entries
    bool entry_accepted{false};
    if (IsMatchingSubscribeEventgroupEntry(entry.entry_id)) {
      entry_accepted = true;
      if (remote_server_ != nullptr) {
        if (entry.multicast_endpoint.has_value()) {
          someip_daemon_core::IpAddress const address{entry.multicast_endpoint.value().address};
          amsr::someip_protocol::internal::Port const port{entry.multicast_endpoint.value().port.port};
          remote_server_->OnSubscribeEventgroupAck(entry.entry_id.eventgroup_id,
                                                   ara::core::Optional<someip_daemon_core::IpAddress>{address}, port);

        } else {
          remote_server_->OnSubscribeEventgroupAck(entry.entry_id.eventgroup_id,
                                                   ara::core::Optional<someip_daemon_core::IpAddress>{},
                                                   configuration::Configuration::kInvalidPort);
        }
      }
    }
    return entry_accepted;
  }

  /*!
   * \brief Function to handle reception of an SubscribeEventgroupNAck SOME/IP SD entry.
   *
   * \param[in] entry The SubscribeEventgroupNAck entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   * \pre     -
   * \context Network
   * \steady TRUE
   *
   * \internal
   * - Checks whether a service entry id matches to this client
   * - If there is a registered remote server
   *   - Trigger remote server
   * \endinternal
   */
  bool OnSubscribeEventgroupNackEntry(message::entries::SubscribeEventgroupNAckEntry const& entry) override {
    // Skip foreign entries
    bool entry_accepted{false};
    if (IsMatchingSubscribeEventgroupEntry(entry.entry_id)) {
      entry_accepted = true;
      if (remote_server_ != nullptr) {
        remote_server_->OnSubscribeEventgroupNack(entry.entry_id.eventgroup_id);
      }
    }
    return entry_accepted;
  }

  /*!
   * \brief Checks the availability of the service.
   *
   * \return true if the service is available (offered, and TTL is not expired), and false otherwise.
   * \pre     -
   * \context App
   * \steady TRUE
   */
  bool IsServiceAvailable() const override { return active_offer_.has_value(); }

  // ---------------------------------------------------------------------------------------------------------

 protected:
  /*!
   * \brief Indicates whether network is active or not.
   *
   * \return true if network is active and false otherwise.
   * \pre     -
   * \context App
   * \steady TRUE
   */
  bool IsNetworkUp() const override { return is_network_up_; }

  /*!
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  bool IsServiceRequested() const override { return service_requested_; }

  /*!
   * \brief Transmits a FindService entry.
   * \pre     -
   * \context App
   * \steady FALSE
   * \trace SPEC-4980067
   *
   * \internal
   * - Create minimum and maximum delay to schedule the entry.
   * - Schedule the FindService entry message for transmission.
   * \endinternal
   */
  void SendFindService() override {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    std::chrono::nanoseconds const min_delay{std::chrono::nanoseconds::zero()};
    std::chrono::nanoseconds const max_delay{std::chrono::nanoseconds::zero()};
    message_scheduler_->ScheduleFindServiceEntry(find_service_entry_, min_delay, max_delay);
  }

 private:
  /*!
   * \brief     Notifies that the TTL of the current offer service has expired.
   * \context   Timer
   * \reentrant False
   * \steady    FALSE
   * \pre     -
   *
   * \internal
   * - Invalidate the current active offer.
   * - Notify current offer is expired to the client state machine.
   * - Trigger StopOfferService to the remote server if registered.
   * \endinternal
   * \trace SPEC-10144857, SPEC-10144860, SPEC-4663490
   */
  void OnOfferTtlExpired() noexcept {
    logger_.LogWarn(
        [](ara::log::LogStream& s) { s << "Offer service TTL has expired. The service is considered stopped."; },
        static_cast<char const*>(__func__), __LINE__);

    assert(active_offer_.value().offer_entry.ttl != configuration::ConfigurationTypesAndDefs::kTtlInfinite);
    active_offer_.reset();

    if (offer_service_listener_ != nullptr) {
      offer_service_listener_->OnStopOfferService(instance_id_);
    }
    state_machine_.OnOfferTtlExpired();
    if (remote_server_ != nullptr) {
      remote_server_->OnStopOfferRemoteService();
    }
  }

  /*!
   * \brief Checks whether an Offer Service entry id matches to this client
   *
   * \param[in] entry_id The received entry id
   * \return    true if the entry id matches to this client
   *
   * \pre       -
   * \context   Network
   * \steady TRUE
   * \trace SPEC-10144600
   * \trace SPEC-4981675, SPEC-4981676, SPEC-4981677
   *
   * \internal
   * - Check if the received entry matches the configuration parameters of this state machine: service id, major version
   *   and instance id.
   * - If they match
   *   - For kMinimumMinorVersion find behavior
   *     - Return true if the received minor version is equal or greater than the configured one.
   *     - Otherwise return false
   *   - For kExactOrAnyMinorVersion find behavior
   *     - If minor version ANY is configured, return true
   *     - If a specific minor version is configured, return true if the configured minor version matches exactly with
   *       the received one. Return false otherwise.
   * - Otherwise return false
   */
  bool IsMatchingOfferServiceEntry(message::entries::ServiceEntryId const entry_id) const noexcept {
    // Check [ServiceId, MajorVersion, InstanceId] which must be a perfect match
    bool accept_entry{
        (entry_id.service_id == required_service_instance_deployment_.deployment_id.service_interface_id) &&
        (entry_id.instance_id == instance_id_) &&
        (entry_id.major_version ==
         required_service_instance_deployment_.deployment_id.service_interface_version.major_version)};

    // If [ServiceId, MajorVersion, InstanceId] match, check MinorVersion based on the search criteria
    if (accept_entry) {
      amsr::someip_protocol::internal::MinorVersion const configured_minor_version{
          required_service_instance_deployment_.deployment_id.service_interface_version.minor_version
              .GetMinorVersion()};
      // For kMinimumMinorVersion, accept any entry with a minor version equal or greater than the configured one
      if (required_service_instance_deployment_.deployment_id.service_interface_version.minor_version
              .GetVersionDrivenFindBehavior() ==
          configuration::ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kMinimumMinorVersion) {
        accept_entry = entry_id.minor_version >= configured_minor_version;
      } else {  // For kExactOrAnyMinorVersion
        // If the configured minor version is ANY, accept this entry
        if (configured_minor_version == amsr::someip_protocol::internal::kMinorVersionAny) {
          accept_entry = true;
        } else {  // Otherwise, accept only an entry that matches exactly with the configured minor version
          accept_entry = entry_id.minor_version == configured_minor_version;
        }
      }
    }

    return accept_entry;
  }

  /*!
   * \brief Checks whether a subscribe eventrgroup entry id matches to this client
   *
   * \param[in] entry_id The received entry id
   * \return    true if the entry id matches to this client
   *
   * \pre       -
   * \context   Network
   * \steady TRUE
   * \trace SPEC-10144600
   * \trace SPEC-4981675, SPEC-4981676, SPEC-4981677
   */
  bool IsMatchingSubscribeEventgroupEntry(message::entries::EventgroupEntryId const entry_id) const noexcept {
    return ((entry_id.service_id == required_service_instance_deployment_.deployment_id.service_interface_id) &&
            (entry_id.instance_id == instance_id_) &&
            (entry_id.major_version ==
             required_service_instance_deployment_.deployment_id.service_interface_version.major_version));
  }

  /*!
   * \brief Checks if the given offer matches the current one.
   *
   * \param[in] sd_address  Service discover address.
   * \param[in] sd_port     Service discover port.
   * \param[in] entry_id    Entry id.
   *
   * \return true if the address/port pair matches, as well as all of service id, instance id, major version and minor
   * version; false otherwise.
   *
   * \pre     An offer must be active
   * \context App
   * \steady TRUE
   *
   * \internal
   * - Compare each member element of offer and return true if all are equal, otherwise, return false
   * \endinternal
   */
  bool CompareOfferServiceEntries(someip_daemon_core::IpAddress const& sd_address, amsr::net::ip::Port const& sd_port,
                                  message::entries::ServiceEntryId const entry_id) const noexcept {
    assert(active_offer_.has_value());
    ActiveOfferServiceEntry const& active_offer{active_offer_.value()};
    message::entries::ServiceEntryId const& active_entry_id{active_offer.offer_entry.entry_id};
    return ((active_offer.sd_address == sd_address) && (active_offer.sd_port.port == sd_port.port) &&
            (active_entry_id.service_id == entry_id.service_id) &&
            (active_entry_id.instance_id == entry_id.instance_id) &&
            (active_entry_id.major_version == entry_id.major_version) &&
            (active_entry_id.minor_version == entry_id.minor_version));
  }

  /*!
   * \brief Checks the consistency of a received UDP IP endpoint option if required by the configuration.
   *
   * \param[in] udp_endpoint  An optional UDP IP endpoint option
   *
   * \return An empty optional if validation succeeds or an appropriate error code
   *
   * \pre     -
   * \context Network
   * \steady TRUE
   *
   * \internal
   * - If a UDP endpoint is required
   *   - If the given endpoint contains a value
   *     - Check if the referenced endpoint belongs to our subnet (sanity check)
   *     - If address invalid, return kAddressNotInSubnet
   *   - If required but not referenced, return UdpRequiredButNotReferenced
   * - If all checks pass, return an empty result
   * \endinternal
   */
  ara::core::Result<void> ValidateUdp(
      ara::core::Optional<message::options::IpEndpointOption> const& udp_endpoint) const noexcept {
    ara::core::Result<void> validation_result{};

    // Check if a UDP endpoint is required
    if (udp_endpoint_required_) {
      // Check if the endpoint has been referenced
      if (udp_endpoint.has_value()) {
        // Check if the referenced endpoint belongs to our subnet
        bool const address_valid{address_validator_.Validate(udp_endpoint.value().address)};
        if (!address_valid) {
          validation_result.EmplaceError(message::RemoteEndpointsValidatorError::kAddressNotInSubnet);
        }
      } else {
        validation_result.EmplaceError(message::RemoteEndpointsValidatorError::kUdpRequiredButNotReferenced);
      }
    }

    return validation_result;
  }

  /*!
   * \brief Checks the consistency of a received TCP IP endpoint option if required by the configuration.
   *
   * \param[in] tcp_endpoint  An optional TCP IP endpoint option
   *
   * \return An empty optional if validation succeeds or an appropriate error code
   *
   * \pre     -
   * \context Network
   * \steady TRUE
   *
   * \internal
   * - If a TCP endpoint is required
   *   - If the given endpoint contains a value
   *     - Check if the referenced endpoint belongs to our subnet (sanity check)
   *     - If address invalid, return kAddressNotInSubnet
   *   - If required but not referenced, return TcpRequiredButNotReferenced
   * - If all checks pass, return an empty result
   * \endinternal
   */
  ara::core::Result<void> ValidateTcp(
      ara::core::Optional<message::options::IpEndpointOption> const& tcp_endpoint) const noexcept {
    ara::core::Result<void> validation_result{};

    // Check if a UDP endpoint is required
    if (tcp_endpoint_required_) {
      // Check if the endpoint has been referenced
      if (tcp_endpoint.has_value()) {
        // Check if the referenced endpoint belongs to our subnet
        bool const address_valid{address_validator_.Validate(tcp_endpoint.value().address)};
        if (!address_valid) {
          validation_result.EmplaceError(message::RemoteEndpointsValidatorError::kAddressNotInSubnet);
        }
      } else {
        validation_result.EmplaceError(message::RemoteEndpointsValidatorError::kTcpRequiredButNotReferenced);
      }
    }

    return validation_result;
  }

  /*!
   * \brief The service deployment configuration
   */
  configuration::types::RequiredSomeIpServiceInstanceDeployment const required_service_instance_deployment_;

  /*!
   * \brief A SOME/IP service instance identifier.
   */
  amsr::someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief A SomeIP Service discovery client service instance config.
   */
  configuration::types::SomeipSdClientServiceInstanceConfig const sdclient_service_instance_config_;

  /*!
   * \brief A SomeIP Service discovery Network Endpoint.
   */
  configuration::types::RequiredNetworkEndpoint const network_endpoint_;

  /*!
   * \brief A service instance responsible for communication handling.
   */
  ServiceDiscoveryClientObserver* remote_server_{nullptr};

  /*!
   * \brief A timer manager.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief Currently active service offer.
   */
  ara::core::Optional<ActiveOfferServiceEntry> active_offer_{};

  /*!
   * \brief Indicates whether the service is requested.
   */
  bool service_requested_{false};

  /*!
   * \brief Indicates whether network is active or not.
   */
  bool is_network_up_{false};

  /*!
   * \brief ServiceDiscoveryClient Context, that receives service offer updates.
   */
  OfferServiceListener* offer_service_listener_{nullptr};

  /*!
   * \brief A message scheduler.
   */
  message::scheduler::SchedulerInterface* message_scheduler_;

  /*!
   * \brief A statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief Indication of whether a UDP endpoint is required in the configuration
   *        by the required service instance using this SD client
   */
  bool const udp_endpoint_required_;

  /*!
   * \brief Indication of whether a TCP endpoint is required in the configuration
   *        by the required service instance using this SD client
   */
  bool const tcp_endpoint_required_;

  /*!
   * \brief Timer to track Offer TTL expiry.
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> offer_ttl_timer_;

  /*!
   * \brief IP address validator
   */
  address_validator::AddressValidator const address_validator_{network_endpoint_.network};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "ServiceDiscoveryClient"_sv, required_service_instance_deployment_.deployment_id, instance_id_)};

  /*!
   * \brief    The Minimum Version that is used in the FindService entry.
   * \details  In case kExactOrAnyMinorVersion is configured, the configured minor version (which could be exact or
   *           any) shall be used here, but if kMinimumMinorVersion is configured, then we shall use always the wild
   *           card ANY in the FindService message (SWS_CM_00202).
   */
  amsr::someip_protocol::internal::MinorVersion const find_service_minor_version_{
      (required_service_instance_deployment_.deployment_id.service_interface_version.minor_version
           .GetVersionDrivenFindBehavior() ==
       configuration::ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kMinimumMinorVersion)
          ? amsr::someip_protocol::internal::kMinorVersionAny
          : required_service_instance_deployment_.deployment_id.service_interface_version.minor_version
                .GetMinorVersion()};

  /*!
   * \brief A Find Service Entry instance.
   */
  message::entries::FindServiceEntry const find_service_entry_{
      message::entries::ServiceEntryId{
          required_service_instance_deployment_.deployment_id.service_interface_id, instance_id_,
          required_service_instance_deployment_.deployment_id.service_interface_version.major_version,
          find_service_minor_version_},
      sdclient_service_instance_config_.ttl};

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief A state machine responsible for finding a matching service provider.
   */
  ServiceDiscoveryClientStateMachine state_machine_{
      this, timer_manager_,
      sdclient_service_instance_config_.sd_config.value_or(configuration::types::InitialSdDelayConfig{}),
      required_service_instance_deployment_.deployment_id, instance_id_};
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_
