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
 *        \brief  Defines common types used in SafeIPC.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_IPC_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_IPC_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/internal/stream_ipc_protocol_common.h"
#include "amsr/ipc/internal/stream_ipc_protocol_format_b.h"
#include "amsr/memory/memory_exchange_handle_interface.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc {

class Connection;

/*!
 * \brief Result of the Send() operation.
 */
enum class SendResult : std::uint32_t {
  /*!
   * \brief   The message was sent completely.
   */
  kSendCompleted,
  /*!
   * \brief   The message could not be sent completely. Asynchronous processing necessary to finalize.
   */
  kAsyncProcessingNecessary
};

/*!
 * \brief Result of a receive operation that also receives memory exchange handles.
 */
struct ReceiveResult {  // VCA_OSA_STL_CALL_OBJ_DTOR
  /*!
   * \brief   Size of the received message.
   */
  std::size_t message_size{0};

  /*!
   * \brief   Received memory exchange handle.
   * \details nullptr if no memory exchange handle was received with the IPC message.
   */
  std::unique_ptr<amsr::memory::MemoryExchangeHandleInterface> mem_exchange_handle;
};

/*!
 * \brief Result of a chunk receive operation.
 */
struct ReceiveChunkResult {
  /*!
   * \brief   Remaining size of the current message to be received.
   */
  std::size_t remaining_message_size{0};
};

/*!
 * \brief             Notifies about a completed asynchronous accept operation.
 *
 * \details           All Acceptor APIs are allowed to be called from an AcceptCompletionCallback.
 *
 * \param[in] result  The result of the accept operation. On success it contains the accepted connection.
 *                    Possible errors:
 *                    osabstraction::OsabErrc::kResource      Callback slots of next connection's reactor exhausted.
 *                    osabstraction::OsabErrc::kDisconnected  Acceptor object is not listening to connect requests
 *                                                            anymore until the Acceptor is closed and reinitialized.
 *                                                            SupportData if error is not set.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-SafeIpcReactor,
 *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer,
 *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer,
 *                  DSGN-Osab-SafeIpcServer
 */
using AcceptCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<Connection>&& result)>;

/*!
 * \brief     Callback that notifies user about a completed asynchronous connect operation.
 *
 * \details   The Connection is still open if the ConnectCompletionCallback reported an error,
 *            Close() is necessary in order to restart connection establishment using ConnectAsync().
 *            All Connection APIs are allowed to be called from an ConnectCompletionCallback.
 *
 * \param[in] result The result of the connect operation.
 *                   Possible errors:
 *
 *                   osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 *                   osabstraction::OsabErrc::kAddressNotAvailable     The server is not available.
 *                   osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to connect to server. This can also
 *                                                                     happen if the server is not completely
 *                                                                     initialized yet.
 *                   osabstraction::OsabErrc::kDisconnected            The peer aborted the connection establishment
 *                                                                     or is not available anymore.
 *                                                                     SupportData of error is not set.
 *                   osabstraction::OsabErrc::kProtocolError           The SafeIPC protocol was violated or the
 *                                                                     system environment is in an unexpected state
 *                                                                     (which might be caused by interference of an
 *                                                                     outside process, a wrong system integration or
 *                                                                     a failure of a system service, e.g. the file
 *                                                                     system).
 *                   osabstraction::OsabErrc::kResource                Not enough system resources to perform connect
 *                                                                     operation.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-SafeIpcReactor,
 *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentClient,
 *                  DSGN-Osab-SafeIpcClient
 */
using ConnectCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

/*!
 * \brief     Informs the user about an incoming message that is ready to be received.
 *
 * \details   The user must provide a buffer that has at least the size of the incoming message, otherwise the message
 *            will be truncated. All SafeIPC Connection APIs can be called from a MessageAvailableCallback.
 *
 * \param[in] message_size   The message size in bytes of the incoming message.
 *
 * \return    View on an IO buffer array provided by the user that points to the memory segments where the received
 *            message data shall be copied to. The IO buffer array that this span points to and the memory segments
 *            that each IO buffer points to have to stay valid until the receive operation completed or the Connection
 *            is closed.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcReceiveAsync
 *
 * \vprivate        Vector product internal API
 */
