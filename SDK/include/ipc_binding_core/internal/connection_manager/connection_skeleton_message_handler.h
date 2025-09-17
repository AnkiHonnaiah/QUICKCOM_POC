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
 *        \brief  Implements a message dispatcher for incoming messages from the proxy.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_MESSAGE_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_MESSAGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/deser_ipc_headers.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Dispatches messages received via the ConnectionSkeleton.
 * \details Each ConnectionSkeleton has its own dispatcher to forward incoming messages. The message handler decodes the
 *          message and forwards it to the packet router.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionSkeleton
 */
class ConnectionSkeletonMessageHandler final {
 public:
  /*!
   * \brief Initializes all the resources of ConnectionSkeletonMessageHandler.
   *
   * \param[in,out] skeleton_router  The skeleton router that shall be used by ConnectionSkeletonMessageHandler. The
   *                                 object must be valid for ConnectionSkeleton's entire life time.
   * \param[in,out] logger           Reference to the logger that will be used for this ConnectionSkeleton.
   * \param[in,out] trace_monitor    The trace monitor.
   * \param[in]     access_control   Reference to an access control implementation. The object must be valid for the
   *                                 entire lifetime of this object.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ConnectionSkeletonMessageHandler(SkeletonRouterInterface& skeleton_router, logging::AraComLogger& logger,
                                   trace::TraceMonitor const& trace_monitor,
                                   access_control::AccessControlInterface const& access_control);

  /*!
   * \brief Destroy the message handler.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ConnectionSkeletonMessageHandler() noexcept = default;

  ConnectionSkeletonMessageHandler(ConnectionSkeletonMessageHandler const&) noexcept = delete;
  auto operator=(ConnectionSkeletonMessageHandler const&) noexcept -> ConnectionSkeletonMessageHandler& = delete;
  ConnectionSkeletonMessageHandler(ConnectionSkeletonMessageHandler&&) noexcept = delete;
  auto operator=(ConnectionSkeletonMessageHandler&&) noexcept -> ConnectionSkeletonMessageHandler& = delete;

  /*!
   * \brief Processes the received IPC message.
   *
   * \param[in] receive_buffer           Incoming IPC packet.
   * \param[in] connection_skeleton_ptr  Shared pointer to ConnectionSkeleton that owns this class.
   * \param[in] peer_id                  Peer id of the connected partner.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  void ProcessReceivedMessage(ipc_protocol::IpcPacketShared&& receive_buffer,
                              ConnectionSkeletonPtr connection_skeleton_ptr,
                              amsr::ipc::Credentials const& peer_id) noexcept;

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = logging::AraComLogger;

  /*!
   * \brief Type alias for the log builder.
   */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*!
   * \brief Processes an IPC request message.
   *
   * \param[in,out] reader                   A reader to access the content of the message.
   * \param[in]     total_message_length     The total length of the message.
   * \param[in]     packet                   Incoming IPC packet.
   * \param[in]     connection_skeleton_ptr  Shared pointer to ConnectionSkeleton that owns this class.
   * \param[in]     peer_id                  Peer id of the connected partner.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   *
   * \spec requires true; \endspec
   */
  void ProcessRequest(ipc_protocol::deserialization::Reader& reader, std::size_t total_message_length,
                      ipc_protocol::IpcPacketShared&& packet, ConnectionSkeletonPtr const& connection_skeleton_ptr,
                      amsr::ipc::Credentials const& peer_id);

  /*!
   * \brief Processes an IPC fire and forget message.
   *
   * \param[in,out] reader                A reader to access the content of the message.
   * \param[in]     total_message_length  The total length of the message.
   * \param[in]     packet                Incoming IPC packet.
   * \param[in]     peer_id               Peer id of the connected partner.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   *
   * \spec requires true; \endspec
   */
  void ProcessRequestNoReturn(ipc_protocol::deserialization::Reader& reader, std::size_t total_message_length,
                              ipc_protocol::IpcPacketShared&& packet, amsr::ipc::Credentials const& peer_id);

  /*!
   * \brief Processes an IPC subscribe to event message.
   *
   * \param[in,out] reader                   A reader to access the content of the message.
   * \param[in]     total_message_length     The total length of the message.
   * \param[in]     connection_skeleton_ptr  Shared pointer to ConnectionSkeleton that owns this class.
   * \param[in]     peer_id                  Peer id of the connected partner.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   *
   * \spec requires true; \endspec
   */
  void ProcessSubscribeEvent(ipc_protocol::deserialization::Reader& reader, std::size_t total_message_length,
                             ConnectionSkeletonPtr& connection_skeleton_ptr, amsr::ipc::Credentials const& peer_id);

  /*!
   * \brief Processes an IPC unsubscribe from event notification message.
   *
   * \param[in,out] reader                   A reader to access the content of the message.
   * \param[in]     total_message_length     The total length of the message.
   * \param[in]     connection_skeleton_ptr  Shared pointer to ConnectionSkeleton that owns this class.
   * \param[in]     peer_id                  Peer id of the connected partner.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   *
   * \spec requires true; \endspec
   */
  void ProcessUnsubscribeEvent(ipc_protocol::deserialization::Reader& reader, std::size_t total_message_length,
                               ConnectionSkeletonPtr const& connection_skeleton_ptr,
                               amsr::ipc::Credentials const& peer_id);

  /*!
   * \brief Our logger.
   */
  Logger& logger_;

  /*!
   * \brief Reference to the trace monitor.
   */
  trace::TraceMonitor const& trace_monitor_;

  /*!
   * \brief Reference to the access control.
   */
  access_control::AccessControlInterface const& access_control_;

  /*!
   * \brief Reference to the skeleton router for routing messages to the corresponding skeleton backend routers.
   */
  SkeletonRouterInterface& skeleton_router_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_MESSAGE_HANDLER_H_
