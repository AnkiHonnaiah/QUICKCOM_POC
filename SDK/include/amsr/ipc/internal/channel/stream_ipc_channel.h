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
 *        \brief  Implements a bidirectional IPC channel over shared memory.
 *         \unit  osabstraction::ipc::IpcConnectionMessageHandling_QNX
 *         \unit  osabstraction::ipc::IpcConnectionMessageHandling_LinuxPikeOs
 *   \complexity  IpcConnectionMessageHandling_QNX exceeds following metrics:
 *                - SUM, tCYC: Reason is that two independent functionalities are implemented in the same unit: read and
 *                  write attempts of IPC messages affect two different ring buffers mapped to different shared memory
 *                  locations. They can be tested independently from each other and they affect different requirements,
 *                  so the safety analysis is not gaining any benefit by splitting this unit.
 *                - mTCYC, mwCYC: Reason is that reading IPC messages (as a byte stream or as a whole) requires
 *                  several steps:
 *                  - Fetch current status of the ring buffer from the shared memory.
 *                  - Interpret byte as header or payload.
 *                  - Eventually discard bytes.
 *                  - Notify the other peer if enough free space available.
 *                  - Store updated status of the ring buffer in the shared memory.
 *                  Listed steps have been fully tested without the need to split this unit further.
 *                - mIMU: Cannot be avoided, since SafeIpc code is spread among different units.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_CHANNEL_STREAM_IPC_CHANNEL_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_CHANNEL_STREAM_IPC_CHANNEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/internal/mapped_memory.h"
#include "amsr/ipc/internal/ring_buffer_view.h"
#include "amsr/ipc/internal/stream_ipc_protocol_common.h"
#include "amsr/ipc/internal/stream_ipc_protocol_format_b.h"
#include "osabstraction/io/io_buffer.h"

namespace amsr {
namespace ipc {
namespace internal {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_StreamIpcChannel
/*!
 * \brief    Implements a bidirectional IPC channel over shared memory.
 *
 * \details  The IPC channel supports both stream-based and datagram-based communication.
 *           This class owns the shared memory.
 */
class StreamIpcChannel final {
 public:
  /*!
   * \brief   Stores temporary data while the send and receive shared memory are not both assigned yet to a
   *          StreamIpcChannel object.
   * \details Needed because a RingBufferView object can only be created when the location of both head and tail indices
   *          of a ring buffer are known which is only the case after both shared memories are assigned as the head and
   *          tail indices of a ring buffer are distributed on both shared memories.
   */
  struct RingBufferCreationData {
    /*!
     * \brief    Points to the atomic head index of the send ring buffer in the shared memory.
     */
    std::atomic<std::uint32_t>* send_ring_buffer_head_index_ptr{nullptr};

    /*!
     * \brief    Points to the atomic tail index of the send ring buffer in the shared memory.
     */
    std::atomic<std::uint32_t>* send_ring_buffer_tail_index_ptr{nullptr};

    /*!
     * \brief    Points to the atomic head index of the receive ring buffer in the shared memory.
     */
    std::atomic<std::uint32_t>* receive_ring_buffer_head_index_ptr{nullptr};

    /*!
     * \brief    Points to the atomic tail index of the receive ring buffer in the shared memory.l
     */
    std::atomic<std::uint32_t>* receive_ring_buffer_tail_index_ptr{nullptr};

    /*!
     * \brief    Pointer to the data buffer of the send ring buffer.
     */
    ::amsr::core::Span<std::uint8_t> send_ring_buffer_data{};

    /*!
     * \brief    Pointer to the data buffer of the receive ring buffer.
     */
    ::amsr::core::Span<std::uint8_t> receive_ring_buffer_data{};
  };

