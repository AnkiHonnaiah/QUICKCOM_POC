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
/*!       \file   amsr/asn1/builder/common/tbs_certificate_builder_state.h
 *        \brief  Builder for TBSCertificate as per RFC 5912.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_TBS_CERTIFICATE_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_TBS_CERTIFICATE_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/common/algorithm_identifier_builder_state.h"
#include "amsr/asn1/builder/common/explicitly_tagged_type_builder.h"
#include "amsr/asn1/builder/common/extension_builder_state.h"
#include "amsr/asn1/builder/common/extensions_builder_state.h"
#include "amsr/asn1/builder/common/name_builder_state.h"
#include "amsr/asn1/builder/common/sequence_builder_state.h"
#include "amsr/asn1/builder/common/subject_public_key_info_builder_state.h"
#include "amsr/asn1/builder/common/validity_builder_state.h"
#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/builder/primitive_type_builder_state.h"
#include "amsr/asn1/structure/tbs_certificate.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for TBSCertificate as per RFC 5912.
 */
class TBSCertificateBuilderState : public SequenceBuilderState<structure::TBSCertificate, TBSCertificateBuilderState> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] parent Parent builder or \c nullptr for root builders.
   */
  explicit TBSCertificateBuilderState(Asn1Builder* parent = nullptr) noexcept;

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
  auto DoYield() const noexcept -> amsr::core::Result<structure::TBSCertificate> override;

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
  ExplicitlyTaggedTypeBuilder<IntegerBuilderState> version_builder_{this}; /*!< Member builder: Version. */
  IntegerBuilderState serial_number_builder_{this};                        /*!< Member builder: Serial number. */
  AlgorithmIdentifierBuilderState signature_builder_{this};                /*!< Member builder: Signature. */
  NameBuilderState issuer_builder_{this};                                  /*!< Member builder: Issuer. */
  ValidityBuilderState validity_builder_{this};                            /*!< Member builder: ValidityInformation. */
  NameBuilderState subject_builder_{this};                                 /*!< Member builder: Subject. */
  SubjectPublicKeyInfoBuilderState subjectpublickeyinfo_builder_{this};    /*!< Member builder: SubjectPublicKeyInfo. */
  BitStringBuilderState issuer_unique_id_builder_{this};                   /*!< Member builder: IssuerUniqueID. */
  BitStringBuilderState subject_unique_id_builder_{this};                  /*!< Member builder: SubjectUniqueID. */
  ExplicitlyTaggedTypeBuilder<ExtensionsBuilderState, UniversalSequence> extensions_builder_{
      this}; /*!< Member builder: Extensions. */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_TBS_CERTIFICATE_BUILDER_STATE_H_
