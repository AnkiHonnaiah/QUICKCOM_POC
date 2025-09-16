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
/**        \file  application_connection.h
 *        \brief  Represent a "BasicIPC" connection between SOME/IP daemon and an application.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstdint>
#include <deque>
#include <memory>
#include <type_traits>
#include "ara/core/vector.h"

#include "amsr/ipc/connection.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/span.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/language/unique_function.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Manage connection, message reception and transmission using "BasicIPC".
 */
class ApplicationConnection final {
 public:
  /*!
   * \brief Deleted default constructor.
   * \steady FALSE
   */
  ApplicationConnection() = delete;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  ~ApplicationConnection() = default;

  ApplicationConnection(ApplicationConnection const&) = delete;
  ApplicationConnection(ApplicationConnection&&) = delete;
  auto operator=(ApplicationConnection const&) & -> ApplicationConnection& = delete;
  auto operator=(ApplicationConnection&&) & -> ApplicationConnection& = delete;

  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Represents a PDU message.
   */
  using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;

  /*!
   * \brief A type alias for the used connection.
   */
  using Connection = amsr::ipc::Connection;

  /*!
   * \brief Callback function to use when disconnection happens.
   */
  using DisconnectionFunction = vac::language::UniqueFunction<void()>;

  /*!
   * \brief Callback function to use when a new message is received.
   */
  using ReceptionFunction =
      vac::language::UniqueFunction<void(amsr::someipd_app_protocol::internal::MessageType const&, SpecificHeaderView&,
                                         vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>)>;

  /*!
   * \brief Constructor.
   *
   * \param[in] connection Handle to an IPC connection provided from application manager.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  explicit ApplicationConnection(Connection&& connection) noexcept;

  /*!
   * \brief Register callback functions for disconnection and reception and start message reception.
   *
   * \param[in] disconnection_function Function to be called when disconnection happens.
   * \param[in] reception_function Function to be called when a message is received.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void StartReceive(DisconnectionFunction&& disconnection_function, ReceptionFunction&& reception_function) noexcept;

  /*!
   * \brief Send SOME/IP message related to routing.
   *
   * \details The method may return before the given message has been transmitted.
   *          Outgoing routing messages might be queued.
   *
   * \param[in] specific_header_buffer Contains specific header buffer.
   * \param[in] packet A shared pointer to SOME/IP message.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   *
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  bool SendSomeIpMessage(SpecificHeaderView specific_header_buffer, Packet packet) noexcept;

  /*!
   * \brief Send routing Pdu message.
   * \details The method may return before the given message has been transmitted.
   *         Outgoing routing messages might be queued.
   * \param[in] specific_header_buffer contains specific header buffer.
   * \param[in] packet A shared pointer to PDU message.
   * \return true if the operation has been successful and false otherwise.
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  bool SendPduMessage(SpecificHeaderView specific_header_buffer, PduPacket packet) noexcept;

  /*!
   * \brief Send the given message (other than SOME/IP).
   *
   * \param[in] message_type The type of control message.
   * \param[in] specific_header_buffer Specific header buffer.
   * \param[in,out] request_memory_buffer A memory buffer containing the message's payload.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool SendMessage(
      amsr::someipd_app_protocol::internal::MessageType message_type, SpecificHeaderView specific_header_buffer,
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> request_memory_buffer) noexcept;

  /*!
   * \brief Send initial field notification message.
   *
   * \details The method may return before the given message has been transmitted.
   *          Outgoing routing messages might be queued.
   *
   * \param[in] specific_header_buffer Contains specific header buffer.
   * \param[in] packet A shared pointer to SOME/IP message.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  bool SendInitialFieldNotificationMessage(SpecificHeaderView specific_header_buffer, Packet packet) noexcept;

  /*!
   * \brief Return the current connection's state.
   *
   * \return The current connection's state.
   *
   * \pre -
   * \context   Timer
   * \reentrant FALSE
   * \steady    FALSE
   */
  ConnectionState GetConnectionState() const noexcept;

