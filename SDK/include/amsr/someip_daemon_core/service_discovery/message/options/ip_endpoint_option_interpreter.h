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
/**        \file  ip_endpoint_option_interpreter.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_INTERPRETER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace options {

/*!
 * \brief Interpretation of SOME/IP service discovery options
 */
class IpEndpointOptionInterpreter final {
 public:
  /*!
   * \brief Interprets a options::ServiceDiscoveryOption into an options::IpEndpointOption
   * \param[in] option  The option to interpret
   *
   * \return A result with the interpreted option or containing an error.
   *
   * \retval SdOptionInterpretationError::kMalformedAddress  The address can't be interpreted
   * \retval SdOptionInterpretationError::kUnknownOptionType  The option type is unknown
   * \retval SdOptionInterpretationError::kInconsistentOption The content of the option is not consistent
   * \retval SdOptionInterpretationError::kUnknownProtocol    The transport protocol of the option is neither TCP nor
   * UDP
   *
   * \pre         -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   * \trace SPEC-10144609
   */
  static ara::core::Result<message::options::IpEndpointOption> InterpretOption(
      message::options::ServiceDiscoveryOption const& option);
};

}  // namespace options
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_INTERPRETER_H_
