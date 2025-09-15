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
/*!        \file  someipd.h
 *        \brief  SOME/IP daemon for POSIX OS.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIPD_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIPD_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <utility>

#include "amsr/ipc/unicast_address.h"
#include "amsr/pdu-aggregation/internal/pdu_aggregator.h"
#include "amsr/someip_daemon/extension_points/iam/extension_point.h"
#include "amsr/someip_daemon/extension_points/message_authentication/extension_point.h"
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/extension_point.h"
#include "amsr/someip_daemon/extension_points/tls/tls_extensions_handler_interface.h"
#include "amsr/someip_daemon/extension_points/tracing/extension_point.h"
#include "amsr/someip_daemon_core/application/application_acceptor.h"
#include "amsr/someip_daemon_core/application/application_manager.h"
#include "amsr/someip_daemon_core/client/required_service_instance_manager_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/json_configuration.h"
#include "amsr/someip_daemon_core/connection_manager/meta_data_manager/meta_data_manager.h"
#include "amsr/someip_daemon_core/iam/iam_adapter.h"
#include "amsr/someip_daemon_core/init/client_initializer.h"
#include "amsr/someip_daemon_core/init/network_initializer.h"
#include "amsr/someip_daemon_core/init/server_initializer.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_manager.h"
#include "amsr/someip_daemon_core/packet_handler/local_incoming_packet_validator.h"
#include "amsr/someip_daemon_core/packet_router/packet_router.h"
#include "amsr/someip_daemon_core/packet_router/someip_message_creator/someip_message_creator.h"
#include "amsr/someip_daemon_core/server/local_server_manager.h"
#include "amsr/someip_daemon_core/service_discovery/dynamic_service_discovery.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_factory.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server.h"
#include "amsr/someip_daemon_core/timer_manager/timer_manager_wrapper.h"
#include "amsr/someip_daemon_core/tracing/tracing_adapter.h"
#include "amsr/steady_timer/timer_manager_factory.h"
#include "ara/core/abort.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "osabstraction/thread/thread.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {

/*!
 * \brief Class that contains functions for construction of members of struct SomeipdMember.
 */
class ConstructSomeipdMember final {
 public:
  /*!
   * \brief Define destructor.
   * \steady FALSE
   */
  ~ConstructSomeipdMember() noexcept = default;

  ConstructSomeipdMember(ConstructSomeipdMember const&) = delete;
  ConstructSomeipdMember(ConstructSomeipdMember&&) = delete;
  ConstructSomeipdMember& operator=(ConstructSomeipdMember const&) & = delete;
  ConstructSomeipdMember& operator=(ConstructSomeipdMember&&) & = delete;

  /*!
   * \brief Reactor type.
   */
  using ReactorType = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Make reactor construction token.
   * \param[in] num_callbacks Number of callbacks that the reactor should be able to handle. Valid range: 0 to 8191.
   * \return    Construction token for Reactor1.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  static ara::core::Result<ReactorType::ConstructionToken> MakeReactorPreConstructToken(
      std::uint16_t const num_callbacks) {
    ara::core::Result<ReactorType::ConstructionToken> result{ReactorType::Preconstruct(num_callbacks)};

    if (!result.HasValue()) {
      ara::core::ErrorCode const& error_code{result.Error()};
      ara::core::Abort(error_code.UserMessage().data());
    }

    return result;
  }

  /*!
   * \brief Make message authentication (extension point).
   * \return    pointer to message authentication.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  static std::unique_ptr<someip_daemon::extension_points::message_authentication::MacInterface>
  MakeMessageAuthentication() {
    ara::core::Result<std::unique_ptr<someip_daemon::extension_points::message_authentication::MacInterface>> result{
        someip_daemon::extension_points::message_authentication::Create()};

    if (!result.HasValue()) {
      ara::core::ErrorCode const& error_code{result.Error()};
      ara::core::Abort(error_code.UserMessage().data());
    }

    return std::move(result.Value());
  }

  /*!
   * \brief Make statistics handler (extension point).
   * \return    pointer to statistics handler.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  static std::unique_ptr<someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface>
  MakeStatisticsHandler() {
    ara::core::Result<std::unique_ptr<someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface>>
        result{someip_daemon::extension_points::statistics_handler::Create()};

    if (!result.HasValue()) {
      ara::core::ErrorCode const& error_code{result.Error()};
      ara::core::Abort(error_code.UserMessage().data());
    }

    return std::move(result.Value());
  }

  /*!
   * \brief Make IAM (extension point).
   *
   * \return    Unique pointer to IAM interface.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * Call method Create of whatever IAM (DummyIam or user-defined IAM) is linked into the SomeIpDaemon binary.
   * \endinternal
   */
  static std::unique_ptr<someip_daemon::extension_points::iam::IamInterface> MakeIam() noexcept {
    return someip_daemon::extension_points::iam::Create();
  }

