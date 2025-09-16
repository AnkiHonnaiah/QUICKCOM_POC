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
 *        \brief  Lifecycle Management of custom tracer instance.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CUSTOM_TRACER_LIFECYCLE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CUSTOM_TRACER_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/comtrace/trace_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {

/*!
 * \brief Life-Cycle Management of the registered custom tracer.
 *
 * \unit ComTrace::CustomTracerLifecycle
 */
class CustomTracerLifecycle final {
 public:
  // ---- Construction -------------------------------------------------------------------------------------------------

  CustomTracerLifecycle() noexcept = delete;

  CustomTracerLifecycle(CustomTracerLifecycle const&) noexcept = delete;
  auto operator=(CustomTracerLifecycle const&) & noexcept -> CustomTracerLifecycle& = delete;
  CustomTracerLifecycle(CustomTracerLifecycle&&) noexcept = delete;
  auto operator=(CustomTracerLifecycle&&) & noexcept -> CustomTracerLifecycle& = delete;

  /*!
   * \brief Destroy the custom tracer lifecycle manager.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~CustomTracerLifecycle() noexcept = default;

  // ---- Static APIs --------------------------------------------------------------------------------------------------

  /*!
   * \brief Get current custom tracer instance.
   *
   * \return Reference to the current tracer instance.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto GetTracer() noexcept -> TraceInterface&;

  /*!
   * \brief Set current custom tracer instance.
   *
   * \param[in] custom_tracer  Custom tracer instance to be set.
   *
   * \pre         -
   * \context     PreInit|PostShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void SetTracer(std::unique_ptr<TraceInterface> custom_tracer) noexcept;

  /*!
   * \brief   Reset the registered custom tracer implementation to the default implementation.
   * \details Release any the previously assigned custom tracer implementation.
   *
   * \pre         -
   * \context     PostShutdown|PreInit
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void ResetTracer() noexcept;

 private:
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_ComTrace_A3.3.2_CustomTracer_StaticStorageNonPOD
  /*!
   * \brief Global instance of the registered custom tracer.
   */
  static std::unique_ptr<TraceInterface> custom_tracer_;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CUSTOM_TRACER_LIFECYCLE_H_
