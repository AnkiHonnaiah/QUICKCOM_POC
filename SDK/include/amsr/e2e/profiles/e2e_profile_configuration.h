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
 *        \brief  Declaration of the E2EProfileConfiguration struct.
 *
 *      \details  Contains the declarations for the E2EProfileConfiguration class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_E2E_PROFILE_CONFIGURATION_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_E2E_PROFILE_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Blueprint of an E2EProfileConfiguration.
 * \vprivate Vector Internal API
 */
struct E2EProfileConfiguration {
  /*!
   * \brief Maximum allowed difference between two counter values of two
   * consecutively received valid messages. For example, if the receiver gets
   * data with counter 1 and MaxDeltaCounter is 3, then at the next reception
   * the receiver can accept Counters with values 2, 3 or 4.
   */
  std::uint32_t const kMaxDeltaCounter;

  /*!
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK
   * was determined, within the last WindowSize checks, for the state Init.
   */
  std::uint8_t const kMinOkStateInit;

  /*!
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR
   * was determined, within the last WindowSize checks, for the state Init.
   */
  std::uint8_t const kMaxErrorStateInit;

  /*!
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK
   * was determined, within the last WindowSize checks, for the state Valid.
   */
  std::uint8_t const kMinOkStateValid;

  /*!
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR
   * was determined, within the last WindowSize checks, for the state Valid.
   */
  std::uint8_t const kMaxErrorStateValid;

  /*!
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK
   * was determined, within the last WindowSize checks, for the state
   * E2E_SM_INVALID.
   */
  std::uint8_t const kMinOkStateInvalid;

  /*!
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR
   * was determined, within the last WindowSize checks, for the state Invalid.
   */
  std::uint8_t const kMaxErrorStateInvalid;

  /*!
   * \brief Size of the monitoring window for the E2E state machine in the state Valid.
   */
  std::uint8_t const kWindowSizeValid;

  /*!
   * \brief Size of the monitoring window for the E2E state machine in the state Init.
   */
  std::uint8_t const kWindowSizeInit;

  /*!
   * \brief Size of the monitoring window for the E2E state machine in the state Invalid.
   */
  std::uint8_t const kWindowSizeInvalid;

  /*!
   * \brief Clear monitoring window data on transition to state Invalid.
   */
  bool kClearToInvalid;

  /*!
   * \brief Allow a direct transition from NoData to Invalid.
   */
  bool kTransitToInvalidExtended;
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_E2E_PROFILE_CONFIGURATION_H_
