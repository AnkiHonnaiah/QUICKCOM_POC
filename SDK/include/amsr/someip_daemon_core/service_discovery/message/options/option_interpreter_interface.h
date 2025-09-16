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
/**        \file  option_interpreter_interface.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTION_INTERPRETER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTION_INTERPRETER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace options {

/*!
 * \brief The interface any option interpreter shall implement
 */
class OptionInterpreterInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \context Network
   * \steady FALSE
   */
  OptionInterpreterInterface() = default;
  /*!
   * \brief Define default destructor.
   * \context Network
   * \steady FALSE
   */
  virtual ~OptionInterpreterInterface() = default;

  OptionInterpreterInterface(OptionInterpreterInterface const&) = delete;
  OptionInterpreterInterface(OptionInterpreterInterface&&) = delete;
  OptionInterpreterInterface& operator=(OptionInterpreterInterface const&) & = delete;
  OptionInterpreterInterface& operator=(OptionInterpreterInterface&&) & = delete;

  /*!
   * \brief Interprets an options::IpEndpointOption containing a TCP endpoint
   * \param[in] endpoint  The endpoint
   *
   * \return An empty result if the interpretation succeeded or containing an error.
   *
   * \retval SdOptionInterpretationError::kContradictingOptions This API has already been called with an option that
   *                                                            contradicts the current one
   * \retval SdOptionInterpretationError::kNotAllowedOption     The entry endpoint is not
   *                                                            allowed by the SOME/IP SD protocol
   *
   * \pre         -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  virtual ara::core::Result<void> OnTcpEndpointOption(options::IpEndpointOption const& endpoint) = 0;
  /*!
   * \brief Interprets an options::IpEndpointOption containing a UDP endpoint
   * \param[in] endpoint  The endpoint
   *
   * \return An empty result if the interpretation succeeded or containing an error.
   *
   * \retval SdOptionInterpretationError::kContradictingOptions This API has already been called with an option that
   *                                                            contradicts the current one
   * \retval SdOptionInterpretationError::kNotAllowedOption     The entry endpoint is not
   *                                                            allowed by the SOME/IP SD protocol
   *
   * \pre         -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  virtual ara::core::Result<void> OnUdpEndpointOption(options::IpEndpointOption const& endpoint) = 0;
};

}  // namespace options
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTION_INTERPRETER_INTERFACE_H_
