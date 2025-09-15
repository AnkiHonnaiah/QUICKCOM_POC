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
/*!        \file  required_service_instance_signal_serialized_pdu_validator.h
 *        \brief  Validates Configuration object 'RequiredServiceInstanceSignalSerializedPdu'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_signal_serialized_pdu_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'service discovery' elements of 'required service instances'.
 */
class RequiredServiceInstanceSignalSerializedPduValidator final {
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
     * \brief The PDU ID is missing.
     */
    kMissingPduId,

    /*!
     * \brief The Event ID is missing.
     */
    kMissingEventId
  };

  /*!
   * \brief Translate the validation result to string for logging.
   * \param[in] validation_result Validation result to translate.
   * \return String representation of the passed-in validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   *
   * \internal
   * - Translate the validation result to string and return it.
   * \endinternal
   * \steady FALSE
   */
  static char const* ValidationResultToString(ValidationResult validation_result) {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingPduId:
        ret = "PDU ID is missing";
        break;

      case ValidationResult::kMissingEventId:
        ret = "Event ID is missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the PDU ID is set.
   * - Verify that the Event ID is set.
   * \param[in] pdu The 'required service instance signal serialized pdu' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Check if PduId is not set
   *   - Set the validation result to missing PduId
   * - Otherwise, check if EventId is not set
   *   - Set the validation result to missing EventId
   * - Otherwise, Set the validation result to Ok
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(RequiredServiceInstanceSignalSerializedPduObject const& pdu) noexcept {
    ValidationResult result{};

    if ((pdu.GetPduId().GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingPduId;
    } else if (pdu.GetEventId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventId;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_VALIDATOR_H_
