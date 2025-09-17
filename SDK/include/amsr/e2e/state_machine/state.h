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
 *        \brief  Declaration of the State class.
 *
 *      \details  Contains the declarations for the State class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace e2e {
namespace state_machine {

/*!
 * \brief Represents the state if samples can be used or not.
 * \vprivate Vector Internal API
 */
enum class State : std::uint8_t {
  /*!
   * \brief Communication of the samples of this event functioning properly according to e2e checks, sample(s) can be
   * used.
   */
  Valid = 0x00u,
  /*!
   * \brief State before amsr::e2e is initialized, sample cannot be used.
   */
  NoData = 0x02u,
  /*!
   * \brief No data from the publisher is available since the initialization, sample(s) cannot be used.
   */
  Init = 0x03u,
  /*!
   * \brief Communication of the sample of this event not functioning properly, sample(s) cannot be used.
   */
  Invalid = 0x04u
};

/*!
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 */
using E2EState = State;

}  // namespace state_machine
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_H_
