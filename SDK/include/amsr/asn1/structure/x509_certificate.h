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
/*!        \file  amsr/asn1/structure/x509_certificate.h
 *        \brief  Representation of parsed X.509 certificate as per RFC 5280.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_X509_CERTIFICATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_X509_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/tbs_certificate.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief X.509 Certificate structure as per RFC 5280.
 */
class X509Certificate {
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
  auto operator==(X509Certificate const& other) const -> bool {
    return std::tie(GetTBSCertificate(), GetSignatureAlgorithm(), GetSignatureValue()) ==
           std::tie(other.GetTBSCertificate(), other.GetSignatureAlgorithm(), other.GetSignatureValue());
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
  auto operator!=(X509Certificate const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property TBSCertificate.
   * \return Value of TBSCertificate.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetTBSCertificate() -> structure::TBSCertificate& { return tbs_certificate_; }

  /*!
   * \brief Access interface for property version.
   * \return Value of version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetTBSCertificate() const -> structure::TBSCertificate const& { return tbs_certificate_; }

  /*!
   * \brief Read-only access interface for property signatureAlgorithm.
   * \return Value of signatureAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetSignatureAlgorithm() -> structure::AlgorithmIdentifier& { return signature_algorithm_; }

  /*!
   * \brief Access interface for property signatureAlgorithm.
   * \return Value of signatureAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetSignatureAlgorithm() const -> structure::AlgorithmIdentifier const& { return signature_algorithm_; }

  /*!
   * \brief Read-only access interface for property signatureValue.
   * \return Value of signatureValue.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetSignatureValue() -> structure::BitString& { return signature_value_; }

  /*!
   * \brief Access interface for property signatureValue.
   * \return Value of signatureValue.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetSignatureValue() const -> structure::BitString const& { return signature_value_; }

  /*!
   * \brief Method to encode X509Certificate class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  structure::TBSCertificate tbs_certificate_;          /*!< TBS Certificate. */
  structure::AlgorithmIdentifier signature_algorithm_; /*!< Signature algorithm. */
  structure::BitString signature_value_;               /*!< Signature value */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_X509_CERTIFICATE_H_
