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
/*!        \file  someip_method_event_props_validator.h
 *        \brief Validator for "SomeipMethodProps" and "SomeipEventProps", as on the JSON both are a flattened.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_VALIDATOR_H_

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/someip_method_event_props_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups checks for parsed SOME/IP configuration element.
 */
class SomeipMethodEventPropsValidator final {
 public:
  /*!
   * \brief Validation results for one SOME/IP configuration object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The SOME/IP configuration object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief Missing Id
     */
    kId,

    /*!
     * \brief Missing Timeout
     */
    kTimeout,

    /*!
     * \brief Missing isTrigger
     */
    kIsTrigger,

    /*!
     * \brief Redundant timeout
     */
    kRedundantTimeout,
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

      case ValidationResult::kId:
        ret = "The id is missing";
        break;

      case ValidationResult::kTimeout:
        ret = "The timeout value is missing";
        break;

      case ValidationResult::kIsTrigger:
        ret = "The trigger value is missing";
        break;

      case ValidationResult::kRedundantTimeout:
        ret = "A timeout value greater than zero is set on a trigger message";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check validity
   *
   * \param object objetct to check
   * \return ValidationResult The result
   * \steady FALSE
   */
  static ValidationResult Check(configuration::model::SomeipMethodEventPropsObject const& object) noexcept {
    ValidationResult ret{ValidationResult::kOk};

    constexpr ElementStatus set{ElementStatus::kSet};

    if (object.GetId().GetStatus() != set) {
      ret = ValidationResult::kId;
    } else if (object.GetIsTrigger().GetStatus() != set) {
      ret = ValidationResult::kIsTrigger;
    } else if ((object.GetTimeout().GetStatus() != set) && (!object.GetIsTrigger().GetElement())) {
      ret = ValidationResult::kTimeout;
    } else if ((object.GetIsTrigger().GetElement()) && (object.GetTimeout().GetStatus() == set) &&
               (object.GetTimeout().GetElement() > std::chrono::nanoseconds::zero())) {
      ret = ValidationResult::kRedundantTimeout;
    } else {
      ret = ValidationResult::kOk;
    }
    return ret;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_VALIDATOR_H_
