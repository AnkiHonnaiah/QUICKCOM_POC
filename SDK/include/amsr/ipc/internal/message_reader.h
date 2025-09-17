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
 *        \brief  MessageReader implementation that manages reads from a StreamIpcChannel.
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_QNX
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_READER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/internal/channel/stream_ipc_channel.h"
#include "osabstraction/io/internal/io_buffer_queue.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief    MessageReader implementation that manages reads from a StreamIpcChannel.
 *
 * \details  Supports both stream-based and datagram-based reads. Manages buffers for stream-based reads.
 */
class MessageReader final {
 public:
  /*!
   * \brief           Creates a MessageReader that reads from a StreamIpcChannel.
   *
   * \param[in]       ipc_channel   StreamIpcChannel to read from.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit MessageReader(StreamIpcChannel& ipc_channel) noexcept : ipc_channel_{ipc_channel} {}

  /*!
   * \brief     Callback called by the MessageReader during a stream read to get the receive buffers for the IPC message
   *            that is about to be received.
   *
   * \param[in] message_size  The size of the incoming message
   *
   * \return    View on the receive buffers or an empty optional if the connection was closed during the callback and
   *            the stream read should not be continued.
   *
   * \context         StreamReadMessage()
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  using ProvideReceiveBuffersCallback =
      vac::language::FunctionRef<::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>>(
          std::uint32_t message_size)>;

  /*!
   * \brief     Callback called by the MessageReader during a chunkwise stream read to get the receive buffers for
   *            the next chunk of the IPC message which is being received.
   *
   * \details   Last message chunk received callback of each message is not called by MessageReader, because there
   *            are no real buffers to be provided by the user in that case
   *            (only empty buffers view is expected to be returned). ConnectionImpl will call the last message chunk
   *            received callback of each message.
   *
   * \param[in] remaining_message_size  The remaining size of the incoming message
   *
   * \return    View on the receive buffers or an empty optional if the connection was closed during the callback and
   *            the stream read should not be continued.
   *
   * \context         StreamReadMessage()
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  using ProvideNextChunkReceiveBuffersCallback =
      vac::language::FunctionRef<::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>>(
          std::uint32_t remaining_message_size)>;

  /*!
   * \brief           Prepares a stream-based read.
   *
   * \details         Has to be called before StreamReadMessage() can be called for the next IPC message.
   *
   * \param[out]      common_header                               Location where the received common header shall be
   *                                                              saved. Must stay valid until the stream read finished.
   * \param[out]      format_b_header                             Location where the possibly received format B header
   *                                                              shall be saved. A format B header will only be written
   *                                                              if the message format in common_header is format B.
   *                                                              Must stay valid until the stream read finished.
   * \param[in]       provide_receive_buffers_callback            Reference to a callable that will be invoked by the
   *                                                              MessageReader after the common header was received to
   *                                                              get the receive buffers for the IPC message data.
   *                                                              The referenced callable must stay valid
   *                                                              until the stream read of the IPC message finished.
   * \param[in]       provide_next_chunk_receive_buffers_callback Reference to a callable that will be invoked by the
   *                                                              MessageReader after the requested message chunk was
   *                                                              received to get the receive buffers for the next chunk
   *                                                              of the same message.
   *                                                              The referenced callable must stay valid
   *                                                              until the stream read of the IPC message finished.
   * \param[in]      chunkwise_receive                            true if chunkwise receive has been requested, false
   *                                                              otherwise.
   *
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Save the provided parameters in the object.
   *  - Set the internal state to reading common header.
   *  - Set the internal state to passed read mode.
   * \endinternal
   */
  void PrepareStreamRead(stream_ipc_protocol::CommonHeader* common_header,
                         stream_ipc_protocol::FormatBHeader* format_b_header,
                         ProvideReceiveBuffersCallback provide_receive_buffers_callback,
                         ProvideNextChunkReceiveBuffersCallback provide_next_chunk_receive_buffers_callback,
                         bool chunkwise_receive) noexcept {
    common_header_ = common_header;
    format_b_header_ = format_b_header;
    provide_receive_buffers_callback_.emplace(provide_receive_buffers_callback);
    provide_next_chunk_receive_buffers_callback_.emplace(  // VCA_OSA_MESSAGEREADER_STORES_FUNCTION_REF
        provide_next_chunk_receive_buffers_callback);
    stream_reader_state_ = StreamReaderState::kReadingCommonHeader;
    stream_read_data_mode_ = chunkwise_receive ? StreamReadDataMode::kChunkWise : StreamReadDataMode::kWholeData;
  }

