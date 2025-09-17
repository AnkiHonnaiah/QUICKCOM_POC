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
 *  \brief Contains wrapper functions with logging around ::ara::core::Abort
 *  \unit IpcServiceDiscovery::Common::AbortWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_ABORT_WRAPPER_ABORT_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_ABORT_WRAPPER_ABORT_H_

#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "ara/core/abort.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Abort execution of the program with an abort message.
 * \param[in] abort_message The message to print before aborting.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCSERVICEDISCOVERY_M0.1.8_voidFunctionHasNoSideEffect
// clang-format off
[[noreturn]] inline void Abort(char const* abort_message) /* COV_IpcSD_utility */ {
  // clang-format on
  ::ara::core::Abort(abort_message);
}

/*!
 * \brief Log a log message with fatal severity and abort execution of the program with an abort message.
 * \param[in] abort_message The message to print before aborting.
 * \param[in] logger A const reference to the Logger.
 * \param[in] log_message A StringView to the message to log with severity fatal.
 * \param[in] location_opt An optional with location information for the Logger.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
[[noreturn]] inline void Abort(
    char const* abort_message, common::internal::Logger const& logger, ::ara::core::StringView const log_message,
    ::ara::core::Optional<common::internal::Logger::Location> const& location_opt = {}) /* COV_IpcSD_utility */ {
  // clang-format on
  logger.Log(::amsr::ipc_service_discovery::common::internal::Logger::LogLevel::kFatal, log_message,
             location_opt);  // COV_IpcSD_utility
  Abort(abort_message);
}

/*!
 * \brief Log a log message with fatal severity and abort execution of the program with an abort message.
 * \param[in] abort_message The message to print before aborting.
 * \param[in] logger A const reference to the Logger.
 * \param[in] log_message A LogCallback to stream the message to log with severity fatal.
 * \param[in] location_opt An optional with location information for the Logger.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
[[noreturn]] inline void Abort(
    char const* abort_message, common::internal::Logger const& logger, Logger::LogCallback log_message,
    ::ara::core::Optional<common::internal::Logger::Location> const& location_opt = {}) /* COV_IpcSD_utility */ {
  // clang-format on
  // VCA_IPCSERVICEDISCOVERY_SLC_20_SAFE_LOG_FUNCTION
  logger.Log(::amsr::ipc_service_discovery::common::internal::Logger::LogLevel::kFatal, std::move(log_message),
             location_opt);  // COV_IpcSD_utility
  Abort(abort_message);
}

/*!
 * \brief Log a log message with fatal severity and abort execution of the program with an abort message.
 * \param[in] abort_message The message to print before aborting.
 * \param[in] logger A const reference to the Logger.
 * \param[in] error An ErrorCode to be logged with severity fatal.
 * \param[in] location_opt An optional with location information for the Logger.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
[[noreturn]] inline void Abort(
    char const* abort_message, common::internal::Logger const& logger, ::ara::core::ErrorCode const& error,
    ::ara::core::Optional<common::internal::Logger::Location> const& location_opt = {}) /* COV_IpcSD_utility */ {
  // clang-format on
  Abort(
      abort_message, logger,
      [abort_message, &error](auto& log_stream) { log_stream << abort_message << " Error: " << error; }, location_opt);
}

/*!
 * \brief Log a log message with fatal severity and abort execution of the program with an abort message.
 * \param[in] abort_message The message to print before aborting.
 * \param[in] logger A const reference to the Logger.
 * \param[in] location_opt An optional with location information for the Logger.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
[[noreturn]] inline void Abort(char const* abort_message, common::internal::Logger const& logger,
                               common::internal::Logger::Location const& location_opt = {}) /* COV_IpcSD_utility */ {
  // clang-format on
  Abort(
      abort_message, logger, [abort_message](auto& log_stream) { log_stream << abort_message; }, location_opt);
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_ABORT_WRAPPER_ABORT_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial abort delegation function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
