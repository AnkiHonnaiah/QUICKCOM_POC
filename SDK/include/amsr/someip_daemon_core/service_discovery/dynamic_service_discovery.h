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
/**        \file  dynamic_service_discovery.h
 *        \brief  Dynamic service discovery.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_DYNAMIC_SERVICE_DISCOVERY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_DYNAMIC_SERVICE_DISCOVERY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_endpoint.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/log/logging.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief ServiceDiscovery.
 * \trace SPEC-4663391
 * \trace SPEC-10144614
 * \trace SPEC-10144615
 */
class DynamicServiceDiscovery : public ServiceDiscoveryInterface {
 public:
  /*!
   * \brief A type alias for Identity Access Manager (IAM).
   */
  using IdentityAccessManager = iam::IamInterface;

  /*!
   * \brief A type alias for the Reactor1.
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Constructor.
   *
   * \param[in] config                             A reference to a configuration.
   * \param[in] reactor                            A reference to a reactor for asynchronous event notification on
   *                                               socket handles.
   * \param[in] timer_manager                      A reference to a timer manager.
   * \param[in] identity_access_manager            A reference to an identity access manager.
   * \param[in] statistics_handler                 A reference to the statistics handler.
   *
   * \context Init
   * \steady FALSE
   *
   * \internal
   * - Create service discovery endpoints.
   * \endinternal
   */
  DynamicServiceDiscovery(
      configuration::Configuration const& config, Reactor& reactor,
      amsr::steady_timer::TimerManagerInterface* timer_manager, IdentityAccessManager& identity_access_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler)
      : ServiceDiscoveryInterface(),
        identity_access_manager_{identity_access_manager},
        config_{config},
        timer_manager_{timer_manager},
        statistics_handler_{statistics_handler} {
    // PTP-B-SomeIpDaemon-DynamicServiceDiscovery_CreateSdEndpoints
    CreateSdEndpoints(reactor);
    // PTP-E-SomeIpDaemon-DynamicServiceDiscovery_CreateSdEndpoints
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~DynamicServiceDiscovery() noexcept override = default;

  DynamicServiceDiscovery() = delete;
  DynamicServiceDiscovery(DynamicServiceDiscovery const&) = delete;
  DynamicServiceDiscovery(DynamicServiceDiscovery&&) = delete;
  DynamicServiceDiscovery& operator=(DynamicServiceDiscovery const&) & = delete;
  DynamicServiceDiscovery& operator=(DynamicServiceDiscovery&&) & = delete;

  /*!
   * \brief Initializes endpoints and state machines.
   *
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Listen to all SD endpoints that are mapped to required service instances.
   * \endinternal
   */
  void Initialize() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    ListenToAllRequiredSdEndpoints();
  }

 protected:
  /*!
   * \brief Alias for "ServiceDiscoveryEndpoint" container.
   */
  using ServiceDiscoveryEndpointContainer =
      ara::core::Map<amsr::someip_protocol::internal::IpAddress, std::shared_ptr<ServiceDiscoveryEndpoint>>;

