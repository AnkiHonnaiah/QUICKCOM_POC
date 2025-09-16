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
/*!        \file  unknown_info.h
 *        \brief  Representation of parsed UnknownInfo
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_UNKNOWN_INFO_H
#define AMSR_VECTOR_FS_VAASN1_UNKNOWN_INFO_H

#include "asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief UnknownInfo Structure (RFC 6960). Modeled as an additional null to be distinguished from a "good" response via
 * its tag.
 */
class UnknownInfo : public Null {};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_UNKNOWN_INFO_H
