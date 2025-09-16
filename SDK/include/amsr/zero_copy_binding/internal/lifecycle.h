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
 *         \brief  Lifecycle for the binding.
 *         \unit   ZeroCopyBinding::ZeroCopyBinding::LifeCycle
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFECYCLE_H_
#define LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace amsr {
namespace zero_copy_binding {
namespace internal {

/*!
 * \brief       Initialize the ZeroCopyBinding component.
 * \return      Empty result value or an error.
 * \error       ComErrc::kWrongInitSequence
 *              if the component is already initialized.
 *
 * \pre         -
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate Product private
 */
auto InitializeComponent() noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief       Deinitialize the ZeroCopyBinding component.
 * \return      Empty result value or an error.
 * \error       ComErrc::kWrongInitSequence
 *              if the component is already deinitialized.
 *
 * \pre         All proxy and skeleton instances using ZeroCopyBinding deployment must have been destroyed.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate Product private
 */
auto DeinitializeComponent() noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief       Get initialization status of the ZeroCopyBinding component.
 * \return      True if the ZeroCopyBinding is initialized, false otherwise.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate Component private
 */
auto IsComponentInitialized() noexcept -> bool;

}  // namespace internal
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFECYCLE_H_
