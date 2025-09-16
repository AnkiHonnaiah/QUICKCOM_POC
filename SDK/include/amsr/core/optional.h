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
/*!    \file  amsr/core/optional.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_OPTIONAL_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_OPTIONAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/optional.h"

namespace amsr {
namespace core {

/*!
 * \brief  Alias for ara::core::Optional.
 */
template <class T>
using Optional = ara::core::Optional<T>;

/*!
 * \brief  Alias for ara::core::nullopt_t.
 */
using nullopt_t = ara::core::nullopt_t;

// VECTOR Next Construct AutosarC++17_10-M17.0.2: MD_VAC_M17.0.2_standardLibraryObjectNameOverride
/*!
 * \brief Named token instance to use when constructing an empty Optional.
 */
constexpr nullopt_t nullopt{};

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::make_optional().
 */
using ara::core::make_optional;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_OPTIONAL_H_
