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
/*!       \file   amsr/asn1/builder/common/rsaes_oaep_params_builder_state.h
 *        \brief  RSAES OAEP Params parameter.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSAES_OAEP_PARAMS_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSAES_OAEP_PARAMS_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/transition_providers.h"
#include "amsr/asn1/structure/rsaes_oaep_params.h"

namespace amsr {
namespace asn1 {
namespace builder {

class AlgorithmIdentifierBuilderState;
/*!
 * \brief Builder state for RSAES OAEP Params.
 */
class RSAEsOaepParamsBuilderState
    : public SequenceBuilderState<structure::RSAEsOaepParams, RSAEsOaepParamsBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit RSAEsOaepParamsBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::RSAEsOaepParams> override;

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
      hash_func_builder_{}; /*!< Member builder: hashFunc */
  Uptr<ExplicitlyTaggedTypeBuilder<AlgorithmIdentifierBuilderState, UniversalSequence>>
      mask_gen_func_builder_{}; /*!< Member builder: maskGenFunc */
  Uptr<ExplicitlyTaggedTypeBuilder<AlgorithmIdentifierBuilderState, UniversalSequence>>
      p_source_func_builder_{}; /*!< Member builder: pSourceFunc */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_RSAES_OAEP_PARAMS_BUILDER_STATE_H_
