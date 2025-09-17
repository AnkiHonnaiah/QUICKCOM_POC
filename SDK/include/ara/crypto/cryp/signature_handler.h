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
/*!        \file  ara/crypto/cryp/signature_handler.h
 *        \brief  A basic interface for both types of the signature handlers: signer and verifier.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_HANDLER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief A basic interface for both types of the signature handlers: signer and verifier.
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class SignatureHandler : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  SignatureHandler() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~SignatureHandler() noexcept override = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  SignatureHandler(SignatureHandler&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  SignatureHandler(SignatureHandler const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  SignatureHandler& operator=(SignatureHandler const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  SignatureHandler& operator=(SignatureHandler&& /*other*/) = delete;

  /*!
   * \brief Get hash size required by current signature algorithm.
   * \return Required hash size in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetRequiredHashSize() const noexcept = 0;

  /*!
   * \brief Get an ID of hash algorithm required by current signature algorithm.
   * \return Required hash algorithm ID or \c kAlgIdAny if the signature algorithm specification
   *         does not include a concrete hash function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept = 0;

  /*!
   * \brief Get size of the signature value produced and required by the current algorithm.
   * \return Size of the signature value in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetSignatureSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_HANDLER_H_
