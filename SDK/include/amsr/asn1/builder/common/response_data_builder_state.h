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
/*!        \file  response_data_builder_state.h
 *        \brief  ASN.1 Builder for ResponseData type.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_BUILDER_STATE_H
#define AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_BUILDER_STATE_H
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/extension_builder_state.h"
#include "amsr/asn1/builder/common/responder_id_builder_state.h"
#include "amsr/asn1/builder/common/sequence_of_builder_state.h"
#include "amsr/asn1/builder/common/single_response_builder_state.h"
#include "amsr/asn1/builder/common/transition_providers.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/response_data.h"
#include "sequence_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for ResponseData elements as per RFC 6960
 */
class ResponseDataBuilderState : public SequenceBuilderState<structure::ResponseData, ResponseDataBuilderState> {
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
  auto DoYield() const noexcept -> amsr::core::Result<structure::ResponseData> override;

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
  ExplicitlyTaggedTypeBuilder<IntegerBuilderState> version_builder_{this}; /*!< Member builder: version*/
  ResponderIdBuilderState responder_id_builder_{this};                     /*!< Member builder: responderID*/
  GeneralizedTimeBuilderState produced_at_builder_{this};                  /*!< Member builder: producedAt*/
  SequenceOfBuilderState<SingleResponseBuilderState, builder::UniversalSequence> responses_builder_{
      this}; /*!< Member builder: responses*/
  ExplicitlyTaggedTypeBuilder<ExtensionsBuilderState> response_extensions_builder_{
      this}; /*!< Member builder: responseExtensions */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_BUILDER_STATE_H