  /*!
   * \brief           Creates a StreamIpcChannel.
   *
   * \details         The receive shared memory, the send shared memory and the notification shared memory have to be
   *                  added to this channel with the Init* or Open* functions before it can be used.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  StreamIpcChannel() noexcept = default;

  /*!
   * \brief           Move constructs a StreamIpcChannel
   *
   * \param[in]       other   StreamIpcChannel to move construct from.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  StreamIpcChannel(StreamIpcChannel&& other) noexcept = default;

  /*!
   * \brief           Move assigns a StreamIpcChannel.
   *
   * \param[in]       other   StreamIpcChannel to move assign.
   *
   * \return          Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  StreamIpcChannel& operator=(StreamIpcChannel&& other) & noexcept;

  StreamIpcChannel(StreamIpcChannel const&) noexcept = delete;
  StreamIpcChannel& operator=(StreamIpcChannel const&) noexcept = delete;

  /*!
   * \brief           Destroys a StreamIpcChannel and unmaps all shared memories.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~StreamIpcChannel() noexcept = default;

  /*!
   * \brief           Initializes a send direction shared memory, assigns it to this IPC channel, optionally
   *                  initializes and assigns the flags used for notifications to this IPC channel (only on PikeOs).
   *
   * \details         This object takes ownership of the passed shared memory.
   *
   * \param[in]       send_direction_shm   Send direction shared memory to initialize and assign to this channel.
   * \param[in]       contains_notification_shm   true if the atomic flags used by the notification mechanism are mapped
   *                                              to the send shared memory (in other words if the calling peer is
   *                                              the client) and the underlying Os is PikeOs, false otherwise.
   *
   * \context         ANY
   *
   * \pre             This IPC channel has currently no send direction shared memory assigned.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AssignAndInitSendShm(MappedMemory&& send_direction_shm, bool contains_notification_shm) noexcept;

  /*!
   * \brief           Initializes a notification shared memory and assigns it to this IPC channel.
   *
   * \details         This object takes ownership of the passed shared memory.
   *
   * \param[in]       notification_shm   Notification shared memory to initialize and assign to this channel.
   *
   * \context         ANY
   *
   * \pre             This IPC channel has currently no notification shared memory assigned.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AssignAndInitNotificationShm(MappedMemory&& notification_shm) noexcept;

  /*!
   * \brief           Assigns a receive direction shared memory and optionally assigns and initializes the flags used
   *                  for notifications to this IPC channel (only on PikeOs).
   *
   * \details         This object takes ownership of the passed shared memory.
   *
   * \param[in]       receive_direction_shm   Receive direction shared memory to assign to this channel.
   *
   * \param[in]       contains_notification_shm   true if the atomic flags used by the notification mechanism are mapped
   *                                              to the receive shared memory (in other words if the calling peer is
   *                                              the server) and the underlying Os is PikeOs, false otherwise.
   *
   * \context         ANY
   *
   * \pre             This IPC channel has currently no receive direction shared memory assigned.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AssignReceiveShm(MappedMemory&& receive_direction_shm, bool contains_notification_shm) noexcept;

  /*!
   * \brief           Assigns a notification shared memory to this IPC channel.
   *
   * \details         This object takes ownership of the passed shared memory.
   *
   * \param[in]       notification_shm   Notification shared memory to assign to this channel.
   *
   * \context         ANY
   *
   * \pre             This IPC channel has currently no notification shared memory assigned.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AssignNotificationShm(MappedMemory&& notification_shm) noexcept;

  /*!
   * \brief           Gets the size of the send direction buffer.
   *
   * \return          Size of the send direction buffer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetSendBufferSize() const noexcept -> std::uint32_t { return send_ring_buffer_.GetCapacity(); }

  /*!
   * \brief           Gets the size of the receive direction buffer.
   *
   * \return          Size of the receive direction buffer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetReceiveBufferSize() const noexcept -> std::uint32_t { return receive_ring_buffer_.GetCapacity(); }

  /*!
   * \brief           Updates the local view on the receive buffer.
   *
   * \details         The atomic head index of the receive ring buffer in the shared memory is loaded and checked if it
   *                  is valid.
   *
   * \error           osabstraction::OsabErrc::kProtocolError    The loaded head index of the receive ring buffer is
   *                                                             invalid. A SafeIPC protocol violation occurred and APIs
   *                                                             of this object should not be called anymore (except
   *                                                             GetSendBufferSize() or GetReceiveBufferSize()) as the
   *                                                             ring buffer is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto UpdateLocalViewOnReceiveBuffer() noexcept -> ::amsr::core::Result<void> {
    return receive_ring_buffer_.LoadHeadIndex();
  }

  /*!
   * \brief           Checks if the receive ring buffer contains data for at least one common IPC header.
   *
   * \details         It is not checked if the data is actually a common IPC header, only that the amount of data in the
   *                  ring buffer is enough for one common IPC header.
   *
   * \return          True if data for a common IPC header is available in the receive ring buffer, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsCommonHeaderAvailable() const noexcept -> bool {
    return receive_ring_buffer_.GetUsedSpace() >= stream_ipc_protocol::CommonHeader::kSize;
  }

  /*!
   * \brief           Checks if the receive ring buffer contains data for at least one format B IPC header.
   *
   * \details         It is not checked if the data is actually a format B IPC header, only that the amount of data in
   *                  the ring buffer is enough for one format B IPC header.
   *
   * \return          True if data for a format B IPC header is available in the receive ring buffer, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsFormatBHeaderAvailable() const noexcept -> bool {
    return receive_ring_buffer_.GetUsedSpace() >= stream_ipc_protocol::FormatBHeader::kSize;
  }

  /*!
   * \brief           Checks if the receive ring buffer contains any data.
   *
   * \return          True if the receive ring buffer contains at least 1 byte of data, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsAnyDataAvailable() const noexcept -> bool { return !receive_ring_buffer_.IsEmpty(); }

  /*!
   * \brief           Checks if the receive ring buffer is filled beyond the water mark.
   *
   * \details         If the receive ring buffer is filled beyond the water mark, it means that it is
   *                  quite full and it is generally not worth it to send a writable notification to the
   *                  peer as he will not be able to write much data.
   *                  The position of the water mark is defined by
   *                  stream_ipc_protocol::kBufferSizeDivisorForWritableNotification and is relative to the
   *                  capacity of the ring buffer.
   *
   * \return          True if the receive ring buffer is filled beyond the water mark, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsReceiveBufferWaterMarkExceeded() const noexcept -> bool {
    return receive_ring_buffer_.GetFreeSpace() <
           (receive_ring_buffer_.GetCapacity() / stream_ipc_protocol::kBufferSizeDivisorForWritableNotification);
  }

  /*!
   * \brief           Look at the next common IPC message header that is about to be received but do not remove it from
   *                  the receive ring buffer.
   *
   * \details         The next common IPC message header has to be the next data to be received in the receive ring
   *                  buffer. If a common header was available it is also validated against the IPC protocol and an
   *                  error is returned if the validation failed.
   *
   * \param[out]      common_header   Location where the peeked common header shall be saved.
   *
   * \error           osabstraction::OsabErrc::kBusy            Not enough data for a common IPC header in the receive
   *                                                            ring buffer available.
   * \error           osabstraction::OsabErrc::kProtocolError   The next common IPC message header violates the IPC
   *                                                            protocol.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto PeekCommonHeader(stream_ipc_protocol::CommonHeader* common_header) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Receive the next common IPC message header and remove it from the receive ring buffer.
   *
   * \details         The next common IPC message header has to be the next data to be received in the receive ring
   *                  buffer.
   *
   * \param[out]      common_header   Location where the received common header shall be saved. Must be valid for
   *                                  write until the function returns.
   *
   * \error           osabstraction::OsabErrc::kBusy            Not enough data for a common IPC header available in the
   *                                                            receive ring buffer.
   * \error           osabstraction::OsabErrc::kProtocolError   The next common IPC message header violates the IPC
   *                                                            protocol.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ReceiveCommonHeader(stream_ipc_protocol::CommonHeader* common_header) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Receive the next format B IPC message header and remove it from the receive ring buffer.
   *
   * \details         The next format B IPC message header has to be the next data to be received in the receive ring
   *                  buffer.
   *
   * \param[out]      format_b_header   Location where the received format B header shall be saved. Must be valid for
   *                                    write until the function returns.
   *
   * \error           osabstraction::OsabErrc::kBusy            Not enough data for a format B IPC header available in
   *                                                            the receive ring buffer.
   * \error           osabstraction::OsabErrc::kProtocolError   The next format B IPC message header violates the IPC
   *                                                            protocol.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ReceiveFormatBHeader(stream_ipc_protocol::FormatBHeader* format_b_header) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief    Result of a ReceivePartOfData() operation.
   */
  struct ReceivePartOfDataResult {
    /*!
     * \brief    Number of bytes that were received.
     */
    std::uint32_t received_bytes;

