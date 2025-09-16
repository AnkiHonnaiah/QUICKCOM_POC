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
 *        \brief  ProfileCheckerImpl class for pimpl idiom.
 *
 *      \details  Contains the declarations for the ProfileCheckerImpl class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

extern "C" {
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_SM.h"
}

// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "ara/log/logging.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "vac/container/static_vector.h"

// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "amsr/e2e/state_machine/state_machine.h"

namespace amsr {
namespace e2e {
namespace state_machine {

/*!
 * \brief Pimpl idiom class for State Machine that exists because E2E_SM.h shall not be included in check_status.h /
 *        state_machine.h / state.h.
 * \unit amsr::e2e::state_machine::StateMachine
 * \vprivate Vector component internal class
 */
// VCA_E2E_SLC15_DESTRUCTOR
class StateMachine::StateMachineImpl final {
 public:
  /*!
   * \brief Constructor that requires a configuration.
   * \param[in] profile_configuration Configuration for the State Machine.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \vprivate Vector component internal constructor
   * \trace SPEC-12100854, SPEC-12100855, SPEC-12100856, SPEC-12100857, SPEC-12100858, SPEC-12100859, SPEC-12100860
   * \trace SPEC-12100861
   */
  explicit StateMachineImpl(profiles::E2EProfileConfiguration const& profile_configuration) noexcept;

  /*!
   * \brief Update E2E state machine due to a Check() event.
   * \param[in] check_status E2E check status of a single event sample
   * \return Current E2E state machine state
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector component internal method
   */
  State Check(CheckStatus check_status) noexcept;

 private:
  /*!
   * \brief Alias for status window type.
   */
  using StatusWindowType = vac::container::StaticVector<uint8>;

  /*!
   * \brief Logger instance.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief E2E state machine status window.
   */
  StatusWindowType check_status_window_;

  /*!
   * \brief E2E State Machine Check state.
   */
  E2E_SMCheckStateType check_state_;

  /*!
   * \brief E2E State Machine Configuration.
   */
  E2E_SMConfigType config_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, StateMachine);
};

}  // namespace state_machine
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_IMPL_H_
