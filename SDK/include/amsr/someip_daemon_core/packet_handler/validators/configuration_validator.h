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
/**        \file  configuration_validator.h
 *        \brief  Configuration Validator.
 *
 *      \details  This class validates the incoming packets against the configuration.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_CONFIGURATION_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_CONFIGURATION_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief This class validates the incoming packets against the configuration.
 */
class ConfigurationValidator final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param config The configuration.
   * \steady FALSE
   */
  explicit ConfigurationValidator(configuration::Configuration const& config) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ConfigurationValidator() noexcept = default;

  ConfigurationValidator(ConfigurationValidator const&) = delete;
  ConfigurationValidator(ConfigurationValidator&&) = delete;
  ConfigurationValidator& operator=(ConfigurationValidator const&) = delete;
  ConfigurationValidator& operator=(ConfigurationValidator&&) = delete;

  /*!
   * \brief Check if the given SOME/IP packet is compatible with the configuration
   *        based on the following checks:
   *        - Service ID exists.
   *        - Method/event ID exists.
   *
   * \param[in] header SOME/IP packet header to be validated.
   *
   * \context Network
   * \steady  TRUE
   * \return "PacketValidatorReturnCode" enumeration value.
   *         "kOk": If no error occurs.
   *         "kConfigurationError_UnknownMethod": If configuration check fails due to method not configured.
   *         "kConfigurationError_UnknownEvent": If configuration check fails due to event not configured.
   *         "kConfigurationError_WrongInterfaceVersion": If configuration check fails due to wrong interface version.
   *         "kConfigurationError_UnknownService": If configuration check fails due to unknown service ID.
   *
   * \trace SPEC-10144321, SPEC-10144322, SPEC-10144323, SPEC-10144324, SPEC-10144325
   */
  PacketValidatorReturnCode CheckAgainstConfiguration(
      someip_protocol::internal::SomeIpMessageHeader const& header) const noexcept;

  /*!
   * \brief Check if the given PDU packet is compatible with the configuration
   *        based on the following checks:
   *        - Service ID exists.
   *        - Method/event ID exists.
   *
   * \param[in] header PDU packet header to be validated.
   *
   * \context Network
   * \steady  TRUE
   * \return "PacketValidatorReturnCode" enumeration value.
   *         "kOk": If no error occurs.
   *         "kConfigurationError_UnknownEvent": If configuration check fails due to event not configured.
   *         "kConfigurationError_UnknownService": If configuration check fails due to unknown service ID.
   */
  PacketValidatorReturnCode CheckAgainstConfiguration(
      someip_protocol::internal::PduMessageHeader const& header) const noexcept;

 private:
  /*!
   * \brief Check if the given SOME/IP method/response is compatible with the configuration
   *        based on the following checks:
   *        - Service ID exists.
   *        - Method ID exists.
   *
   * \param[in] header SOME/IP packet header to be validated.
   *
   * \context Network
   * \steady  TRUE
   * \return "PacketValidatorReturnCode" enumeration value.
   *         "kOk": If no error occurs.
   *         "kConfigurationError_UnknownMethod": If configuration check fails due to method not configured.
   *         "kConfigurationError_WrongInterfaceVersion": If configuration check fails due to wrong interface version.
   *         "kConfigurationError_UnknownService": If configuration check fails due to unknown service ID.
   */
  PacketValidatorReturnCode HandleRequestResponse(
      someip_protocol::internal::SomeIpMessageHeader const& header) const noexcept;

  /*!
   * \brief Check if the given SOME/IP event is compatible with the configuration
   *        based on the following checks:
   *        - Service ID exists.
   *        - Eevent ID exists.
   *
   * \param[in] header SOME/IP packet header to be validated.
   *
   * \context Network
   * \steady  TRUE
   * \return "PacketValidatorReturnCode" enumeration value.
   *         "kOk": If no error occurs.
   *         "kConfigurationError_UnknownEvent": If configuration check fails due to event not configured.
   *         "kConfigurationError_WrongInterfaceVersion": If configuration check fails due to wrong interface version.
   *         "kConfigurationError_UnknownService": If configuration check fails due to unknown service ID.
   */
  PacketValidatorReturnCode HandleNotification(
      someip_protocol::internal::SomeIpMessageHeader const& header) const noexcept;

  /*!
   * \brief Reference to the configuration.
   */
  configuration::Configuration const& config_;
};

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_CONFIGURATION_VALIDATOR_H_