  /*!
   * \brief Result of the StartAsyncStreamRead() operation.
   */
  enum class StartAsyncStreamReadResult : std::uint8_t {
    /*!
     * \brief No common IPC message header is available in the StreamIpcChannel.
     */
    kNoMessageAvailable,
    /*!
     * \brief A common IPC message header is already available in the StreamIpcChannel.
     */
    kMessageAlreadyAvailable
  };

  /*!
   * \brief           Starts an asynchronous stream-based read.
   *
   * \details         Checks if a common IPC header is already available in the StreamIpcChannel to be received and
   *                  requests readable notification from the peer process if this is not the case.
   *
   * \return          Information if data is already available to be received. Even if data is already available to be
   *                  received it is possible that the peer sends a readable notification. This spurious notification
   *                  should be ignored.
   *
   * \error           osabstraction::OsabErrc::kProtocolError    A SafeIPC protocol violation occurred and APIs of this
   *                                                             object should not be called anymore as the
   *                                                             StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StartAsyncStreamRead() noexcept -> ::amsr::core::Result<StartAsyncStreamReadResult>;

  /*!
   * \brief Status of a stream read operation.
   */
  enum class StreamReadStatus : std::uint8_t {
    /*!
     * \brief Stream read operation is not finished and no more data is available in the StreamIpcChannel to be
     *        received.
     */
    kReadOngoingAndNoBytesAvailable,
    /*!
     * \brief Stream read operation is not finished but more data is available in the StreamIpcChannel to be received.
     */
    kReadOngoingAndMoreBytesAvailable,
    /*!
     * \brief Stream read operation finished but the received message is truncated because the provided receive buffers
     *        were not large enough for the whole message.
     */
    kReadCompletedWithTruncation,
    /*!
     * \brief Stream read operation finished and the received message is not truncated.
     */
    kReadCompletedWithoutTruncation
  };

  /*!
   * \brief Result of StreamReadMessage() call.
   */
  struct StreamReadResult {
    /*!
     * \brief Status of the stream read operation.
     */
    StreamReadStatus status;
    /*!
     * \brief True if a writable notification should be sent to the peer, false otherwise.
     */
    bool writable_notification_needed;
  };

  /*!
   * \brief           Reads (part of) an IPC message.
   *
   * \details         Reads an IPC message from the StreamIpcChannel with a stream-based read. PrepareStreamRead()
   *                  has to be called before a stream-based read of an IPC message is started. Requests readable
   *                  notification from the peer process if the stream-based read did not complete because the whole
   *                  data of the IPC message was not available yet in the receive buffer.
   *
   * \return          Status of the stream read operation and information if a writable notification should be sent to
   *                  the IPC peer.
   *
   * \error           osabstraction::OsabErrc::kUninitialized    No receive buffers were provided in the
   *                                                             ProvideReceiveBuffersCallback or in the
   *                                                             ProvideNextChunkReceiveBuffersCallback
   *                                                             for the IPC message.
   *                                                             MessageReader state is reset.
   * \error           osabstraction::OsabErrc::kProtocolError    A SafeIPC protocol violation occurred and APIs of this
   *                                                             object should not be called anymore as the
   *                                                             StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamReadMessage() noexcept -> ::amsr::core::Result<StreamReadResult>;

  /*!
   * \brief           Peeks at the next common IPC header that will be received from the StreamIpcChannel to check the
   *                  size of the next IPC message.
   *
   * \details         This can be used before DatagramReadMessage() to determine the size of the IPC message that will
   *                  be received next so that an appropriately sized receive buffer can be provided.
   *
   * \param[out]      common_header   Location where the received common header shall be saved. Must stay valid during
   *                                  this function call.
   *
   * \error           osabstraction::OsabErrc::kBusy            Not IPC header available to be received in the
   *                                                            StreamIpcChannel.
   * \error           osabstraction::OsabErrc::kProtocolError   A SafeIPC protocol violation occurred and APIs of this
   *                                                            object should not be called anymore as the
   *                                                            StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             No stream read is currently ongoing
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto PeekCommonMessageHeader(stream_ipc_protocol::CommonHeader* common_header) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Result of DatagramReadMessage() call.
   */
  struct DatagramReadResult {
    /*!
     * \brief True if the received message was truncated because the provided receive buffer was not large enough for
     *        the received IPC message, false otherwise.
     */
    bool truncated;
    /*!
     * \brief True if a writable notification should be sent to the peer, false otherwise.
     */
    bool writable_notification_needed;
  };

