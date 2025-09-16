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
/*!    \file  amsr/core/string_view.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_STRING_VIEW_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_STRING_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string_view.h"

namespace amsr {
namespace core {

/*!
 * \brief Alias for ara::core::basic_string_view.
 */
template <typename T>
using basic_string_view = ara::core::basic_string_view<T>;

/*!
 * \brief Alias for ara::core::basic_string_view<char>.
 */
using StringView = basic_string_view<char>;

/*!
 * \brief Alias for ara::core::basic_string_view<char16_t>.
 */
using u16string_view = basic_string_view<char16_t>;

/*!
 * \brief Alias for ara::core::basic_string_view<char32_t>.
 */
using u32string_view = basic_string_view<char32_t>;

}  // namespace core
}  // namespace amsr
#endif  // LIB_VAC_INCLUDE_AMSR_CORE_STRING_VIEW_H_
