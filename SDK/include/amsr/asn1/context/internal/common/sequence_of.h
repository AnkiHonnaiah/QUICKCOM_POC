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
/*!        \file  amsr/asn1/context/internal/common/sequence_of.h
 *        \brief  Special template for SEQUENCE OF contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SEQUENCE_OF_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SEQUENCE_OF_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template for contexts referring to a SEQUENCE OF a structure type
 * \tparam T The context determinant of the element type's definition (e.g. Integer for a SEQUENCE OF INTEGER)
 */
template <typename T>
class SequenceOf : public ContextBase<ara::core::Vector<typename EnsureStructureNature<T>::Type>> {
 public:
  /*!
   * \brief The context for the SEQUENCE OF's element
   */
  using ElementContextType = typename EnsureContextNature<T>::Type;

  /*!
   * \brief Pass the structure type of the inner context
   */
  using StructureType = ara::core::Vector<typename EnsureStructureNature<T>::Type>;

  /*!
   * \brief Default constructor
   */
  SequenceOf() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  SequenceOf(SequenceOf const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  SequenceOf(SequenceOf&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(SequenceOf const&) & noexcept -> SequenceOf& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(SequenceOf&&) & noexcept -> SequenceOf& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~SequenceOf() noexcept override = default;

  /*!
   * \brief Access the context's tag number
   * \return By default, the universal tag for sequences
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kSequence); }

  /*!
   * \brief Access the context's constructedness flag
   * \return True for EXPLICIT outer element
   */
  static constexpr auto IsConstructed() noexcept -> bool { return true; }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SEQUENCE_OF_H_
