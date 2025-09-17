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
/*!        \file  amsr/asn1/context/internal/primitives.h
 *        \brief  Context markers for ASN.1 primitives
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_PRIMITIVES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_PRIMITIVES_H_

#include "amsr/asn1/context/contexts.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief Primitive context for BOOLEAN
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::Boolean> : public ContextBase<structure::Boolean> {
 public:
  /*!
   * \brief Access the context's tag number
   * \return This context's tag number
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kBoolean); }

  /*!
   * \brief Access the context's constructedness flag
   * \return False, as this is a primitive
   */
  static constexpr auto IsConstructed() noexcept -> bool { return false; }
};

/*!
 * \brief Primitive context for INTEGER
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::Integer> : public ContextBase<structure::Integer> {
 public:
  /*!
   * \brief Access the context's tag number
   * \return This context's tag number
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kInteger); }

  /*!
   * \brief Access the context's constructedness flag
   * \return False, as this is a primitive
   */
  static constexpr auto IsConstructed() noexcept -> bool { return false; }
};

/*!
 * \brief Primitive context for OCTET STRING
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::OctetString> : public ContextBase<structure::OctetString> {
 public:
  /*!
   * \brief Access the context's tag number
   * \return This context's tag number
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kOctetString); }

  /*!
   * \brief Access the context's constructedness flag
   * \return False, as this is a primitive
   */
  static constexpr auto IsConstructed() noexcept -> bool { return false; }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_PRIMITIVES_H_
