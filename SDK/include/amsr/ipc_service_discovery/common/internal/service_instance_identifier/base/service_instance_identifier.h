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
 *  \brief Implementation of an abstract service instance identifier
 *  \unit IpcServiceDiscovery::Common::ServiceInstanceIdentifier
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_BASE_SERVICE_INSTANCE_IDENTIFIER_SERVICE_INSTANCE_IDENTIFIER_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_BASE_SERVICE_INSTANCE_IDENTIFIER_SERVICE_INSTANCE_IDENTIFIER_H_

#include <cstdint>
#include <functional>
#include <limits>
#include "ara/log/logstream.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

class ProvidedServiceInstanceIdentifier;
class RequiredServiceInstanceIdentifier;

/*!
 * \brief   An abstract identifier for AUTOSAR service instances at binding level.
 * \details A ServiceInstanceIdentifier consists of a tuple of ServiceId, InstanceId, MajorVersion and MinorVersion.
 *          This tuple identifies a service instance and is unique within a binding.
 *          This class shall not be destructed via a pointer.
 *
 * \vprivate Component internal.
 * \trace DSGN-IpcServiceDiscovery-Common-ServiceInstanceIdentifier
 */
// VECTOR NC AutosarC++17_10-A12.4.2: MD_IPCSERVICEDISCOVERY_A12.4.2_ServiceInstanceIdentifierClassNotFinal
class ServiceInstanceIdentifier {
 public:
  /*!
   * \brief Type of service ID.
   */
  using ServiceId = std::uint32_t;

  /*!
   * \brief Type of instance ID.
   */
  using InstanceId = std::uint32_t;

  /*!
   * \brief Type of major version.
   */
  using MajorVersion = std::uint32_t;

  /*!
   * \brief Type of minor version.
   */
  using MinorVersion = std::uint32_t;

  /*!
   * \brief Construct a ServiceInstanceIdentifier.
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
  constexpr ServiceInstanceIdentifier(ServiceId service_id, InstanceId instance_id, MajorVersion major_version,
                                      MinorVersion minor_version) noexcept
      : service_id_(service_id),
        instance_id_(instance_id),
        major_version_(major_version),
        minor_version_(minor_version) {}

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre Shall not be called via a pointer.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // VECTOR NC AutosarC++17_10-A12.4.1: MD_IPCSERVICEDISCOVERY_A12.4.1_ServiceInstanceIdentifierPublicNonVirtualBaseClassDestructor
  ~ServiceInstanceIdentifier() noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCSERVICEDISCOVERY_A12.8.6_ServiceInstanceIdentifierPublicCopyAndMoveSpecialFunctions
  ServiceInstanceIdentifier(ServiceInstanceIdentifier const&) noexcept = default;
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCSERVICEDISCOVERY_A12.8.6_ServiceInstanceIdentifierPublicCopyAndMoveSpecialFunctions
  ServiceInstanceIdentifier(ServiceInstanceIdentifier&&) noexcept = default;
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCSERVICEDISCOVERY_A12.8.6_ServiceInstanceIdentifierPublicCopyAndMoveSpecialFunctions
  auto operator=(ServiceInstanceIdentifier const&) & noexcept -> ServiceInstanceIdentifier& = default;
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_IPCSERVICEDISCOVERY_A12.8.6_ServiceInstanceIdentifierPublicCopyAndMoveSpecialFunctions
  auto operator=(ServiceInstanceIdentifier&&) & noexcept -> ServiceInstanceIdentifier& = default;

  /*!
   * \brief Log streaming for type ServiceInstanceIdentifier.
   * \param[in] log_stream A reference to the LogStream.
   * \param[in] service_instance_identifier_name Name of the derived ServiceInstanceIdentifier.
   * \param[in] service_instance_identifier A const reference to the ServiceInstanceIdentifier.
   * \tparam ServiceInstanceIdentifierType Type of the given ServiceInstanceIdentifier.
   * \return The reference to the LogStream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ServiceInstanceIdentifierType>
  static auto Log(::ara::log::LogStream& log_stream, ::ara::core::StringView service_instance_identifier_name,
                  ServiceInstanceIdentifierType const& service_instance_identifier) noexcept -> ::ara::log::LogStream& {
    log_stream << service_instance_identifier_name << ": ";
    log_stream << "[";
    log_stream << "ServiceId: " << service_instance_identifier.GetServiceId() << ", ";
    log_stream << "InstanceId: " << service_instance_identifier.GetInstanceId() << ", ";
    log_stream << "MajorVersion: " << service_instance_identifier.GetMajorVersion() << ", ";
    log_stream << "MinorVersion: " << service_instance_identifier.GetMinorVersion();
    return log_stream << "]";
  }

 private:
  // These declarations prevent unintended slicing.
  explicit ServiceInstanceIdentifier(ProvidedServiceInstanceIdentifier const&) = delete;
  auto operator=(ProvidedServiceInstanceIdentifier const&) noexcept -> ServiceInstanceIdentifier& = delete;
  explicit ServiceInstanceIdentifier(RequiredServiceInstanceIdentifier const&) = delete;
  auto operator=(RequiredServiceInstanceIdentifier const&) noexcept -> ServiceInstanceIdentifier& = delete;

 protected:
  /*!
   * \brief Getter function for the ServiceId.
   * \return ServiceId of the ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetServiceId() const noexcept -> ServiceId { return service_id_; }

  /*!
   * \brief Getter function for the InstanceId.
   * \return InstanceId of the ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetInstanceId() const noexcept -> InstanceId { return instance_id_; }

  /*!
   * \brief Getter function for the MajorVersion.
   * \return MajorVersion of the ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetMajorVersion() const noexcept -> MajorVersion { return major_version_; }

  /*!
   * \brief Getter function for the MinorVersion.
   * \return MinorVersion of the ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetMinorVersion() const noexcept -> MinorVersion { return minor_version_; }

  /*!
   * \brief Setter function for the ServiceId.
   * \param[in] service_id ServiceId to be set.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */

