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
/*!        \file  provided_service_instance_subscriber_validator.h
 *        \brief  Validator for config object "ProvidedServiceInstanceSubscriberConfigObject"
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_subscriber_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'provided service instance' elements.
 */
class ProvidedServiceInstanceSubscriberValidator final {
 public:
  /*!
   * \brief Validation results.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief No address element configured.
     */
    kMissingAddress,

    /*!
     * \brief No UDP port element configured.
     */
    kMissingUdpPort,

    /*!
     * \brief No valid UDP port element configured.
     */
    kInvalidUdpPort,

    /*!
     * \brief No 'eventgroups' element configured.
     */
    kMissingEventgroups
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
  static char const* ValidationResultToString(ValidationResult validation_result) {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingAddress:
        ret = "Address is missing";
        break;

      case ValidationResult::kMissingUdpPort:
        ret = "UDP port is missing";
        break;

      case ValidationResult::kInvalidUdpPort:
        ret = "UDP port is invalid";
        break;

      case ValidationResult::kMissingEventgroups:
        ret = "Event groups are missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the address is set.
   * - Verify that the UDP port is set.
   * - Verify that 'eventgroups' element is set.
   * - Don't verify that the provided signal serialized PDU is set.
   * - Additional checks can be placed within this static member function.
   * \param[in] service_subscriber The 'instance subscribers' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if AddressElement is not set, UdpPortElement is not set or invalid, or EventgroupsElement is not set
   *   - Set the validation result to corresponding configuration element is not configured
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(ProvidedServiceInstanceSubscriberConfigObject const& service_subscriber) noexcept {
    ValidationResult result{};

    if (service_subscriber.GetAddress().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingAddress;
    } else if (service_subscriber.GetUdpPort().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingUdpPort;
    } else if (service_subscriber.GetUdpPort().GetElement() == ConfigurationTypesAndDefs::kInvalidPort) {
      result = ValidationResult::kInvalidUdpPort;
    } else if (service_subscriber.GetEventgroups().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventgroups;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_VALIDATOR_H_
