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
/*!        \file  configuration_object.h
 *        \brief  Configuration object for a someip_config.json.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_CONFIGURATION_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_CONFIGURATION_OBJECT_H_
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
 * \brief This is an intermediate representation of a someip_config.json which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class ConfigurationObject final {
 public:
  /*!
   * \brief The services container element.
   */
  using ServicesContainerElement = CfgElement<ConfigurationTypesAndDefs::ServiceContainer>;

  /*!
   * \brief The IPC channels container element.
   */
  using IpcChannelsContainerElement = CfgElement<ConfigurationTypesAndDefs::IpcChannelsContainer>;

  /*!
   * \brief The network end points container element.
   */
  using NetworkEndPointsElement = CfgElement<ConfigurationTypesAndDefs::NetworkEndpointContainer>;

  /*!
   * \brief The Secure com element.
   */
  using SecureComElement = CfgElement<ConfigurationTypesAndDefs::SecureCom>;

  /*!
   * \brief The Generator version element.
   */
  using GeneratorVersionElement = CfgElement<ConfigurationTypesAndDefs::GeneratorVersion>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] services_container Reference to the 'services_container' container to write to.
   * \param[in] ipc_channels_container Reference to the 'ipc_channels_container' container to write to.
   * \param[in] network_end_points_container Reference to the 'network_end_points' container to write to.
   * \param[in] secure_com Reference to the secure_com object to write to.
   * \param[in] generator_version Reference to the generator_version object to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ConfigurationObject(ConfigurationTypesAndDefs::ServiceContainer& services_container,
                      ConfigurationTypesAndDefs::IpcChannelsContainer& ipc_channels_container,
                      ConfigurationTypesAndDefs::NetworkEndpointContainer& network_end_points_container,
                      ConfigurationTypesAndDefs::SecureCom& secure_com,
                      ConfigurationTypesAndDefs::GeneratorVersion& generator_version) noexcept
      : services_container_{services_container},
        ipc_channels_container_{ipc_channels_container},
        network_end_points_container_{network_end_points_container},
        secure_com_{secure_com},
        generator_version_{generator_version} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ConfigurationObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ConfigurationObject(ConfigurationObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ConfigurationObject(ConfigurationObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ConfigurationObject const&) & -> ConfigurationObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ConfigurationObject&&) & -> ConfigurationObject& = delete;

  /*!
   * \brief Set the services container.
   * \param[in] services_container Reference to the service container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServices(ConfigurationTypesAndDefs::ServiceContainer const& services_container) noexcept {
    services_container_.SetElement(services_container);
  }

  /*!
   * \brief Getter for the services container element.
   * \return Reference to the service container of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ServicesContainerElement const& GetServices() const noexcept { return services_container_; }

  /*!
   * \brief Set the IPC channels container.
   * \param[in] ipc_channels_container Reference to the IPC channels container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIpcChannels(ConfigurationTypesAndDefs::IpcChannelsContainer const& ipc_channels_container) noexcept {
    ipc_channels_container_.SetElement(ipc_channels_container);
  }

  /*!
   * \brief Getter for the IPC channels container element.
   * \return Reference to the IPC channels container of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IpcChannelsContainerElement const& GetIpcChannels() const noexcept { return ipc_channels_container_; }

  /*!
   * \brief Set the network end points container.
   * \param[in] network_end_points_container Reference to the network end points container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetNetworkEndPoints(
      ConfigurationTypesAndDefs::NetworkEndpointContainer const& network_end_points_container) noexcept {
    network_end_points_container_.SetElement(network_end_points_container);
  }

  /*!
   * \brief Getter for the network end points container element.
   * \return Reference to the  network end points container of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  NetworkEndPointsElement const& GetNetworkEndPoints() const noexcept { return network_end_points_container_; }

  /*!
   * \brief Set the Secure com.
   * \param secure_com Reference to the secure com object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSecureCom(ConfigurationTypesAndDefs::SecureCom const& secure_com) noexcept {
    secure_com_.SetElement(secure_com);
  }

  /*!
   * \brief Set the generator version.
   * \param[in] generator_version Reference to the generator version object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetGeneratorVersion(ConfigurationTypesAndDefs::GeneratorVersion const& generator_version) noexcept {
    generator_version_.SetElement(generator_version);
  }

 private:
  /*!
   * \brief The services container element.
   */
  ServicesContainerElement services_container_;

  /*!
   * \brief The IPC channels container element.
   */
  IpcChannelsContainerElement ipc_channels_container_;

  /*!
   * \brief The network end points container element.
   */
  NetworkEndPointsElement network_end_points_container_;

  /*!
   * \brief The Secure com element.
   */
  SecureComElement secure_com_;

  /*!
   * \brief The Generator version element.
   */
  GeneratorVersionElement generator_version_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_CONFIGURATION_OBJECT_H_
