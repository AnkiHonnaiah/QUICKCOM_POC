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
/*!        \file  ara/crypto/keys/user_permissions.h
 *        \brief  User permissions definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_USER_PERMISSIONS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_USER_PERMISSIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace keys {

/*!
 * \brief Key slot User's Permissions prototype defined at the Design (or the Integration) stage.
 * \note "Actor" is a permanently identifiable process defined by the Startup Configuration of an Executable.
 * \note Access control management is based on the key slot attributes:
 *   (1) Each persistent slot always has only one "Owner Actor" that can save an object to the slot, clear it or
 *       copy its content to another owned slot. "Owner" is responsible for consistency the slot content.
 *       Only "Owner Actor" can execute export operation of a crypto object (if it is allowed by the object
 *       attributes). Owner's prototype defines the whole set of the allowed usage flags for owned key slot.
 *   (2) A "User" access right for a slot can be granted to an "Actor" by the Owner's manifest.
 *       The "User" access means the right to load a crypto object from the slot to a Crypto Provider's realm via
 *       the trusted container interface. Usage permissions of each "User" may be restricted independently from other.
 *       Additionally all "User Actors" obtain the "Exportability" attribute enforced to \c false, i.e. they cannot
 *       export objects independently from the actual attribute value (visible to "Owner").
 *   (3) In order to have possibility load/use a key slot content, the "Owner" application also must have
 *       the "User" entry in the permissions table.
 * \vpublic
 */
struct UserPermissions {
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  ActorUid mActorUid{}; /*!< UID of a User Actor (application). User actor has right */
                        /*!< to load the crypto object to suitable crypto contexts. */
  // VECTOR NL AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  AllowedUsageFlags mAllowedUsage{}; /*!< Restriction flags of allowed usage of a key stored to */
                                     /*!< the slot for application defined by the actorUid. */
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_USER_PERMISSIONS_H_
