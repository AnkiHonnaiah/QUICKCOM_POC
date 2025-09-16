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
/*!        \file  libosabstraction-posix/include/osabstraction/io/ipc1/multicast_sender.h
 *        \brief  Implementation of basic IPC multicast sender.
 *        \unit   osabstraction::ipc1
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_SENDER_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/result.h"
#include "vac/language/unique_function.h"

#include "amsr/core/span.h"
#include "amsr/net/ip/udp.h"
#include "osabstraction/io/ipc1/internal/multicast_address_utils.h"
#include "osabstraction/io/ipc1/internal/multicast_errors.h"
#include "osabstraction/io/ipc1/multicast_address.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace osabstraction {
namespace io {
namespace ipc1 {

/*!
 * \brief    Multicast sender class.
 * \details  Exploits io::net::Udp::Socket to implement IPC multicast send functionality.
 *
 * \vprivate Vector product internal API
 */
class MulticastSender final {
 public:
  /*!
   * \brief The limit for one UDP datagram package.
   *
   * \details
   * Theoretical limit (16 Bit length field in header) 0xFFFF - (IPv6 Header + UDP Header)
   */
  static constexpr std::uint32_t kMaximumDatagramSize{0xFFFF - (40 + 8)};

  /*!
   * \brief           The udp namespace.
   */
  using Udp = amsr::net::ip::Udp;

  /*!
   * \brief           The reactor interface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief           The socket address abstraction.
   */
  using Address = amsr::net::ip::Address;

  /*!
   * \brief           The operating system port abstraction.
   */
  using Port = amsr::net::ip::Port;

  /*!
   * \brief           The network interface abstraction.
   */
  using NetInterface = amsr::net::Interface;

  /*!
   * \brief         Constructs a MulticastSender.
   *
   * \param[in]     reactor Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                        lifetime of this MulticastSender object.
   * \param[in]     address Multicast address to which messages can be sent.
   *
   * \return          -
   *
   * \error           -
   *
   * \context       ANY
   *
   * \pre             -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate      Vector product internal API
   */
  explicit MulticastSender(ReactorInterface& reactor, MulticastAddress address) noexcept;

  /*!
   * \brief     Deleted default constructor.
   */
  MulticastSender() = delete;

  /*!
   * \brief     Deleted copy constructor.
   */
  MulticastSender(MulticastSender const&) = delete;

  /*!
   * \brief     Deleted move constructor.
   */
  MulticastSender(MulticastSender&&) = delete;

  /*!
   * \brief     Deleted copy operator.
   */
  MulticastSender& operator=(MulticastSender const&) = delete;

  /*!
   * \brief     Deleted move operator.
   */
  MulticastSender& operator=(MulticastSender&&) = delete;

  /*!
   * \brief           Destroys a MulticastSender.
   *
   * \param           -
   *
   * \return          -
   *
   * \error           -
   *
   * \context         ANY
   *
   * \pre             IsInUse() == false
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate        Vector product internal API
   */
  ~MulticastSender() noexcept = default;

  /*!
   * \brief     Notifies about a completed asynchronous send operation.
   *
   * \param[in] result The result of the send operation. On success an empty value is returned, otherwise an error.
   *
   *  Possible errors reported in result formal parameter:
   *        osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                 Not enough system resources to send datagram.
   *        osabstraction::OsabErrc::kDisconnected             No route to destination exists. Note that this may be
   *                                                           an asynchronous network error that has been caused by a
   *                                                           previous datagram.
   *        osabstraction::OsabErrc::kSystemEnvironmentError   The datagram exceeds the maximum allowed size (e.g.
   *                                                           message size > MTU size) or a kernel subsystem is not
   *                                                           configured.
   *
   * \return          -
   *
   * \error           -
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate        Vector product internal API
   */
  using SendCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

