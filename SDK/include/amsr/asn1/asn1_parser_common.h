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
/*!       \file   asn1_parser_common.h
 *        \brief  Helper functions for ASN.1.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_COMMON_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_COMMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/span.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief Alias definition.
 */
using Real = double;

/*!
 * \brief Mantissa value and used bits.
 */
struct Mantissa {
  /*!
   * \brief Mantissa value.
   */
  std::uint64_t const value;
  /*!
   * \brief Used bits in value.
   */
  std::uint8_t const used_bits;
};

/*!
 * \brief Holds sign, mantissa and exponent value.
 */
struct RealValue {
  /*!
   * \brief Hold sign value.
   */
  bool const is_negative;
  /*!
   * \brief Hold mantissa value.
   */
  Mantissa const mantissa;
  /*!
   * \brief Hold exponent value.
   */
  std::int32_t const exponent;
};

/*!
 * \brief Create real based on special values.
 * \details Create real based on special values.
 * \param[in] input Stream of ASN.1 real type.
 * \return Real number result
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
amsr::core::Result<Real> DecodeSpecialRealValues(amsr::core::Span<std::uint8_t const> const input) noexcept;

/*!
 * \brief Creates real number from input.
 * \details Creates real type from sign, mantissa and exponent.
 * \param[in] sme Represents sign, mantissa and exponent.
 * \return Real number result
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
amsr::core::Result<Real> SignMantisaExpToReal(RealValue const& sme) noexcept;

/*!
 * \brief Converts byte stream representing ASN.1 real to real type.
 * \details Converts byte stream representing ASN.1 real to real type.
 * \param[in] input Stream of ASN.1 real type.
 * \return Real number result
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
amsr::core::Result<Real> Asn1BinaryToReal(amsr::core::Span<std::uint8_t const> const input) noexcept;

/*!
 * \brief Converts decimal string to real.
 * \details Creates a real type from NR3 from string.
 * \param[in] input Real number represented in NR3 form.
 * \return Real number result
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
amsr::core::Result<Real> NR3FromToReal(amsr::core::Span<std::uint8_t const> const input) noexcept;

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_COMMON_H_
