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
 *  \brief A registry for required service instances managed by the SdServer.
 *  \unit IpcServiceDiscovery::Centralized::SdServer
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_REQUIRED_SERVICE_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_REQUIRED_SERVICE_REGISTRY_H_

#include <functional>
#include <unordered_set>
#include <utility>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/service_registry/service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/ipc_service_discovery_error_domain.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {
namespace sd_server {

/*!
 * \brief Entry for the RequiredServiceRegistry.
 */
// VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
struct RequiredServiceRegistryEntry {
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  std::unordered_set<ConnectionHandle> requiring_clients{};
};

/*!
 * \brief ServiceRegistry for required service instances managed by the SdServer.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerRequiredServiceRegistry
 */
class RequiredServiceRegistry final
    : public ServiceRegistryBase<RequiredServiceInstanceIdentifier, RequiredServiceRegistryEntry> {
 public:
  /*!
   * \brief Default constructor to create a RequiredServiceRegistry.
   * \details Needed to be C++17 compliant because the definition of aggreate type changed. For C++14 it is not needed.
   */
  RequiredServiceRegistry() : ServiceRegistryBase<RequiredServiceInstanceIdentifier, RequiredServiceRegistryEntry>() {}

  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Error code type for the IpcServiceDiscovery.
   */
  using Errc = IpcServiceDiscoveryErrc;

  /*!
   * \brief Entry type for the RequiredServiceRegistry.
   */
  using Entry = RequiredServiceRegistryEntry;

  /*!
   * \brief Match type for the RequiredServiceRegistry.
   */
  using Match = std::pair<RequiredServiceInstanceIdentifier const, std::reference_wrapper<Entry>>;

  /*!
   * \brief Update an entry in the RequiredServiceRegistry to set the state to kRequired.
   * \param[in] connection_handle Handle to the client making the update.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to update.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired The given ServiceInstanceIdentifier was
   *                                                  already required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Lookup for the ServiceInstanceIdentifier in the registry.
   *  - If entry was found:
   *    - If the service is not already required by this client:
   *      - Emplace the client's handle in the entry's set.
   *      - Update is valid.
   *    - Else:
   *      - Update is invalid.
   *  - Else:
   *    - Create a new entry with a set containing the client's handle.
   *    - Update is valid.
   * \endinternal
   */
  auto UpdateRequired(ConnectionHandle connection_handle,
                      RequiredServiceInstanceIdentifier const& service_instance_identifier) noexcept
      -> ::ara::core::Result<void> {
    ::ara::core::Result<void> update_result{CentralizedErrc::kDefaultError};
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    auto entry_result = Find(service_instance_identifier);
    if (entry_result.HasValue()) {
      Entry& entry{entry_result.Value()};
      // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      bool const was_emplaced{entry.requiring_clients.emplace(connection_handle).second};
      if (was_emplaced) {
        update_result.EmplaceValue();
      } else {
        update_result.EmplaceError(Errc::kAlreadyRequired);
      }
    } else {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      Entry entry{{connection_handle}};
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      Insert(service_instance_identifier, std::move(entry));
      update_result.EmplaceValue();
    }
    return update_result;
  }

  /*!
   * \brief Update an entry in the RequiredServiceRegistry to set the state to kNotRequired.
   * \param[in] connection_handle Handle to the client making the update.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to update.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired The given ServiceInstanceIdentifier is
   *                                              not required by this client.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Lookup for the ServiceInstanceIdentifier in the registry.
   *  - If entry was found:
   *    - If the service is currently required by this client:
   *      - Erase the client's handle from the entry's set.
   *      - Update is valid.
   *    - Else:
   *      - Update is invalid.
   *  - Else:
   *    - Update is invalid.
   * \endinternal
   */
  auto UpdateNotRequired(ConnectionHandle connection_handle,
                         RequiredServiceInstanceIdentifier const& service_instance_identifier) noexcept
      -> ::ara::core::Result<void> {
    ::ara::core::Result<void> update_result{CentralizedErrc::kDefaultError};
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    auto entry_result = Find(service_instance_identifier);
    if (entry_result.HasValue()) {
      Entry& entry{entry_result.Value()};
      if (entry.requiring_clients.find(connection_handle) != entry.requiring_clients.end()) {
        // This always succeeds, because it is ensured the service is currently listened to by this client.
        // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        std::ignore = entry.requiring_clients.erase(connection_handle);
        update_result.EmplaceValue();
      } else {
        update_result.EmplaceError(Errc::kNotRequired);
      }
    } else {
      update_result.EmplaceError(Errc::kNotRequired);
    }
    return update_result;
  }
};

}  // namespace sd_server
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_REQUIRED_SERVICE_REGISTRY_H_
