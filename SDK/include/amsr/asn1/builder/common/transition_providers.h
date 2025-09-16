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
/*!        \file  transition_providers.h
 *        \brief  A collection of useful transition providers for EXPLICIT and SEQUENCE OF builders
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_TRANSITION_PROVIDERS_H
#define AMSR_VECTOR_FS_VAASN1_TRANSITION_PROVIDERS_H

#include "amsr/asn1/asn1_parser.h"
#include "amsr/asn1/builder/transition.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief A generic transition provider. This can be used to inject a tag class and type tag into a decorator like
 *      \c ExplicitlyTaggedTypeBuilder or \c SequenceOfBuilderState
 * \details Some template decorators are almost completely defined, but they require one additional transition because
 *      they represent structures where multiple tags precede a piece of data. An EXPLICITly tagged element, for
 *      instance, is usually preceded by a context-specific tag and a universal one. Usually, one would have to
 *      derive \c ExplicitlyTaggedTypeBuilder and redefine the \c GetTransitions method to provide the inner, universal
 *      transition. With this class, one can write <tt>ExplicitlyTaggedTypeBuilder\<InnerBuilder,
 *      TransitionProvider\<kUniversal, kSequence\>\></tt> instead.
 * \tparam C The tag class, e.g. \c Asn1Class::kContextSpecific
 * \tparam T The type tag, e.g. 0 or \c UniversalTag::kSequence
 */
template <Asn1Parser::Asn1Class C, std::uint64_t T>
class TransitionProviderGeneric {
 public:
  /*!
   * \brief Getter for the internal transition graph represented by this provider
   * \return the internal transition graph represented by this provider
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{C, T}, {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief An alias to facilitate passing uncast tag values.
 */
template <Asn1Parser::Asn1Class C, Asn1Parser::UniversalElementTag T>
using TransitionProvider = TransitionProviderGeneric<C, static_cast<uint64_t>(T)>;

/*!
 * \brief A common transition provider for UNIVERSAL SEQUENCE.
 */
using UniversalSequence =
    TransitionProvider<Asn1Parser::Asn1Class::kUniversal, Asn1Parser::UniversalElementTag::kSequence>;

/*!
 * \brief A common transition provider for UNIVERSAL SET.
 */
using UniversalSet = TransitionProvider<Asn1Parser::Asn1Class::kUniversal, Asn1Parser::UniversalElementTag::kSet>;

/*!
 * \brief A common transition provider for context-specific tags.
 */
template <std::uint64_t T>
using ContextSpecificTag = TransitionProviderGeneric<Asn1Parser::Asn1Class::kContextSpecific, T>;

/*!
 * \brief A common transition provider for EXPLICIT CHOICEs.
 */
using ExplicitChoice = TransitionProviderGeneric<Asn1Parser::Asn1Class::kUniversal,
                                                 ConstructedTypeBuilderState<structure::RawData>::kChoiceInput.tag>;

/*!
 * \brief A common transition provider for Raw Data fallbacks in context-specifically tagged contexts.
 */
using ExplicitRawData = TransitionProviderGeneric<Asn1Parser::Asn1Class::kUniversal,
                                                  ConstructedTypeBuilderState<structure::RawData>::kRawDataInput.tag>;

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_TRANSITION_PROVIDERS_H
