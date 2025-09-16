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
 *        \brief  Interface for a generic ipc connection.
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_CONNECTION_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/ipc/connection.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Generic connection interface class.
 * \details Provides a generic interface for an IPC connection.
 */
class GenericConnection {
 public:
  /*!
   * \brief Destroys this object. The Connection is closed by this destructor if it is open.
   *
   * \pre         No callback (ConnectCompletionCallback / MessageAvailableCallback / ReceiveCompletionCallback /
   *              SendCompletionCallback) is currently executing. If you are not sure that this is not the case,
   *              IsInUse() can be used after the Connection was closed to check if any callback is still executing.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~GenericConnection() noexcept = default;

  GenericConnection(GenericConnection&& other) noexcept = delete;
  auto operator=(GenericConnection&& other) noexcept -> GenericConnection& = delete;
  GenericConnection(GenericConnection const&) noexcept = delete;
  auto operator=(GenericConnection const&) noexcept -> GenericConnection& = delete;

  /*!
   * \brief   Triggers the reception of the next incoming message.
   * \details The message will be received asynchronously.
   *
   * \param[in] msg_available_callback       Callback that is called as soon as a message is received. The callback must
   *                                         return a view to the buffer where the message content should be copied to.
   *                                         The Connection takes ownership of the passed callback. It holds the
   *                                         ownership until the callback has been executed or until the Connection is
   *                                         closed.
   * \param[in] receive_completion_callback  Callback to notify about the result of the reception. The Connection takes
   *                                         ownership of the passed callback. It holds the ownership until the callback
   *                                         has been executed or until the Connection is closed.
   *
   * \error osabstraction::OsabErrc::kUninitialized  Tried to receive from not connected connection.
   * \error osabstraction::OsabErrc::kBusy           An asynchronous receive operation is currently in progress.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto ReceiveAsync(amsr::ipc::MessageAvailableCallback&& msg_available_callback,
                            amsr::ipc::ReceiveCompletionCallback&& receive_completion_callback) noexcept
      -> ara::core::Result<void> = 0;

  /*!
   * \brief   Connects the Connection asynchronously to a remote address.
   * \details The operation is performed asynchronously. The completion callback informs about the completed connect
   *          process. The completion callback may never get called if the server does not continue the connection
   *          establishment protocol. If this error case is critical for the application, a timer should be used and the
   *          ConnectAsync() operation aborted after timeout with a call to Close().
   *
   * \param[in] remote_address  Unicast address of the remote server to connect to.
   * \param[in] callback        Callback to notify about the completion of the connect operation. The Connection takes
   *                            ownership of the passed callback. It holds the ownership until the callback has been
   *                            executed or until the Connection is closed.
   *
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected        Tried to connect already connected connection.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The server is not available.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to connect to server. This can also happen if
   *                                                          the server is not completely initialized yet.
   * \error osabstraction::OsabErrc::kDisconnected            (Linux only) The peer aborted the connection establishment
   *                                                          or is not available anymore.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  The system environment is in an unexpected state. This
   *                                                          might be caused by:
   *                                                          - A file system I/O error or corruption.
   *                                                          - A system service is not available.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to perform connect operation.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  virtual auto ConnectAsync(amsr::ipc::UnicastAddress remote_address,
                            amsr::ipc::ConnectCompletionCallback&& callback) noexcept -> ara::core::Result<void> = 0;

  /*!
   * \brief   Tries to send a message.
   * \details First the function tries to send a message synchronously. If not possible the message will be sent
   *          asynchronously. If the message is sent asynchronously, it is not specified how much data the peer has to
   *          receive before the asynchronous send operation can complete. Note that HandleEvents() of the Reactor of
   *          this Connection has to be called for peer disconnects to be discovered. If HandleEvents() is never called
   *          (e.g. because Send() is expected to always complete synchronously), Send() will never return
   *          osabstraction::OsabErrc::kDisconnected even if the peer has closed the Connection or has terminated. If
   *          such a situation exists but disconnects shall still be discovered, HandleEvents() of the Reactor could be
   *          called in a background thread, for example.
   *
   * \param[in] message   View on an IO buffer array that points to the memory segments that contain the message that
   *                      shall be sent. The IO buffer array that this span points to and the memory segments that each
   *                      IO buffer points to have to stay valid until the send operation completed or the Connection is
   *                      closed.
   * \param[in] callback  The callback which is called to report completion of the operation with the result status if
   *                      the message could not be sent synchronously. The Connection takes ownership of the passed
   *                      callback if the message could not be sent synchronously. It holds the ownership until the
   *                      callback has been executed or until the Connection is closed.
   *
   * \return Returns whether the message has been sent synchronously.
   *
   * \error osabstraction::OsabErrc::kUninitialized  Tried to send via not connected connection.
   * \error osabstraction::OsabErrc::kBusy           An asynchronous send operation is currently in progress.
   * \error osabstraction::OsabErrc::kSize           The size of the passed message exceeds the maximum supported
   *                                                 message size.
   * \error osabstraction::OsabErrc::kDisconnected   The connection has been terminated by the peer.
   * \error osabstraction::OsabErrc::kProtocolError  Failed to send/receive notification to/from IPC peer or SafeIPC
   *                                                 protocol violation detected. The connection is broken and should be
   *                                                 closed.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE if kSendCompleted is returned, otherwise FALSE.
   * \steady      FALSE
   */
  virtual auto Send(ara::core::Span<osabstraction::io::ConstIOBuffer> message,
                    amsr::ipc::SendCompletionCallback&& callback) noexcept
      -> ara::core::Result<amsr::ipc::SendResult> = 0;