  /*!
   * \brief           Reads a whole IPC message (headers + data).
   *
   * \details         Reads an IPC message from the StreamIpcChannel if a whole IPC message is available. Does not read
   *                  anything from the StreamIpcChannel if only a partial IPC message is available. Never requests
   *                  readable notification from the peer process.
   *
   * \param[out]      common_header   Location where the received common header shall be saved. Must stay valid during
   *                                  this function call.
   * \param[out]      format_b_header Location where the possibly received format B header shall be saved. A format B
   *                                  header will only be written if the message format in common_header is format B.
   *                                  Must stay valid during this function call.
   * \param[out]      io_buffers      Buffers where the received IPC message shall be copied to. The provided buffers
   *                                  have to stay valid during this function call.
   *
   * \return          Flags that indicate if the received message was truncated and if a writable notification should be
   *                  send to the IPC peer.
   *
   * \error           osabstraction::OsabErrc::kBusy            No whole IPC message is available in the
   *                                                            StreamIpcChannel yet.
   * \error           osabstraction::OsabErrc::kSize            Size of next IPC message is too large to be received as
   *                                                            a whole (IPC headers + IPC data exceeds the receive ring
   *                                                            buffer size).
   * \error           osabstraction::OsabErrc::kTruncation      Received IPC message was truncated as the provided
   *                                                            buffer was too small.
   * \error           osabstraction::OsabErrc::kProtocolError   A SafeIPC protocol violation occurred and APIs of this
   *                                                            object should not be called anymore as the
   *                                                            StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             No stream read is currently ongoing
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto DatagramReadMessage(stream_ipc_protocol::CommonHeader* common_header,
                           stream_ipc_protocol::FormatBHeader* format_b_header,
                           ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers) noexcept
      -> ::amsr::core::Result<DatagramReadResult>;

 private:
  /*!
   * \brief           Finish read operations and check if a writable notification has to be sent to the peer.
   *
   * \details         Makes all reads that were performed visible to the peer process (otherwise the peer will not see
   *                  them). This function takes into account that a writable notification shall only be sent if a
   *                  significant amount of free space is available in the receive buffer (this amount is never less
   *                  than the size of a common IPC header or a format B IPC header).
   *
   * \return          True if a writable notification should be sent to the peer process, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto CommitRead() noexcept -> bool;

  /*!
   * \brief           Reads common header of IPC message.
   *
   * \details         Sets the remaining_message_size_ and calls the ProvideReceiveBuffersCallback if a header was
   *                  received. stream_reader_state_ may be changed by this function.
   *
   * \return          true of any data was read from the StreamIpcChannel, false otherwise.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   No receive buffers were provided in the
   *                                                            ProvideReceiveBuffersCallback for the IPC message.
   *                                                            MessageReader state is reset.
   * \error           osabstraction::OsabErrc::kProtocolError   The received common B header violates the SafeIPC
   *                                                            protocol and APIs of this object should not be called
   *                                                            anymore as the StreamIpcChannel is in an undefined
   *                                                            state.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kReadingCommonHeader
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamReadCommonHeader() noexcept -> ::amsr::core::Result<bool>;

  /*!
   * \brief           Reads format B header of IPC message.
   *
   * \details         stream_reader_state_ may be changed by this function.
   *
   * \return          true of any data was read from the StreamIpcChannel, false otherwise.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   The received format B header violates the SafeIPC
   *                                                            protocol and APIs of this object should not be called
   *                                                            anymore as the StreamIpcChannel is in an undefined
   *                                                            state.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kReadingFormatBHeader
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamReadFormatBHeader() noexcept -> ::amsr::core::Result<bool>;

  /*!
   * \brief           Reads IPC data of IPC message.
   *
   * \details         stream_reader_state_ may be changed by this function.
   *
   * \return          true of any data was read from the StreamIpcChannel, false otherwise.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   No receive buffers were provided in the
   *                                                            ProvideNextChunkReceiveBuffersCallback
   *                                                            for the IPC message.
   *                                                            MessageReader state is reset.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kReadingData
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamReadData() noexcept -> ::amsr::core::Result<bool>;

  /*!
   * \brief           Discards IPC data of IPC message.
   *
   * \details         stream_reader_state_ may be changed by this function.
   *
   * \return          true of any data was discarded from the StreamIpcChannel, false otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kDiscardingData
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamDiscardData() noexcept -> bool;

  /*!
   * \brief           Checks if data is available in the IPC channel for the currently read part of the IPC message
   *                  (common header / format B header / IPC data) during a stream read.
   *
   * \return          true if data is available for the currently read part of the IPC message, false otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ != StreamReaderState::kReadFinished
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamReadCanContinue() const noexcept -> bool;

  /*!
   * \brief           Handle read bytes as part of a whole IPC message.
   *
   * \details         Updates internal state according to remaining_message_size_ and
   *                  provided truncation parameter.
   *
   * \param[in]       truncation         true if received bytes do not fit in the provided buffers,
   *                                     false otherwise.
   *
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kReadingData
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleReadDataAsWhole(bool truncation) noexcept;

  /*!
   * \brief           Handle read bytes as part of an IPC message chunk.
   *
   * \details         Updates internal state, calls the ProvideNextChunkReceiveBuffersCallback
   *                  if a message chunk has been completely received.
   *
   * \param[in]       truncation         true if received bytes do not fit in the last provided buffers,
   *                                     false otherwise.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   No receive buffers were provided in the
   *                                                            ProvideNextChunkReceiveBuffersCallback
   *                                                            for the IPC message.
   *                                                            MessageReader state is reset.
   *
   * \context         ANY
   *
   * \pre             stream_reader_state_ == StreamReaderState::kReadingData
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HandleReadDataAsChunk(bool truncation) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief    StreamIpcChannel that the MessageReader reads from.
   */
  StreamIpcChannel& ipc_channel_;

