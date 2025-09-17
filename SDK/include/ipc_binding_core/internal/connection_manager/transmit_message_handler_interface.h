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
 *        \brief  Interface of TransmitMessageHandler.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Interface for ConnectionProxy able to send individual proxy-side messages.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class TransmitMessageHandlerInterface {
 public:
  /*!
   * \brief Creates a remote server sink.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  TransmitMessageHandlerInterface() noexcept = default;

  /*!
   * \brief Destroys a remote server sink.
   *
   * \pre       -
   * \context   Reactor | Shutdown.
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~TransmitMessageHandlerInterface() noexcept = default;

  TransmitMessageHandlerInterface(TransmitMessageHandlerInterface const&) noexcept = delete;
  TransmitMessageHandlerInterface(TransmitMessageHandlerInterface&&) noexcept = delete;
  auto operator=(TransmitMessageHandlerInterface const&) noexcept -> TransmitMessageHandlerInterface& = delete;
  auto operator=(TransmitMessageHandlerInterface&&) noexcept -> TransmitMessageHandlerInterface& = delete;

  /*!
   * \brief Send a fire and forget method request message.
   *
   * \param[in] request  The fire & forget method request message to be transmitted.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendRequestNoReturn(ipc_protocol::RequestNoReturnMessage&& request) noexcept -> void = 0;

  /*!
   * \brief Send a method request message.
   *
   * \param[in] request  The method request message to be transmitted.
   *
   * \return True if the concrete TransmitMessageHandlerInterface accepted the request, false if  dropped.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendRequest(ipc_protocol::RequestMessage&& request) noexcept -> bool = 0;

  /*!
   * \brief Send a subscribe event message.
   *
   * \param[in] header  The event subscription header to be transmitted.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      FALSE
   */
  virtual auto SubscribeEvent(ipc_protocol::SubscribeEventMessageHeader const& header) noexcept -> void = 0;

  /*!
   * \brief Send an unsubscribe event message.
   *
   * \param[in] header  The event unsubscription header to be transmitted.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      FALSE
   */
  virtual auto UnsubscribeEvent(ipc_protocol::UnsubscribeEventMessageHeader const& header) noexcept -> void = 0;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_TRANSMIT_MESSAGE_HANDLER_INTERFACE_H_
