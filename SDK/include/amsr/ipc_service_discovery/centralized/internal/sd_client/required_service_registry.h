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
 *  \brief A registry for required service instances managed by the SdClient.
 *  \unit IpcServiceDiscovery::Centralized::SdClient
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_REQUIRED_SERVICE_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_REQUIRED_SERVICE_REGISTRY_H_

#include <algorithm>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_client/provided_service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/ipc_service_discovery_error_domain.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {
namespace sd_client {

/*!
 * \brief Entry value for the RequiredServiceRegistry.
 */
// VCA_IPCSERVICEDISCOVERY_SLC_12_ACCESS_INBOUND
struct RequiredServiceRegistryEntryValue {
  RequiredState required_state{RequiredState::kNotRequired};
  ::ara::core::Optional<ListenServiceCallback> listen_service_callback_opt;
};

/*!
 * \brief ServiceRegistry for required service instances managed by the SdClient.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientRequiredServiceRegistry
 */
class RequiredServiceRegistry final
    : public ServiceRegistryBase<RequiredServiceInstanceIdentifier, RequiredServiceRegistryEntryValue> {
 public:
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;
  using Errc = IpcServiceDiscoveryErrc;
  using EntryValue = RequiredServiceRegistryEntryValue;

  /*!
   * \brief Default constructor to create a RequiredServiceRegistry.
   * \details Needed to be C++17 compliant because the definition of aggreate type changed. For C++14 it is not needed.
   */
  RequiredServiceRegistry()
      : ServiceRegistryBase<RequiredServiceInstanceIdentifier, RequiredServiceRegistryEntryValue>() {}

  /*!
   * \brief Set an entry to required (RequiredState::kRequired). Entry will be created if not present.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to required.
   * \param[in] unicast_address UnicastAddress of the service instance.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired  The given ServiceInstanceIdentifier is already required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   * - Set entry to provided if exits.
   * - Create entry if not present.
   * \endinternal
   */
  auto SetToRequired(RequiredServiceInstanceIdentifier const& service_instance_identifier,
                     ::ara::core::Optional<ListenServiceCallback>&& listen_service_callback_opt) noexcept
      -> ::ara::core::Result<void> {
    return Find(service_instance_identifier)
        .AndThen([&listen_service_callback_opt](EntryValue& entry_value) {
          return SetToRequired(entry_value, std::move(listen_service_callback_opt));
        })
        .OrElse([this, &service_instance_identifier, &listen_service_callback_opt](::ara::core::ErrorCode error_code) {
          ::ara::core::Result<void> result{IpcServiceDiscoveryCentralizedErrc::kDefaultError};
          if (error_code == CentralizedErrc::kNoSuchEntry) {
            Insert(service_instance_identifier,
                   EntryValue{RequiredState::kRequired, std::move(listen_service_callback_opt)});
            result.EmplaceValue();
          } else {
            result.EmplaceError(error_code);
          }
          return result;
        });
  }

  /*!
   * \brief Set an entry to not required (RequiredState::kNotRequired).
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to set to not required.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired     The given ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverProvided   The given ServiceInstanceIdentifier was never provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   * - Set entry to not required if exits.
   * - Map error from no such entry to never required.
   * \endinternal
   */
  auto SetToNotRequired(RequiredServiceInstanceIdentifier const& service_instance_identifier) noexcept
      -> ::ara::core::Result<void> {
    return Find(service_instance_identifier)
        .AndThen([](EntryValue& entry_value) { return SetToNotRequired(entry_value); })
        .MapError([](::ara::core::ErrorCode error_code) {
          return (error_code == CentralizedErrc::kNoSuchEntry) ? Errc::kNeverRequired : error_code;
        });
  }

  /*!
   * \brief Check if service instance is required.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsRequired(RequiredServiceInstanceIdentifier const& service_instance_identifier) const noexcept
      -> ::ara::core::Result<void> {
    return Find(service_instance_identifier)
        .MapError([](::ara::core::ErrorCode) { return MakeErrorCode(Errc::kNeverRequired); })
        .AndThen([](EntryValue const& entry_value) { return IsRequired(entry_value); });
  }

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the required service registry
   *        and get matching entries which are in required state.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \return Result containing either a map of matching ServiceInstanceIdentifier entries with RequiredState kRequired
   *         and references to their respective values or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Get service instance matches.
   *  - No match means, a matching service instance has never been required.
   *  - Removed matches with RequiredState not required.
   *  - If the map is empty, no matching service instance is currently required. So set error to kNotRequired.
   *  - Otherwise return map.
   * \endinternal
   */
  auto GetRequiredMatches(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) noexcept
      -> ::ara::core::Result<MatchAndGetReturn> {
    ::ara::core::Result<MatchAndGetReturn> result{Errc::kNotRequired};
    MatchAndGetReturn required_service_matches{MatchAndGet(provided_service_instance_identifier)};

    if (required_service_matches.empty()) {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_AMSR_CORE_RESULT_FULFILLS_FUNCTION_CONTRACT
      result.EmplaceError(Errc::kNeverRequired);
    }

    auto iter = required_service_matches.begin();
    while (iter != required_service_matches.end()) {
      RequiredState const required_state{iter->second.get().required_state};
      if (required_state == RequiredState::kNotRequired) {
        // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        iter = required_service_matches.erase(iter);
      } else {
        ++iter;
      }
    }

    if (!required_service_matches.empty()) {
      // VCA_IPCSERVICEDISCOVERY_SPC_15_AMSR_CORE_RESULT_FULFILLS_FUNCTION_CONTRACT
      result.EmplaceValue(required_service_matches);
    }

    return result;
  }  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the required service registry
   *        and and check if at least one matching entry is in required state.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \return Result containing an empty value if at least one entry with RequiredState kRequired was matched or an
   *         error.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Get service instance matches.
   *  - No match means, a matching service instance has never been required.
   *  - Check for matches with RequiredState kRequired.
   *    - If one was found emplace result value and break.
   * \endinternal
   */
  auto CheckRequiredMatches(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier)
      const noexcept -> ::ara::core::Result<void> {
    ::ara::core::Result<void> result{Errc::kNotRequired};
    ConstMatchAndGetReturn required_service_matches{MatchAndGet(provided_service_instance_identifier)};

    if (required_service_matches.empty()) {
      result.EmplaceError(Errc::kNeverRequired);
    }

    for (auto const& match : required_service_matches) {
      RequiredState const required_state{match.second.get().required_state};
      if (required_state == RequiredState::kRequired) {
        result.EmplaceValue();
        break;
      }
    }
    return result;
  }  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT

 private:
  /*!
   * \brief Set an EntryValue to required.
   * \param[in] entry_value EntryValue to set to required.
   * \param[in] listen_service_callback ListenServiceCallback to set to the EntryValue.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired  The given EntryValue is already required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToRequired(EntryValue& entry_value,
                            ::ara::core::Optional<ListenServiceCallback>&& listen_service_callback_opt)
      -> ::ara::core::Result<void> {
    return IsNotRequired(entry_value)
        .AndThen([&entry_value, &listen_service_callback_opt]() {  // COV_IpcSD_fluentApiNoErrorReturn
          entry_value.required_state = RequiredState::kRequired;
          entry_value.listen_service_callback_opt = std::move(listen_service_callback_opt);
          return ::ara::core::Result<void>{};
        });
  }

  /*!
   * \brief Set an EntryValue to not required.
   * \param[in] entry_value EntryValue to set to not required.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired  The given EntryValue is currently not required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SetToNotRequired(EntryValue& entry_value) -> ::ara::core::Result<void> {
    return IsRequired(entry_value).AndThen([&entry_value]() {  // COV_IpcSD_fluentApiNoErrorReturn
      entry_value.required_state = RequiredState::kNotRequired;
      return ::ara::core::Result<void>{};
    });
  }

  /*!
   * \brief Check if given EntryValue is set to not required.
   * \param[in] entry_value EntryValue to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired The given EntryValue is already required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsNotRequired(EntryValue const& entry_value) noexcept -> ::ara::core::Result<void> {
    return (entry_value.required_state == RequiredState::kNotRequired)
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>{Errc::kAlreadyRequired};
  }

  /*!
   * \brief Check if given EntryValue is set to required.
   * \param[in] entry_value EntryValue to be checked.
   * \return Result containing an empty value or an error.
   * \error IpcServiceDiscoveryErrc::kNotRequired The given EntryValue is currently not required.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsRequired(EntryValue const& entry_value) noexcept -> ::ara::core::Result<void> {
    return (entry_value.required_state == RequiredState::kRequired) ? ::ara::core::Result<void>{}
                                                                    : ::ara::core::Result<void>{Errc::kNotRequired};
  }
};

}  // namespace sd_client
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_fluentApiNoErrorReturn
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the fluent API AndThen.
//            This is true, since this lambda can only return a result with a value but no error. Therefore,
//            the error case is untestable. It is verified by review that the error case cannot occur.
// COV_JUSTIFICATION_END

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_REQUIRED_SERVICE_REGISTRY_H_
