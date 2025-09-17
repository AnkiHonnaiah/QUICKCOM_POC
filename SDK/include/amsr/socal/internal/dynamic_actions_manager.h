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
/*!        \file  dynamic_actions_manager.h
 *        \brief  Contains phase-2 initialization and phase-1 de-initialization functionality of Socal
 *
 *      \details  Contains DynamicActionsManager class (for initialization and de-initialization of ReactorThreadManager
 *                and ThreadPool)
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_DYNAMIC_ACTIONS_MANAGER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_DYNAMIC_ACTIONS_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class to initialize the ReactorThreadManager and ThreadPool during the initialization of Socal. This class is
 *        meant to be used only for event-driven mode and must not be used if the application has configured polling
 *        runtime processing mode.
 *
 * \unit Socal::Lifecycle
 */
class DynamicActionsManager final {
 public:
  /*!
   * \brief   Starts the phase-2 initialization of Socal (starts the dynamic communication).
   * \details This function initializes the ReactorThreadManager (which spawns the Reactor thread) and the ThreadPools
   *          (which starts the worker threads of the configured and default thread-pools).
   * \pre         Phase-1 of Socal and the bindings have been initialized.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void Start() noexcept;

  /*!
   * \brief   Starts the phase-1 de-initialization of Socal (stops the dynamic communication).
   * \details This function de-initializes the ReactorThreadManager (which stops the Reactor thread) and the ThreadPools
   *          (which stops the worker threads of the configured and default thread-pools).
   * \pre         Socal and the bindings are still initialized.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void Stop() noexcept;

 private:
  /*!
   * \brief Initializes the thread-pools and and spawns their worker threads in the application process context.
   * \pre         Runtime and ThreadPoolsManager have been initialized.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void InitializeThreadPools() noexcept;

  /*!
   * \brief De-initializes the thread-pools and and stops their worker threads in the application process context.
   * \pre         Runtime and ThreadPoolsManager must be in initialized state.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void DeinitializeThreadPools() noexcept;

  /*!
   * \brief Initializes and spawns the reactor thread in the application process context.
   * \pre         Runtime has been initialized.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void InitializeReactorThreadManager() noexcept;

  /*!
   * \brief De-initializes and stops the reactor thread in the application process context.
   * \pre         Runtime must be in initialized state.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void DeinitializeReactorThreadManager() noexcept;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_DYNAMIC_ACTIONS_MANAGER_H_
