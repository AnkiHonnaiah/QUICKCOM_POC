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
/**        \file  remote_server_connection_manager_factory_interface.h
 *        \brief  Class declaration of RemoteServerConnectionManagerFactoryInterface
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/remote_server_connection_manager_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_state_change_handler.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Class declaration of RemoteServerConnectionManagerFactoryInterface
 *
 */
class RemoteServerConnectionManagerFactoryInterface {
 public:
  /*!
   * \brief             Constructs a default instance of Remote Server Connection Manager.
   * \pre               -
   * \context           Init
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  RemoteServerConnectionManagerFactoryInterface() noexcept = default;

  /*!
   * \brief             Destructs instance
   * \pre               -
   * \reentrant         FALSE
   * \steady            FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~RemoteServerConnectionManagerFactoryInterface() noexcept = default;

  RemoteServerConnectionManagerFactoryInterface(RemoteServerConnectionManagerFactoryInterface const &) = delete;
  RemoteServerConnectionManagerFactoryInterface(RemoteServerConnectionManagerFactoryInterface &&) = delete;
  RemoteServerConnectionManagerFactoryInterface &operator=(RemoteServerConnectionManagerFactoryInterface const &) & =
      delete;
  RemoteServerConnectionManagerFactoryInterface &operator=(RemoteServerConnectionManagerFactoryInterface &&) & = delete;

  /*!
   * \brief Create a RemoteServerConnectionManager instance. Needed only for testing purposes to
   * inject mocking remote server connection managers.
   *
   * \param[in] service_deployment_id   The service deployment id
   * \param[in] instance_id             The service instance id
   * \param[in] network_config          The network configuration.
   * \param[in] conman                  A connection manager.
   * \param[in] remote_server           A remote server.
   * \param[in] remote_server_address   The remote server address to connect to.
   * \param[in] tcp_connection_required Defines if a remote TCP connection is required or not.
   * \param[in] udp_connection_required Defines if a remote UDP connection is required or not.
   * \param[in] sec_com_config          The SecCom configuration.
   *
   * \return A remote server connection manager.
   *
   * \context App | Network
   * \steady  FALSE
   *
   */
  virtual amsr::UniquePtr<RemoteServerConnectionManagerInterface> CreateRemoteServerConnectionManager(
      configuration::types::SomeIpServiceInterfaceDeploymentId const &service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id,
      configuration::types::RequiredNetworkEndpoint const &network_config,
      configuration::types::ServiceAddress const remote_server_address,
      RemoteServerConnectionStateChangeHandler *remote_server, bool const tcp_connection_required,
      bool const udp_connection_required,
      configuration::ConfigurationTypesAndDefs::SecComConfig const &sec_com_config) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_FACTORY_INTERFACE_H_
