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
/*!        \file  ara/crypto/common/base_id_types.h
 *        \brief  Definition of base ID types.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/internal/base_id_types_without_uuid.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {

/*!
 * \brief Unique ID of the Actor process (it is a persistent UID defined on the application design phase).
 * \details Actor UID can be associated with "User" or "Owner" process permissions.
 * \vpublic
 */
using ActorUid = Uuid;

/*!
 * \brief Unique ID of a Crypto Provider (it is a persistent UID defined on the provider design phase).
 * \vpublic
 */
using CryptoProviderUid = Uuid;

// Attention! Deviation: Defined as constexpr instead of const to satisfy AutosarC++17_10-A3.3.2
CryptoProviderUid constexpr kAnyCryptoProvider{
    Uuid()}; /*!< Nil UID is reserved for specification of "any" (or default) Crypto Provider. */

/*!
 * \brief Logical Slot UID (it is a persistent UID defined on the application design phase).
 * \vpublic
 */
using LogicalSlotUid = Uuid;

CryptoAlgId const kAlgIdUndefined{0ULL};      /*!< Algorithm ID is undefined. */
CryptoAlgId const kAlgIdAny{kAlgIdUndefined}; /*!< Any Algorithm ID is allowed. */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
CryptoAlgId const kAlgIdDefault{kAlgIdUndefined}; /*!< Default Algorithm ID (in current context/primitive). */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
CryptoAlgId const kAlgIdNone{kAlgIdUndefined}; /*!< None of Algorithm ID */
                                               /*!< (i.e. an algorithm definition is not applicable). */

AllowedUsageFlags const kAllowPrototypedOnly{
    static_cast<AllowedUsageFlags>(0L)}; /*!< The key/seed usage will be fully specified by a key slot */
                                         /*!< prototype (the object can be used only after reloading */
                                         /*!< from the slot). */

AllowedUsageFlags const kAllowDataEncryption{
    static_cast<AllowedUsageFlags>(0x0001L)}; /*!< The key/seed can be used for data encryption */
                                              /*!< initialization (applicable to symmetric and */
                                              /*!< asymmetric keys). */
AllowedUsageFlags const kAllowDataDecryption{
    static_cast<AllowedUsageFlags>(0x0002L)}; /*!< The key/seed can be used for data decryption */
                                              /*!< initialization (applicable to symmetric and */
                                              /*!< asymmetric keys). */
AllowedUsageFlags const kAllowSignature{
    static_cast<AllowedUsageFlags>(0x0004L)}; /*!< The key/seed can be used for digital signature */
                                              /*!< or MAC/HMAC production (applicable to symmetric and */
                                              /*!< asymmetric keys). */
AllowedUsageFlags const kAllowVerification{
    static_cast<AllowedUsageFlags>(0x0008L)}; /*!< The key/seed can be used for digital signature */
                                              /*!< or MAC/HMAC verification (applicable to symmetric and */
                                              /*!< asymmetric keys). */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
AllowedUsageFlags const kAllowKeyAgreement{
    static_cast<AllowedUsageFlags>(0x0010L)}; /*!< The seed or asymmetric key can be used for key-agreement */
                                              /*!< protocol execution. */
AllowedUsageFlags const kAllowKeyDiversify{
    static_cast<AllowedUsageFlags>(0x0020L)}; /*!< The seed or symmetric key can be used */
                                              /*!< for slave-keys diversification. */
AllowedUsageFlags const kAllowDrngInit{
    static_cast<AllowedUsageFlags>(0x0040L)}; /*!< The seed or symmetric key can be used */
                                              /*!< for initialization of a Deterministic */
                                              /*!< Random Number Generators (DRNG). */
AllowedUsageFlags const kAllowKdfMaterial{
    static_cast<AllowedUsageFlags>(0x0080L)}; /*!< The seed or symmetric key can be used */
                                              /*!< as a KeyMaterial for slave-keys derivation */
                                              /*!< via a Key Derivation Function (KDF). */
AllowedUsageFlags const kAllowKeyExporting{
    static_cast<AllowedUsageFlags>(0x0100L)}; /*!< The key can be used as "transport" one */
                                              /*!< for Key-Wrap or Encapsulate transformations */
                                              /*!< (applicable to symmetric and asymmetric keys). */
AllowedUsageFlags const kAllowKeyImporting{
    static_cast<AllowedUsageFlags>(0x0200L)}; /*!< The key can be used as "transport" one */
                                              /*!< for Key-Unwrap or Decapsulate transformations */
                                              /*!< (applicable to symmetric and asymmetric keys). */
AllowedUsageFlags const kAllowExactModeOnly{
    static_cast<AllowedUsageFlags>(0x8000L)}; /*!< The key can be used only for the mode directly */
                                              /*!< specified by Key::AlgId. */

AllowedUsageFlags const kAllowDerivedDataEncryption{kAllowDataEncryption << 16}; /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< for data encryption. */
AllowedUsageFlags const kAllowDerivedDataDecryption{kAllowDataDecryption << 16}; /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< for data decryption. */
AllowedUsageFlags const kAllowDerivedSignature{kAllowSignature << 16};           /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< for MAC/HMAC production. */
AllowedUsageFlags const kAllowDerivedVerification{kAllowVerification << 16};     /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< for MAC/HMAC verification. */
AllowedUsageFlags const kAllowDerivedKeyDiversify{kAllowKeyDiversify << 16};     /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used for */
                                                                                 /*!< slave-keys diversification. */
AllowedUsageFlags const kAllowDerivedDrngInit{kAllowDrngInit << 16};             /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< for initialization of a */
                                                                                 /*!< Deterministic Random Number */
                                                                                 /*!< Generators (DRNG). */
AllowedUsageFlags const kAllowDerivedKdfMaterial{kAllowKdfMaterial << 16};       /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< as a KeyMaterial for */
                                                                                 /*!< slave-keys derivation */
                                                                                 /*!< via a Key Derivation */
                                                                                 /*!< Function (KDF). */
AllowedUsageFlags const kAllowDerivedKeyExporting{kAllowKeyExporting << 16};     /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< as a "transport" one for */
                                                                                 /*!< Key-Wrap transformation. */
AllowedUsageFlags const kAllowDerivedKeyImporting{kAllowKeyImporting << 16};     /*!< A derived seed or */
                                                                                 /*!< symmetric key can be used */
                                                                                 /*!< as a "transport" one for */
                                                                                 /*!< Key-Unwrap transformation. */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
AllowedUsageFlags const kAllowDerivedExactModeOnly{kAllowExactModeOnly << 16}; /*!< A derived seed or */
                                                                               /*!< symmetric key can be used */
                                                                               /*!< only for the mode directly */
                                                                               /*!< specified by Key::AlgId. */

/*!
 * \brief The seed or symmetric key can be used as a KeyMaterial for a Key Derivation Function (KDF)
 *        and the derived "slave" keys can be used without limitations.
 * \vpublic
 */
AllowedUsageFlags const kAllowKdfMaterialAnyUsage{
    kAllowKdfMaterial | kAllowDerivedDataEncryption | kAllowDerivedDataDecryption | kAllowDerivedSignature |
    kAllowDerivedVerification | kAllowDerivedKeyDiversify | kAllowDerivedDrngInit | kAllowDerivedKdfMaterial |
    kAllowDerivedKeyExporting | kAllowDerivedKeyImporting};

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_
