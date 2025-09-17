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
/*!       \file   amsr/asn1/builder/common/builtin_standard_attributes_builder_state.h
 *        \brief  Builtin Standard Attributes builder (RFC5280).
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_BUILTIN_STANDARD_ATTRIBUTES_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_BUILTIN_STANDARD_ATTRIBUTES_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/sequence_builder_state.h"

#include "amsr/asn1/builder/common/administration_domain_name_builder_state.h"
#include "amsr/asn1/builder/common/country_name_builder_state.h"
#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/personal_name_builder_state.h"
#include "amsr/asn1/builder/common/private_domain_name_builder_state.h"
#include "amsr/asn1/builder/common/sequence_of_builder_state.h"
#include "amsr/asn1/builder/common/transition_providers.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/builtin_standard_attributes.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for Builtin Standard Attributes.
 */
class BuiltinStandardAttributesBuilderState
    : public SequenceBuilderState<structure::BuiltinStandardAttributes, BuiltinStandardAttributesBuilderState> {
 public:
  /*!
   * \brief Constructor for state.
   * \param[in] parent Parent state.
   */
  explicit BuiltinStandardAttributesBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::BuiltinStandardAttributes> override;

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
  ExplicitlyTaggedTypeBuilder<CountryNameBuilderState, ExplicitChoice> country_name_builder_{
      this}; /*!< Member builder: country-name*/
  ExplicitlyTaggedTypeBuilder<AdministrationDomainNameBuilderState, ExplicitChoice> administrator_domain_name_builder_{
      this};                                                      /*!< Member builder: administration-domain-name*/
  NumericStringBuilderState network_address_builder_{this};       /*!< Member builder: network-address*/
  PrintableStringBuilderState terminal_identifier_builder_{this}; /*!< Member builder: terminal-identifier*/
  ExplicitlyTaggedTypeBuilder<PrivateDomainNameBuilderState, ExplicitChoice> private_domain_name_builder_{
      this};                                                        /*!< Member builder: private-domain-name*/
  PrintableStringBuilderState organization_name_builder_{this};     /*!< Member builder: organization-name*/
  NumericStringBuilderState numeric_user_identifier_builder_{this}; /*!< Member builder: numberic-user-identifier*/
  PersonalNameBuilderState personal_name_builder_{this};            /*!< Member builder: personal-name*/
  SequenceOfBuilderState<PrintableStringBuilderState> organizational_unit_names_builder_{
      this}; /*!< Member builder: organizational-unit-names*/
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_BUILTIN_STANDARD_ATTRIBUTES_BUILDER_STATE_H_
