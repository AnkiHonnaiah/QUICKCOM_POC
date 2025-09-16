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
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CONSTANTS_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CONSTANTS_H_

#include <cstdint>
#include <utility>
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/version/version.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

struct Constants {
  static std::size_t const kMaximumMessageSize;

  static char const* const kLoggerPrefixSdServer;
  static char const* const kLoggerPrefixSdClient;
  static char const* const kLoggerPrefixIpcServer;
  static char const* const kLoggerPrefixIpcClient;
  static char const* const kLoggerPrefixServiceRegistry;

  static Version::MajorVersion const kMajorVersion;
  static Version::MinorVersion const kMinorVersion;
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CONSTANTS_H_
