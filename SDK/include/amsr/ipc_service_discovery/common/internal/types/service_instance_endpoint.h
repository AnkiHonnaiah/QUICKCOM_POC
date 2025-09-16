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
 *  \brief Public common ServiceInstanceEndpoint type.
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_SERVICE_INSTANCE_ENDPOINT_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_SERVICE_INSTANCE_ENDPOINT_H_

#include <cstdint>
#include <tuple>
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/types/unicast_address.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Service instance endpoint for a provided service instance.
 */
class ServiceInstanceEndpoint {
 public:
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  ProvidedServiceInstanceIdentifier provided_service_instance_identifier;
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  amsr::ipc::UnicastAddress unicast_address;

  /*!
   * \brief Compare the ServiceInstanceEndpoint against another for equality.
   * \param[in] other ServiceInstanceEndpoint to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(ServiceInstanceEndpoint const& other) const noexcept -> bool {
    return std::tie(provided_service_instance_identifier, unicast_address) ==
           std::tie(other.provided_service_instance_identifier, other.unicast_address);
  }
};

/*!
 * \brief Log streaming operator for type ServiceInstanceEndpoint.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] service_instance_endpoint A const reference to the ServiceInstanceEndpoint.
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
                       ServiceInstanceEndpoint const& service_instance_endpoint
                       ) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  ProvidedServiceInstanceIdentifier const provided_service_instance_identifier{
      service_instance_endpoint.provided_service_instance_identifier};
  UnicastAddress const unicast_address{service_instance_endpoint.unicast_address};
  log_stream << "ServiceInstanceEndpoint: ";
  log_stream << "[";
  log_stream << provided_service_instance_identifier << ", ";  // COV_IpcSD_utility
  log_stream << unicast_address;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_SERVICE_INSTANCE_ENDPOINT_H_
