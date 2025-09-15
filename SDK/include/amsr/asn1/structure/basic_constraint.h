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
/*!        \file  amsr/asn1/structure/basic_constraint.h
 *        \brief  Representation of parsed basic constraint.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BASIC_CONSTRAINT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BASIC_CONSTRAINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief BasicConstraint structure.
 */
class BasicConstraint {
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
  auto operator==(BasicConstraint const& other) const -> bool {
    return std::tie(CA(), PathLenConstraint()) == std::tie(other.CA(), other.PathLenConstraint());
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
  auto operator!=(BasicConstraint const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property cA type.
   * \return Value of cA type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CA() const noexcept -> Boolean const& { return ca_; }

  /*!
   * \brief Access interface for property cA type.
   * \return Value of cA type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CA() noexcept -> Boolean& { return ca_; }

  /*!
   * \brief Read-only access interface for property pathlenconstraint.
   * \return Value of pathlenconstraint.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PathLenConstraint() const noexcept -> amsr::core::Optional<Integer> const& { return path_len_constraint_; }

  /*!
   * \brief Access interface for pathlenconstraint.
   * \return Value of pathlenconstraint.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PathLenConstraint() noexcept -> amsr::core::Optional<Integer>& { return path_len_constraint_; }

  /*!
   * \brief Method to encode BasicConstraint structure.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode() const noexcept;

 private:
  Boolean ca_{false};                                   /*!< cA. */
  amsr::core::Optional<Integer> path_len_constraint_{}; /*!< pathLenConstraint. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BASIC_CONSTRAINT_H_
