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
 *        \brief  Provides error mappings for common operating system APIs used in Safe IPC.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *        \unit   osabstraction::ipc::IpcOsInterface_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_ERRORS_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_ERRORS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps mapping memory error to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error during mapping of shared memory.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to map shared memory.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot map shared memory because the file or the file
 *                                                            system does not support memory mapping or the specified
 *                                                            range is invalid for the file or the file cannot be
 *                                                            accessed.
 * \retval osabstraction::OsabErrc::kBusy                     Cannot map shared memory because the underlying file is
 *                                                            busy or because the mapping could not be locked into
 *                                                            memory.
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to map shared memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapMapMemoryError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_OS_API_ERRORS_INTERFACE_H_
