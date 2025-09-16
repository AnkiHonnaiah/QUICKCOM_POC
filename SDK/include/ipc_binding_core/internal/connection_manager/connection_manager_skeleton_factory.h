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
 *        \brief  Constructs a connection manager skeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor_factory_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Constructs a connection manager skeleton.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 */
class ConnectionManagerSkeletonFactory final : public ConnectionManagerSkeletonFactoryInterface {
 public:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ConnectionManagerSkeletonFactory() noexcept = default;

  /*!
   * \brief Default destroys the interface.
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
  ~ConnectionManagerSkeletonFactory() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConnectionManagerSkeletonFactory(ConnectionManagerSkeletonFactory const&) noexcept = delete;
  auto operator=(ConnectionManagerSkeletonFactory const&) noexcept -> ConnectionManagerSkeletonFactory& = delete;
  ConnectionManagerSkeletonFactory(ConnectionManagerSkeletonFactory&&) noexcept = delete;
  auto operator=(ConnectionManagerSkeletonFactory&&) noexcept -> ConnectionManagerSkeletonFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerSkeletonFactoryInterface::Create
   */
  auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor, SkeletonRouterInterface& skeleton_router,
              trace::TraceMonitor const& trace_monitor, access_control::AccessControlInterface const& access_control,
              amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_skeleton_factory,
              amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerSkeletonInterface> final;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_H_
