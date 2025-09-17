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
 *        \brief  MessageWriter implementation that manages writes to a StreamIpcChannel.
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_QNX
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_WRITER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/internal/channel/stream_ipc_channel.h"
#include "osabstraction/io/internal/io_buffer_queue.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief    MessageWriter implementation that manages writes to a StreamIpcChannel.
 *
 * \details  Supports both stream-based and datagram-based writes. Manages buffers for stream-based writes.
 */
class MessageWriter final {
 public:
  /*!
   * \brief           Creates a MessageWriter that writes to a StreamIpcChannel.
   *
   * \param[in]       ipc_channel   StreamIpcChannel to writer to.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit MessageWriter(StreamIpcChannel& ipc_channel) noexcept : ipc_channel_{ipc_channel} {}

  /*!
   * \brief           Prepares a stream-based write.
   *
   * \details         Has to be called before StreamWriteMessage() can be called for the next IPC message.
   *
   * \param[in,out]   common_header      Common header of the IPC message that shall be written. The message size,
   *                                     message size check value and sequence number fields of this header do not
   *                                     need to be filled out, this will be done by this function. Must stay valid
   *                                     until the stream write finished.
   * \param[in]       format_b_header    Format B header of the IPC message that shall be written. This pointer will
   *                                     only by evaluated if the passed common header indicates that the message format
   *                                     of the IPC message is format B. This pointer may be nullptr if the message
   *                                     format is format A. Otherwise it must stay valid until the stream write
   *                                     finished.
   * \param[in]       io_buffers         Buffers that point to the IPC data that shall be sent. The provided buffers
   *                                     have to stay valid during this function call.
   *
   * \error           osabstraction::OsabErrc::kSize            The message exceeds the maximum supported message size.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto PrepareStreamWrite(stream_ipc_protocol::CommonHeader* common_header,
                          stream_ipc_protocol::FormatBHeader const* format_b_header,
                          ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Result of the StartAsyncStreamWrite() operation.
   */
  enum class StartAsyncStreamWriteResult : std::uint8_t {
    /*!
     * \brief No free space for a common IPC message header is available in the StreamIpcChannel.
     */
    kNoFreeSpaceAvailable,
    /*!
     * \brief Free space for a common IPC message header is available in the StreamIpcChannel.
     */
    kFreeSpaceAlreadyAvailable
  };

  /*!
   * \brief           Starts an asynchronous stream-based write.
   *
   * \details         Checks if free space is already available in the StreamIpcChannel for at least the common IPC
   *                  header and requests writable notification from the peer process if this is not the case.
   *
   * \return          Information if free space for at least the IPC header is available. Even if free space is already
   *                  available it is possible that the peer sends a writable notification. This spurious notification
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
  auto StartAsyncStreamWrite() noexcept -> ::amsr::core::Result<StartAsyncStreamWriteResult>;

  /*!
   * \brief Status of a stream write operation.
   */
  enum class StreamWriteStatus : std::uint8_t {
    /*!
     * \brief Stream write operation is not finished and no more free space is available in the StreamIpcChannel.
     */
    kWriteOngoingAndNoFreeSpaceAvailable,
    /*!
     * \brief Stream write operation is not finished but more free space is available in the StreamIpcChannel.
     */
    kWriteOngoingAndMoreFreeSpaceAvailable,
    /*!
     * \brief Stream write operation finished.
     */
    kWriteCompleted
  };

  /*!
   * \brief Result of StreamWriteMessage() call.
   */
  struct StreamWriteResult {
    /*!
     * \brief Status of the stream write operation.
     */
    StreamWriteStatus status;
    /*!
     * \brief True if a readable notification should be sent to the peer, false otherwise.
     */
    bool readable_notification_needed;
  };

  /*!
   * \brief           Writes (part of) an IPC message.
   *
   * \details         Writes an IPC message to the StreamIpcChannel with a stream-based write. PrepareStreamWrite()
   *                  has to be called before a stream-based write of an IPC message is started. Requests writable
   *                  notification from the peer process if the stream-based write did not complete because the free
   *                  space in the send buffer was insufficient for the whole data of the IPC message.
   *
   * \return          Status of the stream write operation and information if a readable notification should be sent to
   *                  the IPC peer.
   *
   * \error           osabstraction::OsabErrc::kProtocolError    A SafeIPC protocol violation occurred and APIs of this
   *                                                             object should not be called anymore as the
   *                                                             StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             The stream write state has to be StreamWriteState::kWritingHeader (see GetStreamWriteState()).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamWriteMessage() noexcept -> ::amsr::core::Result<StreamWriteResult>;

  /*!
   * \brief Result of DatagramWriteMessage() call.
   */
  struct DatagramWriteResult {
    /*!
     * \brief True if a readable notification should be sent to the peer, false otherwise.
     */
    bool readable_notification_needed;
  };

