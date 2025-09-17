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
/*!        \file  ara/crypto/cryp/private_key.h
 *        \brief  Generalized Asymmetric Private Key interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/public_key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Generalized Asymmetric Private Key interface.
 * \vpublic
 */
class PrivateKey : public Key {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c KeyType enumeration.
   * \vpublic
   */
  static KeyType const sKeyType{KeyType::kPrivateKey};

  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kPrivateKey};

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<PrivateKey const, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   *
   * \brief Get the public key correspondent to this private key.
   * \param[in] params  an optional pointer to domain parameters required for full specification of the
   *            transformation
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the public key correspondent to this private key
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type (incompatible with this algorithm)
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has incomplete state
   * \error SecurityErrorDomain::kUnexpectedValue  if provided domain parameters object has \a COUID
   *                different from the \a COUID referenced in this private key
   *                (i.e. returned by \c this->HasDependence())
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not
   *                allocated
   * \error SecurityErrorDomain::kInsufficientResource  if capacity of slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<PublicKey::Uptrc> GetPublicKey(
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
  /*!
   * \brief Downcast and move unique smart pointer of the base \c Key interface to the \c PrivateKey one.
   * \param[in] key  unique smart pointer to the constant \c Key interface
   * \return unique smart pointer to downcasted constant interface \c PrivateKey
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadObjectType  if an actual type of the \c key is not \c PrivateKey::Uptrc
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static ara::core::Result<PrivateKey::Uptrc> Cast(Key::Uptrc&& key) noexcept;
};

inline ara::core::Result<PrivateKey::Uptrc> PrivateKey::Cast(Key::Uptrc&& key) noexcept {
  return Downcast<PrivateKey>(std::move(key));
}

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_H_