using MessageAvailableCallback = vac::language::UniqueFunction<::amsr::core::Span<osabstraction::io::MutableIOBuffer>(
    std::size_t const& message_size)>;

/*!
 * \brief     Informs the user that message reception has been completed or was aborted.
 *
 * \details   To chain message reception, i.e. to immediately trigger the next message reception it is recommended to
 *            call ReceiveAsync() in context of this callback. If the message reception has been aborted a corresponding
 *            error will be reported. All SafeIPC Connection APIs can be called from a ReceiveCompletionCallback.
 *            It is not possible to receive memory exchange handles with this callback. Received memory exchange handles
 *            will be dropped by SafeIPC and only the IPC data will be received if this callback is used instead of
 *            ReceiveCompletionExtendedCallback.
 *
 * \param[in] result  The size of the received message if reception was completed or corresponding error otherwise.
 *                    Possible errors:
 *                    osabstraction::OsabErrc::kTruncation      The provided buffer was too small for the IPC message.
 *                                                              The buffer was filled with the beginning of the message
 *                                                              and the rest of the message was discarded.
 *                    osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer and no
 *                                                              more incoming messages are in the receive buffer.
 *                                                              SupportData of error is set to
 *                                                              amsr::ipc::kOrderlyDisconnect or
 *                                                              amsr::ipc::kAbnormalDisconnect.
 *                    osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
 *                                                              SafeIPC protocol violation detected. The connection is
 *                                                              broken and should be closed.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcReceiveAsync
 *
 * \vprivate        Vector product internal API
 */
using ReceiveCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<std::size_t>&& result)>;

/*!
 * \brief     Informs the user that message reception has been completed or was aborted.
 *
 * \details   To chain message reception, i.e. to immediately trigger the next message reception it is recommended to
 *            call ReceiveAsync() in context of this callback. If the message reception has been aborted a corresponding
 *            error will be reported. All SafeIPC Connection APIs can be called from a
 *            ReceiveCompletionExtendedCallback.
 *
 * \param[in] result  The size and possibly a memory exchange handle of the received message if reception was completed
 *                    or corresponding error otherwise.
 *                    Possible errors:
 *                    osabstraction::OsabErrc::kTruncation      The provided buffer was too small for the IPC message.
 *                                                              The buffer was filled with the beginning of the message
 *                                                              and the rest of the message was discarded.
 *                    osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer and no
 *                                                              more incoming messages are in the receive buffer.
 *                                                              SupportData of error is set to
 *                                                              amsr::ipc::kOrderlyDisconnect or
 *                                                              amsr::ipc::kAbnormalDisconnect.
 *                    osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
 *                                                              SafeIPC protocol violation detected. The connection is
 *                                                              broken and should be closed.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcReceiveAsync,
 *                  DSGN-Osab-SafeIpcQNXTransferOfMemoryExchangeHandles,
 *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
 *
 * \vprivate        Vector product internal API
 */
using ReceiveCompletionExtendedCallback =
    vac::language::UniqueFunction<void(::amsr::core::Result<ReceiveResult>&& result)>;

