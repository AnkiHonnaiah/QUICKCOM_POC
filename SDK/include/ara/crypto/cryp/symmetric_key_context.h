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
/*!        \file  ara/crypto/cryp/symmetric_key_context.h
 *        \brief  Generalized interface of Symmetric Key algorithm Context.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_CONTEXT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/keyed_context.h"
#include "ara/crypto/cryp/symmetric_key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Generalized interface of Symmetric Key algorithm Context.
 * \vpublic
 */
class SymmetricKeyContext : public KeyedContext {
 public:
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Set (deploy) a key to the symmetric algorithm context.
   * \param[in] key  the source key object
   * \param[in] direct_transform  the "direction" indicator: deploy the key for direct transformation (if \c true)
   *            or for reverse one (if \c false)
   * \details If domain parameters are principally required for a crypto algorithm assigned to this context then they
   *        must be set before the loading of a key value (via call of the \c CryptoContext::Reset())!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if the crypto primitive of the provided key object is
   *                incompatible with this symmetric key context
   * \error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
   *                (taking into account the direction specified by \c direct_transform) is prohibited by
   *                the "allowed usage" restrictions of provided key object
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetKey(SymmetricKey const& key, bool direct_transform = true) noexcept = 0;

  /*!
   * \brief Gets configured "direction" of the transformation: direct (e.g. encryption) or reverse (e.g. decryption).
   * \return true if the context configured for a direct transformation and false if for a reverse one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsDirectTransform() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_CONTEXT_H_
