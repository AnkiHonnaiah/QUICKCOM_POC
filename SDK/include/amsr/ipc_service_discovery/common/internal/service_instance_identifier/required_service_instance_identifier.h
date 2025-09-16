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
 *  \brief Implementation of a required service instance identifier
 *  \unit IpcServiceDiscovery::Common::ServiceInstanceIdentifier
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_REQUIRED_SERVICE_INSTANCE_IDENTIFIER_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_REQUIRED_SERVICE_INSTANCE_IDENTIFIER_H_

#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/base/service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

class ProvidedServiceInstanceIdentifier;

/*!
 * \brief   An identifier for required AUTOSAR service instances at binding level.
 * \details A ServiceInstanceIdentifier consists of a tuple of ServiceId, InstanceId, MajorVersion and MinorVersion.
 *          The RequiredServiceInstanceIdentifier can be used to request matching service instances at binding level.
            It also defines wildcards and functionality for matching ProvidedServiceInstances.
 *
 * \vprivate Component internal.
 * \trace DSGN-IpcServiceDiscovery-Common-RequiredServiceInstanceIdentifier
 */
class RequiredServiceInstanceIdentifier final : public ServiceInstanceIdentifier {
 public:
  using ServiceInstanceIdentifier::kInstanceIdAll;
  using ServiceInstanceIdentifier::kMinorVersionAny;

  /*!
   * \brief Construct a RequiredServiceInstanceIdentifier.
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
  using ServiceInstanceIdentifier::ServiceInstanceIdentifier;

  /*!
   * \brief Construct a RequiredServiceInstanceIdentifier out of a ServiceInstanceIdentifier.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier of the service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit RequiredServiceInstanceIdentifier(
      ServiceInstanceIdentifier const& service_instance_identifier) noexcept
      : ServiceInstanceIdentifier{service_instance_identifier} {}

  /*!
   * \brief Match the RequiredServiceInstanceIdentifier against a ProvidedServiceInstanceIdentifier.
   * \details Wildcard fields are treated as wildcards and match any field of the same type.
   *          Furthermore, given MinorVersion must be greater or equal.
   * \param[in] provided_service_instance_identifier ProvidedServiceInstanceIdentifier to match against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMatching(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept -> bool;

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
   * \brief Match the ServiceId of the given ProvidedServiceInstanceIdentifier against ServiceId.
   * \param[in] provided_service_instance_identifier ProvidedServiceInstanceIdentifier to match ServiceId against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsServiceIdMatching(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept
      -> bool;

  /*!
   * \brief Match the InstanceId of the given ProvidedServiceInstanceIdentifier against InstanceId.
   * \details Wildcard InstanceIdAll matches to any InstanceId.
   * \param[in] provided_service_instance_identifier ProvidedServiceInstanceIdentifier to match InstanceId against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsInstanceIdMatching(
      ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept -> bool;

  /*!
   * \brief Match the MajorVersion of the given RequiredServiceInstanceIdentifier against MajorVersion.
   * \param[in] provided_service_instance_identifier ProvidedServiceInstanceIdentifier to match MajorVersion against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMajorVersionMatching(
      ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept -> bool;

  /*!
   * \brief Match the MinorVersion of the given ProvidedServiceInstanceIdentifier against MinorVersion.
   * \details Wildcard MinorVersionAny matches to any InstanceId. Provided MinorVersion must be greater or equal.
   * \param[in] provided_service_instance_identifier ProvidedServiceInstanceIdentifier to match MinorVersion against.
   * \return True if matching, false if not matching.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto IsMinorVersionMatching(
      ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept -> bool;
};

/*!
 * \brief Log streaming operator for type ProvidedServiceInstanceIdentifier.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] required_service_instance_identifier A const reference to the RequiredServiceInstanceIdentifier.
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
                       RequiredServiceInstanceIdentifier const& required_service_instance_identifier
                       ) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  return RequiredServiceInstanceIdentifier::Log(log_stream, "RequiredServiceInstanceIdentifier",
                                                required_service_instance_identifier);
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

/*!
 * \brief A hash functor injection into namespace std for RequiredServiceInstanceIdentifier.
 * \details Is used by std::unordered_set and std::unordered_map containers to hash a
 *          ServiceInstanceIdentifier for efficient access.
 * \trace DSGN-IpcServiceDiscovery-Common-ServiceInstanceIdentifier
 * \vprivate Component internal.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_IPCSERVICEDISCOVERY_M14.7.3_hashFunctorSpecializationNotDeclaredInSameFile
template <>
class std::hash<::amsr::ipc_service_discovery::common::internal::RequiredServiceInstanceIdentifier> {
 public:
  /*!
   * \brief ServiceInstanceIdentifier type.
   */
  using ProvidedServiceInstanceIdentifier =
      ::amsr::ipc_service_discovery::common::internal::RequiredServiceInstanceIdentifier;
  auto operator()(ProvidedServiceInstanceIdentifier const& service_instance_identifier) const noexcept -> std::size_t {
    return service_instance_identifier.CalculateHash();
  }
};

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SERVICE_INSTANCE_IDENTIFIER_REQUIRED_SERVICE_INSTANCE_IDENTIFIER_H_
