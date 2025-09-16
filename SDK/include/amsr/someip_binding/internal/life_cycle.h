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
/*!        \file  someip_binding/internal/life_cycle.h
 *        \brief  Initialization/Deinitialization functions for someip_binding
 *        \unit   SomeIpBinding::SomeIpBinding::LifeCycleManager
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief       Initialize the Some/IP binding.
 * \return      Result storing no value or error, which indicates a positive result.
 * \error       amsr::someip_binding_core::internal::SomeIpBindingErrc::json_loading_failure
 *                if component specific JSON configuration could not be found or loaded.
 *              amsr::someip_binding_core::internal::SomeIpBindingErrc::json_parsing_failure
 *                if component specific JSON configuration parsing failed.
 *              amsr::someip_binding_core::internal::SomeIpBindingErrc::wrong_init_sequence
 *                if the Some/IP binding is already initialized.
 * \pre         This function must only be called once.
 * \context     Init
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \vprivate
 * \synchronous TRUE
 */
// VECTOR NC AutosarC++17_10-M3.2.3: MD_SOMEIPBINDING_AutosarC++17_10-M3.2.3_function_declared_more_than_one_file
amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief       Deinitialize the Some/IP binding.
 * \return      Result storing no value or error, which indicates a positive result.
 * \pre         The related reactor thread must be stopped.
 * \context     Shutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \vprivate
 * \synchronous TRUE
 */
// VECTOR NC AutosarC++17_10-M3.2.3: MD_SOMEIPBINDING_AutosarC++17_10-M3.2.3_function_declared_more_than_one_file
amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief       Get initialization status of the SOME/IP Binding component.
 * \return      true if the SOME/IP Binding is initialized, otherwise false.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
bool IsComponentInitialized() noexcept;

/*!
 * \brief       Start the SOME/IP Binding component.
 *
 * \pre         Component is initialized.
 * \context     INIT
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 */
void Start() noexcept;

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_H_
