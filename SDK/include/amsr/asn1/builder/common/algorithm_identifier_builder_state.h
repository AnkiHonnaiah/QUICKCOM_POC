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
/*!       \file   amsr/asn1/builder/common/algorithm_identifier_builder_state.h
 *        \brief  Algorithm identifier state.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ALGORITHM_IDENTIFIER_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ALGORITHM_IDENTIFIER_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/primitive_type_builder_state.h"
#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/core/variant.h"
#include "domain_parameters_builder_state.h"
#include "dss_parms_builder_state.h"
#include "ec_parameters_builder_state.h"
#include "rsaes_oaep_params_builder_state.h"
#include "rsassa_pss_params_builder_state.h"
#include "value_type_states.h"

namespace amsr {
namespace asn1 {
namespace builder {

class AlgorithmIdentifierBuilderState;

/*!
 * \brief Builder for Algorithm identifier.
 */
class AlgorithmIdentifierBuilderState
    : public SequenceBuilderState<structure::AlgorithmIdentifier, AlgorithmIdentifierBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit AlgorithmIdentifierBuilderState(Asn1Builder* parent = nullptr) noexcept;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
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
  auto DoYield() const noexcept -> amsr::core::Result<structure::AlgorithmIdentifier> override;

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DoReset() noexcept -> void override;

 private:
  OidBuilderState algorithm_builder_{this}; /*!< Member builder: algorithm */

  /*!
   * \brief Type alias for variant builder.
   */
  using AIParametersBuilder =
      amsr::core::Variant<NullBuilderState, OctetStringBuilderState, DomainParametersBuilderState,
                          EcParametersBuilderState, RSASsaPssParamsBuilderState, RSAEsOaepParamsBuilderState,
                          AlgorithmIdentifierBuilderState, DssParmsBuilderState, RawDataBuilderState>;
  std::unique_ptr<AIParametersBuilder> ai_parameters_builder_{}; /*!< Member builder: parameters */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_ALGORITHM_IDENTIFIER_BUILDER_STATE_H_
