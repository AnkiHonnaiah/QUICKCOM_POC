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
 *        \brief  Defines an interface to construct a ConnectionSkeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/connection_manager/server_interface.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Defines an interface to create a ConnectionSkeleton.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionSkeleton
 */
class ConnectionSkeletonFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionSkeletonFactoryInterface() noexcept = default;

  ConnectionSkeletonFactoryInterface(ConnectionSkeletonFactoryInterface const&) noexcept = delete;
  auto operator=(ConnectionSkeletonFactoryInterface const&) noexcept -> ConnectionSkeletonFactoryInterface& = delete;
  ConnectionSkeletonFactoryInterface(ConnectionSkeletonFactoryInterface&&) noexcept = delete;
  auto operator=(ConnectionSkeletonFactoryInterface&&) noexcept -> ConnectionSkeletonFactoryInterface& = delete;

  /*!
   * \brief Constructs a ConnectionSkeleton and returns a shared pointer to its interface.
   *
   * \param[in,out] reactor          Used Reactor.
   * \param[in,out] server           The server which accepted IPC connection. The object must be valid for
   *                                 ConnectionSkeleton's entire life time.
   * \param[in,out] skeleton_router  The skeleton router that shall be used by ConnectionSkeleton. The object must be
   *                                 valid for this object's entire life time.
   * \param[in,out] connection       Unique pointer to the accepted IPC connection.
   * \param[in,out] trace_monitor    The trace monitor. The object must be valid for ConnectionSkeleton's entire life
   *                                 time.
   * \param[in]     access_control   Reference to an access control implementation. The object must be valid for the
   *                                 entire lifetime of this object.
   *
   * \return Shared pointer to a ConnectionSkeleton interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Create(ReactorInterface& reactor, ServerInterface& server, SkeletonRouterInterface& skeleton_router,
                      amsr::UniquePtr<GenericConnection>&& connection, trace::TraceMonitor const& trace_monitor,
                      access_control::AccessControlInterface const& access_control) noexcept
      -> amsr::SharedPtr<ConnectionSkeletonInterface> = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionSkeletonFactoryInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_INTERFACE_H_
