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
/*!        \file  config_wrapper.h
 *        \brief  Validation wrapper of config of services.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::ConfigWrapper
 *
 *   \complexity  This unit provides Getters, Setters and Validator functionality for the configuration at start-up.
 *                It can still be considered of low complexity based on the following reasons:
 *                  - It has no further interactions with the component during runtime.
 *                  - It can be completely tested.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <memory>
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/method_config.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "amsr/someip_binding/internal/configuration/operation_config.h"
#include "amsr/someip_binding/internal/configuration/service_interface_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using namespace ::vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief   Configuration wrapper for save access of config objects.
 */
class ConfigWrapper : public ConfigWrapperInterface {
 public:
  /*!
   * \brief Method to create and validate a config wrapper object hosted by a unique pointer.
   * \param[in] events                        Reference to events runtime configuration.
   * \param[in] fields                        Reference to fields runtime configuration.
   * \param[in] methods                       Reference to methods runtime configuration.
   * \param[in] config_validation_data_access Reference to configuration validation information.
   * \return Unique pointer to created config wrapper.
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static std::unique_ptr<ConfigWrapper> Create(ServiceInterfaceConfig::EventsMap const& events,
                                               ServiceInterfaceConfig::FieldsMap const& fields,
                                               ServiceInterfaceConfig::MethodsMap const& methods,
                                               ConfigValidationDataAccess const& config_validation_data_access);

  /*!
   * \brief Constructor.
   * \param[in] events                        Reference to events runtime configuration.
   * \param[in] fields                        Reference to fields runtime configuration.
   * \param[in] methods                       Reference to methods runtime configuration.
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigWrapper(ServiceInterfaceConfig::EventsMap const& events, ServiceInterfaceConfig::FieldsMap const& fields,
                ServiceInterfaceConfig::MethodsMap const& methods);

  ConfigWrapper(ConfigWrapper const&) = delete;
  ConfigWrapper(ConfigWrapper&&) = delete;

  ConfigWrapper& operator=(ConfigWrapper const&) = delete;
  ConfigWrapper& operator=(ConfigWrapper&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ConfigWrapper() override = default;

  /*!
   * \brief Getter for EventConfig.
   * \param[in] shortname The shortname of the associated event of the EventConfig.
   * \return The EventConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding::internal::configuration::EventConfig const& GetEventConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept override;

  /*!
   * \brief Getter for MethodConfig.
   * \param[in] shortname The shortname of the associated method of the MethodConfig.
   * \return The MethodConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding::internal::configuration::MethodConfig const& GetMethodConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept override;

  /*!
   * \brief Getter for NotifierConfig.
   * \param[in] shortname The shortname of the associated event of the NotifierConfig.
   * \return The NotifierConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding::internal::configuration::NotifierConfig const& GetNotifierConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept override;
  /*!
   * \brief Getter for GetterConfig.
   * \param[in] shortname The shortname of the associated field of the GetterConfig.
   * \return The GetterConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding::internal::configuration::OperationConfig const& GetGetterConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept override;

  /*!
   * \brief Getter for SetterConfig.
   * \param[in] shortname The shortname of the associated field of the SetterConfig.
   * \return The SetterConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding::internal::configuration::OperationConfig const& GetSetterConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept override;

 private:
  /*!
   * \brief Validation method for the events config map.
   * \param[in] config_validation_data_access Reference to the config validation data getter.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void ValidateEventsMap(ConfigValidationDataAccess const& config_validation_data_access) const;

  /*!
   * \brief Validation method for the methods config map.
   * \param[in]    config_validation_data_access Reference to the config validation data getter.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void ValidateMethodsMap(ConfigValidationDataAccess const& config_validation_data_access) const;

  /*!
   * \brief Validation method for the fields config map.
   * \param[in] config_validation_data_access Reference to the config validation data getter.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void ValidateFieldsMap(ConfigValidationDataAccess const& config_validation_data_access) const;

  /*!
   * \brief Reference to events config map.
   */
  ::amsr::someip_binding::internal::configuration::ServiceInterfaceConfig::EventsMap const& events_;

  /*!
   * \brief Reference to fields config map.
   */
  ::amsr::someip_binding::internal::configuration::ServiceInterfaceConfig::FieldsMap const& fields_;

  /*!
   * \brief Reference to methods config map.
   */
  ::amsr::someip_binding::internal::configuration::ServiceInterfaceConfig::MethodsMap const& methods_;

  /*!
   * \brief Method logger for printing debug and error messages.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger const logger_;
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_H_
