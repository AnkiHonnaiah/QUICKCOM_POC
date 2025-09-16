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
/*!        \file  required_service_instance_config.h
 *        \brief  Configuration for the RequiredServiceInstance.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/e2e_props_config.h"
#include "amsr/someip_binding/internal/configuration/service_interface_config.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Struct to hold the data for a required service instance.
 */
// VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
class RequiredServiceInstanceConfig {
 public:
  /*! Type name alias for a vector of E2EPropsConfig. */
  using E2EPropsConfigContainer = ara::core::Vector<E2EPropsConfig>;

  /*! Type name alias for a vector of Instance Specifier. */
  using InstanceSpecifierContainer = ara::core::Vector<InstanceSpecifier>;

  /*!
   * \brief  Gets the instance specifiers.
   * \return The instance specifier container.
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
  auto GetInstanceSpecifiers() const noexcept -> InstanceSpecifierContainer const&;

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
  auto GetServiceIdentifier() const noexcept -> ServiceIdentifier;

  /*!
   * \brief Sets the service identifier.
   * \param[in] service_id The service identifier.
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
  void SetServiceIdentifier(ServiceIdentifier const& service_id) noexcept;

  /*!
   * \brief  Gets the instance identifier.
   * \return The instance identifier.
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
  auto GetInstanceIdentifier() const noexcept -> InstanceIdentifier;

  /*!
   * \brief Sets the instance identifier.
   * \param[in] instance_id The instance identifier.
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
  void SetInstanceIdentifier(InstanceIdentifier const& instance_id) noexcept;

  /*!
   * \brief  Gets the major version.
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
  auto GetMajorVersion() const noexcept -> MajorVersion;

  /*!
   * \brief Sets the major version.
   * \param major_version The major version.
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
  auto GetMinorVersion() const noexcept -> MinorVersion const&;

  /*!
   * \brief Sets the minor version.
   * \param[in] minor_version The minor version.
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
   * \brief Getter implementation for the required service e2e props configurations of an instance.
   * \return a vector containing data of the e2e props configurations.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetEnd2EndProps() const noexcept -> E2EPropsConfigContainer const&;

  /*!
   * \brief Adds a new e2e props configs.
   * \param[in,out] e2e_props_config the e2e props config.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddServiceEnd2EndProps(E2EPropsConfig const& e2e_props_config) noexcept;

  /*!
   * \brief Adds an instance specifier.
   * \param[in] instance_specifier The instance specifier string.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddInstanceSpecifier(InstanceSpecifier const& instance_specifier) noexcept;

  /*!
   * \brief Getter implementation for the service interface reference of an instance.
   * \return a pointer to the service interface configuration.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetServiceInterfaceRef() const noexcept -> ServiceInterfaceConfig const&;

  /*!
   * \brief Set the service interface reference.
   * \param[in] service_interface_config Pointer to the service interface configuration.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetServiceInterfaceRef(ServiceInterfaceConfig const& service_interface_config) noexcept;

 private:
  /*! Service Id */
  ServiceIdentifier service_id_{};

  /*! The instance identifier. */
  InstanceIdentifier instance_identifier_{};

  /*! The major version */
  MajorVersion major_version_{};

  /*! The minor version. */
  MinorVersion minor_version_{};

  /*! E2e props container. */
  E2EPropsConfigContainer e2e_props_{};

  /*! InstanceSpecifier container. */
  InstanceSpecifierContainer instance_specifiers_{};

  /*! Reference to ServiceInterface. */
  ara::core::Optional<std::reference_wrapper<ServiceInterfaceConfig const>> service_interface_config_;
};

/*!
 * \brief Comparator for service instance identifiers
 */
class RequiredServiceInstanceIdentifierComparator {
 public:
  /*!
   * \brief The functor operator to perform the comparison.
   * \param[in] lhs The first instance identifier to be compared.
   * \param[in] rhs The second instance identifier to be compared.
   * \return    Returns true if the major version and service ID of both lhs and rhs are different and the instance ID
                of lhs is less than or equal to that of rhs. It also returns true if the major version and service ID of
                both lhs and rhs are the same and the instance ID of lhs is less than rhs. Returns false otherwise.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC Metric-HIS.RETURN: MD_SOMEIPBINDING_Metric-HIS.RETURN_returned_optional_contains_consts
  // VECTOR NC VectorC++-V6.6.1: MD_SOMEIPBINDING_VectorC++-V6.6.1_returned_optional_contains_consts
  auto operator()(RequiredServiceInstanceConfig const& lhs, RequiredServiceInstanceConfig const& rhs) const noexcept
      -> bool {
    bool result{true};
    if ((lhs.GetServiceIdentifier().service_identifier == rhs.GetServiceIdentifier().service_identifier) &&
        (lhs.GetMajorVersion().major_version == rhs.GetMajorVersion().major_version)) {
      result = lhs.GetInstanceIdentifier().instance_identifier < rhs.GetInstanceIdentifier().instance_identifier;
    } else {
      result = lhs.GetInstanceIdentifier().instance_identifier <= rhs.GetInstanceIdentifier().instance_identifier;
    }
    return result;
  }
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_
