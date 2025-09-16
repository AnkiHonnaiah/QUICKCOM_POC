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
 *        \brief  Incorporate settings of ComTrace trace config into the IpcBinding runtime config.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_TRACE_CONFIG_UPDATER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_TRACE_CONFIG_UPDATER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/config/trace_config_interface.h"

#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {

/*!
 * \brief   Unit to update the IpcBinding runtime config model hierarchy with the trace config settings provided by
 *          ComTrace.
 * \details The ComTrace trace config is designed in a way that 'full optionality' of all model parameters are possible.
 *          By default the tracing of all tracepoints is disabled. Due to this trace config design it is assumed that
 *          the trace config model is typically smaller than the IpcBinding runtime config model. Therefore the update
 *          strategy always tries to iterate over the trace config model elements and then trying to find them in the
 *          runtime config model.
 *
 * \unit       TraceConfigUpdater
 * \complexity Interaction with huge ComTrace and IpcBinding models necessary.
 */
class TraceConfigUpdater final {
 public:
  /*! Type-alias for RuntimeConfig */
  using RuntimeConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig;
  /*! Type-alias for TraceConfigInterface */
  using TraceConfigInterface = ::amsr::comtrace::internal::config::TraceConfigInterface;

  // ---- Construction -------------------------------------------------------------------------------------------------

  TraceConfigUpdater() noexcept = delete;

  TraceConfigUpdater(TraceConfigUpdater const&) noexcept = delete;
  auto operator=(TraceConfigUpdater const&) noexcept -> TraceConfigUpdater& = delete;
  TraceConfigUpdater(TraceConfigUpdater&&) noexcept = delete;
  auto operator=(TraceConfigUpdater&&) noexcept -> TraceConfigUpdater& = delete;

  ~TraceConfigUpdater() noexcept = delete;

  // ---- Public APIs --------------------------------------------------------------------------------------------------

  /*!
   * \brief Update the IpcBinding runtime config model with the trace config settings.
   *
   * \param[in,out] runtime_config  The IpcBinding runtime config model to be updated.
   * \param[in]     trace_config    The ComTrace trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateRuntimeConfigWithTraceConfig(RuntimeConfig& runtime_config,
                                                 TraceConfigInterface const& trace_config) noexcept;

 private:
  // -- RuntimeConfig type aliases --

  /*! Type-Alias for ServiceConfig */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;
  /*! Type-Alias for MutableServiceConfigRef */
  using MutableServiceConfigRef =
      ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig::MutableServiceConfigRef;

  /*! Type-Alias for MethodConfig */
  using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;
  /*! Type-Alias for MutableMethodConfigRef */
  using MutableMethodConfigRef =
      ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig::MutableMethodConfigRef;

  /*! Type-Alias for EventConfig */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;
  /*! Type-Alias for MutableEventConfigRef */
  using MutableEventConfigRef =
      ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig::MutableEventConfigRef;

  /*! Type-Alias for FieldConfig */
  using FieldConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig;
  /*! Type-Alias for MutableFieldConfigRef */
  using MutableFieldConfigRef =
      ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig::MutableFieldConfigRef;

  // -- TraceConfig type aliases --

  /*! Type-Alias for TraceServiceConfigInterface */
  using TraceServiceConfigInterface = ::amsr::comtrace::internal::config::ServiceConfigInterface;
  /*! Type-Alias for TraceServiceConfigInterfaceRef */
  using TraceServiceConfigInterfaceRef = ::amsr::comtrace::internal::config::ServiceConfigInterfaceRef;

  /*! Type-Alias for TraceMethodConfigInterface */
  using TraceMethodConfigInterface = ::amsr::comtrace::internal::config::MethodConfigInterface;
  /*! Type-Alias for TraceMethodConfigInterfaceRef */
  using TraceMethodConfigInterfaceRef = ::amsr::comtrace::internal::config::MethodConfigInterfaceRef;

  /*! Type-Alias for TraceEventConfigInterface */
  using TraceEventConfigInterface = ::amsr::comtrace::internal::config::EventConfigInterface;
  /*! Type-Alias for TraceEventConfigInterfaceRef */
  using TraceEventConfigInterfaceRef = ::amsr::comtrace::internal::config::EventConfigInterfaceRef;

  /*! Type-Alias for TraceFieldConfigInterfaceRef */
  using TraceFieldConfigInterfaceRef = ::amsr::comtrace::internal::config::FieldConfigInterfaceRef;

  // ---- Private APIs -------------------------------------------------------------------------------------------------

  /*!
   * \brief Update all method configurations of a ServiceInterface configuration.
   *
   * \param[in,out] service_config        The IpcBinding ServiceInterface runtime config model to be updated.
   * \param[in]     trace_service_config  The ComTrace ServiceInterface trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateMethodConfigs(ServiceConfig& service_config,
                                  TraceServiceConfigInterface const& trace_service_config) noexcept;

  /*!
   * \brief Update all event configurations of a ServiceInterface configuration.
   *
   * \param[in,out] service_config        The IpcBinding ServiceInterface runtime config model to be updated.
   * \param[in]     trace_service_config  The ComTrace ServiceInterface trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateEventConfigs(ServiceConfig& service_config,
                                 TraceServiceConfigInterface const& trace_service_config) noexcept;

  /*!
   * \brief Update all field configurations of a ServiceInterface configuration.
   *
   * \param[in,out] service_config        The IpcBinding ServiceInterface runtime config model to be updated.
   * \param[in]     trace_service_config  The ComTrace ServiceInterface trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateFieldConfigs(ServiceConfig& service_config,
                                 TraceServiceConfigInterface const& trace_service_config) noexcept;

  /*!
   * \brief Update a single method or field getter/setter configuration.
   *
   * \param[in,out] method_config        The IpcBinding method runtime config model to be updated.
   * \param[in]     trace_method_config  The ComTrace method trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateMethodConfig(MethodConfig& method_config, TraceMethodConfigInterface const& trace_method_config);

  /*!
   * \brief Update a single event or field notifier configuration.
   *
   * \param[in,out] event_config        The IpcBinding event runtime config model to be updated.
   * \param[in]     trace_event_config  The ComTrace event trace config model.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static void UpdateEventConfig(EventConfig& event_config, TraceEventConfigInterface const& trace_event_config);
};

}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_TRACE_CONFIG_UPDATER_H_
