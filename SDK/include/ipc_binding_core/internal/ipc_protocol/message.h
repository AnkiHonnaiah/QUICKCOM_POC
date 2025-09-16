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
/**        \file
 *        \brief  Defines the message types used for IPC message communication.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_MESSAGE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_packet.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

namespace connection_manager {

/*! Forward declaration of ConnectionSkeletonInterface */
class ConnectionSkeletonInterface;

}  // namespace connection_manager

namespace ipc_protocol {

/*!
 * \brief Converts an enum value to a value of its underlying type.
 *
 * \tparam Enum  The type of the enum.
 *
 * \param[in] enum_value  Enum value.
 *
 * \return Value of the enum converted to its underlying type.
 */
template <typename Enum>
auto ToUnderlyingTypes(Enum enum_value) noexcept -> std::underlying_type_t<Enum> {
  return static_cast<std::underlying_type_t<Enum>>(enum_value);
}

/*!
 * \brief IPC message types.
 */
enum class MessageType : std::uint32_t {
  kRequest = 0x00000000U,
  kRequestNoReturn = 0x00000001U,
  kResponse = 0x00000002U,
  kErrorResponse = 0x00000003U,
  kApplicationError = 0x00000004U,
  kNotification = 0x00000005U,
  kSubscribeEvent = 0x80000003U,
  kSubscribeEventAck = 0x80000004U,
  kSubscribeEventNAck = 0x80000005U,
  kUnsubscribeEvent = 0x80000006U,
  kUnsubscribeEventAck = 0x80000007U,
  kUnsubscribeEventNAck = 0x80000008U,
};

/*!
 * \brief IPC return codes for method request errors.
 */
enum class ReturnCode : std::uint32_t {
  kUnknownServiceId = 0x00000000U,
  kUnknownInstanceId = 0x00000001U,
  kUnknownMethodId = 0x00000002U,
  kMalformedMessage = 0x00000003U,
  kServiceNotAvailable = 0x00000004U,
  kMethodRequestSchedulingFailed = 0x00000005U
};

/*!
 * \brief Type definition for the IPC protocol version.
 */
using ProtocolVersion = std::uint32_t;

/*!
 * \brief Type definition for the IPC message type.
 */
using MessageTypeField = std::underlying_type_t<MessageType>;

/*!
 * \brief Type definition for the IPC message length.
 */
using MessageLength = std::uint32_t;

/*!
 * \brief   IPC protocol version.
 * \details Protocol version history:
 *          - Version 1: Initial protocol version.
 *          - Version 2: Removed ApplicationErrorMessageHeader::error_code_ due R19-03 migration of ApApplicationError.
 *          - Version 3: Added kServiceNotAvailable return code.
 */
static constexpr ProtocolVersion kProtocolVersion{0x00000003U};

/*!
 * \brief   Generic header for the IPC protocol which every IPC message should start with, followed by the specific
 *          message header.
 * \details A IPC message always looks as follow:
 *          <pre>
 *          +-----------------------------+
 *          |   Generic header            |
 *          +-----------------------------+
 *          |   Specific message header   |
 *          +-----------------------------+
 *          |   Payload                   |
 *          +-----------------------------+
 *          </pre>
 */
struct ProtocolMessageHeader {
  /*!
   * \brief A IPC protocol version.
   */
  ProtocolVersion protocol_version_;

  /*!
   * \brief A IPC message type.
   */
  MessageType message_type_;

  /*!
   * \brief Contains the length of a IPC message payload in bytes (generic header not included, size is specific message
   *        header + payload).
   */
  MessageLength message_length_;
};

/*!
 * \brief Length (in bytes) of the generic IPC protocol header.
 */
static constexpr MessageLength kProtocolMessageHeaderLength{12U};

/*!
 * \brief Specific message header for IPC message of type 'Request'.
 */
struct RequestMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the method.
   */
  MethodId method_id_;
  /*!
   * \brief Identifier for the client.
   */
  ClientId client_id_;
  /*!
   * \brief Identifier for the session.
   */
  SessionId session_id_;
};

/*!
 * \brief Length of a request message header.
 */
static constexpr MessageLength kRequestMessageHeaderLength{24U};

/*!
 * \brief Length of a notification message header.
 */
static constexpr MessageLength kNotificationMessageHeaderLength{20U};

/*!
 * \brief Specific message header for IPC message of type 'RequestNoReturn'.
 */
