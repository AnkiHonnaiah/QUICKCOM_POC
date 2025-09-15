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
/*!        \file  amsr/asn1/context/contexts.h
 *        \brief  File to include when using VaAsn1 Contexts.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_CONTEXTS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_CONTEXTS_H_

// This file serves mostly to have a central inclusion point for all contexts at once, and to hide context.h.
// Including context.h can cause the template specializations to not be available unless compiled elsewhere.

#include "amsr/asn1/context/internal/common/context.h"

#include "amsr/asn1/context/internal/common/choice.h"
#include "amsr/asn1/context/internal/common/explicit.h"
#include "amsr/asn1/context/internal/common/nature_metafunctions.h"
#include "amsr/asn1/context/internal/common/optional.h"
#include "amsr/asn1/context/internal/common/sequence_of.h"
#include "amsr/asn1/context/internal/common/set_of.h"
#include "amsr/asn1/context/internal/common/tagged.h"
#include "amsr/asn1/context/internal/common/tags.h"
#include "amsr/asn1/context/internal/common/traits.h"
#include "amsr/asn1/context/internal/personal_name.h"
#include "amsr/asn1/context/internal/primitives.h"
#include "amsr/asn1/context/internal/rsassa_pss_params.h"

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_CONTEXTS_H_
