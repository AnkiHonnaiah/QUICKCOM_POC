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
 *  \brief A registry for provided service instances managed by the SdClient.
 *  \unit IpcServiceDiscovery::Centralized::SdClient
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRY_H_

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
namespace sd_client {

/*!
 * \brief Entry value for the ProvidedServiceRegistry.
 */
class ProvidedServiceRegistryEntryValue {
 public:
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  ProvidedState provided_state{ProvidedState::kNotProvided};
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  UnicastAddress unicast_address{0, 0};

  constexpr auto operator==(ProvidedServiceRegistryEntryValue const& other) const noexcept -> bool {
    return std::tie(provided_state, unicast_address) == std::tie(other.provided_state, other.unicast_address);
  }

  constexpr auto operator!=(ProvidedServiceRegistryEntryValue const& other) const noexcept -> bool {
    return !operator==(other);
  }
};

/*!
 * \brief ServiceRegistry for provided service instances managed by the SdClient.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientProvidedServiceRegistry
 */
class ProvidedServiceRegistry final
    : public ServiceRegistryBase<ProvidedServiceInstanceIdentifier, ProvidedServiceRegistryEntryValue> {
 public:
  /*!
   * \brief Error code type for the IpcServiceDiscovery.
   */
  using Errc = IpcServiceDiscoveryErrc;

  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Entry type for the ProvidedServiceRegistry.
   */
  using EntryValue = ProvidedServiceRegistryEntryValue;

  /*!
   * \brief Default constructor to create a ProvidedServiceRegistry.
   * \details Needed to be C++17 compliant because the definition of aggreate type changed. For C++14 it is not needed.
   */
  ProvidedServiceRegistry()
      : ServiceRegistryBase<ProvidedServiceInstanceIdentifier, ProvidedServiceRegistryEntryValue>() {}

  /*!
   * \brief Set an entry to provided (ProvidedState::kProvided). Entry will be created if not present.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \param[in] enable_protocol_validation Enable the protocol validation checks (enabled by default). If not enabled,
   *                                       the entry will be overwritten if different without any protocol checks.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier is already
   *                                                                   provided (only if protocol validation is enabled.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier is
   *                                                                   already provided but with a different
   *                                                                   unicast address (only if protocol validation
   *                                                                   is enabled).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Set entry to provided if exits.
   *  - Create entry if not present.
   * \endinternal
   */
  auto SetToProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                     UnicastAddress const& unicast_address, bool enable_protocol_validation = true) noexcept
      -> ::ara::core::Result<void> {
    return SetToProvidedIfEntryExits(service_instance_identifier, unicast_address, enable_protocol_validation)
        .OrElse([this, &service_instance_identifier, &unicast_address](::ara::core::ErrorCode error_code) {
          ::ara::core::Result<void> result{error_code};
          if (error_code == CentralizedErrc::kNoSuchEntry) {
            Insert(service_instance_identifier, EntryValue{ProvidedState::kProvided, unicast_address});
            result.EmplaceValue();
          }
          return result;
        });
  }

