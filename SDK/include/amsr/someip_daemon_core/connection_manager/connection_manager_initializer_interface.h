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
/*!        \file
 *        \brief  Interface responsible for providing Connection Manager network initialization APIs
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_INITIALIZER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_INITIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Provides Connection Manager network initialization APIs
 * \vprivate        Vector component internal API
 */
class ConnectionManagerInitializerInterface {
 protected:
  // Private aliases for simplified naming
  using ConfigProtocol = configuration::Configuration::Protocol;
  using ConfigNetworkEndpoint = configuration::ConfigurationTypesAndDefs::NetworkEndpoint;
  using ConfigNetworkEndpointPort = configuration::ConfigurationTypesAndDefs::NetworkEndpointPort;

 public:
  /*!
   * \brief           Default constuctor
   * \steady          TRUE
   */
  ConnectionManagerInitializerInterface() noexcept = default;
  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  virtual ~ConnectionManagerInitializerInterface() noexcept = default;

  ConnectionManagerInitializerInterface(ConnectionManagerInitializerInterface const&) = delete;
  ConnectionManagerInitializerInterface(ConnectionManagerInitializerInterface&&) = delete;
  ConnectionManagerInitializerInterface& operator=(ConnectionManagerInitializerInterface const&) = delete;
  ConnectionManagerInitializerInterface& operator=(ConnectionManagerInitializerInterface&&) = delete;

  /*!
   * \brief           Creates a communication network endpoint for use with TCP
   *
   * \param[in]       network_endpoint
   *                  The configuration network endpoint.
   * \param[in]       network_endpoint_port
   *                  The configuration network endpoint port.
   * \param[in]       is_secured
   *                  True if the endpoint should support using TLS; false for plain TCP.
   *
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  virtual void CreateTcpCommunicationNetworkEndpoint(ConfigNetworkEndpoint const& network_endpoint,
                                                     ConfigNetworkEndpointPort const& network_endpoint_port,
                                                     bool is_secured) noexcept = 0;

  /*!
   * \brief           Creates a communication network endpoint for use with UDP
   *
   * \param[in]       network_endpoint
   *                  The configuration network endpoint.
   * \param[in]       network_endpoint_port
   *                  The configuration network endpoint port.
   * \param[in]       is_secured
   *                  True if the endpoint should support using DTLS; false for plain UDP.
   *
   * \context         Init
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual void CreateUdpCommunicationNetworkEndpoint(ConfigNetworkEndpoint const& network_endpoint,
                                                     ConfigNetworkEndpointPort const& network_endpoint_port,
                                                     bool is_secured) noexcept = 0;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_INITIALIZER_INTERFACE_H_
