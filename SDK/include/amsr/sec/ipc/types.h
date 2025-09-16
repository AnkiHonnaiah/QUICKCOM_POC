// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ipc/types.h
 *        \brief  Type definitions for memory regions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TYPES_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/span.h"
#include "amsr/ipc/credentials.h"
#include "osabstraction/process/process_types.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Type containing unix user ids provided by osabstraction.
 * \vprivate Component Private
 */
using UserId = amsr::ipc::Credentials;

/*!
 * \brief Type containing process ids provided by osabstraction.
 * \vprivate Component Private
 */
using ProcessId = osabstraction::process::ProcessId;

/*!
 * \brief Writable Memory Region (intended for [out] arguments)
 * \vprivate Component Private
 */
using WritableMemRegion = amsr::core::Span<std::uint8_t>;

/*!
 * \brief Read-Only Memory Region (intended for [in] arguments)
 * \vprivate Component Private
 */
using ReadOnlyMemRegion = amsr::core::Span<std::uint8_t const>;

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TYPES_H_
