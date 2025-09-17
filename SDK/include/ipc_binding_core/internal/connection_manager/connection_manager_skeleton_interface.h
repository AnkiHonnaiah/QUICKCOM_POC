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
 *        \brief  Interface to an object that manages skeleton's connections.
 *
 *      \details  The connection manager is responsible for establishing of accepting incoming connections from remote
 *                clients.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/ipc/integrity_level.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Type alias for the local IPC unicast address.
 */
using IpcUnicastAddress = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

/*!
 * \brief   Defines interface to manage all connections of skeletons.
 * \details Manages the server connections.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 */
class ConnectionManagerSkeletonInterface {
 public:
  /*!
   * \brief Releases the resources of connection manager.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionManagerSkeletonInterface() noexcept = default;

  ConnectionManagerSkeletonInterface(ConnectionManagerSkeletonInterface const&) noexcept = delete;
  ConnectionManagerSkeletonInterface(ConnectionManagerSkeletonInterface&&) noexcept = delete;
  auto operator=(ConnectionManagerSkeletonInterface const&) noexcept -> ConnectionManagerSkeletonInterface& = delete;
  auto operator=(ConnectionManagerSkeletonInterface&&) noexcept -> ConnectionManagerSkeletonInterface& = delete;

  /*!
   * \brief   Starts accepting of incoming connections and event subscriptions for the given provided service instance.
   * \details This operation should be executed only once for each provided service instance.
   *
   * \param[in] address          A local IPC unicast address to listen on.
   * \param[in] service          The identifier of a provided service instance.
   * \param[in] integrity_level  The IntegrityLevel for given provided service instance.
   *
   * \return Positive result if the given service instance is successfully registered with the server, error containing
   *         the reason otherwise.
   *
   * \error IpcBindingErrc::error_not_ok                      Provided service instance already offered.
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kDisconnected            Internal Acceptor object is not listening to connect
   *                                                          requests anymore until the Acceptor is closed and
   *                                                          reinitialized.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in use.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  The file system in which the server shall be created does
   *                                                          not support a required operation or a file system I/O
   *                                                          error occurred.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto CreateServer(IpcUnicastAddress const& address, ProvidedServiceInstanceId const& service,
                            amsr::ipc::IntegrityLevel integrity_level) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Stops accepting incoming connections and event subscriptions for the given provided service instance.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \pre       The service instance shall be already offered.
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto DisconnectServer(ProvidedServiceInstanceId const& service) noexcept -> void = 0;

 protected:
  /*!
   * \brief Default construct an interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionManagerSkeletonInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr
#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_INTERFACE_H_
