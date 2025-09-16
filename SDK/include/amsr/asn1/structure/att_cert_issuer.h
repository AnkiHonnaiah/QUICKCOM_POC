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
/*!        \file  amsr/asn1/structure/att_cert_issuer.h
 *        \brief  Representation of parsed AttCertIssuer.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_ISSUER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_ISSUER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/general_name.h"
#include "amsr/asn1/structure/v2_form.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Type alias of V1Form structure
 */
using V1Form = structure::GeneralNames;

/*!
 * \brief Type Alias
 */
using AttCertIssuer = amsr::core::Variant<structure::V1Form, structure::V2Form>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_ISSUER_H_
