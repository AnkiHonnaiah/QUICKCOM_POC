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
/*!        \file  ara/crypto/cryp/signature.h
 *        \brief  Signature container interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_object.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Signature container interface.
 * \details This interface is applicable for keeping the Digital Signature,
 *          Hash Digest, (Hash-based) Message Authentication Code (MAC/HMAC).
 * \details In case of a keyed signature (Digital Signature or MAC/HMAC) a COUID
 *          of the signature verification key can be obtained by a call of CryptoObject::HasDependence()!
 * \vpublic
 */
class Signature : public CryptoObject, public Serializable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  Signature() = default;

  // Attention! Deviation: Protected destructor made non-virtual to satisfy AutosarC++17_10-A12.4.1
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~Signature() noexcept override = default;

 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kSignature};

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  Signature(Signature const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  Signature(Signature&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  Signature& operator=(Signature&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  Signature& operator=(Signature const& /*other*/) = delete;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<Signature const, CustomDeleter>;

  /*!
   * \brief Get an ID of hash algorithm used for this signature object production.
   * \return ID of used hash algorithm only (without signature algorithm specification).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNATURE_H_
