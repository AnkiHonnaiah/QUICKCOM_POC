/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *  -------------------------------------------------------------------------------------------------------------------
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/asn1/structure/v2_form.h
 *        \brief  Representation of parsed V2Form.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_V2_FORM_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_V2_FORM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/general_name.h"
#include "amsr/asn1/structure/issuer_serial.h"
#include "amsr/asn1/structure/object_digest_info.h"
#include "amsr/core/optional.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief V2 Form Structure.
 */
class V2Form {
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
  auto operator==(V2Form const& other) const -> bool {
    return std::tie(IssuerName(), BaseCertificateID(), ObjectDigestInfo()) ==
           std::tie(other.IssuerName(), other.BaseCertificateID(), other.ObjectDigestInfo());
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
  auto operator!=(V2Form const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property IssuerName.
   * \return Value of IssuerName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerName() const noexcept -> amsr::core::Optional<structure::GeneralNames> const& { return issuer_name_; }

  /*!
   * \brief Access interface for property IssuerName.
   * \return Value of IssuerName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerName() noexcept -> amsr::core::Optional<structure::GeneralNames>& { return issuer_name_; }

  /*!
   * \brief Read-only access interface for property Base Certificate ID.
   * \return Value of Base Certificate ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BaseCertificateID() const noexcept -> amsr::core::Optional<IssuerSerial> const& { return base_certificate_id_; }

  /*!
   * \brief Access interface for property Base Certificate ID.
   * \return Value of Base Certificate ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BaseCertificateID() noexcept -> amsr::core::Optional<IssuerSerial>& { return base_certificate_id_; }

  /*!
   * \brief Read-only access interface for Object Digest Info.
   * \return Value of Object Digest Info.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ObjectDigestInfo() const noexcept -> amsr::core::Optional<structure::ObjectDigestInfo> const& {
    return object_digest_info_;
  }

  /*!
   * \brief Access interface for property Object Digest Info.
   * \return Value of Object Digest Info.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ObjectDigestInfo() noexcept -> amsr::core::Optional<structure::ObjectDigestInfo>& { return object_digest_info_; }

  /*!
   * \brief Method to encode V2Form class.
   * \param[in] context_specific true if ContextSpecific.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode(bool context_specific) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode V2Form class with a universal header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>> { return Encode(false); }

 private:
  amsr::core::Optional<GeneralNames> issuer_name_{};                       /*!< Issuer Name.*/
  amsr::core::Optional<IssuerSerial> base_certificate_id_{};               /*!< BaseCertificateID.*/
  amsr::core::Optional<structure::ObjectDigestInfo> object_digest_info_{}; /*!< ObjectDigestInfo.*/
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_V2_FORM_H_
