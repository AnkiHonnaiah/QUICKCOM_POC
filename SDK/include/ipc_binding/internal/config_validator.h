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
 *        \brief  Configuration Validator for the IpcBinding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/config_validator_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Checks consistency between runtime configuration and generated code during initialization.
 *
 * \unit IpcBinding::ConfigValidator
 */
class ConfigValidator final : public ConfigValidatorInterface {
 public:
  /*!
   * \brief Constructor of ConfigValidator.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ConfigValidator() noexcept = default;

  /*!
   * \brief Destroy the ConfigValidator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~ConfigValidator() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConfigValidator(ConfigValidator const&) noexcept = delete;
  auto operator=(ConfigValidator const&) noexcept -> ConfigValidator& = delete;
  ConfigValidator(ConfigValidator&&) noexcept = delete;
  auto operator=(ConfigValidator&&) noexcept -> ConfigValidator& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding::internal::ConfigValidatorInterface::AssertGeneratedCodeMatchesConfig()
   */
  auto AssertGeneratedCodeMatchesConfig(RuntimeConfig::ServiceConfigRefContainer const& service_configs,
                                        RuntimeConfig::DeployedServicesVector const& deployed_services) const noexcept
      -> void final;
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr
#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_VALIDATOR_H_
