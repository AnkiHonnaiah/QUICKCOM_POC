/*!********************************************************************************************************************
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
/*!        \file
 *        \brief  Compile time configuration options for logging.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_LOGGING_CFG_H_
#define LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_LOGGING_CFG_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/location.h"

namespace amsr {
namespace zero_copy_common {
namespace common {
namespace logging {

// VECTOR NL AutosarC++17_10-M7.3.6: MD_ZEROCOPYCOMMON_M7.3.6_sv
using vac::container::literals::operator"" _sv;

/*!
 *  MemCon Logging context ID.
 */
constexpr static vac::container::CStringView kZeroCopyCommonLogID{"vZCC"_sv};

/*!
 *  MemCon Logging Description.
 */
constexpr static vac::container::CStringView kZeroCopyCommonLogDescription{"Logger for ZeroCopyCommon libraries"_sv};

}  // namespace logging
}  // namespace common
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_LOGGING_CFG_H_
