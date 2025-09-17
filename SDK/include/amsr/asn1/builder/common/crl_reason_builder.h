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
/*!        \file  crl_reason_builder.h
 *        \brief  Builder for CRLReason
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CRL_REASON_BUILDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CRL_REASON_BUILDER_H_

#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/crl_reason.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder for CRLReason enumerated value
 */
class CrlReasonBuilder : public EnumeratedBuilderState {
 public:
  /*!
   * \brief Constructor for state.
   * \param[in] parent Parent state.
   */
  explicit CrlReasonBuilder(Asn1Builder* parent = nullptr) : EnumeratedBuilderState(parent) {
    SetConstraint(
        std::make_shared<builder::ValueConstraint<structure::Enumerated>>(ara::core::Vector<structure::Enumerated>{
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kUnspecified)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kKeyCompromise)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kCaCompromise)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kAffiliationChanged)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kSuperseded)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kCessationOfOperation)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kCertificateHold)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kRemoveFromCRL)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kPrivilegeWithdrawn)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::CrlReason::kAaCompromise))}));
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_CRL_REASON_BUILDER_H_
