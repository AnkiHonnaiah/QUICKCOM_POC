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
/*!        \file  state_owner.h
 *        \brief  The top header file of statemachine which includes state.h and state_pool.h.
 *         \unit  VaCommonLib::StateMachine
 *
 *      \details  GetState() returns the current state.
 *                TryChangeState changes current state to state associated to given handle.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_
#define LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/statemachine/state.h"
#include "vac/statemachine/state_pool.h"

namespace vac {
namespace statemachine {

/*!
 * \brief  Interface for classes owning a State.
 * \tparam State Interface for State classes.
 *         Limitations on State:
 *         - Must fulfill vac::statemachine::StatePool type requirements.
 * \trace  DSGN-VaCommonLib-StateMachine
 */
template <class State>
class StateOwner {
 public:
  /*!
   * \brief Type of the State Handle.
   */
  using Handle = typename State::HandleType;

  /*!
   * \brief Type of the State Context.
   */
  using Context = typename State::ContextType;

  /*!
   * \brief     Constructor.
   * \param[in] state_pool State pool for constructing the StateOwner.
   * \param     state_handle State handle for constructing the StateOwner.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  StateOwner(StatePool<State>& state_pool, Handle state_handle)
      : state_pool_{state_pool},
        state_{state_pool_.GetState(state_handle)} {  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    static_assert(std::is_base_of<vac::statemachine::State<Handle, Context>, State>::value,
                  "template parameter must derive from vac::statemachine::State<StateHandle, Context>");
  }

  /*!
   * \brief Copy constructor.
   */
  StateOwner(StateOwner const&) = delete;

  /*!
   * \brief Copy assignment.
   * \return
   */
  StateOwner& operator=(StateOwner const&) = delete;

  /*!
   * \brief Move constructor.
   */
  StateOwner(StateOwner&&) = delete;

  /*!
   * \brief Move assignment.
   * \return
   */
  StateOwner& operator=(StateOwner&&) = delete;

  /*!
   * \brief Destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~StateOwner() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_constCastReducesCodeDuplication
  /*!
   * \brief Returns the current state.
   * \return The current state.
   * \spec
   *  requires true;
   * \endspec
   */
  State* GetState() { return state_; }

  /*!
   * \brief Returns the current state.
   * \return The current state.
   * \spec
   *  requires true;
   * \endspec
   */
  State const* GetState() const { return state_; }

  /*!
   * \brief Returns the Context used by context depending State methods.
   * \pre   -
   * \return
   * \spec
   *  requires true;
   * \endspec
   */
  virtual Context& GetContext() = 0;

  /*!
   * \brief     Tries to change current state to state associated to given handle.
   * \param[in] state_handle The handle of the state to which change is requested.
   * \pre       -
   * \return    True if state change was successful, otherwise returns false.
   * \spec
   *  requires true;
   * \endspec
   * \trace     DSGN-VaCommonLib-StateMachine
   */
  bool TryChangeState(Handle const state_handle) {
    bool ret_value{false};
    if (state_->IsValidChange(state_handle)) {      // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      state_->OnLeave(                              // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
          GetContext());                            // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      state_ = state_pool_.GetState(state_handle);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      state_->OnEnter(                              // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
          GetContext());                            // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      ret_value = true;
    }
    return ret_value;
  }

 private:
  /*!
   * \brief Reference to associated State Pool.
   */
  StatePool<State>& state_pool_;

  /*!
   * \brief Pointer to current state.
   */
  State* state_;
};

}  // namespace statemachine
}  // namespace vac
#endif  // LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_
