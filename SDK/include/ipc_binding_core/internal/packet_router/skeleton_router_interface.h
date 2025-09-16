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
 *        \brief  Interface for SkeletonRouter, routing messages to individual skeleton backend routers.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <type_traits>

#include "amsr/core/result.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/packet_router/skeleton_backend_router_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for SkeletonRouter, routing messages to individual skeleton backend routers.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterSkeleton
 */
class SkeletonRouterInterface {
 public:
  /*!
   * \brief Data type for identifying a skeleton backend router by the ServiceInstanceIdentifier.
   */
  using ServiceInstanceIdentifier = ipc_protocol::ServiceInstanceIdentifier;

  /*!
   * \brief Alias for a result containing a skeleton backend router reference.
   */
  using ResultSkeletonBackendRouter = amsr::core::Result<SkeletonBackendRouterInterface&>;

  /*!
   * \brief Construct the empty router map interface.
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
  SkeletonRouterInterface() noexcept = default;

  /*!
   * \brief Destruct the SkeletonRouterInterface.
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
  virtual ~SkeletonRouterInterface() noexcept = default;

  SkeletonRouterInterface(SkeletonRouterInterface const&) noexcept = delete;
  auto operator=(SkeletonRouterInterface const&) noexcept -> SkeletonRouterInterface& = delete;
  SkeletonRouterInterface(SkeletonRouterInterface&&) noexcept = delete;
  auto operator=(SkeletonRouterInterface&&) noexcept -> SkeletonRouterInterface& = delete;

  /*!
   * \brief Get the skeleton backend router associated with a service instance.
   *
   * \param[in] service_instance  The service instance identifier of the skeleton backend router.
   *
   * \return The skeleton backend router, if found.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual auto GetSkeletonBackendRouter(ServiceInstanceIdentifier service_instance) noexcept
      -> ResultSkeletonBackendRouter = 0;

  /*!
   * \brief Register a skeleton backend router, adding it to the map.
   *
   * \param[in] service_instance         The service instance identifier of the skeleton backend router.
   * \param[in] skeleton_backend_router  Reverence of the skeleton backend router to add to the map.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto RegisterSkeletonBackendRouter(ServiceInstanceIdentifier service_instance,
                                             SkeletonBackendRouterInterface& skeleton_backend_router) noexcept
      -> void = 0;

  /*!
   * \brief Remove a skeleton backend router from the map.
   *
   * \param[in] service_instance  The service instance identifier of the skeleton backend router to remove.
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
  virtual auto DeregisterSkeletonBackendRouter(ServiceInstanceIdentifier service_instance) noexcept -> void = 0;

  /*!
   * \brief Remove connections to the given sink from all skeleton backend routers in the map.
   *
   * \param[in] connection_id  The connection to remove from the routers.
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
  virtual auto RemoveConnection(connection_manager::ConnectionId connection_id) noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_INTERFACE_H_
