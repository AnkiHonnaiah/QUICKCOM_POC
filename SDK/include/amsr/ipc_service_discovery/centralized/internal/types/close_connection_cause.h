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
 *  \brief Class for holding the cause of a close request of a SafeIpcConnection.
 *  \unit IpcServiceDiscovery::Centralized::IpcCommon
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CLOSE_CONNECTION_CAUSE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CLOSE_CONNECTION_CAUSE_H_

#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Identifier for the cause of a close of a connection request.
 */
struct CloseConnectionCause {
  /*!
   * \brief The contained value.
   */
  std::uint8_t value{kDefault.value};

  /*!
   * \brief CloseConnectionCause indicating that the close of connection was requested by the client.
   */
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_IPCSERVICEDISCOVERY_A3.3.2_doNotUseNonPODTypeObjectsWithStaticStorageDuration
  static CloseConnectionCause const kDefault;

  /*!
   * \brief CloseConnectionCause indicating that the close of connection was triggered by a shutdown
   *        of the server itself.
   */
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_IPCSERVICEDISCOVERY_A3.3.2_doNotUseNonPODTypeObjectsWithStaticStorageDuration
  static CloseConnectionCause const kShutdown;
  /*!
   * \brief CloseConnectionCause indicating that the close of connection was triggered abnormal connection close.
   */
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_IPCSERVICEDISCOVERY_A3.3.2_doNotUseNonPODTypeObjectsWithStaticStorageDuration
  static CloseConnectionCause const kAbnormalClose;
};

/*!
 * \brief Compare CloseConnectionCause against another.
 * \param[in] left  Left side CloseConnectionCause to compare against.
 * \param[in] right Right side CloseConnectionCause to compare against.
 * \return True if equal, false if different.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component internal.
 */
constexpr auto operator==(CloseConnectionCause const& left, CloseConnectionCause const& right) noexcept -> bool {
  return left.value == right.value;
}

/*!
 * \brief Compare CloseConnectionCause against another.
 * \param[in] left  Left side CloseConnectionCause to compare against.
 * \param[in] right Right side CloseConnectionCause to compare against.
 * \return True if not equal, false if equal.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component internal.
 */
constexpr auto operator!=(CloseConnectionCause const& left, CloseConnectionCause const& right) noexcept -> bool {
  return left.value != right.value;
}

/*!
 * \brief Log streaming operator for type CloseConnectionCause.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] close_connection_cause A const reference to the CloseConnectionCause.
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
  CloseConnectionCause const& close_connection_cause) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "CloseConnectionCause: ";
  log_stream << "[";
  if (close_connection_cause == CloseConnectionCause::kShutdown) {
    log_stream << "kShutdown";
  } else if (close_connection_cause == CloseConnectionCause::kDefault) {
    log_stream << "kDefault";
  } else if (close_connection_cause == CloseConnectionCause::kAbnormalClose) {
    log_stream << "kAbnormalClose";
  } else {
    log_stream << "Custom: " << close_connection_cause.value;
  }
  return log_stream << "]";
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_CLOSE_CONNECTION_CAUSE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
