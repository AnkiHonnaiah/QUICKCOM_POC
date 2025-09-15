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
 *        \brief  Trace callout interface for ara::com internal trace clients.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/memory/memory_types.h"
#include "ara/core/result.h"

#include "amsr/comtrace/internal/client_trace_interface.h"
#include "amsr/comtrace/internal/client_types.h"
#include "amsr/comtrace/meta_data.h"
#include "amsr/comtrace/trace_interface.h"
#include "amsr/comtrace/trace_payload.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_ComTrace_Metric-OO.WMC.One_TraceInterface_TracePoint_Callouts
/*!
 * \brief Interface for trace operations for the ara::com internal trace clients.
 *
 * \unit ComTrace::ClientTracer
 */
class ClientTracer final : public ClientTraceInterface {
 public:
  /*!
   * \brief Type-Alias for StringView.
   */
  using StringView = ::amsr::core::StringView;

  /*!
   * \brief Type-Alias for ClientId.
   */
  using ClientId = ::amsr::comtrace::ClientId;

  // ---- Constructors ----

  /*!
   * \brief Type-Alias for TraceInterface.
   */
  using TraceInterface = ::amsr::comtrace::TraceInterface;

  /*!
   * \brief Construct a ClientTracer.
   *
   * \param[in] internal_client_id  The internal ID of the related trace client.
   * \param[in] client_name         The name of the related trace client.
   * \param[in] external_client_id  The external ID of the related trace client.
   * \param[in] tracer              Reference to the registered custom tracer.
   *
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit ClientTracer(InternalClientId internal_client_id, StringView client_name, ClientId external_client_id,
                        TraceInterface& tracer) noexcept;

  ClientTracer(ClientTracer const&) = delete;
  ClientTracer(ClientTracer&&) = delete;
  auto operator=(ClientTracer const&) & -> ClientTracer& = delete;
  auto operator=(ClientTracer&&) & -> ClientTracer& = delete;

  /*!
   * \brief Destroy the ClientTracer.
   *
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ClientTracer() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  // ---- Registration: Memory resources ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::RegisterMemoryResource
   */
  auto RegisterMemoryResource(::amsr::memory::MemoryHandle memory_handle) const noexcept -> MemoryResourceId final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::UnregisterMemoryResource
   */
  void UnregisterMemoryResource(MemoryResourceId memory_resource_id) const noexcept final;

  // ---- Trace Points: Events ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceEventSendSync
   */
  auto TraceEventSendSync(EventSendAllocateMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceEventSendAsync
   */
  auto TraceEventSendAsync(ClientContextId context_id, EventSendAllocateMetaData const& meta_data,
                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceEventSendAllocateSync
   */
  auto TraceEventSendAllocateSync(EventSendAllocateMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceEventSendAllocateAsync
   */
  auto TraceEventSendAllocateAsync(ClientContextId context_id, EventSendAllocateMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceEventReadSample
   */
  auto TraceEventReadSample(EventReadSampleMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Points: Methods ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodRequestSendSync
   */
  auto TraceMethodRequestSendSync(MethodRequestSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodRequestSendAsync
   */
  auto TraceMethodRequestSendAsync(ClientContextId context_id, MethodRequestSendMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodRequestReceived
   */
  auto TraceMethodRequestReceived(MethodRequestReceivedMetaData const& meta_data) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodResponseSendSync
   */
  auto TraceMethodResponseSendSync(MethodResponseSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodResponseSendAsync
   */
  auto TraceMethodResponseSendAsync(ClientContextId context_id, MethodResponseSendMetaData const& meta_data,
                                    TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodResponseSend
   */
  auto TraceMethodResponseSend(MethodResponseSendMetaData const& meta_data, ErrorCode const& application_error) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceMethodResponseReceived
   */
  auto TraceMethodResponseReceived(MethodResponseReceivedMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Points: Field Notifier ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldUpdateSync
   */
  auto TraceFieldUpdateSync(FieldUpdateMetaData const& meta_data, TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldUpdateAsync
   */
  auto TraceFieldUpdateAsync(ClientContextId context_id, FieldUpdateMetaData const& meta_data,
                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldReadSample
   */
  auto TraceFieldReadSample(FieldReadSampleMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Point Callouts: Field Getter ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetRequestSendSync
   */
  auto TraceFieldGetRequestSendSync(FieldGetRequestSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetRequestSendAsync
   */
  auto TraceFieldGetRequestSendAsync(ClientContextId context_id, FieldGetRequestSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetRequestReceived
   */
  auto TraceFieldGetRequestReceived(FieldGetRequestReceivedMetaData const& meta_data) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetResponseSendSync
   */
  auto TraceFieldGetResponseSendSync(FieldGetResponseSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetResponseSendAsync
   */
  auto TraceFieldGetResponseSendAsync(ClientContextId context_id, FieldGetResponseSendMetaData const& meta_data,
                                      TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldGetResponseReceived
   */
  auto TraceFieldGetResponseReceived(FieldGetResponseReceivedMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Point Callouts: Field Setter ----

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetRequestSendSync
   */
  auto TraceFieldSetRequestSendSync(FieldSetRequestSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetRequestSendAsync
   */
  auto TraceFieldSetRequestSendAsync(ClientContextId context_id, FieldSetRequestSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetRequestReceived
   */
  auto TraceFieldSetRequestReceived(FieldSetRequestReceivedMetaData const& meta_data) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetResponseSendSync
   */
  auto TraceFieldSetResponseSendSync(FieldSetResponseSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetResponseSendAsync
   */
  auto TraceFieldSetResponseSendAsync(ClientContextId context_id, FieldSetResponseSendMetaData const& meta_data,
                                      TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::internal::ClientTraceInterface::TraceFieldSetResponseReceived
   */
  auto TraceFieldSetResponseReceived(FieldSetResponseReceivedMetaData const& meta_data) noexcept -> void final;

 private:
  /*!
   * \brief Build the external context ID for asynchronous trace operations out of the internal client ID and the client
   *        specific context ID.
   *
   * \param[in] client_context_id  Trace client specific context ID.
   *
   * \return The encoded external context ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto BuildContextId(ClientContextId client_context_id) const noexcept -> ContextId;

  /*!
   * \brief The internal client ID of the ara::com trace client using this ClientTracer instance.
   */
  InternalClientId internal_client_id_;

  /*!
   * \brief The name of the ara::com trace client using this ClientTracer instance.
   */
  StringView const client_name_;

  /*!
   * \brief The external client ID of the ara::com trace client using this ClientTracer instance.
   */
  ClientId external_client_id_;

  /*!
   * \brief Reference to the registered custom tracer instance.
   */
  TraceInterface& tracer_;
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACER_H_
