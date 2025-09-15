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
/*!        \file
 *        \brief  The SkeletonRouter holds a map to all SkeletonBackendRouters and routes messages to them.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/string_view.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief The SkeletonRouter holds a map to all SkeletonBackendRouters and routes messages to them.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterSkeleton
 */
class SkeletonRouter final : public SkeletonRouterInterface {
 public:
  /*!
   * \brief Construct the empty router map.
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
  SkeletonRouter() noexcept = default;

  /*!
   * \brief Destruct the router map.
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
  ~SkeletonRouter() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  SkeletonRouter(SkeletonRouter const&) noexcept = delete;
  auto operator=(SkeletonRouter const&) noexcept -> SkeletonRouter& = delete;
  SkeletonRouter(SkeletonRouter&&) noexcept = delete;
  auto operator=(SkeletonRouter&&) noexcept -> SkeletonRouter& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonRouterInterface::GetSkeletonBackendRouter
   */
  auto GetSkeletonBackendRouter(ServiceInstanceIdentifier service_instance) noexcept
      -> ResultSkeletonBackendRouter final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonRouterInterface::RegisterSkeletonBackendRouter
   */
  void RegisterSkeletonBackendRouter(ServiceInstanceIdentifier service_instance,
                                     SkeletonBackendRouterInterface& skeleton_backend_router) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonRouterInterface::DeregisterSkeletonBackendRouter
   */
  void DeregisterSkeletonBackendRouter(ServiceInstanceIdentifier service_instance) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonRouterInterface::RemoveConnection
   */
  void RemoveConnection(connection_manager::ConnectionId connection_id) noexcept final;

 private:
  /*! Type alias for the logger. */
  using Logger = logging::AraComLogger;

  /*! Type alias for the log builder. */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*! Type alias for the skeleton backend router map. */
  using SkeletonBackendRouterMap = ara::core::Map<ServiceInstanceIdentifier, SkeletonBackendRouterInterface&>;

  /*! A map of all registered SkeletonBackendRouter. */
  SkeletonBackendRouterMap skeleton_backend_router_map_{};

  /*! The logger instance. */
  Logger logger_{logging::kIpcLoggerContextId, logging::kIpcLoggerContextDescription,
                 amsr::core::StringView{"SkeletonRouter"}};
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_ROUTER_H_
