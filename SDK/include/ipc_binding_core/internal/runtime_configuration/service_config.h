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
 *        \brief  Configuration class of service
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_SERVICE_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_SERVICE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <unordered_map>

#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/event_config.h"
#include "ipc_binding_core/internal/runtime_configuration/field_config.h"
#include "ipc_binding_core/internal/runtime_configuration/method_config.h"
#include "ipc_binding_core/internal/runtime_configuration/provided_service_instance_config.h"
#include "ipc_binding_core/internal/runtime_configuration/required_service_instance_config.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Class to hold the data for the service.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class ServiceConfig final {
 public:
  /*! Type name alias for the map of shortnames and the associated event. */
  using EventsMap = std::unordered_map<Shortname::type, EventConfig, ara::core::hash<Shortname::type>>;
  /*! Type name alias for the map of shortnames and the associated method. */
  using MethodsMap = std::unordered_map<Shortname::type, MethodConfig, ara::core::hash<Shortname::type>>;
  /*! Type name alias for the map of shortnames and the associated field. */
  using FieldsMap = std::unordered_map<Shortname::type, FieldConfig, ara::core::hash<Shortname::type>>;

  /*! Type name alias for Service Identifier */
  using ServiceIdentifier = ipc_protocol::ServiceId;
  /*! Type name alias of Instance identifier. */
  using InstanceIdentifier = ipc_protocol::InstanceId;
  /*! Type name alias for major version */
  using MajorVersion = ipc_protocol::MajorVersion;
  /*! Type name alias for minor version */
  using MinorVersion = ipc_protocol::MinorVersion;

  /*! Type name alias for mutable reference of event config. */
  using MutableEventConfigRef = std::reference_wrapper<EventConfig>;
  /*! Type name alias for mutable reference of method config. */
  using MutableMethodConfigRef = std::reference_wrapper<MethodConfig>;
  /*! Type name alias for mutable reference of field config. */
  using MutableFieldConfigRef = std::reference_wrapper<FieldConfig>;

  /*! Type name alias for reference of provided service instance config. */
  using ProvidedServiceInstanceConfigRef = std::reference_wrapper<ProvidedServiceInstanceConfig const>;
  /*! Type name alias for reference of required service instance config. */
  using RequiredServiceInstanceConfigRef = std::reference_wrapper<RequiredServiceInstanceConfig const>;
  /*! Type name alias for a vector of ProvidedServiceInstanceConfigRef. */
  using ProvidedServiceInstanceConfigRefContainer = ara::core::Vector<ProvidedServiceInstanceConfigRef>;
  /*! Type name alias for a vector of RequiredServiceInstanceConfigRef. */
  using RequiredServiceInstanceConfigRefContainer = ara::core::Vector<RequiredServiceInstanceConfigRef>;

  /*!
   * \brief Construct an empty service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfig() noexcept = default;

  /*!
   * \brief Destroy service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServiceConfig() noexcept = default;  // VCA_IPCB_STD_UNORDERED_MAP_DESTRUCTOR

  /*!
   * \brief Copy-Construct a service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfig(ServiceConfig const&) = default;  // VCA_IPCB_STD_UNORDERED_MAP_COPY_CONSTRUCTOR

  auto operator=(ServiceConfig const&) noexcept -> ServiceConfig& = delete;
  ServiceConfig(ServiceConfig&&) noexcept = delete;
  auto operator=(ServiceConfig&&) noexcept -> ServiceConfig& = delete;

  /*!
   * \brief Gets the service identifier.
   *
   * \return The service identifier.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetId() const noexcept -> ServiceIdentifier;

  /*!
   * \brief Sets the service identifier.
   *
   * \param[in] service_id  The service identifier.
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
  void SetId(ServiceIdentifier service_id) noexcept;

  /*!
   * \brief Gets the shortname path.
   *
   * \return The shortname path.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetShortnamePath() const noexcept -> ShortnamePath const&;

  /*!
   * \brief Sets the shortname path.
   *
   * \param[in] shortname_path  The shortname path.
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
  void SetShortnamePath(ShortnamePath const& shortname_path) noexcept;

  /*!
   * \brief Gets the major version of the service.
   *
   * \return The major version.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMajorVersion() const noexcept -> MajorVersion;

  /*!
   * \brief Sets the major version of the service.
   *
   * \param[in] major_version  The major version of the service.
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
  void SetMajorVersion(MajorVersion const& major_version) noexcept;

  /*!
   * \brief Gets the minor version of the service.
   *
   * \return The minor version.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMinorVersion() const noexcept -> MinorVersion;

  /*!
   * \brief Sets the minor version of the service.
   *
   * \param[in] minor_version  The minor version of the service.
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
  void SetMinorVersion(MinorVersion const& minor_version) noexcept;

  /*!
   * \brief Gets whether the service is generic.
   *
   * \return Whether the service is generic.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto IsGeneric() const noexcept -> bool;

  /*!
   * \brief Gets whether the service requires generated code.
   *
   * \return Whether the service requires generated code.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto RequiresGeneratedCode() const noexcept -> bool;

  /*!
   * \brief Sets whether the service is generic.
   *
   * \param[in] is_generic  Whether the service is generic.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  void SetIsGeneric(bool is_generic) noexcept;

  /*!
   * \brief Get event configuration for a specific event.
   *
   * \param[in] event_shortname  The event shortname.
   *
   * \return The event.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The passed shortname has to exist in the events map.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetEventConfigChecked(Shortname::type const& event_shortname) const noexcept -> EventConfig const&;

  /*!
   * \brief Get a reference to the event configuration for a specific event.
   *
   * \param[in] event_shortname  The event shortname.
   *
   * \return Optional containing the event configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetEventConfig(Shortname::type const& event_shortname) noexcept -> ara::core::Optional<MutableEventConfigRef>;

  /*!
   * \brief Adds a new event config.
   *
   * \param[in] shortname  Shortname of event, to which the event config belongs.
   * \param[in] event      The event.
   *
   * \pre         Passed shortname is not allowed to already exist in events map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddEvent(Shortname::type const& shortname, EventConfig&& event) noexcept;

  /*!
   * \brief Getter implementation for the method.
   *
   * \param[in] method_shortname  The method shortname.
   *
   * \return The method.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The passed shortname has to exist in the methods map.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMethodConfigChecked(Shortname::type const& method_shortname) const noexcept -> MethodConfig const&;

  /*!
   * \brief Get a reference to the method configuration for a specific method.
   *
   * \param[in] method_shortname  The method shortname.
   *
   * \return Optional containing the method configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMethodConfig(Shortname::type const& method_shortname) noexcept -> ara::core::Optional<MutableMethodConfigRef>;

  /*!
   * \brief Adds a new method config.
   *
   * \param[in] shortname  Shortname of method, to which the method config belongs.
   * \param[in] method     The method.
   *
   * \pre         Passed shortname is not allowed to already exist in methods map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddMethod(Shortname::type const& shortname, MethodConfig&& method) noexcept;

  /*!
   * \brief Getter implementation for the field.
   *
   * \param[in] field_shortname  The shortname of the field.
   *
   * \return The field.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The passed shortname has to exist in the fields map.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetFieldConfigChecked(Shortname::type const& field_shortname) const noexcept -> FieldConfig const&;

  /*!
   * \brief Get a reference to the field configuration for a specific field.
   *
   * \param[in] field_shortname  The field shortname.
   *
   * \return Optional containing the field configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetFieldConfig(Shortname::type const& field_shortname) noexcept
      -> ara::core::Optional<std::reference_wrapper<FieldConfig>>;

  /*!
   * \brief Adds a new field config.
   *
   * \param[in] shortname  Shortname of field, to which the field config belongs.
   * \param[in] field      The field.
   *
   * \pre         Passed shortname is not allowed to already exist in fields map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddField(Shortname::type const& shortname, FieldConfig&& field) noexcept;

  /*!
   * \brief   Adds a new mapping between a provided service instance ID and a provided service instance config.
   * \details Abort if provided service id already exists in services map.
   *
   * \param[in] instance_id        The identifier of the provided service instance, to which the provided service
   *                               instance configuration belongs.
   * \param[in] provided_instance  The configuration of the provided service instance.
   *
   * \pre         Passed id is not allowed to already exist in services map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddProvidedServiceInstanceMapping(InstanceIdentifier instance_id,
                                         ProvidedServiceInstanceConfig&& provided_instance) noexcept;

  /*!
   * \brief   Adds a new mapping between a required service instance ID and a required service instance config.
   * \details Abort if required service id already exists in services map.
   *
   * \param[in] instance_id        The identifier of the required service instance, to which the required service
   *                               instance configuration belongs.
   * \param[in] required_instance  The configuration of the required service instance.
   *
   * \pre         Passed id is not allowed to already exist in services map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddRequiredServiceInstanceMapping(InstanceIdentifier instance_id,
                                         RequiredServiceInstanceConfig&& required_instance) noexcept;

  /*!
   * \brief Getter for the configured provided service instances.
   *
   * \return All configured provided service instances data.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetProvidedServiceInstances() const noexcept -> ProvidedServiceInstanceConfigRefContainer;

  /*!
   * \brief Getter for the configured required service instances.
   *
   * \return All configured required service instances data.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetRequiredServiceInstances() const noexcept -> RequiredServiceInstanceConfigRefContainer;

  /*!
   * \brief Get all the events in this service.
   *
   * \return An immutable reference to the map of event shortname to event configurations.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetEvents() const noexcept -> EventsMap const&;

  /*!
   * \brief Get all the fields in this service.
   *
   * \return An immutable reference to the map of field shortname to field configurations.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetFields() const noexcept -> FieldsMap const&;

  /*!
   * \brief Get all the methods in this service.
   *
   * \return An immutable reference to the map of method shortname to method configurations.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMethods() const noexcept -> MethodsMap const&;

 private:
  /*!
   * \brief Check if a given event id exists in the existing events or field notifiers in the respective maps.
   *
   * \param[in] event_id  Event id.
   *
   * \return True if there is a matching event or field notifier with the given event id.
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
  auto HasEventOrFieldNotifierWithId(EventConfig::EventId event_id) const noexcept -> bool;

  /*!
   * \brief Check if a given method id exists in the existing method or field operations in the respective maps.
   *
   * \param[in] method_id  The method id.
   *
   * \return True if there is a matching method or field with the given method id.
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
  auto HasMethodWithId(MethodConfig::MethodId method_id) const noexcept -> bool;

  /*!
   * \brief Check if a given field member entity id exists in the existing field, event, or method containers.
   *
   * \param[in] field_config  The field config for which to check setter, getter and notifier id.
   *
   * \return True if there is a id found in the existing containers.
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
  auto HasFieldWithId(FieldConfig const& field_config) const noexcept -> bool;

  /*! Service Id */
  ServiceIdentifier service_id_{};

  /*! Short name path for the service interface. */
  ShortnamePath service_shortname_path_{};

  /*! The major version. */
  MajorVersion major_version_{};

  /*! The minor version. */
  MinorVersion minor_version_{};

  /*! Whether the service is generic. */
  bool is_generic_{false};

  /*! Events container. */
  EventsMap events_map_{};

  /*! Methods container. */
  MethodsMap methods_map_{};

  /*! Fields container. */
  FieldsMap fields_map_{};

  /*! Type name alias for the map of service instance identifiers and the associated required service instances. */
  using RequiredServiceInstancesMap =
      std::unordered_map<InstanceIdentifier, RequiredServiceInstanceConfig, InstanceIdHash>;

  /*! RequiredServiceInstance container. */
  RequiredServiceInstancesMap required_service_instances_map_{};

  /*! Type name alias for the map of service instance identifiers and the associated provided service instances. */
  using ProvidedServiceInstancesMap =
      std::unordered_map<InstanceIdentifier, ProvidedServiceInstanceConfig, InstanceIdHash>;

  /*! ProvidedServiceInstance container. */
  ProvidedServiceInstancesMap provided_service_instances_map_{};

  /*! Logger. */
  ::amsr::ipc_binding_core::internal::logging::AraComLogger const logger_{
      ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
      ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription, "ServiceConfig"_sv};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_SERVICE_CONFIG_H_
