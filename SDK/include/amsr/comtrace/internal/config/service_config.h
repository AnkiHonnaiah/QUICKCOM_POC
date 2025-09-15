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
 *        \brief ComTrace trace configuration service data.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unordered_map>

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config.h"
#include "amsr/comtrace/internal/config/field_config.h"
#include "amsr/comtrace/internal/config/method_config.h"
#include "amsr/comtrace/internal/config/service_config_interface.h"
#include "amsr/comtrace/internal/logger.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Class to hold the data for the service.
 *
 * \unit ComTrace::TraceConfig
 */
class ServiceConfig final : public ServiceConfigInterface {
 public:
  /*!
   * \brief Construct an empty service configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceConfig() noexcept = default;

  ServiceConfig(ServiceConfig const&) noexcept = delete;
  auto operator=(ServiceConfig const&) noexcept -> ServiceConfig& = delete;

  /*!
   * \brief Move-construct a service configuration.
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
  ServiceConfig(ServiceConfig&&) noexcept = default;  // VCA_COMTRACE_STD_UNORDERED_MAP

  auto operator=(ServiceConfig&&) noexcept -> ServiceConfig& = delete;

  /*!
   * \brief Destroy the ServiceConfig.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServiceConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::ServiceConfigInterface::GetShortnamePath()
   *
   * \spec requires true; \endspec
   */
  auto GetShortnamePath() const noexcept -> ShortnamePath const& final;

  /*!
   * \brief Sets the shortname path.
   *
   * \param[in] shortname_path  The shortname path.
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
  auto SetShortnamePath(ShortnamePath const& shortname_path) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::ServiceConfigInterface::GetEventConfigs()
   *
   * \spec requires true; \endspec
   */
  auto GetEventConfigs() const noexcept -> EventConfigInterfaceRefContainer final;

  /*!
   * \brief Adds a new event config.
   *
   * \param[in] shortname  Shortname of event, to which the event config belongs.
   * \param[in] event      The event.
   *
   * \pre         Passed shortname is not allowed to already exist in events map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto AddEvent(Shortname const& shortname, EventConfig&& event) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::ServiceConfigInterface::GetMethodConfigs()
   *
   * \spec requires true; \endspec
   */
  auto GetMethodConfigs() const noexcept -> MethodConfigInterfaceRefContainer final;

  /*!
   * \brief Adds a new method config.
   *
   * \param[in] shortname  Shortname of method, to which the method config belongs.
   * \param[in] method     The method.
   *
   * \pre         Passed shortname is not allowed to already exist in methods map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto AddMethod(Shortname const& shortname, MethodConfig&& method) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::ServiceConfigInterface::GetFieldConfigs()
   *
   * \spec requires true; \endspec
   */
  auto GetFieldConfigs() const noexcept -> FieldConfigInterfaceRefContainer final;

  /*!
   * \brief Adds a new field config.
   *
   * \param[in] shortname  Shortname of field, to which the field config belongs.
   * \param[in] field      The field.
   *
   * \pre         Passed shortname is not allowed to already exist in fields map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto AddField(Shortname const& shortname, FieldConfig&& field) noexcept -> void;

 private:
  /*!
   * \brief Type-alias for the logger.
   */
  using Logger = ::amsr::comtrace::internal::Logger;

  /*! Type name alias for the map of shortnames and the associated event. */
  using EventsMap = std::unordered_map<Shortname, EventConfig, ShortnameHash>;

  /*! Type name alias for the map of shortnames and the associated method. */
  using MethodsMap = std::unordered_map<Shortname, MethodConfig, ShortnameHash>;

  /*! Type name alias for the map of shortnames and the associated field. */
  using FieldsMap = std::unordered_map<Shortname, FieldConfig, ShortnameHash>;

  /*! Short name path for the service interface. */
  ShortnamePath service_shortname_path_{};

  /*! Events container. */
  EventsMap events_map_{};

  /*! Methods container. */
  MethodsMap methods_map_{};

  /*! Fields container. */
  FieldsMap fields_map_{};

  /*! Logger. */
  Logger logger_{"ServiceConfig"};
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_SERVICE_CONFIG_H_
