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
/*!        \file  amsr/asn1/structure/policy_mapping.h
 *        \brief  Representation of parsed policy mapping.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_POLICY_MAPPING_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_POLICY_MAPPING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Policy Mapping Structure.
 */
class PolicyMapping {
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
  auto operator==(PolicyMapping const& other) const -> bool {
    return std::tie(IssuerDomainPolicy(), SubjectDomainPolicy()) ==
           std::tie(other.IssuerDomainPolicy(), SubjectDomainPolicy());
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
  auto operator!=(PolicyMapping const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property IssuerDomainPolicy.
   * \return Value of IssuerDomainPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerDomainPolicy() const noexcept -> Oid const& { return issuer_domain_policy_; }

  /*!
   * \brief Access interface for property IssuerDomainPolicy.
   * \return Value of IssuerDomainPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerDomainPolicy() noexcept -> Oid& { return issuer_domain_policy_; }

  /*!
   * \brief Read-only access interface for property SubjectDomainPolicy.
   * \return Value of SubjectDomainPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectDomainPolicy() const noexcept -> Oid const& { return subject_domain_policy_; }

  /*!
   * \brief Access interface for property SubjectDomainPolicy.
   * \return Value of SubjectDomainPolicy.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectDomainPolicy() noexcept -> Oid& { return subject_domain_policy_; }

  /*!
   * \brief Method to encode PolicyMapping class.
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
  Oid issuer_domain_policy_;  /*!< IssuerDomainPolicy. */
  Oid subject_domain_policy_; /*!< SubjectDomainPolicy. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_POLICY_MAPPING_H_
