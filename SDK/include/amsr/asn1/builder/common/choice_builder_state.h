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
/*!       \file   amsr/asn1/builder/common/choice_builder_state.h
 *        \brief  ASN.1 Builder state for CHOICE type.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CHOICE_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CHOICE_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/asn1/builder/constructed_type_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for CHOICE types.
 * \tparam YieldedType The type yielded by this choice. This will probably be a variant.
 * \tparam DerivedThis Provider for Transitions (CRTP), input the subclass itself here.
 */
template <class YieldedType, class DerivedThis>
class ChoiceBuilderState : public ConstructedTypeBuilderState<YieldedType> {
 public:
  // FIXME: This is dummy. All builders are to be constraint-compatible, but for CHOICEs, the currently built internal
  //  ID was chosen to check against. This has no discernible meaning.
  /*!
   * \brief Dummy type for constraint.
   */
  using CPType = std::uint64_t;

  /*!
   * \brief Constructor.
   * \param[in] parent Handle to parent.
   * \param[in] consumes_tag True if the CHOICE builder consumes the tag it was called with. False by default. Use this
   * for context-specifically/non-universally tagged CHOICEs as they deviate from universal CHOICEs that have no tag.
   */
  explicit ChoiceBuilderState(Asn1Builder* parent = nullptr, bool consumes_tag = false) noexcept
      : ConstructedTypeBuilderState<YieldedType>(
            BuildTransitionTable(DerivedThis::GetTransitions()), parent,
            consumes_tag ? ConstructedTypeBuilderState<YieldedType>::kInitId : kChoiceInitId),
        constraint_{} {}

  /*!
   * \brief Sets constraint.
   * \param[in] constraint Constraint to be applied.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void SetConstraint(std::shared_ptr<Constraint<CPType>> constraint) noexcept { constraint_ = std::move(constraint); }

  /*!
   * \brief Checks if constraints are satisfied.
   * \return \c true if valid, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CheckConstraints() const noexcept -> bool override {
    bool accepted{true};
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAASN1_AutosarC++17_10-M0.1.2_dead_branch_false_positive
    if (constraint_) {
      accepted = constraint_->IsAccepted(this->CurrentElement().id);
    }
    return accepted;
  }

 protected:
  /*!
   * \brief Initial state element for CHOICE
   * \details This exists to prevent MoveToNextState to consume a tag since there is no universal CHOICE tag.
   */
  static constexpr ElementIdentifier const kChoiceInitId{0U, ElementPresence::kOptional, false};

 private:
  /*!
   * \brief Creates transition table.
   * \param[in] transitions Pairs of input and target states.
   * \return Set of transitions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto BuildTransitionTable(ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> const& transitions)
      -> std::set<Transition> {
    ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> mutable_states{transitions};
    std::set<Transition> transition_table;
    ElementIdentifier const init{ConstructedTypeBuilderState<YieldedType>::kAnyElementId};
    for (std::pair<ElementInput, ElementIdentifier>& ti : mutable_states) {
      ti.second.is_final = true;
      ti.second.presence_flag = ElementPresence::kOptional;
      static_cast<void>(transition_table.insert(Transition{init, ti.first, ti.second}));
    }
    return transition_table;
  }
  /*! Constraint for CHOICE */
  std::shared_ptr<Constraint<CPType>> constraint_{};

  /*!
   * \brief Receives notification from child upon exiting final state.
   * \return Always success.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PreAccept() noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromValue()};
    this->CurrentElement() = ConstructedTypeBuilderState<YieldedType>::kCompletedId;
    if (ChoiceBuilderState::kInitId.id == this->InitElement().id) {
      // Choice has constructed state, don't forward.
    } else if (Asn1Builder* const parent{this->GetParent()}) {
      result = parent->Accept();
    } else {
      // Do Nothing.
    }
    return result;
  };
};

/*!
 * \brief Initial state element for CHOICE
 * \tparam T Irrelevant from this perpective: Any template argument.
 * \tparam U Irrelevant from this perpective: Any template argument.
 */
template <class T, class U>
constexpr ElementIdentifier const ChoiceBuilderState<T, U>::kChoiceInitId;

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CHOICE_BUILDER_STATE_H_
