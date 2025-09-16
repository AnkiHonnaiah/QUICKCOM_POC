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
/*!        \file  json_keys.h
 *        \brief  Constants used in JSON parsing.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_KEYS_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_KEYS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace crypto {
namespace common {
namespace data {

/*!
 * \brief Typedef to be used for C-style strings in order to satisfy coding guidelines.
 * \vprivate Product Private
 */
using CStringType = char const*;

// VECTOR Disable AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
static constexpr CStringType const kType{"type"};                     /*!< Constant key for JSON parsing */
static constexpr CStringType const kUuid{"uuid"};                     /*!< Constant key for JSON parsing */
static constexpr CStringType const kCouid{"couid"};                   /*!< Constant key for JSON parsing */
static constexpr CStringType const kCouidGenerator{"generator"};      /*!< Constant key for JSON parsing */
static constexpr CStringType const kCouidGeneratorUuid{kUuid};        /*!< Constant key for JSON parsing */
static constexpr CStringType const kCouidVersion{"version"};          /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlots{"slots"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlotNumber{"number"};       /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlotUuid{kUuid};            /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlotProvider{"provider"};   /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlotProviderUuid{kUuid};    /*!< Constant key for JSON parsing */
static constexpr CStringType const kConfigSlotPath{"path"};           /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMeta{"meta"};                 /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaUuid{kUuid};              /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaType{kType};              /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaProvider{"provider"};     /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaProviderUuid{kUuid};      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaDependency{"dependency"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaDependencyUuid{kUuid};    /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaDependencyType{kType};    /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaContentRestriction{
    "contentRestriction"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaContentRestrictionIsExportable{
    "isExportable"};                                                          /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaContentRestrictionAlgId{"algId"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaContentRestrictionType{kType};    /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaCapacity{"capacity"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaVersionControl{"versionControl"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaVersionControlType{kType};        /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaVersionControlPrevContent{
    "prevContent"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaVersionControlPrevContentCouid{
    kCouid};                                                                      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaVersionControlPrevContentType{kType}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaOwner{"owner"};                       /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaOwnerUuid{kUuid};                     /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaUser{"user"};                         /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotMetaUserUuid{kUuid};                      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContent{"content"};                       /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMeta{"meta"};                      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaCouid{kCouid};                 /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaDependency{"dependency"};      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaDependencyCouid{kCouid};       /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaAllowedUsage{"allowedUsage"};  /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaType{kType};                   /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaAlgId{
    kSlotMetaContentRestrictionAlgId};                                      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentMetaBitSize{"bitSize"};      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentPayload{"content"};          /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotCustomPayload{"payload"};           /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentRsaModulus{"modulus"};       /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentRsaPublicExponent{"e"};      /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentRsaPrivateExponent{"d"};     /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentSymmetricKeyData{"keyData"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentEccPublicX{"x"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentEccPublicY{"y"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentEccPrivate{"d"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kSeed{"seed"};                           /*!< Constant key for JSON parsing */
static constexpr CStringType const kSaltLength{"saltLength"};               /*!< Constant key for JSON parsing */
static constexpr CStringType const kSessionFlag{"sessionFlag"};             /*!< Constant key for JSON parsing */
static constexpr CStringType const kLogicalSlotUid{"logicalSlotUid"};       /*!< Constant key for JSON parsing */
static constexpr CStringType const kCryptoProviderUid{"cryptoProviderUid"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kPath{"path"};                           /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlots{"slots"};                         /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotNumber{"number"};                   /*!< Constant key for JSON parsing */
static constexpr CStringType const kActorUid{"actorUid"};                   /*!< Constant key for JSON parsing */
static constexpr CStringType const kEncodedDomainParameters{
    "encodedDomainParams"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kEncodedDomainParametersExt{
    "encodedDomainParamsExt"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kEncodedDomainParametersSpeedupExt{
    "encodedDomainParamsSpeedupExt"};                                      /*!< Constant key for JSON parsing */
static constexpr CStringType const kAccessControlConfigUserIds{"userIDs"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kAccessControlConfigUserId{"userID"};   /*!< Constant key for JSON parsing */
static constexpr CStringType const kAccessControlConfigRestrictions{
    "restrictions"};                                                             /*!< Constant key for JSON parsing */
static constexpr CStringType const kAccessControlConfigSlotNumber{"slotNumber"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kAccessControlConfigOperation{"operation"};   /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigServer{"Server"};                /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigServerMaxConnectionNum{
    "maxConnectionNum"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigServerKeyAccessControl{
    "keyAccessControl"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigServerIdsmReporting{
    "idsmReporting"};                                                    /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509{"X.509"};           /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509Storage{"storage"};  /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509StorageRoot{"root"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509Access{"access"};    /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509AccessCaConnectorId{
    "caConnectorId"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kDaemonConfigX509AccessTrustmasterId{
    "trustmasterId"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentEd25519PrivateKey{
    "ed25519PrivateKey"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentEd25519PublicKey{
    "ed25519PublicKey"}; /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentX25519PrivateKey{
    "x25519PrivateKey"};                                                           /*!< Constant key for JSON parsing */
static constexpr CStringType const kSlotContentX25519PublicKey{"x25519PublicKey"}; /*!< Constant key for JSON parsing */
// VECTOR Enable AutosarC++17_10-M3.4.1

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_KEYS_H_
