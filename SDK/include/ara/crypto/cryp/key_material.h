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
/*!        \file  ara/crypto/cryp/key_material.h
 *        \brief  Generalized Key Material interface (in general case it represents a non-saveable temporary entity).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_MATERIAL_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_MATERIAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Generalized Key Material interface (in general case it represents a non-saveable temporary entity).
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class KeyMaterial : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  KeyMaterial(KeyMaterial&& /*other*/) noexcept = default;

  /*!
   * \brief Virtual destructor.
   */
  ~KeyMaterial() noexcept override = default;

 public:
  /*!
   * \brief Default constructor.
   * \vpublic
   */
  KeyMaterial() = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  KeyMaterial(KeyMaterial const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  KeyMaterial& operator=(KeyMaterial&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  KeyMaterial& operator=(KeyMaterial const& /*other*/) = delete;

  /*!
   * \brief Returns actual key length in bits.
   * \return Actual key length in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetActualKeyBitLength() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_MATERIAL_H_
