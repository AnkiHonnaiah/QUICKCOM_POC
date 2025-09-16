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
 *  \brief A registry for local and remote provided service instances managed by the SdClient.
 *  \unit IpcServiceDiscovery::Centralized::SdClient
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRIES_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRIES_H_

#include "amsr/ipc_service_discovery/centralized/internal/sd_client/provided_service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {
namespace sd_client {

/*!
 * \brief ServiceRegistry for local and remote provided service instances managed by the SdClient.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientProvidedServiceRegistries
 */
class ProvidedServiceRegistries {
 public:
  using Errc = ProvidedServiceRegistry::Errc;
  using CentralizedErrc = ProvidedServiceRegistry::CentralizedErrc;
  using EntryValue = ProvidedServiceRegistry::EntryValue;
  using ConstGetAndReturnEntry = ProvidedServiceRegistry::ConstGetAndReturnEntry;
  using MatchAndGetReturn = ProvidedServiceRegistry::MatchAndGetReturn;
  using ConstMatchAndGetReturn = ProvidedServiceRegistry::ConstMatchAndGetReturn;
  using MatchAndGetReturnEntry = ProvidedServiceRegistry::MatchAndGetReturnEntry;
  using ConstMatchAndGetReturnEntry = ProvidedServiceRegistry::ConstMatchAndGetReturnEntry;
  using MatchReturn = ProvidedServiceRegistry::MatchReturn;
  using FindResult = ProvidedServiceRegistry::FindResult;
  using ConstFindResult = ProvidedServiceRegistry::ConstFindResult;

  /*!
   * \brief Get the local ProvidedRegistry
   * \return Return a const reference to the local ProvidedRegistry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetLocalProvidedServiceRegistry() const noexcept -> ProvidedServiceRegistry const& {
    return local_provided_service_registry_;
  }

  /*!
   * \brief Get the remote ProvidedRegistry
   * \return Return a const reference to the remote ProvidedRegistry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetRemoteProvidedServiceRegistry() const noexcept -> ProvidedServiceRegistry const& {
    return remote_provided_service_registry_;
  }

  /*!
   * \brief Set an entry in the local ProvidedRegistry to provided (ProvidedState::kProvided).
   *        Entry will be created if not present.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier was already
   *                                                                   provided (only if protocol validation is enabled.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier was
   *                                                                   already provided but with a different
   *                                                                   unicast address (only if protocol validation
   *                                                                   is enabled).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetLocalServiceInstanceToProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                         UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> {
    return WasNeverRemotelyProvided(service_instance_identifier)
        .AndThen([this, &service_instance_identifier, &unicast_address]() {
          return local_provided_service_registry_.SetToProvided(service_instance_identifier, unicast_address);
        });
  }

  /*!
   * \brief Set an entry in the remote ProvidedRegistry to provided (ProvidedState::kProvided).
   *        Entry will be created if not present.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier was already
   *                                                                   provided (only if protocol validation is enabled.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier was
   *                                                                   already provided but with a different
   *                                                                   unicast address (only if protocol validation
   *                                                                   is enabled).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetRemoteServiceInstanceToProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                          UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> {
    return WasNeverLocallyProvided(service_instance_identifier)
        .AndThen([this, &service_instance_identifier, &unicast_address]() {
          return remote_provided_service_registry_.SetToProvided(service_instance_identifier, unicast_address, false);
        });
  }

  /*!
   * \brief Set an entry in the local ProvidedRegistry to not provided (ProvidedState::kNotProvided).
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to not provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier is
   *                                                                   provided but with a different unicast address.
   * \error IpcServiceDiscoveryErrc::kNotProvided                      The given ServiceInstanceIdentifier is
   *                                                                   currently not provided.
   * \error IpcServiceDiscoveryErrc::kNeverProvided                    The given ServiceInstanceIdentifier was
   *                                                                   never provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetLocalServiceInstanceToNotProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                            UnicastAddress const& unicast_address) noexcept
      -> ::ara::core::Result<void> {
    return WasNeverRemotelyProvided(service_instance_identifier)
        .AndThen([this, &service_instance_identifier, &unicast_address]() {
          return local_provided_service_registry_.SetToNotProvided(service_instance_identifier, unicast_address);
        });
  }

  /*!
   * \brief Set an entry in the remote ProvidedRegistry to not provided (ProvidedState::kNotProvided).
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to not provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier is
   *                                                                   provided but with a different unicast address.
   * \error IpcServiceDiscoveryErrc::kNotProvided                      The given ServiceInstanceIdentifier is
   *                                                                   currently not provided.
   * \error IpcServiceDiscoveryErrc::kNeverProvided                    The given ServiceInstanceIdentifier was
   *                                                                   never provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetRemoteServiceInstanceToNotProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                             UnicastAddress const& unicast_address) noexcept
      -> ::ara::core::Result<void> {
    return WasNeverLocallyProvided(service_instance_identifier)
        .AndThen([this, &service_instance_identifier, &unicast_address]() {
          return remote_provided_service_registry_.SetToNotProvided(service_instance_identifier, unicast_address,
                                                                    false);
        });
  }

  /*!
   * \brief Clear the remote ProvidedRegistry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ClearRemoteProvidedServiceRegistry() noexcept { remote_provided_service_registry_.Clear(); }

 private:
  /*!
   * \brief Check if service instance has never been provided locally.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentClient  The given ServiceInstanceIdentifier has
   *                                                           ever been provided locally.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto WasNeverLocallyProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<void> {
    return local_provided_service_registry_.WasNeverProvided(service_instance_identifier).HasValue()
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>::FromError(Errc::kProvidedDifferentClient,
                                                      "The service instance has previously been provided remotely."_sv);
  }

  /*!
   * \brief Check if service instance has never been provided locally.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentClient  The given ServiceInstanceIdentifier has
   *                                                           ever been provided remotely.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto WasNeverRemotelyProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<void> {
    return remote_provided_service_registry_.WasNeverProvided(service_instance_identifier).HasValue()
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>::FromError(Errc::kProvidedDifferentClient,
                                                      "The service instance has previously been provided locally."_sv);
  }

  /*! ProvidedServiceRegistry for local provided service instances. */
  ProvidedServiceRegistry local_provided_service_registry_{};

  /*! ProvidedServiceRegistry for remote provided service instances. */
  ProvidedServiceRegistry remote_provided_service_registry_{};
};

}  // namespace sd_client
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRIES_H_
