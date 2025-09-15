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
/*!       \file   amsr/asn1/builder/common/extensions_builder_state.h
 *        \brief  ASN.1 Builder state for Extensions.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSIONS_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSIONS_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/extension_builder_state.h"
#include "amsr/asn1/builder/common/sequence_of_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for Extensions.
 */
class ExtensionsBuilderState : public SequenceOfBuilderState<ExtensionBuilderState, ExtensionsBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit ExtensionsBuilderState(Asn1Builder* parent = nullptr) noexcept
      : SequenceOfBuilderState<ExtensionBuilderState, ExtensionsBuilderState>{parent} {
    SetConstraint(std::make_shared<SizeConstraint<ara::core::Vector<structure::Extension>>>(
        std::make_pair(1, std::numeric_limits<std::uint64_t>::max())));
  }

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSequence)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSIONS_BUILDER_STATE_H_
