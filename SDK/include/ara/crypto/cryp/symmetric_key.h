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
/*!        \file  ara/crypto/cryp/symmetric_key.h
 *        \brief  Symmetric Key interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Symmetric Key interface.
 * \vpublic
 */
class SymmetricKey : public Key {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c KeyType enumeration.
   * \vpublic
   */
  static KeyType const sKeyType{KeyType::kSymmetricKey};

  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kSymmetricKey};

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<SymmetricKey const, CustomDeleter>;

  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SymmetricKey, CustomDeleter>;  // Attention! Deviation: Added typedef for non-const
                                                              // unique_ptr, otherwise the one from CryptoObject is
                                                              // used.
  /*!
   * \brief Downcast and move unique smart pointer of the base \c Key interface to the \c SymmetricKey one.
   * \param[in] key  Unique smart pointer to the constant \c Key interface
   * \return Unique smart pointer to downcasted constant interface \c SymmetricKey.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadObjectType  If an actual type of the \c key is not \c SymmetricKey::Uptrc
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static ara::core::Result<SymmetricKey::Uptrc> Cast(Key::Uptrc&& key) noexcept;
};

inline ara::core::Result<SymmetricKey::Uptrc> SymmetricKey::Cast(Key::Uptrc&& key) noexcept {
  return Downcast<SymmetricKey>(std::move(key));
}

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_H_
