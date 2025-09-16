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
/*!        \file  cert_status.h
 *        \brief  Representation of parsed CertStatus.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CERT_STATUS_H
#define AMSR_VECTOR_FS_VAASN1_CERT_STATUS_H

#include "amsr/core/variant.h"
#include "asn1_primitive_types.h"
#include "revoked_info.h"
#include "unknown_info.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Type Alias
 */
using CertStatus = amsr::core::Variant<Null, RevokedInfo, UnknownInfo>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_CERT_STATUS_H
