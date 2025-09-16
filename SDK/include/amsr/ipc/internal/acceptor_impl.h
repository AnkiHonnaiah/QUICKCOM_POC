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
 *        \brief  Implementation of SafeIPC Acceptor class.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_ACCEPTOR_IMPL_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_ACCEPTOR_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/internal/reactor_dispatcher.h"
#include "amsr/ipc/internal/server.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Implementation of SafeIPC Acceptor class.
 *
 * \details This class contains the actual implementation of the SafeIPC Acceptor. It is expected that the Acceptor
 *          contains a shared pointer to an AcceptorImpl object and forwards API calls to this AcceptorImpl object.
 */
class AcceptorImpl final : public std::enable_shared_from_this<AcceptorImpl> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_DelegateConstructorParameterAreIncompatible
  /*!
   * \brief           Constructs an AcceptorImpl object.
   *
   * \details         The Acceptor is not initialized yet. If this constructor is used, a client that does not continue
   *                  the connection establishment protocol will permanently occupy a slot in the connection backlog of
   *                  this Acceptor. The Acceptor will not be able to accept connections anymore if all connection
   *                  backlog slots are occupied by "stuck" connections.
   *
   * \param[in]       reactor          Reactor interface for asynchronous operation.  The reactor has to exist for the
   *                                   whole lifetime of this AcceptorImpl object.
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
   */
  /*!
   * \internal
   *  - Initialize all fields and pass the AcceptorImpl internal timer manager to the Server constructor. As timeouts of
   *    the AcceptorImpl internal dummy timer manager are never handled, connection establishment timeouts are
   *    effectively deactivated.
   * \endinternal
   */
  // VECTOR Next Construct VectorC++-V12.1.1: MD_OSA_V12.1.1_BaseClassInitInCtor_NoExplicitInit
  explicit AcceptorImpl(osabstraction::io::reactor1::Reactor1Interface& reactor, UnicastAddress server_address) noexcept
      : reactor_{reactor},
        dummy_timer_manager_{&reactor},
        reactor_dispatcher_{*this, reactor},
        server_{reactor_dispatcher_, server_address, dummy_timer_manager_} {}

  /*!
   * \brief           Constructs an AcceptorImpl object.
   *
   * \details         The Acceptor is not initialized yet. If this constructor is used, a client that does not continue
   *                  the connection establishment protocol will only temporarily occupy a slot in the connection
   *                  backlog of this Acceptor. After a timeout the connection establishment for the "stuck" connection
   *                  will be aborted.
   *
   * \param[in]       reactor          Reactor interface for asynchronous operation. The reactor has to exist for the
   *                                   whole lifetime of this AcceptorImpl object.
   * \param[in]       server_address   Local address of the acceptor.
   * \param[in]       timer_manager    Timer manager, used to detect connection establishment timeouts in cases
   *                                   where a client does not continue the connection establishment protocol. The
   *                                   timer manager has to exist for the whole lifetime of the AcceptorImpl object.
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
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer
   */
  /*!
   * \internal
   *  - Initialize all fields and pass the provided timer manager to the Server constructor. As timeouts of the passed
   *    timer manager are handled by the user, connection establishment timeouts are active. The AcceptorImpl internal
   *    dummy timer manager is not used.
   * \endinternal
   */
  // VECTOR Next Construct VectorC++-V12.1.1: MD_OSA_V12.1.1_BaseClassInitInCtor_NoExplicitInit
  explicit AcceptorImpl(osabstraction::io::reactor1::Reactor1Interface& reactor, UnicastAddress server_address,
                        amsr::timer::TimerManager& timer_manager) noexcept
      : reactor_{reactor},
        dummy_timer_manager_{&reactor},
        reactor_dispatcher_{*this, reactor},
        server_{reactor_dispatcher_, server_address, timer_manager} {}

  AcceptorImpl(AcceptorImpl const& other) noexcept = delete;
  AcceptorImpl(AcceptorImpl&& other) noexcept = delete;
  AcceptorImpl& operator=(AcceptorImpl const& other) noexcept = delete;
  AcceptorImpl& operator=(AcceptorImpl&& other) noexcept = delete;

  /*!
   * \brief           Destroys the AcceptorImpl object.
   *
   * \details         This does not close the server or frees system resources, this is done by Close().
   *                  The user has to avoid destructing the AcceptorImpl object while the
   *                  AcceptCompletionCallback is executing. This can be done by using a separate software event
   *                  reactor job, or by ensuring per design that AcceptorImpl is destroyed in the same thread
   *                  where Reactor::HandleEvents() is called.
   *
   * \context         ANY
   *
   * \pre             The AcceptorImpl object is in closed state and no Reactor callback does still use this object.
   *                  Should not be executed in the context of the user callback.
   *                  Reactor of this object should not execute this internal reactor callback while the destructor is
   *                  running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement
   */
  ~AcceptorImpl() noexcept = default;

  /*!
   * \brief           Initializes AcceptorImpl object.
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
   *                                         lower buffer size limit (see OsAbstraction Technical Reference). The actual
   *                                         resulting size of the server-to-client buffer can be queried using
   *                                         amsr::ipc::Connection::GetSendBufferSize() on the accepted connection
   *                                         instance.
   *
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected        Tried to initialize already initialized acceptor.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in use.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  The file system in which the server shall be created
   *                                                          does not support a required operation or a file system
   *                                                          I/O error occurred.
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
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   */
  auto Init(BufferSizeS2C s2c_buffer_size_hint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Accept the next connection asynchronously.
   *
   * \details   AcceptorImpl's Reactor will be used by to-be-constructed Connection object.
   *
   * \param[in] callback  Callback to notify about the completion of the accept operation. The AcceptorImpl object takes
   *                      ownership of the passed callback.The ownership is held until the callback has been executed or
   *                      until the AcceptorImpl object is closed.
   *
   * \error osabstraction::OsabErrc::kUninitialized   Tried to AcceptAsync for uninitialized acceptor.
   * \error osabstraction::OsabErrc::kBusy            AcceptAsync operating already ongoing.
   * \error osabstraction::OsabErrc::kDisconnected    Acceptor object is not listening to connect requests anymore
   *                                                  until the Acceptor is closed and reinitialized.
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
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer
   */
  auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Closes the AcceptorImpl object.
   *
   * \details
   * Stops all ongoing asynchronous operations, releases the system resources and puts the AcceptorImpl object back to
   * uninitialized state. No callback (AcceptCompletionCallback) will be called anymore but a callback might currently
   * be ongoing. IsInUse() can be used after this call to check if a provided callback (AcceptCompletionCallback) is
   * still executing. See ~Acceptor() why such a check may be necessary.
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
   * \trace           DSGN-Osab-SafeIpcClose
   */
  void Close() noexcept;

  /*!
   * \brief   Closes the AcceptorImpl object if it is open and asserts that no user provided callback is currently
   *          executing.
   *
   * \details
   * Stops all ongoing asynchronous operations, releases the system resources and puts the AcceptorImpl object back to
   * uninitialized state. No callback (AcceptCompletionCallback) will be called anymore.
   *
   * \context         ANY
   *
   * \pre             No user provided callback is currently executing.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcClose,
   *                  DSGN-Osab-SafeIpcLifetimeManagement
   */
  void CloseAndAssertNoCallbackExecuting() noexcept;

  /*!
   * \brief     Checks if the AcceptorImpl object is in open state or not.
   *
   * \details   An AcceptorImpl object is in open state after the Init() call until the Close() call.
   *
   * \return          true if the AcceptorImpl object is in open state, otherwise false.
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
   */
  auto CheckIsOpen() const noexcept -> bool;

  /*!
   * \brief   Checks if the AcceptorImpl object still uses a provided callback (AcceptCompletionCallback) after it was
   *          closed.
   *
   * \details
   * This function should be called after the AcceptorImpl object was closed if you want to check if a provided callback
   * is still executing. Note that this function always returns true if the AcceptorImpl object is still initialized
   * (the AcceptorImpl object was once initialized but Close() was not called yet).
   *
   * \return          true if the AcceptorImpl object is initialized or any provided callback is currently executing,
   *                  false otherwise.
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
   */
  auto IsInUse() const noexcept -> bool;

 private:
  /*!
   * \brief         Checks if the AcceptorImpl object is in open state or not.
   *
   * \details       Same functionality as CheckIsOpen() but does not acquire the mutex.
   *
   * \return        true if the AcceptorImpl object is in open state, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckIsOpenInternal() const noexcept -> bool { return !CheckIsClosedInternal(); }

  /*!
   * \brief         Checks if the AcceptorImpl object is in closed state or not.
   *
   * \return        true if the AcceptorImpl object is in closed state, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckIsClosedInternal() const noexcept -> bool { return acceptor_state_ == AcceptorState::kClosed; }

  /*!
   * \brief         Checks if a user provided callback is currently executing.
   *
   * \return        true if a user provided callback is currently executing, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckUserCallbackExecuting() const noexcept -> bool { return user_callback_executing_; }

  /*!
   * \brief         Closes the AcceptorImpl object.
   *
   * \details       Implements the closing functionality for Close() and CloseAndAssertNoCallbackExecuting().
   *
   * \context       ANY
   *
   * \pre           The AcceptorImpl object is in open state.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void CloseInternal() noexcept;

  /*!
   * \brief         Common ReactorDispatcher callback entry point for all callbacks registered to the
   *                ReactorDispatcher.
   *
   * \details       The ReactorDispatcher callbacks are dispatched from this function. The ReactorDispatcher calls the
   *                callbacks only if they were not yet unregistered which means that after Close() it is guaranteed
   *                that no previously registered ReactorDispatcherCallback will be called anymore as Close()
   *                unregisters all callbacks.
   *                This function has to acquire the mutex as the ReactorDispatcher and the Acceptor internals
   *                (Server, etc.) are not thread-safe. This ensures that no AcceptorImpl API call runs in parallel to
   *                any ReactorDispatcher callback.
   *
   * \param[in]     id       Identifier of the ReactorDispatcher callback for which an event occurred.
   * \param[in]     events   Reactor events that occurred for the ReactorDispatcher callback.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   */
  void ReactorCallbackEntryPoint(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief         Handles acceptor software events.
   *
   * \details       This function is called when either a connection was successfully established with a client or the
   *                the user called AcceptAsync() while an established connection is already available or the server
   *                encountered an unrecoverable error. Note that multiple notifications might be coalesced into one
   *                software event if they occurred in the same HandleEvents() cycle.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           The AcceptorImpl object is in open state.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void OnAcceptorSoftwareEvent() noexcept;

  /*!
   * \brief         Registers the acceptor software event to the ReactorDisptacher.
   *
   * \return        -
   *
   * \error         osabstraction::OsabErrc::kResource      Insufficient resources to register the software event to
   *                                                        the ReactorDispatcher.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto RegisterSoftwareEventToReactor() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Unregisters the acceptor software event from the ReactorDispatcher.
   *
   * \context       ANY
   *
   * \pre           The acceptor software event is registered to the ReactorDispatcher.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void UnregisterSoftwareEventFromReactor() noexcept;

  /*!
   * \brief         Triggers the acceptor software event.
   *
   * \context       ANY
   *
   * \pre           The acceptor software event is registered to the ReactorDispatcher.
   *
   * \reentrant     FALSE
   * \synchronous   FALSE
   * \threadsafe    FALSE
   */
  void TriggerReactorSoftwareEvent() noexcept;

  /*!
   * \brief   States of AcceptorImpl's state machine.
   */
  enum class AcceptorState : std::uint32_t {
    /*!
     * \brief   AcceptorImpl is closed.
     */
    kClosed,
    /*!
     * \brief   AcceptorImpl is initialized but no asynchronous accept operation is ongoing.
     */
    kOpenAndIdle,

    /*!
     * \brief   AcceptorImpl is initialized and an asynchronous accept operation is ongoing.
     */
    kOpenAndAccepting
  };

  /*!
   * \brief   Tracks the AcceptorImpl's state.
   */
  AcceptorState acceptor_state_{AcceptorState::kClosed};

  /*!
   * \brief   Flag that indicates if a user provided callback is currently executing.
   */
  bool user_callback_executing_{false};

  /*!
   * \brief   AcceptorImpl's Reactor.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief   Timer manager that is passed to the internal Server if the user did not provide their own timer manager.
   * \details This is just a dummy object that is needed because the internal Server always needs a timer manager.
   *          Timeouts of this timer manager are never handled.
   */
  amsr::timer::TimerManager dummy_timer_manager_;

  /*!
   * \brief   Maximum number of ReactorDispatcher callback slots that the AcceptorImpl object and its internals may
   *          need.
   */
  static constexpr std::size_t kMaxUsedReactorCallbackSlots{
      Server::kMaxUsedReactorCallbackSlots + 1  // acceptor_software_event_
      + 1  // The Acceptor might be closed and reinitialized while a ReactorDispatcher callback is running.
           // This means 1 ReactorDispatcher slot might still be occupied after Close().
  };

  /*!
   * \brief   ReactorDispatcher that handles all event notifications needed by the AcceptorImpl object and its
   *          internals.
   * \details The ReactorDispatcher is used as a wrapper around the Reactor to avoid making the Reactor internals
   *          thread-safe.
   */
  ReactorDispatcher<AcceptorImpl, &AcceptorImpl::ReactorCallbackEntryPoint, kMaxUsedReactorCallbackSlots>
      reactor_dispatcher_;

  /*!
   * \brief   ReactorDispatcher software event that is triggered when either a connection was successfully established
   *          with a client or the user called AcceptAsync() while an established connection is already available or
   *          the server encountered an unrecoverable error.
   * \details Note that multiple notifications might be coalesced into one software event if they occurred in the same
   *          HandleEvents() cycle.
   */
  ReactorDispatcherId software_event_{};

  /*!
   * \brief   Internal Server that accepts and establishes incoming client connections in the background.
   */
  Server server_;

  /*!
   * \brief   Accept completion callback.
   */
  AcceptCompletionCallback accept_completion_callback_{};

  /*!
   * \brief   Mutex to secure the exclusive access to the class members.
   */
  mutable std::mutex mutex_{};
};

/*!
 * \exclusivearea ::amsr::ipc::internal::AcceptorImpl::mutex_
 * Serializes all AcceptorImpl API calls and all Reactor callback executions of Reactor callbacks registered by the
 * AcceptorImpl.
 *
 * \protects All members of an AcceptorImpl object.
 * \usedin   ::amsr::ipc::internal::AcceptorImpl::Init
 *           ::amsr::ipc::internal::AcceptorImpl::AcceptAsync
 *           ::amsr::ipc::internal::AcceptorImpl::Close
 *           ::amsr::ipc::internal::AcceptorImpl::CloseAndAssertNoCallbackExecuting
 *           ::amsr::ipc::internal::AcceptorImpl::CheckIsOpen
 *           ::amsr::ipc::internal::AcceptorImpl::IsInUse
 *           ::amsr::ipc::internal::AcceptorImpl::ReactorCallbackEntryPoint
 * \exclude  AcceptCompletionCallback
 * \length   LONG The lock is held during (de)initialization and connection establishment (creation of shared memories).
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_ACCEPTOR_IMPL_H_
