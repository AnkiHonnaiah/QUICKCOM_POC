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
/**        \file  remote_server_manager_interface.h
 *        \brief  Remote Server Manager Interface.
 *
 *      \details  This interface offers an API to unregister a RemoteServer.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief An interface to offer a RemoteServer unregistration API.
 * \details Any unit that actively destroys a shared pointer to a RemoteServer must use this API to
 *          notify the implementation that the unit does no longer require ownership of that RemoteServer instance
 */
class RemoteServerManagerInterface {
 public:
  /*!
   * \brief   Constructor of RemoteServerManagerInterface.
   * \steady  FALSE
   */
  RemoteServerManagerInterface() = default;
  /*!
   * \brief   Destructor of RemoteServerManagerInterface.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~RemoteServerManagerInterface() noexcept = default;

  RemoteServerManagerInterface(RemoteServerManagerInterface const&) = delete;
  RemoteServerManagerInterface(RemoteServerManagerInterface&&) = delete;
  RemoteServerManagerInterface& operator=(RemoteServerManagerInterface const&) & = delete;
  RemoteServerManagerInterface& operator=(RemoteServerManagerInterface&&) & = delete;

  /*!
   * \brief Requests the ownership of a RemoteServer, uniquely identified by its service instance id
   *
   * \param[in] exact_instance_id  The service instance id of the remote server to release
   *
   * \context App
   * \steady  FALSE
   */
  virtual std::shared_ptr<RemoteServerInterface> RequestRemoteServer(
      amsr::someip_protocol::internal::InstanceId const exact_instance_id) noexcept = 0;

  /*!
   * \brief Releases the ownership of the caller of a RemoteServer, uniquely identified by its service instance id
   *
   * \param[in] exact_instance_id       The service instance id of the remote server to release
   * \param[in] released_remote_server  The remote server to release
   *
   * \context App
   * \steady  FALSE
   */
  virtual void ReleaseRemoteServer(amsr::someip_protocol::internal::InstanceId const exact_instance_id,
                                   std::shared_ptr<RemoteServerInterface>&& released_remote_server) noexcept = 0;
};
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_MANAGER_INTERFACE_H_
