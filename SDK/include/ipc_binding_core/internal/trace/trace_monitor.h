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
 *        \brief  Wrapper for IPC binding specific TraceIf instances.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_MONITOR_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_MONITOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <utility>

#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/trace/trace_if.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace trace {

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief The trace monitor is a IPC-binding specific wrapper for tracing and forwarding the incoming and outgoing IPC
 *        messages to the registered trace interface implementation.
 *
 * \unit IpcBinding::IpcBindingCore::TraceMonitor
 */
class TraceMonitor final {
  /*!
   * \brief Defines the direction of the traced message.
   */
  using Direction = MessageDirection;

 public:
  /*!
   * \brief Initializes the trace monitor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceMonitor() noexcept {};  // NOLINT: qcc cannot produce a default noexcept ctor

  /*!
   * \brief Destroy the TraceMonitor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~TraceMonitor() noexcept {};  // NOLINT: qcc cannot produce a default noexcept dtor

  TraceMonitor(TraceMonitor const&) noexcept = delete;
  TraceMonitor(TraceMonitor&&) noexcept = delete;
  auto operator=(TraceMonitor const&) noexcept -> TraceMonitor& = delete;
  auto operator=(TraceMonitor&&) noexcept -> TraceMonitor& = delete;

  /*!
   * \brief Stores the given trace interface implementation, to which the traced IPC messages should be forwarded later.
   *
   * \param[in,out] trace_if  Pointer to an IPC specific trace implementation. Pass nullptr to unset the trace monitor.
   *
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void SetTraceIf(TraceIf* trace_if) {
    std::lock_guard<std::shared_timed_mutex> const lock{trace_if_lock_};
    trace_if_ = trace_if;
  }

  /*!
   * \brief   Forwards the outgoing IPC request message to the registered trace interface.
   * \details This function is called on transmission of a method request.
   *
   * \param[in] req_message  RequestMessage (header + payload) to be traced. Background: Passed as non-const reference
   *                         due to logical vs physical constness in regards to the RequestMessage GetPacket() API. To
   *                         the public TraceIf interface only a const reference to packet buffer is passed which cannot
   *                         be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendRequest(ipc_protocol::RequestMessage& req_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::RequestMessageHeader const& req_header{req_message.GetMessageHeader()};
      trace::IpcMethodCall const r{Direction::kTx,
                                   req_header.service_id_.value,
                                   req_header.instance_id_.value,
                                   req_header.major_version_.value,
                                   req_header.method_id_.value,
                                   req_header.client_id_.value,
                                   req_header.session_id_.value,
                                   *req_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC request message to the registered trace interface.
   * \details This function is called on reception of a method request.
   *
   * \param[in] req_message  RequestMessage (header + payload) to be traced. Background: Passed as non-const reference
   *                         due to logical vs physical constness in regards to the RequestMessage GetPacket() API. To
   *                         the public TraceIf interface only a const reference to packet buffer is passed which cannot
   *                         be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveRequest(ipc_protocol::RequestMessage& req_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::RequestMessageHeader const& req_header{req_message.GetMessageHeader()};
      trace::IpcMethodCall const r{Direction::kRx,
                                   req_header.service_id_.value,
                                   req_header.instance_id_.value,
                                   req_header.major_version_.value,
                                   req_header.method_id_.value,
                                   req_header.client_id_.value,
                                   req_header.session_id_.value,
                                   *req_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC request-no-return message to the registered trace interface.
   * \details This function is called on transmission of a fire and forget request.
   *
   * \param[in] req_message  RequestNoReturn message (header + payload) to be traced. Background: Passed as non-const
   *                         reference due to logical vs physical constness in regards to the RequestNoReturn
   *                         GetPacket() API. To the public TraceIf interface only a const reference to packet buffer is
   *                         passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendRequestNoReturn(ipc_protocol::RequestNoReturnMessage& req_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::RequestNoReturnMessageHeader const& req_header{req_message.GetMessageHeader()};
      trace::IpcMethodNoReturnCall const r{Direction::kTx,
                                           req_header.service_id_.value,
                                           req_header.instance_id_.value,
                                           req_header.major_version_.value,
                                           req_header.method_id_.value,
                                           req_header.client_id_.value,
                                           req_header.session_id_.value,
                                           *req_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC request-no-return message to the registered trace interface.
   * \details This function is called on reception of a fire and forget request.
   *
   * \param[in] req_message  RequestNoReturn message (header + payload) to be traced. Background: Passed as non-const
   *                         reference due to logical vs physical constness in regards to the RequestNoReturn
   *                         GetPacket() API. To the public TraceIf interface only a const reference to packet buffer is
   *                         passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveRequestNoReturn(ipc_protocol::RequestNoReturnMessage& req_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::RequestNoReturnMessageHeader const& req_header{req_message.GetMessageHeader()};
      trace::IpcMethodNoReturnCall const r{Direction::kRx,
                                           req_header.service_id_.value,
                                           req_header.instance_id_.value,
                                           req_header.major_version_.value,
                                           req_header.method_id_.value,
                                           req_header.client_id_.value,
                                           req_header.session_id_.value,
                                           *req_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC subscribe event message to the registered trace interface.
   * \details This function is called on transmission of a subscribe request.
   *
   * \param[in] subscribe_event_message_header  Subscribe message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendSubscribe(ipc_protocol::SubscribeEventMessageHeader const& subscribe_event_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEvent const r{Direction::kTx,
                                       subscribe_event_message_header.service_id_.value,
                                       subscribe_event_message_header.instance_id_.value,
                                       subscribe_event_message_header.major_version_.value,
                                       subscribe_event_message_header.event_id_.value,
                                       subscribe_event_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC subscribe event message to the registered trace interface.
   * \details This function is called on reception of a subscribe request.
   *
   * \param[in] subscribe_event_message_header  Subscribe message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveSubscribe(ipc_protocol::SubscribeEventMessageHeader const& subscribe_event_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEvent const r{Direction::kRx,
                                       subscribe_event_message_header.service_id_.value,
                                       subscribe_event_message_header.instance_id_.value,
                                       subscribe_event_message_header.major_version_.value,
                                       subscribe_event_message_header.event_id_.value,
                                       subscribe_event_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC unsubscribe event message to the registered trace interface.
   * \details This function is called on transmission of a unsubscribe request.
   *
   * \param[in] unsubscribe_event_message_header  Unsubscribe message header to be used as trace input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendUnsubscribe(ipc_protocol::UnsubscribeEventMessageHeader const& unsubscribe_event_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcUnsubscribeEvent const r{Direction::kTx,
                                         unsubscribe_event_message_header.service_id_.value,
                                         unsubscribe_event_message_header.instance_id_.value,
                                         unsubscribe_event_message_header.major_version_.value,
                                         unsubscribe_event_message_header.event_id_.value,
                                         unsubscribe_event_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC Unsubscribe event message to the registered trace interface.
   * \details This function is called on reception of a unsubscribe request.
   *
   * \param[in] unsubscribe_event_message_header  Unsubscribe message header to be used as trace input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveUnsubscribe(
      ipc_protocol::UnsubscribeEventMessageHeader const& unsubscribe_event_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcUnsubscribeEvent const r{Direction::kRx,
                                         unsubscribe_event_message_header.service_id_.value,
                                         unsubscribe_event_message_header.instance_id_.value,
                                         unsubscribe_event_message_header.major_version_.value,
                                         unsubscribe_event_message_header.event_id_.value,
                                         unsubscribe_event_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC response message to the registered trace interface.
   * \details This function is called on transmission of a method response.
   *
   * \param[in] resp_message  Response message to be used as trace method input parameters. Background: Passed as non-
   *                          const reference due to logical vs physical constness in regards to the ResponseMessage
   *                          GetPacket() API. To the public TraceIf interface only a const reference to packet buffer
   *                          is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendResponse(ipc_protocol::ResponseMessage& resp_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ResponseMessageHeader const& resp_header{resp_message.GetMessageHeader()};
      trace::IpcMethodResponse const r{Direction::kTx,
                                       resp_header.service_id_.value,
                                       resp_header.instance_id_.value,
                                       resp_header.major_version_.value,
                                       resp_header.method_id_.value,
                                       resp_header.client_id_.value,
                                       resp_header.session_id_.value,
                                       *resp_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC error response message to the registered trace interface.
   * \details This function is called on transmission of a method call error response.
   *
   * \param[in] error_resp_message  Error response message to be used as trace method input parameters. Background:
   *                                Passed as non-const reference due to logical vs physical constness in regards to the
   *                                ErrorResponseMessage GetPacket() API. To the public TraceIf interface only a const
   *                                reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_no_effect
  void TraceSendErrorResponse(ipc_protocol::ErrorResponseMessage& error_resp_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ErrorResponseMessageHeader const& error_resp_header{error_resp_message.GetMessageHeader()};
      trace::IpcMethodErrorResponse const r{
          Direction::kTx,
          error_resp_header.service_id_.value,
          error_resp_header.instance_id_.value,
          error_resp_header.major_version_.value,
          error_resp_header.method_id_.value,
          error_resp_header.client_id_.value,
          error_resp_header.session_id_.value,
          static_cast<std::underlying_type_t<ipc_protocol::ReturnCode>>(error_resp_header.return_code_),
          *error_resp_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC response message to the registered trace interface.
   * \details This function is called on reception of a method response.
   *
   * \param[in] resp_message  Response message to be used as trace method input parameters. Background: Passed as non-
   *                          const reference due to logical vs physical constness in regards to the ResponseMessage
   *                          GetPacket() API. To the public TraceIf interface only a const reference to packet buffer
   *                          is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveResponse(ipc_protocol::ResponseMessage& resp_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ResponseMessageHeader const& resp_header{resp_message.GetMessageHeader()};
      trace::IpcMethodResponse const r{Direction::kRx,
                                       resp_header.service_id_.value,
                                       resp_header.instance_id_.value,
                                       resp_header.major_version_.value,
                                       resp_header.method_id_.value,
                                       resp_header.client_id_.value,
                                       resp_header.session_id_.value,
                                       *resp_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC error response message to the registered trace interface.
   * \details This function is called on reception of a method error response.
   *
   * \param[in] error_resp_message  Error response message to be used as trace method input parameters. Background:
   *                                Passed as non-const reference due to logical vs physical constness in regards to the
   *                                ErrorResponseMessage GetPacket() API. To the public TraceIf interface only a const
   *                                reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_no_effect
  void TraceReceiveErrorResponse(ipc_protocol::ErrorResponseMessage& error_resp_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ErrorResponseMessageHeader const& error_resp_header{error_resp_message.GetMessageHeader()};
      trace::IpcMethodErrorResponse const r{
          Direction::kRx,
          error_resp_header.service_id_.value,
          error_resp_header.instance_id_.value,
          error_resp_header.major_version_.value,
          error_resp_header.method_id_.value,
          error_resp_header.client_id_.value,
          error_resp_header.session_id_.value,
          static_cast<std::underlying_type_t<ipc_protocol::ReturnCode>>(error_resp_header.return_code_),
          *error_resp_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC notification message to the registered trace interface.
   * \details This function is called on transmission of a notification.
   *
   * \param[in] notification_message  Notification message (header + payload) to be traced. Background: Passed as non-
   *                                  const reference due to logical vs physical constness in regards to the
   *                                  NotificationMessage GetPacket() API. To the public TraceIf interface only a const
   *                                  reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendNotification(ipc_protocol::NotificationMessage& notification_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::NotificationMessageHeader const& notification_header{notification_message.GetMessageHeader()};
      trace::IpcNotification const r{Direction::kTx,
                                     notification_header.service_id_.value,
                                     notification_header.instance_id_.value,
                                     notification_header.major_version_.value,
                                     notification_header.event_id_.value,
                                     notification_header.session_id_.value,
                                     *notification_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC notification message to the registered trace interface.
   * \details This function is called on reception of a notification.
   *
   * \param[in] notification_message  Notification message (header + payload) to be traced. Background: Passed as non-
   *                                  const reference due to logical vs physical constness in regards to the
   *                                  NotificationMessage GetPacket() API. To the public TraceIf interface only a const
   *                                  reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveNotification(ipc_protocol::NotificationMessage& notification_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::NotificationMessageHeader const& notification_header{notification_message.GetMessageHeader()};
      trace::IpcNotification const r{Direction::kRx,
                                     notification_header.service_id_.value,
                                     notification_header.instance_id_.value,
                                     notification_header.major_version_.value,
                                     notification_header.event_id_.value,
                                     notification_header.session_id_.value,
                                     *notification_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC application error message to the registered trace interface.
   * \details This function is called on transmission of an application error.
   *
   * \param[in] app_err_message  ApplicationError message (header + payload) to be traced. Background: Passed as non-
   *                             const reference due to logical vs physical constness in regards to the
   *                             ApplicationErrorMessage GetPacket() API. To the public TraceIf interface only a const
   *                             reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendApplicationError(ipc_protocol::ApplicationErrorMessage& app_err_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ApplicationErrorMessageHeader const& app_err_header{app_err_message.GetMessageHeader()};
      trace::IpcApplicationError const r{Direction::kTx,
                                         app_err_header.service_id_.value,
                                         app_err_header.instance_id_.value,
                                         app_err_header.major_version_.value,
                                         app_err_header.method_id_.value,
                                         app_err_header.client_id_.value,
                                         app_err_header.session_id_.value,
                                         *app_err_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC application error message to the registered trace interface.
   * \details This function is called on reception of an application error.
   *
   * \param[in] app_err_message  ApplicationError message (header + payload) to be traced. Background: Passed as non-
   *                             const reference due to logical vs physical constness in regards to the
   *                             ApplicationErrorMessage GetPacket() API. To the public TraceIf interface only a const
   *                             reference to packet buffer is passed which cannot be modified by the TraceIf user.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the IPC packet along with the message direction, and create the
   *     corresponding trace datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveApplicationError(ipc_protocol::ApplicationErrorMessage& app_err_message) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      ipc_protocol::ApplicationErrorMessageHeader const& app_err_header{app_err_message.GetMessageHeader()};
      trace::IpcApplicationError const r{Direction::kRx,
                                         app_err_header.service_id_.value,
                                         app_err_header.instance_id_.value,
                                         app_err_header.major_version_.value,
                                         app_err_header.method_id_.value,
                                         app_err_header.client_id_.value,
                                         app_err_header.session_id_.value,
                                         *app_err_message.GetPacket()};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC subscribe event ACK message to the registered trace interface.
   * \details This function is called on transmission of a subscribe acknowledge.
   *
   * \param[in] subscribe_event_ack_message_header  Message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendSubscribeEventAck(
      ipc_protocol::SubscribeEventAckMessageHeader const& subscribe_event_ack_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEventAck const r{Direction::kTx,
                                          subscribe_event_ack_message_header.service_id_.value,
                                          subscribe_event_ack_message_header.instance_id_.value,
                                          subscribe_event_ack_message_header.major_version_.value,
                                          subscribe_event_ack_message_header.event_id_.value,
                                          subscribe_event_ack_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC subscribe event ACK message to the registered trace interface.
   * \details This function is called on reception of a subscribe acknowledge.
   *
   * \param[in] subscribe_event_ack_message_header  Message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveSubscribeEventAck(
      ipc_protocol::SubscribeEventAckMessageHeader const& subscribe_event_ack_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEventAck const r{Direction::kRx,
                                          subscribe_event_ack_message_header.service_id_.value,
                                          subscribe_event_ack_message_header.instance_id_.value,
                                          subscribe_event_ack_message_header.major_version_.value,
                                          subscribe_event_ack_message_header.event_id_.value,
                                          subscribe_event_ack_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the outgoing IPC subscribe event NACK message to the registered trace interface.
   * \details This function is called on transmission of a subscribe not acknowledge.
   *
   * \param[in] subscribe_event_nack_message_header  Message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceSendSubscribeEventNAck(
      ipc_protocol::SubscribeEventNAckMessageHeader const& subscribe_event_nack_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEventNack const r{Direction::kTx,
                                           subscribe_event_nack_message_header.service_id_.value,
                                           subscribe_event_nack_message_header.instance_id_.value,
                                           subscribe_event_nack_message_header.major_version_.value,
                                           subscribe_event_nack_message_header.event_id_.value,
                                           subscribe_event_nack_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

  /*!
   * \brief   Forwards the incoming IPC subscribe event NACK message to the registered trace interface.
   * \details This function is called on reception of a subscribe not acknowledge.
   *
   * \param[in] subscribe_event_nack_message_header  Message header to be used as trace method input parameters.
   *
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the trace interface implementation is available.
   *   - Group the given message header values and the message direction together, and create the corresponding trace
   *     datatype.
   *   - Forward the trace message created above to the registered trace interface implementation.
   * \endinternal
   */
  void TraceReceiveSubscribeEventNAck(
      ipc_protocol::SubscribeEventNAckMessageHeader const& subscribe_event_nack_message_header) const {
    std::shared_lock<std::shared_timed_mutex> const lock{trace_if_lock_};
    if (trace_if_ != nullptr) {
      trace::IpcSubscribeEventNack const r{Direction::kRx,
                                           subscribe_event_nack_message_header.service_id_.value,
                                           subscribe_event_nack_message_header.instance_id_.value,
                                           subscribe_event_nack_message_header.major_version_.value,
                                           subscribe_event_nack_message_header.event_id_.value,
                                           subscribe_event_nack_message_header.client_id_.value};
      // VCA_IPCB_TRACE_IF_POINTER_IS_VALID
      trace_if_->Trace(r);
    }
  }

