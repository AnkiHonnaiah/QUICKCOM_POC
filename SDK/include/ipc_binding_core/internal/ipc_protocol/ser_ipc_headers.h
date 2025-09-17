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
 *        \brief  Provides serializers for the message headers of IPC.
 *        \unit IpcBinding::IpcBindingCore::IpcProtocol
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_SER_IPC_HEADERS_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_SER_IPC_HEADERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "someip-protocol/internal/serialization/writer.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {
namespace serialization {

/*!
 * \brief Shortening for namespace amsr::someip_protocol::internal::serialization.
 */
namespace someip_protocol = ::amsr::someip_protocol::internal::serialization;

/*!
 * \brief Serialization helper function for serializing the IPC request message header.
 *
 * \param[out] writer          Writer holding the buffer to which data is serialized.
 * \param[in]  header          The IPC request message header containing the information for serialization.
 * \param[in]  payload_length  The length of the payload which comes after the header.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeRequestMessageHeader(someip_protocol::Writer& writer,
                                   ::amsr::ipc_binding_core::internal::ipc_protocol::RequestMessageHeader const& header,
                                   std::size_t const payload_length) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC request no return message header.
 *
 * \param[out] writer          Writer holding the buffer to which data is serialized.
 * \param[in]  header          The IPC request no return message header containing the information for serialization.
 * \param[in]  payload_length  The length of the payload which comes after the header.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeRequestNoReturnMessageHeader(
    someip_protocol::Writer& writer,
    ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessageHeader const& header,
    std::size_t payload_length) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC response message header.
 *
 * \param[out] writer          Writer holding the buffer to which data is serialized. The writer must be big enough to
 *                             store all header fields.
 * \param[in]  header          The IPC response message header.
 * \param[in]  payload_length  The length of the payload which comes after the header. May not exceed the maximum
 *                             allowed length, i.e. payload length <= (std::numeric_limits<MessageLength>::max() -
 *                             kResponseMessageHeaderLength).
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeResponseMessageHeader(
    someip_protocol::Writer& writer,
    ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessageHeader const& header,
    std::size_t payload_length) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC error response message header.
 *
 * \param[out] writer  Writer holding the buffer to which data is serialized.
 * \param[in]  header  The IPC error response message header.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeErrorResponseMessageHeader(someip_protocol::Writer& writer,
                                         ErrorResponseMessageHeader const& header) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC subscription messages.
 *
 * \param[out] writer  Writer holding the buffer to which data is serialized.
 * \param[in]  header  Specific message header for IPC message of type SubscribeEvent.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeSubscribeEventMessageHeader(someip_protocol::Writer& writer,
                                          SubscribeEventMessageHeader const& header) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC unsubscription messages.
 *
 * \param[out] writer  Writer holding the buffer to which data is serialized.
 * \param[in]  header  Specific message header for IPC message of type UnsubscribeEvent.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeUnsubscribeEventMessageHeader(someip_protocol::Writer& writer,
                                            UnsubscribeEventMessageHeader const& header) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC subscription Ack messages.
 *
 * \param[out] writer  Writer holding the buffer to which data is serialized.
 * \param[in]  header  Specific message header for IPC message of type SubscribeEventAck.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeSubscribeEventAckMessageHeader(someip_protocol::Writer& writer,
                                             SubscribeEventAckMessageHeader const& header) noexcept;

/*!
 * \brief Serialization helper function for serializing the IPC subscription NAck messages.
 *
 * \param[out] writer  Writer holding the buffer to which data is serialized.
 * \param[in]  header  Specific message header for IPC message of type SubscribeEventNack.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeSubscribeEventNAckMessageHeader(someip_protocol::Writer& writer,
                                              SubscribeEventNAckMessageHeader const& header) noexcept;

/*!
 * \brief Serialization function for serializing the IPC application error message header.
 *
 * \param[out] writer          Writer holding the buffer to which data is serialized.
 * \param[in]  header          The IPC application error message header containing the information for serialization.
 * \param[in]  payload_length  The length of the payload which comes after the header.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void SerializeApplicationErrorMessageHeader(
    someip_protocol::Writer& writer,
    ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessageHeader const& header,
    std::size_t payload_length) noexcept;

/*!
 * \brief Serialization function for serializing the IPC notification message header.
 *
 * \param[out] writer          Writer holding the buffer to which data is serialized.
 * \param[in]  header          The IPC notification message header containing the information for serialization.
 * \param[in]  payload_length  The length of the payload which comes after the header.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 *
 * \spec requires true; \endspec
 */
void SerializeNotificationMessageHeader(
    someip_protocol::Writer& writer,
    ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessageHeader const& header,
    std::size_t payload_length) noexcept;

}  // namespace serialization
}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_SER_IPC_HEADERS_H_
