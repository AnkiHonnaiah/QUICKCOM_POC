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
/*!       \file   amsr/asn1/builder/common/extension_attribute_builder_state.h
 *        \brief  Extension Atributes builder state.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_ATTRIBUTE_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_ATTRIBUTE_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/extension_attributes.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for Builtin Standard Attributes.
 */
class ExtensionAttributeBuilderState
    : public SequenceBuilderState<structure::ExtensionAttribute, ExtensionAttributeBuilderState> {
 public:
  /*!
   * \brief Type alias for variant builder.
   */
  using ContentBuilder = amsr::core::Variant<ExplicitlyTaggedTypeBuilder<RawDataBuilderState>>;
  /*!
   * \brief Constructor for state.
   * \param[in] parent Parent state.
   */
  explicit ExtensionAttributeBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::ExtensionAttribute> override;

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
  IntegerBuilderState extension_attribute_type_builder_{this}; /*!< Member: ExtensionAttributeType Builder*/
  ContentBuilder extension_attribute_value_builder_{};         /*!< Member: ExtensionAttributeValue Builder*/
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_EXTENSION_ATTRIBUTE_BUILDER_STATE_H_
