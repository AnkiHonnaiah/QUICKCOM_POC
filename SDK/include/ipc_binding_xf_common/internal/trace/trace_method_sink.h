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
 *        \brief Declaration of the sink for method tracing.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_H_

#include <memory>

#include "amsr/comtrace/internal/client_trace_interface.h"
#include "amsr/comtrace/meta_data.h"
#include "amsr/comtrace/trace_payload.h"
#include "amsr/core/array.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/runtime_configuration/method_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief Trace sink dispatching method traces to a client tracer provided by ComTrace.
 *
 * \unit       IpcBinding::IpcBindingCore::TraceMethodSink
 * \complexity Many calls to ComTrace for building trace information and making callouts.
 */
class TraceMethodSink final : public TraceMethodSinkInterface {
 public:
  /*!
   * \brief Type-alias for MethodConfig.
   */
  using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;

  /*!
   * \brief Type-alias for ClientTraceInterface.
   */
  using ClientTraceInterface = ::amsr::comtrace::internal::ClientTraceInterface;

  // ---- Constructor / Destructor ----
  /*!
   * \brief Construct a TraceMethodSink.
   *
   * \param[in] client_tracer  Shared pointer to the client tracer (from ComTrace).
   * \param[in] method_config  Runtime configuration for the traced method.
   *
   * \pre         Client tracer is not null, method config stays valid.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  TraceMethodSink(std::shared_ptr<ClientTraceInterface> client_tracer,  // IGNORE_BLACKLIST BSOCIAL-8805
                  MethodConfig const& method_config) noexcept;

  TraceMethodSink(TraceMethodSink const&) noexcept = delete;
  TraceMethodSink(TraceMethodSink&&) noexcept = delete;
  auto operator=(TraceMethodSink const&) noexcept -> TraceMethodSink& = delete;
  auto operator=(TraceMethodSink&&) noexcept -> TraceMethodSink& = delete;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::~TraceMethodSinkInterface
   */
  ~TraceMethodSink() noexcept final = default;  // VCA_IPCB_CLASS_DECLARATION

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceRequestSend
   */
  void TraceRequestSend(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                        RequestMessage request_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceRequestNoReturnSend
   */
  void TraceRequestNoReturnSend(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                                RequestNoReturnMessage request_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceRequestReceived
   */
  void TraceRequestReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                            RemoteRequestMessage request_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceRequestNoReturnReceived
   */
  void TraceRequestNoReturnReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                                    RequestNoReturnMessage request_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceResponseSend
   */
  void TraceResponseSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                         ResponseMessage response_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceApplicationErrorResponseSend
   */
  void TraceApplicationErrorResponseSend(ProvidedServiceInstanceId const& provided_service_instance_id,
                                         ApplicationErrorMessage response_message,
                                         ErrorCode const& error_code) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceResponseReceived
   */
  void TraceResponseReceived(ProvidedServiceInstanceId const& provided_service_instance_id, ClientId client_id,
                             ResponseMessage response_message) const noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface::TraceApplicationErrorResponseReceived
   */
  void TraceApplicationErrorResponseReceived(ProvidedServiceInstanceId const& provided_service_instance_id,
                                             ClientId client_id,
                                             ApplicationErrorMessage response_message) const noexcept final;

 private:
  /*!
   * \brief Type-alias for IpcPacketShared.
   */
  using IpcPacketShared = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared;

  /*!
   * \brief Type-alias for an array containing one ImmutableBufferView.
   */
  using SingleBufferViewContainer = ::amsr::core::Array<::amsr::comtrace::ImmutableBufferView, 1>;

  /*!
   * \brief Type alias for the used Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type-alias for MethodType.
   */
  using MethodType = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodType;

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
   * \brief Build trace-public method meta data from ipcbinding-internal structs.
   *
   * \tparam MessageHeader  Either RequestMessageHeader or RequestNoReturnMessageHeader.
   *
   * \param[in] header  The message header.
   *
   * \return Method meta data.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  template <typename MessageHeader>
  static auto BuildMethodMetaData(MessageHeader const& header) noexcept -> ::amsr::comtrace::MethodMetaData;

  /*!
   * \brief Build a trace payload view from an ipcbinding packet.
   *
   * \param[in] packet         The message packet (containing headers and payload).
   * \param[in] header_length  The length of the header to remove.
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
  auto BuildPayloadBufferView(IpcPacketShared&& packet, std::size_t const header_length) const noexcept
      -> ::amsr::comtrace::ImmutableBufferView;

  /*!
   * \brief Total IpcProtocol method request header length (generic + request message header).
   */
  static constexpr std::size_t kRequestMessageIpcProtocolHeaderLength{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kRequestMessageHeaderLength};

  /*!
   * \brief Total IpcProtocol fire&forget method request header length (generic + request message header).
   */
  static constexpr std::size_t kRequestNoReturnMessageIpcProtocolHeaderLength{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kRequestNoReturnMessageHeaderLength};

  /*!
   * \brief Total IpcProtocol method response header length (generic + response message header).
   */
  static constexpr std::size_t kResponseMessageIpcProtocolHeaderLength{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kResponseMessageHeaderLength};

  /*!
   * \brief Total IpcProtocol ApplicationError header length (generic + application error message header).
   */
  static constexpr std::size_t kApplicationErrorMessageIpcProtocolHeaderLength{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kApplicationErrorMessageHeaderLength};

  /*!
   * \brief The client trace to forward trace calls to.
   */
  std::shared_ptr<ClientTraceInterface> client_tracer_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief The runtime configuration for the traced method.
   */
  MethodConfig const& method_config_;

  /*!
   * \brief Logger used in this class.
   */
  Logger const logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                       ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                       ::amsr::core::StringView{"TraceMethodSink"}};
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_METHOD_SINK_H_
