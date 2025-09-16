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
 *        \brief  The IPC Binding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_MANAGEMENT_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_MANAGEMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ipc_binding_core/internal/access_control/access_control_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace access_control {

/*!
 * \brief Shortcut alias for AccessControlInterface.
 */
using AccessControlInterface = ipc_binding_core::internal::access_control::AccessControlInterface;

/*!
 * \brief   Set the user-defined access control implementation.
 * \details The user can define a custom access control implementation, that have to be set before ipcbinding is
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
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
void SetAccessControl(
    std::unique_ptr<AccessControlInterface> access_control) noexcept;  // IGNORE_BLACKLIST BSOCIAL-8805

/*!
 * \brief   Get the access control implementation.
 * \details Default implementation always grants access control. User-defined implementation can be set by
 *          "SetAccessControl".
 *
 * \return Reference to the access control implementation.
 *
 * \pre         -
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
auto GetAccessControl() noexcept -> ipc_binding_core::internal::access_control::AccessControlInterface&;

/*!
 * \brief   Reset the user-defined access control implementation to the default implementation.
 * \details The user can use this API to reset to default access control implementation after ipcbinding is
 *          deinitialized.
 *
 * \pre         The function must be called after amsr::core::Deinitialize().
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
void ResetAccessControl() noexcept;

}  // namespace access_control
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_MANAGEMENT_H_
