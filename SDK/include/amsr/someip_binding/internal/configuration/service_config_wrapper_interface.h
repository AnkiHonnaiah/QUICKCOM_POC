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
/*!        \file  service_config_wrapper_interface.h
 *        \brief  Interface for validation wrapper of all services.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {
/*!
 * \brief Interface defining a configuration wrapper for safe access of all service config objects.
 */
class ServiceConfigWrapperInterface {
 public:
  /*!
   * \brief Struct containing information of validated service entities.
   */
  struct ValidatedServiceConfig {
    /*!
     * \brief The validated (with respect to events, methods, fields) config wrapper object the respective service .
     */
    std::unique_ptr<ConfigWrapper> config_wrapper_;
    /*!
     * \brief The validated(with respect to shortname path) service object the respective service.
     */
    SomeIpBindingConfig::ServiceInterfaceConfigRef service_config_ref_;
  };

  /*!
   * \brief Map type to store validated services. Key: shortname path, Value: ValidatedServiceConfig.
   */
  using ServiceConfigWrapperMap =
      std::unordered_map<ShortnamePath, ValidatedServiceConfig, ShortnamePathHash, ShortnamePathEqualComparator>;

  /*!
   * \brief Type for references to validated services.
   */
  using ValidatedServiceConfigRef = std::reference_wrapper<ValidatedServiceConfig const>;

  /*!
   * \brief Use default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceConfigWrapperInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServiceConfigWrapperInterface() = default;

  ServiceConfigWrapperInterface(ServiceConfigWrapperInterface const&) = delete;
  ServiceConfigWrapperInterface(ServiceConfigWrapperInterface&&) = delete;
  ServiceConfigWrapperInterface& operator=(ServiceConfigWrapperInterface const&) & = delete;
  ServiceConfigWrapperInterface& operator=(ServiceConfigWrapperInterface&&) & = delete;

  /*!
   * \brief Getter for the validated service configuration.
   * \param[in] service_shortname_path the service shortname path related to the service.
   * \return The validated service configurations (pair of: ConfigWrapper, ServiceInterfaceConfig) associated with the
   *         provided service shortname path.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ValidatedServiceConfigRef GetServiceInterface(ShortnamePath const& service_shortname_path) const noexcept = 0;

  /*!
   * \brief Getter for the map of validated service configurations.
   * \return The map of validated service configurations (pair of: ConfigWrapper, ServiceInterfaceConfig).
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ServiceConfigWrapperMap const& GetServiceInterfaces() const noexcept = 0;
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SERVICE_CONFIG_WRAPPER_INTERFACE_H_
