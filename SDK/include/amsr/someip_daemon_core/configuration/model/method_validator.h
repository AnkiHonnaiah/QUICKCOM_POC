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
/*!        \file  method_validator.h
 *        \brief  Validation module for service methods.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/intermediate_method_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for method elements.
 */
class MethodValidator final {
 public:
  /*!
   * \brief Validation results for one method object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The method is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The method object lacks a method ID.
     */
    kMissingMethodId,

    /*!
     * \brief The method object has no transport protocol defined.
     */
    kMissingTansportProtocol,

    /*!
     * \brief The method object has SOME/IP-TP configured, but the protocol
     * is not UDP as expected.
     */
    kSomeIpTpNotOverUdp
  };

  /*!
   * \brief Check if this method parsed previously is valid.
   * \details
   * - Verify that the method ID is present (mandatory).
   * - Verify that the method protocol is present (mandatory).
   * - Verify that SOME/IP-TP is only used with UDP as the protocol of this method.
   * - This will break on the first error.
   * - Additional checks can be placed within this static member function.
   * \param[in] method_object The method element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult
   * \pre -
   * \context ANY
   * \reentrant FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if MethodIdElement for MethodConfigObject is not set
   *   - Set the validation result to MethodIdElement is not configured
   * - Otherwise, check if the ProtoElement for MethodConfigObject is not set
   *   - Set the validation result to ProtoElement is not configured
   * - Otherwise, check if SomeIpTpElement is set but the ProtoElement is not UDP
   *   - Set the validation result to method object has SOME/IP-TP configured, but the protocol is not UDP as expected
   * - return the validation result
   * \endinternal
   * \steady FALSE
   */
  static ValidationResult Check(configuration::model::MethodConfigObject const& method_object) noexcept {
    ValidationResult result{};
    if (method_object.GetMethodId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMethodId;
    } else if (method_object.GetProto().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingTansportProtocol;
    } else if ((method_object.GetSomeIpTp().GetStatus() == ElementStatus::kSet) &&
               (method_object.GetProto().GetElement() != ConfigurationTypesAndDefs::Protocol::kUDP)) {
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_VALIDATOR_H_
