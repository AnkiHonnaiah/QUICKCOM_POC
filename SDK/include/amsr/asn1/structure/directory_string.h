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
/*!        \file  amsr/asn1/structure/directory_string.h
 *        \brief  Representation of parsed DirectoryString.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DIRECTORY_STRING_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DIRECTORY_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief Type Alias
 */
using DirectoryString = amsr::core::Variant<structure::T61String, structure::UniversalString, structure::BMPString,
                                            structure::PrintableString, structure::Utf8String>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DIRECTORY_STRING_H_
