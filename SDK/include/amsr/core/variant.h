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
/*!    \file  amsr/core/variant.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_VARIANT_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/variant.h"

namespace amsr {
namespace core {

/*!
 * \brief  Alias for ara::core::Variant.
 */
template <typename... Xs>
using Variant = ara::core::Variant<Xs...>;

/*!
 * \brief  Alias for ara::core::variant_alternative.
 */
template <std::size_t I, typename T>
using variant_alternative = ara::core::variant_alternative<I, T>;

/*!
 * \brief  Alias for ara::core::variant_alternative_t.
 */
template <std::size_t I, typename T>
using variant_alternative_t = ara::core::variant_alternative_t<I, T>;

/*!
 * \brief  Alias for ara::core::monostate.
 */
using monostate = ara::core::monostate;

/*!
 * \brief  Alias for ara::core::variant_size.
 */
template <typename T>
using variant_size = ara::core::variant_size<T>;

// VECTOR Next Construct AutosarC++17_10-M17.0.2: MD_VAC_M17.0.2_standardLibraryObjectNameOverride
/*!
 * \brief Alias for ara::core::variant_size_v.
 */
template <typename T>
constexpr std::size_t variant_size_v{variant_size<T>::value};

// VECTOR Next Construct AutosarC++17_10-M17.0.2: MD_VAC_M17.0.2_standardLibraryObjectNameOverride
/*!
 * \brief  Alias for ara::core::variant_npos.
 */
constexpr std::size_t variant_npos{ara::core::variant_npos};

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::holds_alternative().
 */
using ara::core::holds_alternative;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::get().
 */
using ara::core::get;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::get_if().
 */
using ara::core::get_if;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::visit().
 */
using ara::core::visit;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::swap() for ara::core::Variant.
 */
using ara::core::swap;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_VARIANT_H_
