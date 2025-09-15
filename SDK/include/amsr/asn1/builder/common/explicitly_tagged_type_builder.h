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
/*!       \file   amsr/asn1/builder/common/explicitly_tagged_type_builder.h
 *        \brief  ASN.1 Builder for explicit target type.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXPLICITLY_TAGGED_TYPE_BUILDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXPLICITLY_TAGGED_TYPE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/constraint.h"
#include "amsr/asn1/builder/constructed_type_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for explicit tagged type.
 * \tparam DecoratedBuilder The builder underneath the EXPLICIT tag.
 * \tparam TransitionProvider The provider of transitions for the element contained in the EXPLICIT tag.
 *      For constructed types, this is the inner tag, so if, for example, you have some sequence type that is explicitly
 *      tagged, you must use a transition provider for universal sequences here, like \c UniversalSequence or similar.
 *      To facilitate handling of primitive types, this defaults to \c DecoratedBuilder but that will not be correct
 *      in case of constructed types, as their builders provide transitions for contents, not containers.
 */
// VECTOR NC AutosarC++17_10-M3.2.2: MD_VAASN1_AutosarC++17_10-M3.2.2_odr_violation_false_positive
template <typename DecoratedBuilder, typename TransitionProvider = DecoratedBuilder>
class ExplicitlyTaggedTypeBuilder : public ConstructedTypeBuilderState<typename DecoratedBuilder::YieldedType> {
 public:
  /*!
   * \brief Alias for constraint parameter type.
   */
  using CPType = typename DecoratedBuilder::CPType;

  /*!
   * \brief Constructor.
   * \param[in] parent Handle to parent.
   */
  // VECTOR NC VectorC++-V12.1.1: MD_VAASN1_VectorC++-V12.1.1-V12.1.1_incomplete_ctor_false_positive
  explicit ExplicitlyTaggedTypeBuilder(Asn1Builder* parent = nullptr) noexcept
      : ConstructedTypeBuilderState<typename DecoratedBuilder::YieldedType>(
            BuildTransitionTable(TransitionProvider::GetTransitions()), parent),
        sub_builder_{this} {
    static_assert(std::is_base_of<Asn1Builder, DecoratedBuilder>(), "Builder type shall be derived from Asn1Builder.");
  }

  /*!
   * \brief Sets constraint for target type.
   * \param[in] constraint constraint to be applied.
   */
  void SetConstraint(std::shared_ptr<Constraint<CPType>> constraint) noexcept {
    sub_builder_.SetConstraint(constraint);
  };

 protected:
  /*!
   * \brief Passes the subbuilder responsible for input identified by \c element.
   * \param[in] element The internal element identifier for which to return a builder.
   * \return Pointer to the builder that builds the following data.
   */
  auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* override {
    DecoratedBuilder* state{nullptr};
    if (element.id == 1U) {
      state = &sub_builder_;
    }
    return state;
  }

  /*!
   * \brief Creates transition table.
   * \param[in] transitions Input for transition.
   * \return Pointer to next state.
   */
  static auto BuildTransitionTable(
      ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> const& transitions) noexcept
      -> std::set<Transition> {
    std::set<Transition> transaction_table{};
    ElementIdentifier const final{1, ElementPresence::kOptional, true};

    for (std::pair<ElementInput, ElementIdentifier> const& input_pair : transitions) {
      static_cast<void>(transaction_table.insert(
          Transition{ConstructedTypeBuilderState<typename DecoratedBuilder::YieldedType>::kAnyElementId,
                     input_pair.first, final}));
    }
    return transaction_table;
  }

  /*!
   * \brief Checks if constraints are satisfied.
   * \return \c true if valid, \c false otherwise.
   * \vprivate
   */
  auto CheckConstraints() const noexcept -> bool override { return sub_builder_.CheckConstraints(); }

  /*!
   * \brief Returns the built data.
   * \return A result containing the yielded data or an error if yielding was not possible.
   */
  auto DoYield() const noexcept -> amsr::core::Result<typename DecoratedBuilder::YieldedType> override {
    return sub_builder_.Yield();
  }

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   */
  auto DoReset() noexcept -> void override { sub_builder_.Reset(); }

 private:
  /*!
   * \brief Subtype builder
   */
  DecoratedBuilder sub_builder_{};
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXPLICITLY_TAGGED_TYPE_BUILDER_H_
