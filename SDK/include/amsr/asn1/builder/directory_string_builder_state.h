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
/*!       \file   amsr/asn1/builder/directory_string_builder_state.h
 *        \brief  ASN.1 Builder for directory string.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_DIRECTORY_STRING_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_DIRECTORY_STRING_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/choice_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/directory_string.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for DirectoryStrings as per RFC 5912.
 * \tparam MAX_SIZE Maximum size of contained string.
 */
template <std::size_t MAX_SIZE>
class DirectoryStringBuilderState
    : public ChoiceBuilderState<structure::DirectoryString, DirectoryStringBuilderState<MAX_SIZE>> {
 public:
  /*!
   * \brief Type alias for builder variant.
   */
  using ContentBuilder = amsr::core::Variant<T61StringBuilderState, UniversalStringBuilderState, BMPStringBuilderState,
                                             PrintableStringBuilderState, Utf8StringBuilderState>;
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using ChoiceBuilderState<structure::DirectoryString, DirectoryStringBuilderState<MAX_SIZE>>::ChoiceBuilderState;

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
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>> {
    return {
        {{Asn1Parser::Asn1Class::kUniversal, static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kT61String)},
         {1, ElementPresence::kOptional, true}},
        {{Asn1Parser::Asn1Class::kUniversal,
          static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kPrintableString)},
         {2, ElementPresence::kOptional, true}},
        {{Asn1Parser::Asn1Class::kUniversal,
          static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kUniversalString)},
         {3, ElementPresence::kOptional, true}},
        {{Asn1Parser::Asn1Class::kUniversal, static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kUTF8String)},
         {4, ElementPresence::kOptional, true}},
        {{Asn1Parser::Asn1Class::kUniversal, static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kBMPString)},
         {5, ElementPresence::kOptional, true}}};
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
      case 1: {
        T61StringBuilderState& actual_builder{content_builder_.emplace<T61StringBuilderState>(this)};
        actual_builder.SetConstraint(
            std::make_shared<SizeConstraint<structure::T61String>>(std::make_pair(1u, MAX_SIZE)));
        state = &actual_builder;
        break;
      }

      case 2: {
        PrintableStringBuilderState& actual_builder{content_builder_.emplace<PrintableStringBuilderState>(this)};
        actual_builder.SetConstraint(
            std::make_shared<SizeConstraint<structure::PrintableString>>(std::make_pair(1u, MAX_SIZE)));
        state = &actual_builder;
        break;
      }

      case 3: {
        UniversalStringBuilderState& actual_builder{content_builder_.emplace<UniversalStringBuilderState>(this)};
        actual_builder.SetConstraint(
            std::make_shared<SizeConstraint<structure::UniversalString>>(std::make_pair(1u, MAX_SIZE)));
        state = &actual_builder;
        break;
      }

      case 4: {
        Utf8StringBuilderState& actual_builder{content_builder_.emplace<Utf8StringBuilderState>(this)};
        actual_builder.SetConstraint(
            std::make_shared<SizeConstraint<structure::Utf8String>>(std::make_pair(1u, MAX_SIZE)));
        state = &actual_builder;
        break;
      }

      case 5: {
        BMPStringBuilderState& actual_builder{content_builder_.emplace<BMPStringBuilderState>(this)};
        actual_builder.SetConstraint(
            std::make_shared<SizeConstraint<structure::BMPString>>(std::make_pair(1u, MAX_SIZE)));
        state = &actual_builder;
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
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DoYield() const noexcept -> amsr::core::Result<structure::DirectoryString> override {
    structure::DirectoryString yielded{};

    amsr::core::Result<bool> const content_result{
        ConstructedTypeBuilderState<structure::DirectoryString>::YieldToVariant(content_builder_, yielded)};

    return content_result.And(amsr::core::Result<structure::DirectoryString>::FromValue(std::move(yielded)));
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
    ConstructedTypeBuilderState<structure::DirectoryString>::ResetVariantBuilder(content_builder_);
  }

 private:
  ContentBuilder content_builder_{}; /*!< Value builder of this CHOICE. */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_DIRECTORY_STRING_BUILDER_STATE_H_
