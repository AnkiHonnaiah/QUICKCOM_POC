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
/*!        \file  ara/crypto/cryp/x509_public_key_info.h
 *        \brief  X.509 Public Key Information interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_PUBLIC_KEY_INFO_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_PUBLIC_KEY_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/public_key.h"
#include "ara/crypto/cryp/x509_algorithm_id.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief X.509 Public Key Information interface.
 * \vpublic
 */
class X509PublicKeyInfo : public X509AlgorithmId {
 public:
  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<X509PublicKeyInfo const, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get public key object of the subject.
   * \param[in] params An optional pointer to Domain Parameters required for full specification of the transformation.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for this
   *                          allocation or default marker, which says to allocate on the heap.
   * \return Smart unique pointer to the created public key of the subject.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer If the domain parameters are required, but (params == nullptr) was
   * passed.
   * \error SecurityErrorDomain::kIncompatibleObject If (params != nullptr), but provided domain parameters object has
   * inappropriate type.
   * \error SecurityErrorDomain::kIncompleteArgState If (params != nullptr), but provided domain parameters object has
   * an incomplete state.
   * \error SecurityErrorDomain::kBusyResource If the slot specified by \c reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource If the slot specified by \c reserved_index was not allocated.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<PublicKey::Uptrc> GetPublicKey(
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  /*!
   * \brief Verifies the sameness of the provided and kept public keys.
   * \param[in] public_key A public key object for comparison.
   * \return true if values of the stored public key and object provided by argument are identical.
   * \details This method compare the public key values only.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsSameKey(PublicKey const& public_key) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_PUBLIC_KEY_INFO_H_
