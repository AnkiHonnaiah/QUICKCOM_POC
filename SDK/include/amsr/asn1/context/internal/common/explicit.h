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
/*!        \file  amsr/asn1/context/internal/common/explicit.h
 *        \brief  Special template for EXPLICIT contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_EXPLICIT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_EXPLICIT_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/optional.h"
#include "amsr/asn1/context/internal/common/tagged.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template used for marking a context as EXPLICIT
 * \tparam T The context to mark as EXPLICIT
 * \tparam NewTag The outer tag number (in ASN.1, this is the number in [square brackets])
 * \tparam NewClass The outer tag class, "context-specific" by default as in ASN.1
 */
template <typename T, Tag NewTag, Class NewClass = Class::kContextSpecific>
class Explicit : public Tagged<T, NewTag, NewClass> {
 public:
  /*!
   * \brief The context of the element "inside" the EXPLICIT data member
   * \details EXPLICIT is, in ASN.1, a mixture of abstract and encoding concerns. In its abstract sense, it is not
   *        a "container" with some data item inside, but more of an annotation in the schema. For encoding purposes,
   *        it means that, in addition to the tag provided by the data member, another constructed tag precedes the
   *        tagged data member. For example, an octet string meaning "Universal primitive boolean: false" becomes
   *        "Context-specific constructed data, tag number 0: Universal primitive boolean: false". In its DER-encoded
   *        form, EXPLICIT is realized as a container, while in its abstract notion, it is not.
   */
  using Inner = typename EnsureContextNature<T>::Type;

  /*!
   * \brief Pass the structure type of the inner context
   */
  using StructureType = typename EnsureStructureNature<T>::Type;

  /*!
   * \brief Default constructor
   */
  Explicit() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  Explicit(Explicit const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  Explicit(Explicit&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(Explicit const&) & noexcept -> Explicit& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(Explicit&&) & noexcept -> Explicit& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~Explicit() noexcept override = default;

  /*!
   * \brief Access the context's constructedness flag
   * \return True for EXPLICIT outer element
   */
  static constexpr auto IsConstructed() noexcept -> bool { return true; }
};

/*!
 * \brief Exclude a particular nesting order
 * \details There is no semantic difference in nesting order and respecting optionality inside an explicit validator
 *      base class is more error-prone than respecting explicit tagging in an optional validator base class. Hence,
 *      a choice was made to allow only one nesting order.
 * \tparam T See primary template
 * \tparam NewTag See primary template
 * \tparam NewClass See primary template
 */
template <typename T, Tag NewTag, Class NewClass>
class Explicit<context::Optional<T>, NewTag, NewClass> {
  static_assert((sizeof(T) == 0), "Explicit<Optional> not supported, use Optional<Explicit>.");
};
}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_EXPLICIT_H_
