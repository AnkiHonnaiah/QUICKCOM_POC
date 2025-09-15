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
 *        \brief  Configuration Validator Interface for the IpcBinding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_INTERFACE_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Checks consistency between runtime configuration and generated code during initialization.
 *
 * \unit IpcBinding::ConfigValidator
 */
class ConfigValidatorInterface {
 public:
  /*!
   * \brief Type alias for the runtime configuration model.
   */
  using RuntimeConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig;

  /*!
   * \brief Constructor of ConfigValidator.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ConfigValidatorInterface() noexcept = default;

  /*!
   * \brief Destroy the ConfigValidatorInterface.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~ConfigValidatorInterface() noexcept = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConfigValidatorInterface(ConfigValidatorInterface const&) noexcept = delete;
  auto operator=(ConfigValidatorInterface const&) noexcept -> ConfigValidatorInterface& = delete;
  ConfigValidatorInterface(ConfigValidatorInterface&&) noexcept = delete;
  auto operator=(ConfigValidatorInterface&&) noexcept -> ConfigValidatorInterface& = delete;

  /*!
   * \brief Validates the Runtime service configs with the generated code.
   *
   * \param[in] service_configs    The list of service configurations from Runtime config.
   * \param[in] deployed_services  The list of service shortnames that was generated into C++ code.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto AssertGeneratedCodeMatchesConfig(
      RuntimeConfig::ServiceConfigRefContainer const& service_configs,
      RuntimeConfig::DeployedServicesVector const& deployed_services) const noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr
#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_INTERFACE_H_
