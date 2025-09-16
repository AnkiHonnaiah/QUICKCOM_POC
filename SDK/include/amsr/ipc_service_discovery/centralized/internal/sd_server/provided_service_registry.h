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
 *  \brief A registry for provided service instances managed by the SdServer.
 *  \unit IpcServiceDiscovery::Centralized::SdServer
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_PROVIDED_SERVICE_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_PROVIDED_SERVICE_REGISTRY_H_

#include <functional>
#include <utility>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/service_registry/service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/ipc_service_discovery_error_domain.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {
namespace sd_server {

/*!
 * \brief Entry for the ProvidedServiceRegistry.
 */
struct ProvidedServiceRegistryEntry {
  ProvidedState provided_state{ProvidedState::kNotProvided};
  UnicastAddress unicast_address{0, 0};
  ConnectionHandle last_providing_client{};
};

/*!
 * \brief ServiceRegistry for provided service instances managed by the SdServer.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerProvidedServiceRegistry
 */
class ProvidedServiceRegistry final
    : public ServiceRegistryBase<ProvidedServiceInstanceIdentifier, ProvidedServiceRegistryEntry> {
 public:
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Error code type for the IpcServiceDiscovery.
   */
  using Errc = IpcServiceDiscoveryErrc;

  /*!
   * \brief Entry type for the ProvidedServiceRegistry.
   */
  using Entry = ProvidedServiceRegistryEntry;

  /*!
   * \brief Match type for the ProvidedServiceRegistry.
   */
  using Match = std::pair<ProvidedServiceInstanceIdentifier const, std::reference_wrapper<Entry>>;

  /*!
   * \brief Default constructor to create a ProvidedServiceRegistry.
   * \details Needed to be C++17 compliant because the definition of aggreate type changed. For C++14 it is not needed.
   */
  ProvidedServiceRegistry() : ServiceRegistryBase<ProvidedServiceInstanceIdentifier, ProvidedServiceRegistryEntry>() {}

  /*!
   * \brief Update an entry in the ProvidedServiceRegistry to set the state to kProvided.
   * \param[in] connection_handle Handle to the client making the update.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to update.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier was
   *                                                                   already provided.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given EntryValue is already provided
   *                                                                   but with a different unicast address.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *    - Lookup for the ServiceInstanceIdentifier in the registry.
   *    - If entry was found:
   *      - If the current entry is not currently provided:
   *        - Update is valid.
   *      - Else:
   *        - Update is invalid.
   *          - Set error code based on whether the unicast address is different.
   *  - If update is valid:
   *    - Do the update.
   * \endinternal
   */
  auto UpdateProvided(ConnectionHandle connection_handle,
                      ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                      UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> {
    ::ara::core::Result<void> update_result{CentralizedErrc::kDefaultError};
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    auto entry_opt = Find(service_instance_identifier);
    if (entry_opt.HasValue()) {
      Entry const& entry{entry_opt.Value()};
      if (entry.provided_state == ProvidedState::kNotProvided) {
        update_result.EmplaceValue();
      } else {
        // Service already provided, check if Unicast address is different.
        entry.unicast_address == unicast_address ? update_result.EmplaceError(Errc::kAlreadyProvided)
                                                 : update_result.EmplaceError(Errc::kProvidedDifferentEndpoint);
      }
    } else {
      update_result.EmplaceValue();
    }

    if (update_result.HasValue()) {
      Entry entry{ProvidedState::kProvided, unicast_address, connection_handle};
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
      Insert(service_instance_identifier, std::move(entry));
    }
    return update_result;
  }

  /*!
   * \brief Update an entry in the ProvidedServiceRegistry to set the state to kNotProvided.
   * \param[in] connection_handle Handle to the client making the update.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to update.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier was
   *                                                                   already provided by this client with a
   *                                                                   different unicast address.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentClient          The given ServiceInstanceIdentifier was
   *                                                                   already provided by another client.
   * \error IpcServiceDiscoveryErrc::kNotProvided                      The given ServiceInstanceIdentifier is
   *                                                                   currently not provided by any client.
   * \error IpcServiceDiscoveryErrc::kNeverProvided                    The given ServiceInstanceIdentifier was
   *                                                                   never provided by any client.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *    - Lookup for the ServiceInstanceIdentifier in the registry.
   *    - If entry was found:
   *      - If the current entry is provided:
   *        - If last providing client is this client and UnicastAddress is equal:
   *          - Set the entry's state to kNotProvided.
   *          - Update is valid.
   *        - Else:
   *          - Update is invalid.
   *      - Else:
   *        - Update is invalid.
   * \endinternal
   */
  auto UpdateNotProvided(ConnectionHandle connection_handle,
                         ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                         UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> {
    ::ara::core::Result<void> update_result{CentralizedErrc::kDefaultError};
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    auto entry_opt = Find(service_instance_identifier);
    if (entry_opt.HasValue()) {
      Entry& entry{entry_opt.Value()};
      if (entry.provided_state == ProvidedState::kProvided) {
        if (connection_handle == entry.last_providing_client) {
          if (unicast_address == entry.unicast_address) {
            // VCA_IPCSERVICEDISCOVERY_SLC_12_ACCESS_INBOUND
            entry.provided_state = ProvidedState::kNotProvided;
            update_result.EmplaceValue();
          } else {
            update_result.EmplaceError(Errc::kProvidedDifferentEndpoint);
          }
        } else {
          update_result.EmplaceError(Errc::kProvidedDifferentClient);
        }
      } else {
        update_result.EmplaceError(Errc::kNotProvided);
      }
    } else {
      update_result.EmplaceError(Errc::kNeverProvided);
    }
    return update_result;
  }
};

}  // namespace sd_server
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_PROVIDED_SERVICE_REGISTRY_H_
