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
/*!        \file  ocsp_response_status.h
 *        \brief  Enumerators for OCSPResponse (RFC 6960) type.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_OCSP_RESPONSE_STATUS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_OCSP_RESPONSE_STATUS_H_

#include <cstdint>

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Enumerators for OCSPResponseStatus type.
 */
enum class OcspResponseStatus : std::uint8_t {
  kSuccessful = 0,
  kMalformedRequest = 1,
  kInternalError = 2,
  kTryLater = 3,
  kSigRequired = 5,
  kUnauthorized = 6
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_OCSP_RESPONSE_STATUS_H_
