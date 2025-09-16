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
/*!        \file  amsr/asn1/validator/internal/personal_name.h
 *        \brief  Validator for PersonalName
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PERSONAL_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PERSONAL_NAME_H_

#include "amsr/asn1/validator/internal/validator.h"

namespace amsr {
namespace asn1 {
namespace validator {

/*!
 * \brief Validator for PersonalName
 * \tparam PN PersonalName or a context concerning it
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename PN>
class Validator<PN, context::EnableFor<PN, structure::PersonalName>>
    : public ValidatorBase<PN, Validator<PN, context::EnableFor<PN, structure::PersonalName>>> {
 public:
  /*!
   * \brief Evaluates the constraints of this validator's context.
   * \param to_validate The data structure to validate
   * \return A validation error or an empty result on success
   */
  static auto EvaluateValidity(structure::PersonalName const& to_validate) noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{};
    // FIXME: This is an example. Consider turning Constraints into a utility for cases like this.
    //  Think about this well. The only actually used constraints are range, value and size.
    //  Also, consider MIN and MAX for integers. Some framework will be necessary.
    if (to_validate.Surname().empty()) {
      retval.EmplaceError(Asn1Errc::kConstraintCheckFail, "PersonalName: surname is empty.");
    } else if (to_validate.Surname().size() >= structure::ub::kSurname) {
      retval.EmplaceError(Asn1Errc::kConstraintCheckFail, "PersonalName: surname is too long.");
    } else {
      // Valid by default
    }
    if (to_validate.GivenName().has_value()) {
      // TODO: Just an example, would need to implement constraints here or use the Constraint framework.
    }
    return retval;
  }
};

}  // namespace validator
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_PERSONAL_NAME_H_
