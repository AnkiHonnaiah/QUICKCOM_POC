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
 *         \brief Implements a message dispatcher for transmitting messages to the skeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Dispatches messages send via the ConnectionProxy.
 * \details Each ConnectionProxy has its own dispatcher to forward incoming messages. The message handler decodes the
 *          message and forwards it to the packet router.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class TransmitMessageHandler final : public TransmitMessageHandlerInterface {
 public:
  /*!
   * \brief Initializes all the resources of TransmitMessageHandler.
   *
   * \param[in,out] connection_proxy  The ConnectionProxy. The object must be valid for TransmitMessageHandler's entire
   *                                  life time.
   * \param[in,out] logger            Reference to the logger that will be used for this ConnectionSkeleton.
   * \param[in]     trace_monitor     The trace monitor.
   * \param[in]     access_control    Reference to an access control implementation. The object must be valid for the
   *                                  entire lifetime of this object.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  TransmitMessageHandler(ConnectionProxyInterface& connection_proxy,
                         ::amsr::ipc_binding_core::internal::logging::AraComLogger& logger,
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
   *
   * \spec requires true; \endspec
   */
  ~TransmitMessageHandler() noexcept final = default;  // VCA_IPCB_MOLE_1298

  TransmitMessageHandler(TransmitMessageHandler const&) noexcept = delete;
  auto operator=(TransmitMessageHandler const&) noexcept -> TransmitMessageHandler& = delete;
  TransmitMessageHandler(TransmitMessageHandler&&) noexcept = delete;
  auto operator=(TransmitMessageHandler&&) noexcept -> TransmitMessageHandler& = delete;

  // ---- APIs from TransmitMessageHandlerInterface ----

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface::SendRequest
   *
   * \spec requires true; \endspec
   */
  auto SendRequest(ipc_protocol::RequestMessage&& request) noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface::SendRequestNoReturn
   *
   * \spec requires true; \endspec
   */
  auto SendRequestNoReturn(ipc_protocol::RequestNoReturnMessage&& request) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface::SubscribeEvent
   *
   * \spec requires true; \endspec
   */
  auto SubscribeEvent(ipc_protocol::SubscribeEventMessageHeader const& header) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface::UnsubscribeEvent
   *
   * \spec requires true; \endspec
   */
  auto UnsubscribeEvent(ipc_protocol::UnsubscribeEventMessageHeader const& header) noexcept -> void final;

  /*!
   * \brief Set peer id of the connected partner.
   *
   * \param[in] peer_id  Connected peer identification.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetPeerId(amsr::ipc::Credentials peer_id) noexcept;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the log builder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Back reference to the ConnectionProxy.
   */
  ConnectionProxyInterface& connection_proxy_;

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
   * \brief Peer id of the connected partner.
   */
  amsr::ipc::Credentials peer_id_{0};
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_H_
