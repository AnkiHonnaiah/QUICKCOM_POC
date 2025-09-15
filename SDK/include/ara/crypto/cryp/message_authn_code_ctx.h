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
/*!        \file  ara/crypto/cryp/message_authn_code_ctx.h
 *        \brief  Keyed Message Authentication Code Context interface definition (MAC/HMAC).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/buffered_digest.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Keyed Message Authentication Code Context interface definition (MAC/HMAC).
 * \vpublic
 */
class MessageAuthnCodeCtx : public SymmetricKeyContext, public BufferedDigest {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<MessageAuthnCodeCtx, CustomDeleter>;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Start, Update and finish the digest calculation context by a message in one shot.
   * \details Only after call of this method the digest can be signed, verified, extracted or compared!
   * \details This method is convenient for processing of constant tags.
   * \param[in] in The input message that should be processed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by required domain parameters
   *        or key object
   * \error SecurityErrorDomain::kUnsupported if the base algorithm (or its current implementation) principally doesn't
   *        support an IV-less variation
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> StartUpdateFinish(ara::crypto::ReadOnlyMemRegion in) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H_
