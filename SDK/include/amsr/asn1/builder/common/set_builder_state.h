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
/*!       \file   amsr/asn1/builder/common/set_builder_state.h
 *        \brief  ASN.1 Builder for SET.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/constructed_type_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for SET type.
 */
template <class YieldedType, class DerivedThis>
class SetBuilderState : public ConstructedTypeBuilderState<YieldedType> {
 public:
  // FIXME: See ChoiceBuilderState and SECAP-11023. CPType != YieldedType makes no sense.
  /*!
   * \brief Alias for constraint parameter type.
   */
  using CPType = std::uint64_t;

  /*!
   * \brief Constructor.
   * \param[in] parent Handle to parent.
   */
  explicit SetBuilderState(Asn1Builder* parent = nullptr) noexcept
      : ConstructedTypeBuilderState<YieldedType>(BuildTransitionTable(DerivedThis::GetTransitions()), parent) {}

  /*!
   * \brief Conceptually, sets constraint. Does nothing in this class however!
   * \param[in] constraint Constraint to be applied.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAASN1_AutosarC++17_10-M0.1.8_unused_void
  static auto SetConstraint(std::shared_ptr<Constraint<CPType>> constraint) noexcept -> void {
    // Constraints on sets itself were never encountered. This method exists for technical reasons.
    static_cast<void>(constraint);
  }

  /*!
   * \brief Conceptually, checks if constraints are satisfied. Does nothing in this class however!
   * \return \c true if valid, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CheckConstraints() const noexcept -> bool override { return true; }

 private:
  /*!
   * \brief Creates transition table.
   * \param[in] transitions Pair of input and target states.
   * \return Set of transitions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto BuildTransitionTable(
      ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> const& transitions) noexcept
      -> std::set<Transition> {
    ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> mutable_states{};
    std::set<Transition> transition_table;
    std::set<std::uint64_t> sources{0};

    for (std::pair<ElementInput, ElementIdentifier> const ti : transitions) {
      if (ti.second.presence_flag != ElementPresence::kAbsent) {
        mutable_states.push_back(ti);
        static_cast<void>(sources.insert(ti.second.id));
      }
    }
    for (std::pair<ElementInput, ElementIdentifier>& ti : mutable_states) {
      ti.second.is_final = true;
      for (std::uint64_t const& source : sources) {
        if (ti.second.id != source) {
          static_cast<void>(
              transition_table.insert(Transition{{source, ElementPresence::kOptional, false}, ti.first, ti.second}));
        }
      }
    }
    return transition_table;
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_BUILDER_STATE_H_
