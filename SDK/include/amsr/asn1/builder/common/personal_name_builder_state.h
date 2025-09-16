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
/*!       \file   amsr/asn1/builder/common/personal_name_builder_state.h
 *        \brief  Builder for personal name.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_PERSONAL_NAME_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_PERSONAL_NAME_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/set_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/personal_name.h"

namespace amsr {
namespace asn1 {
namespace builder {
/*!
 * \brief Builder for Personal Name.
 */
class PersonalNameBuilderState : public SetBuilderState<structure::PersonalName, PersonalNameBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit PersonalNameBuilderState(Asn1Builder* parent = nullptr) noexcept : SetBuilderState(parent) {
    surname_builder_.SetConstraint(
        std::make_shared<SizeConstraint<structure::PrintableString>>(std::make_pair(1, structure::ub::kSurname)));
    given_name_builder_.SetConstraint(
        std::make_shared<SizeConstraint<structure::PrintableString>>(std::make_pair(1, structure::ub::kGivenName)));
    initials_builder_.SetConstraint(
        std::make_shared<SizeConstraint<structure::PrintableString>>(std::make_pair(1, structure::ub::kInitials)));
    generation_qualifier_builder_.SetConstraint(std::make_shared<SizeConstraint<structure::PrintableString>>(
        std::make_pair(1, structure::ub::kGenerationQualifier)));
  }

  /*!
   * \brief Returns the internal transition graph for this builder.
   * \return The transitions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {{{Asn1Class::kContextSpecific, 0}, {1, ElementPresence::kPresent, true}},
            {{Asn1Class::kContextSpecific, 1}, {2, ElementPresence::kOptional, true}},
            {{Asn1Class::kContextSpecific, 2}, {3, ElementPresence::kOptional, true}},
            {{Asn1Class::kContextSpecific, 3}, {4, ElementPresence::kOptional, true}}};
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
        state = &surname_builder_;
        break;

      case 2:
        state = &given_name_builder_;
        break;

      case 3:
        state = &initials_builder_;
        break;

      case 4:
        state = &generation_qualifier_builder_;
        break;

      default:
        // state initialized with nullptr
        break;
    }
    return state;
  }

 protected:
  /*!
   * \brief Returns the built data.
   * \return A result containing the yielded data or an error if yielding was not possible.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DoYield() const noexcept -> amsr::core::Result<structure::PersonalName> override {
    structure::PersonalName yielded{};

    amsr::core::Result<void> const surname_result{YieldTo(surname_builder_, yielded.Surname())};
    amsr::core::Result<void> const given_name_result{YieldToOptional(given_name_builder_, yielded.GivenName())};
    amsr::core::Result<void> const initials_result{YieldToOptional(initials_builder_, yielded.Initials())};
    amsr::core::Result<void> const generation_qualifier_result{
        YieldToOptional(generation_qualifier_builder_, yielded.GenerationQualifiers())};

    return surname_result.And(given_name_result)
        .And(initials_result)
        .And(generation_qualifier_result)
        .And(amsr::core::Result<structure::PersonalName>::FromValue(std::move(yielded)));
  }

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DoReset() noexcept -> void override {
    surname_builder_.Reset();
    given_name_builder_.Reset();
    initials_builder_.Reset();
    generation_qualifier_builder_.Reset();
  }

 private:
  PrintableStringBuilderState surname_builder_{this};              /*!< Member builder: surname */
  PrintableStringBuilderState given_name_builder_{this};           /*!< Member builder: givenName */
  PrintableStringBuilderState initials_builder_{this};             /*!< Member builder: initials */
  PrintableStringBuilderState generation_qualifier_builder_{this}; /*!< Member builder: generationQualifier*/
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_PERSONAL_NAME_BUILDER_STATE_H_
