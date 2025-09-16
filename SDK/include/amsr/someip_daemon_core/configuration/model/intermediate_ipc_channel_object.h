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
/*!        \file  intermediate_ipc_channel_object.h
 *        \brief  Internal representation of the IPC channel object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_IPC_CHANNEL_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_IPC_CHANNEL_OBJECT_H_
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
 * \brief This is an intermediate representation of an IPC channel object which is filled by a parser.
 * \details The validator then takes an object of this type and validates if all mandatory elements are set or not.
 */
class IpcChannelConfigObject final {
 public:
  /*!
   * \brief Domain element of this IPC channel.
   */
  using DomainElement = CfgElement<ConfigurationTypesAndDefs::IpcDomain>;

  /*!
   * \brief Port element of this IPC channel.
   */
  using PortElement = CfgElement<ConfigurationTypesAndDefs::IpcPort>;

  /*!
   * \brief Container of required service instances mapped to this IPC channel.
   */
  using RequiredServiceInstanceContainerElement =
      CfgElement<ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer>;

  /*!
   * \brief Container of provided service instances mapped to this IPC channel.
   */
  using ProvidedServiceInstanceContainerElement =
      CfgElement<ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] ipc_channel Reference to the IPC channel structure to write the parsed values to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit IpcChannelConfigObject(ConfigurationTypesAndDefs::IpcChannel& ipc_channel) noexcept
      : domain_{ipc_channel.domain},
        port_{ipc_channel.port},
        required_instances_container_{ipc_channel.required_service_instances},
        provided_instances_container_{ipc_channel.provided_service_instances} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~IpcChannelConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  IpcChannelConfigObject(IpcChannelConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  IpcChannelConfigObject(IpcChannelConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment constructor.
   */
  auto operator=(IpcChannelConfigObject const&) & -> IpcChannelConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(IpcChannelConfigObject&& other) & noexcept -> IpcChannelConfigObject& = default;

  /*!
   * \brief Set the domain when the parser finds the domain.
   * \param[in] domain The IPC channel domain.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDomain(ConfigurationTypesAndDefs::IpcDomain const& domain) noexcept { domain_.SetElement(domain); }

  /*!
   * \brief Getter for the domain element.
   * \return A reference to Domain element of this IPC channel.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetDomain() const noexcept -> DomainElement const& { return domain_; }

  /*!
   * \brief Set the port when the parser finds the port.
   * \param[in] port The IPC channel port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPort(ConfigurationTypesAndDefs::IpcPort const& port) noexcept { port_.SetElement(port); }

  /*!
   * \brief Getter for the port element.
   * \return A reference to Port element of this IPC channel.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetPort() const noexcept -> PortElement const& { return port_; }

  /*!
   * \brief Getter for the provided service instances container element.
   * \return Reference to the provided service instances container of this IPC channel.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetProvidedServiceInstances() const noexcept -> ProvidedServiceInstanceContainerElement const& {
    return provided_instances_container_;
  }

  /*!
   * \brief Set the provided service instances container after reading it in completely.
   * \param[in] provided_instances provided service instances container of provided service instances parsed.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetProvidedServiceInstances(
      ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer const& provided_instances) noexcept {
    provided_instances_container_.SetElement(provided_instances);
  }

  /*!
   * \brief Getter for the required service instances container element.
   * \return Reference to the required service instances container of this IPC channel.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  auto GetRequiredServiceInstances() const noexcept -> RequiredServiceInstanceContainerElement const& {
    return required_instances_container_;
  }

  /*!
   * \brief Set the required service instances container after reading it in completely.
   * \param[in] required_instances required service instances container of required service instances parsed.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  void SetRequiredServiceInstances(
      ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer const& required_instances) noexcept {
    required_instances_container_.SetElement(required_instances);
  }

 private:
  /*!
   * \brief The IPC channel domain.
   */
  DomainElement domain_;

  /*!
   * \brief The IPC channel port.
   */
  PortElement port_;
  /*!
   * \brief The container of required_instances this IPC channel contains.
   */
  RequiredServiceInstanceContainerElement required_instances_container_;

  /*!
   * \brief The container of provided instances this IPC channel contains.
   */
  ProvidedServiceInstanceContainerElement provided_instances_container_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_IPC_CHANNEL_OBJECT_H_
