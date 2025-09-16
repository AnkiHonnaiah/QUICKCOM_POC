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
 *        \brief  Trace API interface.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"

#include "amsr/comtrace/meta_data.h"
#include "amsr/comtrace/trace_payload.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {

// VECTOR NC Metric-OO.WMC.One: MD_ComTrace_Metric-OO.WMC.One_TraceInterface_TracePoint_Callouts
/*!
 * \brief   Trace API callout interface to be implemented by a custom tracer.
 * \details Trace events for the different trace points will be notified to the custom tracer with the Trace...() API.
 *          With the non-pure-virtual API OnTraceCompleted() the completion of an asynchronous trace operation be
 *          notified back to ara::com.
 *
 * \vpublic
 *
 * \unit ComTrace::TraceInterface
 */
class TraceInterface {
 public:
  // ---- Construction / Destruction ----
  TraceInterface(TraceInterface const&) = delete;
  TraceInterface(TraceInterface&&) = delete;
  auto operator=(TraceInterface const&) & -> TraceInterface& = delete;
  auto operator=(TraceInterface&&) & -> TraceInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre     -
   * \context ANY
   * \vpublic
   * \steady  TRUE Iff the user called it during the steady phase, it must be implemented steadily.
   */
  virtual ~TraceInterface() noexcept = default;

