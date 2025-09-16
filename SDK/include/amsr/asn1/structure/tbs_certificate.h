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
/*!        \file  amsr/asn1/structure/tbs_certificate.h
 *        \brief  Representation of parsed TBS certificate.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_TBS_CERTIFICATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_TBS_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/extension.h"
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/subject_public_key_info.h"
#include "amsr/asn1/structure/validity.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief TBS Certificate structure.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_structure_has_many_members
class TBSCertificate {
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
  auto operator==(TBSCertificate const& other) const -> bool {
    return std::tie(Version(), SerialNumber(), Signature(), Issuer(), ValidityInformation(), Subject(),
                    SubjectPublicKeyInformation(), IssuerUniqueID(), SubjectUniqueID(), Extensions()) ==
           std::tie(other.Version(), other.SerialNumber(), other.Signature(), other.Issuer(),
                    other.ValidityInformation(), other.Subject(), other.SubjectPublicKeyInformation(),
                    other.IssuerUniqueID(), other.SubjectUniqueID(), other.Extensions());
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
  auto operator!=(TBSCertificate const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property version.
   * \return Value of version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() const noexcept -> Integer const& { return version_; }

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
  auto Version() noexcept -> Integer& { return version_; }

  /*!
   * \brief Read-only access interface for property serial number.
   * \return Value of serial number.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SerialNumber() const noexcept -> Integer const& { return serial_number_; }

  /*!
   * \brief Access interface for property serialnumber.
   * \return Value of serialnumber.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SerialNumber() noexcept -> Integer& { return serial_number_; }

  /*!
   * \brief Read-only access interface for property signature.
   * \return Value of signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() const noexcept -> AlgorithmIdentifier const& { return signature_; }

  /*!
   * \brief Access interface for property signature.
   * \return Value of signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() noexcept -> AlgorithmIdentifier& { return signature_; }

  /*!
   * \brief Read-only access interface for Issuer.
   * \return Value of issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Issuer() const noexcept -> Name const& { return issuer_; }

  /*!
   * \brief Access interface for property Issuer.
   * \return Value of issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Issuer() noexcept -> Name& { return issuer_; }

  /*!
   * \brief Read-only access interface for property validity.
   * \return Value of validity.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ValidityInformation() const noexcept -> Validity const& { return validity_; }

  /*!
   * \brief Access interface for property validity.
   * \return Value of validity.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ValidityInformation() noexcept -> Validity& { return validity_; }

  /*!
   * \brief Read-only access interface for Subject.
   * \return Value of subject.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Subject() const noexcept -> Name const& { return subject_; }

  /*!
   * \brief Access interface for property Subject.
   * \return Value of subject.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Subject() noexcept -> Name& { return subject_; }

  /*!
   * \brief Read-only access interface for property SubjectPublicKeyInfo.
   * \return Value of SubjectPublicKeyInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPublicKeyInformation() const noexcept -> SubjectPublicKeyInfo const& { return subjectpublickeyinfo_; }

  /*!
   * \brief Access interface for property SubjectPublicKeyInfo.
   * \return Value of SubjectPublicKeyInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPublicKeyInformation() noexcept -> SubjectPublicKeyInfo& { return subjectpublickeyinfo_; }

  /*!
   * \brief Read-only access interface for property issuer unique id.
   * \return Value of issuer unique id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUniqueID() const noexcept -> amsr::core::Optional<BitString> const& { return issuer_unique_id_; }

  /*!
   * \brief Access interface for property issuer unique id.
   * \return Value of issuer unique id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUniqueID() noexcept -> amsr::core::Optional<BitString>& { return issuer_unique_id_; }

  /*!
   * \brief Read-only access interface for property subject unique id.
   * \return Value of subject unique id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectUniqueID() const noexcept -> amsr::core::Optional<BitString> const& { return subject_unique_id_; }
  /*!
   * \brief Access interface for property subject unique id.
   * \return Value of subject unique id.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectUniqueID() noexcept -> amsr::core::Optional<BitString>& { return subject_unique_id_; }

  /*!
   * \brief Read-only access interface for property extensions.
   * \return Value of extensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Extensions() const noexcept -> amsr::core::Optional<ara::core::Vector<Extension>> const& { return extensions_; }

  /*!
   * \brief Access interface for property extensions.
   * \return Value of extensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Extensions() noexcept -> amsr::core::Optional<ara::core::Vector<Extension>>& { return extensions_; }

  /*!
   * \brief Method to encode TBSCertificate class.
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
  Integer version_{0x00};                                         /*!< Version. */
  Integer serial_number_;                                         /*!< Serial number. */
  AlgorithmIdentifier signature_;                                 /*!< Signature. */
  Name issuer_;                                                   /*!< Issuer. */
  Validity validity_;                                             /*!< ValidityInformation. */
  Name subject_;                                                  /*!< Subject. */
  SubjectPublicKeyInfo subjectpublickeyinfo_;                     /*!< SubjectPublicKeyInformation. */
  amsr::core::Optional<BitString> issuer_unique_id_;              /*!< IssuerUniqueID. */
  amsr::core::Optional<BitString> subject_unique_id_;             /*!< SubjectUniqueID. */
  amsr::core::Optional<ara::core::Vector<Extension>> extensions_; /*!< Extensions. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_TBS_CERTIFICATE_H_
