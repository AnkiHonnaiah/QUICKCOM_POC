
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
/*!        \file  lifecycle.h
 *        \brief  Provides services to initialize and de-initialize ara::com components.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_COMINTEGRATOR_INCLUDE_AMSR_COMINTEGRATOR_INTERNAL_LIFECYCLE_H_
#define LIB_COMINTEGRATOR_INCLUDE_AMSR_COMINTEGRATOR_INTERNAL_LIFECYCLE_H_

#include "amsr/core/result.h"

namespace amsr {
namespace comintegrator {
namespace internal {

/*!
 * \brief Initialize the ara::com sub components.
 * \return amsr::core::Result with no value.
 * \error ara::com::ComErrc::kWrongInitSequence  if the unit is already initialized.
 * \error ara::com::ComErrc::kJsonParsingFailed  if component specific JSON configuration parsing failed.
 * \error ara::com::ComErrc::kErrorNotOk         if unknown error.
 * \pre         ara::com is not yet initialized.
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Vector product private API.
 *
 * \complexity The generated function may call different components initialization APIs
 *             eventually heading towards an increased number of inter-module calls.
 *             No risk identified.
 * \unit       ComIntegrator::Lifecycle
 */
::amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief De-initialize the ara::com sub components.
 * \return amsr::core::Result with no value.
 * \error ara::com::ComErrc::kErrorNotOk  if unknown error.
 * \pre         ara::com is initialized successfully.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Vector product private API.
 *
 * \complexity The generated function may call different components deinitialization APIs
 *             eventually heading towards an increased number of inter-module calls.
 *             No risk identified.
 * \unit       ComIntegrator::Lifecycle
 */
::amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief Getter function for the initialization status of ara::com.
 * \return true if the component is initialized false otherwise.
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Vector product private API.
 *
 * \unit ComIntegrator::Lifecycle
 */
bool IsComponentInitialized() noexcept;

}  // namespace internal
}  // namespace comintegrator
}  // namespace amsr

#endif  // LIB_COMINTEGRATOR_INCLUDE_AMSR_COMINTEGRATOR_INTERNAL_LIFECYCLE_H_
