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
/*!        \file  ara/crypto/cryp/msg_recovery_public_ctx.h
 *        \brief  A public key context for asymmetric recovery of a
 *                short message and its signature verification (RSA-like).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MSG_RECOVERY_PUBLIC_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MSG_RECOVERY_PUBLIC_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/block_cryptor.h"
#include "ara/crypto/cryp/public_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief A public key context for asymmetric recovery of a short message and its signature verification (RSA-like).
 * \details Restricted groups of trusted subscribers can use this primitive for simultaneous provisioning
 *          of confidentiality, authenticity and non-repudiation of short messages, if the public key is
 *          generated appropriately and kept in secret.
 * \details If (0 == BlockCryptor::ProcessBlock(...)) then the input message-block is violated.
 * \vpublic
 */
class MsgRecoveryPublicCtx : public PublicKeyContext, public BlockCryptor {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<MsgRecoveryPublicCtx, CustomDeleter>;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_MSG_RECOVERY_PUBLIC_CTX_H_
