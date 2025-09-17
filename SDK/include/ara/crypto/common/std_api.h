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
/*!        \file  ara/crypto/common/std_api.h
 *        \brief  Adaptive Autosar Crypto Stack. Common API. C++ standard library headers.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_STD_API_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_STD_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/core/span.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"

namespace ara {
/*!
 * \brief Crypto API namespace
 * \details Namespace of CryptoApi "AUTOSAR Adaptive Platform Crypto API".
 *          The Crypto API defines public interfaces provided by the Crypto Stack.
 *          Crypto API includes 3 functional sub-domains presented by correspondent provider types:
 *            - Crypto Provider (multiple instances in the Stack), namespace ara::crypto::cryp
 *            - Key Storage Provider (single instance in the Stack), namespace ara::crypto::keys
 *            - X.509 Provider (single instance in the Stack), namespace ara::crypto::x509.
 *
 *          But common part of the Crypto API is defined directly in this "root" namespace ara::crypto.
 * \vpublic
 */
namespace crypto {

/*!
 * \brief Alias of the default bytes sequences allocator.
 * \vpublic
 */
using DefBytesAllocator = std::allocator<std::uint8_t>;

/*!
 * \brief Alias of a bytes' vector template with customizable allocator.
 * \tparam Alloc custom allocator of bytes sequences
 * \vpublic
 */
template <class Alloc = DefBytesAllocator>
using ByteVectorT = ara::core::Vector<std::uint8_t, Alloc>;

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_STD_API_H_
