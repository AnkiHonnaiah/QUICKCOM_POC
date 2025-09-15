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
 *        \brief  Generic proxy instance backend class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_BACKEND_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <set>
#include <utility>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/generic_proxy_backend_interface.h"
#include "ara/core/map.h"
#include "ara/core/string_view.h"

#include "ipc_binding_core/internal/aracom_ipc_binding_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/remote_server_interface.h"
#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/runtime_configuration/field_config.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_generic_layer/internal/events/generic_proxy_event_backend.h"
#include "ipc_binding_generic_layer/internal/fields/generic_proxy_field_backend.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_fire_and_forget_method_backend.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_method_backend.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {

/*!
 * \brief Generic Proxy Backend.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyInstance
 */
class AMSR_LG_BETA GenericProxyBackend final : public ::amsr::socal::internal::GenericProxyBackendInterface {
 public:
  /*!
   * \brief Type alias for the proxy backend interface this class implements.
   */
  using ProxyBackendInterface = ::amsr::socal::internal::GenericProxyBackendInterface;

  /*!
   * \brief Type alias for RequiredServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for RequiredServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Runtime service configuration type.
   */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for AraComIpcBindingInterface.
   */
  using AraComIpcBindingInterface = ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface;

  /*!
   * \brief Type alias for the trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Type alias for RemoteServerManagerInterface.
   */
  using RemoteServerManagerInterface = ::amsr::ipc_binding_core::internal::RemoteServerManagerInterface;

  /*!
   * \brief Type alias for StringView.
   */
  using StringView = ara::core::StringView;

  /*!
   * \brief Type alias for IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for the ProxyRouterFactoryInterface.
   */
  using ProxyRouterFactoryInterface = ::amsr::ipc_binding_core::internal::ProxyRouterFactoryInterface;

  /*!
   * \brief Type alias for Client ID.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Construct a generic proxy instance backend.
   *
   * \param[in] remote_server_manager         The remote server manager.
   * \param[in] provided_service_instance_id  The provided service instance id.
   * \param[in] required_service_instance_id  The required service instance id.
   * \param[in] client_id                     The unique client id for this proxy instance.
   * \param[in] integrity_level               The integrity level for this service instance.
   * \param[in] service_config                The runtime configuration for this service.
   * \param[in] trace_sink_factory            The trace sink factory.
   * \param[in] proxy_router_factory          The proxy router factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericProxyBackend(RemoteServerManagerInterface& remote_server_manager,
                      ProvidedServiceInstanceId provided_service_instance_id,
                      RequiredServiceInstanceId required_service_instance_id, ClientId client_id,
                      IntegrityLevel integrity_level, ServiceConfig const& service_config,
                      TraceSinkFactoryInterface const& trace_sink_factory,
                      ProxyRouterFactoryInterface const& proxy_router_factory) noexcept;

  /*!
   * \brief Destructor for the proxy instance backend.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GenericProxyBackend() noexcept final;

  auto operator=(GenericProxyBackend&&) noexcept -> GenericProxyBackend& = delete;
  GenericProxyBackend(GenericProxyBackend const&) noexcept = delete;
  auto operator=(GenericProxyBackend const&) noexcept -> GenericProxyBackend& = delete;
  GenericProxyBackend(GenericProxyBackend&&) noexcept = delete;

  /*!
   * \brief   Get the backend for the given event.
   * \details For each event a distinct backend instance is returned.
   *
   * \param[in] short_name  The short-name of the event.
   *
   * \return A generic event backend. The returned reference is valid at-least until the lifetime of this generic proxy
   *         backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetEventBackend(StringView const& short_name) noexcept -> GenericProxyEventBackendInterface& final;

  /*!
   * \brief   Get the backend for the given field.
   * \details For each field a distinct backend instance is returned.
   *
   * \param[in] short_name  The short-name of the field.
   *
   * \return A generic field backend. The returned reference is valid at-least until the lifetime of this generic proxy
   *         backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetFieldBackend(StringView const& short_name) noexcept -> GenericProxyFieldBackendInterface& final;

  /*!
   * \brief   Get the backend for the given fire-and-forget method.
   * \details For each fire-and-forget method a distinct backend instance is returned.
   *
   * \param[in] short_name  The short-name of the fire-and-forget method.
   *
   * \return A generic fire-and-forget method backend. The returned reference must be valid at-least until the lifetime
   *         of this generic proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetFireAndForgetMethodBackend(StringView const& short_name) noexcept
      -> GenericProxyFireAndForgetMethodBackendInterface& final;

  /*!
   * \brief   Get the backend for the given method.
   * \details For each method a distinct backend instance is returned.
   *
   * \param[in] short_name  The short-name of the method.
   *
   * \return A generic method backend. The returned reference must be valid at-least until the lifetime of this generic
   *         proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetMethodBackend(StringView const& short_name) noexcept -> GenericProxyMethodBackendInterface& final;

  /*!
   * \brief Get the short-names of all the configured events.
   *
   * \return A set containing short-names of all the configured events. The returned StringView objects is valid
   *         at-least until the lifetime of this generic proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllEvents() const noexcept -> std::set<StringView> final;

  /*!
   * \brief Get the short-names of all the configured fields.
   *
   * \return A set containing short-names of all the configured fields. The returned StringView objects is valid
   *         at-least until the lifetime of this generic proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllFields() const noexcept -> std::set<StringView> final;

  /*!
   * \brief Get the short-names of all the configured fire-and-forget methods.
   *
   * \return A set containing short-names of all the configured fire-and-forget methods. The returned StringView objects
   *         must be valid at-least until the lifetime of this generic proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllFireAndForgetMethods() const noexcept -> std::set<StringView> final;

  /*!
   * \brief Get the short-names of all the configured methods.
   *
   * \return A set containing short-names of all the configured methods. The returned StringView objects must be valid
   *         at-least until the lifetime of this generic proxy backend object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE against any API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllMethods() const noexcept -> std::set<StringView> final;

 private:
  /*!
   * \brief Type alias for EventConfig.
   */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;

