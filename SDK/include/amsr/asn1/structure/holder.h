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
/*!        \file  amsr/asn1/structure/holder.h
 *        \brief  Representation of parsed holder.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_HOLDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_HOLDER_H_

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
 * \brief Holder structure.
 */
class Holder {
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
  auto operator==(Holder const& other) const -> bool {
    return std::tie(BaseCertificateID(), EntityName(), ObjectDigestInfo()) ==
           std::tie(other.BaseCertificateID(), other.EntityName(), other.ObjectDigestInfo());
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
  auto operator!=(Holder const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property baseCertificateID.
   * \return Value of baseCertificateID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BaseCertificateID() const noexcept -> amsr::core::Optional<IssuerSerial> const& { return base_certificate_id_; }

  /*!
   * \brief Access interface for property baseCertificateID.
   * \return Value of baseCertificateID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BaseCertificateID() noexcept -> amsr::core::Optional<IssuerSerial>& { return base_certificate_id_; }

  /*!
   * \brief Read-only access interface for property entityName.
   * \return Value of entityName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto EntityName() const noexcept -> amsr::core::Optional<GeneralNames> const& { return entity_name_; }

  /*!
   * \brief Access interface for property entityName.
   * \return Value of entityName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto EntityName() noexcept -> amsr::core::Optional<GeneralNames>& { return entity_name_; }

  /*!
   * \brief Read-only access interface for property objectDigestInfo.
   * \return Value of objectDigestInfo.
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
   * \brief Access interface for property objectDigestInfo.
   * \return Value of objectDigestInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ObjectDigestInfo() noexcept -> amsr::core::Optional<structure::ObjectDigestInfo>& { return object_digest_info_; }

  /*!
   * \brief Method to encode Holder class.
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
  amsr::core::Optional<IssuerSerial> base_certificate_id_{};               /*!< baseCertificateID. */
  amsr::core::Optional<GeneralNames> entity_name_{};                       /*!< entityName. */
  amsr::core::Optional<structure::ObjectDigestInfo> object_digest_info_{}; /*!< objectDigestInfo. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_HOLDER_H_
