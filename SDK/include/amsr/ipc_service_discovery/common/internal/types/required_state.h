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
 *  \brief Public common RequiredState type.
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_REQUIRED_STATE_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_REQUIRED_STATE_H_

#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief States for a required service instance.
 */
enum class RequiredState : std::uint8_t { kRequired, kNotRequired };

/*!
 * \brief Log streaming operator for type RequiredState.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] required_state A const reference to the RequiredState.
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
                       RequiredState const& required_state) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "RequiredState: ";
  log_stream << "[";
  if (required_state == RequiredState::kRequired) {
    log_stream << "kRequired";
  }
  if (required_state == RequiredState::kNotRequired) {
    log_stream << "kNotRequired";
  }
  return log_stream << "]";
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_REQUIRED_STATE_H_
