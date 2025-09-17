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
/*!        \file  someipbinding_config.h
 *        \brief  Configuration for the SomeIpBinding.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SOMEIPBINDING_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SOMEIPBINDING_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <map>
#include <set>
#include <unordered_map>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/e2e_profile_config.h"
#include "amsr/someip_binding/internal/configuration/e2e_props_config.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/field_config.h"
#include "amsr/someip_binding/internal/configuration/ipc_channel_config.h"
#include "amsr/someip_binding/internal/configuration/service_interface_config.h"
#include "amsr/timer/timer.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace std::literals::chrono_literals;  // NOLINT(build/namespaces)

/*!
 * \brief Enumeration defining the available runtime processing modes.
 */
enum class RuntimeProcessingMode : std::uint8_t {
  /*!
   * \brief Runtime processing mode 'Polling'.
   */
  polling = 0,

  /*!
   * \brief Runtime processing mode 'Thread Driven'.
   */
  thread_driven = 1
};

// VECTOR NC Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief SomeIpBinding configuration.
 * \details Configuration class storing the configuration parameters for the SomeIpBinding.
 *          Stored configuration parameters:
 *          - SomeIpDaemon addressing information: IPC domain number
 *          - SomeIpDaemon addressing information: IPC port number
 */
// VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
class SomeIpBindingConfig final {
 public:
  /*!
   * \brief Type-alias for the SOME/IP daemon IPC domain
   */
  using IpcDomain = amsr::ipc::Domain;

  /*!
   * \brief Type-alias for the SOME/IP daemon IPC port
   */
  using IpcPort = amsr::ipc::Port;

  /*!
   * \brief Type-alias for the maximum size of the IPC transmission queue.
   */
  using IpcMaxQueueSize = std::uint32_t;

  /*!
   * \brief Type-alias for the connection retrial interval in milliseconds to the SOME/IP Daemon.
   */
  using SomeIpDaemonConnectionRetrialIntervalMs = std::uint32_t;

  /*!
   * \brief Type name alias for amsr::someip_protocol::internal::ServiceId
   */
  using ServiceIdentifier = ::amsr::someip_binding::internal::configuration::ServiceIdentifier;

  /*!
   * \brief Type name alias for reference of service interface config.
   */
  using ServiceInterfaceConfigRef = std::reference_wrapper<ServiceInterfaceConfig const>;

  /*!
   * \brief Type name alias for vector of references of service interface configs.
   */
  using ServiceInterfaceConfigRefContainer = ara::core::Vector<ServiceInterfaceConfigRef>;

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
   * \brief Type name alias for e2e profile map
   */
  using E2EProfileMap = std::map<E2EProfileConfig::E2EShortname, E2EProfileConfig, ShortnameComparator>;

  /*!
   * \brief Type name alias for ipc channel config.
   */
  using IpcChannelConfig = ::amsr::someip_binding::internal::configuration::IpcChannelConfig;

  /*!
   * \brief Type name alias for set of ipc channel configs.
   */
  using IpcChannelConfigContainer = std::set<IpcChannelConfig, IpcChannelDomainPortComparator>;

  /*!
   * \brief Type name alias for reference of IPC channel configuration.
   */
  using IpcChannelConfigRef = std::reference_wrapper<IpcChannelConfig const>;

  /*!
   * \brief Type name alias for a vector of IpcChannelConfig references.
   */
  using IpcChannelConfigRefContainer = ara::core::Vector<IpcChannelConfigRef>;

  /*!
   * \brief Default value for runtime processing mode.
   */
  static constexpr RuntimeProcessingMode kRuntimeProcessingModeDefaultValue{RuntimeProcessingMode::thread_driven};

  /*!
   * \brief Default value for the SomeIpDaemon domain.
   */
  static constexpr IpcDomain kSomeIpDaemonDomainDefaultValue{42};

  /*!
   * \brief Default value for the SomeIpDaemon port.
   */
  static constexpr IpcDomain kSomeIpDaemonPortDefaultValue{42};

  /*!
   * \brief Default maximum size of the IPC transmission queue.
   */
  static constexpr IpcMaxQueueSize kDefaultSomeipDaemonClientIpcMaxQueueSize{0};

  /*!
   * \brief Default value of the SOME/IP Daemon connection retrial enable flag.
   */
  static constexpr bool kDefaultSomeIpDaemonConnectionRetrialEnabled{false};

  /*!
   * \brief Default connection retrial interval in milliseconds to the SOME/IP Daemon.
   */
  static constexpr SomeIpDaemonConnectionRetrialIntervalMs kDefaultSomeIpDaemonConnectionRetrialIntervalMs{1000};

