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
/*!        \file  ara/crypto/cryp/x509_algorithm_id.h
 *        \brief  X.509 Algorithm ID interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_ALGORITHM_ID_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_ALGORITHM_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/signature_handler.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief X.509 Algorithm ID interface.
 * \details If instance of this interface is created for unsupported algorithm,
 *          then method GetPrimitiveId() will return ALGID_UNKNOWN!
 * \vpublic
 */
class X509AlgorithmId : public CryptoPrimitiveId, public SignatureHandler, public Serializable {
 public:
  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<X509AlgorithmId const, CustomDeleter>;

  /*!
   * \brief Verifies presence of domain parameters in this object.
   * \return \c true if this instance includes domain parameters and \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool HasDomainParameters() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get domain parameters object associated with the public key of the subject.
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return Unique smart pointer to the created Domain parameters object associated with the public key of the
   *          subject.
   * \details This method returns \c nullptr if this instance does not include domain parameters
   * \context ANY
   * \pre -
   * \threadsafe TRUE
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
  virtual ara::core::Result<DomainParameters::Sptrc> GetDomainParameters(
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  /*!
   * \brief Verify the sameness of the provided and internally stored domain parameters.
   * \param[in] params The domain parameters object for comparison.
   * \return \c true if values of the stored domain parameters and object provided by the argument are identical.
   * \details If the domain parameters specified by the \c params argument has an incomplete state then this method
   *       should return \c false without actual comparison.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsSameParameters(DomainParameters const& params) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_ALGORITHM_ID_H_
