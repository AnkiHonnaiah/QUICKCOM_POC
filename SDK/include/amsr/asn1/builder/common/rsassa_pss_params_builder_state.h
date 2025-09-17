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
/*!       \file   amsr/asn1/builder/common/rsassa_pss_params_builder_state.h
 *        \brief  RSASsaPss Params parameter.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSASSA_PSS_PARAMS_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSASSA_PSS_PARAMS_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/transition_providers.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/rsassa_pss_params.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Forward declaration of partial builder class due to cyclic dependency.
 */
class AlgorithmIdentifierBuilderState;

/*!
 * \brief Builder state for RSASSA PSS Params.
 */
class RSASsaPssParamsBuilderState
    : public SequenceBuilderState<structure::RSASsaPssParams, RSASsaPssParamsBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit RSASsaPssParamsBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::RSASsaPssParams> override;

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
  Uptr<ExplicitlyTaggedTypeBuilder<AlgorithmIdentifierBuilderState, UniversalSequence>>
      hash_algorithm_builder_{}; /*!< Member builder: hashAlgorithm */
  Uptr<ExplicitlyTaggedTypeBuilder<AlgorithmIdentifierBuilderState, UniversalSequence>>
      mask_gen_algorithm_builder_{};                                           /*!< Member builder: maskGenAlgorithm */
  ExplicitlyTaggedTypeBuilder<IntegerBuilderState> salt_length_builder_{this}; /*!< Member builder: saltLength */
  ExplicitlyTaggedTypeBuilder<IntegerBuilderState> trailer_field_builder_{this}; /*!< Member builder: trailerField */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSASSA_PSS_PARAMS_BUILDER_STATE_H_
