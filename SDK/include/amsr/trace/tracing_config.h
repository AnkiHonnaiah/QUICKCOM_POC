/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/trace/tracing_config.h
 *        \brief  Statehandler class for trace: TracingConfig.
 *      \details  TracingConfig contains vectors with the enabled trace contexts and the tracesinks.
 *                TracingConfig is part of LoggerBase and shares its lifespan.
 *
 *         \unit LogAndTrace::Trace::TracingConfig
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACING_BASE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACING_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/vector.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/trace/tracesink.h"
#include "ara/core/vector.h"

namespace amsr {
namespace trace {
namespace config {

/*!
 * \brief Contains vectors for enabled contexts and trace sinks and interface for these.
 */
// VCA_LOGAPI_CHECKED_CONTRACT
class TracingConfig {
 public:
  /*! \brief Type for vector with enabled contexts. */
  using ContextVector = typename ara::core::Vector<meta::TraceContextId>;
  /*! \brief Type for vector with trace sinks. */
  using TraceSinkVector = typename ara::core::Vector<std::unique_ptr<amsr::trace::TraceSink>>;

  /*!
   * \brief  Initializes TracingConfig.
   * \return Result of initialization.
   */
  auto Initialize() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief  Deinitializes TracingConfig.
   * \return Result of deinitialization.
   */
  auto Deinitialize() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief  Get vector with enabled contexts.
   * \return Vector with enabled contexts.
   */
  auto GetEnabledContexts() const noexcept -> ContextVector const&;

  /*!
   * \brief  Get vector with trace sinks.
   * \return Vector with trace sinks.
   */
  auto GetTraceSinks() const noexcept -> TraceSinkVector const&;

  /*!
   * \brief  Set the enabled contexts from a vector of enabled contexts.
   * \param  enabled_context Vector with enabled contexts.
   */
  void SetEnabledContexts(ContextVector&& enabled_context) noexcept;

  /*!
   * \brief Add a TraceSink to the vector with trace sinks.
   * \param sink TraceSink to add.
   */
  void AddTraceSink(std::unique_ptr<TraceSink>&& sink) noexcept;

  /*!
   * \brief  Check if time point acquisition is enabled.
   * \return true if time point acquisition is enabled, false otherwise.
   */
  auto IsTimePointAcquisitionEnabled() const noexcept -> bool;
  /*!
   * \brief Set time point acquisition to given value.
   * \param new_value New value to set.
   */
  void SetTimePointAcquisition(bool new_value) noexcept;

  /*!
   * \brief   Reads the comma-seperated list from the environmental variable.
   *          Each item in this list is added to enabled_contexts.
   * \details If the list is malformed, i.e. double comma, an error is returned.
   * \param   envVarString Raw StringView of the environment variable.
   * \return  A Result with either no value or the error kSystemEnvVariableInvalid.
   * \spec
   *  return true
   * \endspec
   * \trace DSGN-LogAndTrace-TraceConfiguration
   */
  static amsr::core::Result<ara::core::Vector<meta::TraceContextId>> ParseEnvVariable(
      ara::core::StringView envVarString) noexcept;

 private:
  // TODO: Should this be atomic?
  /*! \brief Attach a time point to traces. */
  bool enable_time_point_acquisition{true};
  /*! \brief Vector with trace sinks. */
  TraceSinkVector trace_sinks{};
  /*! \brief Vector with enabled contexts. */
  ContextVector enabled_contexts{};
  /*! \brief State of TracingConfig's initialization. */
  bool initialized_{false};
};

}  // namespace config
}  // namespace trace
}  // namespace amsr

#endif
