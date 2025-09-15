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
 *        \brief  Interface for StateMachine.
 *
 *      \details  Contains the declarations for the StateMachineInterface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_INTERNAL_STATE_MACHINE_INTERFACE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_INTERNAL_STATE_MACHINE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/state_machine/check_status.h"
#include "amsr/e2e/state_machine/state.h"

namespace amsr {
namespace e2e {
namespace state_machine {
namespace internal {

/*!
 * \brief Interface for State Machine used for testing purposes.
 * \vprivate Vector component internal base class
 */
class StateMachineInterface {
 public:
  /*!
   * \brief Alias for E2ECheckStatus
   * \vprivate Vector Internal API
   */
  using CheckStatus = amsr::e2e::state_machine::CheckStatus;

  /*!
   * \brief Alias for E2EState
   * \vprivate Vector Internal API
   */
  using State = amsr::e2e::state_machine::E2EState;

  /*!
   * \brief Default Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \vprivate Vector component internal constructor
   */
  StateMachineInterface() noexcept = default;

  /*!
   * \brief Default Destructor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \vprivate Vector component internal destructor
   */
  virtual ~StateMachineInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  StateMachineInterface(StateMachineInterface const&) = delete;
  StateMachineInterface(StateMachineInterface&&) = delete;
  StateMachineInterface& operator=(StateMachineInterface const&) = delete;
  StateMachineInterface& operator=(StateMachineInterface&&) = delete;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_E2E_M9.3.3
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
   * \vprivate Vector component internal pure virtual method
   */
  virtual State Check(CheckStatus check_status) noexcept = 0;
};

}  // namespace internal
}  // namespace state_machine
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_INTERNAL_STATE_MACHINE_INTERFACE_H_
