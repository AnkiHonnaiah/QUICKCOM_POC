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
/*!        \file  response_bytes.h
 *        \brief  Representation of parsed ResponseBytes
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_H
#define AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_H

#include "asn1_primitive_types.h"
#include "basic_ocsp_response.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {
namespace structure {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief ResponseBytes Structure (RFC 6960).
 */
class ResponseBytes {
 public:
  /*!
   * \brief Unified definition of what response types are supported.
   */
  using ResponseVariant = amsr::core::Variant<BasicOcspResponse, RawData>;

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
  auto operator==(ResponseBytes const& other) const -> bool {
    return std::tie(ResponseType(), Response()) == std::tie(other.ResponseType(), other.Response());
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
  auto operator!=(ResponseBytes const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property responseType.
   * \return Value of responseType.
   */
  auto ResponseType() const noexcept -> Oid const& { return response_type_; }
  /*!
   * \brief Access interface for property responseType.
   * \return Value of responseType.
   */
  auto ResponseType() noexcept -> Oid& { return response_type_; }
  /*!
   * \brief Read-only access interface for property response.
   * \return Value of response.
   */
  auto Response() const noexcept -> amsr::core::Variant<BasicOcspResponse, RawData> const& { return response_; }
  /*!
   * \brief Access interface for property response.
   * \return Value of response.
   */
  auto Response() noexcept -> amsr::core::Variant<BasicOcspResponse, RawData>& { return response_; }
  /*!
   * \brief Method to encode ResponseBytes class.
   * \return encoded data.
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

  /*!
   * \brief OID for pkix-ocsp-basic responseType.
   * \details {iso(1) identified-organization(3) dod(6) internet(1) security(5) mechanisms(5) pkix(7) ad(48)
   * id-ad-ocsp(1) id-pkix-ocsp-basic(1)}
   */
  static constexpr amsr::core::StringView kBasicOcspResponse{"1.3.6.1.5.5.7.48.1.1"_sv};

 private:
  // FIXME: This is redundant in Extension and meant for recursive parsing.
  /*!
   * \brief Checks if a raw data structure starts with a universal, constructed OCTET STRING header.
   * \param raw The structure to check
   * \return Whether the structure starts with \c 0x04
   */
  static auto HasOctetStringHeader(RawData const& raw) noexcept -> bool;

  Oid response_type_; /*!< Member: responseType */
  /*!
   * \brief DER-encoded content of possibly varying response types.
   * \details The response field is an OCTET STRING representing a DER-encoded response. RFC 6960 defines one response
   * type called "BasicOCSPResponse" but leaves other response types open for extension. This builder recursively parses
   * the response and interprets it in terms of known response types. The responseType field serves as a discriminator
   * attribute.
   */
  ResponseVariant response_; /*!< Member: response */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_H
