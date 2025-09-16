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
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_TYPES_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_TYPES_H_

#include "amsr/ipc_service_discovery/common/internal/types/binding_type.h"
#include "amsr/ipc_service_discovery/common/internal/types/listen_service_callback.h"
#include "amsr/ipc_service_discovery/common/internal/types/provided_state.h"
#include "amsr/ipc_service_discovery/common/internal/types/required_state.h"
#include "amsr/ipc_service_discovery/common/internal/types/service_instance_endpoint.h"
#include "amsr/ipc_service_discovery/common/internal/types/unicast_address.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*! String literals. */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCSERVICEDISCOVERY_M7.3.6_sv
using vac::container::literals::operator""_sv;

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_TYPES_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
