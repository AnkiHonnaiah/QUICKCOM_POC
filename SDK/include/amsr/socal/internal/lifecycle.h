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
/*!        \file  socal/internal/lifecycle.h
 *        \brief  Initialization/Deinitialization functions for SOCAL
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_H_

#include "amsr/core/result.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief   Starts the phase-1 initialization of Socal (Initializes internal runtime components).
 * \details The configuration data is expected to be stored inside "./etc/socal_config.json" relative to the working
 *          directory of the application.
 * \note    This shall be the first API call, to initialize Socal.
 * \return amsr::core::Result with no value or an error.
 * \error ara::com::ComErrc::kJsonParsingFailed  If component specific JSON configuration parsing failed.
 * \error ara::com::ComErrc::kWrongInitSequence  If the unit is already initialized.
 * \pre         InitializeComponent was not called before.
 * \pre         All instance specifiers from the thread assignment configuration must be known.
 * \pre         When using SomeIpBinding, SomeIpDaemon must be running and must accept connection requests if the
 *              connection retrial feature is disabled.
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
::amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief   Starts the phase-2 initialization of Socal (starts the dynamic communication).
 * \details This function initializes the ReactorThreadManager (which spawns the Reactor thread) and the
 *          ThreadPoolsManager (which starts the worker threads of the configured and default thread-pools). This must
 *          be last step of the initialization phase after all bindings have been initialized.
 * \note    This shall be the second API call to initialize Socal (after the call to InitializeComponent()).
 * \pre         Phase-1 of Socal has been initialized.
 * \pre         This API was not called before.
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
void Start() noexcept;

/*!
 * \brief   Starts the phase-1 de-initialization of Socal (stops the dynamic communication).
 * \details This function de-initializes the ReactorThreadManager (which stops the Reactor thread) and the
 *          ThreadPoolsManager (which stops the worker threads of the configured and default thread-pools).
 * \note    This shall be the first API call, to de-initialize Socal.
 * \return amsr::core::Result with no value or an error.
 * \error ara::com::ComErrc::kWrongInitSequence  If phase-1 de-initialization is already done.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
::amsr::core::Result<void> Stop() noexcept;

/*!
 * \brief Starts the phase-2 de-initialization of Socal (shuts down the runtime).
 * \note  This shall be the second API call, to de-initialize Socal (after the call to Stop()).
 * \return "amsr::core::Result" with no value and an error.
 * \pre         "StopFindService()" must have been called for all active "StartFindService()" requests.
 * \pre         Proxies and skeletons must have been stopped.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
::amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief Query the phase-1 initialization status of the component.
 * \return \c true if the component is initialized \c false otherwise.
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady TRUE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
bool IsComponentInitialized() noexcept;

/*!
 * \brief Query the overall ara::com initialization status.
 * \return \c true if both Socal (phase-1 and phase-2) and Bindings are initialized \c false otherwise.
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady TRUE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
bool IsComInitialized() noexcept;

/*!
 * \brief   Set the overall ara::com initialization status.
 * \details Is called by an integrator component.
 * \param[in] value  Boolean value to set the status to.
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private.
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Lifecycle
 */
void SetAraComInitStatus(bool const value) noexcept;

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_H_
