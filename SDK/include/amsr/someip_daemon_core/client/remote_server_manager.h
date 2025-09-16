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
/**        \file  remote_server_manager.h
 *        \brief  Remote Server Manager.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/remote_server_factory_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_manager_interface.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RemoteServerManager.
 */
class RemoteServerManager final : public RemoteServerManagerInterface {
 public:
  /*!
   * \brief RemoteServerManagerConstructor
   * \details This object is constructed once per RequiredServiceInstance to accomodate all remote servers for it.
   *
   * \param[in] remote_server_factory        The factory used to create remote server objects on demand
   * \param[in] service_instance_deployment  The service instance deployment
   * \param[in] required_instance_id         The required instance id (can be exact or ALL)
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RemoteServerManager(amsr::UniquePtr<client::RemoteServerFactoryInterface>&& remote_server_factory,
                      configuration::types::SomeIpServiceInterfaceDeployment const& service_instance_deployment,
                      amsr::someip_protocol::internal::InstanceId const required_instance_id) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief  Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RemoteServerManager() noexcept = default;

  RemoteServerManager(RemoteServerManager const&) = delete;
  RemoteServerManager(RemoteServerManager&&) = delete;
  RemoteServerManager& operator=(RemoteServerManager const&) & = delete;
  RemoteServerManager& operator=(RemoteServerManager&&) & = delete;

 private:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Request a remote server.
   *
   * \param[in] exact_instance_id  The service instance ID of the remote server to request
   *
   * \return A shared pointer to the requested remote server.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  std::shared_ptr<RemoteServerInterface> RequestRemoteServer(
      amsr::someip_protocol::internal::InstanceId const exact_instance_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Release a remote server.
   *
   * \param[in] exact_instance_id       The service instance ID of the remote server to release
   * \param[in] released_remote_server  The remote server to release
   *
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void ReleaseRemoteServer(amsr::someip_protocol::internal::InstanceId const exact_instance_id,
                           std::shared_ptr<RemoteServerInterface>&& released_remote_server) noexcept override;

  /*!
   * \brief Map of remote servers.
   */
  using RemoteServerMap =
      ara::core::Map<amsr::someip_protocol::internal::InstanceId, std::weak_ptr<RemoteServerInterface>>;

  /*!
   * \brief The remote servers.
   */
  RemoteServerMap remote_servers_{};

  /*!
   * \brief A remote server factory, used to create remote servers on demand.
   */
  amsr::UniquePtr<client::RemoteServerFactoryInterface> remote_server_factory_;

  /*!
   * \brief The service instance deployment for the remote servers that will be owned by this unit
   */
  configuration::types::SomeIpServiceInterfaceDeployment const service_instance_deployment_;

  /*!
   * \brief The required service instance id for this remote server manager
   * \note This could be either a specific instance id or ALL. In case of ALL, this container unit may hold multiple
   *       remote servers. Otherwise, it will hold at most one remote server
   */
  amsr::someip_protocol::internal::InstanceId const required_instance_id_;

  /*!
   * \brief The logger
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("RemoteServerManager", service_instance_deployment_.deployment_id,
                                                      required_instance_id_)};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_H_
