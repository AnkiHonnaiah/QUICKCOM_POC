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
/*!        \file
 *        \brief  Configuration file for logging.
 *
 *      \details  File should be generated in future
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMMON_COMPILE_TIME_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMMON_COMPILE_TIME_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/sync/strategy.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

/*!
 * \brief The sync strategy used for console logging.
 */
static constexpr sync::Strategy kConsoleSyncStrategy{sync::Strategy::kDefault};

/*!
 * \brief The sync strategy used for file logging.
 */
static constexpr sync::Strategy kFileSyncStrategy{sync::Strategy::kDefault};

/*!
 * \brief The sync strategy used for remote logging.
 */
static constexpr sync::Strategy kRemoteSyncStrategy{sync::Strategy::kDefault};

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_COMMON_COMPILE_TIME_CONFIG_H_
