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
/*!        \file  authority_info_access_builder.h
 *        \brief
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_AUTHORITY_INFO_ACCESS_BUILDER_H
#define AMSR_VECTOR_FS_VAASN1_AUTHORITY_INFO_ACCESS_BUILDER_H

#include "amsr/asn1/builder/common/access_description_builder_state.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for AuthorityInfoAccess as per RFC 5280.
 */
class AuthorityInfoAccessBuilder
    : public SequenceOfBuilderState<AccessDescriptionBuilderState, AuthorityInfoAccessBuilder> {
 public:
  /*!
   * \brief Constructor with constraint.
   * \param[in] parent Handle to parent.
   */
  explicit AuthorityInfoAccessBuilder(Asn1Builder* parent = nullptr) noexcept : SequenceOfBuilderState(parent) {
    SetConstraint(std::make_shared<SizeConstraint<ara::core::Vector<structure::AccessDescription>>>(
        std::make_pair(1, std::numeric_limits<std::uint64_t>::max())));
  }
  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSequence)},
             {1, ElementPresence::kPresent, true}}};
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_AUTHORITY_INFO_ACCESS_BUILDER_H
