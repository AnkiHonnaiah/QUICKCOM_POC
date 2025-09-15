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
/*!        \file  crypto/common/constants.h
 *        \brief   Defines several constants like algorithm identifiers and names of algorithms.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CONSTANTS_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CONSTANTS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/map.h"
#include "amsr/core/string.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace common {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Enum containing all Algorithm identifiers
 * \vprivate Product Private
 */
enum class AlgorithmIdentifier : ara::crypto::cryp::CryptoPrimitiveId::AlgId {
  // Hash related IDs
  SHA1 = 10000,
  SHA2_256 = 10001,
  SHA2_384 = 10002,
  SHA2_512 = 10003,
  SHA3_256 = 10031,
  SHA3_384 = 10032,
  SHA3_512 = 10033,
  HMAC_SHA_256 = 11000,
  HMAC_SHA_384 = 11003,
  HMAC_SHA_1 = 11002,
  KEY_HMAC = 11001,

  // RSA related IDs
  VER_RSA_SHA2_256_PSS_PKCS1_V2_1 = 30000,
  VER_RSA_1024_SHA2_256_PSS_PKCS1_V2_1 = 30001,
  VER_RSA_2048_SHA2_256_PSS_PKCS1_V2_1 = 30002,
  VER_RSA_3072_SHA2_256_PSS_PKCS1_V2_1 = 30003,
  VER_RSA_4096_SHA2_256_PSS_PKCS1_V2_1 = 30004,

  SIG_RSA_SHA2_256_PSS_PKCS1_V2_1 = 30100,
  SIG_RSA_1024_SHA2_256_PSS_PKCS1_V2_1 = 30101,
  SIG_RSA_2048_SHA2_256_PSS_PKCS1_V2_1 = 30102,
  SIG_RSA_3072_SHA2_256_PSS_PKCS1_V2_1 = 30103,
  SIG_RSA_4096_SHA2_256_PSS_PKCS1_V2_1 = 30104,

  RSA = 31000,  // RSA algorithm identifier is used to allow limitations for key slot contents
  RSA_1024 = 31001,
  RSA_2048 = 31002,
  RSA_3072 = 31003,
  RSA_4096 = 31004,

  // ECDSA related IDs
  VER_ECDSA_SHA2_256 = 32000,
  VER_ECDSA_SHA2_384 = 32001,
  VER_ECDSA_SHA2_512 = 32002,
  VER_ECDSA = 32003,
  SIG_ECDSA_SHA2_256 = 32100,
  SIG_ECDSA_SHA2_384 = 32101,
  SIG_ECDSA_SHA2_512 = 32102,
  SIG_ECDSA = 32103,

  // ECC related IDs
  PUBLIC_KEY_ECC_NIST_P_256 = 32005,
  PUBLIC_KEY_ECC_NIST_P_384 = 32006,
  PUBLIC_KEY_ECC_NIST_P_521 = 32007,
  PRIVATE_KEY_ECC_NIST_P_256 = 32008,
  PRIVATE_KEY_ECC_NIST_P_384 = 32009,
  PRIVATE_KEY_ECC_NIST_P_521 = 32010,
  PUBLIC_KEY_ECC_NIST_P_192 = 32011,
  PRIVATE_KEY_ECC_NIST_P_192 = 32012,
  PUBLIC_KEY_ECC_NIST_P_224 = 32013,
  PRIVATE_KEY_ECC_NIST_P_224 = 32014,

  DOM_PARAM_NIST_P_256 = 33001,
  DOM_PARAM_NIST_P_384 = 33002,
  DOM_PARAM_NIST_P_521 = 33003,

  // AES related IDs
  AES = 41000,  // AES algorithm identifier is used to allow limitations for key slot contents
  AES_128 = 41001,
  AES_192 = 41002,
  AES_256 = 41003,
  AES_336 = 41010,
  AES_656 = 41011,
  AES_768 = 41012,
  GCM_AES_128 = 41004,
  GCM_AES_192 = 41005,
  GCM_AES_256 = 41006,

  CMAC_AES_128 = 41007,

  CBC_AES_128 = 41008,
  CBC_AES_128_PKCS7 = 41009,
  CBC_AES_192 = 41015,
  CBC_AES_192_PKCS7 = 41016,
  CBC_AES_256 = 41013,
  CBC_AES_256_PKCS7 = 41014,

