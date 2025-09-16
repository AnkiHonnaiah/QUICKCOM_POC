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
 *        \brief  Public APIs to set a trace monitor implementation.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_H_
#define LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_H_

#include "trace_if.h"

namespace amsr {
namespace ipc_binding {

/*!
 * \brief Set the used trace monitor.
 *
 * \param[in,out] trace_if  Pointer to a binding specific trace implementation. Pass \c nullptr to unset. The pointer
 *                          shall remain valid until SetTraceImpl has been called (and has returned) again, changing or
 *                          resetting the trace implementation.
 *
 * \pre         The function is not called from a TraceIf callout.
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady      FALSE
 *
 * \unit IpcBinding::IpcBindingCore::TraceMonitor
 */
void SetTraceImpl(TraceIf* trace_if);

}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_AMSR_IPC_BINDING_TRACE_H_
