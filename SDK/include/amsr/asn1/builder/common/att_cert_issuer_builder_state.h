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
/*!       \file   amsr/asn1/builder/common/att_cert_issuer_builder_state.h
 *        \brief  ASN.1 builder for AttCertIssuer.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATT_CERT_ISSUER_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATT_CERT_ISSUER_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/choice_builder_state.h"
#include "amsr/asn1/structure/att_cert_issuer.h"
#include "general_name_builder_state.h"
#include "v2_form_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for AttCertIssuer.
 */
class AttCertIssuerBuilderState : public ChoiceBuilderState<structure::AttCertIssuer, AttCertIssuerBuilderState> {
 public:
  /*!
   * \brief Type alias for CHOICE content builder.
   */
  using ContentBuilder = amsr::core::Variant<GeneralNamesBuilderState, V2FormBuilderState>;

  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using ChoiceBuilderState::ChoiceBuilderState;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>>;

  /*!
   * \brief Passes the subbuilder responsible for input identified by \c element.
   * \param[in] element The internal element identifier for which to return a builder.
   * \return Pointer to the builder that builds the following data.
   */
  auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* override;

 protected:
  /*!
   * \brief Returns the built data.
   * \return A result containing the yielded data or an error if yielding was not possible.
   */
  auto DoYield() const noexcept -> amsr::core::Result<structure::AttCertIssuer> override;

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   * \return void
   */
  auto DoReset() noexcept -> void override;

 private:
  ContentBuilder content_builder_{}; /*!< CHOICE content builder */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ATT_CERT_ISSUER_BUILDER_STATE_H_
