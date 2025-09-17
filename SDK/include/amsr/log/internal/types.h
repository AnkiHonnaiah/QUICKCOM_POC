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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file
 *      \brief      Contains types for serializing.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TYPES_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/log/internal/log_error_domain.h"
#include "vac/language/byte.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Type of written or read bytes
 */
using Count = std::size_t;

/*!
 * \brief The byte type
 */
using Byte = vac::language::byte;

/*!
 * \brief View into a byte array
 */
using ByteView = core::Span<Byte>;

/*!
 * \brief View into a byte array
 */
using ConstByteView = core::Span<Byte const>;

/*!
 * \brief Serializer results
 */
using SerResult = Result<ByteView>;
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TYPES_H_
