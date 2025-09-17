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
/*!       \file   amsr/asn1/builder/policy_mapping_builder_state.h
 *        \brief  ASN.1 Builder for policy mapping.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_POLICY_MAPPING_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_POLICY_MAPPING_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/sequence_of_builder_state.h"
#include "amsr/asn1/builder/common/transition_providers.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/policy_mapping.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for the anonymous components of PolicyMappings as per RFC 5280. They are being referred to as \c
 *      PolicyMapping in this codebase.
 */
class PolicyMappingBuilderState : public SequenceBuilderState<structure::PolicyMapping, PolicyMappingBuilderState> {
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
  auto DoYield() const noexcept -> amsr::core::Result<structure::PolicyMapping> override;

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
  OidBuilderState issuer_domain_policy_builder_{this};  /*!< Member builder: issuerDomainPolicy. */
  OidBuilderState subject_domain_policy_builder_{this}; /*!< Member builder: subjectDomainPolicy. */
};

/*!
 * \brief Collection builder state for PolicyMappings including a SIZE (1..MAX) constraint (see RFC 5912).
 */
class PolicyMappingsBuilderState : public SequenceOfBuilderState<PolicyMappingBuilderState, UniversalSequence> {
 public:
  /*!
   * \brief Constructor with constraint.
   * \param[in] parent Handle to parent.
   */
  explicit PolicyMappingsBuilderState(Asn1Builder* parent = nullptr) noexcept : SequenceOfBuilderState(parent) {
    SetConstraint(std::make_shared<SizeConstraint<ara::core::Vector<structure::PolicyMapping>>>(
        std::make_pair(1, std::numeric_limits<std::uint64_t>::max())));
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_POLICY_MAPPING_BUILDER_STATE_H_
