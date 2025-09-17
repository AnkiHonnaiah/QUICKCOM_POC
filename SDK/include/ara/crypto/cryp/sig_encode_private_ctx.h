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
/*!        \file  ara/crypto/cryp/sig_encode_private_ctx.h
 *        \brief  A private key context for asymmetric signature calculation and short message encoding (RSA-like).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/block_cryptor.h"
#include "ara/crypto/cryp/private_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief A private key context for asymmetric signature calculation and short message encoding (RSA-like).
 * \details Restricted groups of trusted subscribers can use this primitive for simultaneous provisioning
 *          of confidentiality, authenticity and non-repudiation of short messages, if the public key
 *          is generated appropriately and kept in secret.
 * \vpublic
 */
class SigEncodePrivateCtx : public PrivateKeyContext, public BlockCryptor {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SigEncodePrivateCtx, CustomDeleter>;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H_
