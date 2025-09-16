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
 *        \brief  Initialization/Deinitialization functions for ipc_binding
 *         \unit  IpcBinding::ComponentLifeCycleManagement
 *   \complexity  Unit accesses many units due to initialization/deinitialization functionality.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Initialize the IpcBinding component.
 *
 * \return Empty result value or an error.
 *
 * \error IpcBindingErrc::json_loading_failure  If component specific JSON configuration could not be found or loaded.
 * \error IpcBindingErrc::json_parsing_failure  If component specific JSON configuration parsing failed.
 * \error IpcBindingErrc::wrong_init_sequence   If the IpcBinding is already initialized.
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
 * \brief Deinitialize the IpcBinding component.
 *
 * \return Always a positive empty result.
 *
 * \pre         The related reactor thread must be stopped.
 * \pre         DeinitializeComponent was not called before.
 * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto DeinitializeComponent() noexcept -> amsr::core::Result<void>;

/*!
 * \brief Get initialization status of the IpcBinding component.
 *
 * \return True if the IpcBinding is initialized, otherwise false.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto IsComponentInitialized() noexcept -> bool;
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_H_
