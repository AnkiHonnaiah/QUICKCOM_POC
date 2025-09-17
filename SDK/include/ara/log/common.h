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
/**     \file       ara/log/common.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_ARA_LOGGING_INCLUDE_ARA_LOG_COMMON_H_
#define LIB_ARA_LOGGING_INCLUDE_ARA_LOG_COMMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"

namespace ara {
namespace log {
/*!
 * \brief  List of possible severity levels.
 *
 * \details As defined in SWS_LOG_00018
 * \details As defined in PRS_Dlt_00619
 *
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
using LogLevel = amsr::log::LogLevel;
/*!
 * \brief Log mode. Flags, used to configure the sink for log messages.
 *
 * \note In order to combine flags, at least the OR and AND operators needs to be provided for this type.
 *
 * \details As defined in SWS_LOG_00019
 * \vpublic
 */
using LogMode = amsr::log::LogMode;
}  // namespace log
}  // namespace ara

#endif  // LIB_ARA_LOGGING_INCLUDE_ARA_LOG_COMMON_H_
