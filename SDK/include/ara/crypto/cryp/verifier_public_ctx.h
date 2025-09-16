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
/*!        \file  ara/crypto/cryp/verifier_public_ctx.h
 *        \brief  Signature Verification Public key Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/public_key_context.h"
#include "ara/crypto/cryp/signature.h"
#include "ara/crypto/cryp/signature_handler.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 *  \brief Signature Verification Public key Context interface.
 *  \vpublic
 */
class VerifierPublicCtx : public PublicKeyContext, public SignatureHandler {
 public:
  /*!
   *  \brief Unique smart pointer of the interface.
   *  \vpublic
   */
  using Uptr = std::unique_ptr<VerifierPublicCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   *  \brief Verify signature by a digest value stored in the hash-function context.
   *  \param[in] hash  the finalized hash-function context that contains a digest value ready for the verification
   *  \param[in] signature  the signature object for the verification
   *  \param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
   *  \return \c true if the signature was verified successfully and \c false otherwise
   *  \note The user supplied \c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
   *  \note If any of the 5 mentioned below conditions was violated then this method returns \c false:
   *  - This method must control compliance of the real hash-function algorithm to the hash algorithm specified in
   *    the signature!
   *  - This method must check equality of the real public key COUID and corresponded dependency COUID in
   *    the signature!
   *  - This method must control the hash-function algorithm ID compliance to the signature algorithm specification
   *    of the context!
   *  - This method must control compliance of the signature algorithm ID in the signature object and the signature
   *    verification context!
   *  - This method must control compliance of the signature algorithm ID in the signature object and the user
   *    supplied \c context size!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   *  \error SecurityErrc::kUninitializedContext  if the context was not initialized by a key value
   *  \error SecurityErrc::kIncompatibleObject  if algorithms of \c hash or \c signature arguments
   *                 are incompatible with the context configuration
   *  \error SecurityErrc::kIncompatibleArguments  if algorithms of \c hash or \c signature arguments
   *                 are compatible with the context configuration, but incompatible between each other (it can be
   *                 a case if the verifier context configuration doesn't restrict the hash function algorithm)
   *  \error SecurityErrc::kBadObjectReference  if provided \c signature object references to
   *                 an instance of a public key different from the one loaded to the context,
   *                 i.e. if the COUID of the public key in the context (see \c this->GetActualKeyBitLength()) is
   *                 not equal to the COUID referenced from the \c signature object (see \c signature.HasDependence())
   *  \error SecurityErrc::kProcessingNotFinished  if the method \c hash.Finish() was not called
   *                 before this method call
   *  \error SecurityErrc::kInvalidInputSize  if the \c context argument has unsupported size
   * \reentrant FALSE
   * \synchronous TRUE
   *  \vpublic
   */
  virtual ara::core::Result<bool> Verify(HashFunctionCtx const& hash, Signature const& signature,
                                         ReadOnlyMemRegion context = ReadOnlyMemRegion{}) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   *  \brief Verify signature by a directly provided hash or message value.
   *  \param[in] value  the (pre-)hashed or direct message value that should be verified
   *  \param[in] signature  the signature BLOB for the verification
   *  \param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
   *  \return \c true if the signature was verified successfully and \c false otherwise
   *  \note This method can be used for implementation of the "multiple passes" signature algorithms that process
   *        a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
   *        implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
   *  \note If a size of the \c value, \c signature or \c context BLOB is incorrect then this method returns \c false
   *        before starting of any calculations!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   *  \error SecurityErrc::kUninitializedContext  if the context was not initialized by a key value
   *  \error SecurityErrc::kInvalidInputSize  if the \c value, \c signature or \c context has
   *                 unsupported size
   * \reentrant FALSE
   * \synchronous TRUE
   *  \vpublic
   */
  virtual ara::core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature,
                                         ReadOnlyMemRegion context = ReadOnlyMemRegion{}) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H_
