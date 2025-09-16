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
/*!       \file   amsr/asn1/builder/common/edi_party_name_builder_state.h
 *        \brief  ASN.1 Builder state for EDIPartyName(RFC5280)
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EDI_PARTY_NAME_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EDI_PARTY_NAME_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/directory_string_builder_state.h"
#include "amsr/asn1/structure/edi_party_name.h"
#include "explicitly_tagged_type_builder.h"
#include "transition_providers.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for EDIPartyName.
 */
class EDIPartyNameBuilderState : public SequenceBuilderState<structure::EDIPartyName, EDIPartyNameBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using SequenceBuilderState::SequenceBuilderState;

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
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>>;

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
  auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* override;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::EDIPartyName> override;

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
  auto DoReset() noexcept -> void override;

 private:
  ExplicitlyTaggedTypeBuilder<DirectoryStringBuilderState<structure::EDIPartyName::kUbMax>, builder::ExplicitChoice>
      name_assigner_builder_{this}; /*!< Member builder: nameAssigner*/
  ExplicitlyTaggedTypeBuilder<DirectoryStringBuilderState<structure::EDIPartyName::kUbMax>, builder::ExplicitChoice>
      party_name_builder_{this}; /*!< Member builder: partyName*/
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EDI_PARTY_NAME_BUILDER_STATE_H_
