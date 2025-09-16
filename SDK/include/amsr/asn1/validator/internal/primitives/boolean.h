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
/*!        \file  amsr/asn1/validator/internal/primitives/boolean.h
 *        \brief  Validator for Boolean
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_BOOLEAN_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_BOOLEAN_H_

#include "amsr/asn1/validator/internal/validator.h"

namespace amsr {
namespace asn1 {
namespace validator {

/*!
 * \brief Validator for Boolean
 * \tparam B Boolean or a context concerning it
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename B>
class Validator<B, context::EnableFor<B, structure::Boolean>>
    : public ValidatorBase<B, Validator<B, context::EnableFor<B, structure::Boolean>>> {
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
          if (der_to_validate_no_header.size() != 1) {
            retval.EmplaceError(Asn1Errc::kInvalidContent, "A BOOLEAN value has an invalid length.");
          } else if ((der_to_validate_no_header[0] != 0x00u) && (der_to_validate_no_header[0] != 0xFFu)) {
            retval.EmplaceError(Asn1Errc::kInvalidContent,
                                "Encountered a BOOLEAN value that is neither 0 nor 255. This is forbidden in DER.");
          } else {
            retval.EmplaceValue();
          }
          return retval;
        });
  }
};

}  // namespace validator
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PRIMITIVES_BOOLEAN_H_