  /*!
   * \brief    States of the stream reader.
   */
  enum class StreamReaderState : std::uint32_t {
    /*!
     * \brief    The stream reader is currently reading an IPC common header.
     */
    kReadingCommonHeader,
    /*!
     * \brief    The stream reader is currently reading a format B header.
     */
    kReadingFormatBHeader,
    /*!
     * \brief    The stream reader is currently reading IPC data.
     */
    kReadingData,
    /*!
     * \brief    The stream reader is currently discarding IPC data.
     */
    kDiscardingData,
    /*!
     * \brief    The stream reader finished a stream-based read.
     */
    kReadFinished
  };

  /*!
   * \brief    State of the internal stream reader.
   */
  StreamReaderState stream_reader_state_{StreamReaderState::kReadingCommonHeader};

  /*!
   * \brief    Mode of stream based data read.
   */
  enum class StreamReadDataMode : std::uint32_t {
    /*!
     * \brief    The stream reader reads the whole message payload in one buffer.
     */
    kWholeData,
    /*!
     * \brief    The stream reader reads the message payload in multiple steps, using different buffers in each step.
     */
    kChunkWise
  };

  /*!
   * \brief    Stream data processing mode.
   */
  StreamReadDataMode stream_read_data_mode_{StreamReadDataMode::kWholeData};

  /*!
   * \brief    IO buffer queue that manages the buffers that the user provided for a stream read operation.
   */
  osabstraction::io::internal::IoBufferQueue<osabstraction::io::MutableIOBuffer> stream_read_io_buffer_queue_{};

  /*!
   * \brief    Number of bytes of the IPC message that is currently being stream read that are yet to be read.
   * \details  Has no meaning for datagram-based reads.
   */
  std::uint32_t remaining_message_size_{};

  /*!
   * \brief    Location where a received common header should be saved during a stream-based read.
   * \details  Has no meaning for datagram-based reads.
   */
  stream_ipc_protocol::CommonHeader* common_header_{nullptr};

  /*!
   * \brief    Location where a received format B header should be saved during a stream-based read.
   * \details  Has no meaning for datagram-based reads.
   */
  stream_ipc_protocol::FormatBHeader* format_b_header_{nullptr};

  /*!
   * \brief    Callback that should be called to get the receive buffers during a stream-based read.
   * \details  Has no meaning for datagram-based reads.
   */
  ::amsr::core::Optional<ProvideReceiveBuffersCallback> provide_receive_buffers_callback_{};

  /*!
   * \brief    Callback that should be called to get the receive buffers during a stream-based chunkwise read.
   * \details  Has no meaning for datagram-based reads and for stream-based non chunkwise read.
   */
  ::amsr::core::Optional<ProvideNextChunkReceiveBuffersCallback> provide_next_chunk_receive_buffers_callback_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_READER_H_
