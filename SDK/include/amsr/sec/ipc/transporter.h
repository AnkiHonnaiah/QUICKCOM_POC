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
/*!        \file  transporter.h
 *        \brief  Interface for Transporter.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>

#include "amsr/core/future.h"
#include "amsr/core/promise.h"
#include "amsr/sec/ipc/ipc_error_domain.h"
#include "amsr/sec/ipc/transporter_event_handler.h"
#include "amsr/sec/ipc/types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Buffer size of transport buffer to serialize / deserialize data
 * \vprivate Component Private
 */
constexpr std::uint32_t kTransportBufferSize{2050124};
static_assert(kTransportBufferSize <= std::numeric_limits<ReadOnlyMemRegion::size_type>::max(),
              "Sizing violation: Size of transport buffer must not exceed numeric limit for given container.");
static_assert(kTransportBufferSize > 262144, "Sizing violation: Size of transport buffer must be larger than 262144.");

/*!
 * \brief Buffer size of transport buffer for requests (has to be smaller than response buffer because it does not yet
 * contain the return value).
 * \vprivate Component Private
 */
constexpr std::uint32_t kTransportBufferRequestSize{kTransportBufferSize - 5};

/*!
 * \brief Buffer size of transport buffer for responses.
 * \vprivate Component Private
 */
constexpr std::uint32_t kTransportBufferResponseSize{kTransportBufferSize};

/*!
 * \brief Interface for Transporter.
 * \details High level interface for communication between two processes.
 * \vprivate Component Private
 */
class Transporter {
 public:
  /*!
   * \brief Constructs a transporter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Transporter() = default;

  /*!
   * \brief Destroys a transporter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual ~Transporter() noexcept = default;

  /*!
   * \brief Get the transport buffer for incoming (receive) messages. Note: The buffer is mutable.
   * \return WritableMemRegion pointing to the message buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual WritableMemRegion GetMessageReceiveBuffer() const noexcept { return message_receive_buffer_; }

  /*!
   * \brief Get the transport buffer for outgoing (send) messages. Note: The buffer is mutable.
   * \return WritableMemRegion pointing to the message buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual WritableMemRegion GetMessageSendBuffer() const noexcept { return message_send_buffer_; }

  /*!
   * \brief Send message.
   * \param[in] data Data to be sent.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kUninitialized            Try to send via non-initialized connection.
   * \error osabstraction::OsabErrc::kResource                 Not enough system resources to send data.
   * \error osabstraction::OsabErrc::kDisconnected             The connection has been terminated by peer or
   *                                                           the peer is no longer reachable.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError   Accessing the file system failed.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges   Not enough permissions to access socket.
   * \error osabstraction::OsabErrc::kBusy                     Other send operation is currently in progress.
   * \reentrant FALSE
   * \synchronous FALSE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual amsr::core::Future<void> Send(ReadOnlyMemRegion data) noexcept = 0;

  /*!
   * \brief Send message synchronously.
   * \param[in] data Data to be sent.
   * \context Not on reactor thread.
   * \pre -
   * \threadsafe TRUE
   * \error osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kUninitialized            Try to send via non-initialized connection.
   * \error osabstraction::OsabErrc::kResource                 Not enough system resources to send data.
   * \error osabstraction::OsabErrc::kDisconnected             The connection has been terminated by peer or
   *                                                           the peer is no longer reachable.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError   Accessing the file system failed.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges   Not enough permissions to access socket.
   * \error osabstraction::OsabErrc::kBusy                     Other send operation is currently in progress.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual amsr::core::Result<void> SendSync(ReadOnlyMemRegion data) noexcept = 0;

  /*!
   * \brief Implements the request-response pattern synchronously.
   * \details Only to be used with request-response pattern, i.e. not to be used if any incoming messages can be
   *          expected that are not a response to a previous send request. Otherwise it is not guaranteed that the given
   *          buffer view is not overwritten by a subsequent asynchronously received message.
   * \param[in] data Data to be sent.
   * \return Response data.
   * \context Not on reactor thread.
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrc::kTimeout If the operation takes too long.
   * \error SecurityErrc::kLogicFault If the response is received before the request is sent.
   * \error SecurityErrc::kResourceFault If the connection is closed.
   * \error IpcErrc::kMessageTooLarge                          If the message is too long for the send buffer.
   * \error osabstraction::OsabErrc::kBusy                     If there is already an ongoing receive operation.
   * \error osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kUninitialized            Try to get peer's id via non-initialized connection.
   * \error osabstraction::OsabErrc::kDisconnected             The peer is not connected.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError   The system settings do not support get peer id service.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual amsr::core::Result<ReadOnlyMemRegion> SendAndReceiveSync(ReadOnlyMemRegion data) noexcept = 0;

  /*!
   * \brief Closes the connection.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void Close() noexcept = 0;

  /*!
   * \brief Registers a callback.
   * \param[in] callback The callee to register.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual void RegisterCallback(TransporterEventHandler* callback) noexcept = 0;

  /*!
   * \brief Provides access to a recursive_mutex which can be used to synchronize access to this Transporter.
   * \return Reference to recursive_mutex
   *
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   *
   * \vprivate Component Private
   */
  std::recursive_mutex& GetMutex() noexcept { return mutex_; }

 protected:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  Transporter(Transporter const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  Transporter(Transporter&& /*other*/) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  Transporter& operator=(Transporter const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  Transporter& operator=(Transporter&& /*other*/) & noexcept = delete;

 private:
  std::recursive_mutex mutex_{}; /*!< Mutex that should be used to protect access to send_buffer_ and receive_buffer_*/
  ara::core::Vector<std::uint8_t> receive_buffer_{
      ara::core::Vector<std::uint8_t>(kTransportBufferSize)}; /*!< Internal in data buffer */
  ara::core::Vector<std::uint8_t> send_buffer_{
      ara::core::Vector<std::uint8_t>(kTransportBufferSize)}; /*!< Internal out data buffer */
  WritableMemRegion message_receive_buffer_{receive_buffer_}; /*!< Incoming data to be deserialized */
  WritableMemRegion message_send_buffer_{send_buffer_};       /*!< Outgoing data to be serialized */
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

/*!
 * \exclusivearea sec::ipc::Transporter::mutex_
 * Used to protect access to Transporter::send_buffer_ and Transporter::receive_buffer_.
 * This enables threadsafety when calling remote functions via the RPC framework.
 * \protects sec::ipc::Transporter::send_buffer_, sec::ipc::Transporter::receive_buffer_
 * \usedin crypto::client::BaseProxy::CallRemoteFunction,
 * crypto::client::deletion::ProxyDeletionHandler::HandleDeletion,
 * crypto::client::protocol:::ProtocolHandler::RequestConfiguredCryptoProviderUuid,
 * crypto::client::protocol:::ProtocolHandler::RegisterNewCryptoProviderProxy,
 * crypto::client::protocol:::ProtocolHandler::RegisterNewKeyStorageProviderProxy,
 * crypto::client::protocol:::ProtocolHandler::RegisterNewX509ProviderProxy,
 * \exclude None
 * \length LONG This exclusive area covers calls to several sub-functions.
 * \endexclusivearea
 */

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_H_
