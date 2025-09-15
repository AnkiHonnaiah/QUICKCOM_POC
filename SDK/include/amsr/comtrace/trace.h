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
 *        \brief  Registration of custom tracer implementation.
 *      \details
 *         \unit  ComTrace::CustomTracerLifecycle
 *
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/comtrace/trace_interface.h"

namespace amsr {
namespace comtrace {

/*!
 * \brief Register a custom ara::com tracer implementation.
 *
 * \param[in] tracer_implementation  Unique pointer to the custom tracer implementation. ara::com will release its
 *                                   ownership of the tracer implementation with a call to ResetTracer().
 *
 * \pre         All ara::com components must be in state 'uninitialized'.
 * \context     PreInit|PostShutdown
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
auto SetTracer(std::unique_ptr<TraceInterface> tracer_implementation) noexcept -> void;

/*!
 * \brief   Reset registration of a custom tracer implementation.
 * \details After a call to this API a custom tracer implementation previously registered with SetTracer() is released
 *          and will not be used anymore by ara::com.
 *
 * \pre         All ara::com components must be in state 'uninitialized'.
 * \context     PostShutdown|PreInit
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
auto ResetTracer() noexcept -> void;

}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_H_