  /*!
   * \brief Return the connection's peer identity.
   *
   * \return The connection's peer identity.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  ara::core::Result<Credentials> GetPeerIdentity() const noexcept;

  /*!
   * \brief Return the connection's peer process id.
   *
   * \return The connection's peer process id.
   *
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  osabstraction::process::ProcessId GetPeerProcessId() const noexcept;

 protected:
  /*!
   * \brief Specific buffer type.
   */
  using SpecificBuffer = std::array<std::uint8_t, amsr::someipd_app_protocol::internal::kSpecificMessageHeaderLength>;

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief The type of an entry in the transmit queue.
   */
  class TransmissionQueueEntry {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Constructor.
     *
     * \param[in] message_type The type of the message.
     * \param[in] specific_header_buffer Contains the specific buffer.
     * \param[in] packet A shared pointer to a SOME/IP message.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    TransmissionQueueEntry(amsr::someipd_app_protocol::internal::MessageType const message_type,
                           amsr::UniquePtr<SpecificBuffer> specific_header_buffer, Packet packet) noexcept;

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Constructor.
     *
     * \param[in] message_type The type of the message.
     * \param[in] specific_header_buffer Contains the specific buffer.
     * \param[in] packet A shared pointer to a PDU message.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    TransmissionQueueEntry(amsr::someipd_app_protocol::internal::MessageType const message_type,
                           amsr::UniquePtr<SpecificBuffer> specific_header_buffer, PduPacket packet) noexcept;

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Constructor.
     *
     * \param[in] message_type A SOME/IP service instance identifier.
     * \param[in] specific_header_buffer Contains the specific buffer.
     * \param[in,out] memory_buffer A memory buffer containing the message's payload.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    FALSE
     */
    TransmissionQueueEntry(
        amsr::someipd_app_protocol::internal::MessageType const message_type,
        amsr::UniquePtr<SpecificBuffer> specific_header_buffer,
        vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) noexcept;

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Indicates whether the entry contains a routing SOME/IP message or not.
     *
     * \return "true" if the entry is routing SOME/IP message and "false" otherwise.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    bool IsRoutingSomeIpMessage() const noexcept;

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Indicates whether the entry contains a routing PDU message.
     * \return true if the entry routing PDU message and false otherwise.
     * \pre -
     * \context   App, Network
     * \steady    TRUE
     */
    bool IsRoutingPduMessage() const noexcept;

    /*!
     * \brief Indicates whether the entry contains a routing SOME/IP message with meta data or not.
     * \return "true" if the entry is routing SOME/IP message with meta data and "false" otherwise.
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    bool IsRoutingSomeIpMessageWithMetaData() const noexcept;

    /*!
     * \brief Indicates whether the entry contains a routing PDU message with meta data.
     * \return true if the entry routing PDU message with meta data and false otherwise.
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    bool IsRoutingPduMessageWithMetaData() const noexcept;

    /*!
     * \brief Indicates whether the entry contains a routing initial field notification message or not.
     *
     * \return "true" if the entry is routing initial field notification message and "false" otherwise.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    bool IsRoutingInitialFieldNotificationMessage() const noexcept;

    /*!
     * \brief Gives access to the message type
     * \return The message type
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    amsr::someipd_app_protocol::internal::MessageType GetMessageType() const noexcept { return message_type_; }

    /*!
     * \brief Gives access to the specific header buffer
     * \return A const reference to the specific header buffer
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    SpecificBuffer const& GetSpecificHeaderBuffer() const noexcept;

    /*!
     * \brief Gives access to the packet buffer
     * \return A const pointer to the packet buffer
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer> const* GetPacketBuffer() const noexcept;

    /*!
     * \brief Gives access to the packet timestamp
     * \return The packet timestamp
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    ara::core::Optional<amsr::someip_protocol::internal::TimeStamp> const& GetPacketTimeStamp() const noexcept;

    /*!
     * \brief Gives access to the PDU packet buffer
     * \return A const pointer to the PDU packet buffer
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer> const* GetPduPacketBuffer() const noexcept;
    /*!
     * \brief Gives access to the PDU packet timestamp
     * \return The PDU packet timestamp
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    ara::core::Optional<amsr::someip_protocol::internal::TimeStamp> const& GetPduPacketTimeStamp() const noexcept;
    /*!
     * \brief Gives access to the memory buffer
     * \return A const pointer to the memory buffer
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer> const* GetMemoryBuffer() const noexcept;

   private:
    /*!
     * \brief Constructor.
     *
     * \param[in] message_type The type of the message.
     * \param[in] specific_header_buffer Contains the specific buffer.
     * \param[in] packet A shared pointer to a SOME/IP message.
     * \param[in] pdu_packet A shared pointer to a PDU message.
     * \param[in,out] memory_buffer A memory buffer containing the message's payload.
     *
     * \pre -
     * \context   App, Network
     * \reentrant FALSE
     * \steady    TRUE
     */
    TransmissionQueueEntry(
        amsr::someipd_app_protocol::internal::MessageType message_type,
        amsr::UniquePtr<SpecificBuffer> specific_header_buffer, Packet packet, PduPacket pdu_packet,
        vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) noexcept;

    /*!
     * \brief Contains the specific buffer.
     */
    amsr::UniquePtr<SpecificBuffer> specific_header_buffer_;

    /*!
     * \brief Message type.
     */
    amsr::someipd_app_protocol::internal::MessageType message_type_;

