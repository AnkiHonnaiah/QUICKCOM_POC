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
 *        \brief  Configuration class for events.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief   Configuration for events.
 * \details Configuration class storing the configuration parameters for an event.
 *
 * \unit ComTrace::TraceConfig
 */
class EventConfig final : public EventConfigInterface {
 public:
  /*!
   * \brief Construct an empty event configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfig() noexcept = default;

  /*!
   * \brief Copy-Construct an event configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfig(EventConfig const&) noexcept = default;

  auto operator=(EventConfig const&) noexcept -> EventConfig& = delete;
  EventConfig(EventConfig&&) noexcept = delete;
  auto operator=(EventConfig&&) noexcept -> EventConfig& = delete;

  /*!
   * \brief Destroy event configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~EventConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::EventConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetShortname() const noexcept -> Shortname final;

  /*!
   * \brief Sets the shortname.
   *
   * \param[in] shortname  The shortname.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetShortname(Shortname const& shortname) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::EventConfigInterface::IsTraceSendEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceSendEnabled() const noexcept -> bool final;

  /*!
   * \brief Set tracing of Send to enable or disable.
   *
   * \param[in] enabled  'true' for enabling the tracing of send, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceSendEnabled(bool enabled) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::EventConfigInterface::IsTraceSendAllocateEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceSendAllocateEnabled() const noexcept -> bool final;

  /*!
   * \brief Set tracing of SendAllocate to enable or disable.
   *
   * \param[in] enabled  'true' for enabling the tracing of SendAllocate requests, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceSendAllocateEnabled(bool enabled) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::EventConfigInterface::IsTraceReadSampleEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceReadSampleEnabled() const noexcept -> bool final;

  /*!
   * \brief Set tracing of ReadSample to enable or disable.
   *
   * \param[in] enabled  'true' for enabling the tracing of ReadSample requests, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceReadSample(bool enabled) noexcept -> void;

 private:
  /*! Short name for the event. */
  Shortname shortname_{};

  /*! Trace event send or field notifier update operations */
  bool trace_send_{false};

  /*! Trace event send with pre-allocated sample memory */
  bool trace_send_allocate_{false};

  /*! Trace event read sample */
  bool trace_read_sample_{false};
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_H_
