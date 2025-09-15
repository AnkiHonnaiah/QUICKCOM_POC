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
/*!        \file  ocsp_response_status_builder.h
 *        \brief  Builder for OCSPResponseStatus
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_OCSP_RESPONSE_STATUS_BUILDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_OCSP_RESPONSE_STATUS_BUILDER_H_

#include "amsr/asn1/builder/common/value_type_states.h"
#include "amsr/asn1/structure/ocsp_response_status.h"

namespace amsr {
namespace asn1 {
namespace builder {
/*!
 * \brief Builder for OcspResponseStatus enumerated value
 */
class OcspResponseStatusBuilder : public amsr::asn1::builder::EnumeratedBuilderState {
 public:
  /*!
   * \brief Constructor for state.
   * \param[in] parent Parent state.
   */
  explicit OcspResponseStatusBuilder(Asn1Builder* parent = nullptr) : EnumeratedBuilderState(parent) {
    SetConstraint(
        std::make_shared<builder::ValueConstraint<structure::Enumerated>>(ara::core::Vector<structure::Enumerated>{
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kSuccessful)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kMalformedRequest)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kInternalError)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kTryLater)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kSigRequired)),
            structure::Enumerated(static_cast<std::uint8_t>(structure::OcspResponseStatus::kUnauthorized))}));
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_OCSP_RESPONSE_STATUS_BUILDER_H_
