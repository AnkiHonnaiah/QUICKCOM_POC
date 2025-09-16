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
/*!        \file  required_service_instance_object.h
 *        \brief  Configuration object for 'RequiredServiceInstance'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief This is an intermediate representation of a 'required service instance' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class RequiredServiceInstanceConfigObject final {
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
   * \brief Version driven find behavior element.
   */
  using VersionDrivenFindBehavior = CfgElement<ConfigurationTypesAndDefs::VersionDrivenFindBehavior>;

  /*!
   * \brief Service discovery sub-hierarchy.
   */
  using ServiceDiscoveryElement = CfgElement<ConfigurationTypesAndDefs::RequiredServiceInstanceServiceDiscovery>;

  /*!
   * \brief Port mapping sub-hierarchy.
   */
  using PortMappingElement = CfgElement<ConfigurationTypesAndDefs::MachineMapping>;

  /*!
   * \brief Required PDUs.
   */
  using RequiredPdusElement = CfgElement<ConfigurationTypesAndDefs::SignalSerializedPduContainer>;

  /*!
   * \brief SomeipMethodEventPropsContainer sub-hierarchy.
   */
  using SomeipMethodEventPropsContainerElement = CfgElement<ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] required_service_instance Reference to the 'required service instance' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredServiceInstanceConfigObject(
      ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance& required_service_instance) noexcept
      : service_id_{required_service_instance.service_id_},
        instance_id_{required_service_instance.instance_id_},
        major_version_{required_service_instance.major_version_},
        minor_version_{required_service_instance.minor_version_},
        version_driven_find_behavior_{required_service_instance.version_driven_find_behavior_},
        service_discovery_{required_service_instance.service_discovery_},
        port_mapping_{required_service_instance.port_mapping_},
        pdus_{required_service_instance.required_pdus_},
        method_props_{required_service_instance.method_props_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceConfigObject(RequiredServiceInstanceConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceConfigObject(RequiredServiceInstanceConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstanceConfigObject const&) & -> RequiredServiceInstanceConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(RequiredServiceInstanceConfigObject&& other) & noexcept
      -> RequiredServiceInstanceConfigObject& = default;

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
   * \brief Set the version driven find behavior.
   * \param version_driven_find_behavior
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetVersionDrivenFindBehavior(
      ConfigurationTypesAndDefs::VersionDrivenFindBehavior const& version_driven_find_behavior) noexcept {
    version_driven_find_behavior_.SetElement(version_driven_find_behavior);
  }

  VersionDrivenFindBehavior const& GetVersionDrivenFindBehavior() const noexcept {
    return version_driven_find_behavior_;
  }

  /*!
   * \brief Set the service discovery sub-hierarchy.
   * \param[in] service_discovery The service discovery sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServiceDiscovery(
      ConfigurationTypesAndDefs::RequiredServiceInstanceServiceDiscovery const& service_discovery) noexcept {
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
   * \param[in] port_mapping The port mapping  sub-hierarchy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPortMapping(ConfigurationTypesAndDefs::MachineMapping const& port_mapping) noexcept {
    port_mapping_.SetElement(port_mapping);
  }

  /*!
   * \brief Getter for the port mapping sub-hierarchy.
   * \return Reference to the port mapping sub-hierarchy intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PortMappingElement const& GetPortMapping() const noexcept { return port_mapping_; }

  /*!
   * \brief Set the signal serialized PDUs.
   * \param[in] pdus The signal serialized PDUs.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetRequiredPdus(ConfigurationTypesAndDefs::SignalSerializedPduContainer const& pdus) noexcept {
    pdus_.SetElement(pdus);
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
   * \brief The version driven find behavior element.
   */
  VersionDrivenFindBehavior version_driven_find_behavior_;

  /*!
   * \brief The service discovery sub-hierarchy.
   */
  ServiceDiscoveryElement service_discovery_;

  /*!
   * \brief The port mapping sub-hierarchy.
   */
  PortMappingElement port_mapping_;

  /*!
   * \brief The required PDUs.
   */
  RequiredPdusElement pdus_;

  /*!
   * \brief The SomeipMethodEventPropsContainer sub-hierarchy for methods.
   */
  SomeipMethodEventPropsContainerElement method_props_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_OBJECT_H_
