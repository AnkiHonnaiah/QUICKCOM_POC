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
 *        \brief Trace sink interface for tracing events and field notifiers.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_INTERFACE_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief   Trace sink interface for tracing events and field notifiers.
 * \details Supported trace points:
 *          - Send(SampleType)
 *          - Send(SampleAllocateePtr)
 *          - ReadSample
 *
 * \unit IpcBinding::IpcBindingCore::TraceEventSink
 */
class TraceEventSinkInterface {
 public:
  /*!
   * \brief Type-alias for NotificationMessage.
   */
  using NotificationMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage;

  /*!
   * \brief Type-alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for ClientId.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  TraceEventSinkInterface(TraceEventSinkInterface const&) noexcept = delete;
  TraceEventSinkInterface(TraceEventSinkInterface&&) noexcept = delete;
  auto operator=(TraceEventSinkInterface const&) noexcept -> TraceEventSinkInterface& = delete;
  auto operator=(TraceEventSinkInterface&&) noexcept -> TraceEventSinkInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~TraceEventSinkInterface() noexcept = default;

  /*!
   * \brief Trace sending a NotificationMessage from a skeleton with Send(SampleType) API.
   *
   * \param[in] provided_service_instance_id  The provided service instance that produced the NotificationMessage.
   * \param[in] notification_message          The NotificationMessage that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                         NotificationMessage notification_message) const noexcept -> void = 0;
  /*!
   * \brief Trace sending a NotificationMessage from a skeleton with Send(SampleAllocateePtr) API.
   *
   * \param[in] provided_service_instance_id  The provided service instance that produced the NotificationMessage.
   * \param[in] notification_message          The NotificationMessage that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceSendAllocate(ProvidedServiceInstanceId const& provided_service_instance_id,
                                 NotificationMessage notification_message) const noexcept -> void = 0;

  /*!
   * \brief Trace reading a NotificationMessage from a proxy.
   *
   * \param[in] provided_service_instance_id  The provided service instance that produced the NotificationMessage.
   * \param[in] client_id                     The client id of the proxy reading the NotificationMessage.
   * \param[in] notification_message          The NotificationMessage that was sent.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto TraceReadSample(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                               NotificationMessage notification_message) const noexcept -> void = 0;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceEventSinkInterface() noexcept = default;
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_INTERFACE_H_
