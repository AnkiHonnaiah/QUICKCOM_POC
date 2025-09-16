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
/*!        \file  ara/crypto/keys/elementary_types.h
 *        \brief  Key Storage API. Elementary types definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace keys {

/*!
 * \brief The Slot Number is intended for direct addressing of the "Key Slots" in the Key Storage.
 * \vpublic
 */
using SlotNumber = std::size_t;

/*!
 * \brief A reserved slot number that cannot be used for addressing of real slots of the storage.
 * \vpublic
 */
SlotNumber const kInvalidSlot{static_cast<SlotNumber>(std::numeric_limits<SlotNumber>::max())};

/*!
 * \brief Definition of a transaction identifier type.
 * \vpublic
 */
using TransactionId = std::uint64_t;

/*!
 * \brief Definition of a "transaction scope" type.
 * \details The "transaction scope" defines a list of key slots that are target for update in a transaction.
 * \vpublic
 */
using TransactionScope = ara::core::Vector<SlotNumber>;

/*!
 * \brief Enumeration of all Version Control Types.
 * \vpublic
 */
enum class VersionControlType : std::uint8_t {
  kNone = 0,      // Version control is not applied for content of this slot.
  kLocal = 1,     // Version control is applied, slot initialization is not required, but only locally produced crypto
                  // object can    be saved to the slot.
  kExternal = 2,  // Version control is applied, slot must be initialized by a COUID, specifying concrete external
                  // source of objects and minimal version.
  kSwitchToLocal = 3  // Similar to \c kLocal, but the slot can be initialized by an externally produced crypto
                      // object, which doesn't leave version control "track".
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_
