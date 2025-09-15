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
/*!        \file  service_config_wrapper.h
 *        \brief  Validation wrapper for all services.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::ConfigWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper.h"
#include "amsr/someip_binding/internal/configuration/service_config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/service_interface_config.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief   Configuration wrapper for safe access of all service config objects.
 */
class ServiceConfigWrapper : public ServiceConfigWrapperInterface {
 public:
  /*!
   * \brief Method to create and validate a service config wrapper object hosted by a unique pointer.
   * \param[in] services                      Container of all services to be validated.
   * \param[in] config_validation_data_access Reference to configuration validation information.
   * \return Unique pointer to created service config wrapper.
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static std::unique_ptr<ServiceConfigWrapper> Create(
      SomeIpBindingConfig::ServiceInterfaceConfigRefContainer const services,
      ServiceConfigValidationDataAccess const& config_validation_data_access) noexcept;

  /*!
   * \brief Constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceConfigWrapper();

  ServiceConfigWrapper(ServiceConfigWrapper const&) = delete;
  ServiceConfigWrapper(ServiceConfigWrapper&&) = delete;
  ServiceConfigWrapper& operator=(ServiceConfigWrapper const&) = delete;
  ServiceConfigWrapper& operator=(ServiceConfigWrapper&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~ServiceConfigWrapper() override = default;

  /*!
   * \copydoc amsr::someip_binding::internal::configuration::ServiceConfigWrapperInterface::GetServiceInterface
   * \spec
   *   requires true;
   * \endspec
   */
  ValidatedServiceConfigRef GetServiceInterface(ShortnamePath const& service_shortname_path) const noexcept override;

  /*!
   * \copydoc amsr::someip_binding::internal::configuration::ServiceConfigWrapperInterface::GetServiceInterfaces
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceConfigWrapperMap const& GetServiceInterfaces() const noexcept override;

 private:
  /*!
   * \brief Validates if deployed services match the designed services.
   * \param[in] services                      Container of all services to be validated.
   * \param[in] config_validation_data_access Reference to configuration validation information.
   * \pre -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - For each deployed service:
   *   - Check if deployed service shortname path matches any of the designed services shortname paths.
   *   - If no matching designed service was found:
   *     - Log error message and abort.
   *   - Else:
   *     - Create a ConfigWrapper object to validate the services events, methods and fields.
   *     - Store pair of validated ConfigWrapper and ServiceInterfaceConfig in local map.
   * \endinternal
   */
  void ValidateConfig(SomeIpBindingConfig::ServiceInterfaceConfigRefContainer const services,
                      ServiceConfigValidationDataAccess const& config_validation_data_access) noexcept;
  /*!
   * \brief Logger for printing debug and error messages.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger const logger_;

  /*!
   * \brief The container holding all the validated service entities (pair of: ConfigWrapper, ServiceInterfaceConfig).
   */
  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
  // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
  ServiceConfigWrapperMap config_wrappers_{};
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_H_
