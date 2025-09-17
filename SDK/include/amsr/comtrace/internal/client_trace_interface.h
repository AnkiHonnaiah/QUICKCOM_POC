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
 *        \brief  Interface for trace callout for ara::com internal trace clients.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACE_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/memory/memory_types.h"
#include "ara/core/error_code.h"
#include "ara/core/result.h"
#include "ara/core/string.h"

#include "amsr/comtrace/internal/async_client_trace_interface.h"
#include "amsr/comtrace/internal/client_types.h"
#include "amsr/comtrace/internal/config/trace_config_interface.h"
#include "amsr/comtrace/meta_data.h"
#include "amsr/comtrace/trace_payload.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {
namespace internal {

/*!
 * \brief String view type alias.
 */
using StringView = ::ara::core::StringView;

// VECTOR NC Metric-OO.WMC.One: MD_ComTrace_Metric-OO.WMC.One_TraceInterface_TracePoint_Callouts
/*!
 * \brief Interface for trace operations for the ara::com internal trace clients.
 *
 * \unit ComTrace::ClientTracer
 */
class ClientTraceInterface {
 public:
  /*!
   * \brief Type-alias for ErrorCode.
   */
  using ErrorCode = ::ara::core::ErrorCode;

  // ---- Constructors ----

  ClientTraceInterface(ClientTraceInterface const&) = delete;
  ClientTraceInterface(ClientTraceInterface&&) = delete;
  auto operator=(ClientTraceInterface const&) & -> ClientTraceInterface& = delete;
  auto operator=(ClientTraceInterface&&) & -> ClientTraceInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   */
  virtual ~ClientTraceInterface() noexcept = default;

  // ---- Registration: Memory resources ----

