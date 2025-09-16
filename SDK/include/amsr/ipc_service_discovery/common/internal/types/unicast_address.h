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
 *  \brief Public common UnicastAddress type.
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_UNICAST_ADDRESS_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_UNICAST_ADDRESS_H_

#include "amsr/ipc/unicast_address.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Unicast address for a SafeIpc connection.
 */
using UnicastAddress = ::amsr::ipc::UnicastAddress;

/*!
 * \brief Log streaming operator for type UnicastAddress.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] unicast_address A const reference to the UnicastAddress.
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
                       UnicastAddress const& unicast_address) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  ::amsr::ipc::Domain const domain{unicast_address.GetDomain()};
  ::amsr::ipc::Port const port{unicast_address.GetPort()};
  log_stream << "UnicastAddress: ";
  log_stream << "[";
  log_stream << "Domain: " << domain;
  log_stream << ", Port: " << port;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_UNICAST_ADDRESS_H_
