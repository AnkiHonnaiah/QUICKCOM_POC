/**********************************************************************************************************************
 *  COPYRIGHT
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
/*!
 *      \file traits.h
 *      \brief Type traits checks.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TRAITS_H_
#define LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TRAITS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <type_traits>
#include "ara/core/string.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace traits {

/*!
 * \brief Check if the provided template type is a ara::core::BasicString.
 * \tparam T Type to be checked.
 */
template <typename T>
struct IsBasicString : public std::false_type {};

/*!
 * \brief Check if the provided template type is a ara::core::BasicString.
 * \tparam Alloc allocator
 */
template <typename Alloc>
struct IsBasicString<ara::core::BasicString<Alloc>> : public std::true_type {};

}  // namespace traits
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TRAITS_H_
