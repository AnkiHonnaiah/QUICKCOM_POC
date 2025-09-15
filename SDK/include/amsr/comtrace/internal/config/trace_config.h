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
 *        \brief ComTrace trace configuration class hierarchy.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unordered_map>

#include "ara/core/optional.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/generator_version_config.h"
#include "amsr/comtrace/internal/config/generator_version_config_interface.h"
#include "amsr/comtrace/internal/config/service_config.h"
#include "amsr/comtrace/internal/config/trace_config_interface.h"
#include "amsr/comtrace/internal/logger.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief ComTrace trace configuration class hierarchy.
 *
 * \unit ComTrace::TraceConfig
 */
class TraceConfig final : public TraceConfigInterface {
 public:
  /*!
   * \brief Construct an empty trace configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceConfig() noexcept = default;

  TraceConfig(TraceConfig const&) noexcept = delete;
  auto operator=(TraceConfig const&) noexcept -> TraceConfig& = delete;
  TraceConfig(TraceConfig&&) noexcept = delete;
  auto operator=(TraceConfig&&) noexcept -> TraceConfig& = delete;

  /*!
   * \brief Destroy trace configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~TraceConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::TraceConfigInterface::GetGeneratorVersion()
   *
   * \spec requires true; \endspec
   */
  auto GetGeneratorVersion() const noexcept -> ::ara::core::Optional<GeneratorVersionConfigInterfaceRef const> final;

  /*!
   * \brief Set the generator version.
   *
   * \param[in] version  Generator version.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetGeneratorVersion(GeneratorVersionConfig const& version) noexcept -> void;

  /*!
   * \brief   Adds a new mapping between a service shortname path and a service config.
   * \details Abort if service shortname path already exists in services map.
   *
   * \param[in]     shortname_path  The shortname path of the service, to which the service configuration belongs.
   * \param[in,out] service         The configuration of the service.
   *
   * \pre         Passed shortname is not allowed to already exist in services map.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto AddService(ShortnamePath const& shortname_path, ServiceConfig&& service) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::TraceConfigInterface::GetServices()
   *
   * \spec requires true; \endspec
   */
  auto GetServices() const noexcept -> ServiceConfigInterfaceRefContainer final;

 private:
  /*!
   * \brief Type-alias for the logger.
   */
  using Logger = ::amsr::comtrace::internal::Logger;

  /*!
   * \brief Type name alias for the map of service shortname paths and the associated services.
   */
  using ServicesMap = std::unordered_map<ShortnamePath, ServiceConfig, ShortnamePathHash>;

  /*!
   * \brief The Logger.
   */
  Logger logger_{"TraceConfig"};

  /*!
   * \brief The map of service shortname paths and the associated services.
   */
  ServicesMap services_map_{};

  /*!
   * Generator version configuration.
   */
  ::ara::core::Optional<GeneratorVersionConfig> generator_version_config_{};
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_TRACE_CONFIG_H_