  constexpr void SetServiceId(ServiceId service_id) noexcept { service_id_ = service_id; }
  /*!
   * \brief Setter function for the InstanceId.
   * \param[in] instance_id InstanceId to be set.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */

  constexpr void SetInstanceId(InstanceId instance_id) noexcept { instance_id_ = instance_id; }
  /*!
   * \brief Setter function for the MajorVersion.
   * \param[in] major_version MajorVersion to be set.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */

  constexpr void SetMajorVersion(MajorVersion major_version) noexcept { major_version_ = major_version; }
  /*!
   * \brief Setter function for the MinorVersion.
   * \param[in] minor_version MinorVersion to be set.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr void SetMinorVersion(MinorVersion minor_version) noexcept { minor_version_ = minor_version; }

  /*!
   * \brief Compare the ServiceInstanceIdentifier against another for equality.
   * \details Wildcard fields are treated as normal fields.
   * \param[in] other ServiceInstanceIdentifier to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(ServiceInstanceIdentifier const& other) const noexcept -> bool {
    return std::tie(service_id_, instance_id_, major_version_, minor_version_) ==
           std::tie(other.service_id_, other.instance_id_, other.major_version_, other.minor_version_);
  }

  /*!
   * \brief Compare the ServiceInstanceIdentifier against another for inequality.
   * \details Wildcard fields are treated as normal fields.
   * \param[in] other ServiceInstanceIdentifier to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator!=(ServiceInstanceIdentifier const& other) const noexcept -> bool {
    return std::tie(service_id_, instance_id_, major_version_, minor_version_) !=
           std::tie(other.service_id_, other.instance_id_, other.major_version_, other.minor_version_);
  }

  /*!
   * \brief Compare the ServiceInstanceIdentifier smaller than another.
   * \details Wildcard fields are treated as normal fields.
   * \param[in] other ServiceInstanceIdentifier to compare against.
   * \return True if smaller, false if greater.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator<(ServiceInstanceIdentifier const& other) const noexcept -> bool {
    return std::tie(service_id_, instance_id_, major_version_, minor_version_) <
           std::tie(other.service_id_, other.instance_id_, other.major_version_, other.minor_version_);
  }

  /*!
   * \brief
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto CalculateHash() const noexcept -> std::size_t {
    auto service_instance_identifier_hash = std::hash<typename ServiceInstanceIdentifier::ServiceId>{}(service_id_);
    HashCombine<ServiceInstanceIdentifier::InstanceId>(service_instance_identifier_hash, instance_id_);
    HashCombine<ServiceInstanceIdentifier::MajorVersion>(service_instance_identifier_hash, major_version_);
    HashCombine<ServiceInstanceIdentifier::MinorVersion>(service_instance_identifier_hash, minor_version_);

    return service_instance_identifier_hash;
  }

  /*!
   * \brief Wildcard expression for matching against any instance ID.
   */
  static constexpr InstanceId kInstanceIdAll{std::numeric_limits<InstanceId>::max()};

  /*!
   * \brief Wildcard expression for matching against any minor version.
   */
  static constexpr MinorVersion kMinorVersionAny{std::numeric_limits<MinorVersion>::max()};

 private:
  /*!
   * \brief Combine hash values.
   * \tparam T Type of hash key.
   * \param[in,out] seed Current hash value to be combined.
   * \param[in] value Key to be hash combined with seed.
   */
  template <typename KeyType>
  static void HashCombine(std::size_t& seed, KeyType const& value) noexcept {
    /*!
     * \brief The magic number is supposed to be 32 random bits, where each is equally likely to be 0 or 1, and with
     * no simple correlation between the bits.
     */
    constexpr std::size_t const kMagicNumber{0x9e3779b9};

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCSERVICEDISCOVERY_M0.3.1_arithmeticComputationMayCauseWrapAround
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_IPCSERVICEDISCOVERY_A4.7.1_arithmeticComputationMayCauseWrapAround
    seed ^= std::hash<KeyType>{}(value) + kMagicNumber + (seed << 6) + (seed >> 2);
  }

  /*!
   * \brief Service ID.
   */
  ServiceId service_id_;

  /*!
   * \brief Instance ID.
   */
  InstanceId instance_id_;

  /*!
   * \brief Major version.
   */
  MajorVersion major_version_;

  /*!
   * \brief Minor version.
   */
  MinorVersion minor_version_;
};

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_BASE_SERVICE_INSTANCE_IDENTIFIER_SERVICE_INSTANCE_IDENTIFIER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
