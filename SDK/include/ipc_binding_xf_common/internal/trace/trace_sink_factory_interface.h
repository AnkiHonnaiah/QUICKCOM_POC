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
/**        \file
 *        \brief Definition of interface for a trace sink factory.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ipc_binding_core/internal/runtime_configuration/event_config.h"
#include "ipc_binding_core/internal/runtime_configuration/method_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_event_sink_interface.h"
#include "ipc_binding_xf_common/internal/trace/trace_method_sink_interface.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief Interface for a factory of trace sinks.
 *
 * \unit IpcBinding::IpcBindingCore::TraceSinkFactory
 */
class TraceSinkFactoryInterface {
 public:
  /*!
   * \brief Type alias for EventConfig.
   */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;

  /*!
   * \brief Type alias for MethodConfig.
   */
  using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;

  /*!
   * \brief Type alias for TraceEventSinkInterface.
   */
  using TraceEventSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface;

  /*!
   * \brief Type alias for TraceMethodSinkInterface.
   */
  using TraceMethodSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceMethodSinkInterface;

  // ---- Construction / Destruction ----

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~TraceSinkFactoryInterface() noexcept = default;

  TraceSinkFactoryInterface(TraceSinkFactoryInterface const&) noexcept = delete;
  auto operator=(TraceSinkFactoryInterface const&) noexcept -> TraceSinkFactoryInterface& = delete;
  TraceSinkFactoryInterface(TraceSinkFactoryInterface&&) noexcept = delete;
  auto operator=(TraceSinkFactoryInterface&&) noexcept -> TraceSinkFactoryInterface& = delete;

  // ---- Factory functions ----

  /*!
   * \brief Create a trace event sink for an event configuration.
   *
   * \param[in] event_config  The event configuration.
   *
   * \return Unique pointer to the new trace event sink.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto CreateTraceEventSink(EventConfig const& event_config) const noexcept
      -> std::unique_ptr<TraceEventSinkInterface> = 0;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief Create a trace method sink for an event configuration.
   *
   * \param[in] method_config  The method configuration.
   *
   * \return Unique pointer to the new trace method sink.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto CreateTraceMethodSink(MethodConfig const& method_config) const noexcept
      -> std::unique_ptr<TraceMethodSinkInterface> = 0;  // IGNORE_BLACKLIST BSOCIAL-8805

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceSinkFactoryInterface() noexcept = default;
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_INTERFACE_H_
