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
 *        \brief  A server is responsible for accepting incoming connections from remote clients and spawning
 *                ConnectionSkeletons from them.
 *
 *        \unit IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>

#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Encapsulate IPC communication services for the Server.
 */
class ServerInterface {
 public:
  /*!
   * \brief Type alias for IpcUnicastAddress.
   */
  using IpcUnicastAddress = ipc_protocol::IpcUnicastAddress;
  /*!
   * \brief The type alias for connection Integrity Level.
   */
  using IntegrityLevel = amsr::ipc::IntegrityLevel;

  /*!
   * \brief   Shared pointer to a client sink that will be accepted by the server.
   * \details A client sink represents an established connection to a proxy.
   */
  using ConnectionSkeletonSharedPtr = amsr::SharedPtr<ConnectionSkeletonInterface>;

  /*!
   * \brief Default construct interface.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ServerInterface() noexcept = default;

  /*!
   * \brief Stop accepting connections and release existing connection.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ServerInterface() noexcept = default;

  ServerInterface(ServerInterface const&) noexcept = delete;
  ServerInterface(ServerInterface&&) noexcept = delete;
  auto operator=(ServerInterface const&) noexcept -> ServerInterface& = delete;
  auto operator=(ServerInterface&&) noexcept -> ServerInterface& = delete;

  /*!
   * \brief Gets the local IPC unicast address of the server.
   *
   * \return The local IPC unicast address of the server.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto GetAddress() const noexcept -> IpcUnicastAddress = 0;

  /*!
   * \brief Called when a ConnectionSkeletons is terminated by the remote peer.
   *
   * \param[in,out] connection  A pointer to the ConnectionSkeleton which has been disconnected.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto OnDisconnect(ConnectionSkeletonSharedPtr connection) noexcept -> void = 0;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_INTERFACE_H_
