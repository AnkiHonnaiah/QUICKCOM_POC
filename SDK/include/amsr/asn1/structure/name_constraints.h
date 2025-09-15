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
/*!        \file  amsr/asn1/structure/name_constraints.h
 *        \brief  Representation of parsed NameConstraints.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_CONSTRAINTS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_CONSTRAINTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/general_subtree.h"
#include "amsr/core/optional.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief NameConstraints Structure.
 */
class NameConstraints {
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
  auto operator==(NameConstraints const& other) const -> bool {
    return std::tie(PermittedSubtrees(), ExcludedSubtrees()) ==
           std::tie(other.PermittedSubtrees(), other.ExcludedSubtrees());
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
  auto operator!=(NameConstraints const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access API for PermittedSubtrees.
   * \return Value of property PermittedSubtrees.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>> const& PermittedSubtrees() const noexcept {
    return permitted_subtrees_;
  }

  /*!
   * \brief Access API for PermittedSubtrees.
   * \return Value of property PermittedSubtrees.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>>& PermittedSubtrees() noexcept { return permitted_subtrees_; }

  /*!
   * \brief Read-Only access API for ExcludedSubtrees.
   * \return Value of ExcludedSubtrees.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>> const& ExcludedSubtrees() const noexcept {
    return excluded_subtrees_;
  }

  /*!
   * \brief Access API for ExcludedSubtrees.
   * \return Value of ExcludedSubtrees.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>>& ExcludedSubtrees() noexcept { return excluded_subtrees_; }

  /*!
   * \brief Method to encode NameConstraints class.
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
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>> permitted_subtrees_{}; /*!< PermittedSubtrees. */
  amsr::core::Optional<ara::core::Vector<GeneralSubtree>> excluded_subtrees_{};  /*!< ExcludedSubtrees. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_CONSTRAINTS_H_
