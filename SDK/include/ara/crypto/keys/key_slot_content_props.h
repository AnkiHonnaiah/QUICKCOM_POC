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
/*!        \file  ara/crypto/keys/key_slot_content_props.h
 *        \brief  Key slot content properties definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_

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
 * \brief Properties of current Key Slot Content, i.e. of a current instance stored to the Key Slot.
 * \details A value of the allowed_usage field is bitwise AND of the common usage flags defined at run-time
 *          and the usage flags defined by the UserPermissions prototype for current Actor (application).
 * \vpublic
 */
struct KeySlotContentProps {
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectUid mObjectUid{}; /*!< UID of a Crypto Object stored to the slot. */
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectUid mDependencyUid{}; /*!< UID of a Crypto Object from which depends object stored to the slot. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoAlgId mAlgId{}; /*!< Cryptoalgorithm of actual object stored to slot. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  std::size_t mObjectSize{}; /*!< Actual size of an object currently stored to the slot. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  AllowedUsageFlags mAllowedUsage{}; /*!< Actual usage restriction flags of an object stored */
                                     /*!< to the slot for the current application. */
  // Attention! Deviation: Default value added.
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  SlotNumber mDependencySlot{kInvalidSlot}; /*!< Optional number of a slot where the dependency object is stored, if */
                                            /*!< mDependencyUid is not nil. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  std::uint32_t mReferencesCounter{
      0}; /*!< Number of references to this object instance from other slots in the Key Storage. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  CryptoObjectType mObjectType{CryptoObjectType::kNone}; /*!< Actual type of an object stored to the slot. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  bool mExportability{}; /*!< Effective value of the exportability attribute of current object. */
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_