  /*!
   * \brief           Writes a whole IPC message (headers + data).
   *
   * \details         Writes an IPC message to the StreamIpcChannel if space for the whole IPC message is available in
   *                  the StreamIpcChannel. Does not write anything to the StreamIpcChannel if only space for a part of
   *                  the IPC message is available. Never requests writable notification from the peer process.
   *
   * \param[in,out]   common_header   Common header to send. The message size, message size check value and sequence
   *                                  number fields of this header do not need to be filled out, this will be done by
   *                                  this function. Must stay valid during this function call.
   * \param[in]       format_b_header Format B header to send. This pointer will only by evaluated if the passed common
   *                                  header indicates that the message format of the IPC message is format B. This
   *                                  pointer may be nullptr if the message format is format A.
   * \param[in]       io_buffers      Array of buffers that contain the data of the IPC message that shall be sent.
   *
   * \return          Flag that indicates if a readable notification should be sent to the IPC peer.
   *
   * \error           osabstraction::OsabErrc::kBusy            Not enough space for the whole IPC message is available
   *                                                            in the StreamIpcChannel at the moment.
   * \error           osabstraction::OsabErrc::kSize            Size of the provided IPC message is too large to be sent
   *                                                            as a whole (IPC headers + IPC data exceeds the send ring
   *                                                            buffer size) or the message exceeds the maximum
   *                                                            supported message size
   *                                                            (see stream_ipc_protocol::kMaxIpcMessageSize).
   * \error           osabstraction::OsabErrc::kProtocolError   A SafeIPC protocol violation occurred and APIs of this
   *                                                            object should not be called anymore as the
   *                                                            StreamIpcChannel is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             No stream write is currently ongoing
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto DatagramWriteMessage(stream_ipc_protocol::CommonHeader* common_header,
                            stream_ipc_protocol::FormatBHeader const* format_b_header,
                            ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers) noexcept
      -> ::amsr::core::Result<DatagramWriteResult>;

 private:
  /*!
   * \brief           Finish write operations and check if a readable notification has to be sent to the peer.
   *
   * \details         Makes all writes that were performed visible to the peer process (otherwise the peer will not see
   *                  them).
   *
   * \return          True if a readable notification should be sent to the peer process, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto CommitWrite() noexcept -> bool;

  /*!
   * \brief           Writes common header of IPC message.
   *
   * \details         stream_writer_state_ may be changed by this function.
   *
   * \return          true of any data was written to the StreamIpcChannel, false otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_writer_state_ == StreamWriterState::kWritingCommonHeader
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamWriteCommonHeader() noexcept -> bool;

  /*!
   * \brief           Writes format B header of IPC message.
   *
   * \details         stream_writer_state_ may be changed by this function.
   *
   * \return          true of any data was written to the StreamIpcChannel, false otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_writer_state_ == StreamWriterState::kWritingFormatBHeader
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamWriteFormatBHeader() noexcept -> bool;

  /*!
   * \brief           Writes IPC data of IPC message.
   *
   * \details         stream_writer_state_ may be changed by this function.
   *
   * \return          true of any data was written to the StreamIpcChannel, false otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_writer_state_ == StreamWriterState::kWritingData
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamWriteData() noexcept -> bool;

  /*!
   * \brief           Checks if free space is available in the IPC channel for the currently being written part of the
                      IPC message (common header / format B header / IPC data) during a stream write.
   *
   * \return          true if free space is available for the currently being written part of the IPC message, false
   *                  otherwise.
   *
   * \context         ANY
   *
   * \pre             stream_writer_state_ != StreamReaderState::kWriteFinished
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StreamWriteCanContinue() const noexcept -> bool;

  /*!
   * \brief    StreamIpcChannel that the MessageWriter writes to.
   */
  StreamIpcChannel& ipc_channel_;

  /*!
   * \brief    States of the stream writer.
   */
  enum class StreamWriterState : std::uint32_t {
    /*!
     * \brief    The stream writer is currently writing an IPC common header.
     */
    kWritingCommonHeader,
    /*!
     * \brief    The stream writer is currently writing a format B header.
     */
    kWritingFormatBHeader,
    /*!
     * \brief    The stream writer is currently writing IPC data.
     */
    kWritingData,
    /*!
     * \brief    The stream writer finished a stream-based write.
     */
    kWriteFinished
  };

  /*!
   * \brief    State of the internal stream writer.
   */
  StreamWriterState stream_writer_state_{StreamWriterState::kWritingCommonHeader};

  /*!
   * \brief    IO buffer queue that manages the buffers that the user provided for a stream write operation.
   */
  osabstraction::io::internal::IoBufferQueue<osabstraction::io::ConstIOBuffer> stream_write_io_buffer_queue_{};

  /*!
   * \brief    Common header to send.
   */
  stream_ipc_protocol::CommonHeader* common_header_{nullptr};

  /*!
   * \brief    Format B header to send.
   * \details  This pointer is only valid  if the passed common_header_ indicates that the message format of the IPC
   *           message to send is format B.
   */
  stream_ipc_protocol::FormatBHeader const* format_b_header_{nullptr};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_MESSAGE_WRITER_H_
