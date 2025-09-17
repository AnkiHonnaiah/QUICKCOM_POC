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
/*! \file
 *  \brief Configurable variables.
 *  \unit IpcServiceDiscovery::Centralized::Config
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_CONFIG_CONFIG_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_CONFIG_CONFIG_H_

#include <chrono>
#include "amsr/ipc/unicast_address.h"
#include "amsr/ipc_service_discovery/centralized/internal/config/environment_variable.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_IPCSERVICEDISCOVERY_Metric-OO.WMC.One_weightedMethodsPerClassTooHigh
/*!
 * \brief A class for loading parameter from a config or their default value.
 * \details The Config class contains static methods for different parameters used by the IpcServiceDiscovery.
 *          For the implemented parameter, the value is searched in the environment variable list, if found,
 *          this value is used. If not found, a hard-coded default parameter is used.
 * \vprivate Component Private
 */
class Config {
 public:
  /*!
   * \brief Get the environment variable name for the ConnectionEstablishmentRetryTime parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetConnectionEstablishmentRetryTimeName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the ConnectionEstablishmentRetryTime parameter.
   * \return The default parameter value as std::chrono::milliseconds.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetConnectionEstablishmentRetryTimeDefault() noexcept -> std::chrono::milliseconds;
  /*!
   * \brief Get the value for ConnectionEstablishmentRetryTime parameter.
   * \details This method returns the value of the ConnectionEstablishmentRetryTime parameter. If the parameter value
   *          is set as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The ConnectionEstablishmentRetryTime parameter value as std::chrono::milliseconds.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetConnectionEstablishmentRetryTime() noexcept -> std::chrono::milliseconds;

  /*!
   * \brief Get the environment variable name for the ServerIpcConnectionBufferSize parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetServerIpcConnectionBufferSizeName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the ServerIpcConnectionBufferSize parameter.
   * \return The default parameter value as size_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetServerIpcConnectionBufferSizeDefault() noexcept -> size_t;
  /*!
   * \brief Get the value for ServerIpcConnectionBufferSize parameter.
   * \details This method returns the value of the ServerIpcConnectionBufferSize parameter. If the parameter value is
   *          set as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The ServerIpcConnectionBufferSize parameter value as size_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetServerIpcConnectionBufferSize() noexcept -> size_t;

  /*!
   * \brief Get the environment variable name for the ClientIpcConnectionBufferSize parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetClientIpcConnectionBufferSizeName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the ClientIpcConnectionBufferSize parameter.
   * \return The default parameter value as size_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetClientIpcConnectionBufferSizeDefault() noexcept -> size_t;
  /*!
   * \brief Get the value for ClientIpcConnectionBufferSize parameter.
   * \details This method returns the value of the ClientIpcConnectionBufferSize parameter. If the parameter value is
   *          set as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The ClientIpcConnectionBufferSize parameter value as size_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetClientIpcConnectionBufferSize() noexcept -> size_t;

  /*!
   * \brief Get the environment variable name for the IpcServiceDiscoveryPort parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryPortName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the IpcServiceDiscoveryPort parameter.
   * \return The default parameter value as ::amsr::ipc::Port.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryPortDefault() noexcept -> ::amsr::ipc::Port;
  /*!
   * \brief Get the value for GetIpcServiceDiscoveryPort parameter.
   * \details This method returns the value of the GetIpcServiceDiscoveryPort parameter. If the parameter value is set
   *          as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The GetIpcServiceDiscoveryPort parameter value as ::amsr::ipc::Port.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryPort() noexcept -> ::amsr::ipc::Port;

  /*!
   * \brief Get the environment variable name for the IpcServiceDiscoveryDomain parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryDomainName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the IpcServiceDiscoveryDomain parameter.
   * \return The default parameter value as ::amsr::ipc::Domain.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryDomainDefault() noexcept -> ::amsr::ipc::Domain;
  /*!
   * \brief Get the value for GetIpcServiceDiscoveryDomain parameter.
   * \details This method returns the value of the GetIpcServiceDiscoveryDomain parameter. If the parameter value is
   *          set as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The GetIpcServiceDiscoveryDomain parameter value as ::amsr::ipc::Domain.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryDomain() noexcept -> ::amsr::ipc::Domain;

  /*!
   * \brief Get the default value for the IpcServiceDiscoveryUnicastAddress parameter.
   * \return The default parameter value as ::amsr::ipc::UnicastAddress.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryUnicastAddressDefault() noexcept -> ::amsr::ipc::UnicastAddress;
  /*!
   * \brief Get the value for IpcServiceDiscoveryUnicastAddress parameter.
   * \details This method returns the value of the IpcServiceDiscoveryUnicastAddress parameter. If the parameter value
   *          is set as environment variable, this value is used. Otherwise, the default value is returned.
   * \return The IpcServiceDiscoveryUnicastAddress parameter value as ::amsr::ipc::UnicastAddress.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcServiceDiscoveryUnicastAddress() noexcept -> ::amsr::ipc::UnicastAddress;

  /*!
   * \brief Get the environment variable name for the NumberOfServerReactorSlots parameter.
   * \return The name of the parameter as a CStringView.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetNumberOfServerReactorSlotsName() noexcept -> vac::container::CStringView;
  /*!
   * \brief Get the default value for the NumberOfServerReactorSlots parameter.
   * \return The default parameter value as uint16_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetNumberOfServerReactorSlotsDefault() noexcept -> uint16_t;
  /*!
   * \brief Get the value for NumberOfServerReactorSlots parameter.
   * \details This method returns the value of the NumberOfServerReactorSlots parameter. If the parameter value is set
   *          as environment variable, this value is used. Otherwise, the default value is returned. A Connection uses
   *          up to 3 Reactor callback slots. An Acceptor might use up to 4.
   * \return The NumberOfServerReactorSlots parameter value as uint16_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetNumberOfServerReactorSlots() noexcept -> uint16_t;

 private:
  /*!
   * \brief Get the default value for the IpcConnectionBufferSize parameter.
   * \return The default parameter value as size_t.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto GetIpcConnectionBufferSizeDefault() noexcept -> size_t;
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_CONFIG_CONFIG_H_
