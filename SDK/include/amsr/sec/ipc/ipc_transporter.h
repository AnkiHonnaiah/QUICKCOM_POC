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
/*!        \file  ipc_transporter.h
 *        \brief  Class enabling IPC.
 *
 *      \details  Transporter implementing Basic IPC Callbacks.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <future>
#include <limits>
#include <memory>

#include "amsr/core/promise.h"
#include "amsr/pool_resource.h"
#include "amsr/sec/ipc/connection_wrapper.h"
#include "amsr/sec/ipc/transporter.h"
#include "amsr/sec/ipc/transporter_event_handler.h"
#include "amsr/sec/ipc/types.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace sec {
namespace ipc {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Implementation of IPC based Transporter.
 * \details Implementation of IPC based Transporter. This transporter uses basic IPC for communication
 * and is designed to simulate an API like network sockets. This is e.g. required for server client use
 * where the waiting thread shall block until new data is available.
 * \vprivate Component Private
 */
class IpcTransporter : public Transporter {
 public:
  /*!
   * \brief Constructs an IPC transporter.
   * \param[in] connection The wrapped IPC connection.
   * \return The created transporter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrc::kResourceFault If the connection is closed.
   * \error SecurityErrc::kRuntimeFault If the receive listeners cannot be bound.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<std::unique_ptr<IpcTransporter>> Create(
      std::unique_ptr<ConnectionWrapper> connection) noexcept;

  /*!
   * \brief Cleans up the resources.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~IpcTransporter() noexcept override;

  // The IPC connection should only be accessed by one instance.
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  IpcTransporter(IpcTransporter const& /*other*/) noexcept = delete;

  // Not required.
  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  IpcTransporter(IpcTransporter&& /*other*/) noexcept = delete;

  // The IPC connection should only be accessed by one instance.
  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcTransporter& operator=(IpcTransporter const& /*other*/) noexcept = delete;

  // Not required.
  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcTransporter& operator=(IpcTransporter&& /*other*/) noexcept = delete;

  /*!
   * \copydoc sec::ipc::Transporter::Send
   */
  amsr::core::Future<void> Send(ReadOnlyMemRegion data) noexcept override;

  /*!
   * \copydoc sec::ipc::Transporter::SendSync
   */
  amsr::core::Result<void> SendSync(ReadOnlyMemRegion data) noexcept override;

  /*!
   * \copydoc sec::ipc::Transporter::SendAndReceiveSync
   */
  amsr::core::Result<ReadOnlyMemRegion> SendAndReceiveSync(ReadOnlyMemRegion data) noexcept override;

  /*!
   * \copydoc sec::ipc::Transporter::Close
   */
  void Close() noexcept override;

  /*!
   * \copydoc sec::ipc::Transporter::RegisterCallback
   */
  void RegisterCallback(TransporterEventHandler* callback) noexcept override;

 protected:
  /*!
   * \brief Constructs a transporter.
   * \param[in] connection The wrapped IPC connection.
   * \context ANY
   * \pre \c connection should not be a nullptr.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit IpcTransporter(std::unique_ptr<ConnectionWrapper> connection) noexcept;

  /*!
   * \brief Calls callback whenever a disconnect occurs.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnDisconnect() noexcept;

  /*!
   * \brief Calls callback whenever a message is sent successfully.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnSent() noexcept;

  /*!
   * \brief Calls callback whenever a message is received successfully.
   * \param[in] data Received data.
   * \param[in] user_id Sender user ID.
   * \param[in] process_id Sender process ID.
   * \context Reactor thread.
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnReceived(ReadOnlyMemRegion data, amsr::ipc::Credentials user_id,
                  osabstraction::process::ProcessId process_id) noexcept;

  /*!
   * \brief Initiates listening to incoming messages.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error osabstraction::OsabErrc::kUninitialized            Try to receive from non-initialized connection.
   * \error osabstraction::OsabErrc::kBusy                     Other read operation is currently in progress.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> BindReceive() noexcept;

  /*!
   * \brief Internals of the sending operation.
   * \param[in] data Data to be sent.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kResourceFault If the connection is closed.
   * \error SecurityErrc::kInvalidInputSize If the message is too long for the send buffer.
   * \error SecurityErrc::kRuntimeFault If sending the message fails.
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  amsr::core::Future<void> SendImpl(ReadOnlyMemRegion data) noexcept;

  /*!
   * \brief Provides future for waiting on the reception of the next message.
   * \context ANY
   * \threadsafe TRUE
   * \error osabstraction::OsabErrc::kBusy If there is already an ongoing receive operation.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Future<ReadOnlyMemRegion> ReceiveNext() noexcept;

  /*!
   * \brief Finalizes a message retrieval.
   * \param[in] msg_size Size of the received message.
   * \context Reactor thread.
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReceiveComplete(std::size_t msg_size) noexcept;

  /*!
   * \brief Callback to be called when a send operation has completed.
   * \param[in] send_async_result Result of the send operation, returned by the osabstraction.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SendCompletionCallback(amsr::core::Result<void> const&& send_async_result);

 private:
  std::mutex close_mutex_;              /*!< Used to protect the closing operation. */
  std::recursive_mutex callback_mutex_; /*!< Used to protect the callback. */
  std::mutex send_recv_mutex_; /*!< Used to ensure that no receive operation is performed while still sending. */
  std::unique_ptr<ConnectionWrapper> connection_; /*!< Wrapped BasicIPC connection */
  std::atomic<bool> send_in_progress_;            /*!< Indicates if currently a send operation is ongoing. */
  std::atomic<bool> recv_future_pending_;         /*!< Indicates if currently a receive operation is ongoing. */
  std::atomic<bool> destructing_;                 /*!< Indicates if the destructor has been called. */

  /*! \brief Number of concurrent promises to be allocated by the SharedStateMemoryResources */
  static constexpr std::size_t const kNumConcurrentPromises{2};
  amsr::PoolResource send_promise_memory_pool_;    /*!< Memory pool for send promises */
  amsr::PoolResource receive_promise_memory_pool_; /*!< Memory pool for receive promises */
  ara::core::SharedStateMemoryResource<void, kNumConcurrentPromises>
      send_promise_resource_; /*!< Memory resource for send promise */
  ara::core::SharedStateMemoryResource<ReadOnlyMemRegion, kNumConcurrentPromises>
      receive_promise_resource_;           /*!< Memory resource for receive promise */
  amsr::core::Promise<void> send_promise_; /*!< Promise to be fulfilled on completion of the current send operation. */
  amsr::core::Promise<ReadOnlyMemRegion>
      receive_promise_; /*!< Promise to be fulfilled on completion of the current receive operation. */

  osabstraction::io::ConstIOBuffer send_buffer_; /*!< Buffer for storing the message to be sent. */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  std::size_t send_buffer_size_; /*!< Number of currently used bytes in the send buffer. */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  osabstraction::io::MutableIOBuffer receive_buffer_; /*!< Buffer for storing the next part of a received message. */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  TransporterEventHandler* callback_; /*!< Callee for callbacks */

  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, BindReceive__Uninitialized);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, BindReceive__FailedReceiveAsync);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, ReceiveNext__Busy);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, SendAndReceiveSync);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, ReceiveComplete__Error);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, ReceiveComplete__InconsistentMsgLength);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, Send__NoLock);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, ExpectReceivingState__Fail);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, SetReceivingState__Fail);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, SendSync__NoLock);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, BindReceive__NotReceiving);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, BindReceive__RebindError__ConnectionClosed);
  /*! Friend test declaration */
  FRIEND_TEST(UT__ipc__IpcTransporter, BindReceive__RebindError__ReceiveAsyncFail);
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_H_

