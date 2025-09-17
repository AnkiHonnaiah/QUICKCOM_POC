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
/*!        \file  amsr/asn1/context/internal/common/choice.h
 *        \brief  Special template for CHOICE contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CHOICE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CHOICE_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/nature_metafunctions.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template for contexts referring to a CHOICE
 * \tparam Ts The context determinants from which to choose
 */
template <typename... Ts>
class Choice : public ContextBase<amsr::core::Variant<Ts...>> {
 public:
  /*!
   * \brief The decoded structure type is a variant over contexts
   */
  using StructureType = amsr::core::Variant<typename EnsureStructureNature<Ts>::Type...>;

  /*!
   * \brief Default constructor
   */
  Choice() noexcept = delete;
  /*!
   * \brief Deleted copy constructor
   */
  Choice(Choice const&) noexcept = delete;
  /*!
   * \brief Deleted move constructor
   */
  Choice(Choice&&) noexcept = delete;
  /*!
   * \brief Deleted copy assignment operator
   */
  auto operator=(Choice const&) & noexcept -> Choice& = delete;
  /*!
   * \brief Deleted move assignment operator
   */
  auto operator=(Choice&&) & noexcept -> Choice& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~Choice() noexcept override = default;

  /*!
   * \brief Access the context's tag number
   * \return Per default, CHOICEs don't own a tag.
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kUndefined); }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CHOICE_H_
