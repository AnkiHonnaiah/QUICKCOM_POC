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
 *        \brief  Run-time configuration of IpcBinding.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_RUNTIME_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_RUNTIME_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"

#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/generator_version_config.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief   IpcBinding run-time configuration.
 * \details Configuration class storing the run-time configuration parameters for IpcBinding.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class RuntimeConfig final {
 public:
  /*! Type name alias for reference of service config. */
  using ServiceConfigRef = std::reference_wrapper<ServiceConfig const>;

  /*! Type name alias for mutable reference of service config. */
  using MutableServiceConfigRef = std::reference_wrapper<ServiceConfig>;

  /*! Type name alias for a vector of ServiceConfigRef. */
  using ServiceConfigRefContainer = ara::core::Vector<ServiceConfigRef>;

  /*! Type name alias for a vector of GeneratorVersionConfig. */
  using GeneratorVersionConfig = amsr::ipc_binding_core::internal::runtime_configuration::GeneratorVersionConfig;

  /*! Default value for runtime processing mode. */
  static constexpr RuntimeProcessingMode kRuntimeProcessingModeDefaultValue{RuntimeProcessingMode::thread_driven};

  /*!
   * \brief Shortname path type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ShortnamePath = ::amsr::ipc_binding_core::internal::runtime_configuration::ShortnamePath;

  /*!
   * \brief Container to hold shortname paths.
   */
  using DeployedServicesVector = ara::core::Vector<ShortnamePath>;

  /*!
   * \brief Construct an empty runtime configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RuntimeConfig() noexcept;

  /*!
   * \brief Destroy runtime configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RuntimeConfig() noexcept = default;  // VCA_IPCB_STD_UNORDERED_MAP_DESTRUCTOR

  /*!
   * \brief Copy-Construct a runtime configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RuntimeConfig(RuntimeConfig const&) noexcept = default;

  auto operator=(RuntimeConfig const&) noexcept -> RuntimeConfig& = delete;
  RuntimeConfig(RuntimeConfig&&) noexcept = delete;
  auto operator=(RuntimeConfig&&) noexcept -> RuntimeConfig& = delete;

  /*!
   * \brief Get the configured runtime processing mode.
   *
   * \return Runtime processing mode.
   *
   * \pre       Config object was set by runtime configuration loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetRuntimeProcessingMode() const -> RuntimeProcessingMode;

  /*!
   * \brief Set the runtime processing mode.
   *
   * \param[in] mode  Runtime processing mode.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  void SetRuntimeProcessingMode(RuntimeProcessingMode const& mode);

  /*!
   * \brief Get the configured generator version.
   *
   * \return Generator version.
   *
   * \pre       Config object was set by runtime configuration loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetGeneratorVersion() const -> GeneratorVersionConfig const&;

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
  void SetGeneratorVersion(GeneratorVersionConfig const& version);

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
   */
  void AddServiceMapping(ShortnamePath::type const& shortname_path, ServiceConfig&& service) noexcept;

  /*!
   * \brief Getter for the service configuration.
   *
   * \param[in] service_shortname_path  The service shortname path related to the service.
   *
   * \return The service associated with the provided service identifier.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetServiceConfigChecked(ShortnamePath::type const& service_shortname_path) const noexcept
      -> ServiceConfig const&;

  /*!
   * \brief Get a reference to the service configuration for a specific service.
   *
   * \param[in] service_shortname_path  The service shortname path.
   *
   * \return Optional containing the field configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetServiceConfig(ShortnamePath::type const& service_shortname_path) noexcept
      -> ara::core::Optional<MutableServiceConfigRef>;

  /*!
   * \brief Getter for the service instances.
   *
   * \return All configured service instances data.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetServices() const noexcept -> ServiceConfigRefContainer;

 private:
  /*!
   * \brief Type name alias for the map of service shortname paths and the associated services.
   */
  using ServicesMap = std::unordered_map<ShortnamePath::type, ServiceConfig, ara::core::hash<ShortnamePath::type>>;

  /*!
   * \brief The map of service shortname paths and the associated services.
   */
  ServicesMap services_map_{};

  /*!
   * \brief Runtime processing mode.
   */
  RuntimeProcessingMode runtime_processing_mode_;

  /*!
   * Generator version configuration.
   *
   * \steady FALSE
   */
  GeneratorVersionConfig generator_version_config_{};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_RUNTIME_CONFIG_H_
