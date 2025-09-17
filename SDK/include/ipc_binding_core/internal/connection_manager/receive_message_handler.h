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
 *         \brief Implements a message dispatcher for incoming messages from the skeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_RECEIVE_MESSAGE_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_RECEIVE_MESSAGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/deser_ipc_headers.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Dispatches messages received via the ConnectionProxy.
 * \details Each ConnectionProxy has its own dispatcher to forward incoming messages. The message handler decodes the
 *          message and forwards it to the packet router.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ReceiveMessageHandler final {
 public:
  /*!
   * \brief Initializes all the resources of ReceiveMessageHandler.
   *
   * \param[in,out] logger            Reference to the logger that will be used for this ConnectionSkeleton.
   * \param[in,out] trace_monitor     The trace monitor.
   * \param[in]     access_control    Reference to an access control implementation. The object must be valid for the
   *                                  entire lifetime of this object.
   * \param[in,out] router_connector  The router connector. The reference must be valid for entire life time of this
   *                                  class instance.
   *
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ReceiveMessageHandler(::amsr::ipc_binding_core::internal::logging::AraComLogger& logger,
                        trace::TraceMonitor const& trace_monitor,
                        access_control::AccessControlInterface const& access_control,
                        ProxyRouterConnectorInterface& router_connector) noexcept;

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
  ~ReceiveMessageHandler() noexcept = default;

  ReceiveMessageHandler(ReceiveMessageHandler const&) noexcept = delete;
  auto operator=(ReceiveMessageHandler const&) noexcept -> ReceiveMessageHandler& = delete;
  ReceiveMessageHandler(ReceiveMessageHandler&&) noexcept = delete;
  auto operator=(ReceiveMessageHandler&&) noexcept -> ReceiveMessageHandler& = delete;

  /*!
   * \brief Processes the received IPC message.
   *
   * \param[in] packet   Unique pointer to IPC message.
   * \param[in] peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void ProcessReceivedMessage(ipc_protocol::IpcPacketShared&& packet, amsr::ipc::Credentials const& peer_id) noexcept;

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the log builder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Processes an IPC response message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Shared pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void ProcessResponse(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                       amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Processes an IPC error response message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Shared pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void ProcessErrorResponse(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                            amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Processes an IPC notification message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Unique pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void ProcessNotification(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                           amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Processes an IPC application error message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Unique pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  TRUE
   */
  void ProcessApplicationError(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                               amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Processes an IPC event subscription acknowledge message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Unique pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  FALSE
   */
  void ProcessSubscribeEventAck(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                                amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Processes an IPC event subscription negative acknowledge message.
   *
   * \param[in,out] reader   A reader to access the content of the message.
   * \param[in]     packet   Unique pointer to the packet that the reader is currently reading.
   * \param[in]     peer_id  Peer id of the connected partner.
   *
   * \context Reactor
   * \steady  FALSE
   */
  void ProcessSubscribeEventNAck(ipc_protocol::deserialization::Reader& reader, ipc_protocol::IpcPacketShared&& packet,
                                 amsr::ipc::Credentials const& peer_id) noexcept;

  /*!
   * \brief Reference to the router connector.
   */
  ProxyRouterConnectorInterface& router_connector_;

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
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_RECEIVE_MESSAGE_HANDLER_H_
