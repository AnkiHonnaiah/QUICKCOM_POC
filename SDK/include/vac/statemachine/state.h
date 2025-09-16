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
/*!        \file  state.h
 *        \brief  The header file of the statemachine that defines states.
 *         \unit  VaCommonLib::StateMachine
 *
 *      \details  OnEnter, context sensitive method called when state is entered.
 *                OnLeave, context sensitive method called when state is left.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_H_
#define LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace vac {
namespace statemachine {

/*!
 * \brief  Interface for State classes.
 * \tparam StateHandle Enum type to identify which state.
 *         - Must be a type which fulfill C++ Named Requirements: CopyConstructable.
 * \tparam Context The context type of state machine.
 * \trace  DSGN-VaCommonLib-StateMachine
 */
template <typename StateHandle, class Context>
class State {
 public:
  /*!
   * \brief The Type of the StateHandle.
   *        The StateHandle is used for identifying a particular state by the value of a basic data type, such
   *        as enum values.
   */
  using HandleType = StateHandle;

  /*!
   * \brief The Type of the Context.
   */
  using ContextType = Context;

  /*!
   * \brief Constructor.
   * \pre   -
   * \param handle StateHandle to be constructed from.
   * \spec
   *  requires true;
   * \endspec
   */
  explicit State(StateHandle const handle) noexcept : handle_(handle) {}

  /*!
   * \brief Copy constructor.
   */
  State(State const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return
   */
  State& operator=(State const&) = delete;

  /*!
   * \brief Move constructor.
   */
  State(State&&) = delete;

  /*!
   * \brief Move assignment.
   * \return
   */
  State& operator=(State&&) = delete;

  /*!
   * \brief Destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~State() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-M7.1.2:MD_VAC_M7.1.2_parameterAsReferencePointerToConst
  /*!
   * \brief     Context sensitive method called when state is entered.
   * \param[in] context Context of state machine.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   * \trace     DSGN-VaCommonLib-StateMachine
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  virtual void OnEnter(Context& context) = 0;

  // VECTOR Next Construct AutosarC++17_10-M7.1.2:MD_VAC_M7.1.2_parameterAsReferencePointerToConst
  /*!
   * \brief     Context sensitive method called when state is left.
   * \param[in] context Context of state machine.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   * \trace     DSGN-VaCommonLib-StateMachine
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  virtual void OnLeave(Context& context) = 0;

  /*!
   * \brief     Checks if change from current state to state with given handle is valid.
   * \param[in] handle The handle of the requested state.
   * \pre       -
   * \return    True if change is valid, otherwise returns false.
   * \spec
   *  requires true;
   * \endspec
   * \trace     DSGN-VaCommonLib-StateMachine
   */
  virtual bool IsValidChange(StateHandle const handle) const = 0;

  /*!
   * \brief  Returns the handle of this State.
   * \return The handle of this State.
   * \spec
   *  requires true;
   * \endspec
   */
  StateHandle const GetHandle() const noexcept { return handle_; }

 private:
  /*!
   * \brief The handle for this State.
   */
  StateHandle const handle_;
};

}  // namespace statemachine
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_STATEMACHINE_STATE_H_
