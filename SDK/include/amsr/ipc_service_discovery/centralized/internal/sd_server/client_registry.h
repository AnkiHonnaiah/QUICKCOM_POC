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
 *  \brief A registry for registration of SdClients managed by the SdServer.
 *  \unit IpcServiceDiscovery::Centralized::SdServer
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_CLIENT_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_CLIENT_REGISTRY_H_

#include <unordered_set>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {
namespace sd_server {

/*!
 * \brief Registry for SdClients of the centralized backend.
 * \details Clients who want to participate in the centralized IpcServiceDiscovery backend must register by sending
 *          an InitMessage with a valid Version and a BindingType. The ClientRegistry keeps track of registered clients
 *          and additional information regarding the clients required and provided services.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerClientRegistry
 */
// VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
class ClientRegistry final {
 public:
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Entry for the ClientRegistry.
   */
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  struct Entry {
    BindingType binding_type{};
    std::unordered_set<ProvidedServiceInstanceIdentifier> provided_service_instances{};
    std::unordered_set<RequiredServiceInstanceIdentifier> required_service_instances{};
  };

  /*!
   * \brief Register a client at the ClientRegistry.
   * \details If the client is already registered, this function aborts.
   * \param[in] connection_handle Handle to the client connection.
   * \param[in] binding_type Binding type of the client.
   * \param[in] provided_service_instances Service instances provided by the client.
   * \param[in] required_service_instances Service instances required by the client.
   * \context ANY
   * \pre The client must not be currently registered.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Emplace a new entry into the registry.
   * \endinternal
   */
  void Register(ConnectionHandle connection_handle, BindingType binding_type,
                std::unordered_set<ProvidedServiceInstanceIdentifier>&& provided_service_instances = {},
                std::unordered_set<RequiredServiceInstanceIdentifier>&& required_service_instances = {}) noexcept {
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(client_registry_map_.find(connection_handle) == client_registry_map_.end());
    Entry client_registry_entry{binding_type, std::move(provided_service_instances),
                                std::move(required_service_instances)};
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = client_registry_map_.emplace(connection_handle, std::move(client_registry_entry));
  }

