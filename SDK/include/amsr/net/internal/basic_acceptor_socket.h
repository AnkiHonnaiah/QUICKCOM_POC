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
 *        \brief  Implements a template to accept stream based connections
 *        \unit   osabstraction::net
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_ACCEPTOR_SOCKET_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_ACCEPTOR_SOCKET_H_

#include <utility>

#include "amsr/core/result.h"

#include "vac/language/unique_function.h"

#include "amsr/core/abort.h"
#include "amsr/net/internal/basic_socket.h"
#include "amsr/net/internal/basic_stream_socket.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Internal connection state.
 */
enum class AcceptorState : std::int32_t {
  /*!
   * \brief The socket is either closed or no endpoint has been bound yet.
   */
  kDisconnected = 0,

  /*!
   * \brief Socket is open and an endpoint is bound.
   */
  kEndpointBound = 1,

  /*!
   * \brief The socket is in passive mode and may accept connection requests.
   */
  kListening = 2,

  /*!
   * \brief  Asynchronous accept callback is ongoing.
   */
  kAccepting = 3,
};

/*!
 * \brief Accepts connection requests.
 *
 * \details
 * All basic socket operations like open/close and the reactor handling are implemented in the base class. This class
 * extents this by providing a listen method to wait for incoming connection requests.
 *
 * The internal operating system resource cannot be shared without creating data inconsistency. Therefore, this class
 * cannot be copied.
 *
 * For the behavior of this socket in unconventional circumstances see DSGN-Osab-BasicIP_unconventional_circumstances.
 *
 * \tparam  Protocol  The protocol that shall be used.
 */
template <class Protocol>
class BasicAcceptorSocket final : public BasicSocket<Protocol, BasicAcceptorSocket<Protocol>> {
 public:
  /*!
   * \copydoc amsr::net::internal::BasicSocket::ProtocolType
   */
  using ProtocolType = Protocol;

  /*!
   * \copydoc amsr::net::internal::BasicSocket::EndpointType
   */
  using EndpointType = typename ProtocolType::Endpoint;

  /*!
   * \brief Sockets that are created by this acceptor
   */
  using Socket = amsr::net::internal::BasicStreamSocket<Protocol>;

  /*!
   * \brief Stores the result of an accept operation.
   *
   * \details
   * The Acceptor constructs the socket and returns it together with the peer's endpoint.
   * Properties of the created socket are Os dependent (i.e. they may be inherited from the listening socket or not),
   * therefore the user must always set them explicitely.
   */
  struct AcceptedConnection {
    /*!
     * \brief The peers network endpoint.
     */
    EndpointType remote_endpoint;

    /*!
     * \brief The accepted connection.
     */
    Socket accepted_connection;
  };

  /*!
   * \brief Notifies about a completed asynchronous accept operation.
   *
   * \param[in] result The result of the accept operation. On success it contains the accepted connection.
   *
   *                  Possible errors:
   *                   osabstraction::OsabErrc::kDisconnected  The connection has been terminated by peer or the peer is
   *                                                           not reachable.
   *                   osabstraction::OsabErrc::kUnexpected    Unexpected error that is mapped to no category.
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  using AcceptCompletionCallback =
      vac::language::UniqueFunction<void(::amsr::core::Result<AcceptedConnection>&& result)>;

  /*!
   * \brief Destroys a socket
   *
   * \details         No ongoing asynchronous call is allowed during destruction.
   *                  The user has to avoid destructing the BasicAcceptorSocket object while the internal reactor
   *                  callback is executing. This can be done by using a separate software event reactor job, or
   *                  by ensuring per design that BasicAcceptorSocket is destroyed in the same thread where
   *                  Reactor::HandleEvents() is called.
   *
   * \context         ANY
   *
   * \pre             No ongoing asynchronous callback.
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
   * \trace           DSGN-Osab-BasicIP_Destruct
   *
   * \vprivate        Vector product internal API
   */
  ~BasicAcceptorSocket() noexcept final;

  /*!
   * \brief Deleted copy constructor
   */
  BasicAcceptorSocket(BasicAcceptorSocket const& other) = delete;

