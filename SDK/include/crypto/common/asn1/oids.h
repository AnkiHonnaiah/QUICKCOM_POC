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
/*!        \file  oids.h
 *        \brief  Definitions of known OIDs.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_ASN1_OIDS_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_ASN1_OIDS_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"

#include "vac/container/string_literals.h"

namespace crypto {
namespace common {
namespace asn1 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator""_sv;

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN CommonName attribute */
static constexpr amsr::core::StringView const kCommonNameOid{"2.5.4.3"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN CountryName attribute */
static constexpr amsr::core::StringView const kCountryNameOid{"2.5.4.6"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN StateOrProvinceName attribute */
static constexpr amsr::core::StringView const kStateNameOid{"2.5.4.8"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN LocalityName attribute */
static constexpr amsr::core::StringView const kLocalityNameOid{"2.5.4.7"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN OrganizationName attribute */
static constexpr amsr::core::StringView const kOrgNameOid{"2.5.4.10"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN OrganizationUnitName attribute */
static constexpr amsr::core::StringView const kOrgUnitNameOid{"2.5.4.11"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN StreetAddress attribute */
static constexpr amsr::core::StringView const kStreetAddressOid{"2.5.4.39"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN PostalCode attribute */
static constexpr amsr::core::StringView const kPostalCodeOid{"2.5.4.17"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN Title attribute */
static constexpr amsr::core::StringView const kTitleOid{"2.5.4.12"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN Surname attribute */
static constexpr amsr::core::StringView const kSurnameOid{"2.5.4.4"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN OrganizationName attribute */
static constexpr amsr::core::StringView const kGivenNameOid{"2.5.4.42"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN Initials attribute */
static constexpr amsr::core::StringView const kInitialsOid{"2.5.4.43"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN Pseudonym attribute */
static constexpr amsr::core::StringView const kPseudonymOid{"2.5.4.65"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN GenerationQualifier attribute */
static constexpr amsr::core::StringView const kGenerationQualifierOid{"2.5.4.44"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN DomainComponent attribute */
static constexpr amsr::core::StringView const kDomainComponentOid{"0.9.2342.19200300.100.1.25"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN dnQualifier attribute */
static constexpr amsr::core::StringView const kDnQualifierOid{"2.5.4.46"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN EmailAddress attribute */
static constexpr amsr::core::StringView const kEmailAddressOid{"1.2.840.113549.1.9.1"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN uniqueIdentifier attribute */
static constexpr amsr::core::StringView const kUniqueIdentifierOid{"0.9.2342.1920000.100.1.44"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN dnsName attribute */
static constexpr amsr::core::StringView const kDnsNameOid{"2.5.4.100"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN unstructuredName attribute */
static constexpr amsr::core::StringView const kUnstructuredNameOid{"1.2.840.113549.1.9.2"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN unstructuredAddress attribute */
static constexpr amsr::core::StringView const kUnstructuredAddressOid{"1.2.840.113549.1.9.8"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN serialNumber attribute */
static constexpr amsr::core::StringView const kSerialNumberOid{"2.5.4.5"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN userId attribute */
static constexpr amsr::core::StringView const kUserIdOid{"0.9.2342.19200300.100.1.1"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! ObjectIdentifier for the DN houseIdentifier attribute */
static constexpr amsr::core::StringView const kHouseIdentifierOid{"2.5.4.51"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! EC public key object identifier */
static constexpr amsr::core::StringView const kEcPublicKeyOid{"1.2.840.10045.2.1"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Diffie Hellman curve X25519 object identifier */
static constexpr amsr::core::StringView const kX25519Oid{"1.3.101.110"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Edwards-curve Digital Signature Algorithm (EdDSA) Ed25519 object identifier */
static constexpr amsr::core::StringView const kEd25519Oid{"1.3.101.112"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Edwards-curve Digital Signature Algorithm (EdDSA) Ed25519Ph object identifier, pre-hashed variant */
static constexpr amsr::core::StringView const kEd25519PhOid{"1.3.6.1.4.1.2916.3.6.509.5.110"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Edwards-curve Digital Signature Algorithm (EdDSA) Ed25519Ph object identifier, pre-hashed variant */
static constexpr amsr::core::StringView const kEd25519PhDraftOid{"1.3.101.114"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Elliptic curve domain "secp192r1" object identifier (aka prime192v1) */
static constexpr amsr::core::StringView const kSecp192r1Oid{"1.2.840.10045.3.1.1"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Elliptic curve domain "secp256r1" object identifier (aka prime256v1, NIST P-256) */
static constexpr amsr::core::StringView const kSecp256r1Oid{"1.2.840.10045.3.1.7"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Elliptic curve domain "secp384r1" object identifier (aka ansip384r1, NIST P-384) */
static constexpr amsr::core::StringView const kSecp384r1Oid{"1.3.132.0.34"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Elliptic curve domain "secp521r1" object identifier (aka ansip521r1, NIST P-512) */
static constexpr amsr::core::StringView const kSecp521r1Oid{"1.3.132.0.35"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Signature algorithm (ANSI X9.62 ECDSA algorithm with SHA256) */
static constexpr amsr::core::StringView const kEcdsaWithSha256Oid{"1.2.840.10045.4.3.2"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Signature algorithm (ANSI X9.62 ECDSA algorithm with SHA384) */
static constexpr amsr::core::StringView const kEcdsaWithSha384Oid{"1.2.840.10045.4.3.3"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Signature algorithm (ANSI X9.62 ECDSA algorithm with SHA512) */
static constexpr amsr::core::StringView const kEcdsaWithSha512Oid{"1.2.840.10045.4.3.4"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Authority key identifier certificate extension */
static constexpr amsr::core::StringView const kAuthorityKeyIdentifierOid{"2.5.29.35"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Subject key identifier certificate extension */
static constexpr amsr::core::StringView const kSubjectKeyIdentifierOid{"2.5.29.14"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Key usage certificate extension */
static constexpr amsr::core::StringView const kKeyUsageOid{"2.5.29.15"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! Basic constraints certificate extension */
static constexpr amsr::core::StringView const kBasicConstraintsOid{"2.5.29.19"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! SHA-1 hash function */
static constexpr amsr::core::StringView const kSha1Oid{"1.3.14.3.2.26"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! SHA-256 hash function */
static constexpr amsr::core::StringView const kSha256Oid{"2.16.840.1.101.3.4.2.1"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! SHA-384 hash function */
static constexpr amsr::core::StringView const kSha384Oid{"2.16.840.1.101.3.4.2.2"_sv};

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static_codedesign
/*! SHA-512 hash function */
static constexpr amsr::core::StringView const kSha512Oid{"2.16.840.1.101.3.4.2.3"_sv};

}  // namespace asn1
}  // namespace common
}  // namespace crypto

#endif  //  LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_ASN1_OIDS_H_