  /*!
   * \brief Unregister a client from the ClientRegistry.
   * \details If the client is not currently registered, this function aborts.
   * \param[in] connection_handle Handle to the client connection.
   * \context ANY
   * \pre The client must currently be registered.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Erase the entry from the registry.
   * \endinternal
   */
  void Unregister(ConnectionHandle connection_handle) noexcept {
    // This always succeeds, if precondition is not violated.
    assert(client_registry_map_.find(connection_handle) != client_registry_map_.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = client_registry_map_.erase(connection_handle);
  }

  /*!
   * \brief Get information stored in the ClientRegistry for a given client.
   * \param[in] connection_handle Handle to the client connection.
   * \return A result containing a reference to the ClientRegistry entry or an error.
   * \error IpcServiceDiscoveryCentralizedErrc::kClientNotRegistered The client is not currently registered.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - If client is registered:
   *    - Return the entry.
   *  - Else:
   *    - Return kClientNotRegistered.
   * \endinternal
   */
  auto GetClientInfo(ConnectionHandle connection_handle) const noexcept -> ::ara::core::Result<Entry const&> {
    ::ara::core::Result<Entry const&> get_client_info_result{CentralizedErrc::kClientNotRegistered};
    auto const iter = client_registry_map_.find(connection_handle);
    if (iter != client_registry_map_.end()) {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      get_client_info_result.EmplaceValue(iter->second);
    }
    return get_client_info_result;
  }

  /*!
   * \brief Check whether a client is currently registered.
   * \param[in] connection_handle Handle to the client connection.
   * \return true, if the client is registered, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - If client is registered:
   *    - Return true.
   *  - Else:
   *    - Return false.
   * \endinternal
   */
  auto IsRegistered(ConnectionHandle connection_handle) const noexcept -> bool {
    return client_registry_map_.find(connection_handle) != client_registry_map_.end();
  }

  /*!
   * \brief Update the provided service instances set of the client to indicate the client provides a service instance.
   * \param[in] connection_handle Handle to the client connection.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the provided service instance.
   * \context ANY
   * \pre The client must currently be registered.
   * \pre The service instance must not be provided.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Emplace the service instance into the provided set.
   * \endinternal
   */
  void UpdateProvided(ConnectionHandle connection_handle,
                      ProvidedServiceInstanceIdentifier service_instance_identifier) noexcept {
    // This always succeeds, if precondition is not violated.
    assert(client_registry_map_.find(connection_handle) != client_registry_map_.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT_UNORDERED_SET_AT
    Entry& entry{client_registry_map_.at(connection_handle)};
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(entry.provided_service_instances.find(service_instance_identifier) ==
           entry.provided_service_instances.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = entry.provided_service_instances.emplace(service_instance_identifier);
  }

  /*!
   * \brief Update the provided service instances set of the client to indicate the client no longer provides a service
   * instance.
   * \param[in] connection_handle Handle to the client connection.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the provided service instance.
   * \context ANY
   * \pre The client must currently be registered.
   * \pre The service instance must be provided.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Erase the service instance from the provided set.
   * \endinternal
   */
  void UpdateNotProvided(ConnectionHandle connection_handle,
                         ProvidedServiceInstanceIdentifier service_instance_identifier) noexcept {
    // This always succeeds, if precondition is not violated.
    assert(client_registry_map_.find(connection_handle) != client_registry_map_.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT_UNORDERED_SET_AT
    Entry& entry{client_registry_map_.at(connection_handle)};

    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(entry.provided_service_instances.find(service_instance_identifier) !=
           entry.provided_service_instances.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = entry.provided_service_instances.erase(service_instance_identifier);
  }

  /*!
   * \brief Update the required service instances set of the client to indicate the client requires a service instance.
   * \param[in] connection_handle Handle to the client connection.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the required service instance.
   * \context ANY
   * \pre The client must currently be registered.
   * \pre The service instance must not be required.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Emplace the service instance into the required set.
   * \endinternal
   */
  void UpdateRequired(ConnectionHandle connection_handle,
                      RequiredServiceInstanceIdentifier service_instance_identifier) noexcept {
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(client_registry_map_.find(connection_handle) != client_registry_map_.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT_UNORDERED_SET_AT
    Entry& entry{client_registry_map_.at(connection_handle)};
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(entry.required_service_instances.find(service_instance_identifier) ==
           entry.required_service_instances.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = entry.required_service_instances.emplace(service_instance_identifier);
  }

  /*!
   * \brief Update the required service instances set of the client to indicate the client no longer requires a service
   * instance.
   * \param[in] connection_handle Handle to the client connection.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the required service instance.
   * \context ANY
   * \pre The client must currently be registered.
   * \pre The service instance must not be required.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Erase the service instance from the required set.
   * \endinternal
   */
  void UpdateNotRequired(ConnectionHandle connection_handle,
                         RequiredServiceInstanceIdentifier service_instance_identifier) noexcept {
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(client_registry_map_.find(connection_handle) != client_registry_map_.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT_UNORDERED_SET_AT
    Entry& entry{client_registry_map_.at(connection_handle)};
    // This always succeeds, if precondition is not violated.
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    assert(entry.required_service_instances.find(service_instance_identifier) !=
           entry.required_service_instances.end());
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = entry.required_service_instances.erase(service_instance_identifier);
  }

 private:
  /*!
   * \brief Map for the client registry.
   */
  using ClientRegistryMap = std::unordered_map<ConnectionHandle, Entry>;

  /*! Map for the registered clients. */
  ClientRegistryMap client_registry_map_{};
};

}  // namespace sd_server
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_CLIENT_REGISTRY_H_