  /*!
   * \brief Get the SD endpoint corresponding to the given address.
   *
   * \param[in] address Address of the endpoint.
   * \return The SD endpoint corresponding to the given address, or nullptr if not existing.
   *
   * \pre -
   * \context Init, Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceDiscoveryEndpoint* GetSdEndpoint(amsr::someip_protocol::internal::IpAddress const& address) noexcept override {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    ServiceDiscoveryEndpoint* endpoint_ptr{nullptr};

    typename ServiceDiscoveryEndpointContainer::iterator const sd_endpoint_it{FindServiceDiscoveryEndpoint(address)};
    if (sd_endpoint_it != sd_endpoints_.end()) {
      endpoint_ptr = (sd_endpoint_it->second).get();
    }

    return endpoint_ptr;
  }

  /*!
   * \brief Create service discovery endpoints and cyclic timers.
   * \param[in] reactor  A reference to a reactor for asynchronous event notification on
   *                     socket handles.
   *
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Get all configured network endpoints.
   * - For each configured network endpoint,
   *   - Log the endpoint's address.
   *   - Create a service discovery endpoint from the configured network endpoint.
   *   - Add the created endpoint to the list of service discovery endpoints.
   * - Create cyclic timers for each service discovery endpoint.
   * \endinternal
   */
  void CreateSdEndpoints(Reactor& reactor) {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // Create a service discovery endpoint for each configured network endpoint.
    for (configuration::ConfigurationTypesAndDefs::NetworkEndpointContainer::const_reference network_endpoint_cfg :
         config_.GetNetworkEndpoints()) {
      logger_.LogVerbose(
          [&network_endpoint_cfg](ara::log::LogStream& s) {
            s << "Address ";
            s << network_endpoint_cfg.address_.ToString();
          },
          static_cast<char const*>(__func__), __LINE__);

      // The network endpoint ip addresses are normalized during parsing and aborted if normalization fails.
      // Hence, it is guaranteed that the address is valid.
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
      std::shared_ptr<ServiceDiscoveryEndpoint> const endpoint_ptr{MakeEndpoint(
          &reactor, network_endpoint_cfg.address_, network_endpoint_cfg.service_discovery_.multicast_address_,
          amsr::net::ip::Port{network_endpoint_cfg.service_discovery_.port_}, network_endpoint_cfg.mtu_)};

      amsr::someip_protocol::internal::IpAddress const network_endpoint_str{network_endpoint_cfg.address_.ToString()};
      sd_endpoints_[network_endpoint_str] = endpoint_ptr;
    }
    CreateCyclicTimers();
    CreateRepetitionOfferTimers();
  }

  /*!
   * \brief Create cyclic timers for each service discovery endpoint.
   *
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Loop through cyclic timer configurations and create a timer for each configuration inside
   *   its corresponding service discovery endpoint with its corresponding cyclic offer delay.
   * \endinternal
   */
  void CreateCyclicTimers() {
    //  Loop through cyclic timer configurations and create a timer for each configuration inside its corresponding
    //  service discovery endpoint with its corresponding cyclic offer delay.
    for (configuration::ConfigurationTypesAndDefs::CyclicTimerConfig const& timer_config :
         config_.GetCyclicTimerConfig()) {
      logger_.LogVerbose(
          [&timer_config](ara::log::LogStream& s) {
            s << "Creating cyclic timer with period:";
            s << static_cast<std::uint64_t>(timer_config.cyclic_offer_delay.count()) << "[ns].";
          },
          static_cast<char const*>(__func__), __LINE__);
      typename ServiceDiscoveryEndpointContainer::iterator const sd_endpoint_it{
          FindServiceDiscoveryEndpoint(timer_config.address)};
      (sd_endpoint_it->second)->CreateCyclicTimer(timer_config.cyclic_offer_delay);
    }
  }

  /*!
   * \brief      Creates repetition offer timers for each SD endpoint.
   *
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \vprivate
   */
  void CreateRepetitionOfferTimers() {
    //  Loop through repetition offer timer configurations and create a timer for each configuration inside its
    //  corresponding SD endpoint wits its corresponding initial repetitions base delay
    for (configuration::ConfigurationTypesAndDefs::RepetitionOfferTimerConfig const& timer_config :
         config_.GetRepetitionOfferTimerConfig()) {
      logger_.LogVerbose(
          [&timer_config](ara::log::LogStream& s) {
            s << "Creating repetition offer timer with period:";
            s << static_cast<std::uint64_t>(timer_config.initial_repetitions_base_delay.count()) << "[ns].";
          },
          static_cast<char const*>(__func__), __LINE__);
      typename ServiceDiscoveryEndpointContainer::iterator const sd_endpoint_it{
          FindServiceDiscoveryEndpoint(timer_config.address)};
      (sd_endpoint_it->second)->CreateRepetitionOfferTimer(timer_config.initial_repetitions_base_delay);
    }
  }

