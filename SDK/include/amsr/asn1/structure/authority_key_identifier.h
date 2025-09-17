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
/*!        \file  amsr/asn1/structure/authority_key_identifier.h
 *        \brief  Representation of parsed Authority Key Identifier.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_AUTHORITY_KEY_IDENTIFIER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_AUTHORITY_KEY_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/general_name.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief Authority Key Identifier Structure.
 */
class AuthorityKeyIdentifier {
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
  auto operator==(AuthorityKeyIdentifier const& other) const -> bool {
    return std::tie(KeyIdentifier(), AuthorityCertIssuer(), AuthorityCertSerialNumber()) ==
           std::tie(other.KeyIdentifier(), other.AuthorityCertIssuer(), other.AuthorityCertSerialNumber());
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
  auto operator!=(AuthorityKeyIdentifier const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access API for Key Identifier.
   * \return Value of property Key Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto KeyIdentifier() const noexcept -> amsr::core::Optional<OctetString> const& { return key_identifier_; }

  /*!
   * \brief Access API for Key Identifier.
   * \return Value of property Key Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto KeyIdentifier() noexcept -> amsr::core::Optional<OctetString>& { return key_identifier_; }

  /*!
   * \brief Read-Only access API for Authority Cert Issuer.
   * \return Value of Authority Cert Issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AuthorityCertIssuer() const noexcept -> amsr::core::Optional<GeneralNames> const& {
    return authority_cert_issuer_;
  }

  /*!
   * \brief Access API for Authority Cert Issuer.
   * \return Value of Authority Cert Issuer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AuthorityCertIssuer() noexcept -> amsr::core::Optional<GeneralNames>& { return authority_cert_issuer_; }

  /*!
   * \brief Read-Only access API for Authority Cert SerialNumber.
   * \return Value of Authority Cert SerialNumber.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AuthorityCertSerialNumber() const noexcept -> amsr::core::Optional<Integer> const& {
    return authority_cert_serial_number_;
  }

  /*!
   * \brief Access API for Authority Cert SerialNumber.
   * \return Value of Authority Cert SerialNumber.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AuthorityCertSerialNumber() noexcept -> amsr::core::Optional<Integer>& { return authority_cert_serial_number_; }

  /*!
   * \brief Method to encode AuthorityKeyIdentifier structure.
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

 private:
  amsr::core::Optional<OctetString> key_identifier_{};           /*!< Key Identifier. */
  amsr::core::Optional<GeneralNames> authority_cert_issuer_{};   /*!< Authority Cert Issuer. */
  amsr::core::Optional<Integer> authority_cert_serial_number_{}; /*!< Authority Cert Serial Number. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_AUTHORITY_KEY_IDENTIFIER_H_
