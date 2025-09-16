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
/*!        \file  response_bytes_builder_state.h
 *        \brief  ASN.1 Builder for ResponseBytes type.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_BUILDER_STATE_H
#define AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_BUILDER_STATE_H

#include "amsr/asn1/structure/response_bytes.h"
#include "basic_ocsp_response_builder_state.h"
#include "sequence_builder_state.h"
#include "value_type_states.h"

namespace amsr {
namespace asn1 {
namespace builder {
/*!
 * \brief Builder for ResponseBytes elements as per RFC 6960.
 */
class ResponseBytesBuilderState : public SequenceBuilderState<structure::ResponseBytes, ResponseBytesBuilderState> {
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
  auto DoYield() const noexcept -> amsr::core::Result<structure::ResponseBytes> override;

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

  /*!
   * \brief Peforms recursive parsing of the response attribute.
   * \details The response attribute is a DER-encoded octet string that can represent different types of OCSP responses,
   *    but it will in most cases be a Basic OCSP response. The passed OID is used as a discriminator attribute.
   * \param[in] oid The OID used as a type identifier for the input.
   * \param[in] input The DER-encoded bytes to parse the response field from.
   * \error kRuntimeFault if a parsing or yielding error occurred.
   * \return A result containing the structural value to yield into response.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ParseResponseValue(structure::Oid const& oid, structure::OctetString const& input) noexcept
      -> amsr::core::Result<structure::ResponseBytes::ResponseVariant>;

 private:
  OidBuilderState response_type_builder_{this};    /*!< Member builder: responseType */
  OctetStringBuilderState response_builder_{this}; /*!< Member builder: response */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_RESPONSE_BYTES_BUILDER_STATE_H
