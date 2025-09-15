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
 *        \brief  Read-only interface for service configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "ara/core/vector.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config_interface.h"
#include "amsr/comtrace/internal/config/field_config_interface.h"
#include "amsr/comtrace/internal/config/method_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Read-only interface for service configuration.
 *
 * \unit ComTrace::TraceConfig
 */
class ServiceConfigInterface {
 public:
  /*! Alias for a container of MethodConfigInterfaceRef. */
  using MethodConfigInterfaceRefContainer = ara::core::Vector<MethodConfigInterfaceRef>;

  /*! Alias for a container of EventConfigInterfaceRef. */
  using EventConfigInterfaceRefContainer = ara::core::Vector<EventConfigInterfaceRef>;

  /*! Alias for a container of FieldConfigInterfaceRef. */
  using FieldConfigInterfaceRefContainer = ara::core::Vector<FieldConfigInterfaceRef>;

  auto operator=(ServiceConfigInterface const&) noexcept -> ServiceConfigInterface& = delete;

  auto operator=(ServiceConfigInterface&&) noexcept -> ServiceConfigInterface& = delete;

  /*! Default destructor. */
  virtual ~ServiceConfigInterface() noexcept = default;

  /*!
   * \brief Gets the shortname path.
   *
   * \return The shortname path.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetShortnamePath() const noexcept -> ShortnamePath const& = 0;

  /*!
   * \brief Gets the configured events.
   *
   * \return Container with event references.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetEventConfigs() const noexcept -> EventConfigInterfaceRefContainer = 0;

  /*!
   * \brief Gets the configured methods.
   *
   * \return Container with method references.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetMethodConfigs() const noexcept -> MethodConfigInterfaceRefContainer = 0;

  /*!
   * \brief Gets the configured fields.
   *
   * \return Container with field references.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetFieldConfigs() const noexcept -> FieldConfigInterfaceRefContainer = 0;

 protected:
  /*!
   * \brief Construct an empty service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfigInterface() noexcept = default;

  /*!
   * \brief Copy-construct a service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfigInterface(ServiceConfigInterface const&) noexcept = default;

  /*!
   * \brief Move-construct a service configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfigInterface(ServiceConfigInterface&&) noexcept = default;
};

/*! Type name alias for reference of service config. */
using ServiceConfigInterfaceRef = std::reference_wrapper<ServiceConfigInterface const>;

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_INTERFACE_H_
