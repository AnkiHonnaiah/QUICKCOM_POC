/**********************************************************************************************************************
 * COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 * Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *               This software is copyright protected and proprietary to Vector Informatik GmbH.
 *               Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *               All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 * FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *       \brief  Interface for the skeleton backend router.
 *
 *     \details
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_BACKEND_ROUTER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_BACKEND_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief   The packet router for the IPC binding.
 * \details The packet router is responsible for:
 *          - Routing of requests (incl. fire & forget).
 *          - Routing of responses.
 *          - Routing of event notifications.
 *          - Routing of error messages.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterSkeleton
 */
class SkeletonBackendRouterInterface {
 public:
  /*!
   * \brief Construct the packet router with empty routing tables.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  SkeletonBackendRouterInterface() noexcept = default;

  /*!
   * \brief Destruct the SkeletonBackendRouterInterface.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~SkeletonBackendRouterInterface() noexcept = default;

  SkeletonBackendRouterInterface(SkeletonBackendRouterInterface const&) noexcept = delete;
  auto operator=(SkeletonBackendRouterInterface const&) noexcept -> SkeletonBackendRouterInterface& = delete;
  SkeletonBackendRouterInterface(SkeletonBackendRouterInterface&&) noexcept = delete;
  auto operator=(SkeletonBackendRouterInterface&&) noexcept -> SkeletonBackendRouterInterface& = delete;

  /*!
   * \brief Realization to forward a method to the method xf backends.
   *
   * \param[in,out] request  The request, which is going to be dispatched to a method xf backend for deserialization and
   *                         processing.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  virtual auto OnRequestReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RemoteRequestMessage&& request) noexcept -> void = 0;

  /*!
   * \brief Realization to forward a fire & forget method to the method xf backends.
   *
   * \param[in,out] request  The request, which is going to be dispatched to a method xf backend for deserialization and
   *                         processing.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  virtual auto OnRequestNoReturnReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage&& request) noexcept -> void = 0;

  /*!
   * \brief Realization to forward a subscribe request from the client and forward it to the designated event/field
   *        notification manager.
   *
   * \param[in]     header          The subscription message header from the client.
   * \param[in]     connection_id   The id of the receiving connection.
   * \param[in,out] connection_ptr  A shared pointer to the receiving connection.
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
  virtual auto OnSubscribeEventReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::SubscribeEventMessageHeader const& header,
      connection_manager::ConnectionId connection_id, ConnectionSkeletonPtr& connection_ptr) noexcept -> void = 0;

  /*!
   * \brief Realization to forward an unsubscribe request from the client and forward it to the designated event / field
   *        notification manager.
   *
   * \param[in] header         The unsubscription message header from the client.
   * \param[in] connection_id  The id of the receiving connection.
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
  virtual auto OnUnsubscribeEventReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::UnsubscribeEventMessageHeader const& header,
      connection_manager::ConnectionId connection_id) noexcept -> void = 0;

  /*!
   * \brief   Start offering of the service.
   * \details The service session id is increased.
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
  virtual auto StartOffering() noexcept -> void = 0;

  /*!
   * \brief   Stop offering of the service.
   * \details The service session id is increased.
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
  virtual auto StopOffering() noexcept -> void = 0;

  /*!
   * \brief Removes the connection id from the subscriber list of each managed event backends.
   *
   * \param[in] connection_id  The id of the connection that will be removed.
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

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SKELETON_BACKEND_ROUTER_INTERFACE_H_