  // RNG related IDs
  RNG_FIPS186 = 50000,
  RNG_UNSEEDED_FIPS186 = 50001,
  RNG_SYSTEM = 50002,

  // KeyDerivation related IDs
  TLS_V1_2_PMS_TLS_V1_2_PRF_SHA2_256 = 60001,
  TLS_V1_2_PMS = 60002,
  TLS_V1_2_PRF_SHA2_256 = 60003,
  TLS_V1_2_PREMASTERSECRET = 60004,
  TLS_V1_2_MASTERSECRET = 60005,
  TLS_V1_2_PSK = 60006,
  TLS_V1_2_KEYBLOCK_2048 = 60007,
  HKDF_SHA2_256 = 600008,
  TLS_V1_2_PRF_SHA2_384 = 60009,

  // ED25519 related IDs
  VER_ED25519 = 70000,
  SIG_ED25519 = 70001,
  VER_ED25519_PH = 70002,
  SIG_ED25519_PH = 70003,
  PRIVATE_KEY_ED25519 = 70004,
  PUBLIC_KEY_ED25519 = 70005,
  // X25519 related IDs
  PRIVATE_KEY_X25519 = 90000,
  PUBLIC_KEY_X25519 = 90001,
  X25519_KEY_AGREEMENT = 90002,

  CMAC_AES_192 = 100001,
  CMAC_AES_256 = 100002,

  // CSR algorithm ids
  CSR_ECDSA_SHA2_256 = 800001,
  CSR_ECDSA_SHA2_384 = 800002,
  CSR_ECDSA_SHA2_512 = 800003,
  CSR_EDDSA_ED25519 = 800004,
};

// VECTOR NC AutosarC++17_10-A0.1.3: MD_CRYPTO_AutosarC++17_10-A0.1.3_not_called_static_function
// VECTOR NC Metric-HIS.PATH: MD_CRYPTO_Metric-HIS.PATH
/*!
 * \brief Convert given AlgId to the corresponding enumeration value.
 * \param[in] alg_id AlgId to be converted
 * \return The corresponding enumeration value
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kUnknownIdentifier  if the \c alg_id does not refer to a known algorithm with a matching
 *                                                 enumeration value
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
inline static auto ConvertAlgIdToEnum(ara::crypto::cryp::CryptoPrimitiveId::AlgId alg_id) noexcept
    -> amsr::core::Result<AlgorithmIdentifier> {
  // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
  auto result = amsr::core::Result<AlgorithmIdentifier>::FromError(ara::crypto::SecurityErrc::kUnknownIdentifier);

  // VECTOR NC AutosarC++17_10-A6.4.1: MD_CRYPTO_AutosarC++17_10-A6.4.1_fall_through
  switch (alg_id) {
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA2_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA2_512):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA3_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA3_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SHA3_512):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::HMAC_SHA_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::HMAC_SHA_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::HMAC_SHA_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::KEY_HMAC):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_RSA_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::VER_RSA_1024_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::VER_RSA_2048_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::VER_RSA_3072_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::VER_RSA_4096_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_RSA_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::SIG_RSA_1024_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::SIG_RSA_2048_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::SIG_RSA_3072_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::SIG_RSA_4096_SHA2_256_PSS_PKCS1_V2_1):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RSA):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RSA_1024):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RSA_2048):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RSA_3072):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RSA_4096):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ECDSA_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ECDSA_SHA2_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ECDSA_SHA2_512):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ECDSA):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ECDSA_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ECDSA_SHA2_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ECDSA_SHA2_512):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ECDSA):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_521):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_521):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_224):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_224):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::DOM_PARAM_NIST_P_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::DOM_PARAM_NIST_P_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::DOM_PARAM_NIST_P_521):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_128):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_336):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_656):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::AES_768):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::GCM_AES_128):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::GCM_AES_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::GCM_AES_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CMAC_AES_128):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_128):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_128_PKCS7):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_192_PKCS7):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CBC_AES_256_PKCS7):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RNG_FIPS186):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RNG_UNSEEDED_FIPS186):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::RNG_SYSTEM):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(
        AlgorithmIdentifier::TLS_V1_2_PMS_TLS_V1_2_PRF_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_PMS):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_PRF_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_PRF_SHA2_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_PREMASTERSECRET):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_MASTERSECRET):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_PSK):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::TLS_V1_2_KEYBLOCK_2048):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::HKDF_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ED25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ED25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::VER_ED25519_PH):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::SIG_ED25519_PH):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_ED25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_ED25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CSR_ECDSA_SHA2_256):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CSR_ECDSA_SHA2_384):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CSR_ECDSA_SHA2_512):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CSR_EDDSA_ED25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PRIVATE_KEY_X25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::PUBLIC_KEY_X25519):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::X25519_KEY_AGREEMENT):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CMAC_AES_192):
    case static_cast<ara::crypto::cryp::CryptoPrimitiveId::AlgId>(AlgorithmIdentifier::CMAC_AES_256):
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
      result.EmplaceValue(static_cast<AlgorithmIdentifier>(alg_id));
      break;
    default:
      // result has been initialized with the default already
      break;
  }

  return result;
}

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*!
 * \brief Static mapping between name and algorithm identifier
 * \vprivate Product Private
 */
