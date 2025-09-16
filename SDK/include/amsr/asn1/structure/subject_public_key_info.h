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
/*!       \file   amsr/asn1/structure/subject_public_key_info.h
 *        \brief  Representation of parsed SubjectPublicKeyInfo.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_SUBJECT_PUBLIC_KEY_INFO_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_SUBJECT_PUBLIC_KEY_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief SubjectPublicKeyInfo Structure.
 */
class SubjectPublicKeyInfo {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator==(SubjectPublicKeyInfo const& other) const -> bool {
    return std::tie(Algorithm(), SubjectPublicKey()) == std::tie(other.Algorithm(), other.SubjectPublicKey());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator!=(SubjectPublicKeyInfo const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property Algorithm.
   * \return Value of Algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Algorithm() const noexcept -> AlgorithmIdentifier const& { return algorithm_; }

  /*!
   * \brief Access interface for property Algorithm.
   * \return Value of Algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Algorithm() noexcept -> AlgorithmIdentifier& { return algorithm_; }

  /*!
   * \brief Read-only access interface for property SubjectPublicKey.
   * \return Value of SubjectPublicKey.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPublicKey() const noexcept -> BitString const& { return subject_public_key_; }

  /*!
   * \brief Access interface for property SubjectPublicKey.
   * \return Value of SubjectPublicKey.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPublicKey() noexcept -> BitString& { return subject_public_key_; }

  /*!
   * \brief Method to encode SubjectPublicKey class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  AlgorithmIdentifier algorithm_; /*!< algorithm */
  BitString subject_public_key_;  /*!< subjectPublicKey */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_SUBJECT_PUBLIC_KEY_INFO_H_