  /*!
   * \brief Set an entry to not provided (ProvidedState::kNotProvided).
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to not provided.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \param[in] enable_protocol_validation Enable the protocol validation checks (enabled by default). If not enabled,
   *                                       the entry will be overwritten if different without any protocol checks.
   * \return Result containing an empty value or an error.
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
   *
   * \internal
   *  - Set entry to not provided if exits.
   *  - Map error from no such entry to never provided.
   * \endinternal
   */
  auto SetToNotProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                        UnicastAddress const& unicast_address, bool enable_protocol_validation = true) noexcept
      -> ::ara::core::Result<void> {
    return Find(service_instance_identifier)
        .AndThen([&unicast_address, &enable_protocol_validation](EntryValue& entry_value) {
          return SetToNotProvided(entry_value, unicast_address, enable_protocol_validation);
        })
        .MapError([](::ara::core::ErrorCode error_code) {
          return (error_code == CentralizedErrc::kNoSuchEntry) ? Errc::kNeverProvided : error_code;
        });
  }

  /*!
   * \brief Check if service instance has ever been provided.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNeverProvided  The given ServiceInstanceIdentifier has never been provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto WasEverProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<void> {
    return Contains(service_instance_identifier) ? ::ara::core::Result<void>{}
                                                 : ::ara::core::Result<void>{Errc::kNeverProvided};
  }

  /*!
   * \brief Check if service instance has never been provided.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided  The given ServiceInstanceIdentifier has ever been provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto WasNeverProvided(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<void> {
    return WasEverProvided(service_instance_identifier).HasValue() ? ::ara::core::Result<void>{Errc::kAlreadyProvided}
                                                                   : ::ara::core::Result<void>{};
  }

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the provided service registry
   *        and get matching entries which are in provided state.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \return Result containing either a map of matching ServiceInstanceIdentifier entries with ProvidedState kProvided
   *         and references to their respective values or an error.
   * \error IpcServiceDiscoveryErrc::kNotProvided    A matching ServiceInstanceIdentifier is currently not provided.
   * \error IpcServiceDiscoveryErrc::kNeverProvided  A matching ServiceInstanceIdentifier has never been provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Get service instance matches.
   *  - No match means, a matching service instance has never been provided.
   *  - Removed matches with ProvidedState not provided.
   *  - If the map is empty, no matching service instance is currently provided. So set error to kNotProvided.
   *  - Otherwise return map.
   * \endinternal
   */
  auto GetProvidedMatches(RequiredServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<ConstMatchAndGetReturn> {
    ::ara::core::Result<ConstMatchAndGetReturn> result{Errc::kNotProvided};
    ConstMatchAndGetReturn provided_service_matches{MatchAndGet(service_instance_identifier)};

    if (provided_service_matches.empty()) {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_AMSR_CORE_RESULT_FULFILLS_FUNCTION_CONTRACT
      result.EmplaceError(Errc::kNeverProvided);
    }

    auto iter = provided_service_matches.begin();
    while (iter != provided_service_matches.end()) {
      ProvidedState const required_state{iter->second.get().provided_state};
      if (required_state == ProvidedState::kNotProvided) {
        // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        iter = provided_service_matches.erase(iter);
      } else {
        ++iter;
      }
    }

    if (!provided_service_matches.empty()) {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_AMSR_CORE_RESULT_FULFILLS_FUNCTION_CONTRACT
      result.EmplaceValue(provided_service_matches);
    }

    return result;
  }  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT

 private:
  /*!
   * \brief Set an EntryValue for the given ServiceInstanceIdentifier to provided with or without validation if
   * existing. \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to provided.
   * \param[in] enable_protocol_validation Enable the protocol validation checks. If not enabled, the entry will be
   *                                       overwritten if different without any protocol checks.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided            The given EntryValue is already provided
   *                                                             (only if protocol validation is enabled).
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint  The given EntryValue is already provided but
   *                                                             with a different unicast address (only if protocol
   *                                                             validation is enabled).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetToProvidedIfEntryExits(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                 UnicastAddress const& unicast_address, bool enable_protocol_validation) noexcept
      -> ::ara::core::Result<void> {
    return Find(service_instance_identifier)
        .AndThen([&unicast_address, &enable_protocol_validation](EntryValue& entry_value) {
          return SetToProvided(entry_value, unicast_address, enable_protocol_validation);
        });
  };

  /*!
   * \brief Check if given EntryValue is set to not provided.
   * \param[in] entry_value EntryValue to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided The given EntryValue is already provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsNotProvided(EntryValue const& entry_value) noexcept -> ::ara::core::Result<void> {
    return (entry_value.provided_state == ProvidedState::kNotProvided)
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>{Errc::kAlreadyProvided};
  }

  /*!
   * \brief Check if given EntryValue is set to provided.
   * \param[in] entry_value EntryValue to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotProvided The given EntryValue is currently not provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsProvided(EntryValue const& entry_value) noexcept -> ::ara::core::Result<void> {
    return (entry_value.provided_state == ProvidedState::kProvided) ? ::ara::core::Result<void>{}
                                                                    : ::ara::core::Result<void>{Errc::kNotProvided};
  }

  /*!
   * \brief Set an EntryValue to provided with or without validation.
   * \param[in] entry_value EntryValue to set to provided.
   * \param[in] unicast_address UnicastAddress to set to the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided           The given EntryValue is already provided
   *                                                            (only if protocol validation is enabled).
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint The given EntryValue is already provided but
   *                                                            with a different UnicastAddress (only if protocol
   *                                                            validation is enabled).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToProvided(EntryValue& entry_value, UnicastAddress const& unicast_address,
                            bool enable_protocol_validation) -> ::ara::core::Result<void> {
    return enable_protocol_validation ? SetToProvidedWithValidation(entry_value, unicast_address)
                                      : SetToWithoutValidation(entry_value, unicast_address, ProvidedState::kProvided);
  }

  /*!
   * \brief Set an EntryValue to not provided with or without validation.
   * \param[in] entry_value EntryValue to set to not provided.
   * \param[in] unicast_address UnicastAddress of the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotProvided               The given EntryValue is currently not provided.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint The given EntryValue is provided but
   *                                                            with a different unicast address.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToNotProvided(EntryValue& entry_value, UnicastAddress const& unicast_address,
                               bool enable_protocol_validation) noexcept -> ::ara::core::Result<void> {
    return enable_protocol_validation
               ? SetToNotProvidedWithValidation(entry_value, unicast_address)
               : SetToWithoutValidation(entry_value, unicast_address, ProvidedState::kNotProvided);
  };

  /*!
   * \brief Set an EntryValue to the given ProvidedState.
   * \param[in] entry_value EntryValue to set to not provided.
   * \param[in] unicast_address UnicastAddress of the EntryValue.
   * \param[in] provided_state ProvidedState of the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided   The given EntryValue is already provided
   *                                                    (only if for ProvidedState::kProvided).
   * \error IpcServiceDiscoveryErrc::kNotProvided       The given EntryValue is currently not provided
   *                                                    (only if for ProvidedState::kNotProvided).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Check if new EntryValue is different form the existing one.
   *    - Overwrite old EntryValue.
   *  - Otherwise set error depending on ProvidedState.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_IPCSERVICEDISCOVERY_A8.4.4_outputParameterInsteadOfReturn
  static auto SetToWithoutValidation(EntryValue& entry_value, UnicastAddress const& unicast_address,
                                     ProvidedState provided_state) -> ::ara::core::Result<void> {
    ::ara::core::Result<void> result{Errc::kDefaultError};
    EntryValue const new_entry_value{provided_state, unicast_address};

    if (entry_value != new_entry_value) {
      entry_value = new_entry_value;
      result.EmplaceValue();
    } else {
      if (provided_state == ProvidedState::kProvided) {
        result.EmplaceError(Errc::kAlreadyProvided);
      }
      if (provided_state == ProvidedState::kNotProvided) {
        result.EmplaceError(Errc::kNotProvided);
      }
    }
    return result;
  }

  /*!
   * \brief Set an EntryValue to not provided with validation.
   * \param[in] entry_value EntryValue to set to not provided.
   * \param[in] unicast_address UnicastAddress of the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotProvided                The given EntryValue is currently not provided.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint  The given EntryValue is provided but
   *                                                             with a different unicast address.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToNotProvidedWithValidation(EntryValue& entry_value, UnicastAddress const& unicast_address) noexcept
      -> ::ara::core::Result<void> {
    return IsProvided(entry_value)
        .AndThen([&entry_value, &unicast_address]() { return IsUnicastAddressEqual(entry_value, unicast_address); })
        .AndThen([&entry_value, &unicast_address]() {  // COV_IpcSD_fluentApiNoErrorReturnPossible
          return SetToWithoutValidation(entry_value, unicast_address, ProvidedState::kNotProvided);
        });
  };

  /*!
   * \brief Set an EntryValue to provided with validation.
   * \param[in] entry_value EntryValue to set to provided.
   * \param[in] unicast_address UnicastAddress to set to the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided            The given EntryValue is already provided.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint  The given EntryValue is already provided
   *                                                             but with a different unicast address.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToProvidedWithValidation(EntryValue& entry_value, UnicastAddress const& unicast_address)
      -> ::ara::core::Result<void> {
    return IsNotProvided(entry_value)
        .MapError([&entry_value, &unicast_address](::ara::core::ErrorCode error_code) {
          return IsUnicastAddressEqual(entry_value, unicast_address).HasValue() ? error_code
                                                                                : Errc::kProvidedDifferentEndpoint;
        })
        .AndThen([&entry_value, &unicast_address]() {  // COV_IpcSD_fluentApiNoErrorReturnPossible
          return SetToWithoutValidation(entry_value, unicast_address, ProvidedState::kProvided);
        });
  }

  /*!
   * \brief Check if given UnicastAddress is equal to UnicastAddress of given entry.
   * \param[in] unicast_address UnicastAddress to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint  The given UnicastAddress is different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsUnicastAddressEqual(EntryValue const& entry_value, UnicastAddress unicast_address) noexcept
      -> ::ara::core::Result<void> {
    return (entry_value.unicast_address == unicast_address)
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>{Errc::kProvidedDifferentEndpoint};
  }
};

}  // namespace sd_client
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_fluentApiNoErrorReturnPossible
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the fluent API AndThen.
//            This is true, but it is verified by review that the error case cannot occur since the potential error case
//            of the called method within is already captured before the AndThen and therefore can no longer occur.
// COV_JUSTIFICATION_END

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_PROVIDED_SERVICE_REGISTRY_H_
