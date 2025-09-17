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
/*!        \file  ara/crypto/common/internal/base_id_types_without_uuid.h
 *        \brief  Definition of base ID types except for the ones requiring UUIDs.
 *        \attention Deviation: This file is not specified by the [SWS 19-03] but introduced to circumvent circular
 *         dependencies.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_INTERNAL_BASE_ID_TYPES_WITHOUT_UUID_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_INTERNAL_BASE_ID_TYPES_WITHOUT_UUID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace ara {
namespace crypto {
namespace internal {

/*!
 * \brief Crypto Algorithm Identifier type.
 * \vpublic
 */
using CryptoAlgId = std::uint64_t;

/*!
 * \brief A container type for bit-flags of allowed usages of a key or a secret seed object.
 * \details Only directly specified usages of a key are allowed, all other are prohibited!
 * \details Similar set of flags are defined for the usage restrictions of original key/seed
 *          and for a symmetric key or seed that potentially can be derived from the original one.
 * \details A symmetric key or secret seed can be derived from the original one, only if it supports
 *          ALLOW_KEY_AGREEMENT or ALLOW_KEY_DIVERSIFY or ALLOW_KEY_DERIVATION!
 * \vpublic
 */
using AllowedUsageFlags = std::uint32_t;

/*!
 * \brief Enumeration of all types of crypto objects, i.e. types of content that can be stored to a key slot.
 * \vpublic
 */
enum class CryptoObjectType : std::uint8_t {
  kNone = 0,             /*!< Used for empty containers (key slots) and in a case of the dependency absence. */
  kUnknown = 0,          /*!< Object type unknown (meaning is identical to the previous one) */
  kDomainParameters = 1, /*!< Domain Parameters object */
  kSymmetricKey = 2,     /*!< Symmetric Key object */
  kPrivateKey = 3,       /*!< Private Key object */
  kPublicKey = 4,        /*!< Public Key object */
  kSignature = 5,        /*!< Digital Signature (or MAC/HMAC) object */
  kPasswordHash = 6,     /*!< Password Hash object (hash diversified by a random seed) */
  kSecretSeed = 7,       /*!< Secret Seed object (Note: the Seed cannot have an associated crypto algorithm!) */
  kCertSignRequest = 8,  /*!< Certificate Signing Request object */
  kCertificate = 9       /*!< Certificate object (Not supported yet!) */
};

/*!
 * \brief Enumeration of all known Provider types.
 * \vpublic
 */
enum class ProviderType : std::uint32_t {
  kUndefinedProvider = 0,  /*!< Undefined/Unknown Provider type (or applicable for the whole Crypto Stack) */
  kCryptoProvider = 1,     /*!< Cryptography Provider */
  kKeyStorageProvider = 2, /*!< Key Storage Provider */
  kX509Provider = 3        /*!< X.509 Provider */
};

}  // namespace internal

/*!
 * \brief Crypto Algorithm Identifier type.
 * \vpublic
 */
using CryptoAlgId = internal::CryptoAlgId;

/*!
 * \brief A container type for bit-flags of allowed usages of a key or a secret seed object.
 * \details Only directly specified usages of a key are allowed, all other are prohibited!
 * \details Similar set of flags are defined for the usage restrictions of original key/seed
 *          and for a symmetric key or seed that potentially can be derived from the original one.
 * \details A symmetric key or secret seed can be derived from the original one, only if it supports
 *          ALLOW_KEY_AGREEMENT or ALLOW_KEY_DIVERSIFY or ALLOW_KEY_DERIVATION!
 * \vpublic
 */
using AllowedUsageFlags = internal::AllowedUsageFlags;

/*!
 * \brief Enumeration of all types of crypto objects, i.e. types of content that can be stored to a key slot.
 * \vpublic
 */
using CryptoObjectType = internal::CryptoObjectType;

/*!
 * \brief Enumeration of all known Provider types.
 * \vpublic
 */
using ProviderType = internal::ProviderType;

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_INTERNAL_BASE_ID_TYPES_WITHOUT_UUID_H_
