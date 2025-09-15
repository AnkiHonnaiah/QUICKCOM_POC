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
/**        \file  remote_server_connection_manager_factory.h
 *        \brief  Factory for Remote Server Connection Managers,
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/remote_server_connection_manager.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_manager_factory_interface.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RemoteServerConnectionManagerFactory.
 *
 * \tparam ConnectionManager Type of ConnectionManager
 */
template <typename ConnectionManager>
class RemoteServerConnectionManagerFactory final : public RemoteServerConnectionManagerFactoryInterface {
 public:
  /*!
   * \brief Constructor of RemoteServerConnectionManagerFactory.
   *
   * \param[in] connection_manager                  A connection manager.
   *
   * \context Network | App
   * \steady  FALSE
   */
  explicit RemoteServerConnectionManagerFactory(ConnectionManager& connection_manager) noexcept
      : RemoteServerConnectionManagerFactoryInterface(), connection_manager_{connection_manager} {}

  /*!
   * \brief Construct a RemoteServerConnectionManager and connect to the peer.
   *
   * \param[in] service_deployment_id   The service deployment ID.
   * \param[in] sec_com_config          The SecCom configuration.
   *
   * \return The RemoteServerConnectionManager.
   *
   * \context Network | App
   * \steady  FALSE
   *
   * \internal
   * - Create the result object by calling the constructor
   * - Initiate the asynchronous connection process.
   * - On connection success, return the result object, otherwise return null
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  amsr::UniquePtr<RemoteServerConnectionManagerInterface> CreateRemoteServerConnectionManager(
      configuration::types::SomeIpServiceInterfaceDeploymentId const& service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id,
      configuration::types::RequiredNetworkEndpoint const& network_config,
      configuration::types::ServiceAddress remote_server_address,
      RemoteServerConnectionStateChangeHandler* remote_server, bool const tcp_connection_required,
      bool const udp_connection_required,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept override {
    // Create a RemoteServerConnectionManager
    amsr::UniquePtr<RemoteServerConnectionManager<ConnectionManager>> result{
        RemoteServerConnectionManager<ConnectionManager>::Create(
            service_deployment_id, instance_id, network_config, connection_manager_, remote_server_address,
            remote_server, tcp_connection_required, udp_connection_required, sec_com_config)};

    return result;
  }

  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~RemoteServerConnectionManagerFactory() override = default;
  RemoteServerConnectionManagerFactory() = delete;
  RemoteServerConnectionManagerFactory(RemoteServerConnectionManagerFactory const&) = delete;
  RemoteServerConnectionManagerFactory(RemoteServerConnectionManagerFactory&&) noexcept = default;
  RemoteServerConnectionManagerFactory& operator=(RemoteServerConnectionManagerFactory const&) & = delete;
  RemoteServerConnectionManagerFactory& operator=(RemoteServerConnectionManagerFactory&&) & = delete;

 private:
  /*!
   * \brief   The connection manager.
   */
  ConnectionManager& connection_manager_;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_H_
