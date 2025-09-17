/**********************************************************************************************************************
 *  COPYRIGHT
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  ResourceUsage Linux utilities.
 *        \unit   osabstraction::resource_usage
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_LINUX_UTILITIES_H_
#define LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_LINUX_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace resource_usage {
namespace internal {

/*!
 * \brief           Splits the input string to string array based on the given delimiter character.
 *
 * \param[in]       file_data  amsr::core::String to split.
 * \param[in]       delimiter  delimiter character
 *
 * \return          vector of splitted string data.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::ara::core::Vector<::amsr::core::String> Split(::amsr::core::String const& file_data, char delimiter) noexcept;

/*!
 * \brief           Searches for regex pattern in input string.
 *
 * \param[in]       input_string       input string to search for regex pattern.
 * \param[in]       regex_pattern_str  regex pattern
 *
 * \return          vector of matching results.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::ara::core::Vector<::amsr::core::String> SearchForRegex(  // NOLINT(bugprone-exception-escape)
    ::amsr::core::String const& input_string, ::amsr::core::String const& regex_pattern_str) noexcept;

}  // namespace internal
}  // namespace resource_usage
}  // namespace amsr

#endif  // LIB_RESOURCEUSAGE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_LINUX_UTILITIES_H_
