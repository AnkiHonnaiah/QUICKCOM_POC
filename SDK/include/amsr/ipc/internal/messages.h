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
 *        \brief  Defines messages exchanged during connection establishment.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_MESSAGES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_MESSAGES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include "amsr/core/array.h"
#include "amsr/core/span.h"
#include "osabstraction/io/internal/io_buffer_queue.h"
#include "osabstraction/io/io_buffer.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Manages an IO Buffer array of an IPC message.
 *
 * \tparam  IoBufferType    Type of IO buffer array that shall be managed and returned by GetRemainingBuffer().
 *                          Valid types: osabstraction::io::MutableIOBuffer or osabstraction::io::ConstIOBuffer.
 * \tparam  IoBufferCount   Number of IO buffers that the IO buffer array shall contain.
 *
 * \unit    osabstraction::ipc::IpcUtils_LinuxPikeOs
 */
template <class IoBufferType, std::size_t IoBufferCount>
class Message {
 public:
  static_assert(std::is_same<IoBufferType, osabstraction::io::MutableIOBuffer>::value ||
                    std::is_same<IoBufferType, osabstraction::io::ConstIOBuffer>::value,
                "IoBufferType of amsr::ipc::internal::Message template has to be either "
                "osabstraction::io::MutableIOBuffer or osabstraction::io::ConstIOBuffer");

  /*!
   * \brief           Creates a Message object that manages an IO buffer array.
   *
   * \param[in]       managed_io_buffer_array   IO buffer array that shall be managed.
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
   * - Save the passed IO buffer array and initialize the IoBufferQueue used to manage the buffers.
   * \endinternal
   */
  explicit Message(::amsr::core::Array<IoBufferType, IoBufferCount> managed_io_buffer_array) noexcept
      : io_buffers_{managed_io_buffer_array}, buffer_queue_{::amsr::core::MakeSpan(io_buffers_)} {}

  /*!
   * \brief           Destroy the Message object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual ~Message() noexcept = default;

  Message(Message const&) noexcept = delete;
  Message& operator=(Message const&) & noexcept = delete;
  Message(Message&&) noexcept = delete;
  Message& operator=(Message&&) & noexcept = delete;

  /*!
   * \brief           Get a view on the remaining, unprocessed data.
   *
   * \return          Buffer view on the remaining data that is not yet marked processed with Consume().
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
   * - Get the remaining unprocessed IO buffers from the buffer queue and save them in a temporary buffer array.
   * - Return a view of the temporary buffer array.
   * \endinternal
   */
  auto GetRemainingBuffer() noexcept -> ::amsr::core::Span<IoBufferType> {
    std::size_t const io_buffer_count{buffer_queue_.GetMemory(::amsr::core::MakeSpan(temp_io_buffers_))};
    return ::amsr::core::Span<IoBufferType>{temp_io_buffers_.data(), io_buffer_count};
  }

  /*!
   * \brief           Mark a part of the managed data as processed.
   *
   * \param[in]       num_bytes   Number of bytes to mark as processed. The first num_bytes unprocessed bytes are marked
   *                              as processed.
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
   * - Consume the next num_bytes of the buffer queue.
   * \endinternal
   */
  void Consume(std::size_t num_bytes) noexcept { buffer_queue_.Consume(num_bytes); }

  /*!
   * \brief           Check if all managed data is marked as processed.
   *
   * \return          true if all data was marked as processed, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto CompletelyProcessed() const noexcept -> bool { return buffer_queue_.CompletelyProcessed(); }

 private:
  /*!
   * \brief   IO buffers managed by this Message.
   */
  ::amsr::core::Array<IoBufferType, IoBufferCount> io_buffers_{};

  /*!
   * \brief   Temporary IO buffer array used for buffer_queue_.GetMemory().
   */
  ::amsr::core::Array<IoBufferType, IoBufferCount> temp_io_buffers_{};

  /*!
   * \brief   IO buffer queue that manages the IO buffers.
   */
  osabstraction::io::internal::IoBufferQueue<IoBufferType> buffer_queue_{};
};

/*!
 * \brief Strong type to store IPC Protocol version.
 */
struct IpcProtocolVersion {
  /*!
   * \brief Internal value.
   */
  std::uint16_t value;
};

/*!
 * \brief Strong type to store SHM Identifier.
 */
struct ShmId {
  /*!
   * \brief Internal value.
   */
  std::uint64_t value;
};

/*!
 * \brief   Provides serialization / deserialization for a ClientToServerMessage1 IPC message.
 *
 * \details Vectored IO is used to serialize / deserialize messages. This class manages the IO Buffer array needed for
 *          this vectored IO.
 *
 * \tparam  IoBufferType    Type of IO buffer array that shall be managed and returned by GetRemainingBuffer().
 *                          Valid types: osabstraction::io::MutableIOBuffer or osabstraction::io::ConstIOBuffer.
 *
 * \unit    osabstraction::ipc::IpcUtils_LinuxPikeOs
 */
