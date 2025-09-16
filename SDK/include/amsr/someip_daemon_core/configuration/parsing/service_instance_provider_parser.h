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
/*!        \file  service_instance_provider_parser.h
 *        \brief  vajson parser for service instance provider.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_PROVIDER_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_PROVIDER_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <type_traits>
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_provider_validator.h"
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/number.h"
#include "amsr/log/logstream.h"
#include "amsr/net/ip/network.h"
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_provider_object.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/optional.h"
#include "ara/log/logstream.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief Parse Service Instance Service Provider from the JSON into a container of Service Provider.
 */
class ServiceInstanceProviderParser final : public amsr::json::Parser<ServiceInstanceProviderParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ServiceInstanceProviderParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::RequiredServiceInstanceProviderValidator;

  /*!
   * \brief Type-alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type-alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type-alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief IP address JSON key.
   */
  static constexpr CStringView kAddressKey{"ip_address"_sv};
  /*!
   * \brief UDP port JSON key.
   */
  static constexpr CStringView kUdpPortKey{"udp_port"_sv};
  /*!
   * \brief TCP port JSON key.
   */
  static constexpr CStringView kTcpPortKey{"tcp_port"_sv};

  /*!
   * \brief Event multicast address JSON key.
   */
  static constexpr CStringView kEventMulticastAddressKey{"event_multicast_address"_sv};
  /*!
   * \brief Event multicast port JSON key.
   */
  static constexpr CStringView kEventMulticastPortKey{"event_multicast_port"_sv};

  /*!
   * \brief Input for this parser is the container of 'machine mapping' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] serviceProvider A reference to the element which is written by the parser.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceInstanceProviderParser(amsr::json::JsonData& file,
                                ConfigurationTypesAndDefs::ServiceProviderConfiguration& serviceProvider) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ServiceInstanceProviderParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ServiceInstanceProviderParser(ServiceInstanceProviderParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ServiceInstanceProviderParser(ServiceInstanceProviderParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ServiceInstanceProviderParser const&) -> ServiceInstanceProviderParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ServiceInstanceProviderParser&&) -> ServiceInstanceProviderParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to detect unknown JSON keys.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or error code if any unknown JSON key was detected.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

  /*!
   * \brief Callback on number found in JSON
   * \param[in] num value returned by the parser.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on string found in JSON
   * \param[in] val Parsed string value
   * \return ParserResult Continue parsing.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent();

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Intermediate 'machine mapping' object to fill with parsed information.
   */
  model::RequiredServiceInstanceProviderConfigObject service_provider_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_PROVIDER_PARSER_H_
