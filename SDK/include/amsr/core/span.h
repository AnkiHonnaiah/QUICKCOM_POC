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
/*!    \file  amsr/core/span.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_SPAN_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_SPAN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/span.h"

namespace amsr {
namespace core {

/*!
 * \brief A constant for creating Spans with dynamic sizes.
 */
constexpr std::size_t dynamic_extent{ara::core::dynamic_extent};

/*!
 * \brief Alias for ara::core::Span.
 */
template <typename T, std::size_t Extent = dynamic_extent>
using Span = ara::core::Span<T, Extent>;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief Alias for ara::core::MakeSpan().
 */
using ara::core::MakeSpan;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_SPAN_H_
