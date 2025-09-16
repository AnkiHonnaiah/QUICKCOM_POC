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
/*!        \file  amsr/asn1/structure/extension.h
 *        \brief  Representation of parsed Extension.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "access_description.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "attribute.h"
#include "authority_key_identifier.h"
#include "basic_constraint.h"
#include "distribution_point.h"
#include "general_name.h"
#include "key_usage.h"
#include "name_constraints.h"
#include "policy_constraints.h"
#include "policy_information.h"
#include "policy_mapping.h"

namespace amsr {
namespace asn1 {
namespace structure {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Extension Structure.
 */
class Extension {
 public:
  /*!
   * \brief Alias declaration.
   */
  using ExtnValueType = amsr::core::Variant<
      structure::RawData, structure::KeyUsage, structure::GeneralNames, ara::core::Vector<structure::AccessDescription>,
      structure::AuthorityKeyIdentifier, structure::BasicConstraint, structure::OctetString, structure::Integer,
      ara::core::Vector<structure::DistributionPoint>, ara::core::Vector<AttributeSet>, structure::NameConstraints,
      ara::core::Vector<structure::PolicyInformation>, ara::core::Vector<structure::PolicyMapping>,
      ara::core::Vector<structure::Oid>, structure::PolicyConstraints, structure::Null>;
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
  auto operator==(Extension const& other) const -> bool {
    return std::tie(ExtnID(), Critical(), ExtnValue()) == std::tie(other.ExtnID(), other.Critical(), other.ExtnValue());
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
  auto operator!=(Extension const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property extnID.
   * \return Value of extnid.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtnID() const noexcept -> Oid const& { return extn_id_; }

  /*!
   * \brief Access interface for property extnid.
   * \return Value of extnid.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtnID() noexcept -> Oid& { return extn_id_; }

  /*!
   * \brief Read-only access interface for property critical.
   * \return Value of critical.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Critical() const noexcept -> Boolean const& { return critical_; }

  /*!
   * \brief Access interface for property critical.
   * \return Value of critical.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Critical() noexcept -> Boolean& { return critical_; }

  /*!
   * \brief Read-only access interface for property extnvalue.
   * \return Readonly reference to extnvalue.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtnValue() const noexcept -> ExtnValueType const& { return extn_value_; }

  /*!
   * \brief Access interface for property extnvalue.
   * \return Reference to extnvalue.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtnValue() noexcept -> ExtnValueType& { return extn_value_; }

  /*!
   * \brief Method to encode Extension class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Encode the extension value of this object without type or criticality information.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto EncodeExtensionValue() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   *  OID for AuthorityInfoAccessSyntax.
   */
  static constexpr amsr::core::StringView kAuthorityInfoAccessSyntax{"1.3.6.1.5.5.7.1.1"_sv};

  /*!
   *  OID for AuditIdentity.
   */
  static constexpr amsr::core::StringView kAuditIdentity{"1.3.6.1.5.5.7.1.4"_sv};

  /*!
   *  OID for SubjectInfoAccessSyntax.
   */
  static constexpr amsr::core::StringView kSubjectInfoAccessSyntax{"1.3.6.1.5.5.7.1.11"_sv};

  /*!
   *  OID for SubjectDirectoryAttributes.
   */
  static constexpr amsr::core::StringView kSubjectDirectoryAttributes{"2.5.29.9"_sv};

  /*!
   *  OID for SubjectKeyIdentifier.
   */
  static constexpr amsr::core::StringView kSubjectKeyIdentifier{"2.5.29.14"_sv};

  /*!
   *  OID for keyUsage.
   */
  static constexpr amsr::core::StringView kKeyUsage{"2.5.29.15"_sv};

  /*!
   *  OID for SubjectAltName.
   */
  static constexpr amsr::core::StringView kSubjectAltName{"2.5.29.17"_sv};

  /*!
   *  OID for IssuerAltName.
   */
  static constexpr amsr::core::StringView kIssuerAltName{"2.5.29.18"_sv};

  /*!
   *  OID for BasicConstraint.
   */
  static constexpr amsr::core::StringView kBasicConstraint{"2.5.29.19"_sv};

  /*!
   *  OID for NameConstraints.
   */
  static constexpr amsr::core::StringView kNameConstraints{"2.5.29.30"_sv};

  /*!
   *  OID for CRLDistributionPoints.
   */
  static constexpr amsr::core::StringView kCRLDistributionPoints{"2.5.29.31"_sv};

  /*!
   *  OID for CertificatePolicies.
   */
  static constexpr amsr::core::StringView kCertificatePolicies{"2.5.29.32"_sv};

  /*!
   *  OID for PolicyMappings.
   */
  static constexpr amsr::core::StringView kPolicyMappings{"2.5.29.33"_sv};

  /*!
   *  OID for AuthorityKeyIdentifier.
   */
  static constexpr amsr::core::StringView kAuthorityKeyIdentifier{"2.5.29.35"_sv};

  /*!
   *  OID for PolicyConstraints.
   */
  static constexpr amsr::core::StringView kPolicyConstraints{"2.5.29.36"_sv};

  /*!
   *  OID for ExtKeyUsageSyntax.
   */
  static constexpr amsr::core::StringView kExtKeyUsageSyntax{"2.5.29.37"_sv};

  /*!
   *  OID for freshestCRL.
   */
  static constexpr amsr::core::StringView kFreshestCRL{"2.5.29.46"_sv};

  /*!
   *  OID for InhibitAnyPolicy.
   */
  static constexpr amsr::core::StringView kInhibitAnyPolicy{"2.5.29.54"_sv};

  /*!
   *  OID for TargetInformation.
   */
  static constexpr amsr::core::StringView kTargetInformation{"2.5.29.55"_sv};

  /*!
   *  OID for NoRevAvail.
   */
  static constexpr amsr::core::StringView kNoRevAvail{"2.5.29.56"_sv};

 private:
  // FIXME: This is redundant in ResponseBytes and meant for recursive parsing.
  /*!
   * \brief Checks if a raw data structure starts with a universal, constructed OCTET STRING header.
   * \param raw The structure to check
   * \return Whether the structure starts with \c 0x04
   */
  static auto HasOctetStringHeader(RawData const& raw) noexcept -> bool;

  Oid extn_id_{};              /*!< Extension Identifier. */
  Boolean critical_{false};    /*!< Critical marker. */
  ExtnValueType extn_value_{}; /*!< Extension value. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_H_
