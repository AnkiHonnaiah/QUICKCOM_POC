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
 *        \brief  Factory to generate a ConnectionSkeleton object.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_factory_interface.h"
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
 * \brief Constructs a ConnectionSkeleton object.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionSkeleton
 */
class ConnectionSkeletonFactory final : public ConnectionSkeletonFactoryInterface {
 public:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ConnectionSkeletonFactory() noexcept = default;

  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionSkeletonFactory() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConnectionSkeletonFactory(ConnectionSkeletonFactory const&) noexcept = delete;
  auto operator=(ConnectionSkeletonFactory const&) noexcept -> ConnectionSkeletonFactory& = delete;
  ConnectionSkeletonFactory(ConnectionSkeletonFactory&&) noexcept = delete;
  auto operator=(ConnectionSkeletonFactory&&) noexcept -> ConnectionSkeletonFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonFactoryInterface::Create
   */
  auto Create(ReactorInterface& reactor, ServerInterface& server, SkeletonRouterInterface& skeleton_router,
              amsr::UniquePtr<GenericConnection>&& connection, trace::TraceMonitor const& trace_monitor,
              access_control::AccessControlInterface const& access_control) noexcept
      -> amsr::SharedPtr<ConnectionSkeletonInterface> final;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_FACTORY_H_
