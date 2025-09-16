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
/*!        \file  amsr/asn1/context/internal/common/tagged.h
 *        \brief  Special template for tagged contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGGED_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGGED_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/nature_metafunctions.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template used for re-tagging a context
 * \details This is compatible with both context types and structure types. This class inherits from a class
 *          evaluated by a metafunction which, for structure types, will wrap a \c Context<T> around \c T which
 *          guarantees the tagged context to inherit default values from \c Context<T> which may be explicitly
 *          specialized, for example in the case of primitives.
 * \tparam T The context or determinant to re-tag
 * \tparam NewTag The new tag to assign
 * \tparam NewClass The new tag class to assign. As with ASN.1, the default here is "context-specific".
 */
template <typename T, Tag NewTag, Class NewClass = Class::kContextSpecific>
class Tagged : public EnsureContextNature<T>::Type {
 public:
  /*!
   * \brief Recursively searched structure type
   * \details Similarly to the metafunction in the base class, this iterates through a nested metacontext and finds
   *        the "lowest T". For instance, a \c Tagged<Tagged<Boolean>> will have \c Boolean for StructureType.
   */
  using StructureType = typename EnsureStructureNature<T>::Type;

  /*!
   * \brief Default constructor
   */
  Tagged() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  Tagged(Tagged const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  Tagged(Tagged&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(Tagged const&) & noexcept -> Tagged& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(Tagged&&) & noexcept -> Tagged& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~Tagged() noexcept override = default;

  /*!
   * \brief Access the context's tag class (universal, context-specific, application, private)
   * \return The tag passed in the template argument for \c NewClass
   */
  static constexpr auto TagClass() noexcept -> Class { return NewClass; }

  /*!
   * \brief Access the context's tag number
   * \return The tag passed in the template argument for \c NewTag
   */
  static constexpr auto TagNumber() noexcept -> Tag { return NewTag; }
};

/*!
 * \brief Exclude a particular tagging mode
 * \details Every ASN.1 type can be represented in a UNIVERSALly tagged manner. There is not ever the need for
 *      expressly (neither EXPLICITly nor IMPLICITly) tagging anything as UNIVERSAL. Moreover, the ability to do so
 *      falsely implies that CHOICEs can be universally tagged. Their components can, but not the CHOICEs themselves.
 *      Most likely, context-specific tagging is intended.
 * \tparam T See primary template
 * \tparam NewTag See primary template
 */
template <typename T, Tag NewTag>
class Tagged<T, NewTag, Class::kUniversal> {
  static_assert((sizeof(T) == 0), "Cannot re-tag a context as UNIVERSAL.");
};

/*!
 * \brief Exclude a particular tagging mode
 * \details IMPLICIT Re-tagging an already re-tagged context ignores the inner re-tagging and is never required.
 *      The intention behind this is either EXPLICIT tagging or simply using the outer re-tagging.
 * \tparam T See primary template
 * \tparam NewTag See primary template
 * \tparam NewClass See primary template
 * \tparam InnerTag See primary template as \c NewTag
 * \tparam InnerClass See primary template as \c NewClass
 */
template <typename T, Tag NewTag, Class NewClass, Tag InnerTag, Class InnerClass>
class Tagged<Tagged<T, InnerTag, InnerClass>, NewTag, NewClass> {
  static_assert((sizeof(T) == 0),
                "Tagged<Tagged<T, Tag0, Class0>, Tag1, Class1> ignores Tag0 and Class0. Use unnested Tagged<T, Tag1, "
                "Class1> or Explicit, depending on what was intended.");
};
}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGGED_H_
