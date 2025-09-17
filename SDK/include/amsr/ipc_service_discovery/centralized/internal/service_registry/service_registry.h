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
 *  \brief Implementation of a service registry
 *  \unit IpcServiceDiscovery::Centralized::ServiceRegistry
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SERVICE_REGISTRY_SERVICE_REGISTRY_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SERVICE_REGISTRY_SERVICE_REGISTRY_H_

#include <functional>
#include <type_traits>
#include <unordered_map>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/constants.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/abort_wrapper/abort.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Base class for a registry container to be used by a service discovery to store
 *        and retrieve information related to a service offer or request.
 * \details The ServiceRegistry using an unsorted associative container std::unordered_map that contains key-value
 *          pairs with unique keys. Keys must be of type ServiceInstanceIdentifier and identified and found by
 *          a given service instance identifier.
 *
 * \tparam ServiceInstanceIdentifierEntryKeyType Entry key type must be derived from ServiceInstanceIdentifier.
 * \tparam EntryValueType Type used to store information about an entry to the registry.
 *                        Must be no-throw move constructable and no-throw move assignable.
 * \trace DSGN-IpcServiceDiscovery-Centralized-ServiceRegistry
 * \vprivate Component internal.
 */
// VECTOR NC Metric-OO.WMC.One: MD_IPCSERVICEDISCOVERY_Metric-OO.WMC.One_weightedMethodsPerClassTooHigh
template <class ServiceInstanceIdentifierEntryKeyType, class EntryValueType>
class ServiceRegistryBase {
  static_assert(std::is_base_of<ServiceInstanceIdentifier, ServiceInstanceIdentifierEntryKeyType>::value,
                "ServiceInstanceIdentifierEntryKeyType must be derived from ServiceInstanceIdentifier.");
  static_assert(std::is_nothrow_move_constructible<EntryValueType>::value,
                "EntryValueType has to be no-throw move constructable.");
  static_assert(std::is_nothrow_move_assignable<EntryValueType>::value,
                "EntryValueType has to be no-throw move assignable.");

 public:
  /*!
   * \brief Type of service registry map.
   */
  using ServiceRegistryBaseMap = std::unordered_map<ServiceInstanceIdentifierEntryKeyType, EntryValueType>;

  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;
  using Entry = std::pair<ServiceInstanceIdentifierEntryKeyType const, EntryValueType>;

  using ConstGetAndReturnEntry = std::reference_wrapper<EntryValueType const>;
  using MatchAndGetReturnEntry =
      std::pair<ServiceInstanceIdentifierEntryKeyType const, std::reference_wrapper<EntryValueType>>;
  using ConstMatchAndGetReturnEntry =
      std::pair<ServiceInstanceIdentifierEntryKeyType const, std::reference_wrapper<EntryValueType const>>;

  using MatchAndGetReturn =
      std::unordered_map<ServiceInstanceIdentifierEntryKeyType, std::reference_wrapper<EntryValueType>>;
  using ConstMatchAndGetReturn =
      std::unordered_map<ServiceInstanceIdentifierEntryKeyType, std::reference_wrapper<EntryValueType const>>;
  using MatchReturn = ::ara::core::Vector<ServiceInstanceIdentifierEntryKeyType>;
  using FindResult = ::ara::core::Result<EntryValueType&>;
  using ConstFindResult = ::ara::core::Result<EntryValueType const&>;

  using ConstIterator = typename ServiceRegistryBaseMap::const_iterator;
  using Iterator = typename ServiceRegistryBaseMap::iterator;

