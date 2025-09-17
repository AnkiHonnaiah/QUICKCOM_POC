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
 *        \brief  Initialization / Deinitialization functions for ComTrace
 *      \details
 *         \unit  ComTrace::ComponentLifecycle
 *
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_LIFE_CYCLE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_LIFE_CYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace amsr {
namespace comtrace {
namespace internal {

/*!
 * \brief Initialize the ComTrace component.
 *
 * \return Empty result value or an error.
 *
 * \error ComTraceErrc::json_parsing_failure  If component specific JSON configuration parsing failed.
 *
 * \pre         InitializeComponent was not called before.
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto InitializeComponent() noexcept -> amsr::core::Result<void>;

/*!
 * \brief Deinitialize the ComTrace component.
 *
 * \return Always a positive empty result.
 *
 * \pre         DeinitializeComponent was not called before.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto DeinitializeComponent() noexcept -> amsr::core::Result<void>;

/*!
 * \brief Get initialization status of the ComTrace component.
 *
 * \return True if the component is initialized, otherwise false.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 */
auto IsComponentInitialized() noexcept -> bool;

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_LIFE_CYCLE_H_
