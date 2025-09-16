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
 *        \brief Definition of factory for trace sinks.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_H_
#define LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/comtrace/internal/client_trace_interface.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding_xf_common {
namespace internal {
namespace trace {

/*!
 * \brief A factory to create trace sinks dispatching traces to a client tracer provided by ComTrace.
 *
 * \unit IpcBinding::IpcBindingCore::TraceSinkFactory
 */
class TraceSinkFactory final : public TraceSinkFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ClientTraceInterface.
   */
  using ClientTraceInterface = ::amsr::comtrace::internal::ClientTraceInterface;

  // ---- Construction / Destruction ----

  /*!
   * \brief Construct a trace sink factory.
   *
   * \param[in] client_tracer  Shared pointer to the client tracer to forward trace calls to.
   *
   * \pre         Client tracer not null.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  explicit TraceSinkFactory(
      std::shared_ptr<ClientTraceInterface> client_tracer) noexcept;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface::~TraceSinkFactoryInterface
   */
  ~TraceSinkFactory() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  TraceSinkFactory(TraceSinkFactory const&) noexcept = delete;
  auto operator=(TraceSinkFactory const&) noexcept -> TraceSinkFactory& = delete;
  TraceSinkFactory(TraceSinkFactory&&) noexcept = delete;
  auto operator=(TraceSinkFactory&&) noexcept -> TraceSinkFactory& = delete;

  // ---- Factory functions ----

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface::CreateTraceEventSink
   */
  auto CreateTraceEventSink(EventConfig const& event_config) const noexcept
      -> std::unique_ptr<TraceEventSinkInterface> final;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \copydoc amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface::CreateTraceMethodSink
   */
  auto CreateTraceMethodSink(MethodConfig const& method_config) const noexcept
      -> std::unique_ptr<TraceMethodSinkInterface> final;  // IGNORE_BLACKLIST BSOCIAL-8805

 private:
  /*!
   * \brief Type alias for the used Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief The client trace to forward trace calls to.
   */
  std::shared_ptr<ClientTraceInterface> client_tracer_;  // IGNORE_BLACKLIST BSOCIAL-8805
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_xf_common
}  // namespace amsr

#endif  // LIB_IPC_BINDING_XF_COMMON_INCLUDE_IPC_BINDING_XF_COMMON_INTERNAL_TRACE_TRACE_SINK_FACTORY_H_