using RequestNoReturnMessageHeader = RequestMessageHeader;

/*!
 * \brief Length of a fire & forget message header.
 */
static constexpr MessageLength kRequestNoReturnMessageHeaderLength{kRequestMessageHeaderLength};

/*!
 * \brief Specific message header for IPC message of type 'Response'.
 */
using ResponseMessageHeader = RequestMessageHeader;

/*!
 * \brief Length of a response message header.
 */
static constexpr MessageLength kResponseMessageHeaderLength{kRequestMessageHeaderLength};

/*!
 * \brief Length of a error response message header.
 */
static constexpr MessageLength kErrorResponseMessageHeaderLength{28U};

/*!
 * \brief Length of a application error message header.
 */
static constexpr MessageLength kApplicationErrorMessageHeaderLength{24U};

/*!
 * \brief Length of a subscribe event message header.
 */
static constexpr MessageLength kSubscribeEventMessageHeaderLength{20U};

/*!
 * \brief Length of a subscribe event Ack message header.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_IPCBINDING_AutosarC++17_10-M3.4.1_global
static constexpr MessageLength kSubscribeEventAckMessageHeaderLength{20U};

/*!
 * \brief Length of a subscribe event NAck message header.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_IPCBINDING_AutosarC++17_10-M3.4.1_global
static constexpr MessageLength kSubscribeEventNAckMessageHeaderLength{20U};

/*!
 * \brief Length of an unsubscribe event message header.
 */
static constexpr MessageLength kUnSubscribeEventMessageHeaderLength{20U};

/*!
 * \brief Specific message header for IPC message of type 'ErrorResponse'.
 */
struct ErrorResponseMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the method.
   */
  MethodId method_id_;
  /*!
   * \brief Identifier for the client.
   */
  ClientId client_id_;
  /*!
   * \brief Identifier for the session.
   */
  SessionId session_id_;
  /*!
   * \brief Identifier for the return code.
   */
  ReturnCode return_code_;
};

/*!
 * \brief Specific message header for IPC message of type 'ApplicationError'.
 */
struct ApplicationErrorMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the method.
   */
  MethodId method_id_;
  /*!
   * \brief Identifier for the client.
   */
  ClientId client_id_;
  /*!
   * \brief Identifier for the session.
   */
  SessionId session_id_;
};

/*!
 * \brief Specific message header for IPC message of type 'Notification'.
 */
struct NotificationMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the event.
   */
  EventId event_id_;
  /*!
   * \brief Identifier for the session.
   */
  SessionId session_id_;
};

/*!
 * \brief Specific message header for IPC message of type 'SubscribeEvent'.
 */
struct SubscribeEventMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the event.
   */
  EventId event_id_;
  /*!
   * \brief Identifier for the client that wants to subscribe.
   */
  ClientId client_id_;
};

/*!
 * \brief Specific message header for IPC message of type 'SubscribeEventAck'.
 */
struct SubscribeEventAckMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the event.
   */
  EventId event_id_;
  /*!
   * \brief Identifier for the client that gets the ack after a subscribe request.
   */
  ClientId client_id_;
};

/*!
 * \brief Specific message header for IPC message of type 'SubscribeEventNack'.
 */
struct SubscribeEventNAckMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the event.
   */
  EventId event_id_;
  /*!
   * \brief Identifier for the client that gets the nack after a subscribe request.
   */
  ClientId client_id_;
};

/*!
 * \brief Specific message header for IPC message of type 'UnsubscribeEvent'.
 */
struct UnsubscribeEventMessageHeader {
  /*!
   * \brief Identifier for the service.
   */
  ServiceId service_id_;
  /*!
   * \brief Identifier for the instance.
   */
  InstanceId instance_id_;
  /*!
   * \brief The major version for the service instance.
   */
  MajorVersion major_version_;
  /*!
   * \brief Identifier for the event.
   */
  EventId event_id_;
  /*!
   * \brief Identifier for the client that wants to unsubscribe.
   */
  ClientId client_id_;
};

