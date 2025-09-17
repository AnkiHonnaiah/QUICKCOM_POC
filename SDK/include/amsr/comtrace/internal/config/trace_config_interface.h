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
 *        \brief  Read-only interface for ComTrace configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/optional.h"
#include "ara/core/vector.h"

#include "amsr/comtrace/internal/config/generator_version_config_interface.h"
#include "amsr/comtrace/internal/config/service_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Read-only interface for ComTrace configuration.
 *
 * \unit ComTrace::TraceConfig
 */
class TraceConfigInterface {
 public:
  /*! Alias for a container of ServiceConfigInterfaceRef. */
  using ServiceConfigInterfaceRefContainer = ara::core::Vector<ServiceConfigInterfaceRef>;

  auto operator=(TraceConfigInterface const&) noexcept -> TraceConfigInterface& = delete;
  TraceConfigInterface(TraceConfigInterface&&) noexcept = delete;
  auto operator=(TraceConfigInterface&&) noexcept -> TraceConfigInterface& = delete;

  /*!
   * \brief Destroy trace configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~TraceConfigInterface() noexcept = default;

  /*!
   * \brief Get the configured generator version.
   *
   * \return Generator version.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetGeneratorVersion() const noexcept
      -> ::ara::core::Optional<GeneratorVersionConfigInterfaceRef const> = 0;

  /*!
   * \brief Get the configured services.
   *
   * \return Container with service references.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetServices() const noexcept -> ServiceConfigInterfaceRefContainer = 0;

 protected:
  /*!
   * \brief Construct an empty trace configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceConfigInterface() noexcept = default;

  /*!
   * \brief Copy-construct a trace configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceConfigInterface(TraceConfigInterface const&) noexcept = default;
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_INTERFACE_H_
