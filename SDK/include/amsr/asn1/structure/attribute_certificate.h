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
/*!       \file   amsr/asn1/structure/attribute_certificate.h
 *        \brief  Representation of parsed Attribute Certificate.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/attribute_certificate_info.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief AttributeCertificate Structure as per RFC 5912.
 */
class AttributeCertificate {
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
  auto operator==(AttributeCertificate const& other) const -> bool {
    return std::tie(AcInfo(), SignatureAlgorithm(), SignatureValue()) ==
           std::tie(other.AcInfo(), other.SignatureAlgorithm(), other.SignatureValue());
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
  auto operator!=(AttributeCertificate const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property Attribute Certificate Info
   * \return Value of Attribute Certificate Info
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AcInfo() const noexcept -> AttributeCertificateInfo const& { return acinfo_; }

  /*!
   * \brief Access interface for property Attribute Certificate Info
   * \return Value of Attribute Certificate Info
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AcInfo() noexcept -> AttributeCertificateInfo& { return acinfo_; }

  /*!
   * \brief Read-only access interface for property Signature Algorithm.
   * \return Value of Signature Algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureAlgorithm() const noexcept -> AlgorithmIdentifier const& { return signature_algorithm_; }

  /*!
   * \brief Access interface for property Signature Algorithm.
   * \return Value of Signature Algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureAlgorithm() noexcept -> AlgorithmIdentifier& { return signature_algorithm_; }

  /*!
   * \brief Read-only access interface for property Signature Algorithm.
   * \return Value of Signature Algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureValue() const noexcept -> BitString const& { return signature_value_; }

  /*!
   * \brief Access interface for property Signature Value.
   * \return Value of Signature Value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureValue() noexcept -> BitString& { return signature_value_; }

  /*!
   * \brief Amends the signature_algorithm_ member.
   * \details Set the signature_algorithm_ with the input parameter.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kConstraintCheckFailure  If the value constraint check fails.
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  AttributeCertificateInfo acinfo_;         /*!< Attribute Certificate Info. */
  AlgorithmIdentifier signature_algorithm_; /*!< Signature Algorithm. */
  BitString signature_value_;               /*!< Signature Value. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ATTRIBUTE_CERTIFICATE_H_