    /*!
     * \brief Contains the message's payload.
     */
    vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer_;

    /*!
     * \brief A shared pointer to SOME/IP message.
     */
    Packet packet_;

    /*!
     * \brief A shared pointer to PDU message.
     */
    PduPacket pdu_packet_;
  };

  /*!
   * \brief The container type for outgoing messages.
   */
  using TransmissionQueue = std::deque<TransmissionQueueEntry, ara::core::PolymorphicAllocator<TransmissionQueueEntry>>;

 private:
  /*!
   * \brief Called by the IPC system to notify of a new incoming message.
   *
   * \param[in] message_length The length of the pending message.
   *
   * \return A reference to the memory buffer where the next incoming message is to be stored.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  ara::core::Span<osabstraction::io::MutableIOBuffer> OnMessageAvailable(std::size_t message_length) noexcept;

  /*!
   * \brief Called by the IPC system to notify of the completion of a message reception.
   *
   * \details If message reception is not successfully completed, an error message is logged and
   *          disconnection occurs.
   *
   * \param[in] received_length The length of the received message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void OnMessageReceived(std::size_t received_length) noexcept;

  /*!
   * \brief Remove the latest sent message from the transmit queue and start the transmission of
   *        the next one if that exists.
   *
   * \details Called by the IPC system to notify about the completion of a message transmission.
   *          If message transmission is not successfully completed, an error message is logged
   *          and disconnection occurs.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void OnSendCompleted() noexcept;

  /*!
   * \brief Trigger an asynchronous reception.
   *
   * \details If message reception is not successfully completed, an error is logged and
   *          disconnection occurs. If message reception is successful, the next asynchronous
   *          reception is started immediately.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void ReceiveAsync() noexcept;

  /*!
   * \brief Close the connection between SOME/IP daemon and the application.
   *
   * \details After closing the connection, the state is set to "ConnectionState::kDisconnected".
   *          Upon the disconnection, the application is released in the "ApplicationManager"'s
   *          registry of the SOME/IP daemon and all offered service instances are stopped.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void Disconnect() noexcept;

  /*!
   * \brief The minimum body length of a received message to allocate when a full message cannot be allocated.
   */
  static constexpr std::size_t kReceiveMessageMinBodyLength{64U};

  /*!
   * \brief Underlying type of "MessageType".
   */
  using MessageTypeUnderlyingType = std::underlying_type<amsr::someipd_app_protocol::internal::MessageType>::type;

  /*!
   * \brief Type-alias for the mutable I/O memory buffer.
   */
  using MutableIOBuffer = osabstraction::io::MutableIOBuffer;

  /*!
   * \brief Type-alias for the mutable I/O memory buffer view.
   */
  using MutableIOBufferContainer = ara::core::Vector<MutableIOBuffer>;

  /*!
   * \brief Type-alias for the immutable I/O memory buffer.
   */
  using UnmutableIOBuffer = osabstraction::io::ConstIOBuffer;

  /*!
   * \brief Type-alias for the immutable I/O memory buffer view.
   */
  using UnmutableIOBufferContainer = ara::core::Vector<UnmutableIOBuffer>;

  /*!
   * \brief Processes the received IPC message.
   *
   * \param[in] received_length The length of the received message.
   * \param[in] memory_buffer A memory buffer containing the next received message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  void ProcessReceivedMessage(
      std::size_t received_length,
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) noexcept;

  /*!
   * \brief Record the transmission queue's size if it has either decreased to below the last recorded value
   * or increased by at least kTxqIncreaseLogThreshold compared to the last recorded value, in which case a
   * flood warning is also emitted.
   *
   * \steady TRUE
   */
  void MonitorTransmissionQueueSize() noexcept;

