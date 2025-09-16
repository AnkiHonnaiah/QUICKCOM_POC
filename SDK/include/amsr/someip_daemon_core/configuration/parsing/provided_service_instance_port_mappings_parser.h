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
/*!        \file  provided_service_instance_port_mappings_parser.h
 *        \brief  JSON parser for 'ProvidedServiceInstancePortMappings'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PORT_MAPPINGS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PORT_MAPPINGS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_machine_mapping_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/machine_mapping_parser.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
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
 * \brief Parses 'provided service instance port mappings' elements from JSON into a container.
 */
class ProvidedServiceInstancePortMappingsParser final
    : public amsr::json::Parser<ProvidedServiceInstancePortMappingsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ProvidedServiceInstancePortMappingsParser>;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Input for this parser is the container of 'provided service instance port mappings' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] port_mappings A reference to the container which is extended on every valid 'provided service
   * instance port mapping' element.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ProvidedServiceInstancePortMappingsParser(amsr::json::JsonData& file,
                                            ConfigurationTypesAndDefs::MachineMappingContainer& port_mappings) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstancePortMappingsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstancePortMappingsParser(ProvidedServiceInstancePortMappingsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstancePortMappingsParser(ProvidedServiceInstancePortMappingsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstancePortMappingsParser const&)
      -> ProvidedServiceInstancePortMappingsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ProvidedServiceInstancePortMappingsParser&&) -> ProvidedServiceInstancePortMappingsParser& = delete;

  /*!
   * \brief Start object means that a new port mapping object was found.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief End Array
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnEndArray(std::size_t) noexcept;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent() noexcept;

 private:
  /*!
   * \brief Type-alias for single port mapping object validation.
   */
  using PortMappingValidation = configuration::model::ProvidedServiceInstanceMachineMappingValidator;

  /*!
   *\brief Type-alias for single port mapping object parser.
   */
  using PortMappingParser = configuration::parsing::MachineMappingParser<PortMappingValidation>;

  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the 'provided service instance port mappings' container that is extended after the validation
   * step.
   */
  ConfigurationTypesAndDefs::MachineMappingContainer& port_mappings_;

  /*!
   * \brief This POD structure will be added to the container of 'provided service instance port mappings', if the
   * parsed element is valid.
   */
  ConfigurationTypesAndDefs::MachineMapping port_mapping_{
      {} /*Address*/,
      {} /* udp_port */,
      {} /* tcp_port */,
      someip_daemon_core::IpAddress::MakeAddress("0.0.0.0").Value() /* event_multicast_address */,
      0U /* event_multicast_port */,
      someip_daemon_core::IpAddress::MakeAddress("0.0.0.0").Value() /* network_mask */,
      0U /* prefix_length */,
      amsr::net::ip::Network{},
      ConfigurationTypesAndDefs::ServiceInstanceCommunicationType{
          ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kSdAndCommunication},
      bool{true} /* communication_enabled */,
      bool{true} /* service_discovery_enabled */,
      {} /* subscribers */,
      {} /* provider */,
      {} /* provided_pdus */,
      0U /*udp_collection_buffer_size_threshold_ */,
      {} /* eventgroup_udp_endpoint_address_*/,
      {} /* eventgroup_tcp_endpoint_address_ */,
      {} /* eventgroup_udp_endpoint_port_ */,
      {} /* eventgroup_tcp_endpoint_port_ */,
      {} /* secure_ids_ */};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PORT_MAPPINGS_PARSER_H_
