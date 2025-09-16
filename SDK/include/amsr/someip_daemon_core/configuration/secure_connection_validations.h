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
/*!        \file  secure_connection_validations.h
 *        \brief  Validation functions for SecureConnections validated in the global configuration module for one
 *SOME/IP daemon.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_SECURE_CONNECTION_VALIDATIONS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_SECURE_CONNECTION_VALIDATIONS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
/*!
 * \brief Validate the configuration of SecureConnections used in RequiredServiceInstances.
 * \param[in] global_secure_connections List of all global SecureConnections
 * \param[in] global_required_service_instances List of all global RequiredServiceInstances
 * \param[in] logger Logger object
 * \return true if the SecureConnection configurations are consistent in all RequiredServiceInstances, false
 * otherwise.
 * \pre -
 * \context Init
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionForRequiredServiceInstance(
    ConfigurationTypesAndDefs::SecureConnectionContainer const& global_secure_connections,
    ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer const& global_required_service_instances,
    someip_daemon_core::logging::AraComLogger const& logger) noexcept;

/*!
 * \brief Validate SecureConnection configuration consistency for SecureConnections.
 * \param[in] global_secure_connections List of all global SecureConnections
 * \param[in] logger Logger object
 * \param[in] linked_secure_ids SecureId Container referencing SecureConnections which are validated
 * \return true, if all validations succeeded, false otherwise
 * \context     ANY
 * \pre         -
 * \reentrant   FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionConsistencyForOneRsiOrNep(
    ConfigurationTypesAndDefs::SecureConnectionContainer const& global_secure_connections,
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureIdContainer const& linked_secure_ids) noexcept;

/*!
 * \brief Validates if all SecureConnections linked to a RequiredServiceInstance port mapping are client
 * configurations and if all SecureConnections linked to NetworkEndpointPort are server configurations.
 * \param[in] global_secure_connections List of all global SecureConnections
 * \param[in] logger Logger object
 * \param[in] secure_ids SecureId under investigation
 * \param[in] checkForServerSide Flag if the check should be done for client or server side
 * \return true if all links are correct, false otherwise
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateCorrectSecureIdLinking(
    ConfigurationTypesAndDefs::SecureConnectionContainer const& global_secure_connections,
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureIdContainer const& secure_ids, bool const checkForServerSide) noexcept;

/*!
 * \brief Validates that two SecureConnections have consistent priority and cipher suite ID configuration.
 * \details Checking two implications: 1. If using the same cipher suite ID, the same priority must be configured, 2.
 * If using different cipher suite IDs, different priorities must be configured.
 * \param[in] logger Logger object
 * \param[in] secure_connection SecureConnection
 * \param[in] other_secure_connection SecureConnection
 * \return true if both implications are correct, false otherwise.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionPriorityAndCipherSuiteIdsConsistency(
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureConnection const& secure_connection,
    ConfigurationTypesAndDefs::SecureConnection const& other_secure_connection) noexcept;

/*!
 * \brief Validates that two SecureConnections are using the same TLS version.
 * \param[in] logger Logger object
 * \param[in] secure_connection SecureConnection
 * \param[in] other_secure_connection SecureConnection
 * \return true both SecureConnections are using the same TLS version, false otherwise
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionsUsingSameTlsVersion(
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureConnection const& secure_connection,
    ConfigurationTypesAndDefs::SecureConnection const& other_secure_connection) noexcept;

/*!
 * \brief Validates if all server PSK identity hints have the same value if configured.
 * \param[in] global_secure_connections List of all global SecureConnections
 * \param[in] secure_ids The secure ids array referring to the SecureConnections which are part of the validation
 * \return true if all configured server PSK identity hints have the same value or no server PSK identity hint
 * configured, false if at least one pair of server PSK identity hint is different.
 * \pre -
 * \context Init
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionServerPskHintsAreTheSame(
    ConfigurationTypesAndDefs::SecureConnectionContainer const& global_secure_connections,
    ConfigurationTypesAndDefs::SecureIdContainer const& secure_ids) noexcept;

/*!
 * \brief Validate the configuration of SecureConnections used in NetworkEndpointPorts.
 * \return true if the SecureConnection configurations are consistent in all NetworkEndpointPorts, false
 * otherwise.
 * \param[in] global_secure_connections List of all global SecureConnections
 * \param[in] global_network_endpoints List of all global NetworkEndpoints
 * \param[in] logger Logger object
 * \pre -
 * \context Init
 * \reentrant FALSE
 * \steady FALSE
 */
bool ValidateSecureConnectionForNetworkEndpointPort(
    ConfigurationTypesAndDefs::SecureConnectionContainer const& global_secure_connections,
    ConfigurationTypesAndDefs::NetworkEndpointContainer const& global_network_endpoints,
    someip_daemon_core::logging::AraComLogger const& logger) noexcept;

/*!
 * \brief Checks if SecureIds in the given array matches with a SecureConnection ID from in the given SecureConnection
 * array.
 * \param[in] logger Logger object
 * \param[in] parsed_secure_ids One parsed SecureId array either from a rsi mapping or a network endpoint port.
 * \param[in] parsed_secure_connections SecureConnections to be added to the global configuration.
 * \return true, if for all SecureIds in parsed_secure_ids a SecureConnection in parsed_secure_connections with the
 * same SecureId exists, false otherwise
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \steady FALSE
 */
bool CheckIfAllSecureIdsReferringToASecureConnection(
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureIdContainer const& parsed_secure_ids,
    ConfigurationTypesAndDefs::SecureConnectionContainer const& parsed_secure_connections) noexcept;

/*!
 * \brief Checks if all parsed SecureIds configured in SecureIdArray in Rsi mapping or Network Endpoint Port for one
 * JSON are referring to a SecureConnection object.
 * \param[in] logger Logger object
 * \param[in] parsed_secure_connections  SecureConnections to be added to the global configuration.
 * \param[in] parsed_required_service_instances Required Service Instances parsed from one JSON
 * \param[in] parsed_network_endpoints Network Endpoints parsed from one JSON
 * \return true, if all SecureIds configured in all rsi and all nep are linked to a SecureConnection, false otherwise
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \steady FALSE
 */
bool CheckSecureIdsLinksForRsiAndNep(
    someip_daemon_core::logging::AraComLogger const& logger,
    ConfigurationTypesAndDefs::SecureConnectionContainer const& parsed_secure_connections,
    ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer const& parsed_required_service_instances,
    ConfigurationTypesAndDefs::NetworkEndpointContainer const& parsed_network_endpoints) noexcept;

}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_SECURE_CONNECTION_VALIDATIONS_H_
