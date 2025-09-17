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
 *        \brief  Read-only interface for method configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_INTERFACE_H_

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
 * \brief Read-only interface for method configuration.
 *
 * \unit ComTrace::TraceConfig
 */
class MethodConfigInterface {
 public:
  auto operator=(MethodConfigInterface const&) noexcept -> MethodConfigInterface& = delete;
  MethodConfigInterface(MethodConfigInterface&&) noexcept = delete;
  auto operator=(MethodConfigInterface&&) noexcept -> MethodConfigInterface& = delete;

  /*!
   * \brief Destroy method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~MethodConfigInterface() noexcept = default;

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
   *
   * \spec requires true; \endspec
   */
  virtual auto GetShortname() const noexcept -> Shortname = 0;

  /*!
   * \brief Determines if the tracing for sending requests is enabled for the configured method.
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
  virtual auto IsTraceRequestSendEnabled() const noexcept -> bool = 0;

  /*!
   * \brief Determines if the tracing for received requests is enabled for the configured method.
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
  virtual auto IsTraceRequestReceivedEnabled() const noexcept -> bool = 0;

  /*!
   * \brief Determines if the tracing for sending responses is enabled for the configured method.
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
  virtual auto IsTraceResponseSendEnabled() const noexcept -> bool = 0;

  /*!
   * \brief Determines if the tracing for received responses is enabled for the configured method.
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
  virtual auto IsTraceResponseReceivedEnabled() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Construct an empty method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfigInterface() noexcept = default;

  /*!
   * \brief Copy-construct a method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfigInterface(MethodConfigInterface const&) noexcept = default;
};

/*! Type name alias for reference of method config. */
using MethodConfigInterfaceRef = std::reference_wrapper<MethodConfigInterface const>;

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_INTERFACE_H_