  // ---- Registration / Initialization handling ----
  /*!
   * \brief   A new ara::com internal trace client is registered.
   * \details Callout is used to map the tracer name to a custom tracer specific numerical identification. This
   *          numerical representation is later passed with every trace callout back to the custom tracer. The
   *          registration of trace clients happens during initialization of the ara::com components.
   *
   * \param[in] name  Name of the ara::com internal trace client (e.g. "IpcBinding").
   *
   * \return Client ID mapped by the custom tracer to the ara::com internal trace client.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  virtual auto RegisterClient(StringView name) noexcept -> ClientId = 0;

  /*!
   * \brief   A new ara::com internal memory resource is registered.
   * \details Callout is used to map the tracepoints using an attached memory resource to the underlying memory
   *          resource. The registration of memory resources can happen during the lifetime of ara::com components.
   *          Therefore, this function must be implemented in a threadsafe manner.
   *
   * \param[in] file_descriptor  A file descriptor to the underlying memory resource.
   *
   * \return MemoryResourceId mapped by the custom tracer to the ara::com internal memory resource.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  virtual auto RegisterMemoryResource(FileDescriptor file_descriptor) noexcept -> MemoryResourceId = 0;

  /*!
   * \brief   An ara::com internal memory resource is unregistered.
   * \details Callout is used to inform the extension point that the underlying memory resource was freed. The
   *          unregistration of memory resources can happen during the lifetime of ara::com components. Therefore, this
   *          function must be implemented in a threadsafe manner.
   *
   * \param[in] memory_resource_id  MemoryResourceId of the underlying memory resource returned by
   *                                RegisterMemoryResource() API.
   *
   * \pre         Must only be called for a MemoryResourceId previously returned by a call to RegisterMemoryResource().
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  virtual void UnregisterMemoryResource(MemoryResourceId memory_resource_id) noexcept = 0;

  // ---- Trace Point Callouts: Events ----

  /*!
   * \brief Trace transmission of an event sample triggered by the [Skeleton]->[Event]->Send(SampleType const&) API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced event sample.
   * \param[in] payload    Access to the serialized event sample payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceEventSendSync(ClientId client_id, EventSendMetaData const& meta_data,
                                  TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the [Skeleton]->[Event]->Send(SampleType const&) API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceEventSendAsync(ClientId client_id, ContextId context_id, EventSendMetaData const& meta_data,
                                   TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the
   *        [Skeleton]->[Event]->Send(SampleAllocateePtr<SampleType>) API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced event sample.
   * \param[in] payload    Access to the serialized event sample payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceEventSendAllocateSync(ClientId client_id, EventSendAllocateMetaData const& meta_data,
                                          TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an event sample triggered by the
   *        [Skeleton]->[Event]->Send(SampleAllocateePtr<SampleType>) API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceEventSendAllocateAsync(ClientId client_id, ContextId context_id,
                                           EventSendAllocateMetaData const& meta_data,
                                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace processing of a single event sample received with the [Proxy]::[Event] GetNewSamples API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced event sample.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceEventReadSample(ClientId client_id, EventReadSampleMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Methods ----

  /*!
   * \brief Trace transmission of a method request triggered by the [Proxy]::[Method](Args...) API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced method request.
   * \param[in] payload    Access to the serialized method request payload (method input arguments). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceMethodRequestSendSync(ClientId client_id, MethodRequestSendMetaData const& meta_data,
                                          TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a method request triggered by the [Proxy]::[Method](Args...) API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceMethodRequestSendAsync(ClientId client_id, ContextId context_id,
                                           MethodRequestSendMetaData const& meta_data,
                                           TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a method request received on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced method request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceMethodRequestReceived(ClientId client_id,
                                          MethodRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an method response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced method response.
   * \param[in] payload    Access to the serialized method response payload (method output arguments). The contents of
   *                       the payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceMethodResponseSendSync(ClientId client_id, MethodResponseSendMetaData const& meta_data,
                                           TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an method response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceMethodResponseSendAsync(ClientId client_id, ContextId context_id,
                                            MethodResponseSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace transmission of an method response containing an ApApplicationError triggered by the resolution of the
   *        promise on skeleton-side.
   *
   * \param[in] client_id          ID of the ara::com internal trace client. Provided by extension point in
   *                               RegisterClient() callback.
   * \param[in] meta_data          Meta data of the traced method response.
   * \param[in] application_error  ErrorCode of the ApApplicationError.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceMethodResponseSend(ClientId client_id, MethodResponseSendMetaData const& meta_data,
                                       ErrorCode const& application_error) noexcept = 0;

  /*!
   * \brief Trace reception of a method response received on proxy-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced method response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceMethodResponseReceived(ClientId client_id,
                                           MethodResponseReceivedMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Field Notifier ----

  /*!
   * \brief Trace transmission of an field update triggered by the [Skeleton]->[Proxy]->Update(FieldType const&) API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field value.
   * \param[in] payload    Access to the serialized field value payload. The contents of the payload are only valid
   *                       within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldUpdateSync(ClientId client_id, FieldUpdateMetaData const& meta_data,
                                    TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field update triggered by the [Skeleton]->[Proxy]->Update(FieldType const&) API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceFieldUpdateAsync(ClientId client_id, ContextId context_id, FieldUpdateMetaData const& meta_data,
                                     TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace processing of a single field notifier sample received with the [Proxy]->[Field]->GetNewSamples() API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field notifier sample.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldReadSample(ClientId client_id, FieldReadSampleMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Field Getter ----

  /*!
   * \brief Trace transmission of a field getter request triggered by the [Proxy]->[Proxy]->Get() API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field getter request.
   * \param[in] payload    Access to the serialized field getter request payload. The contents of the payload are only
   *                       valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldGetRequestSendSync(ClientId client_id, FieldGetRequestSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a field getter request triggered by the [Proxy]->[Proxy]->Get() API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceFieldGetRequestSendAsync(ClientId client_id, ContextId context_id,
                                             FieldGetRequestSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field getter request received on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field getter request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldGetRequestReceived(ClientId client_id,
                                            FieldGetRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an field getter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field getter response.
   * \param[in] payload    Access to the serialized field getter response payload (field value). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldGetResponseSendSync(ClientId client_id, FieldGetResponseSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field getter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceFieldGetResponseSendAsync(ClientId client_id, ContextId context_id,
                                              FieldGetResponseSendMetaData const& meta_data,
                                              TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field getter response received on proxy-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field getter response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldGetResponseReceived(ClientId client_id,
                                             FieldGetResponseReceivedMetaData const& meta_data) noexcept = 0;

  // ---- Trace Point Callouts: Field Setter ----

  /*!
   * \brief Trace transmission of a field setter request triggered by the [Proxy]->[Proxy]->Set(FieldType const&) API.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field setter request.
   * \param[in] payload    Access to the serialized field setter request payload. The contents of the payload are only
   *                       valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldSetRequestSendSync(ClientId client_id, FieldSetRequestSendMetaData const& meta_data,
                                            TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of a field setter request triggered by the [Proxy]->[Proxy]->Set(FieldType const&) API.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceFieldSetRequestSendAsync(ClientId client_id, ContextId context_id,
                                             FieldSetRequestSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field setter request received on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field setter request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldSetRequestReceived(ClientId client_id,
                                            FieldSetRequestReceivedMetaData const& meta_data) noexcept = 0;

  /*!
   * \brief Trace transmission of an field setter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field setter response.
   * \param[in] payload    Access to the serialized field setter response payload (field value). The contents of the
   *                       payload are only valid within the context of the callout.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldSetResponseSendSync(ClientId client_id, FieldSetResponseSendMetaData const& meta_data,
                                             TracePayload const& payload) noexcept = 0;

  /*!
   * \brief Trace transmission of an field setter response triggered by the resolution of the promise on skeleton-side.
   *
   * \param[in] client_id   ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                        callback.
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
   * \vpublic
   * \steady      TRUE
   */
  virtual auto TraceFieldSetResponseSendAsync(ClientId client_id, ContextId context_id,
                                              FieldSetResponseSendMetaData const& meta_data,
                                              TracePayload const& payload) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Trace reception of a field setter response received on proxy-side.
   *
   * \param[in] client_id  ID of the ara::com internal trace client. Provided by extension point in RegisterClient()
   *                       callback.
   * \param[in] meta_data  Meta data of the traced field setter response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  virtual void TraceFieldSetResponseReceived(ClientId client_id,
                                             FieldSetResponseReceivedMetaData const& meta_data) noexcept = 0;

  // ---- Non-Interface: Asynchronous Confirmation Handling ----

  /*!
   * \brief Notify ara::com about completion of an ongoing asynchronous trace operation.
   *
   * \param[in] context_id  The Context ID / handle of the asynchronous trace operation. Only context IDs previously
   *                        received with any of the asynchronous trace point callouts must be returned.
   *
   * \pre         All ara::com components must be in state 'initialized'.
   * \pre         \p context_id must contain a context ID previously provided with any async trace callout function.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  static void OnTraceCompleted(ContextId context_id) noexcept;

 protected:
  /*!
   * \brief Default constructor for the interface.
   *
   * \vpublic
   * \steady  FALSE
   */
  TraceInterface() noexcept = default;
};

}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_INTERFACE_H_
