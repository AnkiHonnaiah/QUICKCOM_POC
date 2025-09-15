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
/**        \file  remote_server_connection_state_change_handler.h
 *        \brief  Interface to handle connection state changes of a remote server
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_STATE_CHANGE_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_STATE_CHANGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RemoteServerConnectionStateChangeHandler.
 * \details Allows triggering a RemoteServer to handle a connection state change
 */
class RemoteServerConnectionStateChangeHandler {
 public:
  /*!
   * \brief             Constructs a default instance of Connection State Change Handler.
   * \pre               -
   * \context           Init
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  RemoteServerConnectionStateChangeHandler() noexcept = default;

  /*!
   * \brief             Destructs instance
   * \pre               -
   * \reentrant         FALSE
   * \steady            FALSE
   */
  virtual ~RemoteServerConnectionStateChangeHandler() noexcept = default;

  RemoteServerConnectionStateChangeHandler(RemoteServerConnectionStateChangeHandler const &) = delete;
  RemoteServerConnectionStateChangeHandler(RemoteServerConnectionStateChangeHandler &&) = delete;
  RemoteServerConnectionStateChangeHandler &operator=(RemoteServerConnectionStateChangeHandler const &) & = delete;
  RemoteServerConnectionStateChangeHandler &operator=(RemoteServerConnectionStateChangeHandler &&) & = delete;

  /*!
   * \brief Handles connection establishment.
   *
   * \context Network
   * \steady  FALSE
   */
  virtual void HandleConnectionEstablished() noexcept = 0;

  /*!
   * \brief Handles connection closure.
   *
   * \details Updates the service offering to match the connections. Called when any connection changes to closed.
   *
   * \context Network
   * \steady  FALSE
   */
  virtual void HandleConnectionClosed() noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_STATE_CHANGE_HANDLER_H_