  /*!
   * \brief Get a const iterator to the first element of the ServiceRegistry.
   * \return Const iterator to the first element.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() const noexcept -> ConstIterator { return service_registry_map_.begin(); }

  /*!
   * \brief Get a non-const iterator to the first element of the ServiceRegistry.
   * \return Non-const iterator to the first element.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() noexcept -> Iterator { return service_registry_map_.begin(); }

  /*!
   * \brief Get a const iterator to end of the ServiceRegistry.
   * \return Const iterator to the end.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() const noexcept -> ConstIterator { return service_registry_map_.end(); }

  /*!
   * \brief Get a non-const iterator to end of the ServiceRegistry.
   * \return Non-const iterator to the end.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() noexcept -> Iterator { return service_registry_map_.end(); }

  /*!
   * \brief Get the value at an entry with an equivalent ServiceInstanceIdentifier.
   * \details If there is an entry with an equivalent ServiceInstanceIdentifier,
   *          a const reference is returned. If there is not, this method aborts.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to retrieve.
   * \return A const reference to the value at the found entry
   * \context ANY
   * \pre An entry with an equivalent ServiceInstanceIdentifier is contained in the registry.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto At(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) const noexcept
      -> EntryValueType const& {
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    auto const iter = service_registry_map_.find(service_instance_identifier);
    if (iter == service_registry_map_.end()) {
      Abort(
          kNoSuchEntryAbortMessage, logger_,
          [&service_instance_identifier](auto& log_stream) {
            log_stream << "Service registry has no such entry " << service_instance_identifier;
          },
          LOCATION);  // COV_IpcSD_utility
    }
    // VCA_IPCSERVICEDISCOVERY_LNG_02_VALID_RETURNED_REFERENCE
    return iter->second;
  }

  /*!
   * \brief Get the value at an entry with an equivalent ServiceInstanceIdentifier.
   * \details If there is an entry with an equivalent ServiceInstanceIdentifier,
   *          a reference is returned. If there is not, this method aborts.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to retrieve.
   * \return A reference to the value at the found entry
   * \context ANY
   * \pre An entry with an equivalent ServiceInstanceIdentifier is contained in the registry.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto At(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) noexcept -> EntryValueType& {
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    auto const iter = service_registry_map_.find(service_instance_identifier);
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    if (iter == service_registry_map_.end()) {
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      Abort(
          kNoSuchEntryAbortMessage, logger_,
          [&service_instance_identifier](auto& log_stream) {
            log_stream << "Service registry has no such entry " << service_instance_identifier;
          },
          LOCATION);  // COV_IpcSD_utility
    }
    // VCA_IPCSERVICEDISCOVERY_LNG_02_VALID_RETURNED_REFERENCE
    return iter->second;
    // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*!
   * \brief Find an entry with an equivalent ServiceInstanceIdentifier.
   * \details If an entry is found, a const reference to the entry's value is returned in an ::ara::core::Result.
   *          If no entry is found, the error IpcServiceDiscoveryCentralizedErrc::kNoSuchEntry is returned.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the required entry.
   * \return A result containing either a const reference to the EntryValue on success
   *         or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchEntry The requested entry does not exist.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto Find(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) const noexcept
      -> ConstFindResult {
    ConstFindResult result{CentralizedErrc::kNoSuchEntry};
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    auto const iter = service_registry_map_.find(service_instance_identifier);
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    if (iter != service_registry_map_.end()) {
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_REFERENCE_ARGUMENT_IN_RESULT
      result.EmplaceValue(iter->second);
    }
    return result;
    // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*!
   * \brief Find an entry with an equivalent ServiceInstanceIdentifier.
   * \details If an entry is found, a reference to the entry's value is returned in an ::ara::core::Result.
   *          If no entry is found, the error IpcServiceDiscoveryCentralizedErrc::kNoSuchEntry is returned.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the required entry.
   * \return A result containing either a reference to the EntryValue on success
   *         or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchEntry The requested entry does not exist.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto Find(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) noexcept -> FindResult {
    FindResult result{CentralizedErrc::kNoSuchEntry};
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    auto const iter = service_registry_map_.find(service_instance_identifier);
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    if (iter != service_registry_map_.end()) {
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_REFERENCE_ARGUMENT_IN_RESULT
      result.EmplaceValue(iter->second);
    }
    return result;
    // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*!
   * \brief Check if entry with an equivalent ServiceInstanceIdentifier exists.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the required entry.
   * \return true if entry exists, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto Contains(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) const noexcept -> bool {
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    return Find(service_instance_identifier).HasValue();
  }

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the service registry
   *        and get matching keys.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \return Vector of matching ServiceInstanceIdentifier entries.
   * \context ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // clang-format off
  template<class ServiceInstanceIdentifierType>
  auto Match(ServiceInstanceIdentifierType const& service_instance_identifier) const noexcept -> MatchReturn /* COV_IpcSD_unused */ {
    // clang-format on
    static_assert(std::is_base_of<ServiceInstanceIdentifier, ServiceInstanceIdentifierType>::value,
                  "ServiceInstanceIdentifierEntryKeyType must be derived from ServiceInstanceIdentifier.");

    MatchReturn matches;
    for (auto iter = service_registry_map_.cbegin(); iter != service_registry_map_.cend(); iter++) {
      ServiceInstanceIdentifierEntryKeyType const& entry_identifier{iter->first};
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
      // VCA_IPCSERVICEDISCOVERY_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPILATION_UNIT
      if (entry_identifier.IsMatching(service_instance_identifier)) {
        // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        matches.push_back(entry_identifier);
      }
    }
    return matches;
  }

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the service registry
   *        and get matching entries (const version).
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \tparam ServiceInstanceIdentifierType Type of the given ServiceInstanceIdentifier.
   *                                       Must be derived from ServiceInstanceIdentifier.
   * \return Map of matching ServiceInstanceIdentifier entries with references to their respective values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  template <class ServiceInstanceIdentifierType>
  auto MatchAndGet(ServiceInstanceIdentifierType const& service_instance_identifier) const noexcept
      -> ConstMatchAndGetReturn {
    static_assert(std::is_base_of<ServiceInstanceIdentifier, ServiceInstanceIdentifierType>::value,
                  "ServiceInstanceIdentifierEntryKeyType must be derived from ServiceInstanceIdentifier.");

    ConstMatchAndGetReturn matches;
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    for (auto iter = service_registry_map_.cbegin(); iter != service_registry_map_.cend(); iter++) {
      ServiceInstanceIdentifierEntryKeyType const& entry_identifier{iter->first};
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      if (entry_identifier.IsMatching(service_instance_identifier)) {
        // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
        std::ignore = matches.insert({entry_identifier, At(entry_identifier)});
        // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      }
    }
    return matches;
  }

  /*!
   * \brief Match a ServiceInstanceIdentifier against all entries in the service registry
   *        and get matching entries (non const version).
   * \tparam ServiceInstanceIdentifierType Type of the given ServiceInstanceIdentifier.
   *                                       Must be derived from ServiceInstanceIdentifier.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier to match against.
   * \return Map of matching ServiceInstanceIdentifier entries with const references to their respective values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  template <class ServiceInstanceIdentifierType>
  auto MatchAndGet(ServiceInstanceIdentifierType const& service_instance_identifier) noexcept -> MatchAndGetReturn {
    static_assert(std::is_base_of<ServiceInstanceIdentifier, ServiceInstanceIdentifierType>::value,
                  "ServiceInstanceIdentifierEntryKeyType must be derived from ServiceInstanceIdentifier.");

    MatchAndGetReturn matches;
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
    for (auto iter = service_registry_map_.cbegin(); iter != service_registry_map_.cend(); iter++) {
      ServiceInstanceIdentifierEntryKeyType const& entry_identifier{iter->first};
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      if (entry_identifier.IsMatching(service_instance_identifier)) {
        // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
        std::ignore = matches.insert({entry_identifier, At(entry_identifier)});
        // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      }
    }
    return matches;
  }

  /*!
   * \brief Insert an entry into the service registry.
   * \details If the service registry already contains an entry with an equivalent
   *          ServiceInstanceIdentifier, the value for this identifier is replaced.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to insert.
   * \param[in] value value to insert
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  void Insert(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier,
              EntryValueType&& value) noexcept {
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
    if (Contains(service_instance_identifier)) {
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      At(service_instance_identifier) = std::move(value);
    } else {
      // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
      auto pair = std::make_pair(service_instance_identifier, std::move(value));
      // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
      std::ignore = service_registry_map_.insert(std::move(pair));
      // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    }
  }

  /*!
   * \brief Erase an entry from the service registry.
   * \details If the service registry does not contain an entry with an
   *          equivalent ServiceInstanceIdentifier, this method has no effect.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the entry to erase.
   * \context ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // clang-format off
  void Erase(ServiceInstanceIdentifierEntryKeyType const& service_instance_identifier) noexcept /* COV_IpcSD_unused */ {
    // clang-format on
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::ignore = service_registry_map_.erase(service_instance_identifier);
    // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*!
   * \brief Remove all entries from the registry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  void Clear() noexcept { service_registry_map_.clear(); }

 protected:
  /*!
   * \brief Construct an empty ServiceRegistryBase.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr ServiceRegistryBase() noexcept = default;

  /*!
   * \brief Construct a ServiceRegistryBase from a ServiceRegistryBaseMap.
   * \param[in] service_registry_map A map mapping ServiceInstanceIdentifier to values of type EntryValueType.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component
   * internal.
   */
  constexpr explicit ServiceRegistryBase(ServiceRegistryBaseMap&& service_registry_map) noexcept
      // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
      : service_registry_map_{std::move(service_registry_map)} {}

  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  ~ServiceRegistryBase() noexcept = default;
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  ServiceRegistryBase(ServiceRegistryBase const&) = default;
  ServiceRegistryBase(ServiceRegistryBase&&) noexcept = default;
  auto operator=(ServiceRegistryBase const&) & -> ServiceRegistryBase& = default;
  auto operator=(ServiceRegistryBase&&) & noexcept -> ServiceRegistryBase& = default;

 private:
  /*!
   * \brief Hashmap used as storage with a ServiceInstanceIdentifierEntryKeyType as key and user-defined type
   * EntryValueType as value.
   */
  ServiceRegistryBaseMap service_registry_map_;
  Logger logger_{Constants::kLoggerPrefixServiceRegistry};

  /*!
   * \brief Abort message for no such entry.
   */
  static constexpr char const* const kNoSuchEntryAbortMessage{"Service registry has no such entry."};

  /*! Friend declarations for testing purposes. */
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, EmptyConstructor);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, RegistryMapConstructor);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, InsertExistingEntry);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, InsertNonExistingEntry);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, EraseExistingEntry);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, EraseNonExistingEntry);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, Clear);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, Iterate);
  FRIEND_TEST(UT__ServiceRegistry__ServiceRegistry, IterateConst);
};

