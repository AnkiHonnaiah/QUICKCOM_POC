/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  crl_reason.h
 *        \brief  Enumerators for CRLReason (RFC 2459) type.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_CRL_REASON_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_CRL_REASON_H_

#include <cstdint>

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Enumerators for CRLReason (RFC 5280) type.
 */
enum class CrlReason : std::uint8_t {
  kUnspecified = 0,
  kKeyCompromise = 1,
  kCaCompromise = 2,
  kAffiliationChanged = 3,
  kSuperseded = 4,
  kCessationOfOperation = 5,
  kCertificateHold = 6,
  kRemoveFromCRL = 8,
  kPrivilegeWithdrawn = 9,
  kAaCompromise = 10
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_CRL_REASON_H_
