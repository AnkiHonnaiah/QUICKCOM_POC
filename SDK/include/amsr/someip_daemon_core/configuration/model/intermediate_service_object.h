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
/*!        \file  intermediate_service_object.h
 *        \brief  Internal representation of the service object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_SERVICE_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_SERVICE_OBJECT_H_
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
 * \brief This is an intermediate representation of a service object which is filled by a parser.
 * \details The validator then takes an object of this type and validates if all mandatory elements are set or not.
 */
class ServiceConfigObject final {
 public:
  /*!
   * \brief Service ID element of this service.
   */
  using ServiceIdElement = CfgElement<someip_protocol::internal::ServiceId>;

  /*!
   * \brief Major version element of this service.
   */
  using MajorVersionElement = CfgElement<someip_protocol::internal::MajorVersion>;

  /*!
   * \brief Minor version element of this service.
   */
  using MinorVersionElement = CfgElement<someip_protocol::internal::MinorVersion>;

  /*!
   * \brief Container of methods this service has as a type.
   */
  using MethodContainerElement = CfgElement<ConfigurationTypesAndDefs::MethodContainer>;

  /*!
   * \brief Container of events this service has as a type.
   */
  using EventContainerElement = CfgElement<ConfigurationTypesAndDefs::EventContainer>;

  /*!
   * \brief Container of event groups this service has as a type.
   */
  using EventGroupContainerElement = CfgElement<ConfigurationTypesAndDefs::EventgroupContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] service Reference to the service structure to write the parsed values to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit ServiceConfigObject(ConfigurationTypesAndDefs::Service& service) noexcept
      : service_id_{service.id_},
        major_version_{service.major_version_},
        minor_version_{service.minor_version_},
        methods_container_{service.methods_},
        events_container_{service.events_},
        event_groups_container_{service.eventgroups_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ServiceConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ServiceConfigObject(ServiceConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ServiceConfigObject(ServiceConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment constructor.
   */
  auto operator=(ServiceConfigObject const&) & -> ServiceConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(ServiceConfigObject&& other) & noexcept -> ServiceConfigObject& = default;

  /*!
   * \brief Set the service ID when the parser finds the service ID.
   * \param[in] service_id SOME/IP Service ID.
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
   * \return A reference to Service ID element of this service.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceIdElement const& GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Set the major version, after the parser has the value for it.
   * \param[in] major_version Major version used for the SOME/IP deployment.
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
   * \return Reference to the major version of this intermediate service object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MajorVersionElement const& GetMajorVersion() const noexcept { return major_version_; }

  /*!
   * \brief Set the minor version, after the parser has the value for it.
   * \param[in] minor_version Minor version used for the SOME/IP messages.
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
   * \return Reference to the minor version of this intermediate service object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MinorVersionElement const& GetMinorVersion() const noexcept { return minor_version_; }

  /*!
   * \brief Set the methods container after reading it in completely.
   * \param[in] methods Container of methods parsed.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMethods(ConfigurationTypesAndDefs::MethodContainer const& methods) noexcept {
    methods_container_.SetElement(methods);
  }

  /*!
   * \brief Set the events container after reading it in completely.
   * \param[in] events Container of events parsed.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEvents(ConfigurationTypesAndDefs::EventContainer const& events) noexcept {
    events_container_.SetElement(events);
  }

  /*!
   * \brief Set the event groups
   * \param[in] event_groups Reference to the container to copy.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventGroups(ConfigurationTypesAndDefs::EventgroupContainer const& event_groups) noexcept {
    event_groups_container_.SetElement(event_groups);
  }

 private:
  /*!
   * \brief The service ID element.
   */
  ServiceIdElement service_id_;

  /*!
   * \brief The major version element.
   */
  MajorVersionElement major_version_;

  /*!
   * \brief The minor version element.
   */
  MinorVersionElement minor_version_;

  /*!
   * \brief The container of methods this service contains.
   */
  MethodContainerElement methods_container_;

  /*!
   * \brief The container of events this service contains.
   */
  EventContainerElement events_container_;

  /*!
   * \brief The container of event groups this service contains.
   */
  EventGroupContainerElement event_groups_container_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_SERVICE_OBJECT_H_
