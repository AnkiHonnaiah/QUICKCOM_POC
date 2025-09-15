/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  state_pool.h
 *        \brief  The header file for the statemachine to get the current state and verify the validity.
 *         \unit  VaCommonLib::StateMachine
 *
 *      \details  GetState returns state of given handle and IsValid checks if handle refers to an existing state.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_
#define LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "vac/statemachine/state.h"
namespace vac {
namespace statemachine {

/*!
 * \brief  State Pool. The state pool is responsible for resources of states.
 * \tparam State Interface for State classes
 *         Limitations for State:
 *         - Must inherit from vac::statemachine::State<StateHandle, Context>
 *           Since vac::statemachine::State<StateHandle, Context> has deleted default constructor and copy constructor,
 *           copy assignment operator, move constructor and move assignment operator this means State must also have the
 *           special member functions deleted.
 * \trace  DSGN-VaCommonLib-StateMachine
 */
template <class State>
class StatePool {
 public:
  /*!
   * \brief Type of the State Handle.
   */
  using Handle = typename State::HandleType;

  /*!
   * \brief The Type of the Context.
   */
  using ContextType = typename State::ContextType;

  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  StatePool() noexcept {
    static_assert(std::is_base_of<vac::statemachine::State<Handle, ContextType>, State>::value,
                  "template parameter must derive from vac::statemachine::State<StateHandle, Context>");
  };

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief Copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  StatePool(StatePool const&) noexcept {
    static_assert(std::is_base_of<vac::statemachine::State<Handle, ContextType>, State>::value,
                  "template parameter must derive from vac::statemachine::State<StateHandle, Context>");
  };

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief Move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  StatePool(StatePool&&) noexcept {
    static_assert(std::is_base_of<vac::statemachine::State<Handle, ContextType>, State>::value,
                  "template parameter must derive from vac::statemachine::State<StateHandle, Context>");
  };

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief Default copy assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  StatePool& operator=(StatePool const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief Default move assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  StatePool& operator=(StatePool&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~StatePool() noexcept = default;

  /*!
   * \brief     Returns State of given handle.
   * \param     state_handle The state handle.
   * \return    The State with given handle. If handle does not refer to a State,
   *            exception of type std::out_of_range is thrown.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual State* GetState(Handle const state_handle) = 0;

  /*!
   * \brief     Checks if handle refers to an existing state.
   * \param     state_handle The state handle.
   * \pre       -
   * \return    True if state exists with given handle, otherwise returns false.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual bool IsValid(Handle const state_handle) const = 0;
};

}  // namespace statemachine
}  // namespace vac
#endif  // LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_