template <class IoBufferType>
class ClientToServerMessage1 final : public Message<IoBufferType, 4> {
 public:
  // VECTOR Next Construct  VectorC++-V3.9.2: MD_OSA_V3.9.2_SafeIpc_ClientToServerMessage1
  /*!
   * \brief           Creates a ClientToServerMessage1 object that manages an IO buffer array.
   *
   * \details         The passed parameters should point to the memory locations where the values of the fields that are
   *                  part of a ClientToServerMessage1 are or should be saved.
   *
   * \param[in,out]   ipc_protocol              IPC protocol field of the ClientToServerMessage1.
   * \param[in,out]   client_to_server_shm_id   Client-to-Server-SHM ID field of the ClientToServerMessage1.
   * \param[in,out]   notification_shm_id       Notification-SHM ID field of the ClientToServerMessage1.
   * \param[in,out]   s2c_buffer_size_hint      Hint on size (in bytes) of buffer for server-to-client IPC messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit ClientToServerMessage1(IpcProtocolVersion& ipc_protocol, ShmId& client_to_server_shm_id,
                                  ShmId& notification_shm_id, std::uint64_t& s2c_buffer_size_hint) noexcept
      : Message<IoBufferType, 4>{::amsr::core::Array<IoBufferType, 4>{
            {IoBufferType{&ipc_protocol.value, sizeof(ipc_protocol.value)},
             IoBufferType{&client_to_server_shm_id.value, sizeof(client_to_server_shm_id.value)},
             IoBufferType{&notification_shm_id.value, sizeof(notification_shm_id.value)},
             IoBufferType{&s2c_buffer_size_hint, sizeof(s2c_buffer_size_hint)}}}} {}

  ClientToServerMessage1(ClientToServerMessage1 const&) noexcept = delete;
  ClientToServerMessage1& operator=(ClientToServerMessage1 const&) & noexcept = delete;
  ClientToServerMessage1(ClientToServerMessage1&&) noexcept = delete;
  ClientToServerMessage1& operator=(ClientToServerMessage1&&) & noexcept = delete;

  /*!
   * \brief           Destroy the ClientToServerMessage1 object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ClientToServerMessage1() noexcept final = default;
};

/*!
 * \brief   Provides serialization / deserialization for a ServerToClientMessage1 IPC message.
 *
 * \details Vectored IO is used to serialize / deserialize messages. This class manages the IO Buffer array needed for
 *          this vectored IO.
 *
 * \tparam  IoBufferType    Type of IO buffer array that shall be managed and returned by GetRemainingBuffer().
 *                          Valid types: osabstraction::io::MutableIOBuffer or osabstraction::io::ConstIOBuffer.
 *
 * \unit    osabstraction::ipc::IpcUtils_LinuxPikeOs
 */
template <class IoBufferType>
class ServerToClientMessage1 final : public Message<IoBufferType, 1> {
 public:
  /*!
   * \brief           Creates a ServerToClientMessage1 object that manages an IO buffer array.
   *
   * \details         The passed parameters should point to the memory locations where the values of the fields that are
   *                  part of a ServerToClientMessage1 are or should be saved.
   *
   * \param[in,out]   server_to_client_shm_id   Server-to-Client-SHM ID field of the ServerToClientMessage1.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit ServerToClientMessage1(ShmId& server_to_client_shm_id) noexcept
      : Message<IoBufferType, 1>{::amsr::core::Array<IoBufferType, 1>{
            {IoBufferType{&server_to_client_shm_id.value, sizeof(server_to_client_shm_id.value)}}}} {}

  ServerToClientMessage1(ServerToClientMessage1 const&) noexcept = delete;
  ServerToClientMessage1& operator=(ServerToClientMessage1 const&) & noexcept = delete;
  ServerToClientMessage1(ServerToClientMessage1&&) noexcept = delete;
  ServerToClientMessage1& operator=(ServerToClientMessage1&&) & noexcept = delete;

  /*!
   * \brief           Destroy the ServerToClientMessage1 object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ServerToClientMessage1() noexcept final = default;
};

/*!
 * \brief   Provides serialization / deserialization for a ClientToServerMessage2 IPC message.
 *
 * \details Vectored IO is used to serialize / deserialize messages. This class manages the IO Buffer array needed for
 *          this vectored IO.
 *
 * \tparam  IoBufferType    Type of IO buffer array that shall be managed and returned by GetRemainingBuffer().
 *                          Valid types: osabstraction::io::MutableIOBuffer or osabstraction::io::ConstIOBuffer.
 *
 * \unit    osabstraction::ipc::IpcUtils_LinuxPikeOs
 */
template <class IoBufferType>
class ClientToServerMessage2 final : public Message<IoBufferType, 1> {
 public:
  /*!
   * \brief           Creates a ClientToServerMessage2 object that manages an IO buffer array.
   *
   * \details         As a ClientToServerMessage2 transports no data and is only used as notification, it contains only
   *                  a dummy byte.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ClientToServerMessage2() noexcept
      : Message<IoBufferType, 1>{
            ::amsr::core::Array<IoBufferType, 1>{{IoBufferType{&dummy_byte_, sizeof(dummy_byte_)}}}} {}

  ClientToServerMessage2(ClientToServerMessage2 const&) noexcept = delete;
  ClientToServerMessage2& operator=(ClientToServerMessage2 const&) & noexcept = delete;
  ClientToServerMessage2(ClientToServerMessage2&&) noexcept = delete;
  ClientToServerMessage2& operator=(ClientToServerMessage2&&) & noexcept = delete;

  /*!
   * \brief           Destroy the ClientToServerMessage2 object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ClientToServerMessage2() noexcept final = default;

 private:
  /*!
   * \brief   Dummy byte to transfer in a ClientToServerMessage2. The value of it is not important.
   */
  std::uint8_t dummy_byte_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_MESSAGES_H_
