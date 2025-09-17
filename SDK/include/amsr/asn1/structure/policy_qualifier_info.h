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
/*!        \file  amsr/asn1/structure/policy_qualifier_info.h
 *        \brief  Representation of parsed Policy qualifier info structure.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_QUALIFIER_INFO_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_QUALIFIER_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/display_text.h"
#include "amsr/asn1/structure/notice_reference.h"
#include "amsr/core/optional.h"
#include "amsr/core/variant.h"
#include "user_notice.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {
namespace structure {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Policy qualifier info Structure.
 */
class PolicyQualifierInfo {
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
  auto operator==(PolicyQualifierInfo const& other) const -> bool {
    return std::tie(PolicyQualifierId(), Qualifier()) == std::tie(other.PolicyQualifierId(), other.Qualifier());
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
  auto operator!=(PolicyQualifierInfo const& other) const -> bool { return !(*this == other); }
  /*!
   * Alias for CHOICE type.
   */
  using QualifierType = amsr::core::Variant<Ia5String, UserNotice, RawData>;

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
  auto PolicyQualifierId() const noexcept -> Oid const& { return policy_qualifier_id_; }

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
  auto PolicyQualifierId() noexcept -> Oid& { return policy_qualifier_id_; }

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
  auto Qualifier() const noexcept -> QualifierType const& { return qualifier_; }

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
  auto Qualifier() noexcept -> QualifierType& { return qualifier_; }

  /*!
   * \brief Method to encode PolicyQualifierInfo class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   *  OID for CPSuri.
   */
  static constexpr amsr::core::StringView kCpsUri{"1.3.6.1.5.5.7.2.1"_sv};
  /*!
   *  OID for UserNotice.
   */
  static constexpr amsr::core::StringView kUnotice{"1.3.6.1.5.5.7.2.2"_sv};

 private:
  Oid policy_qualifier_id_{}; /*!< PolicyQualifierId Identifier. */
  QualifierType qualifier_{}; /*!< Qualifier Identifier. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_POLICY_QUALIFIER_INFO_H_
