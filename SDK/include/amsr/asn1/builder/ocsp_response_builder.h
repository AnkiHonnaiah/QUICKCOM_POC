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
/*!        \file  ocsp_response_builder.h
 *        \brief  ASN.1 Builder for OCSPResponse type.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_BUILDER_H
#define AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_BUILDER_H

#include "amsr/asn1/builder/common/ocsp_response_status_builder.h"
#include "amsr/asn1/builder/common/response_bytes_builder_state.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/ocsp_response.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for OCSPResponse elements as per RFC 6960
 */
class OcspResponseBuilder : public SequenceBuilderState<structure::OcspResponse, OcspResponseBuilder> {
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
  auto DoYield() const noexcept -> amsr::core::Result<structure::OcspResponse> override;

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
  OcspResponseStatusBuilder response_status_builder_{this}; /*!< Member builder: responseStatus */
  ExplicitlyTaggedTypeBuilder<ResponseBytesBuilderState, UniversalSequence> response_bytes_builder_{
      this}; /*!< Member builder: responseBytes */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_BUILDER_H
