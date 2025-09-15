/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!    \file  amsr/core/instance_specifier_error_domain.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_INSTANCE_SPECIFIER_ERROR_DOMAIN_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_INSTANCE_SPECIFIER_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/instance_specifier_error_domain.h"

namespace amsr {
namespace core {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief Alias for ara::core::InstanceSpecifier.
 */
using InstanceSpecifierErrc = ara::core::InstanceSpecifierErrc;

/*!
 * \brief Alias for ara::core::InstanceSpecifierException.
 */
using InstanceSpecifierException = ara::core::InstanceSpecifierException;

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief Alias for ara::core::InstanceSpecifierErrorDomain.
 */
using InstanceSpecifierErrorDomain = ara::core::InstanceSpecifierErrorDomain;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief Alias for ara::core::GetInstanceSpecifierErrorDomain().
 */
using ara::core::GetInstanceSpecifierErrorDomain;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief Alias for ara::core::MakeErrorCode() for InstanceSpecifierErrorDomain.
 */
using ara::core::MakeErrorCode;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_INSTANCE_SPECIFIER_ERROR_DOMAIN_H_
