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
/*!       \file   amsr/asn1/builder/relative_distinguished_name_builder_state.h
 *        \brief  ASN.1 Builder for relative distinguished name.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_RELATIVE_DISTINGUISHED_NAME_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_RELATIVE_DISTINGUISHED_NAME_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/attribute_structure_builder.h"
#include "amsr/asn1/builder/common/set_of_builder_state.h"
#include "amsr/asn1/structure/name.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for RelativeDistinguishedName as per RFC 5912.
 */
class RelativeDistinguishedNameBuilderState
    : public SetOfBuilderState<SingleAttributeBuilder, RelativeDistinguishedNameBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  // VECTOR NC VectorC++-V12.1.1: MD_VAASN1_VectorC++-V12.1.1-V12.1.1_incomplete_ctor_false_positive
  explicit RelativeDistinguishedNameBuilderState(Asn1Builder* parent = nullptr) noexcept
      : SetOfBuilderState<SingleAttributeBuilder, RelativeDistinguishedNameBuilderState>(parent) {
    // NOTE: As per RFC 2253, RelativeDistinguishedName must contain at least one entry.
    SetConstraint(std::make_shared<SizeConstraint<ara::core::Vector<structure::SingleAttribute>>>(
        std::make_pair(1, std::numeric_limits<std::uint64_t>::max())));
  }

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>>;
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_RELATIVE_DISTINGUISHED_NAME_BUILDER_STATE_H_
