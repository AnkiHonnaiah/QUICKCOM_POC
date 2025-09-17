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
/*!        \file
 *        \brief  Maps IPC errors to appropriate reactions.
 *
 *      \details  Logs regular error and aborts on development errors.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ERROR_MAPPING_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ERROR_MAPPING_H_

#include "amsr/core/error_code.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Logs a BasicIpc communication error.
 * \details Logs an appropriate error message. Aborts if it is a development error and the error shows that the state
 *          machine has been corrupted.
 *
 * \param[in] logger    Error messages will be logged to this logger.
 * \param[in] error     BasicIpc error code.
 * \param[in] location  Origin of the log output.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE if access to logger is synchronized.
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::IpcBindingLogging
 */
void LogBasicIpcCommunicationError(::amsr::ipc_binding_core::internal::logging::AraComLogger const& logger,
                                   amsr::core::ErrorCode const& error,
                                   logging::AraComLogger::LogLocation const location) noexcept;

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr
#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ERROR_MAPPING_H_
