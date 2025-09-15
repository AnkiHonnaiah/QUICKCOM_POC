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
/*!        \file  ipc_channel_validator.h
 *        \brief  Validates Configuration object 'IpcChannel'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_IPC_CHANNEL_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_IPC_CHANNEL_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/intermediate_ipc_channel_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for ipc channel elements.
 */
class IpcChannelValidator final {
 public:
  /*!
   * \brief Validation results for one IPC channel element.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The IPC channel object is valid.
     */
    kOk = 0x00U,
    /*!
     * \brief The IPC channel object lacks domain.
     */
    kMissingDomain,
    /*!
     * \brief The IPC channel object lacks port.
     */
    kMissingPort,

    /*!
     * \brief The IPC channel object lacks both required and provided service instances.
     */
    kMissingInstances,
  };

  /*!
   * \brief Translate the validation result to string for logging.
   * \param[in] validation_result Validation result to translate.
   * \return String representation of the passed-in validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Translate the validation result to string and return it.
   * \endinternal
   */
  static auto ValidationResultToString(ValidationResult const validation_result) noexcept -> char const* {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;
      case ValidationResult::kMissingDomain:
        ret = "Domain is missing";
        break;
      case ValidationResult::kMissingPort:
        ret = "Port is missing";
        break;
      case ValidationResult::kMissingInstances:
        ret = "Both required service instances and provided service instances are missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this IPC channel parsed is valid.
   * \details
   * - Check that the domain is present (mandatory).
   * - Check that the port is present (mandatory).
   * - Check that at least one required or provided service instance is set (mandatory).
   * \param[in] ipc_channel_element The ipc channel element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok.
   * - Check if any of configuration elements such as Domain and Port for IpcChannelConfigObject is not set
   *   - Set the validation result to the corresponding element is not configured.
   * - Check if either of provided or required instances element is not set or the element is empty
   *   - Set the validation result to both the provided and required instances elements are not configured.
   * - return the validation result.
   * \endinternal
   */
  static auto Check(configuration::model::IpcChannelConfigObject const& ipc_channel_element) noexcept
      -> ValidationResult {
    ValidationResult result{};

    bool const required_instances_set{ipc_channel_element.GetRequiredServiceInstances().GetStatus() ==
                                      ElementStatus::kSet};
    bool const required_instances_empty{required_instances_set &&
                                        ipc_channel_element.GetRequiredServiceInstances().GetElement().empty()};
    bool const provided_instances_set{ipc_channel_element.GetProvidedServiceInstances().GetStatus() ==
                                      ElementStatus::kSet};
    bool const provided_instances_empty{provided_instances_set &&
                                        ipc_channel_element.GetProvidedServiceInstances().GetElement().empty()};

    if (ipc_channel_element.GetDomain().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingDomain;
    } else if (ipc_channel_element.GetPort().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPort;
    } else if (((!required_instances_set) || required_instances_empty) &&
               ((!provided_instances_set) || provided_instances_empty)) {
      result = ValidationResult::kMissingInstances;
    } else {
      result = ValidationResult::kOk;
    }

    return result;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_IPC_CHANNEL_VALIDATOR_H_
