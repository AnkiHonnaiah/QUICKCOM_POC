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
/*!        \file  responder_id.h
 *        \brief  Representation of parsed ResponderID.
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_RESPONDER_ID_H
#define AMSR_VECTOR_FS_VAASN1_RESPONDER_ID_H

#include "amsr/core/variant.h"
#include "name.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Type Alias
 */
using ResponderId = amsr::core::Variant<Name, OctetString>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_RESPONDER_ID_H