/*!
 * \brief     Informs the user that message chunk reception has been completed or was aborted.
 *
 * \details   Can be called multiple times, for the same IPC  message, depending on the overall size of the buffers
 *            returned by MessageAvailableCallback (for the first chunk) and by this callback (for next chunks).
 *            To chain message reception, i.e. to immediately trigger the next message reception it is recommended to
 *            call ReceiveAsync() in context of this callback, only if the passed remaining size is 0
 *            (message completely received).
 *            If the message reception has been aborted a corresponding error will be reported.
 *            All SafeIPC Connection APIs can be called from a MessageChunkReceivedCallback.
 *            It is not possible to receive memory exchange handles with this callback. Received memory exchange handles
 *            will be dropped by SafeIPC and only the IPC data will be received if this callback is used instead of
 *            ReceiveCompletionExtendedCallback.
 *
 * \param[in] result  The remaining size of the being received message if chunk reception was completed or corresponding
 *                    error otherwise. Possible errors:
 *                    osabstraction::OsabErrc::kTruncation     The user provided an empty view in the last callback,
 *                                                             The rest of the message was discarded.
 *                    osabstraction::OsabErrc::kDisconnected   The connection has been terminated by the peer and no
 *                                                             more incoming messages are in the receive buffer.
 *                                                             SupportData of error is set to
 *                                                             amsr::ipc::kOrderlyDisconnect or
 *                                                             amsr::ipc::kAbnormalDisconnect.
 *                    osabstraction::OsabErrc::kProtocolError  Failed to send/receive notification to/from
 *                                                             IPC peer or SafeIPC protocol violation detected.
 *                                                             The connection is broken and should be closed.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \return    View on an IO buffer array provided by the user that points to the memory segments where the next
 *            received message chunk data shall be copied to.
 *            The IO buffer array that this span points to and the memory segments that each IO buffer points to
 *            have to stay valid until the next chunk receive operation completed or the Connection is closed.
 *            It is expected that an empty view is returned in case:
 *            - the result parameter contains an error
 *            - the result parameter contains a value with remaining_message_size equal to 0.
 *            - the user is not anymore interested in current message reception (will lead to kTruncation of the next
 *              callback call).
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcReceiveAsyncChunks
 *
 * \vprivate        Vector product internal API
 */
using MessageChunkReceivedCallback =
    vac::language::UniqueFunction<::amsr::core::Span<osabstraction::io::MutableIOBuffer>(
        ::amsr::core::Result<ReceiveChunkResult>&& result)>;

/*!
 * \brief     Informs the user that asynchronous send operation was finished successfully or an error occurred.
 *
 * \details   All SafeIPC Connection APIs can be called from a SendCompletionCallback.
 *
 * \param[in] result  Empty value if transmission was successful. Error code if an error occurred.
 *                    Possible errors:
 *                    osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer.
 *                                                              SupportData of error is set to
 *                                                              amsr::ipc::kOrderlyDisconnect or
 *                                                              amsr::ipc::kAbnormalDisconnect.
 *                    osabstraction::OsabErrc::kResource        Not enough system resource to send memory exchange
 *                                                              handle to peer.
 *                    osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
 *                                                              SafeIPC protocol violation detected. The connection is
 *                                                              broken and should be closed.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcSendAsync,
 *                  DSGN-Osab-SafeIpcSend
 *
 * \vprivate        Vector product internal API
 */
using SendCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

/*!
 * \brief   A notification that can be send/received by a SafeIPC connection.
 * \details The value is not allowed to exceed kMaxNotificationValue.
 */
struct Notification {
  /*!
   * \brief Notification value.
   */
  std::uint8_t value;
};

/*!
 * \brief     Callback that is called when a notification is received on a SafeIPC connection that was sent by the peer
 *            with Connection::SendNotification().
 *
 * \details   Notifications may still be received even if the SafeIPC connection is in protocol error state.
 *            All SafeIPC Connection APIs can be called from a NotificationCallback.
 *
 * \param[in] notification  Received notification
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 *
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 *
 * \trace           DSGN-Osab-SafeIpcQNXUserNotifications,
 *                  DSGN-Osab-SafeIpcLinuxUserNotifications
 *
 * \vprivate        Vector product internal API
 */
using NotificationCallback = vac::language::UniqueFunction<void(Notification notification)>;

/*!
 * \brief   Maximum allowed notification value.
 * \details Values larger than this are reserved for SafeIPC internal use.
 */
constexpr Notification kMaxNotificationValue{249U};

/*!
 * \brief Limit for the number of IoBuffers that are processed at once.
 */