static amsr::core::Map<amsr::core::String, AlgorithmIdentifier> const kAlgorithmNameMapping{
    {"VER/ECDSA,SHA2-256", AlgorithmIdentifier::VER_ECDSA_SHA2_256}, /*!< Mapping for ECDSA with SHA2-256 */
    {"VER/ECDSA,SHA2-384", AlgorithmIdentifier::VER_ECDSA_SHA2_384}, /*!< Mapping for ECDSA with SHA2-384 */
    {"VER/ECDSA,SHA2-512", AlgorithmIdentifier::VER_ECDSA_SHA2_512}, /*!< Mapping for ECDSA with SHA2-512 */
    {"VER/ECDSA", AlgorithmIdentifier::VER_ECDSA},                   /*!< Mapping for ECDSA without pr hashing */
    {"SIG/ECDSA,SHA2-256", AlgorithmIdentifier::SIG_ECDSA_SHA2_256}, /*!< Mapping for ECDSA with SHA2-256 */
    {"SIG/ECDSA,SHA2-384", AlgorithmIdentifier::SIG_ECDSA_SHA2_384}, /*!< Mapping for ECDSA with SHA2-384 */
    {"SIG/ECDSA,SHA2-512", AlgorithmIdentifier::SIG_ECDSA_SHA2_512}, /*!< Mapping for ECDSA with SHA2-512 */
    {"SIG/ECDSA", AlgorithmIdentifier::SIG_ECDSA},                   /*!< Mapping for ECDSA without pr hashing */
    {"VER/RSA-1024,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::VER_RSA_1024_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS verification with key
                                                                    length */
    {"VER/RSA-2048,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::VER_RSA_2048_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS verification with key
                                                                    length */
    {"VER/RSA-3072,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::VER_RSA_3072_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS verification with key
                                                                    length */
    {"VER/RSA-4096,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::VER_RSA_4096_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS verification with key
                                                                    length */
    {"VER/RSA,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::VER_RSA_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS verification */
    {"SIG/RSA,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::SIG_RSA_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS signing */
    {"SIG/RSA-1024,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::SIG_RSA_1024_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS signing with key length */
    {"SIG/RSA-2048,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::SIG_RSA_2048_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS signing with key length */
    {"SIG/RSA-3072,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::SIG_RSA_3072_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS signing with key length */
    {"SIG/RSA-4096,SHA2-256/PSS,PKCS1.V2_1",
     AlgorithmIdentifier::SIG_RSA_4096_SHA2_256_PSS_PKCS1_V2_1}, /*!< Mapping for RSASSA-PSS signing with key length */

    {"CSR/ECDSA,SHA2-256", AlgorithmIdentifier::CSR_ECDSA_SHA2_256}, /*!< Mapping for Certificate signing requests */
    {"CSR/ECDSA,SHA2-384", AlgorithmIdentifier::CSR_ECDSA_SHA2_384}, /*!< Mapping for Certificate signing requests */
    {"CSR/ECDSA,SHA2-512", AlgorithmIdentifier::CSR_ECDSA_SHA2_512}, /*!< Mapping for Certificate signing requests */
    {"CSR/ED25519", AlgorithmIdentifier::CSR_EDDSA_ED25519},         /*!< Mapping for Certificate signing requests */
    {"RNG/UNSEEDED,FIPS186", AlgorithmIdentifier::RNG_UNSEEDED_FIPS186}, /*!< Mapping for RNG */
    {"RNG,FIPS186", AlgorithmIdentifier::RNG_FIPS186},                   /*!< Mapping for RNG */
    {"RNG,SYSTEM", AlgorithmIdentifier::RNG_SYSTEM},                     /*!< Mapping for RNG */
    {"SHA1", AlgorithmIdentifier::SHA1},                                 /*!< Mapping for SHA-1 */
    {"SHA2-256", AlgorithmIdentifier::SHA2_256},                         /*!< Mapping for SHA2-256 */
    {"SHA2-384", AlgorithmIdentifier::SHA2_384},                         /*!< Mapping for SHA2-384 */
    {"SHA2-512", AlgorithmIdentifier::SHA2_512},                         /*!< Mapping for SHA2-512 */
    {"SHA3-256", AlgorithmIdentifier::SHA3_256},                         /*!< Mapping for SHA3-256 */
    {"SHA3-384", AlgorithmIdentifier::SHA3_384},                         /*!< Mapping for SHA3-384 */
    {"SHA3-512", AlgorithmIdentifier::SHA3_512},                         /*!< Mapping for SHA3-512 */
    {"HMAC/SHA-256", AlgorithmIdentifier::HMAC_SHA_256},                 /*!< Mapping for HMAC using SHA-256 */
    {"HMAC/SHA-384", AlgorithmIdentifier::HMAC_SHA_384},                 /*!< Mapping for HMAC using SHA-384 */
    {"HMAC/SHA-1", AlgorithmIdentifier::HMAC_SHA_1},                     /*!< Mapping for HMAC using SHA-1 */
    {"KEY_HMAC", AlgorithmIdentifier::KEY_HMAC},                         /*!< Mapping for an HMAC key */
    {"RSA", AlgorithmIdentifier::RSA},                                   /*!< Mapping for RSA algorithm */
    {"RSA-1024", AlgorithmIdentifier::RSA_1024},                         /*!< Mapping for RSA with key length */
    {"RSA-2048", AlgorithmIdentifier::RSA_2048},                         /*!< Mapping for RSA with key length */
    {"RSA-3072", AlgorithmIdentifier::RSA_3072},                         /*!< Mapping for RSA with key length */
    {"RSA-4096", AlgorithmIdentifier::RSA_4096},                         /*!< Mapping for RSA with key length */
    {"PUBLIC_KEY_ECC/NIST_P-256", AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_256},   /*! Mapping for ECC public key */
    {"PUBLIC_KEY_ECC/NIST_P-384", AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_384},   /*! Mapping for ECC public key */
    {"PUBLIC_KEY_ECC/NIST_P-521", AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_521},   /*! Mapping for ECC public key */
    {"PRIVATE_KEY_ECC/NIST_P-256", AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_256}, /*! Mapping for ECC private key */
    {"PRIVATE_KEY_ECC/NIST_P-384", AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_384}, /*! Mapping for ECC private key */
    {"PRIVATE_KEY_ECC/NIST_P-521", AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_521}, /*! Mapping for ECC private key */
    {"PUBLIC_KEY_ECC/NIST_P-192", AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_192},   /*! Mapping for ECC public key */
    {"PRIVATE_KEY_ECC/NIST_P-192", AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_192}, /*! Mapping for ECC private key */
    {"PUBLIC_KEY_ECC/NIST_P-224", AlgorithmIdentifier::PUBLIC_KEY_ECC_NIST_P_224},   /*! Mapping for ECC public key */
    {"PRIVATE_KEY_ECC/NIST_P-224", AlgorithmIdentifier::PRIVATE_KEY_ECC_NIST_P_224}, /*! Mapping for ECC private key */
    {"NIST_P-256", AlgorithmIdentifier::DOM_PARAM_NIST_P_256},          /*! Mapping for ECC curve Domain Parameters */
    {"PRIME256V1", AlgorithmIdentifier::DOM_PARAM_NIST_P_256},          /*! Mapping for ECC curve Domain Parameters */
    {"1.2.840.10045.3.1.7", AlgorithmIdentifier::DOM_PARAM_NIST_P_256}, /*! Mapping for ECC curve Domain Parameters */
    {"NIST_P-384", AlgorithmIdentifier::DOM_PARAM_NIST_P_384},          /*! Mapping for ECC curve Domain Parameters */
    {"SECP384R1", AlgorithmIdentifier::DOM_PARAM_NIST_P_384},           /*! Mapping for ECC curve Domain Parameters */
    {"1.3.132.0.34", AlgorithmIdentifier::DOM_PARAM_NIST_P_384},        /*! Mapping for ECC curve Domain Parameters */
    {"NIST_P-521", AlgorithmIdentifier::DOM_PARAM_NIST_P_521},          /*! Mapping for ECC curve Domain Parameters */
    {"SECP521R1", AlgorithmIdentifier::DOM_PARAM_NIST_P_521},           /*! Mapping for ECC curve Domain Parameters */
    {"1.3.132.0.35", AlgorithmIdentifier::DOM_PARAM_NIST_P_521},        /*! Mapping for ECC curve Domain Parameters */
    {"AES", AlgorithmIdentifier::AES},                                  /*!< Mapping for AES algorithm */
    {"AES-128", AlgorithmIdentifier::AES_128},                          /*!< Mapping for AES with key length 128 */
    {"AES-192", AlgorithmIdentifier::AES_192},                          /*!< Mapping for AES with key length 192 */
    {"AES-256", AlgorithmIdentifier::AES_256},                          /*!< Mapping for AES with key length 256 */
    {"AES-336", AlgorithmIdentifier::AES_336},                          /*!< Mapping for AES with key length 336 */
    {"AES-656", AlgorithmIdentifier::AES_656},                          /*!< Mapping for AES with key length 656 */
    {"AES-768", AlgorithmIdentifier::AES_768},                          /*!< Mapping for AES with key length 768 */
    {"GCM/AES-128", AlgorithmIdentifier::GCM_AES_128},                  /*!< Mapping for AES GCM key length 128 */
    {"GCM/AES-192", AlgorithmIdentifier::GCM_AES_192},                  /*!< Mapping for AES GCM key length 192 */
    {"GCM/AES-256", AlgorithmIdentifier::GCM_AES_256},                  /*!< Mapping for AES GCM key length 256 */
    {"CMAC/AES-128", AlgorithmIdentifier::CMAC_AES_128},                /*!< Mapping for CMAC using AES-128 */
    {"CMAC/AES-192", AlgorithmIdentifier::CMAC_AES_192},                /*!< Mapping for CMAC using AES-128 */
    {"CMAC/AES-256", AlgorithmIdentifier::CMAC_AES_256},                /*!< Mapping for CMAC using AES-128 */
    {"CBC/AES-128", AlgorithmIdentifier::CBC_AES_128},                  /*!< Mapping for AES-128-CBC */
    {"CBC/AES-192", AlgorithmIdentifier::CBC_AES_192},                  /*!< Mapping for AES-192-CBC */
    {"CBC/AES-256", AlgorithmIdentifier::CBC_AES_256},                  /*!< Mapping for AES-256-CBC */
    {"CBC/AES-128-PKCS7", AlgorithmIdentifier::CBC_AES_128_PKCS7},      /*!< Mapping for AES-128-CBC-PKCS7 */
    {"CBC/AES-192-PKCS7", AlgorithmIdentifier::CBC_AES_192_PKCS7},      /*!< Mapping for AES-192-CBC-PKCS7 */
    {"CBC/AES-256-PKCS7", AlgorithmIdentifier::CBC_AES_256_PKCS7},      /*!< Mapping for AES-256-CBC-PKCS7 */
    {"TLS.V1_2_PRF/SHA2-256", AlgorithmIdentifier::TLS_V1_2_PRF_SHA2_256}, /*!< Mapping for TLS v1.2 SHA2-256 PRF */
    {"TLS.V1_2_PRF/SHA2-384", AlgorithmIdentifier::TLS_V1_2_PRF_SHA2_384}, /*!< Mapping for TLS v1.2 SHA2-384 PRF */
    {"TLS.V1_2_PMS+TLS.V1_2_PRF/SHA2-256",
     AlgorithmIdentifier::TLS_V1_2_PMS_TLS_V1_2_PRF_SHA2_256},             /*!< Mapping for TLS v1.2 SHA2-256 PMS PRF */
    {"TLS.V1_2_PMS", AlgorithmIdentifier::TLS_V1_2_PMS},                   /*!< Mapping for TLS v1.2 SHA2-256 PMS */
    {"TLS.V1_2_MASTERSECRET", AlgorithmIdentifier::TLS_V1_2_MASTERSECRET}, /*!< Mapping for TLS v1.2 master secret */
    {"TLS.V1_2_PREMASTERSECRET",
     AlgorithmIdentifier::TLS_V1_2_PREMASTERSECRET},     /*!< Mapping for TLS v1.2 pre-master secret */
    {"TLS.V1_2_PSK", AlgorithmIdentifier::TLS_V1_2_PSK}, /*!< Mapping for TLS v1.2 PSK */
    {"TLS.V1_2_KEYBLOCK-2048",
     AlgorithmIdentifier::TLS_V1_2_KEYBLOCK_2048},          /*!< Mapping for 2048-bit TLS v1.2 keyblock */
    {"HKDF/SHA2_256", AlgorithmIdentifier::HKDF_SHA2_256},  /*!< Mapping for HKDF SHA2-256 */
    {"VER/ED25519", AlgorithmIdentifier::VER_ED25519},      /*!< Mapping for ED25519 signature verification */
    {"SIG/ED25519", AlgorithmIdentifier::SIG_ED25519},      /*!< Mapping for ED25519 signature generation */
    {"VER/ED25519PH", AlgorithmIdentifier::VER_ED25519_PH}, /*!< Mapping for ED25519PH signature verification */
    {"SIG/ED25519PH", AlgorithmIdentifier::SIG_ED25519_PH}, /*!< Mapping for ED25519PH signature generation */
    {"PRIVATE_KEY/ED25519", AlgorithmIdentifier::PRIVATE_KEY_ED25519},  /*!< Mapping for ED25519 private key */
    {"PUBLIC_KEY/ED25519", AlgorithmIdentifier::PUBLIC_KEY_ED25519},    /*!< Mapping for ED25519 public key */
    {"PRIVATE_KEY/X25519", AlgorithmIdentifier::PRIVATE_KEY_X25519},    /*!< Mapping for X25519 private key */
    {"PUBLIC_KEY/X25519", AlgorithmIdentifier::PUBLIC_KEY_X25519},      /*!< Mapping for X25519 private key */
    {"KEY_AGREEMENT/X25519", AlgorithmIdentifier::X25519_KEY_AGREEMENT} /*!< Mapping for X25519 private key */
};

/*!
 * \brief Constexpr function that mapps the algorithm ID to a human readable string
 * \tparam alg_id the algorithm Identifier
 * \return Human readable algorithm name.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
template <common::AlgorithmIdentifier alg_id>
static constexpr auto GetAlgName() noexcept -> amsr::core::StringView {
  auto it =
      std::find_if(kAlgorithmNameMapping.begin(), kAlgorithmNameMapping.end(),
                   [](std::pair<amsr::core::String, AlgorithmIdentifier> const& p) { return p.second == alg_id; });
  amsr::core::StringView ret{""};
  if (it != kAlgorithmNameMapping.end()) {
    ret = it->first;
  }
  return ret;
}
// VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
static constexpr amsr::core::StringView const kDefaultKspConfigPath{
    "./etc/ksp.json"_sv}; /*!< Default path for the ksp.json */

// VECTOR NC Compiler-#28: MD_CRYPTO_Compiler-#28_constexpr
/*! Vector crypto provider UUID (keyless, in process operations) */
static constexpr ara::crypto::Uuid const kLocalCryptoProviderUuid{
    // VECTOR NL Compiler-#2706: MD_CRYPTO_Compiler-#2706_constexpr
    ara::crypto::Uuid::From("11111111-1111-1111-1111-111111111111").Value()};

// VECTOR NC Compiler-#28: MD_CRYPTO_Compiler-#28_constexpr
/*! Vector vendor UUID */
static constexpr ara::crypto::Uuid const kVectorUuid{
    // VECTOR NL Compiler-#2706: MD_CRYPTO_Compiler-#2706_constexpr
    ara::crypto::Uuid::From("d8bf7455-d995-41ac-86f3-bdfaf4f7f038").Value()};

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CONSTANTS_H_