  /*!
   * \brief   Check the integrity level of the communication peer process.
   * \details The integrity level of a process cannot be configured on Linux and PikeOS. As such, every process is
   *          treated as a QM process on these operating systems and this function then only returns true if the
   *          expected integrity level is QM.
   *
   * \param[in] min_expected_level  Minimum integrity level that the peer process is expected to have.
   *
   * \return True if the peer process has at least the expected integrity level, false otherwise.
   *
   * \error osabstraction::OsabErrc::kUninitialized  Tried to check integrity level of peer process via not connected
   *                                                 connection. This service does not fail with this error if the
   *                                                 connection was once connected but the peer disconnected.
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
  virtual auto CheckPeerIntegrityLevel(amsr::ipc::IntegrityLevel min_expected_level) const noexcept
      -> ara::core::Result<bool> = 0;

  /*!
   * \brief Query the identity of the communication peer.
   *
   * \return Credentials of communication peer.
   *
   * \error osabstraction::OsabErrc::kUnexpected     Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kUninitialized  Tried to get peer's credentials via not connected connection.
   * \error osabstraction::OsabErrc::kApiError       Getting the peer identity is not implemented for this operating
   *                                                 system.
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
  virtual auto GetPeerIdentity() const noexcept -> ara::core::Result<amsr::ipc::Credentials> = 0;

  /*!
   * \brief   Closes the Connection.
   * \details Stops all ongoing asynchronous operations, releases the system resources and puts the Connection back into
   *          closed state. No callback will be called anymore but a callback might currently be ongoing. IsInUse() can
   *          be used after this call to check if a provided callback is still executing. See ~Connection() why such a
   *          check may be necessary.
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
  virtual auto Close() noexcept -> void = 0;

  /*!
   * \brief   Checks if the Connection is in open state or not.
   * \details A client side Connection is in open state after the ConnectAsync() call until the Close() call. A server
   *          side Connection is in open state from the AcceptCompletionCallback onwards until the Close() call.
   *
   * \return True if the Connection is in open state, otherwise false.
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
  virtual auto CheckIsOpen() const noexcept -> bool = 0;

  /*!
   * \brief   Checks if the Connection still uses any provided callback after it was closed.
   * \details This function should be called after the Connection was closed if you want to check if any provided
   *          callback is still executing. Note that this function always returns true if the Connection is still open
   *          (the Connection was once opened but Close() was not called yet). See ~Connection() why such a check may be
   *          necessary.
   *
   * \return True if the Connection is open or any provided callback is currently executing, false otherwise.
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
  virtual auto IsInUse() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericConnection() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_CONNECTION_H_
