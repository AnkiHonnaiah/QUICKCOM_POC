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
/*!        \file  someip_daemon_client/internal/message_sender.h
 *        \brief  A class responsible for sending messages.
 *        \details  This class sends messages regardless of their types (routing/control), to do that it makes use of
 *                  the passed IPC connection wrapper object.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_SENDER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_SENDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "ara/core/array.h"
#include "ara/core/vector.h"
#include "someip_daemon_client/internal/common_types.h"
#include "someip_daemon_client/internal/config_model.h"
#include "someip_daemon_client/internal/ipc_connection_wrapper.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "someip_daemon_client/internal/types.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

namespace details {
/*!
 * \brief Transmission states, to handle internal transmission.
 */
enum class MessageSenderTransmissionState : std::uint8_t {
  kImmediate, /*!< Transmission was immediately done. */
  kDeferred,  /*!< Transmission was not completed yet. */
  kError      /*!< Transmission error. */
};
}  // namespace details

namespace serialization = ::amsr::someipd_app_protocol::internal;

/*!
 * \brief A message sender class that makes use of the IPC wrapper to provide functionalities for sending all different
 * types of messages between application and SOME/IP daemon.
 * \tparam TemplateConfiguration The type of a class used for message forwarding and event notification.
 */
template <typename TemplateConfiguration>
class MessageSender {
  /*!
   * \brief Type-alias for the const I/O memory buffer.
   */
  using ConstIOBuffer = osabstraction::io::ConstIOBuffer;

  /*!
   * \brief Type-alias for the const I/O memory buffer view.
   */
  using ConstIOBufferContainerView = amsr::core::Span<ConstIOBuffer>;

 public:
  /*!
   * \brief Constructor that initializes all the resources of the message sender.
   * \param[in] config Configuration for SOME/IP daemon client.
   * \param[in] ipc_connection_wrapper Wrapper for BasicIpc operations.
   * \param[in] is_running Pointer to flag indicating if the SomeIpDaemonClient is started.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  MessageSender(SomeIpDaemonClientConfigModel const& config,
                IpcConnectionWrapper<TemplateConfiguration>& ipc_connection_wrapper, bool* is_running) noexcept
      : config_model_{config},
        lock_{},
        transmit_queue_{},
        logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                amsr::core::StringView{"MessageSender"}},
        ipc_connection_wrapper_{ipc_connection_wrapper},
        is_running_{is_running} {
    transmit_queue_.reserve(config.ipc_max_queue_size);  // VCA_SDCL_FUNCTION_NO_PRECONDITION
  }

  /*!
   * \brief Delete default constructor.
   */
  MessageSender() = delete;

  /*!
   * \brief Delete copy constructor.
   */
  MessageSender(MessageSender const&) = delete;

  /*!
   * \brief Delete move constructor.
   */
  MessageSender(MessageSender&&) = delete;

  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(MessageSender const&) & -> MessageSender& = delete;

  /*!
   * \brief Delete move assignment.
   */
  auto operator=(MessageSender&&) & -> MessageSender& = delete;