  /*!
   * \brief     Get the configured runtime processing mode.
   * \return    runtime processing mode.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  RuntimeProcessingMode GetRuntimeProcessingMode() const;

  /*!
   * \brief     Set the runtime processing mode.
   * \param[in] mode Runtime processing mode.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetRuntimeProcessingMode(RuntimeProcessingMode const& mode);

  /*!
   * \brief Set the maximum size of the IPC transmission queue.
   * \details The size defines the maximum size of the IPC transmission queue.
   * \param[in] size The maximum queue size.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetIpcMaxQueueSize(IpcMaxQueueSize const size) noexcept;

  /*!
   * \brief Get the maximum size of the IPC transmission queue.
   * \return The maximum size of the IPC transmission queue.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  IpcMaxQueueSize GetIpcMaxQueueSize() const noexcept;

  /*!
   * \brief Set the SOME/IP Daemon connection retrial enable flag.
   * \details The flag enables connection retrial if the SOME/IP Daemon is not available on application startup. It uses
   * the provided retrial interval to retry the connection indefinitely without shutting down the application in order
   * to allow the application process to be started before the SOME/IP Daemon process.
   * \param[in] enabled SOME/IP Daemon connection retrial enable flag.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSomeIpDaemonConnectionRetrialEnabled(bool enabled) noexcept;

  /*!
   * \brief Get the SOME/IP Daemon connection retrial enable flag.
   * \details The flag enables connection retrial if the SOME/IP Daemon is not available on application startup. It uses
   * the provided retrial interval to retry the connection indefinitely without shutting down the application in order
   * to allow the application process to be started before the SOME/IP Daemon process.
   * \return SOME/IP Daemon connection retrial enable flag.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool GetSomeIpDaemonConnectionRetrialEnabled() const noexcept;

  /*!
   * \brief Set the SOME/IP Daemon connection retrial interval in milliseconds.
   * \details The interval determines the time between each connection retrial to the SOME/IP Daemon if the connection
   * retrial feature is enabled.
   * \param[in] interval_ms SOME/IP Daemon connection retrial interval in milliseconds.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSomeIpDaemonConnectionRetrialIntervalMs(SomeIpDaemonConnectionRetrialIntervalMs interval_ms) noexcept;

  /*!
   * \brief Get the SOME/IP Daemon connection retrial interval in milliseconds.
   * \details The interval determines the time between each connection retrial to the SOME/IP Daemon if the connection
   * retrial feature is enabled.
   * \return SOME/IP Daemon connection retrial interval in milliseconds.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpDaemonConnectionRetrialIntervalMs GetSomeIpDaemonConnectionRetrialIntervalMs() const noexcept;

  /*!
   * \brief Adds a new mapping between a service ID and a service interface config.
   * \pre -
   * \param[in]     service_shortname_path the shortname path of the service, to which the service interface
   *                configuration belongs.
   * \param[in,out] service_interface the configuration of the service interface.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddServiceInterfaceMapping(ShortnamePath const& service_shortname_path,
                                  ServiceInterfaceConfig&& service_interface) noexcept;

  /*!
   * \brief Getter for the service interface configuration.
   * \param[in] service_shortname_path the service shortname path related to the service interface.
   * \return The service interface associated with the provided service shortname path.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceInterfaceConfig const& GetServiceInterface(ShortnamePath const& service_shortname_path) const noexcept;

  /*!
   * \brief Getter for the service interface configurations.
   * \return The vector of references of service interface configs.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceInterfaceConfigRefContainer GetServiceInterfaces() const noexcept;

  /*!
   * \brief Adds an IpcChannelConfig to the IPC channels config container.
   * \param[in] ipc_channel_config the IPC channel config.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void AddIpcChannel(IpcChannelConfig const&& ipc_channel_config) noexcept;

  /*!
   * \brief Getter for the IPC channel configurations.
   * \return The IPC channels config container.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetIpcChannels() const noexcept -> IpcChannelConfigRefContainer;

  /*!
   * \brief Adds an E2EProfileConfig to the map of E2EProfileConfigs
   * \param[in] e2e_profile_config the e2e profile config
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddE2EProfile(E2EProfileConfig&& e2e_profile_config) noexcept;

  /*!
   * \brief Getter for the E2EProfileConfig map.
   * \return The E2EProfileConfig map.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfileMap const& GetE2EProfileConfigMap() const noexcept;

  /*!
   * \brief Getter for the E2EProfileConfig based on shortname
   * \param[in] shortname the shortname of the E2EProfileConfig
   * \return The E2EProfileConfig that corresponds to the shortname
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<E2EProfileConfig::E2EProfileConfigRef> E2EProfileConfigOf(
      E2EProfileConfig::E2EShortname const& shortname) const noexcept;

 private:
  /*!
   * \brief Runtime processing mode.
   */
  RuntimeProcessingMode runtime_processing_mode_{kRuntimeProcessingModeDefaultValue};

  /*!
   * \brief The maximum size of the IPC transmission queue.
   */
  IpcMaxQueueSize ipc_max_queue_size_{kDefaultSomeipDaemonClientIpcMaxQueueSize};

  /*!
   * \brief SOME/IP Daemon connection retrial enable flag.
   */
  bool someipdaemon_connection_retrial_enabled_{kDefaultSomeIpDaemonConnectionRetrialEnabled};

  /*!
   * \brief Connection retrial interval in milliseconds to the SOME/IP Daemon.
   */
  SomeIpDaemonConnectionRetrialIntervalMs someipdaemon_connection_retrial_interval_ms_{
      kDefaultSomeIpDaemonConnectionRetrialIntervalMs};

  /*!
   * \brief The map of service identifiers and the associated service interface.
   */
  std::unordered_map<ShortnamePath, ServiceInterfaceConfig, ShortnamePathHash, ShortnamePathEqualComparator>
      service_interfaces_{};

  /*!
   * \brief The map of e2e profiles extracted.
   */
  E2EProfileMap e2e_profile_config_map_{};

  /*!
   * \brief The set of IPC channels.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  IpcChannelConfigContainer ipc_channels_{};
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_SOMEIPBINDING_CONFIG_H_
