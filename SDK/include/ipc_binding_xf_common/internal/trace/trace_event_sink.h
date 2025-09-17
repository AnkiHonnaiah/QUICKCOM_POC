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
 *        \brief Declaration of the sink for event tracing.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/comtrace/internal/client_trace_interface.h"
#include "amsr/comtrace/meta_data.h"
#include "amsr/core/array.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/runtime_configuration/event_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_event_sink_interface.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief Trace sink dispatching event traces to a client tracer provided by ComTrace.
 *
 * \unit IpcBinding::IpcBindingCore::TraceEventSink
 */
class TraceEventSink final : public TraceEventSinkInterface {
 public:
  /*!
   * \brief Type-alias for EventConfig.
   */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;

  /*!
   * \brief Type-alias for ClientTraceInterface.
   */
  using ClientTraceInterface = ::amsr::comtrace::internal::ClientTraceInterface;

  // ---- Constructor / Destructor ----
  /*!
   * \brief Construct a TraceEventSink.
   *
   * \param[in] client_tracer  Shared pointer to the client tracer (from ComTrace).
   * \param[in] event_config   Runtime configuration for the traced event.
   *
   * \pre         Client tracer is not null, event config stays valid.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  TraceEventSink(std::shared_ptr<ClientTraceInterface> client_tracer,  // IGNORE_BLACKLIST BSOCIAL-8805
                 EventConfig const& event_config) noexcept;

  TraceEventSink(TraceEventSink const&) noexcept = delete;
  TraceEventSink(TraceEventSink&&) noexcept = delete;
  auto operator=(TraceEventSink const&) noexcept -> TraceEventSink& = delete;
  auto operator=(TraceEventSink&&) noexcept -> TraceEventSink& = delete;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface::~TraceEventSinkInterface
   */
  ~TraceEventSink() noexcept final = default;  // VCA_IPCB_CLASS_DECLARATION

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface::TraceSend
   */
  void TraceSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                 NotificationMessage notification_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface::TraceSendAllocate
   */
  void TraceSendAllocate(ProvidedServiceInstanceId const& provided_service_instance_id,
                         NotificationMessage notification_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface::TraceReadSample
   */
  void TraceReadSample(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                       NotificationMessage notification_message) const noexcept final;

 private:
  /*!
   * \brief Type-alias for IpcPacketShared.
   */
  using IpcPacketShared = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared;

  /*!
   * \brief Type-alias for NotificationMessageHeader.
   */
  using NotificationMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessageHeader;

  /*!
   * \brief Type-alias for an array containing one ImmutableBufferView.
   */
  using SingleBufferViewContainer = ::amsr::core::Array<::amsr::comtrace::ImmutableBufferView, 1>;

  /*!
   * \brief Type alias for the used Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type-alias for EventType.
   */
  using EventType = ::amsr::ipc_binding_core::internal::runtime_configuration::EventType;

  /*!
   * \brief Build trace-public instance meta data from ipcbinding-internal structs.
   *
   * \param[in] provided_service_instance_id  The provided service instance id.
   *
   * \return Instance meta data.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildInstanceMetaData(ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> ::amsr::comtrace::InstanceMetaData;

  /*!
   * \brief Build trace-public proxy instance meta data from ipcbinding-internal structs.
   *
   * \param[in] provided_service_instance_id  The provided service instance id.
   * \param[in] client_id                     The client id.
   *
   * \return Proxy instance meta data.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildProxyInstanceMetaData(ProvidedServiceInstanceId const& provided_service_instance_id,
                                         ClientId client_id) noexcept -> ::amsr::comtrace::ProxyInstanceMetaData;

  /*!
   * \brief Build trace-public event meta data from ipcbinding-internal structs.
   *
   * \param[in] header  The notification message header.
   *
   * \return Event meta data.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildEventMetaData(NotificationMessageHeader const& header) noexcept -> ::amsr::comtrace::EventMetaData;

  /*!
   * \brief Build a trace payload view from an ipcbinding packet.
   *
   * \param[in] packet  The message packet (containing headers and payload).
   *
   * \return View of the message payload.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto BuildPayloadBufferView(IpcPacketShared const& packet) const noexcept -> ::amsr::comtrace::ImmutableBufferView;

  /*!
   * \brief Total IpcProtocol header length (generic + notification message header).
   */
  static constexpr std::size_t kNotificationMessageIpcProtocolHeaderLength{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kNotificationMessageHeaderLength};

  /*!
   * \brief The client trace to forward trace calls to.
   */
  std::shared_ptr<ClientTraceInterface> client_tracer_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief The runtime configuration for the traced event.
   */
  EventConfig const& event_config_;

  /*!
   * \brief Logger used in this class.
   */
  Logger const logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                       ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                       ::amsr::core::StringView{"TraceEventSink"}};
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_EVENT_SINK_H_
