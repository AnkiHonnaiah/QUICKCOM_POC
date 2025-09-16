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
 *  \brief Startup method for running the centralized daemon application.
 *  \unit IpcServiceDiscovery::Centralized::DaemonApplication
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_STARTUP_STARTUP_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_STARTUP_STARTUP_H_

#include "amsr/ipc_service_discovery/centralized/internal/daemon_application/daemon_application.h"
#include "amsr/ipc_service_discovery/common/internal/abort_wrapper/abort.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_IPCSERVICEDISCOVERY_A18.1.1_cStyleArraysShouldNotBeUsedForMainParameters
// VECTOR Next Construct VectorC++-V3.0.1: MD_IPCSERVICEDISCOVERY_V3.0.1_cStyleArraysShallNotBeUsedForMainParameters
// VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_IPCSERVICEDISCOVERY_V3.9.1_fixedWidthIntegerTypesShallBeUsedForMainParameters
/*!
 * \brief         Starts IpcServiceDiscovery daemon application.
 * \details       Creates application and starts runtime.
 * \param[in]     argc  Command line argument count.
 * \param[in]     argv  Array of pointers to command line arguments.
 * \return        Exit code which shall be returned during EM process termination.
 * \context       InitPhase
 * \pre           -
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \threadsafe    FALSE
 */
auto Startup(int argc, char const* const argv[]) -> int;

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_STARTUP_STARTUP_H_
