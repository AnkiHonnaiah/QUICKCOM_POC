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
/**     \file
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_NS_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_NS_H_

// VECTOR Disable AutosarC++17_10-A16.0.1: MD_LOGU_AutosarC++17_10-A16.0.1_ifdef
/*!
 * \brief Centralized toggle for the LogStream operators provided by log-utility.
 */
#ifdef AMSR_LOGUTI_IS_AMSR_LOGGING
#define AMSR_LOGUTI_LOG_NS amsr
#else
#define AMSR_LOGUTI_LOG_NS ara
#endif
// VECTOR Enable AutosarC++17_10-A16.0.1: MD_LOGU_AutosarC++17_10-A16.0.1_ifdef

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_NS_H_