  /*!
   * \brief           Acquires and initializes all resources needed to perform multicast send operation.
   *
   * \details         The io::udp::Socket is opened, bound and connected to the IPv6 address created out of the
   *                  multicast address passed to the constructor.
   *                  The network interface with index 1 (loopback device) is set to be used for multicast datagrams
   *                  send operations.
   *
   * \return          The result of the service.
   *
   * \error           OsabErrc::kUnexpected                Some unexpected error was detected.
   * \error           OsabErrc::kResource                  Not enough resources to create a multicast sender.
   * \error           OsabErrc::kAddressNotAvailable       The address is already in use on the local machine.
   * \error           OsabErrc::kApiError                  The socket has been concurrently closed.
   * \error           OsabErrc::OsabErrc::kDisconnected    The peer is not reachable or the route to the peer is
   *                                                       configured as blackhole in the routing table.
   *
   * \context         ANY
   *
   * \pre             Init() has not been called.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate        Vector product internal API
   */
  auto Init() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Requests an optimistic send operation.
   *
   * \note          Can be called from SendCompletionCallback callback.
   *
   * \details       This function will first try to send the message with a non-blocking synchronous service.
   *                If this approach fails due to the osabstraction::OsabErrc::kBusy error the asynchronous service is
   *                performed and the user has to drive the HandleEvents loop until the SendCompletionCallback
   *                is called
   *                The service finishes if one of the three conditions evaluates to true.
   *                - This function returns SendResult::kSendCompleted.
   *                - The passed completion callback is called.
   *                - IsInUse() == false.
   *
   * \param[in] buffers_view                  View to an array of IOBuffers. The IOBuffers must point to valid buffers.
   *                                          The MulticastSender will take ownership of the buffers and will hold the
   *                                          ownership till the callback has been executed or till IsInUse() == false.
   * \param[in] user_send_completion_callback Callback to notify the completion of the ReceiveAsync operation.
   *                                          The MulticastSender will take ownership of the passed
   *                                          callback and will hold the ownership till the callback has been
   *                                          executed or till IsInUse() == false.
   *
   * \return The result of the send operation
   *
   * \retval SendResult::kSendCompleted             The operation has been executed directly. MulticastSender
   *                                                has released all ownerships and the callback will not be called.
   * \retval SendResult::kAsyncProcessingNecessary  The operation could not be executed directly. It will be processed
   *                                                asynchronously in the reactor context. The passed completion
   *                                                callback will notify about a finished send operation.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                            an asynchronous network error that has been caused by a
   *                                                            previous datagram.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                            message size > MTU size) or a kernel subsystem is not
   *                                                            configured.
   *
   * \context       ANY
   *
   * \pre           Init() has been successfully executed.
   * \pre           Close() has not been called.
   * \pre           There are no pending send requests.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate      Vector product internal API
   */
  auto Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> buffers_view,
            SendCompletionCallback&& user_send_completion_callback) noexcept
      -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief         Releases all resources allocated by the MulticastSender.
   *
   * \details       The socket callback will be unregistered and the associated file descriptor automatically closed:
   *                this can be delayed if the socket callback is simultaneously executing on another thread.
   *
   * \return        Result of the request.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                  Socket is already closed.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE unless callback is simultaneously executing on a different thread.
   * \threadsafe    FALSE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate      Vector product internal API
   */
  auto Close() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Checks if any of the resource acquired by MulticastSender may be still in use.
   *
   * \return        true if there are resources in use, otherwise false.
   *
   * \error           -
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate      Vector product internal API
   */
  auto IsInUse() noexcept -> bool;

 private:
  /*!
   * \brief         Multicast send completion callback.
   *
   * \details       Intermediate callback that is executed to notify the MulticastSender of completed send
   *                operation from the Udp::Socket layer. Error handling and call of the send_completion_callback_
   *                is performed.
   *
   * \param[in]     result The result of the send operation.
   *
   * Possible errors reported in result formal parameter:
   *        osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   *        osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   *        osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                            message size > MTU size) or a kernel subsystem is not
   *                                                            configured.
   *        osabstraction::OsabErrc::kDisconnected              No route to destination exists or the route is
   *                                                            configured as blackhole in the routing table. Note that
   *                                                            this may be an asynchronous network error that has been
   *                                                            caused by a previous datagram.
   *
   * \context       Reactor context
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastSenderPosix
   *
   * \vprivate      Vector component internal API
   */
  void MulticastSendCompletionCallback(::amsr::core::Result<void>&& result) noexcept;

  /*!
   * \brief         Sets the socket send buffer size if it was configured at compile time.
   *
   * \error         osabstraction::OsabErrc::kUnexpected   Unexpected error that is mapped to no category.
   *
   * \context       MulticastSender::Init()
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        TRUE
   */
  ::amsr::core::Result<void> ConfigureSendBufferSize() noexcept;

  /*!
   * \brief  Multicast address.
   */
  MulticastAddress address_;

  /*!
   * \brief  Udp socket.
   */
  Udp::Socket socket_;

  /*!
   * \brief  Stores the callback to be executed on a completed asynchronous send operation.
   */
  SendCompletionCallback send_completion_callback_;
};

}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_SENDER_H_