/*!
 * \brief Request message.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class RequestMessage {
 public:
  /*!
   * \brief Construct a RequestMessage.
   *
   * \param[in]     request_header  The header for this request to be stored within this message object.
   * \param[in,out] packet          Serialized IPC packet (incl. protocol header | message-specific header | payload).
   *                                Packet payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  RequestMessage(RequestMessageHeader const& request_header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-Construct a RequestMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  RequestMessage(RequestMessage const&) noexcept = default;

  /*!
   * \brief Move-Construct a RequestMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  RequestMessage(RequestMessage&&) noexcept = default;

  auto operator=(RequestMessage const&) noexcept -> RequestMessage& = delete;
  auto operator=(RequestMessage&&) noexcept -> RequestMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~RequestMessage() = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Returns the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> RequestMessageHeader const&;

  /*!
   * \brief Returns the serialized IPC request message packet.
   *
   * \return The serialized request message packet (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The request message header as an attribute.
   */
  RequestMessageHeader const message_header_;

  /*!
   * \brief Complete request message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief Error response message.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class ErrorResponseMessage {
 public:
  /*!
   * \brief Construct a ErrorResponseMessage.
   *
   * \param[in]     response_header  The header for this response error to be stored within this message object.
   * \param[in,out] packet           Serialized IPC packet (incl. protocol header | message-specific header | payload).
   *                                 Packet payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  explicit ErrorResponseMessage(ErrorResponseMessageHeader const& response_header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-Construct a ErrorResponseMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ErrorResponseMessage(ErrorResponseMessage const&) noexcept = default;

  /*!
   * \brief Move-Construct a ErrorResponseMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ErrorResponseMessage(ErrorResponseMessage&&) noexcept = default;

  auto operator=(ErrorResponseMessage const&) noexcept -> ErrorResponseMessage& = delete;
  auto operator=(ErrorResponseMessage&&) noexcept -> ErrorResponseMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ErrorResponseMessage() = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Return the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> ErrorResponseMessageHeader const&;

  /*!
   * \brief Transfers the ownership of the serialized packet.
   *
   * \return The serialized response message packet (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The error response message header.
   */
  ErrorResponseMessageHeader const message_header_;

  /*!
   * \brief Complete response message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief   Remote request message extends the RequestMessage with weak_ptr to the related ConnectionSkeleton from which
 *          the request was received.
 * \details This message is created by the skeleton when received a request from a proxy.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class RemoteRequestMessage final : public RequestMessage {
 public:
  /*! Alias to the weak ptr of a ConnectionSkeleton */
  using ConnectionSkeletonWeakPtr =
      amsr::WeakPtr<amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface>;

  /*!
   * \brief Construct a RemoteRequestMessage.
   *
   * \param[in]     request_header  The header for this request to be stored within this message object.
   * \param[in,out] packet          Serialized IPC packet (incl. protocol header | message-specific header | payload).
   *                                Packet payload must be valid (no empty shared pointer).
   * \param[in]     connection_ptr  A weak pointer to the target connection skeleton where the response to this message
   *                                should be routed.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RemoteRequestMessage(RequestMessageHeader const& request_header, IpcPacketShared packet,
                       ConnectionSkeletonWeakPtr connection_ptr) noexcept;

  /*!
   * \brief Copy-construct a RemoteRequestMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RemoteRequestMessage(RemoteRequestMessage const&) noexcept = default;

  /*!
   * \brief Move-construct a RemoteRequestMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RemoteRequestMessage(RemoteRequestMessage&&) noexcept = default;

  auto operator=(RemoteRequestMessage const&) noexcept -> RemoteRequestMessage& = delete;
  auto operator=(RemoteRequestMessage&&) noexcept -> RemoteRequestMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ~RemoteRequestMessage() final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Returns the weak_ptr to the target connection.
   *
   * \return A weak_ptr to the target connection.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetTargetConnection() const noexcept -> ConnectionSkeletonWeakPtr;

 private:
  /*! Weak pointer to the connection skeleton where this message should be routed. */
  ConnectionSkeletonWeakPtr connection_ptr_{};
};

