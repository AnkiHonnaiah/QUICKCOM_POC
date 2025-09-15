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
/*!        \file  amsr/asn1/context/internal/rsassa_pss_params.h
 *        \brief  Context markers for RSASSA-PSS-params
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_RSASSA_PSS_PARAMS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_RSASSA_PSS_PARAMS_H_

#include "amsr/asn1/context/contexts.h"
#include "amsr/asn1/structure/rsassa_pss_params.h"

namespace amsr {
namespace asn1 {
namespace context {
/*!
 * \brief Specialization for RSASSA-PSS-params context
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::RSASsaPssParams> : public ContextBase<structure::RSASsaPssParams> {
 public:
  // TODO: Incomplete, currently contains only what's required for testing DEFAULT values
  /*!
   * \brief Local context determinant
   */
  class SaltLength {};
};

/*!
 * \brief Specialization for RSASSA-PSS-params's saltLength field
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<Context<structure::RSASsaPssParams>::SaltLength> final : public Explicit<structure::Integer, 2> {
 public:
  /*!
   * \brief Access the context's DEFAULT value if present
   * \return The DEFAULT value specified in the corresponding RFC
   */
  static auto DefaultValue() noexcept -> amsr::core::Optional<structure::Integer> { return structure::Integer{20}; }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_INTERNAL_RSASSA_PSS_PARAMS_H_
