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
 *         \brief  An interface class that allows its derived class to track its initialization status.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_INITIALIZE_STATE_MACHINE_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_INITIALIZE_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief   Interface class that helps tracking the initialization state.
 * \details The user can overwrite the OnInitialize() and OnDeinitialize() to specify what shall be done in calls to
 *          the predefined Initialize() and Deinitialize() methods. The functions AbortIfNotInitialized() and
 *          AbortIfInitialized() can be used in all other methods that require that the class is initialized or
 *          uninitialized, respectively.
 *
 * \unit    ZeroCopyBinding::InitializeStateMachine
 */
class InitializeStateMachine {
 public:
  /*!
   * \brief       Initialize the InitializeStateMachine.
   * \context     Init
   * \pre         The InitializeStateMachine object is not already initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void Initialize() noexcept;

  /*!
   * \brief       Deinitialize the InitializeStateMachine.
   * \context     Shutdown
   * \pre         The InitializeStateMachine object is initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void Deinitialize() noexcept;

  InitializeStateMachine() = delete;
  InitializeStateMachine(InitializeStateMachine const&) = delete;
  InitializeStateMachine(InitializeStateMachine&&) = delete;
  auto operator=(InitializeStateMachine const&) -> InitializeStateMachine& = delete;
  auto operator=(InitializeStateMachine&&) -> InitializeStateMachine& = delete;

 protected:
  /*!
   * \brief       Constructor of InitializeStateMachine.
   * \param[in]   logger_prefix  A prefix from which the logger will be constructed.
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  explicit InitializeStateMachine(::amsr::core::StringView logger_prefix);

  /*!
   * \brief       Destructor of InitializeStateMachine.
   * \context     Shutdown
   * \pre         The InitializeStateMachine object must not be initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  ~InitializeStateMachine() noexcept;

  /*!
   * \brief       Provides the stored logger to the derived class.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto GetLogger() noexcept -> logging::ZeroCopyLogger&;

  /*!
   * \brief       User specified method to be called in Initialize().
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual void OnInitialize() noexcept = 0;

  /*!
   * \brief       User specified method to be called in Deinitialize().
   * \context     Shutdown
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual void OnDeinitialize() noexcept = 0;

  /*!
   * \brief       Helper function that aborts with the specified error message if object is already initialized.
   * \param[in]   message  The message to be logged. Shall have static storage duration.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void AbortIfInitialized(::amsr::core::StringView message) const noexcept;

  /*!
   * \brief       Helper function that aborts with the specified error message if object is still deinitialized.
   * \param[in]   message  The message to be logged. Shall have static storage duration.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void AbortIfNotInitialized(::amsr::core::StringView message) const noexcept;

  /*!
   * \brief       Helper function that sets the current initialized state.
   * \details     This function shall only be used in derived classes that have to implement their own Initialize() or
   *              Deinitialize() functions, i.e., because the function shall have a different signature.
   * \param[in]   new_state  The state the initialization state machine will be set to.
   * \context     Init | Shutdown
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void SetInitializationState(bool new_state) noexcept;

 private:
  /*!
   * \brief       Helper function that aborts with the specified error message.
   * \param[in]   message  The message to be logged. Shall have static storage duration.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void AbortWithErrorMessage(::amsr::core::StringView message) const noexcept;

  /*!
   * \brief Indicator for the initialization status.
   */
  bool initialized_{false};

  /*!
   * \brief The logger.
   */
  logging::ZeroCopyLogger logger_;
};

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_INITIALIZE_STATE_MACHINE_H_
