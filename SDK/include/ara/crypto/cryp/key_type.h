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
/*!        \file  ara/crypto/cryp/key_type.h
 *        \brief  Generalized Key object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_TYPE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/keyed_context.h"
#include "ara/crypto/cryp/restricted_use_object.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Enumeration of all known types of supported key objects.
 * \details Storage type: 8 bit unsigned integer.
 * \vpublic
 */
enum class KeyType : std::uint8_t {
  kUnknown = 0,       // A value reserved for erroneous situations.
  kSymmetricKey = 1,  // Symmetric key (\c SymmetricKey interface).
  kPrivateKey = 2,    // Private key (\c PrivateKey interface).
  kPublicKey = 3      // Public key (\c PublicKey interface).
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_TYPE_H_