  /*!
   * \brief   A new ara::com internal memory resource is registered.
   * \details Callout is used to map the tracepoints using an attached memory resource to the underlying memory
   *          resource. The registration of memory resources can happen during the lifetime of ara::com components.
   *
   * \param[in] memory_handle  Handle to the underlying memory resource.
   *
   * \return MemoryResourceId mapped by the custom tracer to the ara::com internal memory resource.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto RegisterMemoryResource(::amsr::memory::MemoryHandle memory_handle) const noexcept
      -> MemoryResourceId = 0;

  /*!
   * \brief   An ara::com internal memory resource is unregistered.
   * \details Callout is used to inform the extension point that the underlying memory resource was freed. The
   *          unregistration of memory resources can happen during the lifetime of ara::com components.
   *
   * \param[in] memory_resource_id  MemoryResourceId of the underlying memory resource returned by
   *                                RegisterMemoryResource() API.
   *
   * \pre         Shall only be called for a MemoryResourceId previously returned by a call to RegisterMemoryResource().
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void UnregisterMemoryResource(MemoryResourceId memory_resource_id) const noexcept = 0;

  // ---- Trace Points: Events ----

  /*!
   * \brief Trace transmission of an event sample triggered by the [Skeleton]->[Event]->Send(SampleType const&) API.
   *
   * \param[in] meta_data  Meta data of the traced event sample.
   * \param[in] payload    Access to the serialized event sample payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceEventSendSync(EventSendMetaData const& meta_data, TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the [Skeleton]->[Event]->Send(SampleType const&) API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced event sample.
   * \param[in] payload     Access to the serialized event sample payload. The contents of the payload are valid until
   *                        the asynchronous trace operation is completed with a call to OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceEventSendAsync(ClientContextId context_id, EventSendMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the
   *        [Skeleton]->[Event]->Send(SampleAllocateePtr<SampleType>) API.
   *
   * \param[in] meta_data  Meta data of the traced event sample.
   * \param[in] payload    Access to the serialized event sample payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceEventSendAllocateSync(EventSendAllocateMetaData const& meta_data,
                                          TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the
   *        [Skeleton]->[Event]->Send(SampleAllocateePtr<SampleType>) API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced event sample.
   * \param[in] payload     Access to the serialized event sample payload. The contents of the payload are valid until
   *                        the asynchronous trace operation is completed with a call to OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceEventSendAllocateAsync(ClientContextId context_id, EventSendAllocateMetaData const& meta_data,
                                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace processing of a single event sample received with the [Proxy]->[Event] GetNewSamples API.
   *
   * \param[in] meta_data  Meta data of the traced event sample.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceEventReadSample(EventReadSampleMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Methods ----

  /*!
   * \brief Trace transmission of a method request triggered by the [Proxy]->[Method](Args...) API.
   *
   * \param[in] meta_data  Meta data of the traced method request.
   * \param[in] payload    Access to the serialized method request payload (method input arguments). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceMethodRequestSendSync(MethodRequestSendMetaData const& meta_data,
                                          TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a method request triggered by the [Proxy]->[Method](Args...) API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced method request.
   * \param[in] payload     Access to the serialized method request payload (method input arguments). The contents of
   *                        the payload are valid until the asynchronous trace operation is completed with a call to
   *                        OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceMethodRequestSendAsync(ClientContextId context_id, MethodRequestSendMetaData const& meta_data,
                                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a method request received on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced method request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceMethodRequestReceived(MethodRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an method response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced method response.
   * \param[in] payload    Access to the serialized method response payload (method output arguments). The contents of
   *                       the payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceMethodResponseSendSync(MethodResponseSendMetaData const& meta_data,
                                           TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an method response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced method response.
   * \param[in] payload     Access to the serialized method response payload (method output arguments). The contents of
   *                        the payload are valid until the asynchronous trace operation is completed with a call to
   *                        OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual auto TraceMethodResponseSendAsync(ClientContextId context_id, MethodResponseSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace transmission of an method response containing an ApApplicationError triggered by the resolution of the
   *        promise on skeleton-side.
   *
   * \param[in] meta_data          Meta data of the traced method response.
   * \param[in] application_error  ErrorCode of the ApApplicationError.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceMethodResponseSend(MethodResponseSendMetaData const& meta_data,
                                       ErrorCode const& application_error) noexcept = 0;

  /*!
   * \brief Trace reception of a method response received on proxy-side.
   *
   * \param[in] meta_data  Meta data of the traced method response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceMethodResponseReceived(MethodResponseReceivedMetaData const& meta_data) noexcept = 0;

  // --- Trace Point Callouts: Fields ----

  /*!
   * \brief Trace transmission of an field update triggered by the [Skeleton]->[Proxy]->Update(FieldType const&) API.
   *
   * \param[in] meta_data  Meta data of the traced field value.
   * \param[in] payload    Access to the serialized field value payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldUpdateSync(FieldUpdateMetaData const& meta_data, TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field update triggered by the [Skeleton]->[Proxy]->Update(FieldType const&) API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced field value.
   * \param[in] payload     Access to the serialized field value payload. The contents of the payload are valid until
   *                        the asynchronous trace operation is completed with a call to OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceFieldUpdateAsync(ClientContextId context_id, FieldUpdateMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace processing of a single field notifier sample received with the [Proxy]->[Field]->GetNewSamples() API.
   *
   * \param[in] meta_data  Meta data of the traced field notifier sample.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldReadSample(FieldReadSampleMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Field Getter ----

  /*!
   * \brief Trace transmission of a field getter request triggered by the [Proxy]->[Proxy]->Get() API.
   *
   * \param[in] meta_data  Meta data of the traced field getter request.
   * \param[in] payload    Access to the serialized field getter request payload. The contents of the payload are only
   *                       valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldGetRequestSendSync(FieldGetRequestSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a field getter request triggered by the [Proxy]->[Proxy]->Get() API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced field getter request.
   * \param[in] payload     Access to the serialized field getter request payload. The contents of the payload are valid
   *                        until the asynchronous trace operation is completed with a call to OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceFieldGetRequestSendAsync(ClientContextId context_id, FieldGetRequestSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field getter request received on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced field getter request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldGetRequestReceived(FieldGetRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an field getter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced field getter response.
   * \param[in] payload    Access to the serialized field getter response payload (field value). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldGetResponseSendSync(FieldGetResponseSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field getter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced field getter response.
   * \param[in] payload     Access to the serialized field getter response payload (field value). The contents of the
   *                        payload are valid until the asynchronous trace operation is completed with a call to
   *                        OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual auto TraceFieldGetResponseSendAsync(ClientContextId context_id, FieldGetResponseSendMetaData const& meta_data,
                                              TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field getter response received on proxy-side.
   *
   * \param[in] meta_data  Meta data of the traced field getter response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldGetResponseReceived(FieldGetResponseReceivedMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Field Setter ----

  /*!
   * \brief Trace transmission of a field setter request triggered by the [Proxy]->[Proxy]->Set(FieldType const&) API.
   *
   * \param[in] meta_data  Meta data of the traced field setter request.
   * \param[in] payload    Access to the serialized field setter request payload. The contents of the payload are only
   *                       valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldSetRequestSendSync(FieldSetRequestSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a field setter request triggered by the [Proxy]->[Proxy]->Set(FieldType const&) API.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced field setter request.
   * \param[in] payload     Access to the serialized field setter request payload. The contents of the payload are valid
   *                        until the asynchronous trace operation is completed with a call to OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto TraceFieldSetRequestSendAsync(ClientContextId context_id, FieldSetRequestSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field setter request received on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced field setter request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldSetRequestReceived(FieldSetRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an field setter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] meta_data  Meta data of the traced field setter response.
   * \param[in] payload    Access to the serialized field setter response payload (field value). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldSetResponseSendSync(FieldSetResponseSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field setter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] context_id  Context ID / handle of the asynchronous trace operation. This context ID must be passed
   *                        unchanged to the OnTraceCompleted() API to confirmation completion of the trace operation.
   * \param[in] meta_data   Meta data of the traced field setter response.
   * \param[in] payload     Access to the serialized field setter response payload (field value). The contents of the
   *                        payload are valid until the asynchronous trace operation is completed with a call to
   *                        OnTraceCompleted().
   *
   * \return Empty result if the asynchronous trace operation is started and will be confirmed with OnTraceCompleted().
   *
   * \error any_error_code  Asynchronous trace operation is not started. The payload will not be valid after completion
   *                        of the callout. A call of OnTraceCompleted() for the \p context_id is not necessary in this
   *                        case.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual auto TraceFieldSetResponseSendAsync(ClientContextId context_id, FieldSetResponseSendMetaData const& meta_data,
                                              TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field setter response received on proxy-side.
   *
   * \param[in] meta_data  Meta data of the traced field setter response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void TraceFieldSetResponseReceived(FieldSetResponseReceivedMetaData const& meta_data) noexcept = 0;

 protected:
  /*!
   * \brief Default constructor of the ClientTraceInterface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ClientTraceInterface() noexcept = default;
};

// ---- Global Functions -----------------------------------------------------------------------------------------------

/*!
 * \brief Register an ara::com internal trace client.
 *
 * \param[in] name          Name of trace client. Must be ara::com unique name.
 * \param[in] async_tracer  [Optional] callback interface for asynchronous trace clients. Callback interface must only
 *                          be provided for trace clients using the asynchronous trace callout functions.
 *
 * \return ClientTraceInterface for the registered trace client.
 *
 * \pre         Component must be in state 'initialized'.
 * \pre         Maximum number of registered trace clients must not be reached (65535).
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto RegisterTraceClient(StringView name, std::weak_ptr<AsyncClientTraceInterface> async_tracer =
                                              std::weak_ptr<AsyncClientTraceInterface>{}) noexcept
    -> std::shared_ptr<ClientTraceInterface>;

/*!
 * \brief Get read-only access to the configuration model hierarchy.
 *
 * \return Read-only access to the configuration model hierarchy.
 *
 * \pre         Component must be in state 'initialized'.
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
auto GetTraceConfig() noexcept -> config::TraceConfigInterface const&;

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CLIENT_TRACE_INTERFACE_H_
