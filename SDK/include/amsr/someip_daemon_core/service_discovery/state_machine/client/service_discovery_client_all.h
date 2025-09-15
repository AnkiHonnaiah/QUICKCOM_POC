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
/**        \file  service_discovery_client_all.h
 *        \brief  A service discovery client for required service instance ID ALL
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ALL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ALL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <utility>
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_group.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/connection_manager/connection_manager.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/stop_offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_ack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_nack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine_context.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/offer_service_listener.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_endpoint_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_observer.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "someip-protocol/internal/types.h"
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
 * \brief A Service Discovery client for instance ID = ALL.
 * \details This class processes incoming offer SD messages, creates relevant SD Clients if needed, or forwards
 *          to the existing ones. It also creates RemoteServers for newly received offers.
 *          It forwards incoming Subscribe ACK/NACK messages to the corresponding Sd client.
 */
class ServiceDiscoveryClientAll final : public ServiceDiscoveryClientInterface,
                                        public find_service::ServiceDiscoveryClientStateMachineContext,
                                        public OfferServiceListener {
 public:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Define destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryClientAll() noexcept override;

  ServiceDiscoveryClientAll() = delete;
  ServiceDiscoveryClientAll(ServiceDiscoveryClientAll const&) = delete;
  ServiceDiscoveryClientAll(ServiceDiscoveryClientAll&&) = delete;
  ServiceDiscoveryClientAll& operator=(ServiceDiscoveryClientAll const&) & = delete;
  ServiceDiscoveryClientAll& operator=(ServiceDiscoveryClientAll&&) & = delete;

  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief  A timeout list for UDP Method message accumulation purposes.
   */
  using MethodsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::MethodId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::MethodId>,
      std::equal_to<someip_protocol::internal::MethodId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::MethodId const, std::chrono::nanoseconds>>>;

  /*!
   * \brief Constructor of ServiceDiscoveryClientAll.
   *
   * \param[in] required_service_instance_deployment  Service interface deployment.
   *                                                  Used to get the service interface ID, to know if
   *                                                  TCP/UDP is required and to create corresponding RemoteServers.
   * \param[in] required_event_groups                 List of all required event groups by this service instance.
   * \param[in] required_pdus                         List of required PDUs, needed for the multicast event listener.
   * \param[in] someip_sd_client_si_config            ServiceDiscovery Config (TTL, initial delay).
   * \param[in] local_network_endpoint                Local network endpoint for this required service instance.
   *                                                  Used to create corresponding SdClients and RemoteServers.
   * \param[in] timer_manager                         Timer manager. Used to to create the SD client state machine.
   * \param[in] reactor                               Reactor, used to trigger SW events for connection cleanup.
   * \param[in] message_scheduler                     Message scheduler to schedule outcoming Find service entries.
   * \param[in] udp_message_accumulation_timeouts     Service instance timeouts for message_accumulation.
   * \param[in] custom_subscription_endpoints         Custom (optional) endpoints for eventgroup subscription
   * \param[in] connection_manager                    A connection manager.
   *
   * \pre     -
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientAll(
      amsr::someip_daemon_core::configuration::types::RequiredSomeIpServiceInstanceDeployment const&
          required_service_instance_deployment,
      RequiredEventgroupsContainer const& required_event_groups,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus,
      amsr::someip_daemon_core::configuration::types::SomeipSdClientServiceInstanceConfig const&
          someip_sd_client_si_config,
      amsr::someip_daemon_core::configuration::types::RequiredNetworkEndpoint const& local_network_endpoint,
      amsr::steady_timer::TimerManagerInterface*, osabstraction::io::reactor1::Reactor1Interface& reactor,
      message::scheduler::SchedulerInterface* message_scheduler,
      MethodsMessageAccumulationTimeoutMap const& udp_message_accumulation_timeouts,
      configuration::types::ServiceAddress const& custom_subscription_endpoints,
      connection_manager::ConnectionManager const& connection_manager) noexcept;

  /*!
   * \brief Register statistics handler.
   *
   * \param[in] statistics_handler A reference to the statistics handler.
   *
   * \pre -
   * \context Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterStatisticsHandler(
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface* statistics_handler) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
  void RegisterOfferServiceListener(OfferServiceListener* offer_service_listener) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Unregister the listener.
   *
   * \pre -
   * \context Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterOfferServiceListener() noexcept override;

  // ---- Local Calls from SOME/IP Daemon --------------------------------------------------------------------

 private:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called on service instance shutdown.
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void StopServiceDiscoveryClient() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when network is brought up.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnNetworkUp() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when network is brought down.
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnNetworkDown() noexcept override;

  // ---- Calls from Local applications ----------------------------------------------------------------------

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Called when the service discovery for this service shall start.
   * \details     Once service is requested, FindServiceAll messages shall be sent out according to Initial
   *              SD Timing Config.
   *
   * \pre         Service has not been requested before
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RequestService() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Called when the service discovery for this service is not needed anymore.
   * \details     Indicates that this service is not needed anymore, no more FindService messages will be sent
   *              after release.
   *
   * \pre         Service has been requested before
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void ReleaseService() noexcept override;

  // ---- Calls from Remote services --------

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief     Called on detection of reboot.
   *
   * \details   Called from ServiceDiscovery to ALL state machines on reboot detection of a source.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port    A port identifying sender's port.
   *
   * \pre          -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace   SPEC-10144488
   */
  void OnRebootDetected(someip_daemon_core::IpAddress const& from_address,
                        amsr::net::ip::Port const& from_port) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief     Function to handle reception of an OfferService SOME/IP SD entry.
   * \details   In case corresponding SdClient has been already created, the offer shall be forwarded to it.
   *            Otherwise, a new SdClient (and a remote server) will be created.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The OfferService entry
   * \param[in] is_multicast  A flag indicating whether the received SD message came from a multicast channel
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \pre          -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-10144715
   */
  bool OnOfferServiceEntry(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                           message::entries::OfferServiceEntry const& entry, bool const is_multicast) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Function to handle reception of a StopOfferService SOME/IP SD entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry         The StopOfferService entry
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool OnStopOfferServiceEntry(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                               message::entries::StopOfferServiceEntry const& entry) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief      Function to handle reception of an SubscribeEventgroupAck SOME/IP SD entry.
   * \details    The received entry will be forwarded to the corresponding SdClient if exists.
   *
   * \param[in]  entry  The received Subscribe ACK entry.
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool OnSubscribeEventgroupAckEntry(message::entries::SubscribeEventgroupAckEntry const& entry) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief      Function to handle reception of an SubscribeEventgroupNAck SOME/IP SD entry.
   * \details    The received entry will be forwarded to the corresponding SdClient if exists.
   *
   * \param[in] entry  The received Subscribe NACK entry.
   *
   * \return true if the entry is processed by the observer, false if it is discarded
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool OnSubscribeEventgroupNackEntry(message::entries::SubscribeEventgroupNAckEntry const& entry) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether network is active or not.
   *
   * \return true if network is active and false otherwise.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool IsNetworkUp() const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool IsServiceRequested() const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks the availability of the service.
   *
   * \return always false, to indicate that FindService messages shall not be stopped.
   *
   * \pre         -
   * \context     App, Timer
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool IsServiceAvailable() const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Transmits a FindService entry.
   *
   * \pre         -
   * \context     App, Timer
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \trace SPEC-4980067
   */
  void SendFindService() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Register an observer into the correct Client State Machine.
   *
   * \param[in] instance_id  The ID of the RemoteServer.
   * \param[in] observer     The observer to the Client State Machine.
   *
   * \pre -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id,
                            ServiceDiscoveryClientObserver* const observer) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Unregister the remote server.
   * \details This API is needed as RemoteServers are destroyed on-demand when no more proxies request the service,
   *          but SD will still be running, so we should avoid notifying an object that has been destroyed.
   *
   * \param[in] instance_id  The ID of the RemoteServer.
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
  void UnregisterRemoteServer(amsr::someip_protocol::internal::InstanceId const instance_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief     Called from an internal SM when an offer is accepted
   *
   * \param[in] instance_id The instance identifier of the service.
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnOfferService(
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const& remote_server_address) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Called from an internal SM when a service is no longer offered offered (reboot detected, TTL or
   *              StopOffer received)
   *
   * \param[in]   instance_id The instance identifier of the service.
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnStopOfferService(amsr::someip_protocol::internal::InstanceId const instance_id) noexcept override;

  /*!
   * \brief Checks whether an Offer Service entry id matches to this client
   *
   * \param[in] entry_id The received entry id
   * \return    true if the entry id matches to this client
   *
   * \pre       -
   * \context   Network
   * \steady    TRUE
   */
  bool IsMatchingOfferServiceEntry(message::entries::ServiceEntryId const entry_id) const noexcept;

  /*!
   * \brief Checks whether an eventrgroup entry id matches to this client
   *
   * \param[in] entry_id The received entry id
   * \return    true if the entry id matches to this client
   *
   * \pre       -
   * \context   Network
   * \steady    TRUE
   */
  bool IsMatchingSubscribeEventgroupEntry(message::entries::EventgroupEntryId const entry_id) const noexcept;

  /*!
   * \brief     Go through all services that are reported as stopped and clean their resources.
   * \details   If service_discovery_started_ = true, do nothing,
   *            else, for each SdClient corresponding to each id within stopped_service_container_,
   *            if service has stopped, remove the corresponding SdClient and RemoteServer.
   *
   * \pre       -
   * \context   Timer
   * \steady    FALSE
   */
  void CleanupStoppedServices() noexcept;

  /*!
   * \brief State machine type definition
   */
  using ServiceDiscoveryClientStateMachine = find_service::ServiceDiscoveryClientStateMachine;

  /*!
   * \brief Container type to store instance IDs.
   */
  using InstanceIdContainer = ara::core::Vector<amsr::someip_protocol::internal::InstanceId>;

  /*!
   * \brief Map to store SdClient objects.
   */
  using ServiceDiscoveryClientContainer =
      ara::core::Map<someip_protocol::internal::InstanceId, std::shared_ptr<ServiceDiscoveryClient<>>>;

  /*!
   * \brief Timer manager, used to create instances of type ServiceDiscoveryClient.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief The reactor, used to trigger SW events for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief A message scheduler.
   */
  message::scheduler::SchedulerInterface* message_scheduler_;

  /*!
   * \brief A statistics handler; used to report invalid received entries.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface* statistics_handler_{nullptr};

  /*!
   * \brief ServiceDiscoveryClientAll Context, that receives service offer updates.
   */
  OfferServiceListener* offer_service_listener_{nullptr};

  /*!
   * \brief Service interface deployment.
   */
  amsr::someip_daemon_core::configuration::types::RequiredSomeIpServiceInstanceDeployment const
      required_service_instance_deployment_;

  /*!
   * \brief Local endpoint mapped to this RequiredServiceInstance.  These are needed for:
   *        - Do sanity check for incoming entries.
   *        - creation of SdClients
   *        - creation of RemoteServers
   */
  amsr::someip_daemon_core::configuration::types::RequiredNetworkEndpoint const local_network_endpoint_;

  /*!
   * \brief Eventgroups required by this RequiredServiceInstance. These are needed for:
   *        - determining the required protocol for service communication (to do sanity check for incoming entries)
   *        - creation of SdClients
   *        - creation of RemoteServers
   */
  RequiredEventgroupsContainer const required_event_groups_;

  /*!
   * \brief PDUs required by this RequiredServiceInstance. These are needed for creating a new RemoteServer instance,
   *        which will need them for event multicast listening.
   */
  configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const required_pdus_;

  /*!
   * \brief SdClient configuration; defines the TTL of the find service messages and the initial delay for sending the
   * messages.
   */
  amsr::someip_daemon_core::configuration::types::SomeipSdClientServiceInstanceConfig const someip_sd_client_si_config_;

  /*!
   * \brief A state machine responsible for sending FindService entries with instance ID = ALL.
   */
  ServiceDiscoveryClientStateMachine state_machine_{
      this, timer_manager_,
      (someip_sd_client_si_config_.sd_config.value_or(
          amsr::someip_daemon_core::configuration::types::InitialSdDelayConfig{})),
      required_service_instance_deployment_.deployment_id, amsr::someip_protocol::internal::kInstanceIdAll};

  /*!
   * \brief Container to keep the life cycle of the created state machines.
   */
  ServiceDiscoveryClientContainer sd_client_map_{};

  /*!
   * \brief   List of all services that are stopped and ready for cleanup.
   * \details This list is filled whenever StopOffer entry is received, Reboot is detected or TTL is expired for a
   *          specific service. Once it is changed, a job is scheduled with 0 timer to cleanup all SdClients and Remote
   *          servers with the stored IDs (after double check that require service instance is still released and the
   *          service is not found again).
   */
  InstanceIdContainer stopped_service_container_{};

  /*!
   * \brief Indicates whether the any application has started service discovery for this required service instance.
   */
  bool service_discovery_started_{false};

  /*!
   * \brief Indicates whether network is active or not.
   */
  bool is_network_up_{false};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("ServiceDiscoveryClientAll"_sv,
                                                      required_service_instance_deployment_.deployment_id)};

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
   * \brief A Find Service Entry instance that is sent by this state machine.
   */
  message::entries::FindServiceEntry const find_service_entry_{
      message::entries::ServiceEntryId{
          required_service_instance_deployment_.deployment_id.service_interface_id,
          amsr::someip_protocol::internal::kInstanceIdAll,
          required_service_instance_deployment_.deployment_id.service_interface_version.major_version,
          find_service_minor_version_},
      someip_sd_client_si_config_.ttl};

  /*!
   * \brief Method props for UDP message accumulation.
   */
  MethodsMessageAccumulationTimeoutMap const udp_message_accumulation_timeouts_;

  /*!
   * \brief Custom subscription endpoints, needed to create new Remote Servers.
   */
  configuration::types::ServiceAddress const custom_subscription_endpoints_;

  /*!
   * \brief An empty SecCom configuration (no secure endpoints).
   * \details This is needed to guarantee the lifetime of SecComConfig.
   */
  configuration::ConfigurationTypesAndDefs::SecComConfig const sec_com_config_{};

  /*!
   * \brief Handle for Reactor software event for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::CallbackHandle sw_event_handle_cleanup_{};
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_ALL_H_
