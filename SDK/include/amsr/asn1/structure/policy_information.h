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
/*!        \file  amsr/asn1/structure/policy_information.h
 *        \brief  Representation of parsed Policy information structure.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_INFORMATION_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_INFORMATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/policy_qualifier_info.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Policy information structure.
 */
class PolicyInformation {
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
  auto operator==(PolicyInformation const& other) const -> bool {
    return std::tie(PolicyIdentifier(), PolicyQualifiers()) ==
           std::tie(other.PolicyIdentifier(), other.PolicyQualifiers());
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
  auto operator!=(PolicyInformation const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property PolicyQualifierId.
   * \return Value of PolicyQualifierId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PolicyIdentifier() const noexcept -> structure::Oid const& { return policy_identifier_; }

  /*!
   * \brief Access interface for property PolicyQualifierId.
   * \return Value of PolicyQualifierId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PolicyIdentifier() noexcept -> structure::Oid& { return policy_identifier_; }

  /*!
   * \brief Read-only access interface for property explicitText.
   * \return Value of explicitText.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PolicyQualifiers() const noexcept
      -> amsr::core::Optional<ara::core::Vector<structure::PolicyQualifierInfo>> const& {
    return policy_qualifiers_;
  }

  /*!
   * \brief Access interface for property explicitText.
   * \return Value of explicitText.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PolicyQualifiers() noexcept -> amsr::core::Optional<ara::core::Vector<structure::PolicyQualifierInfo>>& {
    return policy_qualifiers_;
  }

  /*!
   * \brief Method to encode PolicyInformation class.
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
  structure::Oid policy_identifier_{}; /*!< PolicyIdentifier Identifier. */
  amsr::core::Optional<ara::core::Vector<structure::PolicyQualifierInfo>>
      policy_qualifiers_{}; /*!< PolicyQualifiers Identifier. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_INFORMATION_H_
