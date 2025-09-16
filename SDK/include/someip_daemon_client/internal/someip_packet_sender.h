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
/*!        \file  someip_daemon_client/internal/someip_packet_sender.h
 *        \brief  A class responsible for sending messages.
 *        \details  This class sends messages regardless of their types (routing/control), to do that it makes use of
 *                  the passed IPC connection wrapper object.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_PACKET_SENDER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_PACKET_SENDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief A packet sender class that makes use of the IPC wrapper to provide functionalities for sending all different
 * types of messages between application and SOME/IP daemon.
 * \tparam TemplateConfiguration The type of a class used for message forwarding and event notification.
 */
template <typename TemplateConfiguration>
class SomeIpPacketSender {
 public:
  /*!
   * \brief Type alias for used IPC connection handler.
   */
  using MessageSender = typename TemplateConfiguration::MessageSenderType;

  /*!
   * \brief Constructor that initializes the someip packet sender.
   * \param[in] message_sender Reference to command connection.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit SomeIpPacketSender(MessageSender& message_sender) noexcept : message_sender_{message_sender} {}

  /*!
   * \brief Delete default constructor.
   */
  SomeIpPacketSender() = delete;

  /*!
   * \brief Delete copy constructor.
   */
  SomeIpPacketSender(SomeIpPacketSender const&) = delete;

  /*!
   * \brief Delete move constructor.
   */
  SomeIpPacketSender(SomeIpPacketSender&&) = delete;

  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(SomeIpPacketSender const&) & -> SomeIpPacketSender& = delete;

  /*!
   * \brief Delete move assignment.
   */
  auto operator=(SomeIpPacketSender&&) & -> SomeIpPacketSender& = delete;

  /*!
   * \brief Destructor.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  virtual ~SomeIpPacketSender() noexcept = default;

  /*!
   * \brief Initiates the transmission of a SomeIp routing message.
   * \details The method may return before the given message has been transmitted.
   *          Outgoing routing messages might be queued.
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] packet A memory buffer containing a SomeIp routing message.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected, sending in disconnected state.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   *        queue is full.
   * \pre -
   * \context App
   * \threadsafe  TRUE
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Lock mutex.
   * - Check if the packet is valid.
   *   - Serialize the ipc headers for the SomeIp routing packet.
   *   - Send the SomeIp routing packet.
   * - Return the result.
   * \endinternal
   */
  auto operator()(amsr::someip_protocol::internal::InstanceId const& instance_id,
                  vac::memory::allocator::MemoryBufferPtr packet) noexcept -> amsr::core::Result<void> {
    // PTP-B-SomeipDaemonClient-SomeIpPacketSender_SendSomeIpMessage
    std::lock_guard<std::mutex> const lock_guard{lock_};

    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(SomeIpDaemonClientErrc::kPacketNull)};
    if (packet != nullptr) {
      // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
      std::size_t const packet_size{packet->GetView(0U).size()};
      serializer_.Serialize(instance_id, packet_size);

      logger_.LogDebug(
          [&instance_id, &packet_size](ara::log::LogStream& s) {
            s << "Sending SOMEIP packet with Instance ID: 0x" << instance_id
              << " and memory buffer length: " << packet_size;
          },
          {__func__}, {__LINE__});

      // VCA_SDCL_PASSING_REFERENCE, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
      result = message_sender_.SendMessage(serializer_.GetIpcMessageHeader(), std::move(packet));
    }
    // PTP-E-SomeipDaemonClient-SomeIpPacketSender_SendSomeIpMessage
    return result;
  }

 private:
  /*!
   * \brief SomeIpPacketSender logger.
   */
  // VCA_SDCL_STATIC_STRINGS
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_STATIC_STRINGS
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"SomeIpPacketSender"}};

  /*!
   * \brief Reference to the command connection used to send out command requests to the SOME/IP daemon. The command
   * connection forwards responses synchronously.
   */
  MessageSender& message_sender_;

  /*!
   * \brief SomeIpPacketSender lock.
   */
  std::mutex lock_{};

  /*!
   * \brief An instance of the IpcPacketSerializer class.
   */
  amsr::someipd_app_protocol::internal::IpcPacketSerializer serializer_{
      amsr::someipd_app_protocol::internal::MessageType::kSomeIp};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_PACKET_SENDER_H_

/*!
 * \exclusivearea ::amsr::someip_daemon_client::internal::SomeIpPacketSender::lock_
 *                Ensures that there is no concurrent read/write to the message_sender_ instance and to the memory
 *                buffers for the someip packet sender
 *
 * \protects      ::amsr::someip_daemon_client::internal::SomeIpPacketSender::serializer_
 *                ::amsr::someip_daemon_client::internal::SomeIpPacketSender::message_sender_
 *
 * \usedin        ::amsr::someip_daemon_client::internal::SomeIpPacketSender::operator()
 *
 * \length        LONG
 *                Serializing routing message and sending the massage to the message_sender_
 * \endexclusivearea
 */
