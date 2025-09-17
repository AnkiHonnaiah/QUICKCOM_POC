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
/*!       \file   amsr/asn1/builder/common/sequence_of_builder_state.h
 *        \brief  ASN.1 Builder for SEQUENCE OF.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SEQUENCE_OF_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SEQUENCE_OF_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/asn1/builder/constructed_type_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for SEQUENCE OF type.
 * \tparam BuilderType The type of the builder for the element. For instance, for a SEQUENCE OF INTEGER, this would be
 *      IntegerBuilderState.
 * \tparam TransitionProvider The concrete subclass derived from SequenceOfBuilderState in case you
 *      need to define element tags wrapped each of the elements to make sequences from. Primitive and CHOICE elements
 *      should not need this, but for constructed types, you may need to define an enclosing SEQUENCE or SET for the
 *      elements. This template parameter is asked for GetTransitions() and defaults to BuilderType.
 */
// VECTOR NC AutosarC++17_10-M3.2.2: MD_VAASN1_AutosarC++17_10-M3.2.2_odr_violation_false_positive
template <typename BuilderType, typename TransitionProvider = BuilderType>
class SequenceOfBuilderState
    : public ConstructedTypeBuilderState<ara::core::Vector<typename BuilderType::YieldedType>> {
  static_assert(std::is_base_of<Asn1Builder, BuilderType>(), "Builder type should be derived from Asn1Builder.");

 public:
  /*!
   * \brief Alias for constraint parameter type.
   */
  using CPType = ara::core::Vector<typename BuilderType::YieldedType>;

  /*!
   * \brief Constructor.
   * \param[in] parent Handle to parent.
   */
  // VECTOR NC VectorC++-V12.1.1: MD_VAASN1_VectorC++-V12.1.1-V12.1.1_incomplete_ctor_false_positive
  explicit SequenceOfBuilderState(Asn1Builder* parent = nullptr) noexcept
      : ConstructedTypeBuilderState<ara::core::Vector<typename BuilderType::YieldedType>>(
            BuildTransitionTable(TransitionProvider::GetTransitions()), parent),
        element_builder_{this} {
    sequence_of_.clear();
  }

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
  void SetConstraint(std::shared_ptr<Constraint<CPType>> constraint) noexcept { constraint_ = constraint; }

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
  void SetChildConstraint(std::shared_ptr<Constraint<typename BuilderType::CPType>> constraint) noexcept {
    element_builder_.SetConstraint(constraint);
  }

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
    if (constraint_) {
      accepted = constraint_->IsAccepted(sequence_of_);
    }
    return accepted;
  }

 protected:
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
  auto CreateState(builder::ElementIdentifier const& element) noexcept -> Asn1Builder* override {
    BuilderType* state{nullptr};
    if ((element.id != SequenceOfBuilderState<BuilderType, TransitionProvider>::kInitId.id) && (element.id != 0)) {
      state = &element_builder_;
    }
    return state;
  }

  /*!
   * \brief     Special pre-accept handling for SEQUENCE OF types.
   * \details   Since data is cached for SEQUENCE OF builders, before each element's accept call, we must
   *            save the yielded value into the internal data buffer. We reset the element builder, so its data
   *            would otherwise be lost.
   * \return    An error if the builder is in an unexpected state or the element yield failed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PreAccept() noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> retval{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A SEQUENCE OF or SET OF builder is in an internally invalid state."_sv)};
    if (this->CurrentState() == &element_builder_) {
      retval = element_builder_.Yield()
                   .Inspect([this](typename BuilderType::YieldedType value) {
                     sequence_of_.push_back(std::move(value));
                     element_builder_.Reset();
                   })
                   .template Replace<void>();
    }
    return retval;
  }

  /*!
   * \brief Returns the built data.
   * \return A result containing the yielded data or an error if yielding was not possible.
   */
  auto DoYield() const noexcept -> amsr::core::Result<ara::core::Vector<typename BuilderType::YieldedType>> override {
    return sequence_of_;
  }

  /*!
   * \brief Resets element builder and saved data.
   */
  auto DoReset() noexcept -> void override {
    sequence_of_.clear();
    element_builder_.Reset();
  }

 private:
  /*!
   * \brief Creates transition table for multiple possible inputs.
   * \param[in] transitions Transitions for the elements contained in the sequence. Use this with SEQUENCE OF CHOICE.
   * \return Sequence of transitions.
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
    std::set<Transition> transition_table;
    ElementIdentifier init{
        ConstructedTypeBuilderState<ara::core::Vector<typename BuilderType::YieldedType>>::kAnyElementId};
    init.is_final = true;
    ElementIdentifier const final{1, ElementPresence::kOptional, true};

    for (std::pair<ElementInput, ElementIdentifier> const& input_pair : transitions) {
      ElementInput const& input{input_pair.first};
      static_cast<void>(transition_table.insert(Transition{init, input, final}));
      static_cast<void>(transition_table.insert(Transition{final, input, final}));
    }
    return transition_table;
  }

  ara::core::Vector<typename BuilderType::YieldedType> sequence_of_{}; /*!< Data to be built */
  BuilderType element_builder_{};                                      /*!< Builder for element type */
  std::shared_ptr<Constraint<CPType>> constraint_{};                   /*!< Constraint for SEQUENCE OF */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SEQUENCE_OF_BUILDER_STATE_H_
