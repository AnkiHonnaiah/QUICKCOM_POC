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
/*!        \file  amsr/asn1/context/internal/personal_name.h
 *        \brief  Context markers for PersonalName
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_PERSONAL_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_PERSONAL_NAME_H_

#include "amsr/asn1/context/contexts.h"
#include "amsr/asn1/structure/personal_name.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief Specialization for PersonalName context
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::PersonalName> : public ContextBase<structure::PersonalName> {
 public:
  /*!
   * \brief Access the context's tag number
   * \return This context's tag number
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kSet); }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_PERSONAL_NAME_H_
