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
/*!
 * \file crypto_adapter_types.h
 *
 * \brief Defines general types needed for the Crypto Adapter submodule
 *
 * \details Includes several type declarations and enumerations related to the Crypto Adapter submodule.
 * Following enumerations are covered:
 * - Hash algorithm
 * - Pseudo Random Function
 * - Cipher algorithm
 * - MAC algorithm
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_TYPES_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include "ara/core/vector.h"

#include "amsr/core/array.h"
#include "amsr/core/span.h"
#include "ara/crypto/cryp/crypto_provider.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {
/*!
 * \brief Contains the vendor specific binary algorithm identifier.
 * \vprivate This enum class is component private.
 */
struct AlgIdMasterSecret {
  /*!
   * \brief Algorithm identifier.
   */
  ara::crypto::cryp::CryptoProvider::AlgId const value_;
};

/*!
 * \brief Contains the vendor specific binary algorithm identifier.
 * \vprivate This enum class is component private.
 */
struct AlgIdType {
  /*!
   * \brief Algorithm identifier.
   */
  ara::crypto::cryp::CryptoProvider::AlgId const value_;
};

/*!
 * \brief enum class for type of hash algorithm
 * \details
 * kSha256: SHA2-256 hashing
 * kSha384: SHA2-384 hashing
 */
enum class EnHashAlgorithm : std::uint8_t { kNone, kSha256, kSha384 };

/*!
 * \brief PRF algorithm enumeration.
 * \details
 * kNotDefined
 * kTlsPrfSha256: SHA256 based TLS 1.2 PRF
 * kTlsPrfSha384: SHA384 based TLS 1.2 PRF
 */
enum class EnPrfAlgorithm : std::uint8_t { kNone, kTlsPrfSha256, kTlsPrfSha384 };

/*!
 * \brief Cipher algorithm enumeration.
 * \details
 * kNone: No cipher
 * kAes128Gcm: AES_128/GCM cipher
 * kAes128Cbc: AES_128/CBC cipher
 * kAes256Gcm: AES_256/GCM cipher
 * kAes256Cbc: AES_256/CBC cipher
 */
enum class EnCipherAlgorithm : std::uint8_t { kNone, kAes128Gcm, kAes128Cbc, kAes256Gcm, kAes256Cbc };

/*!
 * \brief MAC algorithm enumeration.
 * \details
 * kNone: No hashed MAC
 * kHmacSha256: HMAC SHA256
 * kHmacSha1: HMAC SHA1
 * kHmacSha384: HMAC SHA384
 */
enum class EnMacAlgorithm : std::uint8_t { kNone, kHmacSha256, kHmacSha1, kHmacSha384 };

/*!
 * \brief Length of the TLS 1.2 master secret as defined in RFC 5246.
 * \trace SPEC-6561976
 */
static constexpr std::size_t kMasterSecretLength{48U};

/*!
 * \brief Master secret type definition.
 */
using MasterSecretContainer = std::array<std::uint8_t, kMasterSecretLength>;

/*!
 * \brief Raw data buffer type that allows dynamic allocation / deallocation.
 */
using DynamicMemoryBuffer = ara::core::Vector<std::uint8_t>;

/*!
 * \brief Writeable data buffer view type.
 */
using WriteableBufferView = amsr::core::Span<std::uint8_t>;

/*!
 * \brief Read-Only data buffer view type.
 */
using ReadOnlyBufferView = amsr::core::Span<std::uint8_t const>;

/*!
 * \brief Alias for the input parameter used to generate a signature from.
 */
using RawData = ara::core::Vector<std::uint8_t>;

/*! Signature byte size */
static constexpr std::size_t kSignatureByteSize{64};

/*!
 * \brief Type alias for Ed25519 signature.
 * \details The size is equal to 64 bytes.
 */
using Signature = amsr::core::Array<std::uint8_t, kSignatureByteSize>;

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_TYPES_H_