  /*!
   * \brief Type alias for MethodConfig.
   */
  using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;

  /*!
   * \brief Type alias for FieldConfig.
   */
  using FieldConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig;

  /*!
   * \brief Type alias for GenericProxyEventBackend.
   */
  using GenericProxyEventBackend = ::amsr::ipc_binding_generic_layer::internal::events::GenericProxyEventBackend;

  /*!
   * \brief Type alias for GenericProxyMethodBackend.
   */
  using GenericProxyMethodBackend = ::amsr::ipc_binding_generic_layer::internal::methods::GenericProxyMethodBackend;

  /*!
   * \brief Type alias for GenericProxyMethodBackend.
   */
  using GenericProxyFireAndForgetMethodBackend =
      ::amsr::ipc_binding_generic_layer::internal::methods::GenericProxyFireAndForgetMethodBackend;

  /*!
   * \brief Type alias for GenericProxyFieldBackend.
   */
  using GenericProxyFieldBackend = ::amsr::ipc_binding_generic_layer::internal::fields::GenericProxyFieldBackend;

  /*!
   * \brief Type alias for EventMap.
   */
  using EventMap = ara::core::Map<StringView, GenericProxyEventBackend>;

  /*!
   * \brief Type alias for MethodMap.
   */
  using MethodMap = ara::core::Map<StringView, GenericProxyMethodBackend>;

  /*!
   * \brief Type alias for MethodMap.
   */
  using FireAndForgetMethodMap = ara::core::Map<StringView, GenericProxyFireAndForgetMethodBackend>;

  /*!
   * \brief Type alias for FieldMap.
   */
  using FieldMap = ara::core::Map<StringView, GenericProxyFieldBackend>;

  /*!
   * \brief Type alias for ProxyRouterInterface.
   */
  using ProxyRouterInterface = ::amsr::ipc_binding_core::internal::ProxyRouterInterface;

  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for RemoteServer.
   */
  using RemoteServerInterface = ::amsr::ipc_binding_core::internal::RemoteServerInterface;

  /*!
   * \brief The ProxyRouter instance.
   */
  amsr::UniquePtr<ProxyRouterInterface> proxy_router_;

  /*!
   * \brief This proxy's client id.
   */
  ClientId const client_id_;

  /*!
   * \brief The map storing event backends by shortname.
   */
  EventMap event_backend_map_{};

  /*!
   * \brief The map storing method backends by shortname.
   */
  MethodMap method_backend_map_{};

  /*!
   * \brief The map storing fire and forget method backends by shortname.
   */
  FireAndForgetMethodMap fire_and_forget_method_backend_map_{};

  /*!
   * \brief The map storing field backends by shortname.
   */
  FieldMap field_backend_map_{};

  /*!
   * \brief The remote server.
   */
  amsr::SharedPtr<RemoteServerInterface> remote_server_;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;
};

}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_BACKEND_H_
