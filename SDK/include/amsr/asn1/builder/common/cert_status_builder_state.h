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
/*!        \file  cert_status_builder_state.h
 *        \brief  ASN.1 Builder state for CertStatus type.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CERT_STATUS_BUILDER_STATE_H
#define AMSR_VECTOR_FS_VAASN1_CERT_STATUS_BUILDER_STATE_H

#include "amsr/asn1/structure/cert_status.h"
#include "choice_builder_state.h"
#include "revoked_info_builder_state.h"
#include "value_type_states.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for CertStatus elements as per RFC 6960.
 */
class CertStatusBuilderState : public ChoiceBuilderState<structure::CertStatus, CertStatusBuilderState> {
 public:
  /*!
   * \brief Type alias for status builder.
   */
  using ContentBuilder = amsr::core::Variant<NullBuilderState, RevokedInfoBuilderState,
                                             SpecializedNullBuilderState<structure::UnknownInfo>>;

  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using ChoiceBuilderState::ChoiceBuilderState;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::CertStatus> override;

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
  ContentBuilder status_builder_{}; /*!< Member builder: status */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_CERT_STATUS_BUILDER_STATE_H
