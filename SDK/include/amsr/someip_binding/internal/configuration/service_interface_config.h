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
/*!        \file  service_interface_config.h
 *        \brief  ServiceInterface configuration class.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_INTERFACE_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_INTERFACE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include <unordered_map>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/field_config.h"
#include "amsr/someip_binding/internal/configuration/method_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

// VECTOR NC Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Class to hold the data for the service interface.
 */
// VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
class ServiceInterfaceConfig {
 public:
  /*! Type name alias for the map of shortnames and the associated event. */
  using EventsMap = std::unordered_map<Shortname, EventConfig, ShortnameHash>;

  /*! Type name alias for the map of shortnames and the associated field. */
  using FieldsMap = std::unordered_map<Shortname, FieldConfig, ShortnameHash>;

  /*! Type name alias for a map associating shortname to a method config. */
  using MethodsMap = std::unordered_map<MethodConfig::Shortname, MethodConfig, ShortnameHash>;

  /*!
   * \brief  Gets the service identifier.
   * \return The service identifier.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceIdentifier GetServiceId() const noexcept;

  /*!
   * \brief Sets the service identifier.
   * \param service_id The service identifier.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetServiceId(ServiceIdentifier const& service_id) noexcept;

  /*!
   * \brief  Gets the is_generic flag.
   * \return The is_generic flag.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  IsGeneric const& GetIsGeneric() const noexcept;

  /*!
   * \brief Sets the is_generic flag.
   * \param is_generic The is_generic flag.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetIsGeneric(IsGeneric const& is_generic) noexcept;

  /*!
   * \brief  Gets the shortname path.
   * \return The shortname path.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ShortnamePath const& GetShortnamePath() const noexcept;

  /*!
   * \brief Sets the shortname path.
   * \param shortname_path The shortname path.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetShortnamePath(ShortnamePath const& shortname_path) noexcept;

  /*!
   * \brief  Gets the service major version.
   * \return The major version.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MajorVersion GetMajorVersion() const noexcept;

  /*!
   * \brief Sets the service major version.
   * \param major_version The service major version.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMajorVersion(MajorVersion const& major_version) noexcept;

  /*!
   * \brief  Gets the minor version.
   * \return The minor version.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MinorVersion GetMinorVersion() const noexcept;

  /*!
   * \brief Sets the minor version.
   * \param minor_version The minor version.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMinorVersion(MinorVersion const& minor_version) noexcept;

  /*!
   * \brief Getter implementation for the event configs.
   * \return a map containing data of the event configs.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  EventsMap const& GetEvents() const noexcept;

  /*!
   * \brief Getter implementation for the event config.
   * \param[in] event_id the event id.
   * \return the event config.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<EventConfig> GetEventOf(EventConfig::EventId const& event_id) const noexcept;

  /*!
   * \brief Adds a new event config to the event configs.
   * \param[in] event the event config.
   * \pre -
   * \return a value result if event adding was successful.
   * \error  SomeIpBindingErrc::json_parsing_failure if an event with matching shortname already exists.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> AddEvent(EventConfig&& event) noexcept;

  /*!
   * \brief Getter implementation for the field configs.
   * \return a map containing data of the field configs.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  FieldsMap const& GetFields() const noexcept;

  /*!
   * \brief Getter implementation for the field config.
   * \param[in] notifier_id The notifier id.
   * \return the field config.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<FieldConfig> GetFieldOf(NotifierConfig::NotifierId const& notifier_id) const noexcept;

  /*!
   * \brief Adds a new field config to the field configs.
   * \param[in] field the field config.
   * \pre -
   * \return a value result if field adding was successful.
   * \error  SomeIpBindingErrc::json_parsing_failure if a field with matching shortname already exists.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> AddField(FieldConfig&& field) noexcept;

  /*!
   * \brief Get the configuration of all methods.
   * \return a map containing data of the methods.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MethodsMap const& GetMethods() const noexcept;

  /*!
   * \brief Get the configuration of a specific method.
   * \param[in] method_id the method id.
   * \return the method config.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<MethodConfig> GetMethodOf(MethodConfig::MethodId const& method_id) const noexcept;

  /*!
   * \brief Adds a new method to the configuration.
   * \param[in] method the method configuration.
   * \pre -
   * \error  SomeIpBindingErrc::json_parsing_failure if a method with a matching shortname or id already exists.
   * \return a value result if field adding was successful.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> AddMethod(MethodConfig&& method) noexcept;

 private:
  /*!
   * \brief Check if a given event id exists in the existing events or field notifiers in the respective maps.
   * \param[in] event_id event id
   * \return a value result if event id is unique.
   * \error  SomeIpBindingErrc::json_parsing_failure if an event or field notifier with matching id already exists.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> EventIdExists(EventConfig::EventId const& event_id) const noexcept;

  /*!
   * \brief Check if a given method id exists in the existing method or field operations in the respective maps.
   * \param[in] method_id the method id.
   * \return a value result if method id is unique.
   * \error  SomeIpBindingErrc::json_parsing_failure if a method or field with matching id already exists.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> OperationIdExists(MethodConfig::MethodId const& method_id) const noexcept;

  /*!
   * \brief Check if a given field member entity id exists in the existing field, event, or method containers.
   * \param[in] field_config the field config for which to check setter, getter and notifier id.
   * \return a value result if field id is unique.
   * \error  SomeIpBindingErrc::json_parsing_failure if a field with matching id already exists.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> IdOfFieldMemberEntityExists(FieldConfig const& field_config) const noexcept;

  /*! Service Id */
  ServiceIdentifier service_id_{};

  /*! IsGeneric flag. This is optional and hence default initialized to false. */
  IsGeneric is_generic_{false};

  /*! Short name path for the service interface. */
  ShortnamePath shortname_path_{};

  /*! Major Version. */
  MajorVersion major_version_{};

  /*! Minor Version. */
  MinorVersion minor_version_{};

  /*! Events container. */
  EventsMap events_{};

  /*! Fields container. */
  FieldsMap fields_{};

  /*! Methods map. */
  MethodsMap methods_{};
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_INTERFACE_CONFIG_H_
