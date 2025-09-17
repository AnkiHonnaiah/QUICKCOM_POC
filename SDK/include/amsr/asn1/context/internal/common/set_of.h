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
/*!        \file  amsr/asn1/context/internal/common/set_of.h
 *        \brief  Special template for SET OF contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SET_OF_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SET_OF_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template for contexts referring to a SET OF a structure type
 * \tparam T The context determinant of the element type's definition (e.g. Integer for a SET OF INTEGER)
 */
template <typename T>
class SetOf : public ContextBase<ara::core::Vector<typename EnsureStructureNature<T>::Type>> {
 public:
  /*!
   * \brief The context for the SET OF's element
   */
  using ElementContextType = typename EnsureContextNature<T>::Type;

  /*!
   * \brief Pass the structure type of the inner context
   */
  using StructureType = ara::core::Vector<typename EnsureStructureNature<T>::Type>;

  /*!
   * \brief Default constructor
   */
  SetOf() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  SetOf(SetOf const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  SetOf(SetOf&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(SetOf const&) & noexcept -> SetOf& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(SetOf&&) & noexcept -> SetOf& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~SetOf() noexcept override = default;

  /*!
   * \brief Access the context's tag number
   * \return By default, the universal tag for sets
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kSet); }

  /*!
   * \brief Access the context's constructedness flag
   * \return True for SET OF
   */
  static constexpr auto IsConstructed() noexcept -> bool { return true; }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_SET_OF_H_
