/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ipc_channel_config.h
 *        \brief  Configuration for the IPC Channel.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_IPC_CHANNEL_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_IPC_CHANNEL_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/ipc/unicast_address.h"
#include "amsr/someip_binding/internal/configuration/provided_service_instance_config.h"
#include "amsr/someip_binding/internal/configuration/required_service_instance_config.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Class to hold the data for the IPC channel.
 */
class IpcChannelConfig final {
 public:
  /*!
   * \brief Type-alias for the IPC channel domain.
   */
  using IpcDomain = amsr::ipc::Domain;

  /*!
   * \brief Type-alias for the IPC channel port.
   */
  using IpcPort = amsr::ipc::Port;

  /*!
   * \brief Type name alias for reference of provided service instance configuration.
   */
  using ProvidedServiceInstanceConfigRef = std::reference_wrapper<ProvidedServiceInstanceConfig const>;

  /*!
   * \brief Type name alias for reference of required service instance configuration.
   */
  using RequiredServiceInstanceConfigRef = std::reference_wrapper<RequiredServiceInstanceConfig const>;

  /*!
   * \brief Type name alias for a vector of ProvidedServiceInstanceConfig references.
   */
  using ProvidedServiceInstanceConfigRefContainer = ara::core::Vector<ProvidedServiceInstanceConfigRef>;

  /*!
   * \brief Type name alias for a vector of RequiredServiceInstanceConfig references.
   */
  using RequiredServiceInstanceConfigRefContainer = ara::core::Vector<RequiredServiceInstanceConfigRef>;

  /*!
   * \brief Default value for the IPC channel domain.
   */
  static constexpr IpcDomain kIpcDomainDefaultValue{42};

  /*!
   * \brief Default value for the IPC channel port.
   */
  static constexpr IpcPort kIpcPortDefaultValue{42};

  /*!
   * \brief Default constructor.
   */
  IpcChannelConfig() noexcept = default;

  /*!
   * \brief Destructor.
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  ~IpcChannelConfig() noexcept = default;

  /*!
   * \brief Copy constructor.
   * \param[in] other Source IpcChannelConfig.
   */
  IpcChannelConfig(IpcChannelConfig const& other) noexcept
      // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
      : domain_{other.domain_},
        // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
        port_{other.port_},
        // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
        // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
        provided_service_instance_configs_{other.provided_service_instance_configs_},
        // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
        // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
        required_service_instance_configs_{other.required_service_instance_configs_} {}

  /*!
   * \brief Copy assignment.
   * \return A reference to self.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  IpcChannelConfig& operator=(IpcChannelConfig const&) & = default;

  /*!
   * \brief Move constructor.
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  IpcChannelConfig(IpcChannelConfig&&) noexcept = default;

  /*!
   * \brief Move assignment operator.
   * \return A reference to self.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  IpcChannelConfig& operator=(IpcChannelConfig&&) & noexcept = default;

  /*!
   * \brief     Get the IPC channel domain.
   * \return    The IPC channel domain.
   *
   * \pre       -
   * \context   Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   */
  auto GetIpcChannelDomain() const -> IpcDomain;

  /*!
   * \brief     Set the IPC channel domain.
   * \param[in] domain the IPC channel domain.
   *
   * \pre       -
   * \context   Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   */
  void SetIpcChannelDomain(IpcDomain const& domain);

  /*!
   * \brief     Get the IPC channel port.
   * \return    The IPC channel port.
   *
   * \pre       -
   * \context   Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   */
  auto GetIpcChannelPort() const -> IpcPort;

  /*!
   * \brief     Set the IPC channel port.
   * \param[in] port the IPC channel port.
   *
   * \pre       -
   * \context   Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   */
  void SetIpcChannelPort(IpcPort const& port);

  /*!
   * \brief Add a provided service instance config.
   * \pre -
   * \param[in] provided_service_instance_config the configuration of the provided service instance.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void AddProvidedServiceInstanceConfig(ProvidedServiceInstanceConfig const& provided_service_instance_config) noexcept;

  /*!
   * \brief Add a required service instance config.
   * \pre -
   * \param[in] required_service_instance_config the configuration of the required service instance.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void AddRequiredServiceInstanceConfig(RequiredServiceInstanceConfig const& required_service_instance_config) noexcept;

  /*!
   * \brief Getter for the provided service instance configs.
   * \return All provided service instance config data.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetProvidedServiceInstanceConfigs() const noexcept -> ProvidedServiceInstanceConfigRefContainer;

  /*!
   * \brief Getter for the required service instance configs.
   * \return All required service instance config data.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetRequiredServiceInstanceConfigs() const noexcept -> RequiredServiceInstanceConfigRefContainer;

 private:
  /*!
   * \brief Typename alias for a set of ProvidedServiceInstanceConfigs.
   */
  using ProvidedServiceInstanceConfigContainer =
      std::set<ProvidedServiceInstanceConfig, ProvidedServiceInstanceIdentifierComparator>;

  /*!e
   * \brief Typename alias for a set of RequiredServiceInstanceConfigs.
   */
  using RequiredServiceInstanceConfigContainer =
      std::set<RequiredServiceInstanceConfig, RequiredServiceInstanceIdentifierComparator>;

  /*!
   * \brief The IPC channel domain.
   */
  IpcDomain domain_{kIpcDomainDefaultValue};

  /*!
   * \brief The IPC channel port.
   */
  IpcPort port_{kIpcPortDefaultValue};

  /*!
   * \brief The set of provided service instance configs.
   */
  ProvidedServiceInstanceConfigContainer provided_service_instance_configs_{};

  /*!
   * \brief The set of required service instance configs.
   */
  RequiredServiceInstanceConfigContainer required_service_instance_configs_{};
};

/*!
 * \brief Comparator for ipc channels.
 */
class IpcChannelDomainPortComparator {
 public:
  /*!
   * \brief The functor operator to perform the comparison.
   * \param[in] lhs The first ipc channel to be compared.
   * \param[in] rhs The second ipc channel to be compared.
   * \return True if the tuple of ipc channel domain and port of lhs is less than that of rhs.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator()(IpcChannelConfig const& lhs, IpcChannelConfig const& rhs) const noexcept -> bool {
    return std::make_tuple(lhs.GetIpcChannelDomain(), lhs.GetIpcChannelPort()) <
           std::make_tuple(rhs.GetIpcChannelDomain(), rhs.GetIpcChannelPort());
  }
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_IPC_CHANNEL_CONFIG_H_
