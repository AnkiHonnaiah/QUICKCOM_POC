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
 *        \brief    Indirection to access MSRA ThreadConfig.
 *      \details    Indirections facilitate mocking of external libraries / generated source files in test suite.
 *
 **********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_SOCAL_EXTERNAL_THREAD_THREAD_CONFIG_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_SOCAL_EXTERNAL_THREAD_THREAD_CONFIG_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/thread/thread_config.h"

namespace amsr {
namespace socal {
namespace external {
namespace thread {

/*!
 * \brief Indirection to access MSRA amsr::thread::ThreadConfig.
 */
// VECTOR NL AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using ::amsr::thread::ThreadConfig;

/*!
 * \brief Indirection to access MSRA amsr::thread::GetThreadConfiguration().
 */
// VECTOR NL AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using ::amsr::thread::GetThreadConfiguration;

}  // namespace thread
}  // namespace external
}  // namespace socal
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_SOCAL_EXTERNAL_THREAD_THREAD_CONFIG_H_
