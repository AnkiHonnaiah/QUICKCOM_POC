/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  connection_wrapper.h
 *        \brief  Wraps BasicIPC Connection.
 *
 *      \details  Required in order to allow proper mocking of the connection.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTION_WRAPPER_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTION_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/connection.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Wraps the BasicIPC connection.
 * \vprivate Component Private
 */
class ConnectionWrapper {
 public:
  /*!
   * \brief         Constructs wrapper around a connection.
   * \param[in]     connection BasicIPC connection.
   * \context       ANY
   * \pre           \c connection must not be a nullpointer.
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \vprivate      Component Private
   */
  explicit ConnectionWrapper(std::unique_ptr<amsr::ipc::Connection> connection) noexcept;

  /*!
   * \brief         Default destructor.
   * \vprivate      Component Private
   */
  virtual ~ConnectionWrapper() noexcept = default;

  /*!
   * \brief         Deleted copy constructor.
   * \vprivate      Component Private
   */
  ConnectionWrapper(ConnectionWrapper const&) noexcept = delete;

  /*!
   * \brief         Deleted move constructor.
   * \vprivate      Component Private
   */
  ConnectionWrapper(ConnectionWrapper&&) noexcept = delete;

  /*!
   * \brief         Deleted copy assignment operator.
   * \return        -
   * \vprivate      Component Private
   */
  ConnectionWrapper& operator=(ConnectionWrapper const&) & noexcept = delete;

  /*!
   * \brief         Deleted move assignment operator.
   * \return        -
   * \vprivate      Component Private
   */
  ConnectionWrapper& operator=(ConnectionWrapper&&) & noexcept = delete;

  /*!
   * \brief           Checks if the Connection is in open state or not.
   * \details         A client side Connection is in open state after the ConnectAsync() call until the Close() call.
   *                  A server side Connection is in open state from the AcceptCompletionCallback onwards until the
   *                  Close() call.
   * \return          true if the Connection is in open state, otherwise false.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \vprivate        Component Private
   */
  virtual bool CheckIsOpen() const noexcept;

  /*!

   * \brief           Triggers the reception of the next incoming message.
   * \param[in]       msg_available_cb      Callback that is called as soon as a message is received. The
   *                                        callback must return a view to the buffer where the message content
   *                                        should be copied to. The Connection takes ownership of the passed
   *                                        callback. It holds the ownership until the callback has been executed
   *                                        or until the Connection is closed.
   * \param[in]       receive_complete_cb   Callback to notify about the result of the reception. The Connection
   *                                        akes ownership of the passed callback. It holds the ownership until
   *                                        the callback has been executed or until the Connection is closed.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to receive from not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous receive operation is currently in
   *                                                            progress.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \vprivate        Component Private
   */
  virtual amsr::core::Result<void> ReceiveAsync(amsr::ipc::MessageAvailableCallback&& msg_available_cb,
                                                amsr::ipc::ReceiveCompletionCallback&& receive_complete_cb) const;

  /*!
   * \brief           Checks if the Connection still uses any provided callback after it was closed.
   * \details         This function should be called after the Connection was closed if you want to check if any
   *                  provided callback is still executing. Note that this function always returns true if the
   *                  Connection is still open (the Connection was once opened but Close() was not called yet).
   *                  See ~Connection() why such a check may be necessary.
   * \return          true if the Connection is open or any provided callback is currently executing, false otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \vprivate        Component Private
   */
  virtual bool IsInUse() const noexcept;

  /*!
   * \brief           Tries to send a message.
   * \param[in]       message    View on an IO buffer array that points to the memory segments that contain the
   *                             message that shall be sent. The IO buffer array that this span points to and
   *                             the memory segments that each IO buffer points to have to stay valid until the send
   *                             operation completed or the Connection is closed.
   * \param[in]       callback   The callback which is called to report completion of the operation with the result
   *                             status if the message could not be sent synchronously. The Connection takes ownership
   *                             of the passed callback if the message could not be sent synchronously. It holds the
   *                             ownership until the callback has been executed or until the Connection is closed.
   * \return          Returns whether the message has been sent synchronously.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send via not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous send operation is currently in progress.
   * \error           osabstraction::OsabErrc::kSize            The size of the passed message exceeds the maximum
   *                                                            supported message size.
   * \error           osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
   *                                                            SafeIPC protocol violation detected. The connection is
   *                                                            broken and should be closed.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE if kSendCompleted is returned, otherwise FALSE
   * \threadsafe      TRUE
   * \vprivate        Component Private
   */
  virtual amsr::core::Result<amsr::ipc::SendResult> Send(amsr::core::Span<osabstraction::io::ConstIOBuffer> message,
                                                         amsr::ipc::SendCompletionCallback&& callback) const noexcept;

  /*!
   * \brief           Query the identity of the communication peer.
   * \return          Credentials of communication peer.
   * \error           osabstraction::OsabErrc::kUnexpected      (Linux only) Unexpected error that is
   *                                                            mapped to no category.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to get peer's credentials via not connected
   *                                                            connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   * \error           osabstraction::OsabErrc::kApiError        (PikeOS only) Getting the peer identity is not
   *                                                            implemented for this operating system.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \vprivate Component Private
   */
  virtual amsr::core::Result<amsr::ipc::Credentials> GetPeerIdentity() const noexcept;

  /*!
   * \brief           Query the process ID of the communication peer.
   * \return          Process ID of communication peer.
   * \error           osabstraction::OsabErrc::kUnexpected      Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to get peer's PID via not connected connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \vprivate Component Private
   */
  virtual amsr::core::Result<osabstraction::process::ProcessId> GetPeerProcessId() const noexcept;

  /*!
   * \brief           Closes the Connection.
   * \return          -
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \vprivate Component Private
   */
  virtual void Close() const noexcept;

 protected:
  /*!
   * \brief Creates an empty wrapper.
   * \vprivate Component Private
   */
  ConnectionWrapper() = default;

 private:
  /*! Wrapped connection. */
  std::unique_ptr<amsr::ipc::Connection> connection_;
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTION_WRAPPER_H_
