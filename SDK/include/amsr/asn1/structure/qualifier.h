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
/*!        \file  amsr/asn1/structure/qualifier.h
 *        \brief  Representation of parsed Qualifier structure.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_QUALIFIER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_QUALIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/oraddress.h"
#include "amsr/asn1/structure/other_name.h"
#include "amsr/asn1/structure/user_notice.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Type alias of Qualifier structure
 */

using Qualifier = amsr::core::Variant<structure::Ia5String, structure::UserNotice>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_QUALIFIER_H_
