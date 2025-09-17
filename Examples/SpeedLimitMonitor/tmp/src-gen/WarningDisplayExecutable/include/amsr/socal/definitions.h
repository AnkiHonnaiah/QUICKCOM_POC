/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/include/amsr/socal/definitions.h
 *        \brief  Updates the runtime processing mode with configuration from ARXML.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOCAL_DEFINITIONS_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOCAL_DEFINITIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/configuration/configuration.h"

namespace amsr {
namespace socal {

/*!
 * \brief The configured runtime processing mode. This expression is only valid if
 *        araComRuntimeProcessingMode is configured in the ARXML model. If it is not configured,
 *        the value defaults to the processing mode ThreadDriven.
 * \vpublic
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr ::amsr::socal::internal::configuration::RuntimeProcessingMode kRuntimeProcessingMode{::amsr::socal::internal::configuration::RuntimeProcessingMode::kThreadDriven};

}  // namespace socal
}  // namespace amsr

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOCAL_DEFINITIONS_H_

