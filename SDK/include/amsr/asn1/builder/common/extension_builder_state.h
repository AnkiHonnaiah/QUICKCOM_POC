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
/*!       \file   amsr/asn1/builder/common/extension_builder_state.h
 *        \brief  ASN.1 Builder for extension type.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/extension.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for extension type.
 */
class ExtensionBuilderState : public SequenceBuilderState<structure::Extension, ExtensionBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using SequenceBuilderState::SequenceBuilderState;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>>;

  /*!
   * \brief Creates child states based on current state.
   * \param[in] element Identifier for state.
   * \return Pointer to next state.
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

  auto DoYield() const noexcept -> amsr::core::Result<structure::Extension> override;
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
  /*!
   * \brief Peforms recursive parsing of the ExtnValue attribute.
   * \details The ExtnValue attribute is a DER-encoded octet string that can represent various types of Extension.
   *    This method returns a structured representation of said string if known. The types of Extension "known"
   *    depend on the ExtnId attribute being passed.
   * \param[in] yielded_extension The octet string value of ExtnValue that shall be parsed.
   * \param[in] extn_id The ExtnId value indicating which type of Extension to parse as.
   * \error kRuntimeFault if a parsing or yielding error occurred.
   * \return A result containing the structural value to yield into ExtnValue.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ParseExtensionValue(structure::OctetString const& yielded_extension,
                                  structure::Oid const& extn_id) noexcept
      -> amsr::core::Result<structure::Extension::ExtnValueType>;

  OidBuilderState extn_id_builder_{this};            /*!< Member builder: ExtnId */
  BooleanBuilderState critical_builder_{this};       /*!< Member builder: Critical */
  OctetStringBuilderState extn_value_builder_{this}; /*!< Member builder: ExtnValue */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_BUILDER_STATE_H_