  /*!
   * \brief Make Tracing (extension point).
   *
   * \return    Unique pointer to tracing implementation.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Call method Create of whatever tracing (null or user-defined tracing) is linked into the SomeIpDaemon binary.
   * \endinternal
   */
  static std::unique_ptr<someip_daemon::extension_points::tracing::TracingInterface> MakeTracing() noexcept {
    return someip_daemon::extension_points::tracing::Create();
  }
};

/*!
 * \brief Struct that contains the data and objects necessary to initialize the SOME/IP daemon.
 */
struct SomeipdMember {
  /*!
   * \brief Configuration type.
   */
  using ConfigurationType = configuration::Configuration;

  /*!
   * \brief Reactor type.
   */
  using ReactorType = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Timestamp generator type.
   */
  using TimeStampGeneratorType = connection_manager::meta_data_manager::SystemClockTimeStampGenerator;

  /*!
   * \brief Meta data management type.
   */
  using MetaDataManagerType = connection_manager::meta_data_manager::MetaDataManager;

  /*!
   * \brief PDU Aggregator type.
   */
  using PduAggregatorType = amsr::pdu_aggregation::internal::PduAggregator;

  /*!
   * \brief Packet validator type to validate the routed packet.
   */
  using PacketValidatorType = packet_handler::LocalIncomingPacketValidator;

  /*!
   * \brief Connection Manager type.
   */
  using ConnectionManagerType = connection_manager::ConnectionManager;

  /*!
   * \brief Required service instance manager type.
   */
  using RequiredServiceInstanceManagerType = client::RequiredServiceInstanceManager;

  /*!
   * \brief Application Manager type.
   */
  using ApplicationManagerType =
      application::ApplicationManager<application::ApplicationConnection, application::ApplicationAcceptor>;

  /*!
   * \brief Remote client type, used to communicate with local server applications.
   */
  using RemoteClientType = server::RemoteClient<ConnectionManagerType>;

  /*!
   * \brief SD Server type, used to handle SOME/IP SD protocol for Provided Service Instances.
   */
  using ServiceDiscoveryServerType = service_discovery::state_machine::server::ServiceDiscoveryServer;

  /*!
   * \brief SD client type, used to handle SOME/IP SD protocol for Required Service Instances.
   */
  using ServiceDiscoveryClientType = service_discovery::state_machine::client::ServiceDiscoveryClient<>;

  /*!
   * \brief Sd client factory type.
   */
  using ServiceDiscoveryClientFactoryType = service_discovery::state_machine::client::ServiceDiscoveryClientFactory<>;

  /*!
   * \brief TLS extensions handlers type.
   */
  using TlsExtensionsHandlerContainer =
      ara::core::Vector<std::unique_ptr<someip_daemon::extension_points::tls::TlsExtensionsHandlerInterface>>;

  /*!
   * \brief Constructor for SomeipdMember struct.
   * \param[in] config A configuration.
   * \param[in] tls_extensions_handlers A container of extensions handlers provided by user application.
   *
   */
  // VECTOR NC VectorC++-V11.0.3:MD_SomeIpDaemon_V11.0.3_refactoring
  SomeipdMember(configuration::Configuration& config, TlsExtensionsHandlerContainer const& tls_extensions_handlers)
      : config_{config}, tls_extensions_handlers_{tls_extensions_handlers} {};

  /*!
   * \brief SomeIpd configuration.
   */
  configuration::Configuration& config_;

  /*!
   * \brief SomeIpd reactor object.
   */
  ReactorType reactor_{ConstructSomeipdMember::MakeReactorPreConstructToken(config_.kNumberReactorCallbacks).Value()};

  /*!
   * \brief Timer management member
   * \note A timer manager factory shall be moved into the unit for the creation of the timer manager. This
   *       helps testing the behavior of the unit by injecting a mock. The factory is not needed anywhere else outside
   *       of this wrapper.
   *       This object must be destructed only after all users of the timer manager have been destructed.
   */
  timer_manager::TimerManagerWrapper timer_manager_wrapper_{reactor_, amsr::steady_timer::TimerManagerFactory{}};

