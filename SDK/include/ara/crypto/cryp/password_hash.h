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
/*!        \file  ara/crypto/cryp/password_hash.h
 *        \brief  Secure Password Hash object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_HASH_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_HASH_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_object.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Secure Password Hash object interface.
 * \details The password hash object cannot be exportable!
 * \details This object includes "seed" (randomization vector) and hash value of the password and seed.
 * \vpublic
 */
class PasswordHash : public CryptoObject {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kPasswordHash};

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<PasswordHash, CustomDeleter>;

  /*!
   * \brief Get the hash size of this object.
   * \return Size of the hash value in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetHashSize() const noexcept = 0;

  /*!
   * \brief Get the seed size of this object.
   * \return Size of the seed value in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetSeedSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_HASH_H_
