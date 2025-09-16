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
/*!     \file
 *      \brief      Header file for helper types & constants.
 *      \details    -
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_HELPER_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/c_string_view.h"

namespace amsr {
namespace loguti {
namespace internal {

/*! \brief The context ID that is reserved for logutility. */
constexpr vac::container::CStringView const kVctrCtxId{vac::container::CStringView::FromLiteral("VCTR", 4)};
/*! \brief An empty string that replaces prefixes. */
constexpr vac::container::CStringView const kEmptyString{vac::container::CStringView::FromLiteral("", 0)};
}  // namespace internal
}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_HELPER_H_
