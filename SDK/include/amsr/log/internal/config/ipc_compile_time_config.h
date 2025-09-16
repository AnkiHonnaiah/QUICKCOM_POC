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
 *        \brief  Configuration elements for ipc related logging functionality.
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_IPC_COMPILE_TIME_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_IPC_COMPILE_TIME_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace log {
namespace internal {
namespace config {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
/*!
 * \brief The constant IPC domain used for the Log Daemon.
 *
 * \vprivate Component private.
 */
static constexpr std::uint32_t kIpcDomain{1001};

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_IPC_COMPILE_TIME_CONFIG_H_