  /*!
   * \brief listen to all SD endpoints that are mapped to required service instances.
   *
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Go through all the Required service instances in the configuration.
   *   - If the service is configured and an endpoint is found,
   *     - Trigger service discovery endpoint to open and configure the sockets if not already done.
   * \endinternal
   */
  void ListenToAllRequiredSdEndpoints() {
    for (configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer::const_reference
             required_service_instance_cfg : config_.GetIpcChannel().GetRequiredServiceInstances()) {
      configuration::ConfigurationTypesAndDefs::MachineMapping const& port_mapping_cfg{
          required_service_instance_cfg.port_mapping_};
      // check service discovery is enabled for this service instance
      if (required_service_instance_cfg.port_mapping_.communication_type_ !=
          configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kCommunicationOnly) {
        assert(port_mapping_cfg.address_.has_value());
        // Find the endpoint to which the state machine belongs
        typename ServiceDiscoveryEndpointContainer::iterator const sd_endpoint_it{
            FindServiceDiscoveryEndpoint(port_mapping_cfg.address_.value().ToString())};
        if (sd_endpoint_it != sd_endpoints_.end()) {
          logger_.LogVerbose(
              [&port_mapping_cfg](ara::log::LogStream& s) {
                s << "Start listen to SD communication at multicast address ";
                s << port_mapping_cfg.address_.value().ToString();
              },
              static_cast<char const*>(__func__), __LINE__);
          (sd_endpoint_it->second)->OnSocketRequired();
        }
      }
    }
  }

  /*!
   * \brief Creates a new Service Discovery endpoint.
   *
   * \remark Override it for testing.
   *
   * \param[in] reactor A reactor for asynchronous event notification on socket handles.
   * \param[in] unicast_address A unicast IP address used for sending and receiving of service discovery messages.
   * \param[in] multicast_address A multicast IP address used for receiving of service discovery messages.
   * \param[in] port A UDP port.
   * \param[in] max_datagram_length The maximum length of a UDP datagram received or sent.
   *
   * \return A pointer to the newly created endpoint.
   *
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Create a new service discovery endpoint and return its reference.
   * \endinternal
   */
  virtual std::shared_ptr<ServiceDiscoveryEndpoint> MakeEndpoint(Reactor* reactor,
                                                                 someip_daemon_core::IpAddress const& unicast_address,
                                                                 someip_daemon_core::IpAddress const& multicast_address,
                                                                 amsr::net::ip::Port const port,
                                                                 std::size_t const max_datagram_length) {
    std::shared_ptr<ServiceDiscoveryEndpoint> endpoint_ptr{
        someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<ServiceDiscoveryEndpoint>(
            reactor, timer_manager_, unicast_address, multicast_address, port, max_datagram_length,
            identity_access_manager_, statistics_handler_)};
    return endpoint_ptr;
  }

 private:
  /*!
   * \brief Find the service discovery endpoint with the given unicast IP address.
   *
   * \param[in] unicast_address The unicast IP address of the endpoint to be found.
   *
   * \return Iterator pointing to the found instance or to the end of the list if no instance is found.
   *
   * \context Init, Shutdown
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Find a service discovery endpoint based on a unicast address and return it.
   * \endinternal
   */
  typename ServiceDiscoveryEndpointContainer::iterator FindServiceDiscoveryEndpoint(
      amsr::someip_protocol::internal::IpAddress const& unicast_address) {
    return sd_endpoints_.find(unicast_address);
  }

  /*!
   * \brief A container of endpoints for transmission and reception of SOME/IP service discovery messages.
   */
  ServiceDiscoveryEndpointContainer sd_endpoints_{};

  /*!
   * \brief A reference to an identity and access manager instance.
   */
  IdentityAccessManager& identity_access_manager_;

  /*!
   * \brief A reference to a configuration instance.
   */
  configuration::Configuration const& config_;

  /*!
   * \brief A reference to a timer manager instance.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief A statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"DynamicServiceDiscovery"}};
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_DYNAMIC_SERVICE_DISCOVERY_H_