  /*!
   * \brief Destructor.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  virtual ~MessageSender() noexcept = default;

  /*!
   * \brief Initiates the transmission of a SomeIp/PDU routing or Control message.
   * \details The method may return before the given message has been transmitted.
   *          Outgoing messages might be queued.
   * \param[in] ipc_header A reference to a memory buffer holding the ipc headers.
   * \param[in] packet     A unique pointer to a memory buffer holding the message payload.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected, sending in disconnected state.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   *        queue is full.
   * \pre Start() API of SomeIpDaemonClient is called.
   * \note Only one message is scheduled for sending at a time.
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Initialize send result to kNotConnected.
   * - Enter the critical section.
   * - Check if the connection has been established.
   *   - Enqueues message.
   *   - Update the result.
   * - Otherwise, log an error that the connection is disconnected.
   * - Leave the critical section.
   * - Return the result.
   * \endinternal
   */
  auto SendMessage(vac::memory::allocator::MemoryBuffer const& ipc_header,
                   vac::memory::allocator::MemoryBufferPtr packet) noexcept -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-MessageSender_SendMessage
    CheckIsRunning();

    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(SomeIpDaemonClientErrc::kNotConnected)};
    std::lock_guard<std::mutex> const lock_guard{lock_};
    if (ipc_connection_wrapper_.GetConnectionState() == ConnectionState::kConnected) {
      result = EnqueueMessage(ipc_header, std::move(packet));  // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    } else {
      logger_.LogError([](ara::log::LogStream& stream) { stream << "Trying to send a message in disconnected state"; },
                       {__func__}, {__LINE__});
    }
    // PTP-E-SomeipDaemonClient-MessageSender_SendMessage
    return result;
  }

  /*!
   * \brief Finalize message send operation. Remove the just transmitted message from the queue and start transmission
   *        of the next one.
   * \details Called from BasicIPC OnSendCompletion callback and in case asynchronous message send immediately
   *          completed.
   *
   * \pre Asychronous message transmissions initiated.
   * \context Reactor | App
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Check if any message is pending in the transmit queue
   *   - Remove the first message from the transmit queue (the just transmitted message).
   *   - Check if we are connected.
   *     - Start asynchronous transmission of the next pending message (chaining of send requests / exhaustive mode), if
   * the queue is empty then no message will be sent and transmit_next will be set to false.
   *   - Otherwise log an error.
   * - Otherwise log an error if no message is pending in the transmit queue.
   *
   * - Send messages until the queue is empty.
   * \endinternal
   */
  void CompleteMessageSend() noexcept {
    logger_.LogDebug([](ara::log::LogStream& stream) { stream << "Completing message send"; }, {__func__}, {__LINE__});
    // PTP-B-SomeipDaemonClient-MessageSender_CompleteMessageSend
    bool transmit_next{false};
    {
      std::lock_guard<std::mutex> const lock_guard{lock_};
      if (!transmit_queue_.empty()) {
        static_cast<void>(transmit_queue_.erase(transmit_queue_.begin()));  // VCA_SDCL_TRANSMIT_QUEUE_ERASE
        if (ipc_connection_wrapper_.GetConnectionState() == ConnectionState::kConnected) {
          transmit_next = TransmitNextMessage();
        } else {
          logger_.LogError(
              [](ara::log::LogStream& stream) { stream << "Cannot send next message: Client is disconnected!"; },
              {__func__}, {__LINE__});
        }

      } else {
        logger_.LogError(
            [](ara::log::LogStream& stream) {
              stream << "Call to OnSendCompletion(), but the transmission queue is empty!";
            },
            {__func__}, {__LINE__});
      }
    }

    // Keep sending messages as long as queue is filled and latest message was sent immediately
    while (transmit_next) {
      std::lock_guard<std::mutex> const lock_guard{lock_};
      transmit_next = TransmitNextMessage();
    }
    // PTP-E-SomeipDaemonClient-MessageSender_CompleteMessageSend
  }

 protected:
  /*!
   * \brief The container type for outgoing messages.
   */
  using TransmitQueue = ara::core::Vector<TransmitQueueEntry>;

  /*!
   * \brief     Obtain a reference to the transmission queue.
   * \return    Reference to the transmission queue.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTransmitQueue() -> TransmitQueue& { return transmit_queue_; }

 private:
  /*!
   * \brief Type for the MessageSender transmission states
   */
  using MessageSenderTransmissionState = details::MessageSenderTransmissionState;

  /*!
   * \brief Enqueues a serialized message for transmission.
   * \param[in] ipc_header A reference to a memory buffer holding the ipc headers.
   * \param[in] packet     A unique pointer to a memory buffer holding the message payload.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   *        queue is full.
   * \pre Caller is responsible of protecting access to the message queue (e.g acquiring the lock_)
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if a (non zero) maximum queue size has been set (zero means the queue size is unlimited).
   * - If a maximum size has been set and the buffer exceeds it.
   *   - Log a warning, queue overflow.
   *   - Update result to queue full, kRuntimeResourceOutOf.
   * - Otherwise,
   *   - If the transmit queue is empty, try send the message immediately.
   *     - If the message cannot be sent out immediately, emplace the message to the transmit queue.
   *   - Otherwise, Emplace the message to the transmit queue.
   * - Return result.
   * \endinternal
   */
  auto EnqueueMessage(vac::memory::allocator::MemoryBuffer const& ipc_header,
                      vac::memory::allocator::MemoryBufferPtr packet) noexcept -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-MessageSender_EnqueueMessage
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(SomeIpDaemonClientErrc::kRuntimeResourceOutOf)};
    bool const is_maximum_queue_size_set{config_model_.ipc_max_queue_size > 0U};
    if (is_maximum_queue_size_set && (transmit_queue_.size() >= config_model_.ipc_max_queue_size)) {
      logger_.LogWarn(
          [](ara::log::LogStream& stream) {
            stream << "Overflow occurred while transferring messages to the SOME/IP daemon. Message could "
                      "not be transmitted.";
          },
          {__func__}, {__LINE__});
    } else {
      result.EmplaceValue();
      // Only schedule the next transmission if the transmit queue was empty before
      if (transmit_queue_.empty()) {
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
        MessageSenderTransmissionState const send_result{TransmitOverConnection(ipc_header, *packet)};
        if (send_result != MessageSenderTransmissionState::kImmediate) {
          transmit_queue_.emplace_back(ipc_header, std::move(packet));  // VCA_SDCL_FUNCTION_NO_PRECONDITION
        }
      } else {
        transmit_queue_.emplace_back(ipc_header, std::move(packet));  // VCA_SDCL_FUNCTION_NO_PRECONDITION
      }
    }
    // PTP-E-SomeipDaemonClient-MessageSender_EnqueueMessage
    return result;
  }

  /*!
   * \brief Starts transmission of the first message in the transmit queue.
   * \details Message removed if transmission was done immediately.
   * \return True if conditions for additional transmits are fulfilled, false otherwise.
   * \pre Caller is responsible for protecting access to the transmit_queue_.
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Send if there is at least one message in the queue.
   * - On immediately sent message, delete element in queue.
   * - Return true when connected and immediate send and queue is not empty, otherwise return false.
   * \endinternal
   */
  auto TransmitNextMessage() noexcept -> bool {
    // PTP-B-SomeipDaemonClient-MessageSender_TransmitNextMessage
    bool do_transmit_next{false};

    if (!transmit_queue_.empty()) {
      TransmitQueueEntry const& queue_entry{transmit_queue_.front()};  // VCA_SDCL_VECTOR_FRONT
      MessageSenderTransmissionState const tx_state_result{
          TransmitOverConnection(queue_entry.GetIpcHeader(), queue_entry.GetPacket())};  // VCA_SDCL_PASSING_REFERENCE

      // On immediate sent message, delete element in queue
      if (tx_state_result == MessageSenderTransmissionState::kImmediate) {
        static_cast<void>(transmit_queue_.erase(transmit_queue_.begin()));   // VCA_SDCL_TRANSMIT_QUEUE_ERASE
        do_transmit_next = (ipc_connection_wrapper_.GetConnectionState() ==  // COV_SOMEIPDAEMON_CLIENT_FALSE_POSITIVE
                            ConnectionState::kConnected) &&
                           (!transmit_queue_.empty());
      }
    } else {
      logger_.LogVerbose([](ara::log::LogStream& stream) { stream << "Transmit queue empty"; }, {__func__}, {__LINE__});
      do_transmit_next = false;
    }
    // PTP-E-SomeipDaemonClient-MessageSender_TransmitNextMessage
    return do_transmit_next;
  }

  /*!
   * \brief Transmit internal filled IOVec over active connection.
   * \param[in] ipc_header A reference to a memory buffer holding the ipc headers.
   * \param[in] packet     A reference to a memory buffer holding the message payload.
   * \return kImmediate if transmission was done immediately, kDeferred if transmission is deferred of underlying
   *         layer or otherwise kError.
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Create the I/O vector container view.
   * - Send the container view asynchronously.
   * - Check if the asynchronous transmission immediately completed.
   * - Check if the transmission failed.
   *   - Log a sending error.
   * \endinternal
   */
  auto TransmitOverConnection(vac::memory::allocator::MemoryBuffer const& ipc_header,
                              vac::memory::allocator::MemoryBuffer const& packet) noexcept
      -> MessageSenderTransmissionState {
    MessageSenderTransmissionState result{MessageSenderTransmissionState::kError};

    // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    ConstIOBuffer const ipc_header_buffer{osabstraction::io::ConvertSpanToConstIOBuffer(ipc_header.GetView(0U))};
    io_container_[0] = ipc_header_buffer;

    // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    ConstIOBuffer const payload_buffer{osabstraction::io::ConvertSpanToConstIOBuffer(packet.GetView(0U))};
    io_container_[1] = payload_buffer;

    ConstIOBufferContainerView const view{io_container_.data(), io_container_.size()};
    // Trigger asynchronous transmission via BasicIpc connection
    amsr::core::Result<amsr::ipc::SendResult> send_result{ipc_connection_wrapper_.Send(view)};

    if (send_result.HasValue()) {
      // Handle immediate message send
      if (send_result.Value() == amsr::ipc::SendResult::kSendCompleted) {
        logger_.LogVerbose([](ara::log::LogStream& stream) { stream << "Completing immediate message send."; },
                           {__func__}, {__LINE__});
        result = MessageSenderTransmissionState::kImmediate;
      } else {
        logger_.LogDebug([](ara::log::LogStream& stream) { stream << "Message sending was deferred."; }, {__func__},
                         {__LINE__});
        result = MessageSenderTransmissionState::kDeferred;
      }
    } else {
      logger_.LogError(
          [&send_result](ara::log::LogStream& stream) {
            // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
            ::amsr::core::StringView const err_str{send_result.Error().Message()};
            stream << "Asynchronous message send request failed with error: " << err_str;
          },
          {__func__}, {__LINE__});
    }
    return result;
  }

  /*!
   * \brief Check that we are in running state. A detected mismatch will be handled as violation (abort).
   * \details Ensures that Start() API has been called before any communication request is triggered.
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   *
   * \internal
   * - If client is not in state 'running'
   *   - log an error and abort further execution (violation).
   * \endinternal
   */
  void CheckIsRunning() const noexcept {
    if (!*is_running_) {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "SomeIpDaemonClient must be started before triggering any communication request.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Reference to the config model of the SomeIpDaemonClient.
   */
  SomeIpDaemonClientConfigModel const& config_model_;

  /*!
   * \brief This lock provides mutual exclusion for the whole client.
   */
  std::mutex lock_;

  /*!
   * \brief Contains all messages pending for transmission.
   */
  TransmitQueue transmit_queue_;

  /*!
   * \brief An IO container used to build up the ipc packet from the memory buffers holding the ipc headers and the
   *        message payload.
   */
  ara::core::Array<ConstIOBuffer, 2> io_container_{};

  /*!
   * \brief Our logger.
   */
  logging::AraComLogger logger_;

  /*!
   * \brief Wrapper for IPC communication with SOME/IP daemon.
   */
  IpcConnectionWrapper<TemplateConfiguration>& ipc_connection_wrapper_;

  /*!
   * \brief A pointer flag indicating if the SomeIpDaemonClient is started.
   */
  bool* is_running_;

  /*!
   * \brief Friend test to access the method TransmitNextMessage() and queue size.
   */
  FRIEND_TEST(UT__IpcConnection__MessageSender,
              DISABLED_CompleteMessageSend__CheckMutexAndInvokeTransmitNextWhenQueueEmpty);
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOMEIPDAEMON_CLIENT_FALSE_POSITIVE
//   \ACCEPT TF tx tf
//   \REASON CompleteMessageSend() validates the connection state as kConnected before calling TransmitNextMessage
//           API.Therefore, in this scenario, the GetConnectionState() false condition won't occur.
//
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_SENDER_H_

/*!
 * \exclusivearea amsr::someip_daemon_client::internal::MessageSender::lock_
 *                Used for protecting the IPC connection transmit queue.
 *
 * \protects amsr::someip_daemon_client::internal::MessageSender::transmit_queue_
 *
 * \usedin  amsr::someip_daemon_client::internal::MessageSender::SendMessage
 *          amsr::someip_daemon_client::internal::MessageSender::CompleteMessageSend
 * \exclude User threads from accessing a variable that is already in access.
 * \length LONG IPC communication.
 * \endexclusivearea
 */