    /*!
     * \brief    True if parts of the received IPC message could not be received because the provided buffer was too
     *           small, false otherwise.
     */
    bool truncation;
  };

  /*!
   * \brief           Receives data of an IPC message and removes it from the receive ring buffer.
   *
   * \details         The IPC message data has to be the next data to be received in the receive ring buffer. This
   *                  function does not receive the headers of the IPC message and receives as much data of the IPC
   *                  message as possible.
   *
   * \param[in]       remaining_message_bytes   Number of bytes of the IPC message that were not received yet. This is
   *                                            the maximum number of bytes that will be received but less bytes can
   *                                            also be received (even 0) if the whole rest of the IPC message is not
   *                                            available in the receive ring buffer yet.
   * \param[out]      receive_buffer            Array of buffers where the received message shall be copied to. If the
   *                                            overall size of the buffers is smaller than remaining_message_bytes, the
   *                                            truncation flag might get set in the receive result.
   *
   * \return          Number of bytes of the IPC message that were received and a flag that indicates if truncation
   *                  occurred. If the truncation flag is set, the number of received bytes is still correct and will be
   *                  less than the remaining_message_bytes. The rest of the message is not discarded by this function,
   *                  this has to be done with DiscardRestOfData().
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ReceivePartOfData(std::uint32_t remaining_message_bytes,
                         ::amsr::core::Span<osabstraction::io::MutableIOBuffer> receive_buffer) noexcept
      -> ReceivePartOfDataResult;

  /*!
   * \brief           Remove remaining IPC data of an IPC message from the receive ring buffer.
   *
   * \details         The IPC data that shall be discarded has to be the next data to be received in the receive ring
   *                  buffer.
   *
   * \param[in]       remaining_message_bytes   Remaining bytes of the IPC message that shall be discarded. If the
   *                                            receive ring buffer does not contain this many bytes, as many bytes as
   *                                            possible are discarded.
   *
   * \return          Number of bytes that were discarded.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto DiscardRestOfData(std::uint32_t remaining_message_bytes) noexcept -> std::uint32_t;

  /*!
   * \brief    Result of a ReceiveWholeMessage() operation.
   */
  struct ReceiveWholeMessageResult {
    /*!
     * \brief    True if parts of the received IPC message could not be received because the provided buffer was too
     *           small, false otherwise.
     */
    bool truncation;
  };

