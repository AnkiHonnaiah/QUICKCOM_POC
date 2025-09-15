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
/*!        \file  ara/com/e2e_types.h
 *        \brief  E2E types used by Socal API
 *      \details  Specific types needed by the Socal API for E2E.
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_E2E_TYPES_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_E2E_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/e2e_result.h"

namespace ara {
namespace com {

namespace E2E_state_machine {
/*!
 * \brief Represents the results of the check of a single sample.
 * \vpublic
 *
 * \trace SPEC-4980338
 */
using E2ECheckStatus = ::amsr::e2e::state_machine::E2ECheckStatus;

/*!
 * \brief Represents the state if samples can be used or not.
 * \vpublic
 *
 * \trace SPEC-4980339
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_Can_be_moved_inside_function
using E2EState = ::amsr::e2e::state_machine::E2EState;
}  // namespace E2E_state_machine

namespace e2e {
/*!
 * \brief Represents the e2e result.
 * \vpublic
 *
 * \trace SPEC-4980052, SPEC-4980057
 */
using Result = ::amsr::e2e::Result;

/*!
 * \brief Represents the E2E profile check status of a single sample.
 * \vpublic
 *
 * \trace SPEC-4980338
 */
using ProfileCheckStatus = ::amsr::e2e::state_machine::E2ECheckStatus;

/*!
 * \brief Type alias to E2E state enum.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_Can_be_moved_inside_function
using SMState = ::amsr::e2e::state_machine::State;
}  // namespace e2e

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_E2E_TYPES_H_
