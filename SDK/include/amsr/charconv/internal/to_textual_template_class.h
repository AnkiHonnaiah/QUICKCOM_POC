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
/*!        \file
 *        \brief  Converts a numerical value to a textual representation.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_TO_TEXTUAL_TEMPLATE_CLASS_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_TO_TEXTUAL_TEMPLATE_CLASS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace charconv {

/*!
 * \brief Converts a numerical value to a textual representation.
 * \tparam T Resulting data type.
 * \tparam Enable Selects data type category.
 * \vprivate Component Private
 */
template <class T, typename Enable = void>
class ToTextual final {};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_TO_TEXTUAL_TEMPLATE_CLASS_H_
