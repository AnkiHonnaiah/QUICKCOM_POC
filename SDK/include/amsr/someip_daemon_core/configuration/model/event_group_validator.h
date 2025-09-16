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
/*!        \file  event_group_validator.h
 *        \brief  Validates parsed event group element.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_GROUP_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_GROUP_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/intermediate_event_group_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups checks for parsed event group elements.
 */
class EventGroupValidator final {
 public:
  /*!
   * \brief Validation results for one method object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The event group is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The event group object is missing.
     */
    kMissingEventGroup,
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
        ret = "The event group is valid";
        break;

      case ValidationResult::kMissingEventGroup:
        ret = "The event group object is missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this event group parsed previously is valid.
   * \details Verify that the event group ID is present (mandatory).
   *          Additional checks can be placed within this static member function.
   * \param[in] event_group_object The event group element to validate (whether all mandatory elements are set).
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if event group ID element is not set
   *   - Update the validation to event group object is missing
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(configuration::model::EventGroupConfigObject const& event_group_object) noexcept {
    ValidationResult validation_result{ValidationResult::kOk};

    // Mandatory fields for an event group
    if (event_group_object.GetId().GetStatus() != ElementStatus::kSet) {
      validation_result = ValidationResult::kMissingEventGroup;
    }

    return validation_result;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_GROUP_VALIDATOR_H_