  /*!
   * \brief           Receives a whole IPC message and removes it from the receive ring buffer.
   *
   * \details         The IPC message has to be the next data to be received in the receive ring buffer. This function
   *                  receives all the headers of the IPC message and the data of the IPC message. This function only
   *                  receives whole messages, no data is received from the ring buffer if all headers and data of
   *                  the next IPC message are not yet available in the ring buffer.
   *
   * \param[out]      common_header             Location where the received common header shall be saved.
   * \param[out]      format_b_header           Location where the possibly received format B header shall be saved. A
   *                                            format B header will only be written if the message format in
   *                                            common_header is format B.
   * \param[out]      receive_buffer            Array of buffers where the received message shall be copied to. If the
   *                                            overall size of the buffers is smaller than the size of the received
   *                                            message, the received message will get truncated.
   *
   * \return          Flag that indicates if truncation occurred. If the truncation flag is set, the received bytes that
   *                  could not be copied into the provided buffer are discarded by this function.
   *
   * \error           osabstraction::OsabErrc::kBusy            No whole IPC message is available in the receive ring
   *                                                            buffer yet.
   * \error           osabstraction::OsabErrc::kSize            Size of next IPC message is too large to be received as
   *                                                            a whole (IPC header + IPC data exceeds the receive ring
   *                                                            buffer size).
   * \error           osabstraction::OsabErrc::kProtocolError   The next IPC message header violates the IPC protocol.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ReceiveWholeMessage(stream_ipc_protocol::CommonHeader* common_header,
                           stream_ipc_protocol::FormatBHeader* format_b_header,
                           ::amsr::core::Span<osabstraction::io::MutableIOBuffer> receive_buffer) noexcept
      -> ::amsr::core::Result<ReceiveWholeMessageResult>;

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Updates the atomic tail index of the receive ring buffer in the shared memory.
   *
   * \details         This makes the removal of data from the receive ring buffer by receive and/or discard operations
   *                  visible to the IPC peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void CommitReceive() noexcept { return receive_ring_buffer_.StoreTailIndex(); }

  /*!
   * \brief           Updates the local view on the send buffer.
   *
   * \details         The atomic tail index of the send ring buffer in the shared memory is loaded and checked if it
   *                  is valid.
   *
   * \error           osabstraction::OsabErrc::kProtocolError    The loaded tail index of the send ring buffer is
   *                                                             invalid. A SafeIPC protocol violation occurred and APIs
   *                                                             of this object should not be called anymore (except
   *                                                             GetSendBufferSize() or GetReceiveBufferSize()) as the
   *                                                             ring buffer is in an undefined state.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto UpdateLocalViewOnSendBuffer() noexcept -> ::amsr::core::Result<void> {
    return send_ring_buffer_.LoadTailIndex();
  }

  /*!
   * \brief           Checks if the send ring buffer has empty space for at least one common IPC header.
   *
   * \return          True if empty space for a common IPC header is available in the send ring buffer, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsSpaceForCommonHeaderAvailable() const noexcept -> bool {
    return send_ring_buffer_.GetFreeSpace() >= stream_ipc_protocol::CommonHeader::kSize;
  }

  /*!
   * \brief           Checks if the send ring buffer has empty space for at least one format B IPC header.
   *
   * \return          True if empty space for a format B IPC header is available in the send ring buffer, false
   *                  otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsSpaceForFormatBHeaderAvailable() const noexcept -> bool {
    return send_ring_buffer_.GetFreeSpace() >= stream_ipc_protocol::FormatBHeader::kSize;
  }

  /*!
   * \brief           Checks if the send ring buffer has any free space.
   *
   * \return          True if the send ring buffer has at least 1 byte of free space, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsSpaceForAnyDataAvailable() const noexcept -> bool { return !send_ring_buffer_.IsFull(); }

  /*!
   * \brief           Adds a common IPC message header to the send ring buffer.
   *
   * \param[in,out]   common_header   Common header to send. The message sequence number field of this header does not
   *                                  need to be filled out, this will be done by this function.
   *
   * \error           osabstraction::OsabErrc::kBusy   Not enough empty space for a common IPC header available in the
   *                                                   send ring buffer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendCommonHeader(stream_ipc_protocol::CommonHeader* common_header) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Adds a format B IPC message header to the send ring buffer.
   *
   * \param[in]       format_b_header Format B header to send.
   *
   * \error           osabstraction::OsabErrc::kBusy   Not enough empty space for a format B IPC header available in the
   *                                                   send ring buffer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendFormatBHeader(stream_ipc_protocol::FormatBHeader const* format_b_header) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief           Adds data of an IPC message to the send ring buffer.
   *
   * \details         This function does not send the headers of the IPC message and sends as much data of the IPC
   *                  message as possible.
   *
   * \param[in]       remaining_message_bytes   Number of bytes of the IPC message that were not sent yet. This is
   *                                            the maximum number of bytes that will be sent but less bytes can also
   *                                            be sent (even 0) if not enough empty space for the whole remaining
   *                                            IPC message is available in the send ring buffer. This value shall be
   *                                            equal to the overall size of the provided send_buffer.
   * \param[in]       send_buffer               Array of buffers that contain the rest of the message that shall be
   *                                            sent.
   *
   * \return          Number of bytes of the IPC message that were sent.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendPartOfData(std::uint32_t remaining_message_bytes,
                      ::amsr::core::Span<osabstraction::io::ConstIOBuffer> send_buffer) noexcept -> std::uint32_t;

  /*!
   * \brief           Adds a whole IPC message to the send ring buffer.
   *
   * \details         This function sends both the headers of the IPC message and the data of the IPC message. This
   *                  function only sends whole messages, no data is added to the ring buffer if all headers and
   *                  data of the provided IPC message do not fit into the send ring buffer at the moment.
   *
   * \param[in,out]   common_header   Common header to send. The message sequence number field of this header does not
   *                                  need to be filled out, this will be done by this function.
   * \param[in]       format_b_header Format B header to send. This pointer will only by evaluated if the passed common
   *                                  header indicates that the message format of the IPC message is format B. This
   *                                  pointer may be nullptr if the message format is format A.
   * \param[in]       send_buffer     Array of buffers that contain the data of the IPC message that shall be sent.
   *
   * \error           osabstraction::OsabErrc::kBusy   Not enough space for the whole IPC message is available in the
   *                                                   send ring buffer yet.
   * \error           osabstraction::OsabErrc::kSize   Size of the provided IPC message is too large to be sent as
   *                                                   a whole (IPC headers + IPC data exceeds the send ring buffer
   *                                                   size).
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendWholeMessage(stream_ipc_protocol::CommonHeader* common_header,
                        stream_ipc_protocol::FormatBHeader const* format_b_header,
                        ::amsr::core::Span<osabstraction::io::ConstIOBuffer> send_buffer) noexcept
      -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Updates the atomic head index of the send ring buffer in the shared memory.
   *
   * \details         This makes the addition of data to the send ring buffer by send operations visible to the IPC
   *                  peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void CommitSend() noexcept { return send_ring_buffer_.StoreHeadIndex(); }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Requests a readable notification from the peer.
   *
   * \details         This function only sets the flag in the notification shared memory but does not notify the peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void RequestReadableNotification() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    readable_notification_fulfilled_->clear(std::memory_order_seq_cst);
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Requests a writable notification from the peer.
   *
   * \details         This function only sets the flag in the notification shared memory but does not notify the peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void RequestWritableNotification() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    writable_notification_fulfilled_->clear(std::memory_order_seq_cst);
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Cancels a readable notification request that was issued with RequestReadableNotification().
   *
   * \details         This function only resets the flag in the notification shared memory but does not notify the peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void ResetReadableNotificationRequest() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    static_cast<void>(readable_notification_fulfilled_->test_and_set(std::memory_order_seq_cst));
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Cancel a writable notification request that was issued with RequestWritableNotification().
   *
   * \details         This function only resets the flag in the notification shared memory but does not notify the peer.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void ResetWritableNotificationRequest() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    static_cast<void>(writable_notification_fulfilled_->test_and_set(std::memory_order_seq_cst));
  }

  /*!
   * \brief           Checks if the peer requested a readable notification and reset this readable notification request.
   *
   * \details         This function only checks and resets the flag in the notification shared memory but does not
   *                  notify the peer.
   *
   * \return          True if the peer requested a readable notification, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto TestAndResetPeerReadableNotificationRequest() noexcept -> bool {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    return !peer_readable_notification_fulfilled_->test_and_set(std::memory_order_seq_cst);
  }

  /*!
   * \brief           Checks if the peer requested a writable notification and reset this writable notification request.
   *
   * \details         This function only checks and resets the flag in the notification shared memory but does not
   *                  notify the peer.
   *
   * \return          True if the peer requested a writable notification, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto TestAndResetPeerWritableNotificationRequest() noexcept -> bool {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    return !peer_writable_notification_fulfilled_->test_and_set(std::memory_order_seq_cst);
  }

  /*!
   * \brief           Checks if the RingBufferCreationData is fully initialized.
   *
   * \details         If the RingBufferCreationData is fully initialized, its members can be used to construct the
   *                  RingBufferView objects from it.
   *
   * \param[in]   data    Creation data for a RingBufferView.
   *
   * \return          True if the RingBufferCreationData is fully initialized, false otherwise.
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
   *  - Check and return if all members of the passed data do not contain default values anymore.
   * \endinternal
   */
  static auto IsInitialized(RingBufferCreationData const& data) noexcept -> bool {
    return (data.send_ring_buffer_head_index_ptr != nullptr) && (data.send_ring_buffer_tail_index_ptr != nullptr) &&
           (data.receive_ring_buffer_head_index_ptr != nullptr) &&
           (data.receive_ring_buffer_tail_index_ptr != nullptr) && (!data.send_ring_buffer_data.empty()) &&
           (!data.receive_ring_buffer_data.empty());
  }

