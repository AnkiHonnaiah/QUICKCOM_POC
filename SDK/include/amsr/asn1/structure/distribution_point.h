/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/asn1/structure/distribution_point.h
 *        \brief  Representation of parsed distributionpoint.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISTRIBUTION_POINT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISTRIBUTION_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/distribution_point_name.h"
#include "amsr/asn1/structure/general_name.h"
#include "amsr/asn1/structure/reason_flags.h"
#include "amsr/core/optional.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief DistributionPoint structure.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_false_positive
class DistributionPoint {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(DistributionPoint const& other) const -> bool {
    return std::tie(Distributionpoint(), Reasons(), CRLIssuer()) ==
           std::tie(other.Distributionpoint(), other.Reasons(), other.CRLIssuer());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(DistributionPoint const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property distributionPoint.
   * \return Value of distributionPoint.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Distributionpoint() const noexcept -> amsr::core::Optional<DistributionPointName> const& {
    return distribution_point_;
  }

  /*!
   * \brief Access interface for property distributionPoint.
   * \return Value of distributionPoint.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Distributionpoint() noexcept -> amsr::core::Optional<DistributionPointName>& { return distribution_point_; }

  /*!
   * \brief Read-only access interface for property reasons.
   * \return Value of reasons.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Reasons() const noexcept -> amsr::core::Optional<ReasonFlags> const& { return reasons_; }

  /*!
   * \brief Access interface for property reasons.
   * \return Value of reasons.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Reasons() noexcept -> amsr::core::Optional<ReasonFlags>& { return reasons_; }

  /*!
   * \brief Read-only access interface for property cRLIssuer.
   * \return Value of cRLIssuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CRLIssuer() const noexcept -> amsr::core::Optional<GeneralNames> const& { return crl_issuer_; }

  /*!
   * \brief Access interface for property cRLIssuer.
   * \return Value of cRLIssuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CRLIssuer() noexcept -> amsr::core::Optional<GeneralNames>& { return crl_issuer_; }

  /*!
   * \brief Encodes Distribution Point class to encoded ASN.1 data.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kIncompleteInput  Content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  /*!
   * \brief Encodes GeneralNames structure.
   * \param[in] general_names Reference to structure GeneralNames structure.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kIncompleteInput  Content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  static auto EncodeGeneralName(structure::GeneralNames const& general_names) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Encodes Distribution point name variant type RelativeDistinguishedName.
   * \param[in] relative_distinguished Reference to DistributionPointName structure.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kIncompleteInput  Content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  static auto EncodeRelativeDistinguishedName(
      structure::RelativeDistinguishedName const& relative_distinguished) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode distributionPoint, the first member of "DistributionPoint".
   * \param[out] encoded_general_names The place to insert fullName
   * \param[out] encode_distribtn_point The place to insert nameRelativeToCRLIssuer
   * \param[out] sub_structures The place to insert the built structure
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto EncodeDistributionPointMember(ara::core::Vector<std::uint8_t>& encoded_general_names,
                                     ara::core::Vector<std::uint8_t>& encode_distribtn_point,
                                     ara::core::Vector<std::uint8_t>& sub_structures) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  amsr::core::Optional<DistributionPointName> distribution_point_{}; /*!< Parameter distributionPoint. */
  amsr::core::Optional<ReasonFlags> reasons_{};                      /*!< Parameter reasons. */
  amsr::core::Optional<GeneralNames> crl_issuer_{};                  /*!< Parameter cRLIssuer. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISTRIBUTION_POINT_H_