constexpr std::size_t const kMaximumNumberOfIoBuffers{32};

/*!
 * \brief Maximum size of an IPC message that is is supported by SafeIPC (in bytes).
 */
constexpr std::size_t const kMaximumMessageSize{internal::stream_ipc_protocol::kMaxIpcMessageSize};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief   Number of bytes that each IPC messages without an attached memory exchange handle uses up in the
 *          client-to-server or server-to-client shared buffer additionally to the actually transferred message data.
 * \details The SafeIPC protocol adds additional bytes to each transferred IPC message which also takes up space in the
 *          allocated shared buffer. Thus the amount of IPC data that can be stored in a shared buffer is lower than the
 *          size of the shared buffer, how much depends on the number of IPC messages.
 */
constexpr std::size_t const kMessageProtocolOverhead{internal::stream_ipc_protocol::CommonHeader::kSize};

/*!
 * \brief   Number of bytes that each IPC messages with an attached memory exchange handle uses up in the
 *          client-to-server or server-to-client shared buffer additionally to the actually transferred message data.
 * \details The SafeIPC protocol adds additional bytes to each transferred IPC message which also takes up space in the
 *          allocated shared buffer. Thus the amount of IPC data that can be stored in a shared buffer is lower than the
 *          size of the shared buffer, how much depends on the number of IPC messages.
 */
constexpr std::size_t const kMessageProtocolOverheadMemoryHandleExchange{
    internal::stream_ipc_protocol::CommonHeader::kSize + internal::stream_ipc_protocol::FormatBHeader::kSize};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief   SupportData for kDisconnected error returned by SafeIPC that indicates that the peer closed the connection
 *          orderly.
 * \details
 * Orderly close means that the peer did call Connection::Close() or Connection::~Connection() to close the
 * connection.
 *
 * SupportData for kDisconnected error is only set for kDisconnected returned by following APIs:
 *
 *  - amsr::ipc::Connection::SendSync()
 *  - amsr::ipc::Connection::Send()
 *  - amsr::ipc::SendCompletionCallback
 *  - amsr::ipc::Connection::GetPendingMsgInfo()
 *  - amsr::ipc::Connection::ReceiveSync()
 *  - amsr::ipc::ReceiveCompletionCallback
 *  - amsr::ipc::ReceiveCompletionExtendedCallback
 */
constexpr ::amsr::core::ErrorDomain::SupportDataType kOrderlyDisconnect{1};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief   SupportData for kDisconnected error returned by SafeIPC that indicates that the connection was closed on
            peer side abnormally.
 * \details
 * Abnormal close means that the peer did not call Connection::Close() or Connection::~Connection() to close the
 * connection. This normally means that the operating system closed the connection because the peer process terminated.
 * Note that abnormal disconnect may also be indicated if the peer did not follow the SafeIPC close procedure, e.g
 * because he ran into undefined behavior.
 *
 * SupportData for kDisconnected error is only set for kDisconnected returned by following APIs:
 *
 *  - amsr::ipc::Connection::SendSync()
 *  - amsr::ipc::Connection::Send()
 *  - amsr::ipc::SendCompletionCallback
 *  - amsr::ipc::Connection::GetPendingMsgInfo()
 *  - amsr::ipc::Connection::ReceiveSync()
 *  - amsr::ipc::ReceiveCompletionCallback
 *  - amsr::ipc::ReceiveCompletionExtendedCallback
 */
constexpr ::amsr::core::ErrorDomain::SupportDataType kAbnormalDisconnect{2};

/*!
 * \brief   The size of a server-to-client buffer.
 */
struct BufferSizeS2C {
  /*!
   * \brief The size of the buffer in bytes.
   */
  std::size_t size;
};

/*!
 * \brief   The size of a client-to-server buffer.
 */
struct BufferSizeC2S {
  /*!
   * \brief The size of the buffer in bytes.
   */
  std::size_t size;
};

}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_IPC_TYPES_H_
