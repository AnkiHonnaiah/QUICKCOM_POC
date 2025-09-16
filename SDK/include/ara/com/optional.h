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
/*!        \file  optional.h
 *        \brief  Type alias of ara::core::Optional.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_OPTIONAL_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_OPTIONAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/optional.h"

namespace ara {
namespace com {

/*!
 * \brief Type-alias to conform to the AUTOSAR COM SWS that specifies ara::com::Optional.
 * \tparam T  Value type.
 * \vpublic
 *
 * \trace SPEC-4980294, SPEC-4980295, SPEC-4980296, SPEC-4980297, SPEC-4980298, SPEC-4980299, SPEC-4980300,
 *        SPEC-4980301, SPEC-4980302, SPEC-4980303, SPEC-4980304
 */
template <class T>
using Optional = ::ara::core::Optional<T>;

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_OPTIONAL_H_
