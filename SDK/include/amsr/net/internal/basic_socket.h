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
 *        \brief  Provides common socket services.
 *        \unit   osabstraction::net
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_H_

#include <type_traits>
#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "amsr/net/ethernet_address.h"
#include "amsr/net/interface.h"
#include "amsr/net/internal/basic_socket_errors.h"
#include "amsr/net/internal/os_interface.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/socket_base.h"
#include "amsr/net/internal/socket_option.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "amsr/core/error_code.h"
#include "amsr/net_utils/internal/thread_aware_lock.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Helper class to generate dummy types for function overloading.
 *
 * \tparam Type to generate a dummy type for.
 */
template <typename T>
struct OverloadType {};

/*!
 * \brief Internal Basic socket state.
 *
 * \details Bauhaus has issues with defining this as nested enum within BasicSocket template.
 */
enum class BasicSocketState : std::uint32_t {
  /*!
   * \brief The socket is closed.
   *
   * \details
   * Not system resources are allocated and no operation is ongoing.
   */
  kClosed = 0,

  /*!
   * \brief Socket is ready to be used.
   *
   * \details
   * Socket is ready to be used.
   */
  kOpen = 1,

  /*!
   * \brief The socket is currently closing
   *
   * \details
   * Closing has been requested. No new asynchronous callback will be started. The socket will be closed as soon as
   * all ongoing asynchronous callbacks complete.
   */
  kClosing = 2,
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_BasicSocket
/*!
 * \brief Template for all sockets. It implements basic functionality required by every socket.
 *
 * \tparam Protocol The protocol type refers to a transportation protocol (e.g. TCP, UDP or PTP). A protocol class
 *                   provides the at least the methods:
 *                   - auto GetFamily() -> internal::SocketFamily
 *                   - auto GetType() -> internal::SocketType
 *                   - auto GetProtocol() -> internal::SocketProtocol
 *                  Theses types can be mapped to the POSIX socket() arguments. Additionally a protocol provides the
 *                  type Endpoint that can be used with the protocol.
 *
 * \tparam DerivedSocket The socket that implements the logic. Use the CRTP instead of private virtual functions to
 *         customize the implementation in the derived class.
 *
 * \details
 *  Provide the functionality that is common to all socket that exchange data regardless of the used protocol.
 *  In general it provides the functionality for:
 *  - Opening a socket
 *  - Binding a socket to an address
 *  - Changing settings of a socket
 *  - Terminating communication over a socket
 *
 *  Protocol sockets may inherit from a BasicSocket template instantiation and extend the class with protocol specific
 *  services. A socket has exclusive ownership over his operating system resources. The ownership can never be shared
 *  with other objects.
 */
template <class Protocol, class DerivedSocket>
class BasicSocket : public SocketBase {
 public:
  /*!
   * \brief Alias to the template parameter Protocol. Check class description for details.
   */
  using ProtocolType = Protocol;

  /*!
   * \brief Alias to an Endpoint that can be used with the specified protocol.
   *
   * \details
   * The Endpoint Type shall satisfy the following requirements:
   * - auto GetData() const noexcept -> net::internal::SocketConstAddrView
   * - auto GetData() noexcept -> net::internal::SocketMutableAddrView
   *
   */
  using EndpointType = typename ProtocolType::Endpoint;

  /*!
   * Reactor for asynchronous operations.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief           Destroys the socket
   *
   * \details         User has to avoid to destruct the BasicSocket object while the internal reactor callback is
   *                  executing. This can be done by using a separate software event reactor job, or by ensuring per
   *                  design that BasicSocket is destroyed in the same thread where Reactor::HandleEvents() is
   *                  called.
   * \context         -
   *
   * \pre             No asynchronous operation in progress.
   *                  Should not be executed in the context of the user callback.
   *                  Reactor of this object should not execute this internal reactor callback while the destructor is
   *                  running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-BasicIP_Destruct
   *
   * \vprivate        Vector product internal API
   */
  ~BasicSocket() noexcept override = default;

  /*!
   * \brief Deleted copy constructor
   */
  BasicSocket(BasicSocket const&) = delete;

  /*!
   * \brief Regular move constructor is not supported. Use derived class operator.
   */
  BasicSocket(BasicSocket&& other) = delete;

  /*!
   * \brief Deleted assignment operator
   */
  BasicSocket& operator=(BasicSocket const& other) = delete;

  /*!
   * \brief Deleted move operator
   */
  BasicSocket& operator=(BasicSocket&& other) = delete;

  /*!
   * \brief Returns the native handle.
   *
   * \return Native handle
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto GetNativeHandle() const noexcept -> osabstraction::io::NativeHandle { return native_handle_; }

  /*!
   * \brief Checks if the socket is in state open or not
   *
   * \details State open means that Open() has been called on the socket but Close() has not yet been called on it.
   *
   * \return True if the socket is in state open. Otherwise false
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-BasicIP_CheckIsOpen
   */
  auto CheckIsOpen() noexcept -> bool;

  /*!
   * \brief Checks if the socket is in state closed or not.
   *
   * \details
   * Open() has not been called for it since it was created.
   * It does not have a reactor, Close() has been called for it and Open() has not been called for it since the
   * call of Close().
   * Close() has been called for it, it's event handler is not executing and Open() has not been called for it
   * since the call of Close().
   *
   * \return True if the socket is in state closed. Otherwise false
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-BasicIP_CheckIsClosed
   */
  auto CheckIsClosed() noexcept -> bool;

