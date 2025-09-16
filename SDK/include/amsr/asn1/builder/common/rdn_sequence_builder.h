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
/*!        \file  rdn_sequence_builder.h
 *        \brief  Builder for RDNSequence as per RFC 5912.
 *
 *********************************************************************************************************************/

#ifndef AMSR_VECTOR_FS_VAASN1_RDN_SEQUENCE_BUILDER_H_
#define AMSR_VECTOR_FS_VAASN1_RDN_SEQUENCE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/asn1/builder/relative_distinguished_name_builder_state.h"
#include "amsr/asn1/structure/name.h"
#include "sequence_of_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for RDNSequence as per RFC 5912.
 */
class RdnSequenceBuilder : public SequenceOfBuilderState<RelativeDistinguishedNameBuilderState, RdnSequenceBuilder> {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using SequenceOfBuilderState::SequenceOfBuilderState;

  /*!
   * \brief Getter for the internal transition graph for this builder.
   * \return The transitions.
   */
  static auto GetTransitions() noexcept -> ara::core::Vector<std::pair<ElementInput, ElementIdentifier>>;
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_RDN_SEQUENCE_BUILDER_H_