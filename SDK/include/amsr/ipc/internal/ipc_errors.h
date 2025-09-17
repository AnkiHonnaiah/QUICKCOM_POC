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
 *        \brief  Provides error mappings for Reactor errors.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *        \unit   osabstraction::ipc::IpcOsInterface_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_IPC_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_IPC_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/error_code.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief       Maps Reactor1::Register() errors.
 *
 * \details     Aborts on unexpected errors.
 *
 * \param[in]   error   Error code returned by the API.
 *
 * \return      The mapped error
 *
 * \retval      osabstraction::OsabErrc::kResource   No memory to register another callback or system limit reached.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
auto MapReactorRegistrationErrors(::amsr::core::ErrorCode const& error) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief       Maps Reactor1::Unregister() errors.
 *
 * \details     Aborts on unexpected errors. As all errors are unexpected the return value is void and the function does
 *              not return.
 *
 * \param[in]   error   Error code returned by the Reactor1 API.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
[[noreturn]] void MapReactorUnregistrationErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief       Maps Reactor1::RegisterSoftwareEvent() errors.
 *
 * \details     Aborts on unexpected errors.
 *
 * \param[in]   error   Error code returned by the Reactor1 API.
 *
 * \return      The mapped error
 *
 * \retval      osabstraction::OsabErrc::kResource   No memory to register another callback.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
auto MapReactorSoftwareEventRegistrationErrors(::amsr::core::ErrorCode const& error) noexcept
    -> ::amsr::core::ErrorCode;

/*!
 * \brief       Maps Reactor1::UnregisterSoftwareEvent() errors.
 *
 * \details     Aborts on unexpected errors. As all errors are unexpected the return value is void and the function does
 *              not return.
 *
 * \param[in]   error   Error code returned by the Reactor1 API.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
[[noreturn]] void MapReactorSoftwareEventUnregistrationErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief       Maps Reactor1::TriggerSoftwareEvent() errors.
 *
 * \details     Aborts on unexpected errors. As all errors are unexpected the return value is void and the function does
 *              not return.
 *
 * \param[in]   error   Error code returned by the Reactor1 API.
 *
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
[[noreturn]] void MapReactorTriggerSoftwareEventErrors(::amsr::core::ErrorCode const& error) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_IPC_ERRORS_H_
