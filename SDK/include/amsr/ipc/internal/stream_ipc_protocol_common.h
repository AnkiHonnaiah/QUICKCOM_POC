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
 *        \brief  Defines common parts of IPC protocol of SafeIPC.
 *        \unit   osabstraction::ipc::IpcUtils_LinuxPikeOs
 *        \unit   osabstraction::ipc::IpcUtils_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_COMMON_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_COMMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace stream_ipc_protocol {

/*!
 * \brief    IPC protocol version.
 * \details  Used to verify that both peers use the same protocol during connection establishment.
 */
static constexpr std::uint16_t kId{2};

/*!
 * \brief    Maximum supported size of an IPC message.
 */
static constexpr std::size_t kMaxIpcMessageSize{std::numeric_limits<std::uint32_t>::max()};

/*!
 * \brief    Defines what fraction of the receive buffer has to be empty for a peer to send
 *           a writable notification if it was requested.
 * \details  Writable notifications are only sent if they are requested if a significant amount
 *           of the free space exists in the buffer after a read operation so that the ping-pong
 *           between the peer processes is reduced (when the peer receives the writable notification
 *           he will be able to actually write a significant amount of data).
 *           This value defines what a "significant amount" exactly means:
 *           significant amount of free space = capacity of buffer / kBufferSizeDivisorForWritableNotification
 */
static constexpr std::size_t kBufferSizeDivisorForWritableNotification{4};

/*!
 * \brief           Gets default buffer size.
 *
 * \details         Read value of AMSR_IPC_DEFAULT_SHM_SIZE environment variable.
 *                  If not valid or not defined, returns 128 KB.
 *
 * \return          Default buffer size (in bytes).
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \trace           DSGN-Osab-SafeIpcBufferSizeConfiguration
 *
 */
auto GetDefaultBufferSize() -> std::size_t;

/*!
 * \brief           Gets a buffer size value that causes the value set on server side to be used as buffer size.
 *
 * \details         Returns the maximum possible value for the type. This means there are two possible situations:
 *                  - The value set for the server is smaller than this value. As the value set for the server defines
 *                    an upper limit, the value of the server is used in this case.
 *                  - The value set for the server is the same as this value. As server and client requested the same
 *                    value, this value will be used.
 *                  As can be seen, the value set for the server is used in both cases.
 *
 * \return          Buffer size for server-to-client buffer (in bytes).
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \trace           DSGN-Osab-SafeIpcBufferSizeConfiguration
 *
 */
auto UseServerValue() -> std::size_t;

/*!
 * \brief           Gets a valid client-to-server shared memory size from a requested client-to-server buffer size.
 *
 * \param[in]       requested_buffer_size   Size that the client-to-server buffer should have.
 *
 * \return          Valid client-to-server shared memory size. This considers the management data in the shared
 *                  memory. If the requested client-to-server buffer size is below the minimum, the minimum
 *                  allowed client-to-server buffer size is used to calculate the client-to-server shared memory size.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetValidClientToServerShmSize(std::size_t requested_buffer_size) -> std::size_t;

/*!
 * \brief           Gets a valid server-to-client shared memory size from a requested server-to-client buffer size.
 *
 * \param[in]       requested_buffer_size   Size that the sever-to-client buffer should have.
 *
 * \return          Valid server-to-client shared memory size. This considers the management data in the shared
 *                  memory. If the requested server-to-client buffer size is below the minimum, the minimum
 *                  allowed server-to-client buffer size is used to calculate the server-to-client shared memory size.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetValidServerToClientShmSize(std::size_t requested_buffer_size) -> std::size_t;

/*!
 * \brief           Gets a valid notification shared memory size.
 *
 * \return          Valid notification shared memory size.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetValidNotificationShmSize() -> std::size_t;

/*!
 * \brief           Validates a client-to-server shared memory size.
 *
 * \details         Considers management data in the shared memory.
 *
 * \param[in]       shared_memory_size   Size of the client-to-server shared memory.
 *
 * \return          True if the client-to-server shared memory size is valid, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ValidateClientToServerShmSize(std::size_t shared_memory_size) -> bool;

/*!
 * \brief           Validates a server-to-client shared memory size.
 *
 * \details         Considers management data in the shared memory.
 *
 * \param[in]       shared_memory_size   Size of the server-to-client shared memory.
 *
 * \return          True if the server-to-client shared memory size is valid, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ValidateServerToClientShmSize(std::size_t shared_memory_size) -> bool;

/*!
 * \brief           Validates a notification shared memory size.
 *
 * \param[in]       shared_memory_size   Size of the notification shared memory.
 *
 * \return          True if the notification shared memory size is valid, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ValidateNotificationShmSize(std::size_t shared_memory_size) -> bool;

/*!
 * \brief    Common header that is added by SafeIPC to the beginning of each IPC message.
 */
class CommonHeader {
 public:
  /*!
   * \brief    Size of the common header in bytes.
   */
  static constexpr std::size_t kSize{sizeof(std::uint8_t) + sizeof(std::uint8_t) + sizeof(std::uint32_t) +
                                     sizeof(std::uint32_t) + sizeof(std::uint16_t)};

  /*!
   * \brief    Format of an IPC message.
   */
  enum class MessageFormat : std::uint8_t {
    /*!
     * \brief    Format A: The message consists of a common header followed by IPC data.
     */
    kFormatA = 1,
    /*!
     * \brief    Format B: The message consists of a common header followed by a format B header followed by IPC data.
     */
    kFormatB = 2
  };

  // VECTOR NC AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_InvertedCheckValue
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_OSA_A4.7.1_InvertedCheckValue
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_CtorInvalidatedMembers
  /*!
   * \brief           Creates a CommonHeader object for an IPC message.
   *
   * \details         The created header is not initially valid because message size, message size check value and
   *                  message sequence number fields of the header are not initialized to valid values. These fields
   *                  have to be initialized to valid values after object creation with SetMessageSize() and
   *                  SetMessageSeqNum().
   *
   * \param[in]       message_format    Format of the IPC message.
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
   *  - Partially initialize the fields.
   * \endinternal
   */
  explicit CommonHeader(MessageFormat message_format) noexcept
      : message_format_{static_cast<std::uint8_t>(message_format)},
        message_format_check_value_{static_cast<std::uint8_t>(~static_cast<std::uint8_t>(message_format))} {}

  /*!
   * \brief           Creates an invalid CommonHeader object for an IPC message.
   *
   * \details         The created header is not valid because all fields of the header are not initialized to valid
   *                  values.
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
   *  - Default initialize the fields.
   * \endinternal
   */
  CommonHeader() noexcept : message_format_{}, message_format_check_value_{} {}

  /*!
   * \brief           Creates a CommonHeader object from a received common header.
   *
   * \details         The common message header is deserialized and its contents stored in this object.
   *
   * \param[in]       data_view   View on the received data that represents the common header.
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
   *  - Deserialize the received common header and initialize all fields.
   * \endinternal
   */
  explicit CommonHeader(::amsr::core::Span<std::uint8_t const, kSize> data_view) noexcept
      : message_format_{}, message_format_check_value_{} {
    std::size_t offset{0};

    message_format_ = data_view[offset];
    offset += sizeof(message_format_);

    message_format_check_value_ = data_view[offset];
    offset += sizeof(message_format_check_value_);

    ::amsr::core::Span<std::uint8_t const> sub_span{data_view.subspan(offset, sizeof(message_size_))};
    std::memcpy(&message_size_, sub_span.data(), sub_span.size());  // VCA_OSA_STL_MEMCPY
    offset += sizeof(message_size_);

    sub_span = data_view.subspan(offset, sizeof(message_size_check_value_));
    std::memcpy(&message_size_check_value_, sub_span.data(), sub_span.size());  // VCA_OSA_STL_MEMCPY
    offset += sizeof(message_size_check_value_);

    sub_span = data_view.subspan(offset, sizeof(message_seq_num_));
    std::memcpy(&message_seq_num_, sub_span.data(), sub_span.size());  // VCA_OSA_STL_MEMCPY
  }

  /*!
   * \brief           Serializes a CommonHeader object into a data buffer.
   *
   * \param[out]      data_view   View on the memory where the contents of the CommonHeader shall be serialized to.
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
   *  - Serialize the variables of this object into the provided data buffer.
   * \endinternal
   */
  void Serialize(::amsr::core::Span<std::uint8_t, kSize> data_view) const noexcept {
    std::size_t offset{0};

    data_view[offset] = message_format_;
    offset += sizeof(message_format_);

    data_view[offset] = message_format_check_value_;  // VCA_OSA_THIS_DEREF, VCA_OSA_EXT_CALL_PASS_VALUE
    offset += sizeof(message_format_check_value_);

    ::amsr::core::Span<std::uint8_t> sub_span{data_view.subspan(offset, sizeof(message_size_))};
    std::memcpy(sub_span.data(), &message_size_, sizeof(message_size_));  // VCA_OSA_STL_MEMCPY
    offset += sizeof(message_size_);

    sub_span = data_view.subspan(offset, sizeof(message_size_check_value_));
    // VCA_OSA_STL_MEMCPY
    std::memcpy(sub_span.data(), &message_size_check_value_, sizeof(message_size_check_value_));
    offset += sizeof(message_size_check_value_);

    sub_span = data_view.subspan(offset, sizeof(message_seq_num_));
    // VCA_OSA_STL_MEMCPY
    std::memcpy(sub_span.data(), &message_seq_num_, sizeof(message_seq_num_));
  };

  /*!
   * \brief           Gets the message format of this header.
   *
   * \return          Message format of this header.
   *
   * \context         ANY
   *
   * \pre             The message format of this header is valid.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetMessageFormat() const noexcept -> MessageFormat {
    // VECTOR Next Line AutosarC++17_10-A7.2.1: MD_OSA_A7.2.1_value_enum_range
    return static_cast<MessageFormat>(message_format_);
  }

  /*!
   * \brief           Gets the message size of this header.
   *
   * \return          Message size of this header.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetMessageSize() const noexcept -> std::uint32_t { return message_size_; }

  /*!
   * \brief           Sets the message size and message size check value fields in this header.
   *
   * \param[in]       message_size   Size of the IPC message.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void SetMessageSize(std::uint32_t message_size) noexcept {
    message_size_ = message_size;
    message_size_check_value_ = ~message_size;
  }

  /*!
   * \brief           Sets the message sequence number field in this header.
   *
   * \param[in]       message_seq_num   Sequence number of the IPC message.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void SetMessageSeqNum(std::uint16_t message_seq_num) noexcept { message_seq_num_ = message_seq_num; }

  /*!
   * \brief           Validates if the fields of this CommonHeader are valid.
   *
   * \details         Should be used to check if a received message header is corrupted or not.
   *
   * \param[in]       expected_message_seq_num   IPC message sequence number that the common header is expected to have.
   *
   * \return          Empty value if the message header is valid, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   The message header is not valid.
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
   *  - Validate that
   *    - The message format is equal to one of the known formats.
   *    - The message format check value is the bitwise inverse of the message format.
   *    - The message size check value is the bitwise inverse of the message size value.
   *    - The message sequence number is equal to the expected sequence number.
   *  - If the validation succeeded
   *    - Return an empty value.
   *  - Otherwise return an kProtocol error.
   * \endinternal
   */
  auto Validate(std::uint32_t expected_message_seq_num) const noexcept -> ::amsr::core::Result<void> {
    ::amsr::core::Result<void> result{};

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_InvertedCheckValue
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_OSA_A4.7.1_InvertedCheckValue
    std::uint8_t const inverted_message_format{static_cast<std::uint8_t>(~message_format_)};
    std::uint32_t const inverted_message_size{static_cast<std::uint32_t>(~message_size_)};
    if (((message_format_ != static_cast<std::uint8_t>(MessageFormat::kFormatA)) &&
         (message_format_ != static_cast<std::uint8_t>(MessageFormat::kFormatB))) ||
        (inverted_message_format != message_format_check_value_) ||
        (inverted_message_size != message_size_check_value_) || (message_seq_num_ != expected_message_seq_num)) {
      result.EmplaceError(osabstraction::OsabErrc::kProtocolError, "IPC message header corrupted (common header).");
    }

    return result;
  }

 private:
  /*!
   * \brief    Message format field of the common header.
   * \details  Specifies the format of the message.
   */
  std::uint8_t message_format_;

  /*!
   * \brief    Message format check value field of the common header.
   * \details  Should be the bitwise inverse of the message format field. Used to detect shared memory corruptions that
   *           corrupted the message format field of the common header.
   */
  std::uint8_t message_format_check_value_;

  /*!
   * \brief    IPC message size field of the common header.
   * \details  Specifies the size of the following IPC message (in bytes).
   */
  std::uint32_t message_size_{};

  /*!
   * \brief    IPC message size check value field of the common header.
   * \details  Should be the bitwise inverse of the message size field. Used to detect shared memory corruptions that
   *           corrupted the message size field of the common header.
   */
  std::uint32_t message_size_check_value_{};

  /*!
   * \brief    Sequence number of the IPC message.
   * \details  Sequence numbers of IPC messages start a 1, are incremented by 1 for each following IPC message and
   *           wrap around to 0 on std::uint16_t overflow. The sequence numbers of each communication direction
   *           (client-to-server and server-to-client) are independent of each other. The sequence numbers are used to
   *           detect corruptions of the head or tail index of the ring buffer.
   */
  std::uint16_t message_seq_num_{};
};

}  // namespace stream_ipc_protocol
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_COMMON_H_