 private:
  /*!
   * \brief           Remove the next common IPC message header that is about to be received from the receive ring
   *                  buffer.
   *
   * \details         The next common IPC message header has to be the next data to be received in the receive ring
   *                  buffer.
   *
   * \context         ANY
   *
   * \pre             Data for a common IPC header has to be available in the receive ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void DiscardCommonHeader() noexcept;

  /*!
   * \brief   Send direction shared memory of this IPC channel.
   */
  MappedMemory send_shm_{};

  /*!
   * \brief   Receive direction shared memory of this IPC channel.
   */
  MappedMemory receive_shm_{};

  /*!
   * \brief    Notification shared memory of this IPC channel.
   * \details  Not used on PikeOs.
   */
  MappedMemory notification_shm_{};

  /*!
   * \brief   View on the send ring buffer inside the shared memory.
   */
  RingBufferView send_ring_buffer_{};

  /*!
   * \brief   View on the receive ring buffer inside the shared memory.
   */
  RingBufferView receive_ring_buffer_{};

  // std::atomic_flag is always lock-free. It is assumed that lock-free atomics are also address-free on all
  // architectures supported by AMSR. This is also a precondition for the lock-free ring buffer algorithm to work in
  // shared memory but this cannot be checked by the compiler.

  /*!
   * \brief   Points to the atomic flag in the notification shared memory that is used for readable notification
   *          requests issue by this process.
   * \details If the flag is cleared it means that a readable notification is requested.
   */
  std::atomic_flag* readable_notification_fulfilled_{nullptr};

