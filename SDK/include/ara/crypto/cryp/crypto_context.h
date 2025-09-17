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
/*!        \file  ara/crypto/cryp/crypto_context.h
 *        \brief  A common interface of a mutable cryptographic context,
 *                i.e. that is not binded to a single crypto object.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/cryp/crypto_primitive_id.h"
#include "ara/crypto/cryp/domain_parameters.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief A common interface of a mutable cryptographic context, i.e. that is not binded to a single crypto object.
 * \vpublic
 */
class CryptoContext : public CryptoPrimitiveId {
 public:
  /*!
   * \brief Checks if the crypto context requires initialization by a key value.
   * \return true if the crypto context requires initialization by a key value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsKeyedContext() const noexcept = 0;

  /*!
   * \brief Check if the crypto context is already initialized and ready to use. It checks all required values,
   * including: domain parameters, key value, IV/seed, etc.
   * \return true if the crypto context is completely initialized and ready to use, and false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsInitialized() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Clear the crypto context and set the domain parameters to it.
   * \param[in] params  an optional shared pointer to a domain parameters object, which should be set to this
   *  crypto context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if provided domain parameters object is incompatible
   *                with an algorithm assigned to this cryptographic context
   * \error SecurityErrorDomain::kIncompleteArgState  if provided domain parameters object has incomplete state
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Reset(DomainParameters::Sptrc params = nullptr) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the COUID of a domain parameters object assigned to the context.
   * \param[out] parameters_uid  optional pointer to the output buffer for the target COUID saving
   * \return \c true if the context has assigned domain parameters object and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool GetParametersUid(CryptoObjectUid* parameters_uid = nullptr) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_