/*!
 * \brief Request-no-return message structure for the PacketRouter and PacketSinks.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class RequestNoReturnMessage final {
 public:
  /*!
   * \brief Construct a RequestNoReturnMessage.
   *
   * \param[in]     header  The header for this request-no-return to be stored within this message object.
   * \param[in,out] packet  Serialized IPC packet (incl. protocol header | message-specific header | payload). Packet
   *                        payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RequestNoReturnMessage(RequestNoReturnMessageHeader const& header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-Construct a RequestNoReturnMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RequestNoReturnMessage(RequestNoReturnMessage const&) noexcept = default;

  /*!
   * \brief Move-Construct a RequestNoReturnMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RequestNoReturnMessage(RequestNoReturnMessage&&) noexcept = default;

  auto operator=(RequestNoReturnMessage const&) noexcept -> RequestNoReturnMessage& = delete;
  auto operator=(RequestNoReturnMessage&&) noexcept -> RequestNoReturnMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ~RequestNoReturnMessage() = default;

  /*!
   * \brief Returns the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> RequestNoReturnMessageHeader const&;

  /*!
   * \brief Returns the serialized IPC request message packet.
   *
   * \return The serialized request message packet (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The request message header.
   */
  RequestNoReturnMessageHeader const message_header_;

  /*!
   * \brief Complete request message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief Response message structure for the PacketRouter and PacketSinks.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class ResponseMessage {
 public:
  /*!
   * \brief Construct a ResponseMessage.
   *
   * \param[in]     response_header  The header for this response to be stored within this message object.
   * \param[in,out] packet           Serialized IPC packet (incl. protocol header | message-specific header | payload).
   *                                 Packet payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ResponseMessage(ResponseMessageHeader const& response_header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-Construct a ResponseMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ResponseMessage(ResponseMessage const&) noexcept = default;

  /*!
   * \brief Move-Construct a ResponseMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ResponseMessage(ResponseMessage&&) noexcept = default;

  auto operator=(ResponseMessage const&) noexcept -> ResponseMessage& = delete;
  auto operator=(ResponseMessage&&) noexcept -> ResponseMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ResponseMessage() = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Returns the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> ResponseMessageHeader const&;

  /*!
   * \brief Returns the serialized IPC notification message packet.
   *
   * \return The serialized response message packet (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The response message header.
   */
  ResponseMessageHeader const message_header_;

  /*!
   * \brief Complete response message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief Application error message.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class ApplicationErrorMessage {
 public:
  /*!
   * \brief Construct an ApplicationErrorMessage.
   *
   * \param[in]     header  The header for this application error to be stored within this message object.
   * \param[in,out] packet  Serialized IPC packet (incl. protocol header | message-specific header | payload). Packet
   *                        payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ApplicationErrorMessage(ApplicationErrorMessageHeader const& header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-construct a ApplicationErrorMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ApplicationErrorMessage(ApplicationErrorMessage const&) noexcept = default;

  /*!
   * \brief Move-construct a ApplicationErrorMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCBINDING_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  ApplicationErrorMessage(ApplicationErrorMessage&&) noexcept = default;

  auto operator=(ApplicationErrorMessage const&) noexcept -> ApplicationErrorMessage& = delete;
  auto operator=(ApplicationErrorMessage&&) noexcept -> ApplicationErrorMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ApplicationErrorMessage() = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Returns the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> ApplicationErrorMessageHeader const&;

  /*!
   * \brief Transfers the ownership of the serialized packet.
   *
   * \return The serialized application error message (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The application error message header.
   */
  ApplicationErrorMessageHeader const message_header_;

  /*!
   * \brief Complete response message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief   Notification message.
 * \details Encapsulates a complete serialized notification message (including all headers) and meta data about the
 *          message (deserialized packet header).
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class NotificationMessage final {
 public:
  /*!
   * \brief Construct a NotificationMessage.
   *
   * \param[in]     notification_header  The header for this notification to be stored within this message object.
   * \param[in,out] packet               Serialized IPC packet (incl. protocol header | message-specific header |
   *                                     payload). Packet payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  NotificationMessage(NotificationMessageHeader const& notification_header, IpcPacketShared packet) noexcept;

  /*!
   * \brief Copy-construct a NotificationMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  NotificationMessage(NotificationMessage const&) noexcept = default;

  /*!
   * \brief Move-construct a NotificationMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  NotificationMessage(NotificationMessage&&) noexcept = default;

  /*!
   * \brief Move-assign a NotificationMessage.
   *
   * \return Reference to the move-assigned NotificationMessage.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator=(NotificationMessage&&) & noexcept -> NotificationMessage& = default;

  NotificationMessage() noexcept = delete;
  auto operator=(NotificationMessage const&) noexcept -> NotificationMessage& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  ~NotificationMessage() noexcept = default;

  /*!
   * \brief Return the message header.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> NotificationMessageHeader const&;

  /*!
   * \brief Returns the serialized IPC notification message packet.
   *
   * \return The serialized notification message packet (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetPacket() noexcept -> IpcPacketShared;  // NOLINT(vector-method-can-be-const) logical constness

 private:
  /*!
   * \brief The notification message header.
   */
  NotificationMessageHeader message_header_;

  /*!
   * \brief Complete notification message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief SubscribeEventMessage message structure for the PacketRouter and provided PacketSinks that receive this
 *        subscription messages from the client.
 *
 * \tparam HeaderType  All the subscribe (ack/nack) messages have the same layout.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
template <typename HeaderType>
class SubscribeEventMessageTemplate final {
 public:
  /*!
   * \brief Compile-time check to limit the template usage.
   */
  static_assert(std::is_same<HeaderType, SubscribeEventMessageHeader>::value ||
                    std::is_same<HeaderType, SubscribeEventAckMessageHeader>::value ||
                    std::is_same<HeaderType, SubscribeEventNAckMessageHeader>::value ||
                    std::is_same<HeaderType, UnsubscribeEventMessageHeader>::value,
                "The header for the event subscription must be one of these types: SubscribeEventMessageHeader, "
                "SubscribeEventAckMessageHeader, SubscribeEventNackMessageHeader, UnsubscribeEventMessageHeader.");

  /*!
   * \brief Construct a SubscribeEventMessage.
   *
   * \param[in]     message_header  The header for this subscribe request to be stored within this message object.
   * \param[in,out] packet          Serialized IPC packet (incl. protocol header | message-specific header | payload).
   *                                Packet payload must be valid (no empty shared pointer).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  SubscribeEventMessageTemplate(HeaderType const& message_header, IpcPacketShared packet)
      : message_header_{message_header}, packet_{std::move(packet)} {  // VCA_IPCB_SHARED_REF_POINTER_MOVE
    if (!packet_) {
      ::amsr::core::Abort("Tried to construct SubscribeEventMessageTemplate with an empty IpcPacketShared packet.");
    }
  }

  /*!
   * \brief Copy-Construct SubscribeEventMessageTemplate.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  SubscribeEventMessageTemplate(SubscribeEventMessageTemplate const&) noexcept = default;

  /*!
   * \brief Move-Construct SubscribeEventMessageTemplate.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  SubscribeEventMessageTemplate(SubscribeEventMessageTemplate&&) noexcept = default;

  auto operator=(SubscribeEventMessageTemplate const&) noexcept -> SubscribeEventMessageTemplate& = delete;
  auto operator=(SubscribeEventMessageTemplate&&) noexcept -> SubscribeEventMessageTemplate& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ~SubscribeEventMessageTemplate() = default;

  /*!
   * \brief Returns the message header of this subscription (ack/nack) message.
   *
   * \return The message-specific header as a const-ref.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMessageHeader() const noexcept -> HeaderType const& {
    return message_header_;
  }  // COV_IpcBinding_FunctionCallCoverage_unused_utility_function

  /*!
   * \brief Transfers the ownership of the serialized packet.
   *
   * \return The serialized subscribe event message (incl. protocol header | message-specific header | payload).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10-M9.3.3_const_method_getter
  auto GetPacket() noexcept -> IpcPacketShared { return packet_; }  // NOLINT(vector-method-can-be-const) logical const

 private:
  /*!
   * \brief The subscribe event message header stored within this message for easy access.
   */
  HeaderType const message_header_;

  /*!
   * \brief Complete subscription message as serialized byte stream.
   */
  IpcPacketShared packet_;
};

/*!
 * \brief Subscribe event message.
 */
using SubscribeEventMessage = SubscribeEventMessageTemplate<SubscribeEventMessageHeader>;

/*!
 * \brief Subscribe event ACK message.
 */
using SubscribeEventAckMessage = SubscribeEventMessageTemplate<SubscribeEventAckMessageHeader>;

/*!
 * \brief Subscribe event NACK message.
 */
using SubscribeEventNAckMessage = SubscribeEventMessageTemplate<SubscribeEventNAckMessageHeader>;

/*!
 * \brief Unsubscribe event message.
 */
using UnsubscribeEventMessage = SubscribeEventMessageTemplate<UnsubscribeEventMessageHeader>;

}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_MESSAGE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_FunctionCallCoverage_unused_utility_function
//   \ACCEPT  XX
//   \REASON  The API is existing to provide a complete basic utility class.
//            A coverage gap is reported for the API which is never directly used by other units.
//            The API is verified with unit tests. Therefore no further component or multi unit tests are necessary.
// COV_JUSTIFICATION_END
