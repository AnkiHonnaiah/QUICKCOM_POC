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
/*!       \file   amsr/asn1/builder/common/access_description_builder_state.h
 *        \brief  Access Description builder.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ACCESS_DESCRIPTION_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ACCESS_DESCRIPTION_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/general_name_builder_state.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/access_description.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for AccessDescription as per RFC 5280.
 */
class AccessDescriptionBuilderState
    : public SequenceBuilderState<structure::AccessDescription, AccessDescriptionBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using SequenceBuilderState::SequenceBuilderState;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kObjectIdentifier)},
             {1, ElementPresence::kPresent, false}},
            {kChoiceInput, {2, ElementPresence::kPresent, true}}};
  }

  /*!
   * \brief Passes the subbuilder responsible for input identified by \c element.
   * \param[in] element The internal element identifier for which to return a builder.
   * \return Pointer to the builder that builds the following data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* override {
    Asn1Builder* state{nullptr};

    switch (element.id) {
      case 1:
        state = &access_method_builder_;
        break;

      case 2:
        state = &access_location_builder_;
        break;

      default:
        // Do nothing.
        break;
    }
    return state;
  }

 protected:
  /*!
   * \brief Returns the built data.
   * \return A result containing the yielded data or an error if yielding was not possible.
   */
  auto DoYield() const noexcept -> amsr::core::Result<structure::AccessDescription> override {
    structure::AccessDescription yielded{};
    amsr::core::Result<void> const access_method_result{YieldTo(access_method_builder_, yielded.AccessMethod())};
    amsr::core::Result<void> const access_location_result{YieldTo(access_location_builder_, yielded.AccessLocation())};

    return access_method_result.And(access_location_result)
        .And(amsr::core::Result<structure::AccessDescription>::FromValue(std::move(yielded)));
  }

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   */
  auto DoReset() noexcept -> void override {
    access_method_builder_.Reset();
    access_location_builder_.Reset();
  }

 private:
  OidBuilderState access_method_builder_{this};           /*!< Member builder: accessMethod */
  GeneralNameBuilderState access_location_builder_{this}; /*!< Member builder: accessLocation */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ACCESS_DESCRIPTION_BUILDER_STATE_H_