  /*!
   * \brief Opens a socket
   *
   * \param[in] protocol     Specifies the communication protocol to be used.
   * \param[in] netstack_id  Specifies the network stack to be used. Defaults to default network stack.
   *
   * \details
   * Uses the syscall socket() to create a socket file descriptor. Registers the file descriptor to the reactor if a
   * reactor is available. The created socket will be in blocking mode.
   *
   * The protocol parameter provides the arguments for the socket() system call.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to create a
   *                                                            socket.
   * \error osabstraction::OsabErrc::kResource                  Not enough resources to open the socket. Either the
   *                                                            reactor has insufficient space left to register a new
   *                                                            handle or the handle cannot be created due to
   *                                                            insufficient system resources.
   * \error osabstraction::OsabErrc::kApiError                  The socket is already open.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_Open
   */
  auto Open(ProtocolType const& protocol,
            net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Requests to close the connection.
   *
   * \details
   * Stops all operations and releases the system resources. If any asynchronous operation callback is currently
   * executed then the release of the system resource is delayed to the end of the asynchronous operation. It is safe
   * to delete the socket as soon as it enters the "closed" state (CheckIsClosed() == true). The socket always leaves
   * the "Open" state. Most services are not permitted anymore after a call to Close().
   * It is possible to skip the CheckIsClosed() call if it can be assured that there is no ongoing asynchronous
   * operation. One way would be to call this function in the Reactor thread after a HandleEvents() call.
   *
   * \context         ANY
   *
   * \return Result of the operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  Socket is already closed.
   *
   * \pre             -
   *
   * \post            CheckIsOpen() == false
   *
   * \reentrant       FALSE
   * \synchronous     FALSE if operation has been delayed by a concurrent callback in the reactor context.
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_Close
   */
  auto Close() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sets the blocking mode.
   *
   * \param[in] enable True to enabled blocking. False to disable blocking
   *
   * \return A value if the blocking mode was set successfully, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto SetBlockingMode(bool enable) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Returns the cached blocking mode.
   *
   * \details
   * Returns the cached blocking mode.
   *
   * \return True if socket services block otherwise false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto IsBlockingModeEnabled() noexcept -> bool {
    this->AcquireLock();
    bool const is_enabled{cached_blocking_mode_};
    this->ReleaseLock();
    return is_enabled;
  }

  /*!
   * \brief Sets a socket option.
   *
   * \tparam SocketOption Socket option type that fulfills the socket option requirements.
   *
   * \details
   * Updates the socket attributes.
   *
   * \param[in] option Socket option to be set.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the passed option.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Insufficient privileges to set socket option.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_SetOption
   *
   */
  template <class SocketOption>
  auto SetOption(SocketOption option) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Gets a socket option
   *
   * \tparam SocketOption Socket option type that fulfills the socket option requirements.
   *
   * \details
   * Retrieves the socket attributes for the specified socket option.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the requested option.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  template <class SocketOption>
  auto GetOption() noexcept -> ::amsr::core::Result<SocketOption>;

  /*!
   * \brief           Gets the ID of the network stack used by this socket.
   *
   * \return          The network stack ID. Only valid until socket is closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto GetNetworkStackId() const noexcept -> net_utils::NetworkStackId const& { return network_stack_id_; }

 protected:
  /*!
   * \brief Socket option to set the network interface used for outgoing multicast packages.
   */
  using SocketOptionMulticastNetworkInterface =
      SocketOptionIPNetworkInterface<IPPROTO_IP, IP_MULTICAST_IF, IPPROTO_IPV6, kIPv6MulticastIF>;

  /*!
   * \brief Constructs a socket that is restricted to synchronous operations.
   *
   * \details
   * No reactor has been provided. Thus asynchronous operations are not possible.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  BasicSocket() noexcept = default;

  /*!
   * \brief Closes this sockets and moves the other socket into this one.
   *
   * \param[in] other Other socket. Must not be this socket. Its lock must not be locked. It must not have an
   *                  asynchronous operation ongoing on it.
   *
   * \context         Child class' move constructor or operator
   *
   * \pre             No ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  void MoveSocket(BasicSocket&& other) noexcept;

  /*!
   * \brief Constructs a socket that support synchronous and asynchronous operations.
   *
   * \param[in] reactor Interface to reactor used for asynchronous operations. The reactor has to exist
   *                    for the whole lifetime of this BasicSocket object.
   *
   * \details
   * Reactor is provided, therefore asynchronous operations are possible.
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
  explicit BasicSocket(ReactorInterface& reactor) noexcept : SocketBase{}, reactor_{&reactor} {}

  /*!
   * \brief Assigns a native handle.
   *
   * \details
   * Assigns the native handle to the socket and sets it to open state.
   *
   * \post The native handle has been assigned or the native handle has been closed.
   *
   * \param[in] handle       A native handle.
   * \param[in] protocol     The protocol that was used to created the handle.
   * \param[in] netstack_id  ID of the network stack the native handle is assigned to.
   *
   * \return Result of operation.
   *
   * \error           osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                          reached.
   * \context         ANY
   *
   * \pre             GetNativeHandle() == kInvalidNativeHandle
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto AssignNativeHandle(ProtocolType const& protocol, osabstraction::io::NativeHandle handle,
                          net_utils::NetworkStackId const& netstack_id) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Changes the native handle observation state for read operations.
   *
   * \param[in]  enable True to enable observation and false to disable observation.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             CheckIsOpen() == true
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void ChangeReadObservation(bool enable) noexcept;

  /*!
   * \brief Changes the native handle observation state for write operations.
   *
   * \param[in]  enable True to enable observation and false to disable observation.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             CheckIsOpen() == true
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void ChangeWriteObservation(bool enable) noexcept;

  /*!
   * \brief Changes the native handle observation state.
   *
   * \param[in]  single_event Reactor event whose observation shall be changed
   * \param[in]  enable       True to enable observation and false to disable observation.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             CheckIsOpen() == true
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void ChangeReactorObservation(osabstraction::io::reactor1::EventTypes single_event, bool enable) noexcept;

  /*!
   * \brief Returns the protocol that is bound to the socket.
   *
   * \details
   * A protocol will be assigned with opening the socket or assigning a valid native handle. It stays valid until the
   * socket closes.
   *
   * \return The sockets protocol.
   *
   * \context         ANY
   *
   * \pre             CheckIsOpen() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto GetProtocolUnsafe() const noexcept -> ProtocolType const& { return protocol_.value(); }

  /*!
   * \brief Checks whether a reactor interface is available or not.
   *
   * \return True if a reactor can be used for asynchronous operations, otherwise false.
   *
   * \context         ANY
   *
   * \pre             CheckIsOpen() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  auto HasReactor() const noexcept -> bool { return reactor_ != nullptr; }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_ptr
  /*!
   * \brief Returns a reference to a reactor.
   *
   * \return Reference to the reactor interface for asynchronous objects.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  auto GetReactorUnsafe() noexcept -> ReactorInterface& { return *reactor_; }

  /*!
   * \brief           Check whether the socket is open.
   *
   * \return          Returns a value if the socket is open, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError    The socket is not open.
   *
   * \context         ANY
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
  auto CheckSocketIsOpen() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is closed.
   *
   * \return          Returns a value if the socket is open, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError    The socket is not closed.
   *
   * \context         ANY
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
  auto CheckSocketIsClosed() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Asserts that blocking mode is disabled on the socket.
   *
   * \context         ANY
   *
   * \pre             Blocking Mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void AssertBlockingModeIsDisabled() const noexcept;

  /*!
   * \brief           Asserts that the passed IO Vector array size is valid.
   *
   * \param[in]       size    The size to check. Must be smaller than or equal to kMaxIOBufferArraySize.
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
   * \vprivate        Vector component internal API
   */
  static void AssertProperIOBufferArraySize(
      ::amsr::core::Span<osabstraction::io::ConstIOBuffer>::size_type size) noexcept;

  /*!
   * \brief           Asserts that the passed IO buffer size is valid.
   *
   * \param[in]       size    The size to check. Must be smaller than or equal to kMaxIOBufferSize.
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
   * \vprivate        Vector component internal API
   */
  static void AssertProperIOBufferSize(::amsr::core::Span<std::uint8_t>::size_type size) noexcept;

  /*!
   * \brief           Asserts that the socket has a reactor.
   *
   * \context         ANY
   *
   * \pre             HasReactor() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void AssertReactorAvailable() const noexcept;

  /*!
   * \brief           Checks whether the protocol type of the provided endpoint matches the protocol type of this
   *                  socket.
   *
   * \param[in]       endpoint  The endpoint to check the protocol type against the protocol type of this socket.
   *
   * \return          Value, if the protocol type of the provided endpoint matches the protocol type of this socket,
   *                  error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Protocol types do not match.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckEndpointProtocolMatches(EndpointType const& endpoint) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Checks whether the socket uses a specific network stack.
   *
   * \param[in]       network_stack_id  The ID of the network stack that this socket is expected to use.
   *
   * \return          Value, if the socked uses the passed network stack, error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket uses different network stack.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketUsesNetworkStack(net_utils::NetworkStackId const& network_stack_id) const noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Returns the bound network endpoint by using the provided OS Interface function.
   *
   * \param[in]       get_endpoint_function   OS Interface function to get the endpoint.
   *
   * \return The endpoint.
   *
   * \error osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
   * \error osabstraction::OsabErrc::kApiError           The socket is not open and/or not bound/connected.
   * \error osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
   *
   * \context         ANY
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
  auto GetEndpoint(os_interface::OsInterfaceGetEndpointFunction get_endpoint_function) noexcept
      -> ::amsr::core::Result<EndpointType>;

  /*!
   * \brief           Extracts the endpoint the socket is bound to.
   *
   * \return          The local endpoint.
   *
   * \error           OsabErrc::kApiError           The socket is not open.
   *
   * \error           OsabErrc::kResource           Not enough system resources to acquire
   *                                                the address of the local endpoint.
   *
   * \error           OsabErrc::kUnexpected         Some unexpected internal state was detected.
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
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSocketLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType>;

  /*!
   * \brief           Returns the physical address.
   *
   * \return          The physical MAC address of the network interface the socket is bound to.
   *
   * \error           OsabErrc::kApiError                The socket is not open and/or not bound.
   *
   * \error           OsabErrc::kResource                Not enough system resources to acquire the address of the
   *                                                     local bound endpoint.
   *
   * \error           OsabErrc::kUnexpected              Some unexpected internal state was detected.
   *
   * \error           OsabErrc::kSystemEnvironmentError  The Interface the socket is bound to cannot be identified
   *                                                     uniquely.
   *
   * \error           OsabErrc::kAddressError            The network interface does not have a physical address or the
   *                                                     IP address the socket is bound to is assigned to multiple
   *                                                     network interfaces.
   *
   * \error           OsabErrc::kInsufficientPrivileges  Insufficient privileges to get physical address.
   *
   * \context         ANY
   *
   * \pre             The socket must be open and bound.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSocketPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress>;

  /*!
   * \brief Sets a socket option.
   *
   * \tparam SocketOption Socket option type that fulfills the socket option requirements.
   *
   * \details
   * Updates the socket attributes.
   *
   * \param[in] option Socket option to be set.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the passed option.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Insufficient privileges to set socket option.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   *
   * \trace DSGN-Osab-BasicIP_SetOption
   *
   */
  template <class SocketOption>
  // clang11 fails to compile without const in declaration
  // NOLINTNEXTLINE(readability-avoid-const-params-in-decls)
  auto SetOptionInternal(SocketOption const option) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Gets a socket option
   *
   * \tparam SocketOption Socket option type that fulfills the socket option requirements, not
   *         SocketOptionMulticastNetworkInterface.
   *
   * \details
   * Retrieves the socket attributes for the specified socket option.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the requested option.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
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
  template <class SocketOption>
  auto GetOptionInternal(OverloadType<SocketOption>) const noexcept -> ::amsr::core::Result<SocketOption>;

  /*!
   * \brief Gets the SocketOptionMulticastNetworkInterface socket option.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the requested option.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
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
  auto GetOptionInternal(OverloadType<SocketOptionMulticastNetworkInterface>) const noexcept
      -> ::amsr::core::Result<SocketOptionMulticastNetworkInterface>;

  /*!
   * \brief Gets the SocketOptionMulticastNetworkInterface socket option.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the requested option.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
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
  auto GetOptionInternal(OverloadType<SockOptBindToDevice>) const noexcept -> ::amsr::core::Result<SockOptBindToDevice>;

  /*!
   * \brief Acquires the lock for this object.
   *
   * \context         ANY
   *
   * \pre             The lock is not held by the calling thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void AcquireLock() noexcept { lock_.Lock(); }

  /*!
   * \brief Releases the lock for this object.
   *
   * \context         ANY
   *
   * \pre             The lock is held by the calling thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void ReleaseLock() noexcept { lock_.Unlock(); }

  /*!
   * \brief Assert that the calling thread holds the lock.
   *
   * \context         ANY
   *
   * \pre             The lock is held by the calling thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void AssertLockHeldByThread() const noexcept { lock_.AssertThreadHasLocked(); }

 private:
  /*!
   * \brief Registers the native handle to the reactor.
   *
   * \return Result of operation.
   *
   * \error           osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                          reached.
   *
   * \context         ANY
   *
   * \pre             Has valid native handle
   * \pre             this->HasReactor() == true
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto RegisterToReactor() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Checks whether a closing socket can be set to closed.
   *
   * \details The direct transition to closing is not possible if the reactor informs us about an ongoing callback in
   *          another thread. The transition is delay till the next CheckIsClosed() call. Is open calls this function to
   *          update the socket state.
   *
   * \context         ANY
   *
   * \pre             Socket is in closing state.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UpdateSocketClosingState() noexcept;

  /*!
   * \brief Sets all internal members to a defined close state.
   *
   * \details This function resets all internal variable.
   *
   * \context         UpdateSocketClosingState() call.
   *
   * \pre             The socket fulfills the requirements for the close state.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void SetSocketStateClosed() noexcept;

  /*!
   * \brief Implements a reactor callback.
   *
   * \param[in] events Pending events.
   *
   * \details Implement the reactor callback. Forward the events directly to the derived class.
   *
   * \context         ANY
   *
   * \pre             Blocking mode is disabled.
   * \pre             Lock is not held on call.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void OnReactorCallback(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Implements the move operation to move other socket into this socket.
   *
   * \param[in]  other Other socket that shall be moved into this one. The lock is not locked.
   *
   * \context         In move operator or move constructor.
   *
   * \pre             This object is in state closed. No operating system resources are allocated. The lock is not
   *                  locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   */
  void MoveOtherSocket(BasicSocket& other) noexcept;

  /*!
   * \brief Checks if the object is in the expected state.
   *
   * \details Updates the state if required.
   *
   * \param[in]  expected_state The expected state.
   *
   * \return True if the object is in the expected state, false if not.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto CheckSocketState(BasicSocketState expected_state) noexcept -> bool;

  /*!
   * \brief Native handle.
   *
   * \details Does not need to be protected by critical sections because it is only set once (during Open/
   *          AssignNativeHandle) and asynchronous operations (against which the exclusive area protects) are only
   *          possible after this has been done.
   */
  osabstraction::io::NativeHandle native_handle_{osabstraction::io::kInvalidNativeHandle};

  /*!
   * \brief Reactor for asynchronous operations.
   *
   * The reactor can only be provided during object construction. If no reactor has been provided then the object
   * can only be used synchronously.
   */
  ReactorInterface* reactor_{nullptr};

  /*!
   * \brief Stores the reactor's handle to the registered file descriptor.
   *
   * The handle the handle is only valid after registering to the reactor.
   */
  osabstraction::io::reactor1::CallbackHandle reactor_handle_{};

  /*!
   * \brief The state of the internal socket
   */
  BasicSocketState socket_state_{BasicSocketState::kClosed};

  /*!
   * \brief Socket's blocking mode.
   *
   * \details
   * Due to performance reasons the blocking mode is cached.
   * True:  The socket blocks on system calls.
   * False: The socket does not block in an operating system service.
   */
  bool cached_blocking_mode_{true};

  /*!
   * \brief Stores the protocol that the socket uses.
   *
   * \details
   * The value is only valid during open() state. The protocol stores the values that are usually passed to the
   * socket() call. This includes the protocol family.
   * Does not need to be protected by critical sections because it is only set once (during Open/AssignNativeHandle) and
   * asynchronous operations (against which the exclusive area protects) are only possible after this has been done.
   */
  ::amsr::core::Optional<ProtocolType> protocol_{};

  /*!
   * \brief   Lock that protects the socket object attributes.
   *
   * \details For further information, see the description of the exclusive areas for
   *          ::amsr::net::internal::BasicSocket, ::amsr::net::internal::BasicDatagramSocket,
   *          ::amsr::net::internal::BasicStreamSocket and
   *          ::amsr::net::internal::BasicAcceptorSocket.
   */
  net_utils::internal::ThreadAwareLock lock_{};

  /*!
   * \brief ID of the network stack used by this socket.
   */
  net_utils::NetworkStackId network_stack_id_{};
};

/***********************************************************************************************************************
 * Template implementation
 **********************************************************************************************************************/

/*!
 * \internal
 * - Enter critical section.
 * - Check if the socket is in open state.
 * - Leave critical section.
 * - Output if the socket is in open state.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckIsOpen() noexcept -> bool {
  this->AcquireLock();
  bool const is_open{this->CheckSocketIsOpen().HasValue()};
  this->ReleaseLock();

  return is_open;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the socket is in closed state.
 * - Leave critical section.
 * - Output if the socket is in closed state.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckIsClosed() noexcept -> bool {
  this->AcquireLock();
  bool const is_closed{CheckSocketState(BasicSocketState::kClosed)};
  this->ReleaseLock();

  return is_closed;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the socket object is in closed state
 *   - create an operating system socket.
 *   - Assign the native handle of the operating system to the socket object.
 * - Leave critical section.
 * - Output the result of opening the socket.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::Open(ProtocolType const& protocol,
                                                net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<void> {
  // For the happy path, nothing can be going on in the reactor context before Open() is called. But for error cases
  // (e.g. trying to open the same object multiple times) this is not guaranteed, thus making the lock necessary here.
  this->AcquireLock();

  ::amsr::core::Result<void> const open_result{
      this->CheckSocketIsClosed()
          .AndThen([&protocol, &netstack_id]() -> ::amsr::core::Result<osabstraction::io::NativeHandle> {
            SocketFamily const family{protocol.GetFamily()};
            SocketType const type{protocol.GetType()};
            SocketProtocol const sock_protocol{protocol.GetProtocol()};
            return os_interface::CreateSocket(family, type, sock_protocol, netstack_id);
          })
          .AndThen(
              [this, &protocol, &netstack_id](osabstraction::io::NativeHandle handle) -> ::amsr::core::Result<void> {
                return this->AssignNativeHandle(protocol, handle, netstack_id);
              })};

  this->ReleaseLock();

  return open_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - If socket is in state open
 *   - set socket to state closing.
 *   - Inform child class object part about closed state. They have to reset their internal state too.
 *   - If the socket has a reactor
 *     - unregister the native handle. Set the reactor to close the native handle.
 *     - Update the closing state.
 *   - If the socket does not have a reactor
 *     - close the native handle.
 *     - Set socket state to closed.
 * - Leave critical section.
 * - If the socket was in state open
 *   - output success.
 * - If the socket was not in state open
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::Close() noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const result{this->CheckSocketIsOpen()};

  if (result.HasValue()) {
    socket_state_ = BasicSocketState::kClosing;

    // Child must be informed about close before the handle is unregistered from the reactor so they can adjust their
    // state while the reactor is still open. This is required to avoid race conditions in case the child is
    // concurrently changing reactor events with the handle already unregistered.
    // VECTOR Next Construct VectorC++-V5.2.3: MD_OSA_V5.2.3_CRTP
    // VECTOR Next Construct AutosarC++17_10-M5.2.3: MD_OSA_M5.2.3_CRTP
    static_cast<DerivedSocket*>(this)->OnCloseEvent();

    if (this->HasReactor()) {
      static_cast<void>(this->GetReactorUnsafe().Unregister(
          this->reactor_handle_, osabstraction::io::reactor1::CloseOnUnregister::kCloseHandle));
      UpdateSocketClosingState();
    } else {
      os_interface::Close(this->GetNativeHandle());
      SetSocketStateClosed();
    }
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - If socket is open
 *   - update socket blocking state.
 *   - Cache new blocking state on success.
 * - Leave critical section.
 * - If the socket was open
 *   - output success.
 * - If the socket was not open
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::SetBlockingMode(bool enable) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const blocking_mode_set_result{this->CheckSocketIsOpen().Inspect([this, enable]() {
    // Lock is held during system call because this call is not a blocking call and holding it here allows to prevent
    // operations in the reactor from blocking.
    os_interface::SetBlockingMode(this->GetNativeHandle(), internal::SocketBlockingMode{enable});
    cached_blocking_mode_ = enable;
  })};

  this->ReleaseLock();

  return blocking_mode_set_result;
}

/*!
 * \internal
 * - If this socket state is open:
 *   - Close the socket.
 * - Abort if socket is still open as an asynchronous operation is still ongoing on this socket because continuing
 *   would result in undefined behavior
 * - Move other socket.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::MoveSocket(BasicSocket&& other) noexcept {
  // Self assignment forbidden by precondition.
  if (this->CheckIsOpen()) {
    // Ignore error. We check after the close if close succeeded. If not we have to abort anyway at this point.
    static_cast<void>(this->Close());

    // Ongoing operation on socket while it has to be closed during move operation.
    if (!this->CheckIsClosed()) {
      ::amsr::core::Abort("Socket cannot be moved if it cannot be closed.");
    }
  }
  MoveOtherSocket(other);
}

/*!
 * \internal
 * - Enter critical section.
 * - Set the option.
 * - Leave critical section.
 * - Output the result of setting the option.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
template <class SocketOption>
auto BasicSocket<Protocol, DerivedSocket>::SetOption(SocketOption option) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const set_option_result{SetOptionInternal<SocketOption>(option)};

  this->ReleaseLock();

  return set_option_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Get the option value.
 * - Leave critical section.
 * - Output the result of getting the option value.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
template <class SocketOption>
auto BasicSocket<Protocol, DerivedSocket>::GetOption() noexcept -> ::amsr::core::Result<SocketOption> {
  this->AcquireLock();

  ::amsr::core::Result<SocketOption> const result{this->GetOptionInternal(OverloadType<SocketOption>{})};

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Verify the precondition.
 * - If a reactor interface is available
 *   - register to the reactor.
 *   - If registration fails
 *     - close the socket.
 *     - output the error that made registration fail.
 *   - If registration succeeds
 *     - set socket state to open.
 *     - Store network stack ID.
 *     - output success.
 * - If no reactor interface is available
 *   - set socket state to open.
 *   - Store network stack ID.
 *   - Output success.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::AssignNativeHandle(ProtocolType const& protocol,
                                                              osabstraction::io::NativeHandle handle,
                                                              net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<void> {
  AssertLockHeldByThread();

  native_handle_ = handle;
  ::amsr::core::Result<void> result{};  // Success for sockets without reactors.
  if (HasReactor()) {
    result = RegisterToReactor();

    if (!result.HasValue()) {
      os_interface::Close(this->GetNativeHandle());
      this->native_handle_ = osabstraction::io::kInvalidNativeHandle;
    }
  }

  if (result.HasValue()) {
    protocol_.emplace(protocol);
    socket_state_ = BasicSocketState::kOpen;
    network_stack_id_ = netstack_id;
  }

  return result;
}

/*!
 * \internal
 * - Verify the precondition.
 * - Enable or disable the registration of the read event as specified by the input.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::ChangeReadObservation(bool enable) noexcept {
  AssertLockHeldByThread();

  osabstraction::io::reactor1::EventTypes read_event{};
  read_event.SetReadEvent(true);
  ChangeReactorObservation(read_event, enable);
}

/*!
 * \internal
 * - Verify the precondition.
 * - Enable or disable the registration of the write event as specified by the input.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::ChangeWriteObservation(bool enable) noexcept {
  AssertLockHeldByThread();

  osabstraction::io::reactor1::EventTypes write_event{};
  write_event.SetWriteEvent(true);
  ChangeReactorObservation(write_event, enable);
}

/*!
 * \internal
 * - Verify the precondition.
 * - Enable or disable the registration of the event as specified by the input.
 * - Filter fatal reactor errors.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::ChangeReactorObservation(
    osabstraction::io::reactor1::EventTypes single_event, bool enable) noexcept {
  AssertLockHeldByThread();

  ::amsr::core::Result<void> result{};
  if (enable) {
    result = this->GetReactorUnsafe().AddMonitoredEvents(this->reactor_handle_, single_event);
  } else {
    result = this->GetReactorUnsafe().RemoveMonitoredEvents(this->reactor_handle_, single_event);
  }
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  static_cast<void>(result.InspectError(
      [](::amsr::core::ErrorCode error) -> void { FilterFatalReactorChangeMonitoredEventsErrors(error); }));
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open:
 *   - Return a value.
 * - Otherwise:
 *   - Return the kApiError.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckSocketIsOpen() const noexcept -> ::amsr::core::Result<void> {
  AssertLockHeldByThread();

  return (socket_state_ == BasicSocketState::kOpen)
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError, "Socket is not open")};
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is closed:
 *   - Return a value.
 * - Otherwise:
 *   - Return the kApiError.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckSocketIsClosed() noexcept -> ::amsr::core::Result<void> {
  AssertLockHeldByThread();

  return CheckSocketState(BasicSocketState::kClosed)
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError, "Socket is not closed")};
}

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
/*!
 * \internal
 * - Verify the precondition.
 * - If blocking mode is enabled:
 *   - Abort the process.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::AssertBlockingModeIsDisabled() const noexcept {
  if (this->cached_blocking_mode_) {
    ::amsr::core::Abort("Asynchronous send/receive/connect with blocking mode enabled is not allowed.");
  }
}

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
/*!
 * \internal
 * - If the provided size exceeds the allowed IO buffer array size:
 *   - Abort the process.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::AssertProperIOBufferArraySize(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer>::size_type size) noexcept {
  if (size > osabstraction::io::kMaxIOBufferArraySize) {
    ::amsr::core::Abort(
        "The IO buffer array's size exceeds the maximum limit of osabstraction::io::kMaxIOBufferArraySize.");
  }
}

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
/*!
 * \internal
 * - If the provided size exceeds the allowed IO buffer size:
 *   - Abort the process.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::AssertProperIOBufferSize(
    ::amsr::core::Span<std::uint8_t>::size_type size) noexcept {
  if (size > osabstraction::io::kMaxIOBufferSize) {
    ::amsr::core::Abort("The IO buffer's size exceeds the maximum limit of osabstraction::io::kMaxIOBufferSize.");
  }
}

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
/*!
 * \internal
 * - If the socket does not have a reactor:
 *   - Abort the process.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::AssertReactorAvailable() const noexcept {
  if (!HasReactor()) {
    ::amsr::core::Abort("Socket does not have a reactor.");
  }
}

// VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_OSA_A15.5.3_QNXReactor1VectorAllocation
// VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_OSA_A15.4.2_QNXReactor1VectorAllocation
/*!
 * \internal
 * - Register to reactor with all events disabled.
 * - If registering to the reactor failed
 *   - abort for fatal errors.
 *   - Output non-fatal errors.
 * - If registering to the reactor succeeded
 *   - store callback handle and deactivate all active observations.
 *   - Output success.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::RegisterToReactor() noexcept -> ::amsr::core::Result<void> {
  // The lock must be held at this point because the callback may be called before Reactor1 Register function returns.
  ::amsr::core::Result<void> result{
      this->GetReactorUnsafe()
          .Register(this->GetNativeHandle(), osabstraction::io::reactor1::EventTypes{},
                    [this](osabstraction::io::reactor1::CallbackHandle,
                           osabstraction::io::reactor1::EventTypes events) noexcept -> void {
                      this->OnReactorCallback(events);
                    })
          .AndThen([this](osabstraction::io::reactor1::CallbackHandle reactor_handle) -> ::amsr::core::Result<void> {
            this->reactor_handle_ = reactor_handle;
            return ::amsr::core::Result<void>{};
          })};

  if (!result.HasValue()) {
    result.EmplaceError(FilterFatalReactorRegistrationErrors(result.Error()));
  }

  return result;
}

/*!
 * \internal
 * - If socket has reactor
 *   - set state to close if no callback is executed.
 * - If the socket does not have a reactor
 *   - set state to closed.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::UpdateSocketClosingState() noexcept {
  if (this->HasReactor()) {
    if (!this->GetReactorUnsafe().IsInUse(reactor_handle_)) {
      SetSocketStateClosed();
    }
  } else {
    SetSocketStateClosed();
  }
}

/*!
 * \internal
 * - Reset protocol
 * - Reset native handle
 * - Set state to closed.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::SetSocketStateClosed() noexcept {
  protocol_.reset();
  native_handle_ = osabstraction::io::kInvalidNativeHandle;
  socket_state_ = BasicSocketState::kClosed;
}

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_OnReactorCallbackMayHaveSideEffects
/*!
 * \internal
 * - Enter critical section.
 * - If the socket is open
 *   - forward the reactor call to the derived class part of the object.
 * - Leave critical section.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::OnReactorCallback(osabstraction::io::reactor1::EventTypes events) noexcept {
  // PTP-B-OsAbstraction-Net-ReactorTask
  this->AcquireLock();
  if (this->CheckSocketIsOpen().HasValue()) {
    // Do not filter here for closing state. Its up to the reactor and CheckIsClosed() calls to deal with the closing
    // to closed transition.
    // VECTOR Next Construct VectorC++-V5.2.3: MD_OSA_V5.2.3_CRTP
    // VECTOR Next Construct AutosarC++17_10-M5.2.3: MD_OSA_M5.2.3_CRTP
    static_cast<DerivedSocket*>(this)->OnReactorEvents(events);
  }
  this->ReleaseLock();
  // PTP-E-OsAbstraction-Net-ReactorTask
}

/*!
 * \internal
 * - Copy member variables and set other sockets member variables to initial value. Do not copy other's members.
 * - Set other to closed as it is now invalid.
 * - If socket state is open and reactor is used
 *   - change the callback registered to the reactor to this objects handler.
 *   - If changing the callback failed
 *     - abort execution for the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
void BasicSocket<Protocol, DerivedSocket>::MoveOtherSocket(BasicSocket& other) noexcept {
  this->cached_blocking_mode_ = other.cached_blocking_mode_;
  this->native_handle_ = other.native_handle_;
  if (other.protocol_.has_value()) {
    this->protocol_.emplace(other.protocol_.value());
  }
  this->reactor_ = other.reactor_;
  this->reactor_handle_ = other.reactor_handle_;
  this->socket_state_ = other.socket_state_;
  this->network_stack_id_ = other.network_stack_id_;

  other.SetSocketStateClosed();

  if (this->HasReactor()) {
    if (this->CheckIsOpen()) {
      static_cast<void>(
          this->GetReactorUnsafe()
              .SetCallbackTarget(this->reactor_handle_,
                                 [this](osabstraction::io::reactor1::CallbackHandle,
                                        osabstraction::io::reactor1::EventTypes events) noexcept -> void {
                                   this->OnReactorCallback(events);
                                 })
              // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
              .InspectError(
                  [](::amsr::core::ErrorCode error) -> void { FilterFatalReactorSetCallbackTargetErrors(error); }));
    }
  }
}

/*!
 * \internal
 * - If the protocol type of the provided endpoint matches protocol type of this socket:
 *   - Return a value.
 * - Otherwise:
 *   - Return kApiError error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckEndpointProtocolMatches(EndpointType const& endpoint) const noexcept
    -> ::amsr::core::Result<void> {
  ProtocolType const& protocol{this->GetProtocolUnsafe()};
  return (endpoint.GetProtocol() == protocol)
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{
                   MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                 "Provided endpoint protocol does not match the protocol of the socket")};
}

/*!
 * \internal
 * - If the passed network stack ID matches the ID of the network stack used by this socket:
 *   - Return a value.
 * - Otherwise:
 *   - Return kApiError error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckSocketUsesNetworkStack(
    net_utils::NetworkStackId const& network_stack_id) const noexcept -> ::amsr::core::Result<void> {
  return (network_stack_id_ == network_stack_id)
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{
                   MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                 "Provided network stack ID does not match the network stack used by this socket.")};
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open and bound.
 *   - resolve endpoint according to the provided function and report result.
 * - If the socket is not open and/or not bound
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::GetEndpoint(
    os_interface::OsInterfaceGetEndpointFunction get_endpoint_function) noexcept -> ::amsr::core::Result<EndpointType> {
  this->AssertLockHeldByThread();

  return this->CheckSocketIsOpen()
      .AndThen([this, get_endpoint_function]() -> ::amsr::core::Result<EndpointType> {
        ProtocolType const& protocol{this->GetProtocolUnsafe()};
        net_utils::NetworkStackId const& network_stack_id{this->GetNetworkStackId()};
        EndpointType endpoint_buffer{protocol, network_stack_id};
        osabstraction::io::NativeHandle const handle{this->GetNativeHandle()};
        ::amsr::core::Result<void> const get_endpoint_result{get_endpoint_function(handle, endpoint_buffer.GetData())};

        return (get_endpoint_result.HasValue()) ? ::amsr::core::Result<EndpointType>{endpoint_buffer}
                                                : ::amsr::core::Result<EndpointType>{get_endpoint_result.Error()};
      })
      .OrElse([](::amsr::core::ErrorCode error) -> ::amsr::core::Result<EndpointType> {
        return ::amsr::core::Result<EndpointType>{error};
      });
}

/*!
 * \internal
 * - Enter critical section.
 * - Resolve local endpoint.
 * - Leave critical section.
 * - Report result of resolving the endpoint.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::GetSocketLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  this->AcquireLock();

  ::amsr::core::Result<EndpointType> result{MakeErrorCode(osabstraction::OsabErrc::kApiError)};

  // VECTOR Next Construct VectorC++-V5.2.3: MD_OSA_V5.2.3_CRTP
  // VECTOR Next Construct AutosarC++17_10-M5.2.3: MD_OSA_M5.2.3_CRTP
  if (static_cast<DerivedSocket*>(this)->IsSocketBound()) {
    result = this->GetEndpoint(&os_interface::GetSocketName);
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Extract the local endpoint the socket is bound to.
 * - Extract the interface from the local endpoint.
 * - Extract the Ethernet address from the interface.
 * - If one of the above steps failed return the appropriate error and skip the remaining steps.
 * - Otherwise return the extracted Ethernet address.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::GetSocketPhysicalAddress() noexcept
    -> ::amsr::core::Result<amsr::net::EthernetAddress> {
  return GetSocketLocalEndpoint()
      .AndThen([](EndpointType const& local_endpoint) -> ::amsr::core::Result<Interface> {
        return local_endpoint.GetInterface();
      })
      .AndThen([](Interface const& interface) -> ::amsr::core::Result<amsr::net::EthernetAddress> {
        return interface.ExtractEthAddress();
      });
}

/*!
 * \internal
 * - If the socket is in closing state
 *   - update the socket state.
 * - Output if the socket is in the expected state.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::CheckSocketState(BasicSocketState expected_state) noexcept -> bool {
  if (socket_state_ == BasicSocketState::kClosing) {
    UpdateSocketClosingState();
  }
  return socket_state_ == expected_state;
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open
 *   - set the passed socket option.
 *   - Output the result of setting the option.
 * - If the socket is not open
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
template <class SocketOption>
auto BasicSocket<Protocol, DerivedSocket>::SetOptionInternal(SocketOption const option) noexcept
    -> ::amsr::core::Result<void> {
  this->AssertLockHeldByThread();

  return this->CheckSocketIsOpen().AndThen([this, &option]() -> ::amsr::core::Result<void> {
    ProtocolType const& protocol{this->GetProtocolUnsafe()};
    osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
    SockOptLevel const level{option.GetLevel(protocol)};
    SockOptName const name{option.GetName(protocol)};
    SockOptConstValueView const data_view{option.GetDataView(protocol)};
    return os_interface::SetSocketOption(native_handle, level, name, data_view);
  });
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open
 *   - Try to acquire the value of the passed socket option.
 *   - Output the result of getting the value.
 * - If the socket is not open
 *   - Output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
template <class SocketOption>
auto BasicSocket<Protocol, DerivedSocket>::GetOptionInternal(OverloadType<SocketOption>) const noexcept
    -> ::amsr::core::Result<SocketOption> {
  this->AssertLockHeldByThread();

  SocketOption option{};

  return this->CheckSocketIsOpen()
      .AndThen([this, &option]() -> ::amsr::core::Result<void> {
        ProtocolType const& protocol{this->GetProtocolUnsafe()};
        osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
        SockOptLevel const level{option.GetLevel(protocol)};
        SockOptName const name{option.GetName(protocol)};
        SockOptMutableValueView const data_view{option.GetDataView(protocol)};

        return os_interface::GetSocketOption(native_handle, level, name, data_view);
      })
      .Map([&option]() -> SocketOption { return option; });
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open
 *   - Initialize a SocketOptionMulticastNetworkInterface socket option with the network stack ID of this socket.
 *   - Try to acquire the value of the SocketOptionMulticastNetworkInterface socket option.
 *   - Output the result of getting the value.
 * - If the socket is not open
 *   - Output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::GetOptionInternal(OverloadType<SocketOptionMulticastNetworkInterface>)
    const noexcept -> ::amsr::core::Result<SocketOptionMulticastNetworkInterface> {
  this->AssertLockHeldByThread();

  SocketOptionMulticastNetworkInterface option{network_stack_id_};

  return this->CheckSocketIsOpen()
      .AndThen([this, &option]() -> ::amsr::core::Result<void> {
        ProtocolType const& protocol{this->GetProtocolUnsafe()};
        osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
        SockOptLevel const level{option.GetLevel(protocol)};
        SockOptName const name{option.GetName(protocol)};
        SockOptMutableValueView const data_view{option.GetDataView(protocol)};

        return os_interface::GetSocketOption(native_handle, level, name, data_view);
      })
      .Map([&option]() -> SocketOptionMulticastNetworkInterface { return option; });
}

/*!
 * \internal
 * - Verify the precondition.
 * - If the socket is open
 *   - Initialize a SockOptBindToDevice socket option with the network stack ID of this socket.
 *   - Try to acquire the value of the SockOptBindToDevice socket option.
 *   - Output the result of getting the value.
 * - If the socket is not open
 *   - Output the respective error.
 * \endinternal
 */
template <class Protocol, class DerivedSocket>
auto BasicSocket<Protocol, DerivedSocket>::GetOptionInternal(OverloadType<SockOptBindToDevice>) const noexcept
    -> ::amsr::core::Result<SockOptBindToDevice> {
  this->AssertLockHeldByThread();

  SockOptBindToDevice option{network_stack_id_};

  return this->CheckSocketIsOpen()
      .AndThen([this, &option]() -> ::amsr::core::Result<void> {
        ProtocolType const& protocol{this->GetProtocolUnsafe()};
        osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
        SockOptLevel const level{option.GetLevel(protocol)};
        SockOptName const name{option.GetName(protocol)};
        SockOptMutableValueView const data_view{option.GetDataView(protocol)};

        return os_interface::GetSocketOption(native_handle, level, name, data_view);
      })
      .Map([&option]() -> SockOptBindToDevice { return option; });  // COV_OSAB_BINDTODEVICE_IO_SOCK
}

/*!
 * \exclusivearea ::amsr::net::internal::BasicSocket::lock_::mutex_
 * Ensures atomicity when jointly accessing object state and reactor event settings for data transfer operations.
 * \protects ::amsr::net::internal::BasicSocket::reactor_,
 *           ::amsr::net::internal::BasicSocket::socket_state_,
 *           ::amsr::net::internal::BasicSocket::cached_blocking_mode_,
 *           data of derived classes
 * \usedin   ::amsr::net::internal::BasicSocket::Close,
 *           ::amsr::net::internal::BasicSocket::SetBlockingMode,
 *           ::amsr::net::internal::BasicSocket::OnReactorCallback,
 *           ::amsr::net::internal::BasicSocket::CheckSocketState,
 *           ::amsr::net::internal::BasicSocket::AssertBlockingModeIsDisabled,
 *           ::amsr::net::internal::BasicSocket::ChangeReadObservation,
 *           ::amsr::net::internal::BasicSocket::ChangeWriteObservation,
 *           ::amsr::net::internal::BasicSocket::ChangeReactorObservation,
 *           ::amsr::net::internal::BasicSocket::Open,
 *           ::amsr::net::internal::BasicSocket::AssignNativeHandle,
 *           ::amsr::net::internal::BasicSocket::RegisterToReactor,
 *           ::amsr::net::internal::BasicSocket::GetOption,
 *           ::amsr::net::internal::BasicSocket::SetOption,
 *           ::amsr::net::internal::BasicSocket::GetOptionInternal,
 *           ::amsr::net::internal::BasicSocket::SetOptionInternal,
 *           ::amsr::net::internal::BasicSocket::GetSocketLocalEndpoint
 * \exclude  All other of these methods of the same object.
 * \length   LONG Enabling and disabling events on the reactor.
 *
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_H_

// COV_JUSTIFICATION_BEGIN
//  \ID COV_OSAB_BINDTODEVICE_IO_SOCK
//  \ACCEPT XX
//  \REASON This lambda is not expected to ever be called if io-sock is used as network manager on QNX as it handles the
//          case of a successful getsockopt(SO_BINDTODEVICE) call which is not possible as SO_BINDTODEVICE is not
//          supported by by io-sock.
// COV_JUSTIFICATION_END