  /*!
   * \brief MAC management member.
   */
  std::unique_ptr<someip_daemon::extension_points::message_authentication::MacInterface> mac_manager_{
      ConstructSomeipdMember::MakeMessageAuthentication()};

  /*!
   * \brief PDU aggregator member.
   */
  PduAggregatorType pdu_aggregator_{};

  /*!
   * \brief Statistics handler member.
   */
  std::unique_ptr<someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface> statistics_handler_{
      ConstructSomeipdMember::MakeStatisticsHandler()};

  /*!
   * \brief IAM member.
   */
  iam::IamAdapter iam_{config_, ConstructSomeipdMember::MakeIam()};

  /*!
   * \brief Tracing member.
   */
  tracing::TracingAdapter tracing_{ConstructSomeipdMember::MakeTracing()};

  /*!
   * \brief SOME/IP Message Creator.
   */
  packet_router::someip_message_creator::SomeIpMessageCreator someip_message_creator_{};

  /*!
   * \brief Packet router management member.
   */
  std::shared_ptr<packet_router::PacketRouter> packet_router_{
      someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<packet_router::PacketRouter>(
          &config_, timer_manager_wrapper_.GetTimerManager(), iam_, *statistics_handler_, someip_message_creator_)};

  /*!
   * \brief Packet validator member.
   */
  PacketValidatorType const packet_validator_{config_, iam_};

  /*!
   * \brief Timestamp generator member
   */
  TimeStampGeneratorType systemsclock_timestamp_generator_{};

  /*!
   * \brief Meta data management member
   */
  MetaDataManagerType meta_data_manager_{&config_, &systemsclock_timestamp_generator_};

  /*!
   * \brief   A vector of TLS extensions handlers provided by user application.
   * \details The handlers will be used by TLS library to provide information on the TLS extensions received
   *          from the remote peer during the TLS handshake.
   */
  TlsExtensionsHandlerContainer const& tls_extensions_handlers_;

  /*!
   * \brief Connection management member
   */
  ConnectionManagerType connection_manager_{&config_,
                                            &reactor_,
                                            packet_router_,
                                            timer_manager_wrapper_.GetTimerManager(),
                                            *mac_manager_,
                                            *statistics_handler_,
                                            &meta_data_manager_,
                                            &pdu_aggregator_,
                                            tls_extensions_handlers_};

  /*!
   * \brief Service discovery member.
   */
  service_discovery::DynamicServiceDiscovery service_discovery_{
      config_, reactor_, timer_manager_wrapper_.GetTimerManager(), iam_, *statistics_handler_};

  /*!
   * \brief Sd Client Factory.
   */
  ServiceDiscoveryClientFactoryType sd_client_factory_{timer_manager_wrapper_.GetTimerManager(), *statistics_handler_};

  /*!
   * \brief Required Service Instance Manager member that keeps track of remote offers.
   */
  RequiredServiceInstanceManagerType required_service_instance_manager_{};

  /*!
   * \brief Local Server Manager member that keeps track of local servers.
   */
  server::LocalServerManager local_server_manager_{};

  /*!
   * \brief Application management member.
   */
  ApplicationManagerType application_manager_{reactor_,
                                              packet_validator_,
                                              iam_,
                                              config_,
                                              required_service_instance_manager_,
                                              local_server_manager_,
                                              tracing_ /* Communication tracing part*/,
                                              tracing_ /* SD Tracing part*/};

  /*!
   * \brief Interface card Monitor Manager Instance.
   */
  network::InterfaceCardMonitorManager interface_card_monitor_manager_{};
};

/*!
 * \brief SomeIpd.
 */
class SomeIpd final {
 public:
  /*!
   * \brief Default destructor for SomeIpd.
   * \steady FALSE
   */
  ~SomeIpd() noexcept = default;

  /*!
   * \brief Default move constructor.
   * \steady FALSE
   */
  SomeIpd(SomeIpd&&) noexcept = default;

  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  SomeIpd(SomeIpd const&) noexcept = default;

  /*!
   * \brief Default move assignment.
   * \steady FALSE
   */
  SomeIpd& operator=(SomeIpd&&) & noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \steady FALSE
   */
  SomeIpd& operator=(SomeIpd const&) & noexcept = default;

