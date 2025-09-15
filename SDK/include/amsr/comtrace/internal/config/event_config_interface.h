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
 *        \brief  Read-only interface for event configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "amsr/comtrace/internal/config/config_types.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Read-only interface for event configuration.
 *
 * \unit ComTrace::TraceConfig
 */
class EventConfigInterface {
 public:
  auto operator=(EventConfigInterface const&) noexcept -> EventConfigInterface& = delete;
  EventConfigInterface(EventConfigInterface&&) noexcept = delete;
  auto operator=(EventConfigInterface&&) noexcept -> EventConfigInterface& = delete;

  /*!
   * \brief Destroy event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~EventConfigInterface() noexcept = default;

  /*!
   * \brief Gets the shortname.
   *
   * \return The shortname.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetShortname() const noexcept -> Shortname = 0;

  /*!
   * \brief Determines if the tracing for Send is enabled for the configured event.
   *
   * \return True if enabled.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto IsTraceSendEnabled() const noexcept -> bool = 0;

  /*!
   * \brief Determines if the tracing for SendAllocate is enabled for the configured event.
   *
   * \return True if enabled.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto IsTraceSendAllocateEnabled() const noexcept -> bool = 0;

  /*!
   * \brief Determines if the tracing for ReadSample is enabled for the configured event.
   *
   * \return True if enabled.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto IsTraceReadSampleEnabled() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Construct an empty event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfigInterface() noexcept = default;

  /*!
   * \brief Copy-Construct an event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfigInterface(EventConfigInterface const&) noexcept = default;
};

/*! Type name alias for reference of event config. */
using EventConfigInterfaceRef = std::reference_wrapper<EventConfigInterface const>;

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_EVENT_CONFIG_INTERFACE_H_