  /*!
   * \brief Moves the other socket into this one.
   *
   * \param[in] other   Other socket used to construct this socket. Its lock must not be locked. It must not have an
   *                    asynchronous operation ongoing on it.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  BasicAcceptorSocket(BasicAcceptorSocket&& other) noexcept;

  /*!
   * \brief Construct an acceptor socket for synchronous communication.
   *
   * \details The synchronous accept function that does not take a reactor as input produces socket objects without
   *          reactor for acceptors constructed with this constructor.
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
   * \vprivate        Vector product internal API
   */
  BasicAcceptorSocket() noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_OSA_A12.1.6_ProtectedInheritance
  /*!
   * \brief Constructs an acceptor for synchronous and asynchronous operation.
   *
   * \param[in] reactor Reactor interface for asynchronous operation. The reactor has to exist for the whole lifetime
   *                    of this BasicAcceptorSocket object.
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
   * \vprivate        Vector product internal API
   */
  explicit BasicAcceptorSocket(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      : BasicSocket<Protocol, BasicAcceptorSocket<Protocol>>(reactor) {}

  /*!
   * \brief Deleted assignment operator
   */
  BasicAcceptorSocket& operator=(BasicAcceptorSocket const& other) = delete;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
  /*!
   * \brief Closes this socket and moves other socket into this one.
   *
   * \param[in] other Other socket. It must not have an asynchronous operation ongoing on it. Its lock must not be
   *                  locked.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             No ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto operator=(BasicAcceptorSocket&& other) & noexcept -> BasicAcceptorSocket&;

  /*!
   * \brief Binds a socket to a local endpoint.
   *
   * \details
   * The endpoint will be the server address that other can connect to. A peer can connect to this endpoint and a
   * communication channel will be established on accepting the connection.
   *
   * \param[in] endpoint The local network endpoint. The endpoint must use the same network stack as this socket
   *                     (specified during Open() previously) and must be available on the local machine.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kApiError                The socket is not open or an endpoint has already been
   *                                                          bound to this socket or the socket uses a different
   *                                                          network stack than the passed endpoint.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges to bind address.
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError            The address is already in use on the local machine.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_Bind
   */
  auto Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sets socket into passive mode and starts to listen.
   *
   * \details
   * The socket starts to listen for incoming connection request. Connection request will be stored in an internal
   * backlog queue and can be accepted with the Accept() call.
   *
   * \return Result of service.
   *
   * \error osabstraction::OsabErrc::kApiError             The listen request was rejected since one
   *                                                       or more of these conditions is/are not met:
   *                                                       - The socket is open
   *                                                       - A local endpoint has been bound to this socket.
   *                                                       - Listen() has not been called yet.
   *                                                       - AcceptAsync() has not been called yet.
   * \error osabstraction::OsabErrc::kUnexpected           Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable  Cannot establish connection because local endpoint is already
   *                                                       bound by another socket or the operating system used up all
   *                                                       ephemeral ports.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto Listen() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Accept the next connection synchronously
   *
   * \details
   * Accept the next connection request in the backlog queue. Creates new sockets using the reactor passed during
   * acceptor's construction. If the acceptor did not get a reactor passed during construction, the sockets created by
   * this functions will not have a reactor either.
   *
   * \return A struct containing the accepted connection.
   *
   * \error osabstraction::OsabErrc::kApiError                  The accept request was rejected since one
   *                                                            or more of these conditions is/are not met:
   *                                                            - socket is open
   *                                                            - socket is bound to a local end point
   *                                                            - socket is in passive mode
   *                                                            - no ongoing asynchronous accept.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to accept connection.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Not allowed to accept connection.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto Accept() noexcept -> ::amsr::core::Result<AcceptedConnection>;

  /*!
   * \brief Accept the next connection synchronously
   *
   * \param[in] reactor The reactor that shall be used to construct the accepted connection socket. The reactor has
   *                    to exist for the whole lifetime of this BasicAcceptorSocket object.
   *
   * \details
   * Accept the next connection request in the backlog queue.
   *
   * \return A struct containing the accepted connection.
   *
   * \error osabstraction::OsabErrc::kApiError                  The accept request was rejected since one
   *                                                            or more of these conditions is/are not met:
   *                                                            - socket is open
   *                                                            - socket is bound to a local end point
   *                                                            - socket is in passive mode
   *                                                            - no ongoing asynchronous accept.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to accept connection.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Not allowed to accept connection.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto Accept(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      -> ::amsr::core::Result<AcceptedConnection>;

  /*!
   * \brief Accept the next connection asynchronously
   *
   * \param[in] reactor  The reactor that shall be used to construct the accepted connection socket. The reactor has
   *                     to exist for the whole lifetime of the accepted connection socket.
   * \param[in] callback Callback to notify about the completion of the accept operation. The socket will take
   *                     ownership of the passed callback. Holds the ownership till the callback has been executed or
   *                     till the socket closed.
   *
   * \return A value if the asynchronous accept request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous accept request was rejected since one
   *                                                             or more of these conditions is/are not met:
   *                                                             - socket is open
   *                                                             - socket is bound to a local end point
   *                                                             - socket is in passive mode
   *                                                             - no ongoing asynchronous accept.
   *
   * \details
   * Accept the next connection request in the backlog queue. This sockets reactor notifies the socket about a new
   * pending request.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto AcceptAsync(osabstraction::io::reactor1::Reactor1Interface& reactor,
                   AcceptCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Accept the next connection asynchronously
   *
   * \param[in] callback Callback to notify about the completion of the accept operation. The socket will take
   *                     ownership of the passed callback. Holds the ownership till the callback has been executed or
   *                     till the socket closed.
   *
   * \return A value if the asynchronous accept request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous accept request was rejected since one
   *                                                             or more of these conditions is/are not met:
   *                                                             - socket is open
   *                                                             - socket is bound to a local end point
   *                                                             - socket is in passive mode
   *                                                             - no ongoing asynchronous accept.
   *
   * \details
   * Accept the next connection request in the backlog queue. This socket's reactor notifies the socket about a new
   * pending request. Creates a new socket using the reactor passed during acceptor's construction.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
    this->AssertReactorAvailable();
    return this->AcceptAsync(this->GetReactorUnsafe(), std::move(callback));
  }

  /*!
   * \brief Handles asynchronous notifications by the reactor.
   *
   * \details Main entry point for the socket state machine.
   *
   * \note The function is public to reduce the template complexity.
   *
   * \param[in] reactor_events The reactor callback events.
   *
   * \context         Reactor context.
   *
   * \pre             Blocking mode is disabled.
   * \pre             Socket can perform asynchronous operations.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector component internal API
   */
  void OnReactorEvents(osabstraction::io::reactor1::EventTypes reactor_events) noexcept;

  /*!
   * \brief Close has been called.
   *
   * \details BasicSocket::Close() has been called by the user and the socket is closing. Reset internal state machine.
   *
   * \context         Called with a Close() call.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void OnCloseEvent() noexcept;

  /*!
   * \brief Checks whether socket is bound.
   *
   * \return          True if the socket is bound, false otherwise
   *
   * \context         ANY.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto IsSocketBound() const noexcept -> bool {
    this->AssertLockHeldByThread();

    return (this->acceptor_state_ == AcceptorState::kEndpointBound);
  }

  /*!
   * \copydoc amsr::net::internal::BasicSocket::GetSocketLocalEndpoint()
   *
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType>;

  /*!
   * \copydoc amsr::net::internal::BasicSocket::GetSocketPhysicalAddress()
   *
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress>;

 private:
  /*!
   * \brief The operating systems pending connection request queue size.
   */
  static constexpr std::int32_t const kListenBacklogQueueSize{32};

  /*!
   * \brief Handles reactor events during an ongoing asynchronous accept service.
   *
   * \details Finishes an AcceptAsync() call. AcceptAsync() completes by calling the completion callback either with a
   *          positive or negative result.
   *
   * \context         Reactor context.
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void HandleAsyncAccept() noexcept;

  /*!
   * \brief Calls accept and creates the socket.
   *
   * \details
   * The synchronous accept and the asynchronous accept both use this function to perform an actual call.
   *
   * \return A struct containing the accepted connection.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to accept connection.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Not allowed to accept connection.
   *
   * \context         ANY
   *
   * \pre             Socket is bound to a local endpoint.
   * \pre             Socket is in passive mode (implies CheckIsOpen() == true).
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CallAccept() noexcept -> ::amsr::core::Result<AcceptedConnection>;

  /*!
   * \brief Conducts a synchronous connection acceptance.
   *
   * \param[in] reactor The reactor that shall be used to construct the accepted connection socket, nullptr if the
   *                    constructed socket shall not have a reactor. The reactor has to exist for the whole lifetime
   *                    of the accepted connection socket.
   *
   * \return A struct containing the accepted connection.
   *
   * \error osabstraction::OsabErrc::kApiError                  The accept request was rejected since one
   *                                                            or more of these conditions is/are not met:
   *                                                            - socket is open
   *                                                            - socket is bound to a local end point
   *                                                            - socket is in passive mode
   *                                                            - no ongoing asynchronous accept.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to accept connection.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Not allowed to accept connection.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto DoSyncAccept(osabstraction::io::reactor1::Reactor1Interface* reactor) noexcept
      -> ::amsr::core::Result<AcceptedConnection>;

  /*!
   * \brief Moves the content from other socket into this one.
   *
   * \details Moves other socket into this one. Closes this socket if it is still open.
   *
   * \post Other socket is in state kDisconnected error and shall not be used anymore.
   *       This socket contains now the context of other socket.
   *
   * \param[in] other   The other socket which content shall be moved into this one. Its lock must not be locked. It
   *                    must not have an asynchronous operation ongoing on it.
   *
   * \context         -
   *
   * \pre             No ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void MoveAcceptorSocket(BasicAcceptorSocket&& other) noexcept;

  /*!
   * \brief Stores the connection state.
   */
  AcceptorState acceptor_state_{AcceptorState::kDisconnected};

  /*!
   * \brief Stores the callback for an ongoing asynchronous accept service.
   */
  AcceptCompletionCallback on_accept_callback_{};

  /*!
   * \brief Stores the reactor for the next accepted socket.
   *
   * \details
   * The value is only valid during the state AcceptorState::kAccepting or during execution of Accept(), holds a nullptr
   * if none of these conditions holds.
   */
  osabstraction::io::reactor1::Reactor1Interface* next_accepted_socket_reactor_{nullptr};
};

/***********************************************************************************************************************
 * Template implementation
 **********************************************************************************************************************/

// VECTOR NC AutosarC++17_10-A12.8.4: MD_OSA_A12.8.4_SocketMoveConstructor
/*!
 * \internal
 * - Let the base class move reactor related attributes from other to this socket.
 * - Move acceptor socket context.
 * - Restart asynchronous operations.
 * \endinternal
 */
template <class Protocol>
BasicAcceptorSocket<Protocol>::BasicAcceptorSocket(BasicAcceptorSocket&& other) noexcept
    : BasicSocket<Protocol, BasicAcceptorSocket<Protocol>>{} {
  this->MoveAcceptorSocket(std::move(other));
}

/*!
 * \internal
 * - Close the socket if it is not closed yet.
 * - Abort if object is still open to prevent access after delete.
 * \endinternal
 */
template <class Protocol>
BasicAcceptorSocket<Protocol>::~BasicAcceptorSocket() noexcept {
  // Ignore error. We check after the close if close succeeded. If not we have to abort anyway at this point.
  static_cast<void>(this->Close());

  if (!this->CheckIsClosed()) {
    ::amsr::core::Abort("Failed to close socket in destructor. Object has been destroyed while it was still in use.");
  }
}

// VECTOR NC AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
/*!
 * \internal
 * - If not self assignment:
 *    - Move acceptor socket context.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::operator=(BasicAcceptorSocket&& other) & noexcept -> BasicAcceptorSocket& {
  if (&other != this) {
    this->MoveAcceptorSocket(std::move(other));
  }
  return *this;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the socket is open, not yet bound and uses the network stack of the passed endpoint
 *   - bind the endpoint.
 *   - If binding succeeded
 *     - set the state to endpoint bound state.
 * - Leave critical section
 * - If the socket is not open or was already bound or uses a different network stack than the passed endpoint
 *   - output the respective error.
 * - If binding was attempted
 *   - report the result of binding.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> result{
      this->CheckSocketIsOpen().AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
        return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
      })};
  if (result.HasValue()) {
    if (this->acceptor_state_ == AcceptorState::kDisconnected) {
      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      result = os_interface::Bind(native_handle, endpoint.GetData()).Inspect([this]() {
        this->acceptor_state_ = AcceptorState::kEndpointBound;
      });
    } else if (this->acceptor_state_ == AcceptorState::kEndpointBound) {
      result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError, "Bind() has already been called"));
    } else {
      result = ::amsr::core::Result<void>{
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "Bind() must be called before Listen()")};
    }
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the socket is bound and not yet listening
 *   - make it listen for connection requests.
 *   - If making the socket listen succeeded
 *     - set the state to listening state.
 * - Leave critical section
 * - If the socket was not bound or already listening
 *   - output the respective error.
 * - If setting to listening was attempted
 *   - report the result of setting to listening.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::Listen() noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> result{MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, never output.")};

  if (this->acceptor_state_ == AcceptorState::kEndpointBound) {
    // The socket being bound means it is open.

    result =
        os_interface::Listen(this->GetNativeHandle(), ListenBacklog{this->kListenBacklogQueueSize}).Inspect([this]() {
          this->acceptor_state_ = AcceptorState::kListening;
        });
  } else if (this->acceptor_state_ == AcceptorState::kDisconnected) {
    result.EmplaceError(
        MakeErrorCode(osabstraction::OsabErrc::kApiError, "Bind() needs to be called before Listen()."));
  } else if (this->acceptor_state_ == AcceptorState::kListening) {
    result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError, "Listen() has already been called."));
  } else {
    result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                      "Listen() was called after asynchronous accept request has been issued."));
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - If this acceptor has a reactor
 *   - select that the accepted socket shall use the reactor this socket has.
 * - If this acceptor does not have a reactor
 *   - select that the accepted socket shall not use a reactor.
 * - Request an accept call.
 * - Leave critical section.
 * - Output the result of this request.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::Accept() noexcept -> ::amsr::core::Result<AcceptedConnection> {
  osabstraction::io::reactor1::Reactor1Interface* reactor{nullptr};

  this->AcquireLock();

  if (this->HasReactor()) {
    reactor = &this->GetReactorUnsafe();
  }

  ::amsr::core::Result<AcceptedConnection> result{this->DoSyncAccept(reactor)};

  this->ReleaseLock();

  return std::move(result);
}

/*!
 * \internal
 * - Enter critical section.
 * - Select that the accepted socket shall use the input reactor.
 * - Request an accept call.
 * - Leave critical section.
 * - Output the result of this request.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::Accept(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
    -> ::amsr::core::Result<AcceptedConnection> {
  this->AcquireLock();

  ::amsr::core::Result<AcceptedConnection> result{this->DoSyncAccept(&reactor)};

  this->ReleaseLock();

  return std::move(result);
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check for correct socket state.
 * - If the state is the correct state
 *   - set state to accepting.
 *   - Store reactor interface and completion callback.
 *   - Wait in reactor for incoming connection request.
 * - Leave critical section.
 * - If the state was incorrect
 *   - output the respective error.
 * - If the asynchornous accept was set up
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::AcceptAsync(osabstraction::io::reactor1::Reactor1Interface& reactor,
                                                AcceptCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  this->AcquireLock();

  this->AssertReactorAvailable();

  ::amsr::core::Result<void> result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError,
                    "AcceptAsync() called before calling Bind() and Listen() or after socket has been closed.")};

  // VECTOR Next Construct AutosarC++17_10-M6.4.3: MD_OSA_M6.4.3_switch_state_machine_enum_no_default_required
  // VECTOR Next Construct AutosarC++17_10-M6.4.6: MD_OSA_M6.4.6_switch_state_machine_enum_no_default_required
  switch (this->acceptor_state_) {
    case AcceptorState::kDisconnected: {
      // This can be entered in two cases:
      // - Bind() has not been called yet.
      // - The socket has been closed.

      // result already contains the correct error.
      break;
    }

    case AcceptorState::kEndpointBound: {
      result.EmplaceError(
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "AcceptAsync() called before calling Listen()."));
      break;
    }

    case AcceptorState::kListening: {
      // The socket being listening means it is open and bound.

      this->acceptor_state_ = AcceptorState::kAccepting;

      this->next_accepted_socket_reactor_ = &reactor;

      // Only store the callback after the state has been set to accepting to avoid the callback being moved in a
      // concurrent move operation (which is anyway prohibited because the move operation of this class is not
      // thread-safe) after it was stored but before the state was set.
      this->on_accept_callback_ = std::move(callback);

      this->ChangeReadObservation(true);
      result.EmplaceValue();
      break;
    }

    case AcceptorState::kAccepting: {
      result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                        "There is already an ongoing asynchronous accept operation."));
      break;
    }
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Create endpoint for correct protocol with default arguments. The endpoint will be filled with valid data during
 *   the accept call.
 * - Release lock.
 * - Call accept system call and pass the endpoint data view.
 * - Re-acquire lock.
 * - On Success:
 *    - Create socket with or without reactor as selected by operation request.
 *    - Assign the native handle to the created socket.
 *    - Set the socket into connected state.
 * - Return the socket and the remote endpoint or the error.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::CallAccept() noexcept -> ::amsr::core::Result<AcceptedConnection> {
  Socket accepted_connection_socket{
      (this->next_accepted_socket_reactor_ == nullptr) ? Socket{} : Socket{*this->next_accepted_socket_reactor_}};

  // Assert that the endpoint uses the correct protocol family (either IPv4 or IPv6)
  // Accept can only be called on an open socket. Thus the pre-condition of GetProtocolUnsafe() holds.
  ::amsr::core::Result<AcceptedConnection> result{
      AcceptedConnection{EndpointType{this->GetProtocolUnsafe()}, std::move(accepted_connection_socket)}};

  osabstraction::io::NativeHandle const acceptor_native_handle{this->GetNativeHandle()};

  this->ReleaseLock();
  ::amsr::core::Result<osabstraction::io::NativeHandle> handle{
      os_interface::Accept(acceptor_native_handle, result.Value().remote_endpoint.GetData())};
  this->AcquireLock();

  if (!handle.HasValue()) {
    result.EmplaceError(handle.Error());
  } else {
    Socket& new_connection{result.Value().accepted_connection};
    // Either assigns and opens the socket or it closes the native handle and release the operating system resources.
    Protocol const protocol{this->GetProtocolUnsafe()};
    osabstraction::io::NativeHandle const accepted_native_handle{handle.Value()};
    ::amsr::core::Result<void> const open_result{
        new_connection.AssignHandleAndConnectSocket(protocol, accepted_native_handle, this->GetNetworkStackId())};
    if (!open_result.HasValue()) {
      result.EmplaceError(open_result.Error());
    }
  }
  return result;
}

/*!
 * \internal
 * - Verify the precondition..
 * - Sanity check for events.
 * - Call HandleAsyncAccept on read event.
 * \endinternal
 */
template <class Protocol>
void BasicAcceptorSocket<Protocol>::OnReactorEvents(osabstraction::io::reactor1::EventTypes reactor_events) noexcept {
  this->AssertLockHeldByThread();

  if (reactor_events.HasWriteEvent()) {
    // Write events are not used and never enabled for an acceptor.
    ::amsr::core::Abort("Corrupted native handle supervision. The internal state is undefined.");
  }
  this->HandleAsyncAccept();
}

/*!
 * \internal
 * - Verify the precondition.
 * - Set the state to disconnected.
 * \endinternal
 */
template <class Protocol>
void BasicAcceptorSocket<Protocol>::OnCloseEvent() noexcept {
  this->AssertLockHeldByThread();

  acceptor_state_ = AcceptorState::kDisconnected;
}

/*!
 * \internal
 *  - Call accept.
 *  - If not spurious wake-up:
 *    - Set acceptor state back to listening.
 *    - If success
 *      - Release lock.
 *      - Call acceptor callback.
 *      - Re-acquire lock.
 *      - If the acceptor is still open and no new accept has been requested in the callback
 *        - disable read monitoring.
 * \endinternal
 */
template <class Protocol>
void BasicAcceptorSocket<Protocol>::HandleAsyncAccept() noexcept {
  ::amsr::core::Result<AcceptedConnection> result{this->CallAccept()};
  this->next_accepted_socket_reactor_ = nullptr;

  bool const spurious_wake{result == osabstraction::OsabErrc::kBusy};

  if (!spurious_wake) {
    if (this->acceptor_state_ == AcceptorState::kAccepting) {
      // Hold the ownership on the stack till the callback returns. A new callback might be assigned during the ongoing
      // callback.
      // Take callback to local variable before resetting the state to avoid a race condition with a concurrent move
      // (which is prohibited because the move operation of this class is not thread-safe) that could take place after
      // the state was reset but before the callback was taken to a local variable.
      AcceptCompletionCallback cb{std::move(this->on_accept_callback_)};  // NOLINT(vector-object-not-changed)
      this->acceptor_state_ = AcceptorState::kListening;

      this->ReleaseLock();
      cb(std::move(result));
      this->AcquireLock();

      // Explicitly check against listening. Avoid a call to a closing handle although the reactor is protected
      // against it.
      if (this->acceptor_state_ == AcceptorState::kListening) {
        // No new request. Disable monitoring.
        this->ChangeReadObservation(false);
      }
    } else {
      // Acceptor has been closed in the background. Do not perform any further actions.
    }
  }
}

/*!
 * \internal
 * - If the state allows accepting
 *   - conduct the accept operation.
 *   - Output the result of the operation.
 * - If the state does not allow accepting
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::DoSyncAccept(osabstraction::io::reactor1::Reactor1Interface* reactor) noexcept
    -> ::amsr::core::Result<AcceptedConnection> {
  ::amsr::core::Result<AcceptedConnection> result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError,
                    "Accept() called before calling Bind() and Listen() or after socket has been closed.")};

  // VECTOR Next Construct AutosarC++17_10-M6.4.3: MD_OSA_M6.4.3_switch_state_machine_enum_no_default_required
  // VECTOR Next Construct AutosarC++17_10-M6.4.6: MD_OSA_M6.4.6_switch_state_machine_enum_no_default_required
  switch (this->acceptor_state_) {
    case AcceptorState::kDisconnected: {
      // This can be entered in two cases:
      // - Bind() has not been called yet.
      // - The socket has been closed.

      // result already contains the correct error.
    } break;

    case AcceptorState::kEndpointBound: {
      result.EmplaceError(
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "Accept() called before calling Listen()."));
    } break;

    case AcceptorState::kListening: {
      // The socket being listening means it is open and bound.

      // Assignment must only take place after it is assured that no asynchronous operation is ongoing.
      this->next_accepted_socket_reactor_ = reactor;
      result = this->CallAccept();
      this->next_accepted_socket_reactor_ = nullptr;
    } break;

    case AcceptorState::kAccepting: {
      result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                        "There is a pending asynchronous Accept request to be processed."));
    } break;
  }

  return result;
}

/*!
 * \internal
 * - Get other sockets state and set other sockets state to disconnected.
 * - Assert that no asynchronous operation has been scheduled.
 * - Let base class move reactor related attributes from other to this socket.
 * \endinternal
 */
template <class Protocol>
void BasicAcceptorSocket<Protocol>::MoveAcceptorSocket(BasicAcceptorSocket&& other) noexcept {
  AcceptorState const others_state{other.acceptor_state_};
  other.acceptor_state_ = AcceptorState::kDisconnected;
  // Abort on ongoing asynchronous operation. The socket is not thread safe. The behavior is undefined.
  if (others_state == AcceptorState::kAccepting) {
    ::amsr::core::Abort("Tried to move socket with scheduled asynchronous operation.");
  }

  this->acceptor_state_ = others_state;

  // This socket will be closed during the base class move.
  this->MoveSocket(std::move(other));

  // No need to move additional attributes. They are only valid during an ongoing asynchronous operation.
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::GetLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  return BasicSocket<Protocol, BasicAcceptorSocket<Protocol>>::GetSocketLocalEndpoint();
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicAcceptorSocket<Protocol>::GetPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress> {
  return BasicSocket<Protocol, BasicAcceptorSocket<Protocol>>::GetSocketPhysicalAddress();
}

/*!
 * \exclusivearea ::amsr::net::internal::BasicSocket::lock_::mutex_
 * Ensures atomicity when jointly accessing object state and reactor event settings for accept operations.
 * \protects ::amsr::net::internal::BasicSocket::reactor_,
 *           ::amsr::net::internal::BasicSocket::socket_state_,
 *           ::amsr::net::internal::BasicSocket::cached_blocking_mode_,
 *           ::amsr::net::internal::BasicAcceptorSocket::acceptor_state_,
 *           ::amsr::net::internal::BasicAcceptorSocket::next_accepted_socket_reactor_
 * \usedin   ::amsr::net::internal::BasicAcceptorSocket::Bind,
 *           ::amsr::net::internal::BasicAcceptorSocket::Listen,
 *           ::amsr::net::internal::BasicAcceptorSocket::Accept,
 *           ::amsr::net::internal::BasicAcceptorSocket::AcceptAsync,
 *           ::amsr::net::internal::BasicAcceptorSocket::HandleAsyncAccept,
 *           ::amsr::net::internal::BasicAcceptorSocket::OnReactorEvents,
 *           ::amsr::net::internal::BasicAcceptorSocket::OnCloseEvent,
 *           ::amsr::net::internal::BasicAcceptorSocket::IsSocketBound,
 *           ::amsr::net::internal::BasicAcceptorSocket::CallAccept,
 *           ::amsr::net::internal::BasicAcceptorSocket::DoSyncAccept
 * \exclude  All other of these methods of the same object.
 * \length   LONG Enabling and disabling events on the reactor.
 *
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_ACCEPTOR_SOCKET_H_
