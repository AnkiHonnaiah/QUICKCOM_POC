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
/*!        \file  event_validator.h
 *        \brief  Groups validation rules for service events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/intermediate_event_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for event objects.
 */
class EventValidator final {
 public:
  /*!
   * \brief Validation results for one event objects.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The event object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The event object lacks an ID.
     */
    kMissingId,

    /*!
     * \brief The event object lacks a field flag.
     */
    kMissingField,

    /*!
     * \brief The event object has no transport protocol defined.
     */
    kMissingTansportProtocol,

    /*!
     * \brief The event object has SOME/IP-TP configured, but the protocol
     * is not UDP as expected.
     */
    kSomeIpTpNotOverUdp
  };

  /*!
   * \brief Check if this event parsed previously is valid.
   * \details
   * - Check that the event ID is present (mandatory).
   * - Check that the event protocol is present (mandatory).
   * - Check that the field flag is present (mandatory).
   * - Check that SOME/IP-TP may only be used together with UDP as the protocol.
   * - Additional checks can be placed within this static member function.
   * \param[in] event_object The event element to verify.
   * \return kOk if the event element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if any of configuration elements such as ID, ProtoElement, IsFieldElement for EventConfigObject is not set
   *   - Set the validation result to the corresponding element is not configured
   * - Otherwise, check if SomeIpTpElement is set but the ProtoElement is not UDP
   *   - Set the validation result to method object has SOME/IP-TP configured, but the protocol is not UDP as expected
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(configuration::model::EventConfigObject const& event_object) noexcept {
    ValidationResult result{};
    if (event_object.GetId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingId;
    } else if (event_object.GetProto().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingTansportProtocol;
    } else if (event_object.GetIsField().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingField;
    } else if ((event_object.GetSomeIpTp().GetStatus() == ElementStatus::kSet) &&
               (event_object.GetProto().GetElement() != ConfigurationTypesAndDefs::Protocol::kUDP)) {
      result = ValidationResult::kSomeIpTpNotOverUdp;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_VALIDATOR_H_