/*!
 * \brief Default implementation of a ServiceRegistry with no additional behavior.
 * \details The base class cannot be instantiated, as it has no public constructors or destructors to
 *          prevent polymorphic use. In order to make the destructor public, it should be declared virtual. But the
 *          ServiceRegistry should never be used in a polymorphic way, and therefore this default implementation is
 *          required.
 * \tparam ServiceInstanceIdentifierEntryKeyType Entry key type must be derived from ServiceInstanceIdentifier.
 * \tparam EntryValueType Type used to store information about an entry to the registry.
 *         Must be no-throw move constructable and no-throw move assignable.
 * \vprivate Component Private
 * \trace DSGN-IpcServiceDiscovery-Centralized-ServiceRegistry
 */
template <class ServiceInstanceIdentifierEntryKeyType, class EntryValueType>
class ServiceRegistry final : public ServiceRegistryBase<ServiceInstanceIdentifierEntryKeyType, EntryValueType> {
 public:
  /*!
   * \brief Type of service registry map.
   */
  using ServiceRegistryMap =
      typename ServiceRegistryBase<ServiceInstanceIdentifierEntryKeyType, EntryValueType>::ServiceRegistryBaseMap;

  /*!
   * \brief Construct an empty ServiceRegistry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr ServiceRegistry() noexcept = default;

  /*!
   * \brief Construct a ServiceRegistry from a ServiceRegistryMap.
   * \param[in] service_registry_map A map mapping ServiceInstanceIdentifierEntryKeyType
   *                                 to values of type EntryValueType.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit ServiceRegistry(ServiceRegistryMap&& service_registry_map) noexcept
      : ServiceRegistryBase<ServiceInstanceIdentifierEntryKeyType, EntryValueType>(std::move(service_registry_map)) {}
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SERVICE_REGISTRY_SERVICE_REGISTRY_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_unused
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is part of the public unit interface and supposed to be helpful for future extensions of
//            the component and tested by unit tests, no component test function coverage needs to be achieved for this
//            function. Usage of the function is strictly forbidden by precondition and compliance with this
//            precondition is checked by review.
// COV_JUSTIFICATION_END
