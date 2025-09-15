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
/*!        \file  ara/crypto/cryp/private_key_context.h
 *        \brief  Generalized interface of Private Key algorithm Context.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_CONTEXT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/keyed_context.h"
#include "ara/crypto/cryp/private_key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Generalized interface of Private Key algorithm Context.
 * \vpublic
 */
class PrivateKeyContext : public KeyedContext {
 public:
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Set (load) a key to the algorithm context.
   * \param[in] key  a reference to a source key
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if provided key object is incompatible with
   *                an algorithm assigned to this private key context
   * \error SecurityErrorDomain::kBadObjectReference  if provided private key object references to
   *                an instance of the domain parameters different from the one loaded to the context,
   *                i.e. if the COUID of the domain parameters in the context (see \c GetParametersUid()) is
   *                not equal to the COUID referenced from the private key object
   * \error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
   *                is prohibited by the "allowed usage" restrictions of provided key object
   *                restrictions of provided key object.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetKey(PrivateKey const& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PRIVATE_KEY_CONTEXT_H_
