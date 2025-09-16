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
 *        \brief  E2E state machine declaration
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "vac/testing/test_adapter.h"

#include "amsr/e2e/e2e_types.h"
#include "amsr/e2e/state_machine/internal/state_machine_interface.h"

namespace amsr {
namespace e2e {
namespace state_machine {

/*!
 * \brief E2E state machine implementation
 * \unit amsr::e2e::state_machine::StateMachine
 * \vprivate Vector Internal API
 */
class StateMachine final : public internal::StateMachineInterface {
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
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  explicit StateMachine(profiles::E2EProfileConfiguration const& profile_configuration) noexcept;

  /*!
   * \brief Default Destructor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ~StateMachine() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  StateMachine() = delete;
  StateMachine(StateMachine const&) = delete;
  StateMachine(StateMachine&&) = delete;
  StateMachine& operator=(StateMachine const&) = delete;
  StateMachine& operator=(StateMachine&&) = delete;

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
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  State Check(CheckStatus check_status) noexcept final;

 private:
  // Forward declaration because this header must not include any C header file
  class StateMachineImpl;

  /*!
   * \brief Pimpl Idiom used here because this header must not include any C header file.
   */
  std::unique_ptr<StateMachineImpl> p_impl_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, StateMachine);
};

}  // namespace state_machine
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_STATE_MACHINE_STATE_MACHINE_H_
