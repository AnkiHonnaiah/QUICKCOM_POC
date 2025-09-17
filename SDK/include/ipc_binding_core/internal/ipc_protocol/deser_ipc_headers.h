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
 *        \brief  Provides deserializers for the message headers of IPC.
 *        \unit IpcBinding::IpcBindingCore::IpcProtocol
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_DESER_IPC_HEADERS_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_DESER_IPC_HEADERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "someip-protocol/internal/deserialization/reader.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {
namespace deserialization {

/*!
 * \brief Reads out primitive data types from a buffer view.
 */
using Reader = ::amsr::someip_protocol::internal::deserialization::Reader;

/*!
 * \brief Buffer view to wrap the data to inspect.
 */
using BufferView = ::amsr::someip_protocol::internal::deserialization::BufferView;

/*!
 * \brief Deserializes the IPC 'protocol message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \spec requires true; \endspec
 */
auto DeserializeProtocolMessageHeader(Reader& reader) -> ::amsr::core::Result<ProtocolMessageHeader>;

/*!
 * \brief Deserializes the IPC 'notification message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeNotificationMessageHeader(Reader& reader) -> ::amsr::core::Result<NotificationMessageHeader>;

/*!
 * \brief Deserializes the IPC 'request message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeRequestMessageHeader(Reader& reader) -> ::amsr::core::Result<RequestMessageHeader>;

/*!
 * \brief Deserializes the IPC 'request no return message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeRequestNoReturnMessageHeader(Reader& reader) -> ::amsr::core::Result<RequestNoReturnMessageHeader>;

/*!
 * \brief Deserializes the IPC 'response message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeResponseMessageHeader(Reader& reader) -> ::amsr::core::Result<ResponseMessageHeader>;

/*!
 * \brief Deserializes the IPC 'error response message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeErrorResponseMessageHeader(Reader& reader) -> ::amsr::core::Result<ErrorResponseMessageHeader>;

/*!
 * \brief Deserializes the IPC 'application error message' header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeApplicationErrorMessageHeader(Reader& reader) -> ::amsr::core::Result<ApplicationErrorMessageHeader>;

/*!
 * \brief Deserializes the IPC SubscribeEvent header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeSubscribeEventMessageHeader(Reader& reader) -> ::amsr::core::Result<SubscribeEventMessageHeader>;

/*!
 * \brief Deserializes the IPC UnsubscribeEvent header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeUnsubscribeEventMessageHeader(Reader& reader) -> ::amsr::core::Result<UnsubscribeEventMessageHeader>;

/*!
 * \brief Deserializes the IPC SubscribeEventAck header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeSubscribeEventAckMessageHeader(Reader& reader) -> ::amsr::core::Result<SubscribeEventAckMessageHeader>;

/*!
 * \brief Deserializes the IPC SubscribeEventNack header from the given byte stream.
 *
 * \param[in,out] reader  Reader holding the view on the serialized data.
 *
 * \return Result storing the deserialized header or an errorcode.
 *
 * \error IpcBindingErrc::deserialization_error  Deserialization of header failed.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto DeserializeSubscribeEventNAckMessageHeader(Reader& reader)
    -> ::amsr::core::Result<SubscribeEventNAckMessageHeader>;

}  // namespace deserialization
}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_DESER_IPC_HEADERS_H_