/*!
 * \exclusivearea sec::ipc::IpcTransporter::close_mutex_
 * Used for protecting the closing operation. Ensures that only one
 * closing operation is executed at a time.
 *
 * \protects sec::ipc::IpcTransporter::connection_::Close
 * \usedin sec::ipc::IpcTransporter::Close
 * \exclude sec::ipc::IpcTransporter::Close
 * \length LONG Held until the connection is closed.
 * \endexclusivearea
 */

/*!
 * \exclusivearea sec::ipc::IpcTransporter::callback_mutex_
 * Used for protecting the callback handler. Ensures that the handler is not exchanged while a callback is executed.
 *
 * \protects sec::ipc::IpcTransporter::callback_
 * \usedin sec::ipc::IpcTransporter::OnDisconnect,
 * sec::ipc::IpcTransporter::OnReceived,
 * sec::ipc::IpcTransporter::OnSent,
 * sec::ipc::IpcTransporter::RegisterCallback,
 * \exclude sec::ipc::IpcTransporter::OnDisconnect,
 * sec::ipc::IpcTransporter::OnReceived,
 * sec::ipc::IpcTransporter::OnSent,
 * sec::ipc::IpcTransporter::RegisterCallback
 * \length LONG Held until the callback is executed.
 * \endexclusivearea
 */
