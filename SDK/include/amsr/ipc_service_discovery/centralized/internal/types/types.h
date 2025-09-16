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
 *  \brief Public common types.
 *  \unit IpcServiceDiscovery::Centralized::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_TYPES_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_TYPES_H_

#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCSERVICEDISCOVERY_M7.3.6_usingForCommonLibrary
// VECTOR NC AutosarC++17_10-M7.3.4: MD_IPCSERVICEDISCOVERY_M7.3.4_usingForCommonLibrary
/*!
 * \brief Using for common types for the IpcServiceDiscovery component.
 */
using namespace ::amsr::ipc_service_discovery::common::internal;

/*!
 * \brief Handle to identify a connection.
 */
using ConnectionHandle = std::uint64_t;

/*!
 * This explicit operator call is needed due to the c++ adl (Argument-dependent lookup) behaviour and in combination
 * with the use of UnicastAddress by "using UnicastAddress = ...", which then points to another namespace.
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       UnicastAddress const& unicast_address) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  return common::internal::operator<<(log_stream, unicast_address);
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_TYPES_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
