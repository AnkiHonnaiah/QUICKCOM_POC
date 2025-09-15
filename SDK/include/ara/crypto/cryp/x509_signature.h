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
/*!        \file  ara/crypto/cryp/x509_signature.h
 *        \brief  X.509 Signature interface (X.509 Algorithm Identifier & Signature Value).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_SIGNATURE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_SIGNATURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/signature.h"
#include "ara/crypto/cryp/x509_algorithm_id.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief X.509 Signature interface (X.509 Algorithm Identifier & Signature Value).
 * \vpublic
 */
class X509Signature : public X509AlgorithmId {
 public:
  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<X509Signature const, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get signature object of the hosted DER-structure.
   * \param[in] reserved_index An optional index of reserved Object slot that should be
   *            used for this allocation or default marker, which says to allocate on the heap.
   * \return Smart unique pointer to the created signature object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBusyResource If the slot specified by reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource If the slot specified by reserved_index was not allocated.
   * \error SecurityErrorDomain::kInsufficientResource If capacity of slot specified by reserved_index is not enough for
   * placing of the target object.
   * \error SecurityErrorDomain::kBadAlloc If (reserved_index == kAllocObjectOnHeap), but allocation memory on the heap
   * is impossible.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<Signature::Uptrc> GetSignature(
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_SIGNATURE_H_