  /*!
   * \brief Constructor.
   * \param[in] member Struct containing all necessary members to create the instance.
   * \steady FALSE
   */
  explicit SomeIpd(SomeipdMember& member) noexcept
      : config_(member.config_),
        packet_router_{member.packet_router_},
        timer_manager_wrapper_{member.timer_manager_wrapper_},
        statistics_handler_{*member.statistics_handler_},
        local_server_manager_{member.local_server_manager_},
        service_discovery_{member.service_discovery_},
        application_manager_{member.application_manager_},
        required_service_instance_manager_{member.required_service_instance_manager_},
        sd_client_factory_{member.sd_client_factory_},
        connection_manager_{member.connection_manager_},
        reactor_{member.reactor_},
        interface_card_monitor_manager_{member.interface_card_monitor_manager_} {}

  /*!
   * \brief Initialize the SOME/IP daemon and notify the execution manager about its state.
   * \internal
   * - Initialize the connection manager.
   * - Initialize network state monitoring.
   * - Client initialization: initialize required service instances.
   * - Server initialization: initialize provided service instances.
   * - Initialize service discovery .
   * - Initialize application manager: start listening for applications.
   * \endinternal
   * \steady FALSE
   */
  void Initialize() noexcept {
    init::NetworkInitializer::InitializeConnectionManager(config_, connection_manager_);
    init::NetworkInitializer::InitializeNetworkMonitoring(config_, interface_card_monitor_manager_, reactor_);
    init::ClientInitializer::InitializeRequiredServices(config_, required_service_instance_manager_, service_discovery_,
                                                        sd_client_factory_, timer_manager_wrapper_.GetTimerManager(),
                                                        reactor_, statistics_handler_, connection_manager_,
                                                        packet_router_, interface_card_monitor_manager_);
    init::ServerInitializer<SomeipdMember::ConnectionManagerType>::InitializeProvidedServices(
        config_, service_discovery_, timer_manager_wrapper_.GetTimerManager(), reactor_, connection_manager_,
        local_server_manager_, packet_router_);
    service_discovery_.Initialize();
    application_manager_.Listen(
        amsr::ipc::UnicastAddress{config_.GetIpcChannel().domain, config_.GetIpcChannel().port});
  }

  /*!
   * \brief Deinitialize the SOME/IP daemon.
   *
   * \internal
   * - Deinitialize provided services.
   * - Accept no more applications.
   * - Destruct all active applications connections.
   * - Deinitialize required services.
   * \endinternal
   * \steady FALSE
   */
  void Deinitialize() noexcept {
    init::ServerInitializer<SomeipdMember::ConnectionManagerType>::DeinitializeProvidedServices(config_,
                                                                                                service_discovery_);

    application_manager_.Close();
    std::size_t number_of_active_connections{application_manager_.GetNumberOfActiveConnections()};
    SomeipdMember::ApplicationManagerType::ApplicationPool& application_pool{application_manager_.GetApplicationPool()};
    application_manager_.DestroyApplications(application_pool, number_of_active_connections);

    init::ClientInitializer::DeinitializeRequiredServices(config_, required_service_instance_manager_,
                                                          service_discovery_, interface_card_monitor_manager_);
  }

  /*!
   * \brief Provide access to the required service instance manager.
   * \steady FALSE
   */
  client::RequiredServiceInstanceManagerInterface const& GetRequiredServiceInstanceManager() const noexcept {
    return required_service_instance_manager_;
  }

 private:
  /*!
   * \brief SomeIpd configuration.
   */
  SomeipdMember::ConfigurationType& config_;

  /*!
   * \brief Packet router management member.
   */
  std::shared_ptr<packet_router::PacketRouter> packet_router_;

  /*!
   * \brief Timer management member.
   */
  timer_manager::TimerManagerWrapper& timer_manager_wrapper_;

  /*!
   * \brief Statistics handler member.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief Local Server Manager member that keeps track of local servers.
   */
  server::LocalServerManager& local_server_manager_;

  /*!
   * \brief Service discovery member.
   */
  service_discovery::DynamicServiceDiscovery& service_discovery_;

  /*!
   * \brief Application management member.
   */
  SomeipdMember::ApplicationManagerType& application_manager_;

  /*!
   * \brief Required Service Instance Manager.
   */
  client::RequiredServiceInstanceManagerInterface& required_service_instance_manager_;

  /*!
   * \brief Sd client factory member.
   */
  SomeipdMember::ServiceDiscoveryClientFactoryType& sd_client_factory_;

  /*!
   * \brief Connection Manager.
   */
  SomeipdMember::ConnectionManagerType& connection_manager_;

  /*!
   * \brief Reactor.
   */
  SomeipdMember::ReactorType& reactor_;

  /*!
   * \brief Interface Card Monitor Manager.
   */
  network::InterfaceCardMonitorManager& interface_card_monitor_manager_;
};

}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIPD_H_
