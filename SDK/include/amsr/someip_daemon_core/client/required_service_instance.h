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
/**        \file  required_service_instance.h
 *        \brief  Required Service Instance.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/client/client_id_generator_interface.h"
#include "amsr/someip_daemon_core/client/local_client_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_factory_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_manager.h"
#include "amsr/someip_daemon_core/client/required_service_instance_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/packet_router/packet_router.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_offer_service_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_observer.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
using vac::container::literals::operator""_sv;

/*!
 * \brief Required Service Instance.
 * \details This class represents SomeIpRequiredServiceInstance.
 *          It shall do the service discovery for required services.
 */
class RequiredServiceInstance final : public RequiredServiceInstanceInterface {
 private:
  /*!
   * \brief Alias for SomeIpServiceInterfaceDeployment type.
   */
  using SomeIpServiceInterfaceDeployment = configuration::types::SomeIpServiceInterfaceDeployment;

  /*!
   * \brief Alias for Instance ID type.
   */
  using InstanceId = amsr::someip_protocol::internal::InstanceId;

  /*!
   * \brief Alias for Client Id type.
   */
  using ClientId = amsr::someip_protocol::internal::ClientId;

  /*!
   * \brief Alias for "ServiceDiscoveryOfferServiceObserver" raw pointer.
   */
  using ServiceObserverPtr = std::add_pointer<service_discovery::ServiceDiscoveryOfferServiceObserver>::type;

  /*!
   * \brief Container of service offers update observers.
   */
  using ServiceObserverContainer = ara::core::Vector<ServiceObserverPtr>;

  /*!
   * \brief Alias for ServiceDiscoveryClientInterfaceSharedPtr.
   */
  using ServiceDiscoveryClientInterfaceSharedPtr =
      std::shared_ptr<someip_daemon_core::service_discovery::state_machine::client::ServiceDiscoveryClientInterface>;

 public:
  /*!
   * \brief Constructor
   * \param[in] service_interface_deployment   Service deployment.
   * \param[in] instance_id                    Required instance ID.
   * \param[in] service_discovery_client       Service Discovery Client Interface.
   * \param[in] remote_server_factory          The Remote Server factory.
   * \param[in] packet_router                  The packet router.
   * \param[in] client_id_generator            A unique pointer to the client Id generator.
   *
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstance(
      SomeIpServiceInterfaceDeployment const& service_interface_deployment, InstanceId const instance_id,
      std::shared_ptr<someip_daemon_core::service_discovery::state_machine::client::ServiceDiscoveryClientInterface>
          service_discovery_client,
      amsr::UniquePtr<client::RemoteServerFactoryInterface>&& remote_server_factory,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      amsr::UniquePtr<ClientIdGeneratorInterface> client_id_generator) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Destructor
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RequiredServiceInstance() noexcept override;

  RequiredServiceInstance() = delete;
  RequiredServiceInstance(RequiredServiceInstance&&) = delete;
  RequiredServiceInstance(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance&&) = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief     Called when a service has been offered
   *
   * \param[in] instance_id The instance identifier of the service.
   *                        This parameter can be instance ID ALL
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \pre       The service instance must not be already offered
   * \context   Network
   * \steady    FALSE
   */
  void OnOfferService(
      InstanceId const instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const& remote_server_address) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief     Called when a service is no longer offered offered
   *
   * \param[in] instance_id The instance identifier of the service.
   *
   * \pre       The service instance must be already offered
   * \context   Network | Timer
   * \steady    FALSE
   */
  void OnStopOfferService(InstanceId const instance_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return a container(map) with the offered instance id and the server address.
   *
   * \details If the configured instance id is kInstanceIdAll, this API will return a list of all offered instance ids
   * that match the deployment id. Otherwise, a container with at most one element will be returned if the specific
   * instance id has been offered.
   *
   * \return A container of offered services
   * \steady FALSE
   *
   * \context   App
   */
  RequiredServiceInstanceInterface::ServiceInstanceAddressMap const& GetOfferedServices() const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Start service discovery
   *
   * \param[in] requestor The service observer
   *
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  void StartServiceDiscovery(ServiceObserverPtr const requestor) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Stop service discovery
   *
   * \param[in] requestor The service observer
   *
   * \pre       The service must have been requested at least once
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  void StopServiceDiscovery(ServiceObserverPtr const requestor) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Request the service.
   * \details Request a service from the remote server by the connected application.
   *
   * \param[in] requested_instance_id  The specific instance id requested by the application.
   *                                   It must not be kInstanceIdAll
   *
   * \pre    requested_instance_id must not be kInstanceIdAll.
   * \pre    If this object is not configured with kInstanceIdAll, requested_instance_id must coincide with the
   *         configured instance id.
   *
   * \return The newly allocated SOME/IP client identifier if the operation is successful and error otherwise.
   * \error SomeIpDaemonErrc::remote_server_not_found If no remote server is found (service is not configured).
   * \error SomeIpDaemonErrc::client_ids_overflow If the client Id generator reaches the maximum number of simultaneous
   * clients.
   * \context App
   * \steady  FALSE
   */
  ara::core::Result<amsr::UniquePtr<client::LocalClientInterface>> RequestService(
      someip_protocol::internal::InstanceId const requested_instance_id) noexcept override;

 private:
  /*!
   * \brief Unregister an observer for service offers updates.
   *
   * \param[in] observer A service discovery offer service observer.
   *
   * \pre       The observer must be already registered
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  void UnRegisterServiceDiscoveryOfferServiceObserver(ServiceObserverPtr const observer) noexcept;

  /*!
   * \brief A container of registered service offers observers.
   */
  ServiceObserverContainer offer_service_observers_{};

  /*!
   * \brief The required service interface deployment.
   */
  SomeIpServiceInterfaceDeployment const service_interface_deployment_;

  /*!
   * \brief The required instance ID.
   */
  InstanceId const instance_id_;

  /*!
   * \brief The instance that is handling SOME/IP SD Protocol for this Required Service Instance, or nullptr if no SD
   * is configured.
   */
  ServiceDiscoveryClientInterfaceSharedPtr service_discovery_client_;

  /*!
   * \brief The remote server manager for this required service instance.
   * \note Each RequiredServiceInstance has its own RemoteServerManager. In a scenario where a specific instance id is
   *       configured, this RemoteServerManager will hold at most one RemoteServer. In a scenario where Instance Id ALL
   *       is configured, this RemoteServerManager may hold multiple RemoteServer instances, one per found and requested
   *       Instance Id.
   */
  client::RemoteServerManager remote_server_manager_;

  /*!
   * \brief Reference to the packet router.
   */
  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router_;

  /*!
   * \brief Unique pointer to the client Id generator.
   */
  amsr::UniquePtr<ClientIdGeneratorInterface> client_id_generator_;

  /*!
   * \brief The offered services.
   * \details This is emplace with the exact found service ID once the remote service is offered.
   *          It is remove once the remote service is stopped.
   */
  RequiredServiceInstanceInterface::ServiceInstanceAddressMap offered_services_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("RequiredServiceInstance"_sv,
                                                      service_interface_deployment_.deployment_id, instance_id_)};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_H_
