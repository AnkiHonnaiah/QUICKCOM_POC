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
/*!        \file  optional_traits.h
 *        \brief  Type-Traits relating to amsr::core::optional.
 *         \unit  VaCommonLib::MemoryManagement::Optional
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/core/optional.h"

namespace vac {
namespace memory {

/*!
 * \brief   Check if type is optional type.
 * \details Implementation if type is not optional.
 * \tparam  T The type to check if it is an optional type.
 */
template <typename T>
struct is_optional : std::false_type {};

/*!
 * \brief   Check if type is optional type.
 * \details Implementation if type is optional.
 * \tparam  T The type to construct amsr::core::Optional.
 */
template <typename T>
struct is_optional<amsr::core::Optional<T>> : std::true_type {};

}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_
