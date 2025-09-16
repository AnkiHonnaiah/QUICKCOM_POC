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
/*!        \file  ara/crypto/cryp/x509_request_signer_ctx.h
 *        \brief  X.509 Request Signer Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_REQUEST_SIGNER_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_REQUEST_SIGNER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/private_key_context.h"
#include "ara/crypto/cryp/signature_handler.h"
#include "ara/crypto/cryp/x509_cert_request.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief X.509 Request Signer Context interface.
 * \details Any private key (including keys without Key::ALLOW_SIGNATURE attribute)
 *          can be loaded to this interface context!
 * \vpublic
 */
class X509RequestSignerCtx : public PrivateKeyContext, public SignatureHandler {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<X509RequestSignerCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Creates certification request for a private key loaded to the context.
   * \param[in] der_subject_dn A DER-encoded subject distinguished name (DN) of the private key owner.
   * \param[in] x509_extensions A DER-encoded X.509 Extensions that should be included to the certification request.
   * \param[in] version Format version of the target certification request.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for this
   *                          allocation or default marker, which says to allocate on the heap.
   * \return Unique smart pointer to created certification request.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnexpectedValue If any of arguments has incorrect/unsupported value.
   * \error SecurityErrorDomain::kBusyResource If the slot specified by \c reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource If the slot specified by \c reserved_index was not allocated.
   * \error SecurityErrorDomain::kInsufficientResource If capacity of slot specified by \c reserved_index is not enough
   * for placing of the target object.
   * \error SecurityErrorDomain::kBadAlloc If (reserved_index == kAllocObjectOnHeap), but allocation memory ons the heap
   * is impossible.
   * \error SecurityErrorDomain::kUninitializedContext If the context is used before initialization.
   * \error SecurityErrorDomain::kRuntimeFault If a runtime error occurred, e.g. signature creation failed
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<X509CertRequest::Uptrc> CreateCertRequest(
      ReadOnlyMemRegion der_subject_dn, ReadOnlyMemRegion x509_extensions = ReadOnlyMemRegion{},
      std::uint32_t version = 1, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_X509_REQUEST_SIGNER_CTX_H_
