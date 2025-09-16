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
/**       \file
 *        \brief  A server is responsible for accepting incoming connections from remote clients and spawning
 *                ConnectionSkeletons from them.
 *
 *        \unit IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/server_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Encapsulate IPC communication services for the Server.
 */
class Server final : public ServerInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Stop accepting connections and release existing connection.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ~Server() noexcept final;

  Server(Server const&) noexcept = delete;
  auto operator=(Server const&) noexcept -> Server& = delete;
  Server(Server&&) noexcept = delete;
  auto operator=(Server&&) noexcept -> Server& = delete;

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
  auto GetAddress() const noexcept -> IpcUnicastAddress final;

  /*!
   * \brief Registers a provided service instance with the server.
   *
   * \param[in] service  Valid identifier of a provided service instance.
   *
   * \return Void result on success otherwise error containing the reason.
   *
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
   */
  auto AddProvidedServiceInstance(ProvidedServiceInstanceId const& service) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Unregisters a provided service instance with the server.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  void RemoveProvidedServiceInstance(ProvidedServiceInstanceId const& service) noexcept;

  /*!
   * \brief Checks whether a provided service instance is already registered with the server.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \return True if the given provided service instance is registered with the server, false otherwise.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto HasProvidedServiceInstance(ProvidedServiceInstanceId const& service) const noexcept -> bool;
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
  void OnDisconnect(ConnectionSkeletonSharedPtr connection) noexcept final;

  /*!
   * \brief Creates a new server object an returns a unique pointer to it.
   *
   * \param[in,out] reactor             The reactor interface. The object must be valid for Server's entire life time.
   * \param[in]     address             The IPC unicast address on which the server shall listen for incoming
   *                                    connections.
   * \param[in,out] skeleton_router     The skeleton router that shall be used by ConnectionSkeleton. The object must be
   *                                    valid for Server's entire life time.
   * \param[in]     acceptor_factory    Constructs a new generic acceptor.
   * \param[in,out] trace_monitor       The trace monitor.
   * \param[in]     access_control      Reference to an access control implementation. The object must be valid for the
   *                                    entire lifetime of this object.
   * \param[in]     integrity_level     The configured IntegrityLevel for the server.
   * \param[in]     connection_factory  Constructs a new ConnectionSkeleton.
   *
   * \return Unique pointer to new Server object.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto Create(ReactorInterface& reactor, SkeletonRouterInterface& skeleton_router,
                     IpcUnicastAddress const& address,
                     amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory,
                     trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control, IntegrityLevel integrity_level,
                     amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_factory) noexcept
      -> amsr::UniquePtr<Server>;

 private:
  /*!
   * \brief Type alias for the logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the log builder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief The container type for the provided service instances.
   */
  using ProvidedServiceInstanceContainer = ara::core::Vector<ProvidedServiceInstanceId>;

  /*!
   * \brief The container type for the ConnectionSkeletons.
   */
  using ConnectionSkeletonContainer = ara::core::Vector<ConnectionSkeletonSharedPtr>;

  /*!
   * \brief Type alias for Reactor callback handle.
   */
  using ReactorCallbackHandle = osabstraction::io::reactor1::CallbackHandle;

  /*!
   * \brief Type alias for Reactor event types.
   */
  using ReactorEventTypes = ::osabstraction::io::reactor1::EventTypes;

  /*!
   * \brief Initialize all the resources of the server.
   *
   * \param[in,out] reactor             The reactor interface. The object must be valid for Server's entire life time.
   * \param[in]     address             The IPC unicast address on which the server shall listen for incoming
   *                                    connections.
   * \param[in,out] skeleton_router     The skeleton router that shall be used by ConnectionSkeleton. The object must be
   *                                    valid for Server's entire life time.
   * \param[in]     acceptor_factory    Constructs a new generic acceptor. Must contain a valid pointer.
   * \param[in,out] trace_monitor       The trace monitor.
   * \param[in]     access_control      Reference to an access control implementation. The object must be valid for the
   *                                    entire lifetime of this object.
   * \param[in]     integrity_level     The configured IntegrityLevel for the server.
   * \param[in,out] connection_factory  Constructs a new ConnectionSkeleton. Must contain a valid pointer.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  Server(osabstraction::io::reactor1::Reactor1Interface& reactor, SkeletonRouterInterface& skeleton_router,
         IpcUnicastAddress const& address, amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory,
         trace::TraceMonitor const& trace_monitor, access_control::AccessControlInterface const& access_control,
         IntegrityLevel integrity_level,
         amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_factory) noexcept;

  /*!
   * \brief Build the used logger incl. a prefix visualizing the unicast address.
   *
   * \param[in] address  Unicast address.
   *
   * \return Constructed logger.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildLogger(IpcUnicastAddress const& address) noexcept -> Logger;

  /*!
   * \brief Starts accepting connections.
   *
   * \return Result of the operation to accept the connection.
   *
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
   */
  auto StartAcceptingConnections() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Triggers the asynchronous accept operation on the IPC acceptor object.
   *
   * \return Result of the operation to accept the connection.
   *
   * \error osabstraction::OsabErrc::kDisconnected  Internal Acceptor object is not listening to connect requests
   *                                                anymore until the Acceptor is closed and reinitialized.
   *
   * \pre       The connection_acceptor_ must have been initialized.
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto AcceptNextConnection() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Notifies about a finished asynchronous accept operation.
   *
   * \param[in] result  The result of the asynchronous operation. It either contains the newly established connection or
   *                    an error code.
   *
   * \pre       The connection_acceptor_ must have been initialized.
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  void OnAcceptCompletionCallback(amsr::core::Result<amsr::UniquePtr<GenericConnection>>&& result) noexcept;

  /*!
   * \brief   Trigger asynchronous reception of the given IPC connection.
   * \details Called when a new potential IPC connection has been established to our server.
   *
   * \param[in] connection  Represents a new incoming IPC connection.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  void ConnectionAccepted(amsr::UniquePtr<GenericConnection>&& connection) noexcept;

  /*!
   * \brief Stops accepting connections and closes all established connections.
   *
   * \pre       Reactor shall not be busy.
   * \context   ANY (The calling thread should be synchronized with Reactor thread).
   * \reentrant FALSE
   * \steady    FALSE
   */
  void StopAcceptingConnections() noexcept;

  /*!
   * \brief Finds a provided service instance by its identifier.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \return The iterator pointing to the corresponding provided service instance or end-iterator otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto FindProvidedServiceInstance(ProvidedServiceInstanceId const& service) const noexcept
      -> ProvidedServiceInstanceContainer::const_iterator;

  /*!
   * \brief A reference to an IPC Service Discovery.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief Handle of reactor software event for cleaning terminated connections.
   */
  ReactorCallbackHandle reactor_handle_cleaning_terminated_connections_{};

  /*!
   * \brief The local IPC unicast address of the server.
   */
  IpcUnicastAddress const address_;

  /*!
   * \brief IntegrityLevel required for the server.
   */
  IntegrityLevel const expected_client_integrity_level_;

  /*!
   * \brief Our logger.
   */
  Logger logger_;

  /*!
   * \brief The container of all provided service instances.
   */
  ProvidedServiceInstanceContainer provided_service_instances_{};

  /*!
   * \brief The container of all ConnectionSkeletons established to our server.
   */
  ConnectionSkeletonContainer connection_skeletons_{};

  /*!
   * \brief The container of all terminated ConnectionSkeletons.
   */
  ConnectionSkeletonContainer terminated_connections_{};

  /*!
   * \brief The IPC connection acceptor.
   */
  amsr::UniquePtr<GenericAcceptor> connection_acceptor_{};

  /*!
   * \brief Constructs a new IPC acceptor.
   */
  amsr::SharedPtr<GenericAcceptorFactoryInterface> const acceptor_factory_{};

  /*!
   * \brief Reference to the trace monitor.
   */
  trace::TraceMonitor const& trace_monitor_;

  /*!
   * \brief Reference to the access control.
   */
  access_control::AccessControlInterface const& access_control_;

  /*!
   * \brief Factory to create new ConnectionSkeletons.
   */
  amsr::SharedPtr<ConnectionSkeletonFactoryInterface> const connection_skeleton_factory_{};

  /*!
   * \brief Skeleton router for routing messages to the corresponding skeleton backend routers.
   */
  SkeletonRouterInterface& skeleton_router_;
};

/*!
 * \brief   Logs a BasicIpc acceptor initialization error.
 * \details Logs an appropriate error message. Aborts if it is a development error and the error shows that the state
 *          machine has been corrupted.
 *
 * \param[in] logger    Error messages will be logged to this logger.
 * \param[in] error     BasicIpc error code.
 * \param[in] location  Origin of the log output.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE if access to logger is synchronized.
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
void LogBasicIpcAcceptorInitError(::amsr::ipc_binding_core::internal::logging::AraComLogger const& logger,
                                  amsr::core::ErrorCode const& error,
                                  logging::AraComLogger::LogLocation location) noexcept;

/*!
 * \brief   Logs a BasicIpc accept error returned by the acceptor.
 * \details Logs an appropriate error message. Aborts if it is a development error and the error shows that the state
 *          machine has been corrupted.
 *
 * \param[in] logger    Error messages will be logged to this logger.
 * \param[in] error     BasicIpc error code.
 * \param[in] location  Origin of the log output.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE if access to logger is synchronized.
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
void LogBasicIpcAcceptError(::amsr::ipc_binding_core::internal::logging::AraComLogger const& logger,
                            amsr::core::ErrorCode const& error, logging::AraComLogger::LogLocation location) noexcept;

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_SERVER_H_
