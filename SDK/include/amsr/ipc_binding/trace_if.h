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
 *        \brief  Definition of IPC binding trace interface and parameters.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_IF_H_
#define LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_IF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ipc_binding_core/internal/trace/trace_if.h"

namespace amsr {
namespace ipc_binding {

/*!
 * \brief   Type definition for IPC binding trace interface.
 * \details By implementing this interface a custom trace interface can be provided via
 *          amsr::ipc_binding::SetTraceImpl().
 *
 * \vpublic
 */
using TraceIf = amsr::ipc_binding_core::internal::trace::TraceIf;

/*!
 * \brief The direction of the traced message.
 *
 * \vpublic
 */
using MessageDirection = amsr::ipc_binding_core::internal::trace::MessageDirection;

/*!
 * \brief Type definition for IPC binding method call container.
 *
 * \vpublic
 */
using IpcMethodCall = amsr::ipc_binding_core::internal::trace::IpcMethodCall;

/*!
 * \brief Type definition for IPC binding method no return call container.
 *
 * \vpublic
 */
using IpcMethodNoReturnCall = amsr::ipc_binding_core::internal::trace::IpcMethodNoReturnCall;

/*!
 * \brief Type definition for IPC binding method response container.
 *
 * \vpublic
 */
using IpcMethodResponse = amsr::ipc_binding_core::internal::trace::IpcMethodResponse;

/*!
 * \brief Type definition for IPC binding method error response container.
 *
 * \vpublic
 */
using IpcMethodErrorResponse = amsr::ipc_binding_core::internal::trace::IpcMethodErrorResponse;

/*!
 * \brief Type definition for IPC binding notification container.
 *
 * \vpublic
 */
using IpcNotification = amsr::ipc_binding_core::internal::trace::IpcNotification;

/*!
 * \brief Type definition for IPC binding application error container.
 *
 * \vpublic
 */
using IpcApplicationError = amsr::ipc_binding_core::internal::trace::IpcApplicationError;

/*!
 * \brief Type definition for IPC binding subscribe event container.
 *
 * \vpublic
 */
using IpcSubscribeEvent = amsr::ipc_binding_core::internal::trace::IpcSubscribeEvent;

/*!
 * \brief Type definition for IPC binding unsubscribe event container.
 *
 * \vpublic
 */
using IpcUnsubscribeEvent = amsr::ipc_binding_core::internal::trace::IpcUnsubscribeEvent;

/*!
 * \brief Type definition for IPC binding subscribe event acknowledge container.
 *
 * \vpublic
 */
using IpcSubscribeEventAck = amsr::ipc_binding_core::internal::trace::IpcSubscribeEventAck;

/*!
 * \brief Type definition for IPC binding subscribe event not acknowledge container.
 *
 * \vpublic
 */
using IpcSubscribeEventNack = amsr::ipc_binding_core::internal::trace::IpcSubscribeEventNack;

/*!
 * \brief Type definition for IPC binding subscribe event acknowledge container.
 *
 * \vpublic
 */
using IpcSubscribeEventAck = amsr::ipc_binding_core::internal::trace::IpcSubscribeEventAck;

/*!
 * \brief Type definition for IPC binding subscribe event acknowledge container.
 *
 * \vpublic
 */
using IpcSubscribeEventAck = amsr::ipc_binding_core::internal::trace::IpcSubscribeEventAck;

}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_IF_H_
