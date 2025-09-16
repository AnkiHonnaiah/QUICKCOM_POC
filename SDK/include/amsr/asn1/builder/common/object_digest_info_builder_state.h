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
/*!       \file   amsr/asn1/builder/common/object_digest_info_builder_state.h
 *        \brief  Representation of ObjectDigestInfo Builder.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_COMMON_OBJECT_DIGEST_INFO_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_COMMON_OBJECT_DIGEST_INFO_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/builder/common/algorithm_identifier_builder_state.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/object_digest_info.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for ObjectDigestInfo.
 */
class ObjectDigestInfoBuilderState
    : public SequenceBuilderState<structure::ObjectDigestInfo, ObjectDigestInfoBuilderState> {
 public:
  /*!
   * \brief Constructor for state.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit ObjectDigestInfoBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::ObjectDigestInfo> override;

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
  EnumeratedBuilderState digested_object_type_builder_{this};      /*!< DigestedObjectType. */
  OidBuilderState other_object_type_id_builder_{this};             /*!< OtherObjectTypeID. */
  AlgorithmIdentifierBuilderState digest_algorithm_builder_{this}; /*!< DigestAlgorithm. */
  BitStringBuilderState object_digest_builder_{this};              /*!< ObjectDigest. */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_COMMON_OBJECT_DIGEST_INFO_BUILDER_STATE_H_
