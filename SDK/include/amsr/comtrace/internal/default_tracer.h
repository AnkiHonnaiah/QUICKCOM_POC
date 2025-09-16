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
 *        \brief  Default / null implementation of the TraceInterface.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_DEFAULT_TRACER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_DEFAULT_TRACER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"

#include "amsr/comtrace/meta_data.h"
#include "amsr/comtrace/trace_interface.h"
#include "amsr/comtrace/trace_payload.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_ComTrace_Metric-OO.WMC.One_TraceInterface_TracePoint_Callouts
/*!
 * \brief   Default implementation of a custom tracer.
 * \details Default implementation mimics an empty implementation not performing any trace operation.
 *
 * \unit ComTrace::DefaultTracer
 */
class DefaultTracer final : public TraceInterface {
 public:
  // ---- Construction ----
  /*!
   * \brief Construct a new DefaultTracer.
   *
   * \pre         -
   * \context     PreInit|PostShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  DefaultTracer() noexcept = default;

  DefaultTracer(DefaultTracer const&) noexcept = delete;
  auto operator=(DefaultTracer const&) & noexcept -> DefaultTracer& = delete;
  DefaultTracer(DefaultTracer&&) noexcept = delete;
  auto operator=(DefaultTracer&&) & noexcept -> DefaultTracer& = delete;

  /*!
   * \brief Destroy the DefaultTracer.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~DefaultTracer() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  // ---- Registration / Initialization ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::RegisterClient
   */
  auto RegisterClient(StringView name) noexcept -> ClientId final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::RegisterMemoryResource
   */
  auto RegisterMemoryResource(FileDescriptor file_descriptor) noexcept -> MemoryResourceId final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::UnregisterMemoryResource
   */
  void UnregisterMemoryResource(MemoryResourceId memory_resource_id) noexcept final;

  // ---- Trace Points: Events ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceEventSendSync
   */
  auto TraceEventSendSync(ClientId client_id, EventSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceEventSendAsync
   */
  auto TraceEventSendAsync(ClientId client_id, ContextId context_id, EventSendMetaData const& meta_data,
                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceEventSendAllocateSync
   */
  auto TraceEventSendAllocateSync(ClientId client_id, EventSendAllocateMetaData const& meta_data,
                                  TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceEventSendAllocateAsync
   */
  auto TraceEventSendAllocateAsync(ClientId client_id, ContextId context_id, EventSendAllocateMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceEventReadSample
   */
  auto TraceEventReadSample(ClientId client_id, EventReadSampleMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Points: Methods ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodRequestSendSync
   */
  auto TraceMethodRequestSendSync(ClientId client_id, MethodRequestSendMetaData const& meta_data,
                                  TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodRequestSendAsync
   */
  auto TraceMethodRequestSendAsync(ClientId client_id, ContextId context_id, MethodRequestSendMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodRequestReceived
   */
  auto TraceMethodRequestReceived(ClientId client_id, MethodRequestReceivedMetaData const& meta_data) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodResponseSendSync
   */
  auto TraceMethodResponseSendSync(ClientId client_id, MethodResponseSendMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodResponseSendAsync
   */
  auto TraceMethodResponseSendAsync(ClientId client_id, ContextId context_id,
                                    MethodResponseSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodResponseSend
   */
  auto TraceMethodResponseSend(ClientId client_id, MethodResponseSendMetaData const& meta_data,
                               ErrorCode const& application_error) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceMethodResponseReceived
   */
  auto TraceMethodResponseReceived(ClientId client_id, MethodResponseReceivedMetaData const& meta_data) noexcept
      -> void final;

  // ---- Trace Points: Field Notifier ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldUpdateSync
   */
  auto TraceFieldUpdateSync(ClientId client_id, FieldUpdateMetaData const& meta_data,
                            TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldUpdateAsync
   */
  auto TraceFieldUpdateAsync(ClientId client_id, ContextId context_id, FieldUpdateMetaData const& meta_data,
                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldReadSample
   */
  auto TraceFieldReadSample(ClientId client_id, FieldReadSampleMetaData const& meta_data) noexcept -> void final;

  // ---- Trace Point Callouts: Field Getter ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetRequestSendSync
   */
  auto TraceFieldGetRequestSendSync(ClientId client_id, FieldGetRequestSendMetaData const& meta_data,
                                    TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetRequestSendAsync
   */
  auto TraceFieldGetRequestSendAsync(ClientId client_id, ContextId context_id,
                                     FieldGetRequestSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetRequestReceived
   */
  auto TraceFieldGetRequestReceived(ClientId client_id, FieldGetRequestReceivedMetaData const& meta_data) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetResponseSendSync
   */
  auto TraceFieldGetResponseSendSync(ClientId client_id, FieldGetResponseSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetResponseSendAsync
   */
  auto TraceFieldGetResponseSendAsync(ClientId client_id, ContextId context_id,
                                      FieldGetResponseSendMetaData const& meta_data,
                                      TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldGetResponseReceived
   */
  auto TraceFieldGetResponseReceived(ClientId client_id, FieldGetResponseReceivedMetaData const& meta_data) noexcept
      -> void final;

  // ---- Trace Point Callouts: Field Setter ----

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetRequestSendSync
   */
  auto TraceFieldSetRequestSendSync(ClientId client_id, FieldSetRequestSendMetaData const& meta_data,
                                    TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetRequestSendAsync
   */
  auto TraceFieldSetRequestSendAsync(ClientId client_id, ContextId context_id,
                                     FieldSetRequestSendMetaData const& meta_data, TracePayload const& payload) noexcept
      -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetRequestReceived
   */
  auto TraceFieldSetRequestReceived(ClientId client_id, FieldSetRequestReceivedMetaData const& meta_data) noexcept
      -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetResponseSendSync
   */
  auto TraceFieldSetResponseSendSync(ClientId client_id, FieldSetResponseSendMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> void final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetResponseSendAsync
   */
  auto TraceFieldSetResponseSendAsync(ClientId client_id, ContextId context_id,
                                      FieldSetResponseSendMetaData const& meta_data,
                                      TracePayload const& payload) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc amsr::comtrace::TraceInterface::TraceFieldSetResponseReceived
   */
  auto TraceFieldSetResponseReceived(ClientId client_id, FieldSetResponseReceivedMetaData const& meta_data) noexcept
      -> void final;

 private:
  /*!
   * \brief Counter for the registered memory resources.
   */
  MemoryResourceId memory_resource_id_counter_{0};
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_DEFAULT_TRACER_H_
