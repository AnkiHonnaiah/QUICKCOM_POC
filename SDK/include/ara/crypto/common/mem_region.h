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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ara/crypto/common/mem_region.h
 *        \brief  Type definitions for memory regions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_MEM_REGION_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_MEM_REGION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "ara/core/span.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {

/*!
 * \brief A unified type definition for a single byte.
 * \vpublic
 */
using Byte = std::uint8_t;

/*!
 * \brief Read-Write Memory Region (intended for [in/out] arguments)
 * \vpublic
 */
using ReadWriteMemRegion = ara::core::Span<Byte>;

/*!
 * \brief Writable Memory Region (intended for [out] arguments)
 * \vpublic
 */
using WritableMemRegion = ara::core::Span<Byte>;

/*!
 * \brief Read-Only Memory Region (intended for [in] arguments)
 * \vpublic
 */
using ReadOnlyMemRegion = ara::core::Span<Byte const>;

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_MEM_REGION_H_
