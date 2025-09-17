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
 *        \brief  Declaration of the CheckStatus class.
 *
 *      \details  Contains the declarations for the CheckStatus class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_CHECK_STATUS_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_CHECK_STATUS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/e2e/profiles/proprietary/types.h"

namespace amsr {
namespace e2e {
namespace state_machine {

/*!
 * \brief Enumeration representing the results of the check of a single sample.
 * \vprivate Vector Internal API
 */
using CheckStatus = amsr::e2e::profiles::proprietary::CheckStatus;

/*!
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 * \vprivate Vector Internal API
 */
using E2ECheckStatus = CheckStatus;

}  // namespace state_machine
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_CHECK_STATUS_H_
