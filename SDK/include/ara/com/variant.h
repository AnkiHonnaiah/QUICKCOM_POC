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
/*!        \file  variant.h
 *        \brief  Variant implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_VARIANT_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/variant.h"

namespace ara {
namespace com {

/*!
 * \brief  Using directive for ara::com::Variant<Alternatives...>.
 * \remark This will be potentially replaced with ara::core::Variant<Alternatives...>.
 * \vpublic
 *
 * \trace SPEC-5951137
 */
template <typename... Alternatives>
using Variant = ::ara::core::Variant<Alternatives...>;

/*!
 * \brief Using directive for ara::com::variant_alternative_t<I, T>.
 * \tparam I  Index of alternative, allowed unsigned value range cannot be smaller than: [0 - 65535].
 * \tparam T  Type of variant.
 * \vpublic
 */
template <std::size_t I, class T>
using variant_alternative_t = typename ::ara::core::variant_alternative_t<I, T>;

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_VARIANT_H_
