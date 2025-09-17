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
/**        \file  options_interpreter.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTIONS_INTERPRETER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTIONS_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/options/option_interpreter_interface.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace options {

/*!
 * \brief Interpretation of SOME/IP service discovery options
 */
class OptionsInterpreter final {
 public:
  /*!
   * \brief Interpret the referenced unicast options by a start index and an amount of consecutive options
   * \param[in] idx_1st_opts    Offset of the first run within the given container
   * \param[in] count_1st_opts  Amount of options in the first run
   * \param[in] idx_2nd_opts    Offset of the second run within the given container
   * \param[in] count_2nd_opts  Amount of options in the second run
   * \param[in] options         Options container
   * \param[in] interpreter     The option interpreter
   *
   * \return A result containing an error or void if interpretation succeeded.
   *
   * \retval SdOptionInterpretationError::kBadIndexing        Not possible to access an option from the given index
   * \retval SdOptionInterpretationError::kMalformedAddress   The address of an option can't be interpreted
   * \retval SdOptionInterpretationError::kUnknownOptionType  A referenced option type is unknown
   * \retval SdOptionInterpretationError::kNotAllowedOption   A referenced option type is not allowed and not
   *                                                          discardable
   *
   * \pre          -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   * \trace SPEC-10144601
   * \trace SPEC-4981593
   */
  static ara::core::Result<void> InterpretOptions(
      amsr::someip_protocol::internal::SdEntryOptionIndex const idx_1st_opts,
      amsr::someip_protocol::internal::SdEntryOptionIndex const count_1st_opts,
      amsr::someip_protocol::internal::SdEntryOptionIndex const idx_2nd_opts,
      amsr::someip_protocol::internal::SdEntryOptionIndex const count_2nd_opts,
      message::options::ServiceDiscoveryOptionContainer const& options, OptionInterpreterInterface& interpreter);

 private:
  /*!
   * \brief Interpret a single referenced option
   * \param[in] option      The option to interpret
   * \param[in] interpreter An interface to handle entry-specific interpretation
   *
   * \return A result containing an error or void if interpretation succeeded.
   *
   * \pre          -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   * \trace SPEC-10144604
   */
  static ara::core::Result<void> InterpretReferencedOption(options::ServiceDiscoveryOption const& option,
                                                           OptionInterpreterInterface& interpreter);

  /*!
   * \brief Interpret a single TCP endpoint option
   * \param[in] interpreter     An interface to handle entry-specific interpretation
   * \param[in] endpoint        The referenced endpoint
   * \param[in] is_discardable  Whether the option is discardable or not
   *
   * \return A result containing an error or void if interpretation succeeded.
   *
   * \pre          -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  static ara::core::Result<void> InterpretTcpEndpointOption(OptionInterpreterInterface& interpreter,
                                                            options::IpEndpointOption const& endpoint,
                                                            bool const is_discardable);

  /*!
   * \brief Interpret a single UDP endpoint option
   * \param[in] interpreter     An interface to handle entry-specific interpretation
   * \param[in] endpoint        The referenced endpoint
   * \param[in] is_discardable  Whether the option is discardable or not
   *
   * \return A result containing an error or void if interpretation succeeded.
   *
   * \pre          -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  static ara::core::Result<void> InterpretUdpEndpointOption(OptionInterpreterInterface& interpreter,
                                                            options::IpEndpointOption const& endpoint,
                                                            bool const is_discardable);

  /*!
   * \brief Checks if the given start index and option count is consistent with the size of the options container
   * \param[in] idx_1st_opts    Offset of the first run within the given container
   * \param[in] count_1st_opts  Amount of options in the first run
   * \param[in] idx_2nd_opts    Offset of the second run within the given container
   * \param[in] count_2nd_opts  Amount of options in the second run
   * \param[in] options_size    Size of the options container
   *
   * \return true if the indexing is consistent
   *
   * \pre          -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  static bool CheckIndexing(amsr::someip_protocol::internal::SdEntryOptionIndex const idx_1st_opts,
                            amsr::someip_protocol::internal::SdEntryOptionIndex const count_1st_opts,
                            amsr::someip_protocol::internal::SdEntryOptionIndex const idx_2nd_opts,
                            amsr::someip_protocol::internal::SdEntryOptionIndex const count_2nd_opts,
                            std::size_t const options_size);
};

}  // namespace options
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_OPTIONS_INTERPRETER_H_
