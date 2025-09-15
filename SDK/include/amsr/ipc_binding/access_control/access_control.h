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
 *        \brief  Public APIs to set and reset an access control implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_ACCESS_CONTROL_ACCESS_CONTROL_H_
#define LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_ACCESS_CONTROL_ACCESS_CONTROL_H_

#include "ara/core/result.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"

namespace amsr {
namespace ipc_binding {
namespace access_control {

/*!
 * \brief Shortcut alias for AccessControlInterface.
 */
using AccessControlInterface = ipc_binding_core::internal::access_control::AccessControlInterface;

/*!
 * \brief   Set the user-defined access control implementation.
 * \details The user can define a custom access control implementation, that have to be set before ipc binding is
 *          initialized.
 *
 * \param[in,out] access_control  Non-empty pointer to a user-defined access control implementation.
 *
 * \pre         When a user-defined access control implementation is used, it must be called before
 *              amsr::core::Initialize().
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
void SetAccessControl(
    std::unique_ptr<AccessControlInterface> access_control) noexcept;  // IGNORE_BLACKLIST BSOCIAL-8805

/*!
 * \brief Reset the user-defined access control implementation to the default implementation.
 *
 * \pre         The function must be called after : amsr::core::Deinitialize().
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
void ResetAccessControl() noexcept;

}  // namespace access_control
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_ACCESS_CONTROL_ACCESS_CONTROL_H_
