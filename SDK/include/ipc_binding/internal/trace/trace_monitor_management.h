
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
 *        \brief  Internal functions for setting and accessing the packet tracing interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TRACE_TRACE_MONITOR_MANAGEMENT_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TRACE_TRACE_MONITOR_MANAGEMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_binding/trace_if.h"

#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace trace {

/*!
 * \brief Set the user-specified interface for packet tracing callouts.
 *
 * \param[in,out] trace_if  Pointer to a binding specific trace implementation. Pass \c nullptr to unset.
 *
 * \pre         The ara::core::Initialize() has been called.
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::TraceMonitor
 */
void SetTraceIf(::amsr::ipc_binding::TraceIf* trace_if);

/*!
 * \brief Get the trace monitor for packet tracing.
 *
 * \return The trace monitor.
 *
 * \pre         -
 * \context     Any
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 *
 * \unit IpcBinding::IpcBindingCore::TraceMonitor
 */
auto GetTraceMonitor() -> ::amsr::ipc_binding_core::internal::trace::TraceMonitor const&;

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TRACE_TRACE_MONITOR_MANAGEMENT_H_