  /*!
   * \brief Enqueue a SOME/IP message for transmission.
   *
   * \param[in] specific_header_buffer Specific header buffer.
   * \param[in] message_type Type of routing message.
   * \param[in] packet A shared pointer to SOME/IP message.
   *
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  void EnqueueSomeipMessage(SpecificHeaderView specific_header_buffer,
                            amsr::someipd_app_protocol::internal::MessageType const message_type,
                            Packet packet) noexcept;

  /*!
   * \brief Enqueue a PDU message for transmission.
   * \param[in] specific_header_buffer Specific header buffer.
   * \param[in] packet A shared pointer to PDU message.
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  void EnqueuePduMessage(SpecificHeaderView specific_header_buffer, PduPacket packet) noexcept;

  /*!
   * \brief Enqueue a message other than SOME/IP for transmission.
   *
   * \param[in] specific_header_buffer Specific header buffer.
   * \param[in] message_type A type of control/routing message.
   * \param[in,out] memory_buffer A memory buffer containing a control message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  void EnqueueMessage(SpecificHeaderView specific_header_buffer,
                      amsr::someipd_app_protocol::internal::MessageType message_type,
                      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) noexcept;

  /*!
   * \brief Start transmission of the next message in the transmit queue.
   *
   * \return The result of sending the message.
   *
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  ara::core::Result<amsr::ipc::SendResult> TransmitNextMessage() noexcept;

  /*!
   * \brief Initiate the transmission of a routing message with meta data information inside.
   *
   * \param[in] message_type A type of routing message.
   * \param[in] specific_header_buffer Specific header buffer.
   * \param[in] memory_buffer A memory buffer containing a control message.
   * \param[in] time_stamp A time stamp for when the message was received by the daemon.
   *
   * \return The result of sending the message.
   *
   * \pre -
   * \context   App, Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  ara::core::Result<amsr::ipc::SendResult> TransmitMessage(
      amsr::someipd_app_protocol::internal::MessageType message_type, SpecificBuffer const& specific_header_buffer,
      vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer> const* memory_buffer,
      ara::core::Optional<amsr::someip_protocol::internal::TimeStamp> const& time_stamp) noexcept;

  /*!
   * \brief Set up a new memory buffer for the next incoming message.
   *
   * \param[in] message_length The total length of the next expected message.
   *
   * \return A reference to the memory region for the next incoming message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  ara::core::Span<osabstraction::io::MutableIOBuffer> PrepareReceiveMemoryBuffer(
      std::size_t const message_length) noexcept;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    ara::core::StringView{"ApplicationConnection"}};

  /*!
   * \brief A memory allocator for received messages.
   */
  someip_daemon_core::MemoryBufferAllocator receive_message_allocator_{};

  /*!
   * \brief An IPC connection.
   */
  ara::core::Optional<Connection> connection_{};

  /*!
   * \brief The connection's state.
   */
  ConnectionState connection_state_{ConnectionState::kConnected};

  /*!
   * \brief Queue of all messages pending for transmission.
   */
  TransmissionQueue transmit_queue_{};

  /*!
   * \brief Keep track of increasing transmission queue fill level.
   */
  std::size_t txq_log_counter_{0};

  /*!
   * \brief Transmission queue fill level increase alert threshold
   */
  static std::size_t constexpr kTxqIncreaseLogThreshold{100};

  /*!
   * \brief A container of I/O vectors for transmission.
   */
  UnmutableIOBufferContainer transmit_iovec_container_{};

  /*!
   * \brief A buffer for the generic message header of transmitted messages.
   */
  std::array<std::uint8_t, amsr::someipd_app_protocol::internal::kGenericMessageHeaderLength>
      transmit_generic_header_{};

  /*!
   * \brief A buffer for the generic message header of received messages.
   */
  std::array<std::uint8_t, amsr::someipd_app_protocol::internal::kGenericMessageHeaderLength> receive_generic_header_{};

  /*!
   * \brief A buffer for the generic message header of received messages.
   */
  std::array<std::uint8_t, amsr::someipd_app_protocol::internal::kSpecificMessageHeaderLength>
      receive_specific_header_{};

  /*!
   * \brief Contains the next received message.
   */
  vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> receive_memory_buffer_{};

  /*!
   * \brief A container of I/O vectors for reception.
   */
  MutableIOBufferContainer receive_iovec_container_{};

  /*!
   * \brief Callback function to use when disconnection occurs.
   */
  DisconnectionFunction disconnection_function_{};

  /*!
   * \brief Callback function to use when a new message is received.
   */
  ReceptionFunction reception_function_{};

  /*!
   * \brief Peer identity of the connected application.
   */
  ara::core::Result<Credentials> const peer_identity_;

  /*!
   * \brief Peer Process ID of the connected application.
   */
  ara::core::Result<osabstraction::process::ProcessId> const peer_process_id_;

  /*!
   * \brief An instance of the DeserMessageHeaders class.
   */
  amsr::someipd_app_protocol::internal::DeserMessageHeaders deserializer_{};

  /*!
   * \brief Friend test to access the private member transmit_queue_
   */
  FRIEND_TEST(UT__SomeIpDaemon__ApplicationConnection, application_connection_SendMultiplePduMessages_InConnectedState);
  /*!
   * \brief Friend test to access the private member transmit_queue_
   */
  FRIEND_TEST(UT__SomeIpDaemon__ApplicationConnection, application_connection_SendMultipleSomeIpMessages);
  /*!
   * \brief Friend test to access the private member transmit_queue_
   */
  FRIEND_TEST(UT__SomeIpDaemon__ApplicationConnection, application_connection_SendMultipleCommands_InConnectedState);
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CONNECTION_H_
