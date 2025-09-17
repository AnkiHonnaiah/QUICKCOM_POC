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
 *  \brief Implementation of a provided service instance identifier
 *  \unit IpcServiceDiscovery::Common::ServiceInstanceIdentifier
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_PROVIDED_SERVICE_INSTANCE_IDENTIFIER_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_PROVIDED_SERVICE_INSTANCE_IDENTIFIER_H_

#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/base/service_instance_identifier.h"
#include "ara/core/abort.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

class RequiredServiceInstanceIdentifier;

/*!
 * \brief   An identifier for provided AUTOSAR service instances at binding level.
 * \details A ServiceInstanceIdentifier consists of a tuple of ServiceId, InstanceId, MajorVersion and MinorVersion.
 *          This tuple identifies a service instance and is unique within a binding.
 *
 * \vprivate Component internal.
 * \trace DSGN-IpcServiceDiscovery-Common-ProvidedServiceInstanceIdentifier
 */
class ProvidedServiceInstanceIdentifier final : public ServiceInstanceIdentifier {
 public:
  /*!
   * \brief Construct a ProvidedServiceInstanceIdentifier.
   * \param[in] service_id ServiceId of the service instance.
   * \param[in] instance_id InstanceId of the service instance.
   * \param[in] major_version MajorVersion of the service instance.
   * \param[in] minor_version MinorVersion of the service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr ProvidedServiceInstanceIdentifier(ServiceId service_id, InstanceId instance_id, MajorVersion major_version,
                                              MinorVersion minor_version) noexcept
      : ServiceInstanceIdentifier{service_id, instance_id, major_version, minor_version} {
    AssertNoWildcards();
  }

  /*!
   * \brief Construct a ProvidedServiceInstanceIdentifier out of a ServiceInstanceIdentifier.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit ProvidedServiceInstanceIdentifier(
      ServiceInstanceIdentifier const& service_instance_identifier) noexcept
      : ServiceInstanceIdentifier{service_instance_identifier} {
    AssertNoWildcards();
  }

  /*!
   * \brief Transform ProvidedServiceInstanceIdentifier to RequiredServiceInstanceIdentifier.
   * \return Transformed RequiredServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto ToRequiredServiceInstanceIdentifier() const noexcept -> RequiredServiceInstanceIdentifier;

  /*!
   * \brief Match the ProvidedServiceInstanceIdentifier against a RequiredServiceInstanceIdentifier.
   * \details Wildcard fields are treated as wildcards and match any field of the same type.
   *          Furthermmore, given MinorVersion must be smaller or equal.
   * \param[in] required_service_instance_identifier RequiredServiceInstanceIdentifier to match against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMatching(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept -> bool;

  /*!
   * \brief Check if ProvidedServiceInstanceIdentifier is valid.
   * \details Check if no wildcards are set.
   * \return True if ProvidedServiceInstanceIdentifier is valid, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto IsValid() const noexcept -> bool { return !HasWildcards(); }

  /*!
   * \brief Fetch protected functions of the base class into the public namespace.
   */
  using ServiceInstanceIdentifier::GetInstanceId;
  using ServiceInstanceIdentifier::GetMajorVersion;
  using ServiceInstanceIdentifier::GetMinorVersion;
  using ServiceInstanceIdentifier::GetServiceId;
  using ServiceInstanceIdentifier::SetInstanceId;
  using ServiceInstanceIdentifier::SetMajorVersion;
  using ServiceInstanceIdentifier::SetMinorVersion;
  using ServiceInstanceIdentifier::SetServiceId;
  using ServiceInstanceIdentifier::operator==;
  using ServiceInstanceIdentifier::operator!=;
  using ServiceInstanceIdentifier::operator<;
  using ServiceInstanceIdentifier::CalculateHash;

 private:
  /*!
   * \brief Check if ProvidedServiceInstanceIdentifier has wildcards.
   * \return
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr auto HasWildcards() const noexcept -> bool {
    return ((GetInstanceId() == ServiceInstanceIdentifier::kInstanceIdAll) ||
            (GetMinorVersion() == ServiceInstanceIdentifier::kMinorVersionAny));
  }

  /*!
   * \brief Assert ProvidedServiceInstanceIdentifier has no wildcards.
   * \return
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCSERVICEDISCOVERY_M0.1.8_voidFunctionHasNoSideEffect
  constexpr void AssertNoWildcards() const noexcept {
    if (HasWildcards()) {
      ::ara::core::Abort("Wildcards are not allowed for ProvidedServiceInstanceIdentifier.");
    }
  }

  /*!
   * \brief Match the ServiceId of the given RequiredServiceInstanceIdentifier against ServiceId.
   * \param[in] required_service_instance_identifier RequiredServiceInstanceIdentifier to match ServiceId against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsServiceIdMatching(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept
      -> bool;

  /*!
   * \brief Match the InstanceId of the given RequiredServiceInstanceIdentifier against InstanceId.
   * \details Wildcard InstanceIdAll matches to any InstanceId.
   * \param[in] required_service_instance_identifier RequiredServiceInstanceIdentifier to match InstanceId against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsInstanceIdMatching(
      RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept -> bool;

  /*!
   * \brief Match the MajorVersion of the given RequiredServiceInstanceIdentifier against MajorVersion.
   * \param[in] required_service_instance_identifier RequiredServiceInstanceIdentifier to match MajorVersion against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMajorVersionMatching(
      RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept -> bool;

  /*!
   * \brief Match the MinorVersion of the given RequiredServiceInstanceIdentifier against MinorVersion.
   * \details Wildcard MinorVersionAny matches to any InstanceId. Required MinorVersion must be smaller or equal.
   * \param[in] required_service_instance_identifier RequiredServiceInstanceIdentifier to match MinorVersion against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMinorVersionMatching(
      RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept -> bool;
};

/*!
 * \brief Log streaming operator for type ProvidedServiceInstanceIdentifier.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] provided_service_instance_identifier A const reference to the ProvidedServiceInstanceIdentifier.
 * \return The reference to the LogStream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       ProvidedServiceInstanceIdentifier const& service_instance_identifier
                       ) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  return ProvidedServiceInstanceIdentifier::Log(log_stream, "ProvidedServiceInstanceIdentifier",
                                                service_instance_identifier);
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

/*!
 * \brief A hash functor injection into namespace std for ProvidedServiceInstanceIdentifier.
 * \details Is used by std::unordered_set and std::unordered_map containers to hash a
 *          ServiceInstanceIdentifier for efficient access.
 * \trace DSGN-IpcServiceDiscovery-Common-ServiceInstanceIdentifier
 * \vprivate Component internal.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_IPCSERVICEDISCOVERY_M14.7.3_hashFunctorSpecializationNotDeclaredInSameFile
template <>
class std::hash<::amsr::ipc_service_discovery::common::internal::ProvidedServiceInstanceIdentifier> {
 public:
  /*!
   * \brief ServiceInstanceIdentifier type.
   */
  using ProvidedServiceInstanceIdentifier =
      ::amsr::ipc_service_discovery::common::internal::ProvidedServiceInstanceIdentifier;
  auto operator()(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept -> std::size_t {
    return service_instance_identifier.CalculateHash();
  }
};

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_PROVIDED_SERVICE_INSTANCE_IDENTIFIER_H_
