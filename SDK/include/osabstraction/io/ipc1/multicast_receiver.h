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
/*!        \file  libosabstraction-posix/include/osabstraction/io/ipc1/multicast_receiver.h
 *        \brief  Implementation of basic IPC multicast receiver.
 *        \unit   osabstraction::ipc1
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_RECEIVER_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_RECEIVER_H_

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
 * \brief    Multicast receiver class.
 *
 * \details  Exploits io::net::Udp::Socket to implement IPC multicast receive functionality.
 *
 * \vprivate Vector product internal API
 */
class MulticastReceiver final {
 public:
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
   * \brief         Constructs a MulticastReceiver.
   *
   * \param[in]     reactor Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                        lifetime of this MulticastReceiver object.
   *
   * \param[in]     address Multicast address that has to be joined.
   *
   * \return          -
   *
   * \error           -
   *
   * \context       ANY
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate      Vector product internal API
   */
  explicit MulticastReceiver(ReactorInterface& reactor, MulticastAddress address) noexcept;

  /*!
   * \brief     Deleted default constructor.
   */
  MulticastReceiver() = delete;

  /*!
   * \brief     Deleted copy constructor.
   */
  MulticastReceiver(MulticastReceiver const&) = delete;

  /*!
   * \brief     Deleted move constructor.
   */
  MulticastReceiver(MulticastReceiver&&) = delete;

  /*!
   * \brief     Deleted copy operator.
   */
  MulticastReceiver& operator=(MulticastReceiver const&) = delete;

  /*!
   * \brief     Deleted move operator.
   */
  MulticastReceiver& operator=(MulticastReceiver&&) = delete;

  /*!
   * \brief           Destroys a MulticastReceiver.
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
   * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate        Vector product internal API
   */
  ~MulticastReceiver() noexcept = default;

  /*!
   * \brief Notifies about a completed asynchronous receive operation.
   *
   * \param[in] result The result of the receive operation. On success it contains the number of bytes
   *                   written to the user buffer.
   *
   * Possible errors reported in result formal parameter:
   *        osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   *        osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by
   *                                                             a previous datagram.
   *        osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received datagram. Datagram has been truncated.
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
   * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate        Vector product internal API
   */
  using ReceiveCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<std::size_t>&& result)>;

  /*!
   * \brief           Acquires and initializes all resources needed to perform multicast receive operation.
   *
   * \details         The io::udp::Socket is opened, bound to the IPv6 address created out of the multicast address
   *                  passed to the constructor and joins the multicast group defined by the previously mentioned
   *                  address. The used network interface is the one with index 1 (loopback device).
   *
   * \return          amsr::core::Result<void>.
   *
   * \error           OsabErrc::kUnexpected                Some unexpected error was detected.
   * \error           OsabErrc::kResource                  Not enough resources to create or join a multicast group.
   * \error           OsabErrc::kApiError                  The MulticastReceiver is already initialized.
   * \error           OsabErrc::kAddressNotAvailable       The address is already in use on the local machine.
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
   * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate        Vector product internal API
   */
  auto Init() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Requests an asynchronous receive operation.
   *
   * \note          Can be called from ReceiveCompletionCallback callback.
   *
   * \details       The caller has to drive the HandleEvents loop until the ReceiveCompletionCallback will be called
   *                (this will happen only if there are datagrams available in the kernel buffer to be received).
   *
   * \param[in]     buffers_view                       View to an array of IOBuffers. The IOBuffers must point to valid
   *                                                   buffers. The incoming datagram will be written to the provided
   *                                                   buffers. The content of the buffers is undefined if the service
   *                                                   reports an error.
   *                                                   The MulticastReceiver will take ownership of the buffer and will
   *                                                   hold the ownership till the callback has been executed or till
   *                                                   IsInUse() == false.
   * \param[in]     user_receive_completion_callback   Callback to notify the completion of the ReceiveAsync operation.
   *                                                   The MulticastReceiver will take ownership of the passed
   *                                                   callback and will hold the ownership till the callback has been
   *                                                   executed or till IsInUse() == false.
   *
   * \context       ANY
   *
   * \pre           Init() has been successfully executed.
   * \pre           Close() has not been called.
   * \pre           There are no pending read requests.
   *
   * \reentrant     FALSE
   * \synchronous   FALSE
   * \threadsafe    FALSE
   * \steady        FALSE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate      Vector product internal API
   */
  void ReceiveAsync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffers_view,
                    ReceiveCompletionCallback&& user_receive_completion_callback) noexcept;

  /*!
   * \brief         Releases all resources allocated by the MulticastReceiver.
   *
   * \details       The socket callback will be unregistered and the associated file descriptor automatically closed:
   *                this can be delayed if the socket callback is simultaneously executing on another thread.
   *
   * \return        amsr::core::Result<void>.
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
   * \trace         DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate      Vector product internal API
   */
  auto Close() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Checks if any of the resource acquired by MulticastReceiver may be still in use.
   *
   * \return        true if there are resources in use, otherwise false.
   *
   * \error           -
   *
   * \context       ANY
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate      Vector product internal API
   */
  auto IsInUse() noexcept -> bool;

 private:
  /*!
   * \brief         Multicast receive completion callback.
   *
   * \details       Intermediate callback that is executed to notify the MulticastReceiver of completed receive
   *                operation from the Udp::Socket layer. Error handling and call of the receive_completion_callback_
   *                is performed.
   *
   * \param[in]     result The result of the receive operation.
   *
   * Possible errors reported in result formal parameter:
   *        osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   *        osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   *        osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by
   *                                                             a previous datagram.
   *        osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received datagram. The trailing bytes of the datagram
   *                                                             have been dropped.
   *
   * \context         Reactor context
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix
   *
   * \vprivate        Vector component internal API
   */
  void MulticastReceiveCompletionCallback(::amsr::core::Result<Udp::Socket::DatagramInfo>&& result) noexcept;

  /*!
   * \brief         Sets the socket receive buffer size if it was configured at compile time.
   *
   * \error         osabstraction::OsabErrc::kUnexpected   Unexpected error that is mapped to no category.
   *
   * \context       MulticastReceiver::Init()
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        TRUE
   */
  ::amsr::core::Result<void> ConfigureReceiveBufferSize() noexcept;

  /*!
   * \brief  Multicast address.
   */
  MulticastAddress address_;

  /*!
   * \brief  Udp socket.
   */
  Udp::Socket socket_;

  /*!
   * \brief  Stores the callback to be executed on a completed asynchronous receive operation.
   */
  ReceiveCompletionCallback receive_completion_callback_;
};

}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_RECEIVER_H_
