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
 *        \brief  IPC Message Builder.
 *
 *      \details  Builder for IPC Messages.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_MESSAGE_BUILDER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/error_code.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {

/*!
 * \brief Builder for IPC Messages.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class IpcMessageBuilder final {
 public:
  /*! Alias to the weak ptr of a ConnectionSkeleton */
  using ConnectionSkeletonWeakPtr =
      amsr::WeakPtr<amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface>;

  IpcMessageBuilder() noexcept = delete;
  IpcMessageBuilder(IpcMessageBuilder const&) noexcept = delete;
  auto operator=(IpcMessageBuilder const&) noexcept -> IpcMessageBuilder& = delete;
  IpcMessageBuilder(IpcMessageBuilder&&) noexcept = delete;
  auto operator=(IpcMessageBuilder&&) noexcept -> IpcMessageBuilder& = delete;

  /*!
   * \brief   Destroy the message builder.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \context ANY
   * \steady  FALSE
   */
  ~IpcMessageBuilder() noexcept = default;

  /*!
   * \brief Creates an error response message header from the given RequestMessageHeader and ReturnCode.
   *
   * \param[in] message_header  The request message header from the message that encountered an error.
   * \param[in] return_code     The corresponding return code.
   *
   * \return The response error message header created from the request message and the error code.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateErrorResponseIpcPacketHeader(RequestMessageHeader const& message_header,
                                                 ReturnCode const return_code) noexcept -> ErrorResponseMessageHeader;

  /*!
   * \brief Creates an error response message from a RequestMessageHeader and ReturnCode.
   *
   * \param[in] request_header  The request message header.
   * \param[in] return_code     The error response return code to use.
   *
   * \return The newly created error response message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateErrorResponseIpcPacket(RequestMessageHeader const& request_header, ReturnCode return_code) noexcept
      -> ErrorResponseMessage;

  /*!
   * \brief Creates an error response message from a ErrorResponseMessageHeader.
   *
   * \param[in] error_header  The error response header.
   *
   * \return The newly created error response message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateErrorResponseMessage(ErrorResponseMessageHeader const& error_header) noexcept
      -> ErrorResponseMessage;

  /*!
   * \brief Creates a subscribe message from the given SubscribeEventMessageHeader.
   *
   * \param[in] subscribe_header  Subscribe event message header.
   *
   * \return The newly created subscribe message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateSubscribeEventMessage(SubscribeEventMessageHeader const& subscribe_header) noexcept
      -> SubscribeEventMessage;

  /*!
   * \brief Creates an unsubscribe message from the given UnsubscribeEventMessageHeader.
   *
   * \param[in] unsubscribe_header  Unsubscribe event message header.
   *
   * \return The newly created unsubscribe message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateUnsubscribeEventMessage(UnsubscribeEventMessageHeader const& unsubscribe_header) noexcept
      -> UnsubscribeEventMessage;

  /*!
   * \brief Creates a subscribe ACK message from the given SubscribeEventAckMessageHeader.
   *
   * \param[in] subscribe_ack_header  Subscribe event ACK message header.
   *
   * \return The newly created subscribe ACK message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateSubscribeEventAckMessage(SubscribeEventAckMessageHeader const& subscribe_ack_header) noexcept
      -> SubscribeEventAckMessage;

  /*!
   * \brief Creates a subscribe NACK message from the given SubscribeEventNAckMessageHeader.
   *
   * \param[in] subscribe_nack_header  Subscribe event NACK message header.
   *
   * \return The newly created subscribe NACK message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateSubscribeEventNAckMessage(SubscribeEventNAckMessageHeader const& subscribe_nack_header) noexcept
      -> SubscribeEventNAckMessage;

  /*!
   * \brief Creates a SubscribeEventAckHeader from the given SubscribeEventMessageHeader.
   *
   * \param[in] request_header  The SubscribeEventMessageHeader.
   *
   * \return The created ACK header.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateSubscribeEventAckHeader(SubscribeEventMessageHeader const& request_header) noexcept
      -> SubscribeEventAckMessageHeader;

  /*!
   * \brief Creates a SubscribeEventNAckHeader from the given SubscribeEventMessageHeader.
   *
   * \param[in] request_header  The subscribe event message header.
   *
   * \return The created NACK header.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateSubscribeEventNAckHeader(SubscribeEventMessageHeader const& request_header) noexcept
      -> SubscribeEventNAckMessageHeader;

  /*!
   * \brief Creates an ApplicationErrorMessage from the given RequestMessageHeader and ErrorCode.
   *
   * \param[in] header      The RequestMessageHeader.
   * \param[in] error_code  The corresponding error code.
   *
   * \return The created Application error message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static auto CreateApplicationErrorMessage(RequestMessageHeader const& header,
                                            ::amsr::core::ErrorCode const& error_code) noexcept
      -> ApplicationErrorMessage;

 private:
  /*!
   * \brief Creates an IPC error response message from the given ErrorResponseMessageHeader.
   *
   * \param[in] header  The error response message header.
   *
   * \return The created error response message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateMessage(ErrorResponseMessageHeader const& header) noexcept -> ErrorResponseMessage;

  /*!
   * \brief Creates an IPC subscribe event message from the given SubscribeEventMessageHeader.
   *
   * \param[in] header  The subscribe event message header.
   *
   * \return The created subscribe event message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateMessage(SubscribeEventMessageHeader const& header) noexcept -> SubscribeEventMessage;
  /*!
   * \brief Creates an IPC unsubscribe event message from the given UnsubscribeEventMessageHeader.
   *
   * \param[in] header  The unsubscribe event message header.
   *
   * \return The created unsubscribe event message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateMessage(UnsubscribeEventMessageHeader const& header) noexcept -> UnsubscribeEventMessage;

  /*!
   * \brief Creates an IPC subscribe event ACK message from the given SubscribeEventAckMessageHeader.
   *
   * \param[in] header  The subscribe event ACK message header.
   *
   * \return The created subscribe event ACK message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateMessage(SubscribeEventAckMessageHeader const& header) noexcept -> SubscribeEventAckMessage;

  /*!
   * \brief Creates an IPC subscribe event NACK message from the given SubscribeEventNAckMessageHeader.
   *
   * \param[in] header  The subscribe event NACK message header.
   *
   * \return The created subscribe event NACK message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateMessage(SubscribeEventNAckMessageHeader const& header) noexcept -> SubscribeEventNAckMessage;
};

}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_MESSAGE_BUILDER_H_
