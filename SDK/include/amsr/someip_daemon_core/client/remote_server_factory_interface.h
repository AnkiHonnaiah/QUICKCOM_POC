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
/**        \file  remote_server_factory_interface.h
 *        \brief  Remote Server Factory Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RemoteServerFactoryInterface.
 */
class RemoteServerFactoryInterface {
 public:
  /*!
   * \brief   Constructor of RemoteServerFactoryInterface.
   * \steady  FALSE
   */
  RemoteServerFactoryInterface() = default;

  /*!
   * \brief   Destructor of RemoteServerFactoryInterface.
   * \steady  FALSE
   */
  virtual ~RemoteServerFactoryInterface() noexcept = default;

  RemoteServerFactoryInterface(RemoteServerFactoryInterface const&) = delete;
  RemoteServerFactoryInterface(RemoteServerFactoryInterface&&) = delete;
  RemoteServerFactoryInterface& operator=(RemoteServerFactoryInterface const&) & = delete;
  RemoteServerFactoryInterface& operator=(RemoteServerFactoryInterface&&) & = delete;

  /*!
   * \brief Shared pointer to a RemoteServerInterface.
   */
  using RemoteServerInterfaceSharedPtr = std::shared_ptr<RemoteServerInterface>;

  /*!
   * \brief       Create the remote server defined in the configuration.
   *
   * \param[in] service_deployment                       The service deployment configuration.
   * \param[in] instance_id                              A SOME/IP service instance identifier.
   *
   * \return      The remote server.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual RemoteServerInterfaceSharedPtr CreateRemoteServer(
      configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
      someip_protocol::internal::InstanceId const instance_id) const noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_INTERFACE_H_
