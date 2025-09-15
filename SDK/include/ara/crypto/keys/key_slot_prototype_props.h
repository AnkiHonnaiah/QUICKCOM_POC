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
/*!        \file  ara/crypto/keys/key_slot_prototype_props.h
 *        \brief  Key slot prototype properties definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/keys/elementary_types.h"

namespace ara {
namespace crypto {
namespace keys {

/*!
 * \brief Prototype Properties of a Key Slot.
 * \vpublic
 */
struct KeySlotPrototypeProps {
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  LogicalSlotUid mLogicalSlotUid{}; /*!< Logical Slot UID defined at Design phase of the Owner software. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  LogicalSlotUid mDependencySlotUid{}; /*!< Optional reference to Dependency Logical Slot UID defined at Design */
  /*!< phase of the Owner software. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  ActorUid mOwnerUid{}; /*!< UID of an Owner actor (application) of this key slot. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoProviderUid mCryptoProviderUid{}; /*!< UUID of a Crypto Provider assigned for this key slot processing */
  /*!< (at Integration stage). */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectUid mVersionTrack{}; /*!< COUID of last object stored to this slot */
  /*!< (versionTrack.generatorUid restricts objects' source). */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoAlgId mAlgId{ara::crypto::kAlgIdAny}; /*!< Cryptoalgorithm restriction (kAlgIdAny means without restriction). */
  /*!< Algorithm can be partially specified: family & length, mode, padding. */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  std::size_t mSlotCapacity{0}; /*!< Capacity of the slot in bytes. */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectType mObjectType{
      ara::crypto::CryptoObjectType::kNone}; /*!< Restriction of an object type that can be stored the slot */
  /*!< (if CryptoObjectType::UNKNOWN then without restriction). */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectType mDependencyType{
      ara::crypto::CryptoObjectType::kNone}; /*!< Type of object from which depends type prototyped for this slot */
  /*!< (if CryptoObjectType::NONE then without dependency). */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  VersionControlType mVersionControl{VersionControlType::kNone}; /*!< Version Control Type selects a rule restriction */
  /*!< source of stored objects. */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  bool mExportability{true}; /*!< Exportability restriction: if false then any exportable */
  /*!< object cannot be saved to this slot. */
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_
