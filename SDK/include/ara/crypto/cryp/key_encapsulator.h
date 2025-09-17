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
/*!        \file  ara/crypto/cryp/key_encapsulator.h
 *        \brief  Basic interface of Asymmetric Key Encapsulation Mechanism (KEM).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/std_api.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Basic interface of Asymmetric Key Encapsulation Mechanism (KEM).
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class KeyEncapsulator : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  KeyEncapsulator() = default;

  /*!
   * \brief Virtual destructor.
   * \vprivate Component Private
   */
  ~KeyEncapsulator() noexcept override = default;

 public:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  KeyEncapsulator(KeyEncapsulator const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  KeyEncapsulator(KeyEncapsulator&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  KeyEncapsulator& operator=(KeyEncapsulator const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \vprivate Component Private
   */
  KeyEncapsulator& operator=(KeyEncapsulator&& /*other*/) & noexcept = delete;

  /*!
   * \brief Get entropy (bit-length) of the key encryption key (KEK) material.
   * \details For RSA system the returned value corresponds to the length of module N (minus 1).
   * \details For DH-like system the returned value corresponds to the length of module q (minus 1).
   * \return Entropy of the KEK material in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetKekEntropy() const noexcept = 0;

  /*!
   * \brief Get fixed size of the encapsulated data block.
   * \return Size of the encapsulated data block in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetEncapsulatedSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_H_
