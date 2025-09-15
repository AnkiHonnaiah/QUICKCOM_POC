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
 *        \brief  Manages skeleton's connections.
 *
 *      \details  The connection manager is responsible for establishing of accepting incoming connections from remote
 *                clients.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/server.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   IPC connection manager.
 * \details Manages the server connections.
 *
 * \unit       IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 * \complexity Many inter-unit calls to 'libosabstraction' and 'libiostream'.
 */
class ConnectionManagerSkeleton final : public ConnectionManagerSkeletonInterface {
 public:
  /*!
   * \brief The type-alias for Service Instance IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Releases the resources of connection manager.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionManagerSkeleton() noexcept final = default;  //  VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConnectionManagerSkeleton() noexcept = delete;
  ConnectionManagerSkeleton(ConnectionManagerSkeleton const&) noexcept = delete;
  auto operator=(ConnectionManagerSkeleton const&) noexcept -> ConnectionManagerSkeleton& = delete;
  ConnectionManagerSkeleton(ConnectionManagerSkeleton&&) noexcept = delete;
  auto operator=(ConnectionManagerSkeleton&&) noexcept -> ConnectionManagerSkeleton& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerSkeletonInterface::CreateServer
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto CreateServer(IpcUnicastAddress const& address, ProvidedServiceInstanceId const& service,
                    IntegrityLevel integrity_level) noexcept -> amsr::core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerSkeletonInterface::DisconnectServer
   */
  void DisconnectServer(ProvidedServiceInstanceId const& service) noexcept final;

  /*!
   * \brief Create a connection manager skeleton an returns pointer to the newly created object.
   *
   * \param[in,out] reactor                      The reactor interface. The object must be valid for this object's
   *                                             entire life time.
   * \param[in,out] skeleton_router              The skeleton router that shall be used by ConnectionSkeleton. The
   *                                             object must be valid for this objects's entire life time.
   * \param[in,out] trace_monitor                The trace monitor.
   * \param[in]     access_control               Reference to an access control implementation. The object must be valid
   *                                             for the entire lifetime of this object.
   * \param[in,out] connection_skeleton_factory  Valid factory to create a ConnectionSkeleton. It will be shared with
   *                                             each created server instance.
   * \param[in,out] acceptor_factory             Valid factory to create an ipc acceptor. It will be shared with each
   *                                             created server instance.
   *
   * \return Valid unique pointer to a connection manager skeleton interface.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  static auto Create(ReactorInterface& reactor, SkeletonRouterInterface& skeleton_router,
                     trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control,
                     amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_skeleton_factory,
                     amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerSkeletonInterface>;

 private:
  /*!
   * \brief Initializes the resources of connection manager.
   *
   * \param[in,out] reactor                      The reactor interface. The object must be valid for this object's
   *                                             entire life time.
   * \param[in,out] skeleton_router              The skeleton router that shall be used by ConnectionSkeleton. The
   *                                             object must be valid for this objects's entire life time.
   * \param[in,out] trace_monitor                The trace monitor.
   * \param[in]     access_control               Reference to an access control implementation. The object must be valid
   *                                             for the entire lifetime of this object.
   * \param[in,out] connection_skeleton_factory  Valid factory to create a ConnectionSkeleton. It will be shared with
   *                                             each created server instance.
   * \param[in,out] acceptor_factory             Valid factory to create an ipc acceptor. It will be shared with each
   *                                             created server instance.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ConnectionManagerSkeleton(ReactorInterface& reactor, SkeletonRouterInterface& skeleton_router,
                            trace::TraceMonitor const& trace_monitor,
                            access_control::AccessControlInterface const& access_control,
                            amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_skeleton_factory,
                            amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory) noexcept;

  /*!
   * \brief   Determines whether the given service has been already offered.
   * \details Declared protected to support access by test implementation.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \return True if the given service is already offered, false otherwise.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto IsProvidedServiceInstanceAlreadyOffered(ProvidedServiceInstanceId const& service) const noexcept -> bool;

  /*!
   * \brief Type alias for the logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the log builder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief   The server unique pointer type.
   * \details We have to use a unique pointer because the IPC acceptor used inside the Server class is neither copy-able
   *          nor movable.
   */
  using ServerUniquePtr = amsr::UniquePtr<Server>;

  /*!
   * \brief The container type for servers.
   */
  using ServerContainer = ara::core::Vector<ServerUniquePtr>;

  /*!
   * \brief The iterator type for the server container.
   */
  using ServerContainerIterator = ServerContainer::iterator;

  /*!
   * \brief Finds a server by its unique local IPC unicast address.
   *
   * \param[in] address  The unique local IPC unicast address of the server to be found.
   *
   * \return The iterator pointing to the corresponding server, end-iterator otherwise.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto FindServer(IpcUnicastAddress const& address) noexcept -> ServerContainerIterator;
  /*!
   * \brief Finds a server on which the given service has been offered.
   *
   * \param[in] service  The identifier of a provided service instance.
   *
   * \return The iterator pointing to the corresponding server, end-iterator otherwise.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto FindServer(ProvidedServiceInstanceId const& service) noexcept -> ServerContainerIterator;

  /*!
   * \brief Our logger.
   */
  Logger logger_;

  /*!
   * \brief A reference to an IPC Service Discovery.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;
  /*!
   * \brief A container of all active servers.
   */
  ServerContainer servers_{};
  /*!
   * \brief Reference to the trace monitor.
   */
  trace::TraceMonitor const& trace_monitor_;
  /*!
   * \brief Reference to the access control.
   */
  access_control::AccessControlInterface const& access_control_;
  /*!
   * \brief Reference to skeleton router for forwarding requests.
   */
  SkeletonRouterInterface& skeleton_router_;
  /*!
   * \brief Factory to create a ConnectionSkeleton.
   */
  amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_skeleton_factory_;

  /*!
   * \brief Factory to create an acceptor.
   */
  amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_H_
