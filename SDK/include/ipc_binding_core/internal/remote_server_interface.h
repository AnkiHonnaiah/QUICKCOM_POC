
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
/**        \file
 *        \brief  RemoteServer Interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/connection_manager/connection_state_change_handler_interface.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/service_discovery/service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
/*!
 * \brief Interface for the RemoteServer.
 *
 * \unit IpcBinding::IpcBindingCore::RemoteServer
 */
class RemoteServerInterface
    : public ::amsr::ipc_binding_core::internal::connection_manager::ConnectionStateChangeHandlerInterface,
      public ::amsr::ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface {
 public:
  RemoteServerInterface(RemoteServerInterface const&) noexcept = delete;
  RemoteServerInterface(RemoteServerInterface&&) noexcept = delete;
  auto operator=(RemoteServerInterface const&) noexcept -> RemoteServerInterface& = delete;
  auto operator=(RemoteServerInterface&&) noexcept -> RemoteServerInterface& = delete;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RemoteServerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RemoteServerInterface() noexcept override = default;

  /*!
   * \brief Request the service instance.
   *
   * \param[in] client_id  A unique client ID to represent an instance of ProxyXf.
   * \param[in] router     A proxyRouter corresponding to the ProxyXf.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Request(ipc_binding_core::internal::ipc_protocol::ClientId client_id,
                       ipc_binding_core::internal::ProxyRouterInterface& router) noexcept -> void = 0;

  /*!
   * \brief Release the service instance.
   *
   * \param[in] client_id  A unique client ID to represent an instance of ProxyXf.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Release(ipc_binding_core::internal::ipc_protocol::ClientId client_id) noexcept -> void = 0;

  /*!
   * \brief   Get the current connection state to the remote server.
   * \details Be aware that this API only returns the currently known connection state. The connection can be already
   *          terminated with the next reactor cycle.
   *
   * \return True if connection to the remote server is established. Otherwise false.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto IsConnected() const noexcept -> bool = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_INTERFACE_H_
