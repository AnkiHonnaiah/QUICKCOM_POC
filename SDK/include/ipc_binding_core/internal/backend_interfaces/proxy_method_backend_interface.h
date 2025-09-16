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
 *        \brief  Defines an interface for proxy method backends.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_METHOD_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for proxy method backends.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ProxyMethodBackendInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyMethodBackendInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ProxyMethodBackendInterface() noexcept = default;

  ProxyMethodBackendInterface(ProxyMethodBackendInterface const&) noexcept = delete;
  ProxyMethodBackendInterface(ProxyMethodBackendInterface&&) noexcept = delete;
  auto operator=(ProxyMethodBackendInterface const&) noexcept -> ProxyMethodBackendInterface& = delete;
  auto operator=(ProxyMethodBackendInterface&&) noexcept -> ProxyMethodBackendInterface& = delete;

  /*!
   * \brief Set the connection state.
   *
   * \param[in] connection_state  Boolean indicating new connection state (connected or disconnected).
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetConnectionState(bool connection_state) noexcept -> void = 0;

  /*!
   * \brief Set the service instance state.
   *
   * \param[in] service_state  Boolean indicating new service state (ServiceUp or ServiceDown).
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetServiceState(bool service_state) noexcept -> void = 0;

  /*!
   * \brief Forwards a response message to its destination.
   *
   * \param[in] response  The response message to pass to the realized sink.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto OnResponseReceived(ipc_protocol::ResponseMessage&& response) noexcept -> void = 0;

  /*!
   * \brief Forwards the application error message to its destination.
   *
   * \param[in] message  The application error message to pass to the realized sink.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto OnApplicationErrorReceived(ipc_protocol::ApplicationErrorMessage&& message) noexcept -> void = 0;

  /*!
   * \brief Forwards the error response message to its destination.
   *
   * \param[in] message  The request error message to pass to the realized sink.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto OnErrorResponseReceived(ipc_protocol::ErrorResponseMessage&& message) noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_METHOD_BACKEND_INTERFACE_H_
