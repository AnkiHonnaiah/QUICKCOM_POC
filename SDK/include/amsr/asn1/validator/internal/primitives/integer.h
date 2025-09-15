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
/*!        \file  amsr/asn1/validator/internal/primitives/integer.h
 *        \brief  Validator for Integer
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_INTEGER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_INTEGER_H_

#include "amsr/asn1/validator/internal/validator.h"

namespace amsr {
namespace asn1 {
namespace validator {

/*!
 * \brief Validator for Integer
 * \tparam I Integer or a context concerning it
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename I>
class Validator<I, context::EnableFor<I, structure::Integer>>
    : public ValidatorBase<I, Validator<I, context::EnableFor<I, structure::Integer>>> {
 public:
  /*!
   * \brief Evaluates the well-formedness of a DER representation under this validator's context.
   * \param der_to_validate The DER-encoded ASN.1 representation to validate
   * \return A validation error or an empty result on success
   */
  static auto EvaluateSpecialWellFormedness(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    return Asn1DERParser::WithoutHeader(der_to_validate)
        .AndThen([](amsr::core::Span<std::uint8_t const> const der_to_validate_no_header) {
          amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
          if (der_to_validate_no_header.empty()) {
            retval.EmplaceError(Asn1Errc::kInvalidContent, "An INTEGER is empty.");
          } else if (AreFirstNineBitsEqual(der_to_validate_no_header)) {
            retval.EmplaceError(Asn1Errc::kInvalidContent, "An INTEGER's first nine bits are equal.");
          } else {
            retval.EmplaceValue();
          }
          return retval;
        });
  }

 private:
  /*!
   * \brief Local well-formedness check: Are the first nine bits equal?
   * \details Every ASN.1 value must have exactly one DER representation, hence, leading zeroes are disallowed.
   *    However, without a leading zero, numbers like \c 0x80 would be interpreted as negative in two's complement.
   *    In such cases, a single zero octet is allowed to indicate positive sign. This explains why the first 9 bits
   *    must not be zero. The same principle applies for negative numbers (no leading Fs), so in conclusion, these
   *    9 bits must not be equal.
   * \param der The DER-encoded Integer data without header to check
   * \return true iff the first nine bits of \c der are equal
   */
  static auto AreFirstNineBitsEqual(amsr::core::Span<std::uint8_t const> const der) noexcept -> bool {
    bool retval{false};
    if (der.size() > 1) {
      static constexpr std::uint8_t const mask{0x80u};
      if ((der[0] == 0x00u) && ((der[1] & mask) != mask)) {
        retval = true;
      } else if ((der[0] == 0xFFu) && ((der[1] & mask) == mask)) {
        retval = true;
      } else {
        // Do nothing
      }
    }
    return retval;
  }
};

}  // namespace validator
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_INTEGER_H_
