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
/*!        \file  amsr/asn1/structure/policy_constraints.h
 *        \brief  Representation of parsed Policy Constraint.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_CONSTRAINTS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_CONSTRAINTS_H_

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
 * \brief Policy Constraints Structure.
 */
class PolicyConstraints {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(PolicyConstraints const& other) const -> bool {
    return std::tie(RequireExplicitPolicy(), InhibitPolicyMapping()) ==
           std::tie(other.RequireExplicitPolicy(), other.InhibitPolicyMapping());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(PolicyConstraints const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access API for RequireExplicitPolicy.
   * \return Value of property RequireExplicitPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RequireExplicitPolicy() const noexcept -> amsr::core::Optional<Integer> const& {
    return require_explicit_policy_;
  }

  /*!
   * \brief Access API for RequireExplicitPolicy.
   * \return Value of property RequireExpliciyPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RequireExplicitPolicy() noexcept -> amsr::core::Optional<Integer>& { return require_explicit_policy_; }

  /*!
   * \brief Read-Only access API for InhibitPolicyMapping.
   * \return Value of InhibitPolicyMapping.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto InhibitPolicyMapping() const noexcept -> amsr::core::Optional<Integer> const& { return inhibit_policy_mapping_; }

  /*!
   * \brief Access API for InhibitPolicyMapping.
   * \return Value of InhibitPolicyMapping.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto InhibitPolicyMapping() noexcept -> amsr::core::Optional<Integer>& { return inhibit_policy_mapping_; }

  /*!
   * \brief Method to encode PolicyConstraints class.
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
  amsr::core::Optional<Integer> require_explicit_policy_{}; /*!< RequireExplicitPolicy. */
  amsr::core::Optional<Integer> inhibit_policy_mapping_{};  /*!< InhibitPolicyMapping. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_CONSTRAINTS_H_
