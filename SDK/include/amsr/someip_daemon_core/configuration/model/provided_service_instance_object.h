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
/*!        \file  provided_service_instance_object.h
 *        \brief  Configuration object for 'ProvidedServiceInstance'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'provided service instance' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class ProvidedServiceInstanceConfigObject final {
 public:
  /*!
   * \brief Service ID element.
   */
  using ServiceIdElement = CfgElement<someip_protocol::internal::ServiceId>;

  /*!
   * \brief Instance ID element.
   */
  using InstanceIdElement = CfgElement<someip_protocol::internal::InstanceId>;

  /*!
   * \brief Major version element.
   */
  using MajorVersionElement = CfgElement<someip_protocol::internal::MajorVersion>;

  /*!
   * \brief Minor version element.
   */
  using MinorVersionElement = CfgElement<someip_protocol::internal::MinorVersion>;

  /*!
   * \brief Service discovery sub-hierarchy.
   */
  using ServiceDiscoveryElement = CfgElement<ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery>;

  /*!
   * \brief Port mappings container sub-hierarchy.
   */
  using PortMappingsContainerElement = CfgElement<ConfigurationTypesAndDefs::MachineMappingContainer>;

  /*!
   * \brief SomeipMethodEventPropsContainer sub-hierarchy.
   */
  using SomeipMethodEventPropsContainerElement = CfgElement<ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] provided_service_instance Reference to the 'provided service instance' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit ProvidedServiceInstanceConfigObject(
      ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstance& provided_service_instance) noexcept
      : service_id_{provided_service_instance.service_id_},
        instance_id_{provided_service_instance.instance_id_},
        major_version_{provided_service_instance.major_version_},
        minor_version_{provided_service_instance.minor_version_},
        service_discovery_{provided_service_instance.service_discovery_},
        port_mappings_{provided_service_instance.port_mappings_},
        event_props_{provided_service_instance.event_props_},
        method_props_{provided_service_instance.method_props_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceConfigObject(ProvidedServiceInstanceConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceConfigObject(ProvidedServiceInstanceConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstanceConfigObject const&) & -> ProvidedServiceInstanceConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(ProvidedServiceInstanceConfigObject&& other) & noexcept
      -> ProvidedServiceInstanceConfigObject& = default;

  /*!
   * \brief Set the service ID when the parser finds the service ID.
   * \param[in] service_id The service ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServiceId(someip_protocol::internal::ServiceId const& service_id) noexcept {
    service_id_.SetElement(service_id);
  }

  /*!
   * \brief Getter for the service ID element.
   * \return Reference to the service ID of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceIdElement const& GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Set the instance ID when the parser finds the instance ID.
   * \param[in] instance_id The instance ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInstanceId(someip_protocol::internal::InstanceId const& instance_id) noexcept {
    instance_id_.SetElement(instance_id);
  }

  /*!
   * \brief Getter for the instance ID element.
   * \return Reference to the instance ID of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InstanceIdElement const& GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Set the major version, after the parser has the value for it.
   * \param[in] major_version The major version.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMajorVersion(someip_protocol::internal::MajorVersion const& major_version) noexcept {
    major_version_.SetElement(major_version);
  }

  /*!
   * \brief Getter for the major version element.
   * \return Reference to the major version of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MajorVersionElement const& GetMajorVersion() const noexcept { return major_version_; }

  /*!
   * \brief Set the minor version, after the parser has the value for it.
   * \param[in] minor_version The minor version.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMinorVersion(someip_protocol::internal::MinorVersion const& minor_version) noexcept {
    minor_version_.SetElement(minor_version);
  }

  /*!
   * \brief Getter for the minor version element.
   * \return Reference to the minor version of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MinorVersionElement const& GetMinorVersion() const noexcept { return minor_version_; }

  /*!
   * \brief Set the service discovery sub-hierarchy.
   * \param[in] service_discovery The service discovery sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServiceDiscovery(
      ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery const& service_discovery) noexcept {
    service_discovery_.SetElement(service_discovery);
  }

  /*!
   * \brief Getter for the service discovery sub-hierarchy.
   * \return Reference to the service discovery sub-hierarchy intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceDiscoveryElement const& GetServiceDiscovery() const noexcept { return service_discovery_; }

  /*!
   * \brief Set the port mapping sub-hierarchy.
   * \param[in] port_mappings The port mapping sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPortMappings(ConfigurationTypesAndDefs::MachineMappingContainer const& port_mappings) noexcept {
    port_mappings_.SetElement(port_mappings);
  }

  /*!
   * \brief Getter for the port mapping sub-hierarchy.
   * \return Reference to the port mapping sub-hierarchy intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PortMappingsContainerElement const& GetPortMappings() const noexcept { return port_mappings_; }

  /*!
   * \brief Set the event props sub-hierarchy.
   * \param[in] value The event props sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventProps(ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer const& value) noexcept {
    event_props_.SetElement(value);
  }

  /*!
   * \brief Set the method props sub-hierarchy.
   * \param[in] value The method props sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMethodProps(ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer const& value) noexcept {
    method_props_.SetElement(value);
  }

 private:
  /*!
   * \brief The service ID element.
   */
  ServiceIdElement service_id_;

  /*!
   * \brief The instance ID element.
   */
  InstanceIdElement instance_id_;

  /*!
   * \brief The major version element.
   */
  MajorVersionElement major_version_;

  /*!
   * \brief The minor version element.
   */
  MinorVersionElement minor_version_;

  /*!
   * \brief The service discovery sub-hierarchy.
   */
  ServiceDiscoveryElement service_discovery_;

  /*!
   * \brief The port mappings container sub-hierarchy.
   */
  PortMappingsContainerElement port_mappings_;

  /*!
   * \brief The SomeipMethodEventPropsContainer sub-hierarchy for events.
   */
  SomeipMethodEventPropsContainerElement event_props_;

  /*!
   * \brief The SomeipMethodEventPropsContainer sub-hierarchy for methods.
   */
  SomeipMethodEventPropsContainerElement method_props_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_OBJECT_H_
