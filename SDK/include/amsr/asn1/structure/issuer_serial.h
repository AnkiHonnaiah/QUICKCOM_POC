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
/*!        \file  amsr/asn1/structure/issuer_serial.h
 *        \brief  Representation of parsed Issuer Serial.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ISSUER_SERIAL_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ISSUER_SERIAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/general_name.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Issuer Serial structure.
 */
class IssuerSerial {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(IssuerSerial const& other) const -> bool {
    return std::tie(Issuer(), Serial(), IssuerUID()) == std::tie(other.Issuer(), other.Serial(), other.IssuerUID());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(IssuerSerial const& other) const -> bool { return !(*this == other); }
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
  auto Issuer() const noexcept -> structure::GeneralNames const& { return issuer_; }

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
  auto Issuer() noexcept -> structure::GeneralNames& { return issuer_; }

  /*!
   * \brief Read-only access interface for property Serial.
   * \return Value of Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Serial() const noexcept -> structure::Integer const& { return serial_; }

  /*!
   * \brief Access interface for property Serial.
   * \return Value of Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Serial() noexcept -> structure::Integer& { return serial_; }

  /*!
   * \brief Read-only access interface for property IssuerUID.
   * \return Value of IssuerUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUID() const noexcept -> amsr::core::Optional<structure::BitString> const& { return issuer_uid_; }

  /*!
   * \brief Access interface for property IssuerUID.
   * \return Value of IssuerUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IssuerUID() noexcept -> amsr::core::Optional<structure::BitString>& { return issuer_uid_; }

  /*!
   * \brief Method to encode IssuerSerial class.
   * \param[in] context_specific If context-specific \c true, otherwise \c false.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode(bool context_specific = false) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  structure::GeneralNames issuer_{};                        /*!<  Issuer. */
  structure::Integer serial_{};                             /*!<  Serial. */
  amsr::core::Optional<structure::BitString> issuer_uid_{}; /*!<  issuerUID. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ISSUER_SERIAL_H_