 private:
  /*!
   * \brief A monitor to trace ECU-internal IPC communication.
   */
  TraceIf* trace_if_{nullptr};

  /*!
   * \brief Mutex for protection of the trace if pointer.
   */
  mutable std::shared_timed_mutex trace_if_lock_{};
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_MONITOR_H_

// clang-format off
/*!
 * \exclusivearea
 * amsr::ipc_binding_core::internal::trace::TraceMonitor::trace_if_lock_
 * Protect the user-specified trace implementation pointer against concurrent modification and access.
 *
 * \protects amsr::ipc_binding_core::internal::trace::TraceMonitor::trace_if_
 *
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::SetTraceIf
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendRequest
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveRequest
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendRequestNoReturn
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveRequestNoReturn
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendSubscribe
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveSubscribe
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendUnsubscribe
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveUnsubscribe
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendResponse
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendErrorResponse
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveResponse
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveErrorResponse
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendNotification
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveNotification
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendApplicationError
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveApplicationError
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendSubscribeEventAck
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveSubscribeEventAck
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceSendSubscribeEventNAck
 * \usedin   amsr::ipc_binding_core::internal::trace::TraceMonitor::TraceReceiveSubscribeEventNAck
 *
 * \exclude  All other public APIs of MemoryPool.
 * \length   LONG if the user callback is set and is long, SHORT otherwise.
 * \endexclusivearea
 */
// clang-format on
