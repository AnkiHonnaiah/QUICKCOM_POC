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
 *        \brief  Provides SafeIPC Acceptor class.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *        \unit   osabstraction::ipc::IpcAcceptor_QNX
 *
 *      \details  The Acceptor class implements a server for SafeIPC clients.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_ACCEPTOR_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_ACCEPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/internal/stream_ipc_protocol_common.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {

namespace internal {
class AcceptorImpl;
}  // namespace internal

/*!
 * \brief   SafeIPC Acceptor class.
 * \details Implements a server for SafeIPC connections.
 */
class Acceptor final {
 public:
  /*!
   * \brief             Notifies about a completed asynchronous accept operation.
   *
   * \details           All Acceptor APIs are allowed to be called from an AcceptCompletionCallback.
   *
   * \param[in] result  The result of the accept operation. On success it contains the accepted connection.
   *                    Possible errors:
   *                    osabstraction::OsabErrc::kResource      (Linux only) Callback slots of the reactor
   *                                                            exhausted.
   *                    osabstraction::OsabErrc::kDisconnected  (Linux only) Acceptor object is not
   *                                                            listening to connect requests anymore until the Acceptor
   *                                                            is closed and reinitialized.
   *                                                            SupportData of error is not set.
   *
   * \context         Reactor1::HandleEvents()
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-SafeIpcReactor,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcServer
   *
   * \vprivate        Vector product internal API
   */
  using AcceptCompletionCallback = amsr::ipc::AcceptCompletionCallback;

  /*!
   * \brief           Constructs an Acceptor object.
   *
   * \details         The Acceptor is not initialized yet. If this constructor is used, a client that does not continue
   *                  the connection establishment protocol will permanently occupy a slot in the connection backlog of
   *                  this Acceptor. The Acceptor will not be able to accept connections anymore if all connection
   *                  backlog slots are occupied by "stuck" connections.
   *
   * \param[in]       reactor          Reactor interface for asynchronous operation. The reactor has to exist for the
   *                                   whole lifetime of this Acceptor object. An Acceptor uses up to 4 Reactor callback
   *                                   slots.
   * \param[in]       server_address   Local address of the acceptor.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcServer,
   *                  DSGN-Osab-SafeIpcReactor
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   *  - Create the AcceptorImpl shared object.
   * \endinternal
   */
  explicit Acceptor(osabstraction::io::reactor1::Reactor1Interface& reactor, UnicastAddress server_address) noexcept;

  /*!
   * \brief           Constructs an Acceptor object.
   *
   * \details         The Acceptor is not initialized yet. If this constructor is used, a client that does not continue
   *                  the connection establishment protocol will only temporarily occupy a slot in the connection
   *                  backlog of this Acceptor. After a timeout the connection establishment for the "stuck" connection
   *                  will be aborted.
   *
   *                  HandleTimerExpiry() of the passed timer manager has to be called from the same thread as
   *                  HandleEvents() of the passed reactor. If this constructor is used, it is not allowed to close or
   *                  destroy this Acceptor from any timer manager callback of the passed timer manager.
   *
   * \param[in]       reactor          Reactor interface for asynchronous operation. The reactor has to exist for the
   *                                   whole lifetime of this Acceptor object. An Acceptor uses up to 3 Reactor callback
   *                                   slots (4 in some multi-threading scenarios).
   * \param[in]       server_address   Local address of the acceptor.
   * \param[in]       timer_manager    Timer manager, used to detect connection establishment timeouts in cases
   *                                   where a client does not continue the connection establishment protocol. The
   *                                   timer manager has to exist for the whole lifetime of the Acceptor object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcServer,
   *                  DSGN-Osab-SafeIpcReactor,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   *  - Create the AcceptorImpl shared object.
   * \endinternal
   */
  explicit Acceptor(osabstraction::io::reactor1::Reactor1Interface& reactor, UnicastAddress server_address,
                    amsr::timer::TimerManager& timer_manager) noexcept;

  /*!
   * \brief           Destroys the Acceptor object. The Acceptor is closed by this destructor if it is open.
   *
   * \context         Not timer_manager::HandleTimerExpiry() if a timer manager is used.
   *
   * \pre             No callback (AcceptCompletionCallback) is currently executing. If you are not sure that this is
   *                  not the case, IsInUse() can be used after the Acceptor was closed to check if a callback is still
   *                  executing.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   *  - Close the Acceptor if it is still open and assert that no user callback is currently executing.
   * \endinternal
   */
  ~Acceptor() noexcept;

  Acceptor(Acceptor const& other) noexcept = delete;
  Acceptor(Acceptor&& other) noexcept = delete;
  Acceptor& operator=(Acceptor const& other) noexcept = delete;
  Acceptor& operator=(Acceptor&& other) noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief          Initializes Acceptor object.
   *
   * \details
   * Starts accepting connection requests from clients in the background and enqueues them in a connection backlog.
   * Call AcceptAsync() to get a callback (AcceptCompletionCallback) when an established connection is
   * available in the connection backlog. The established connection is then returned in the AcceptCompletionCallback
   * and removed from the connection backlog.
   *
   * \param[in]       s2c_buffer_size_hint   Optional parameter that supplies a hint for the maximum size that the
   *                                         clients can request for the buffer used for server-to-client IPC messages
   *                                         (see amsr::ipc::Connection::ConnectAsync() for details on how a client
   *                                         can request the buffer size). If a client requests a larger buffer, the
   *                                         buffer will have the supplied size instead. See the OsAbstraction Technical
   *                                         Reference for details about the used size in case no hint is supplied.
   *                                         Consider that the amount of IPC data that can be stored in these buffers
   *                                         will be smaller than this size due to SafeIPC protocol overhead for each
   *                                         IPC message that is stored in such a shared buffer (see
   *                                         amsr::ipc::kMessageProtocolOverhead and
   *                                         amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                         The SafeIPC implementation does not honor this value if it is below the
   *                                         lower buffer size limit (see OsAbstraction Technical Reference). The
   *                                         actual resulting size of the server-to-client buffer can be queried using
   *                                         amsr::ipc::Connection::GetSendBufferSize() on the accepted connection
   *                                         instance.
   *
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected        Tried to initialize already initialized acceptor.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in use.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  (Linux only) The file system in which the
   *                                                          server shall be created does not support a required
   *                                                          operation or a file system I/O error occurred.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Init(std::size_t s2c_buffer_size_hint = internal::stream_ipc_protocol::GetDefaultBufferSize()) noexcept
      -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief          Initializes Acceptor object.
   *
   * \details
   * Starts accepting connection requests from clients in the background and enqueues them in a connection backlog.
   * Call AcceptAsync() to get a callback (AcceptCompletionCallback) when an established connection is
   * available in the connection backlog. The established connection is then returned in the AcceptCompletionCallback
   * and removed from the connection backlog.
   *
   * \param[in]       s2c_buffer_size_hint   Hint for the maximum size that the clients can request for the buffer used
   *                                         for server-to-client IPC messages (see
   *                                         amsr::ipc::Connection::ConnectAsync() for details on how a client can
   *                                         request the buffer size). If a client requests a larger buffer, the buffer
   *                                         will have the supplied size instead. See the OsAbstraction Technical
   *                                         Reference for details about the used size in case no hint is supplied.
   *                                         Consider that the amount of IPC data that can be stored in these buffers
   *                                         will be smaller than this size due to SafeIPC protocol overhead for each
   *                                         IPC message that is stored in such a shared buffer (see
   *                                         amsr::ipc::kMessageProtocolOverhead and
   *                                         amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                         The SafeIPC implementation does not honor this value if it is below the
   *                                         lower buffer size limit (see OsAbstraction Technical Reference). The
   *                                         actual resulting size of the server-to-client buffer can be queried using
   *                                         amsr::ipc::Connection::GetSendBufferSize() on the accepted connection
   *                                         instance.
   *
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected        Tried to initialize already initialized acceptor.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in use.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  (Linux only) The file system in which the
   *                                                          server shall be created does not support a required
   *                                                          operation or a file system I/O error occurred.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Init(BufferSizeS2C s2c_buffer_size_hint) noexcept -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Accept the next connection asynchronously.
   *
   * \details   Acceptor's Reactor will be used by to-be-constructed Connection object.
   *
   * \param[in] callback  Callback to notify about the completion of the accept operation. The Acceptor object takes
   *                      ownership of the passed callback.The ownership is held until the callback has been executed or
   *                      until the Acceptor object is closed.
   *
   * \error osabstraction::OsabErrc::kUninitialized   Tried to AcceptAsync for uninitialized acceptor.
   * \error osabstraction::OsabErrc::kBusy            AcceptAsync operating already ongoing.
   * \error osabstraction::OsabErrc::kDisconnected    (Linux only) Acceptor object is not listening to
   *                                                  connect requests anymore until the Acceptor is closed and
   *                                                  reinitialized.
   *                                                  SupportData of error is not set.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcServer,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Closes the Acceptor.
   *
   * \details
   * Stops all ongoing asynchronous operations, releases the system resources and puts the Acceptor object back to
   * uninitialized state. No callback (AcceptCompletionCallback) will be called anymore but a callback might currently
   * be ongoing. IsInUse() can be used after this call to check if a provided callback (AcceptCompletionCallback) is
   * still executing. See ~Acceptor() why such a check may be necessary.
   *
   * \context         Not timer_manager::HandleTimerExpiry() if a timer manager is used.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcClose
   *
   * \vprivate        Vector product internal API
   */
  void Close() noexcept;

  /*!
   * \brief     Checks if the Acceptor is in open state or not.
   *
   * \details   An Acceptor is in open state after the Init() call until the Close() call.
   *
   * \return          true if the Acceptor is in open state, otherwise false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcClose
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckIsOpen() const noexcept -> bool;

  /*!
   * \brief     Checks if the Acceptor object still uses a provided callback (AcceptCompletionCallback) after it was
   *            closed.
   *
   * \details
   * This function should be called after the Acceptor was closed if you want to check if a provided callback is still
   * executing. Note that this function always returns true if the Acceptor is still initialized (the Acceptor was once
   * initialized but Close() was not called yet). See ~Acceptor() why such a check may be necessary.
   *
   * \return          true if the Acceptor is initialized or any provided callback is currently executing, false
   *                  otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement,
   *                  DSGN-Osab-SafeIpcClose
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInUse() const noexcept -> bool;

 private:
  /*!
   * \brief   Implementation of the Acceptor.
   * \details Ownership of this AcceptorImpl object is shared between this Acceptor object and all Reactor callbacks
   *          that the AcceptorImpl object registers. This prevents the AcceptorImpl object from being destroyed while
   *          it is still in use in the Reactor.
   */
  std::shared_ptr<internal::AcceptorImpl> acceptor_impl_;
};

}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_ACCEPTOR_H_
