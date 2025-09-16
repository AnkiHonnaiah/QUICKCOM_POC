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
/*!        \file  ara/crypto/cryp/memory_pool.h
 *        \brief  Memory pool type definitions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MEMORY_POOL_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MEMORY_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Type of Reserved Context Index (maximal value means "NOT RESERVED").
 * \details Values of this type should be used for indexing slots reserved
 *          for Cryptographic Contexts in the "Memory Pool".
 * \vpublic
 */
using ReservedContextIndex = std::size_t;

/*!
 * \brief Type of Reserved Object Index (maximal value means "NOT RESERVED").
 * \details Values of this type should be used for indexing slots reserved
 *          for Cryptographic Objects in the "Memory Pool".
 * \vpublic
 */
using ReservedObjectIndex = std::size_t;

/*! Marker that the Context should be allocated on the heap. */
static ReservedContextIndex const kAllocContextOnHeap{
    static_cast<ReservedContextIndex>(std::numeric_limits<ReservedContextIndex>::max())};

/*! Marker that the Object should be allocated on the heap. */
static ReservedObjectIndex const kAllocObjectOnHeap{
    static_cast<ReservedObjectIndex>(std::numeric_limits<ReservedContextIndex>::max())};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MEMORY_POOL_H_