  /*!
   * \brief   Points to the atomic flag in the notification shared memory that is used for writable notification
   *          requests issue by this process.
   * \details If the flag is cleared it means that a writable notification is requested.
   */
  std::atomic_flag* writable_notification_fulfilled_{nullptr};

  /*!
   * \brief   Points to the atomic flag in the notification shared memory that is used for readable notification
   *          requests issue by the peer process.
   * \details If the flag is cleared it means that a readable notification is requested.
   */
  std::atomic_flag* peer_readable_notification_fulfilled_{nullptr};

  /*!
   * \brief   Points to the atomic flag in the notification shared memory that is used for writable notification
   *          requests issue by the peer process.
   * \details If the flag is cleared it means that a writable notification is requested.
   */
  std::atomic_flag* peer_writable_notification_fulfilled_{nullptr};

  /*!
   * \brief   IPC message sequence number that the next sent IPC message header shall contain.
   * \details Starts at 1 and wraps around to 0 on std::uint16_t overflow.
   */
  std::uint16_t next_send_message_seq_num_{1};

  /*!
   * \brief   IPC message sequence number that the next received IPC message header shall contain.
   * \details Starts at 1 and wraps around to 0 on std::uint16_t overflow.
   */
  std::uint16_t next_receive_message_seq_num_{1};

  /*!
   * \brief   Stores temporary data while the send and receive shared memory are not both assigned yet to this object.
   */
  RingBufferCreationData ring_buffer_creation_data_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_CHANNEL_STREAM_IPC_CHANNEL_H_
