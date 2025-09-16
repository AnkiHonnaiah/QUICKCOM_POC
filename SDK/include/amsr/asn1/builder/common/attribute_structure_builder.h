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
/*!       \file   amsr/asn1/builder/common/attribute_structure_builder.h
 *        \brief  ASN.1 Builder for ATTRIBUTE representations as per RFC 5912.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATTRIBUTE_STRUCTURE_BUILDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATTRIBUTE_STRUCTURE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/directory_string_builder_state.h"
#include "amsr/asn1/structure/attribute.h"
#include "set_of_builder_state.h"
#include "transition_providers.h"
#include "value_type_states.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for attribute structures.
 * \tparam BuilderVariantTemplate The variant template containing the builders that an attribute builder needs
 * \tparam YieldedTypeVariantTemplate The variant template containing the yielded types of the underlying builders
 */
template <template <typename...> class BuilderVariantTemplate, template <typename...> class YieldedTypeVariantTemplate>
class AttributeStructureBuilder
    : public SequenceBuilderState<structure::AttributeStructure<YieldedTypeVariantTemplate>,
                                  AttributeStructureBuilder<BuilderVariantTemplate, YieldedTypeVariantTemplate>> {
 public:
  /*!
   * \brief Constructor for state.
   * \vprivate Product Private
   */
  using SequenceBuilderState<
      structure::AttributeStructure<YieldedTypeVariantTemplate>,
      AttributeStructureBuilder<BuilderVariantTemplate, YieldedTypeVariantTemplate>>::SequenceBuilderState;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {{{AttributeStructureBuilder::Asn1Class::kUniversal,
              static_cast<std::uint64_t>(AttributeStructureBuilder::UniversalTag::kObjectIdentifier)},
             {1, ElementPresence::kPresent, false}},
            {{AttributeStructureBuilder::kChoiceInput}, {2, ElementPresence::kPresent, true}}};
  }

  /*!
   * \brief Passes the subbuilder responsible for input identified by \c element.
   * \param[in] element The internal element identifier for which to return a builder.
   * \return Pointer to the builder that builds the following data.
   */
  // VECTOR NC Metric-HIS.VG: MD_VAASN1_Metric-HIS.VG_Simplification_is_in_progress_CRYPT_556
  auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* override {
    Asn1Builder* state{nullptr};

    switch (element.id) {
      case 1:
        state = &type_builder_;
        break;

      case 2: {
        structure::Oid const none{};
        structure::Oid const string_type_oid{type_builder_.Yield().ValueOr(none)};

        if ((structure::attribute::kIdAtName == string_type_oid) ||
            (structure::attribute::kIdAtSurname == string_type_oid) ||
            (structure::attribute::kIdAtGivenName == string_type_oid) ||
            (structure::attribute::kIdAtInitials == string_type_oid) ||
            (structure::attribute::kIdAtGenerationQualifier == string_type_oid) ||
            (structure::attribute::kIdAtHouseIdentifier == string_type_oid)) {
          // Upper bound: 32768
          state = MultiplicityHandling<YieldedTypeVariantTemplate,
                                       DirectoryStringBuilderState<structure::attribute::kUbName>>::
              EmplaceAccordingToMultiplicity(value_builder_, this);
        } else if (structure::attribute::kIdRfc4519UserId == string_type_oid) {
          // Upper bound: 255
          state = MultiplicityHandling<YieldedTypeVariantTemplate,
                                       DirectoryStringBuilderState<structure::attribute::kUbRfc4519UserId>>::
              EmplaceAccordingToMultiplicity(value_builder_, this);
        } else if (structure::attribute::kIdAtEmailAddress == string_type_oid) {
          auto* const actual_builder =
              MultiplicityHandling<YieldedTypeVariantTemplate, Ia5StringBuilderState>::EmplaceAccordingToMultiplicity(
                  value_builder_, this);
          MultiplicityHandling<YieldedTypeVariantTemplate, Ia5StringBuilderState>::ApplyElementConstraint(
              actual_builder, std::make_shared<SizeConstraint<structure::Ia5String>>(
                                  std::make_pair(1u, structure::attribute::kUbEmailaddressLength)));
          state = actual_builder;
        } else if ((structure::attribute::kIdAtX520StateOrProvinceName == string_type_oid) ||
                   (structure::attribute::kIdAtPseudonym == string_type_oid) ||
                   (structure::attribute::kIdAtX520LocalityName == string_type_oid)) {
          // Upper bound: 128
          state = MultiplicityHandling<YieldedTypeVariantTemplate,
                                       DirectoryStringBuilderState<structure::attribute::kUbStateName>>::
              EmplaceAccordingToMultiplicity(value_builder_, this);
        } else if ((structure::attribute::kIdAtCommonName == string_type_oid) ||
                   (structure::attribute::kIdAtX520OrganizationName == string_type_oid) ||
                   (structure::attribute::kIdAtX520OrganizationalUnitName == string_type_oid) ||
                   (structure::attribute::kIdAtTitle == string_type_oid)) {
          // Upper bound: 64
          state = MultiplicityHandling<YieldedTypeVariantTemplate,
                                       DirectoryStringBuilderState<structure::attribute::kUbCommonName>>::
              EmplaceAccordingToMultiplicity(value_builder_, this);
        } else if (structure::attribute::kIdAtDnQualifier == string_type_oid) {
          state =
              MultiplicityHandling<YieldedTypeVariantTemplate,
                                   PrintableStringBuilderState>::EmplaceAccordingToMultiplicity(value_builder_, this);
        } else if (structure::attribute::kIdAtCountryName == string_type_oid) {
          auto* const actual_builder =
              MultiplicityHandling<YieldedTypeVariantTemplate,
                                   PrintableStringBuilderState>::EmplaceAccordingToMultiplicity(value_builder_, this);
          MultiplicityHandling<YieldedTypeVariantTemplate, PrintableStringBuilderState>::ApplyElementConstraint(
              actual_builder, std::make_shared<SizeConstraint<structure::PrintableString>>(std::make_pair(2u, 2u)));
          state = actual_builder;
        } else if (structure::attribute::kIdAtSerialNumber == string_type_oid) {
          auto* const actual_builder =
              MultiplicityHandling<YieldedTypeVariantTemplate,
                                   PrintableStringBuilderState>::EmplaceAccordingToMultiplicity(value_builder_, this);
          MultiplicityHandling<YieldedTypeVariantTemplate, PrintableStringBuilderState>::ApplyElementConstraint(
              actual_builder, std::make_shared<SizeConstraint<structure::PrintableString>>(
                                  std::make_pair(1u, structure::attribute::kUbSerialNumber)));
          state = actual_builder;
        } else if (structure::attribute::kIdAtDomainComponent == string_type_oid) {
          state =
              MultiplicityHandling<YieldedTypeVariantTemplate, Ia5StringBuilderState>::EmplaceAccordingToMultiplicity(
                  value_builder_, this);
        } else {
          state = MultiplicityHandling<YieldedTypeVariantTemplate, RawDataBuilderState>::EmplaceAccordingToMultiplicity(
              value_builder_, this);
        }
        break;
      }

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
   */
  auto DoYield() const noexcept
      -> amsr::core::Result<structure::AttributeStructure<YieldedTypeVariantTemplate>> override {
    structure::AttributeStructure<YieldedTypeVariantTemplate> yielded{};
    amsr::core::Result<void> const type_result{AttributeStructureBuilder::YieldTo(type_builder_, yielded.Type())};
    amsr::core::Result<bool> const value_result{
        AttributeStructureBuilder::YieldToVariant(value_builder_, yielded.Value())};

    amsr::core::Result<structure::AttributeStructure<YieldedTypeVariantTemplate>> retval{
        type_result.And(value_result)
            .And(amsr::core::Result<structure::AttributeStructure<YieldedTypeVariantTemplate>>::FromValue(
                std::move(yielded)))};

    if (value_result.HasValue() && (!value_result.Value())) {
      retval.EmplaceError(Asn1Errc::kRuntimeFault, "An ATTRIBUTE (AttributeSet or SingleAttribute) contains no value.");
    }

    return retval;
  }

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   */
  auto DoReset() noexcept -> void override {
    type_builder_.Reset();
    AttributeStructureBuilder::ResetVariantBuilder(value_builder_);
  }

 private:
  /*!
   * \brief Definition of what builders are usable for attribute values.
   */
  using AttributeValueBuilderVariant =
      BuilderVariantTemplate<DirectoryStringBuilderState<structure::attribute::kUbOrganizationName>,
                             DirectoryStringBuilderState<structure::attribute::kUbPseudonym>,
                             DirectoryStringBuilderState<structure::attribute::kUbEmailaddressLength>,
                             DirectoryStringBuilderState<structure::attribute::kUbName>, Ia5StringBuilderState,
                             PrintableStringBuilderState, RawDataBuilderState>;
  /*!
   * \brief Template for handling calls differring between SingleAttribute and AttributeSet.
   * \details The only place where \c SingleAttribute and \c AttributeSet builders would differ is their multiplicity.
   *    One needs to yield T, the other yields Vectors of T. For builders, this has implications on the \c CreateState
   *    method. Builders need to be emplaced and constraints need to be defined. This plays out differently w.r.t.
   *    the multiplicity of the builder. This template handles these two cases with static polymorphism in hopes of
   *    eliminating the need for two separate builder classes.
   */
  template <template <typename...> class, typename>
  class MultiplicityHandling {};
  /*!
   * \brief Multiplicity handling for AttributeSet.
   * \tparam T The builder type for which to handle AttributeSet concerns.
   */
  template <typename T>
  class MultiplicityHandling<structure::VariantVector, T> {
   public:
    /*!
     * \brief Emplace a builder for the elements of an AttributeSet.
     * \param value_builder Pass \c value_builder_ here.
     * \param that Pass \c this here.
     * \return The address of the freshly-emplaced builder (as \c Variant::emplace would)
     */
    static auto EmplaceAccordingToMultiplicity(AttributeValueBuilderVariant& value_builder,
                                               AttributeStructureBuilder* that) -> SetOfBuilderState<T>* {
      SetOfBuilderState<T>& emplaced_builder{value_builder.template emplace<SetOfBuilderState<T>>(that)};
      emplaced_builder.SetConstraint(std::make_shared<SizeConstraint<ara::core::Vector<typename T::YieldedType>>>(
          std::make_pair(1, std::numeric_limits<std::uint64_t>::max())));
      return &emplaced_builder;
    }

    /*!
     * \brief Apply a constraint to the elements of an AttributeSet.
     * \param actual_builder Pass the return value of \c EmplaceAccordingToMultiplicity here.
     * \param constraint The constraint to apply to each element of the AttributeSet.
     * \return void
     */
    static auto ApplyElementConstraint(SetOfBuilderState<T>* actual_builder,
                                       std::shared_ptr<Constraint<typename T::YieldedType>> constraint) -> void {
      actual_builder->SetChildConstraint(constraint);
    }
  };
  /*!
   * \brief Multiplicity handling for SingleAttribute.
   * \tparam T The builder type for which to handle SingleAttribute concerns.
   */
  template <typename T>
  class MultiplicityHandling<amsr::core::Variant, T> {
   public:
    /*!
     * \brief Emplace a builder for the elements of a SingleAttribute.
     * \param value_builder Pass \c value_builder_ here.
     * \param that Pass \c this here.
     * \return The address of the freshly-emplaced builder (as \c Variant::emplace would)
     */
    static auto EmplaceAccordingToMultiplicity(AttributeValueBuilderVariant& value_builder,
                                               AttributeStructureBuilder* that) -> T* {
      return &value_builder.template emplace<T>(that);
    }

    /*!
     * \brief Apply a constraint to the elements of a SingleAttribute.
     * \param actual_builder Pass the return value of \c EmplaceAccordingToMultiplicity here.
     * \param constraint The constraint to apply to each element of the SingleAttribute.
     * \return void
     */
    static auto ApplyElementConstraint(T* actual_builder,
                                       std::shared_ptr<Constraint<typename T::YieldedType>> constraint) -> void {
      actual_builder->SetConstraint(constraint);
    }
  };
  OidBuilderState type_builder_{this};           /*!< Member builder: type */
  AttributeValueBuilderVariant value_builder_{}; /*!< Member builder: value */
};

/*!
 * \brief Builder for SingleAttribute as per RFC 5912.
 */
using SingleAttributeBuilder = AttributeStructureBuilder<amsr::core::Variant, amsr::core::Variant>;

/*!
 * \brief Builder analogue to VariantVector: A variant of builders building SET OF T.
 */
template <typename... Ts>
using VariantVectorBuilder = amsr::core::Variant<SetOfBuilderState<Ts>...>;

/*!
 * \brief Builder for AttributeSet as per RFC 5912.
 */
using AttributeSetBuilder = AttributeStructureBuilder<VariantVectorBuilder, structure::VariantVector>;

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATTRIBUTE_STRUCTURE_BUILDER_H_
