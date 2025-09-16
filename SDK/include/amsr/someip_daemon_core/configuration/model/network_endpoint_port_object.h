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
/*!     \file     network_endpoint_port_object.h
 *      \brief    Configuration object for 'network_endpoint_port'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief This is an intermediate representation of a 'Network endpoint port' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class NetworkEndpointPortConfigObject final {
 public:
  /*!
   * \brief Port element.
   */
  using PortElement = CfgElement<someip_protocol::internal::Port>;

  /*!
   * \brief Proto element.
   */
  using ProtoElement = CfgElement<ConfigurationTypesAndDefs::Protocol>;

  /*!
   * \brief QoS sub-hierarchy.
   */
  using QosElement = CfgElement<someip_protocol::internal::QoSOption>;

  /*!
   * \brief Keep alive sub-hierarchy.
   */
  using KeepAliveElement = CfgElement<someip_protocol::internal::TcpKeepAliveOption>;

  /*!
   * \brief Socket Option Linger sub-hierarchy.
   */
  using SocketOptionLingerElement = CfgElement<someip_protocol::internal::EnableSocketOptionLingerOptional>;

  /*!
   * \brief Socket Option TCP Nagle sub-hierarchy.
   */
  using SocketOptionTcpNaglesAlgorithmElement =
      CfgElement<someip_protocol::internal::SocketOptionTcpNaglesAlgorithmOptional>;

  /*!
   * \brief Socket Option Bulk read receive period [us] sub-hierarchy
   */
  using BulkReadReceivePeriodUsElement = CfgElement<someip_protocol::internal::BulkReadReceivePeriodUs>;

  /*!
   * \brief Number of invalid tcp messages before disconnection sub-hierarchy.
   */
  using NoOfInvalidTcpMessagesBeforeDisconnectionElement =
      CfgElement<someip_protocol::internal::TcpMessageCounterThresholdType>;

  /*!
   * \brief Number of consecutive valid tcp messages before error reset sub-hierarchy.
   */
  using NoOfConsecutiveValidTcpMessagesBeforeErrorResetElement =
      CfgElement<someip_protocol::internal::TcpMessageCounterThresholdType>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] network_endpoint_port Reference to the 'Network endpoint port' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit NetworkEndpointPortConfigObject(
      ConfigurationTypesAndDefs::NetworkEndpointPort& network_endpoint_port) noexcept
      : port_{network_endpoint_port.port_},
        proto_{network_endpoint_port.proto_},
        qos_{network_endpoint_port.options_.qos_},
        keep_alive_{network_endpoint_port.options_.keep_alive_},
        enable_so_linger_{network_endpoint_port.options_.enable_so_linger_},
        enable_tcp_nagle_{network_endpoint_port.options_.enable_tcp_nagle_},
        bulk_read_receive_period_us_{network_endpoint_port.options_.bulk_read_receive_period_us_},
        no_of_invalid_tcp_messages_before_disconnection_{
            network_endpoint_port.options_.no_of_invalid_tcp_messages_before_disconnection_},
        no_of_consecutive_valid_tcp_messages_before_error_reset_{
            network_endpoint_port.options_.no_of_consecutive_valid_tcp_messages_before_error_reset_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointPortConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointPortConfigObject(NetworkEndpointPortConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointPortConfigObject(NetworkEndpointPortConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointPortConfigObject const&) & -> NetworkEndpointPortConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(NetworkEndpointPortConfigObject&& other) & noexcept -> NetworkEndpointPortConfigObject& = default;

  /*!
   * \brief Set the Port when the parser finds the Port.
   * \param[in] port Port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPort(someip_protocol::internal::Port const& port) noexcept { port_.SetElement(port); }

  /*!
   * \brief Getter for the Port element.
   * \return Reference to the Port  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PortElement const& GetPort() const noexcept { return port_; }

  /*!
   * \brief Set the Proto when the parser finds the Proto.
   * \param[in] proto Proto.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetProto(ConfigurationTypesAndDefs::Protocol const& proto) noexcept { proto_.SetElement(proto); }

  /*!
   * \brief Getter for the Proto element.
   * \return Reference to the Proto  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ProtoElement const& GetProto() const noexcept { return proto_; }

  /*!
   * \brief Set the Qos option.
   * \param[in] qos_priority QoS priority to be used.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetQos(someip_protocol::internal::QoSPriority const qos_priority) noexcept {
    qos_.SetElement(someip_protocol::internal::QoSOption(qos_priority));
  }

  /*!
   * \brief Getter for KeepAlive element.
   * \return Reference to the QoS element of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  QosElement const& GetQos() const noexcept { return qos_; }

  /*!
   * \brief Set the KeepAlive sub-hierarchy.
   * \param[in] keep_alive Reference to the KeepAlive parameters.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetKeepAlive(someip_protocol::internal::KeepAliveParameters const& keep_alive) noexcept {
    keep_alive_.SetElement(someip_protocol::internal::TcpKeepAliveOption(keep_alive));
  }

  /*!
   * \brief Getter for KeepAlive element.
   * \return Reference to the KeepAlive element of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  KeepAliveElement const& GetKeepAlive() const noexcept { return keep_alive_; }

  /*!
   * \brief Set the Socket Option Linger sub-hierarchy.
   * \param[in] enable_so_linger Boolean value to enable or disable the parameter.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSocketOptionLinger(bool const enable_so_linger) noexcept { enable_so_linger_.SetElement(enable_so_linger); }

  /*!
   * \brief Getter for Socket Option Linger element.
   * \return true if the option is enabled, otherwise false.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SocketOptionLingerElement const& GetSocketOptionLinger() const noexcept { return enable_so_linger_; }

  /*!
   * \brief Set the Socket Option Nagle's algorithm sub-hierarchy.
   * \param[in] enable_tcp_nagle Boolean value to enable or disable the parameter.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSocketOptionTcpNagle(bool const enable_tcp_nagle) noexcept { enable_tcp_nagle_.SetElement(enable_tcp_nagle); }

  /*!
   * \brief Getter for Socket Option Nagle's algorithm element.
   * \return true if the option is enabled, otherwise false.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SocketOptionTcpNaglesAlgorithmElement const& GetSocketOptionTcpNagle() const noexcept { return enable_tcp_nagle_; }

  /*!
   * \brief Set the Socket Option Bulk read receive period [us] element.
   * \param[in] bulk_read_receive_period_us Reference to Bulk read receive period [us].
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSocketOptionBulkReadReceivePeriodUs(
      someip_protocol::internal::BulkReadReceivePeriodUsType const& bulk_read_receive_period_us) noexcept {
    bulk_read_receive_period_us_.SetElement(
        ConfigurationTypesAndDefs::BulkReadReceivePeriodUs{bulk_read_receive_period_us});
  }

  /*!
   * \brief Getter for Socket Option number of invalid tcp messages before disconnection element.
   * \return Reference to the number of invalid tcp messages before disconnection element of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetNoOfInvalidTcpMessagesBeforeDisconnection() const noexcept
      -> NoOfInvalidTcpMessagesBeforeDisconnectionElement const& {
    return no_of_invalid_tcp_messages_before_disconnection_;
  }

  /*!
   * \brief Set the Socket Option number of invalid tcp messages before disconnection element.
   * \param[in] no_of_invalid_tcp_messages_before_disconnection Reference to number of invalid tcp messages before
   * disconnection value.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetNoOfInvalidTcpMessagesBeforeDisconnection(someip_protocol::internal::TcpMessageCounterThresholdType const&
                                                        no_of_invalid_tcp_messages_before_disconnection) noexcept {
    no_of_invalid_tcp_messages_before_disconnection_.SetElement(no_of_invalid_tcp_messages_before_disconnection);
  }

  /*!
   * \brief Getter for Socket Option number of of consecutive valid tcp messages before error reset element.
   * \return Reference to the number of of consecutive valid tcp messages before error reset element of this
   * intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetNoOfConsecutiveValidTcpMessagesBeforeErrorReset() const noexcept
      -> NoOfConsecutiveValidTcpMessagesBeforeErrorResetElement const& {
    return no_of_consecutive_valid_tcp_messages_before_error_reset_;
  }

  /*!
   * \brief Set the Socket Option number of of consecutive valid tcp messages before error reset.
   * \param[in] no_of_consecutive_valid_tcp_messages_before_error_reset Reference to number of of consecutive valid tcp
   * messages before error reset value.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetNoOfConsecutiveValidTcpMessagesBeforeErrorReset(
      someip_protocol::internal::TcpMessageCounterThresholdType const&
          no_of_consecutive_valid_tcp_messages_before_error_reset) noexcept {
    no_of_consecutive_valid_tcp_messages_before_error_reset_.SetElement(
        no_of_consecutive_valid_tcp_messages_before_error_reset);
  }

 private:
  /*!
   * \brief The Port element.
   */
  PortElement port_;

  /*!
   * \brief The Proto element.
   */
  ProtoElement proto_;

  /*!
   * \brief The QoS option.
   */
  QosElement qos_;

  /*!
   * \brief The TCP keep alive socket option.
   */
  KeepAliveElement keep_alive_;

  /*!
   * \brief The TCP socket option lingering.
   */
  SocketOptionLingerElement enable_so_linger_;

  /*!
   * \brief The TCP socket option for Nagle's algorithm.
   */
  SocketOptionTcpNaglesAlgorithmElement enable_tcp_nagle_;

  /*!
   * \brief The Bulk read receive period [us] option.
   */
  BulkReadReceivePeriodUsElement bulk_read_receive_period_us_;

  /*!
   * \brief The number of invalid tcp messages before disconnection option.
   */
  NoOfInvalidTcpMessagesBeforeDisconnectionElement no_of_invalid_tcp_messages_before_disconnection_;

  /*!
   * \brief The number of consecutive valid tcp messages before error reset option.
   */
  NoOfConsecutiveValidTcpMessagesBeforeErrorResetElement no_of_consecutive_valid_tcp_messages_before_error_reset_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_OBJECT_H_
