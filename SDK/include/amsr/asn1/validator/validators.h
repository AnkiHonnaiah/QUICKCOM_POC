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
/*!        \file  amsr/asn1/validator/validators.h
 *        \brief  File to include when using VaAsn1 Validators.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATORS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATORS_H_

// This file serves mostly to have a central inclusion point for all validators at once, and to hide validator.h.
// Including validator.h can cause the template specializations to not be available unless compiled elsewhere.

#include "amsr/asn1/validator/internal/validator.h"

#include "amsr/asn1/validator/internal/personal_name.h"
#include "amsr/asn1/validator/internal/primitives/boolean.h"
#include "amsr/asn1/validator/internal/primitives/integer.h"

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATORS_H_
