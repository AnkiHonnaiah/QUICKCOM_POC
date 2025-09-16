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
/*!        \file  amsr/asn1/structure/attribute_certificate_info.h
 *        \brief  Representation of parsed Attribute Certificate info.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_INFO_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/att_cert_issuer.h"
#include "amsr/asn1/structure/att_cert_validity_period.h"
#include "amsr/asn1/structure/attribute.h"
#include "amsr/asn1/structure/holder.h"
#include "extension.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief AttributeCertificateInfo Structure as per RFC 5912.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_structure_has_many_members
class AttributeCertificateInfo {
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
  auto operator==(AttributeCertificateInfo const& other) const -> bool {
    return std::tie(Version(), Holder(), Issuer(), Signature(), SerialNumber(), AttrCertValidityPeriod(), Attributes(),
                    IssuerUniqueId(), Extensions()) ==
           std::tie(other.Version(), other.Holder(), other.Issuer(), other.Signature(), other.SerialNumber(),
                    other.AttrCertValidityPeriod(), other.Attributes(), other.IssuerUniqueId(), other.Extensions());
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
  auto operator!=(AttributeCertificateInfo const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property Version.
   * \return Value of Version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() const noexcept -> Integer const& { return version_; }

  /*!
   * \brief Access interface for property Version.
   * \return Value of Version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() noexcept -> Integer& { return version_; }

  /*!
   * \brief Read-only access interface for property Holder.
   * \return Value of Holder.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Holder() const noexcept -> structure::Holder const& { return holder_; }

  /*!
   * \brief Access interface for property Holder.
   * \return Value of Holder.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Holder() noexcept -> structure::Holder& { return holder_; }

  /*!
   * \brief Read-only access interface for property Issuer.
   * \return Value of Issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Issuer() const noexcept -> structure::AttCertIssuer const& { return issuer_; }

  /*!
   * \brief Access interface for property Issuer.
   * \return Value of Issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Issuer() noexcept -> structure::AttCertIssuer& { return issuer_; }

  /*!
   * \brief Read-only access interface for property Signature.
   * \return Value of Signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() const noexcept -> AlgorithmIdentifier const& { return signature_; }

  /*!
   * \brief Access interface for property Signature.
   * \return Value of Signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() noexcept -> AlgorithmIdentifier& { return signature_; }

  /*!
   * \brief Read-only access interface for property AttrCertValidityPeriod.
   * \return Value of AttrCertValidityPeriod.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AttrCertValidityPeriod() const noexcept -> structure::AttCertValidityPeriod const& {
    return attr_cert_validity_period_;
  }

  /*!
   * \brief Access interface for property AttrCertValidityPeriod.
   * \return Value of AttrCertValidityPeriod.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AttrCertValidityPeriod() noexcept -> structure::AttCertValidityPeriod& { return attr_cert_validity_period_; }

  /*!
   * \brief Read-only access interface for property Serial Number.
   * \return Value of Serial Number.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SerialNumber() const noexcept -> Integer const& { return serial_number_; }

  /*!
   * \brief Access interface for property Serial Number.
   * \return Value of Serial Number.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SerialNumber() noexcept -> Integer& { return serial_number_; }

  /*!
   * \brief Read-only access interface for property Attributes.
   * \return Value of Attributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Attributes() const noexcept -> ara::core::Vector<AttributeSet> const& { return attributes_; }

  /*!
   * \brief Access interface for property Attributes.
   * \return Value of Attributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Attributes() noexcept -> ara::core::Vector<AttributeSet>& { return attributes_; }

  /*!
   * \brief Read-only access interface for property Issuer Unique Id.
   * \return Value of Issuer Unique Id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUniqueId() const noexcept -> amsr::core::Optional<BitString> const& { return issuer_unique_id_; }

  /*!
   * \brief Access interface for property Issuer Unique Id.
   * \return Value of Issuer Unique Id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUniqueId() noexcept -> amsr::core::Optional<BitString>& { return issuer_unique_id_; }

  /*!
   * \brief Read-only access interface for propertyExtensions.
   * \return Value of Extensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Extensions() const noexcept -> amsr::core::Optional<ara::core::Vector<Extension>> const& { return extensions_; }

  /*!
   * \brief Access interface for property Extensions.
   * \return Value of Extensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Extensions() noexcept -> amsr::core::Optional<ara::core::Vector<Extension>>& { return extensions_; }

  /*!
   *\brief Method to encode 'AttributeCertificateInfo' class.
   *\return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  Integer version_;                                               /*!< Version. */
  structure::Holder holder_;                                      /*!< Holder. */
  structure::AttCertIssuer issuer_;                               /*!< Issuer. */
  AlgorithmIdentifier signature_;                                 /*!< Signature. */
  Integer serial_number_;                                         /*!< Serial Number. */
  structure::AttCertValidityPeriod attr_cert_validity_period_;    /*!< AttCertValidityPeriod. */
  ara::core::Vector<AttributeSet> attributes_{};                  /*!< Attributes. */
  amsr::core::Optional<BitString> issuer_unique_id_;              /*!< Issuer Unique ID. */
  amsr::core::Optional<ara::core::Vector<Extension>> extensions_; /*!< Extensions. */

  /*!
   * \brief Method to encode 'AttCertIssuer' data member.
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
  auto EncodeAttCertIssuer() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode the first few, simple members
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
  auto EncodeSimpleMembers(ara::core::Vector<std::uint8_t>&) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode the extensions member
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
  auto EncodeExtensions(ara::core::Vector<std::uint8_t>&) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_INFO_H_
