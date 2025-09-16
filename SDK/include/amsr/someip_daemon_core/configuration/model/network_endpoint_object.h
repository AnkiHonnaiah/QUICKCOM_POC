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
/*!     \file     network_endpoint_object.h
 *      \brief    Configuration object for 'network_endpoint'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief This is an intermediate representation of a 'network endpoint' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class NetworkEndpointConfigObject final {
 public:
  /*!
   * \brief Address element.
   */
  using AddressElement = CfgElement<someip_daemon_core::IpAddress>;

  /*!
   * \brief MTU element.
   */
  using MtuElement = CfgElement<someip_protocol::internal::MTU>;

  /*!
   * \brief Service discovery sub-hierarchy.
   */
  using ServiceDiscoveryElement = CfgElement<ConfigurationTypesAndDefs::NetworkEndpointServiceDiscovery>;

  /*!
   * \brief Ports sub-hierarchy.
   */
  using PortsElement = CfgElement<ConfigurationTypesAndDefs::NetworkEndpointPortContainer>;

  /*!
   * \brief Element to contain bulk read count.
   */
  using BulkReadCountElement = CfgElement<ConfigurationTypesAndDefs::BulkReadCountType>;

  /*!
   * \brief Element to contain bulk read receive period in micro seconds.
   */
  using BulkReadReceivePeriodUsElement = CfgElement<ConfigurationTypesAndDefs::BulkReadReceivePeriodUs>;

  /*!
   * \brief Element to contain bulk read maximum number of consecutive receive calls.
   */
  using BulkReadMaxNoOfConsecutiveReceiveCallsElement =
      CfgElement<ConfigurationTypesAndDefs::BulkReadMaxNoOfConsecutiveReceiveCalls>;

  /*!
   * \brief Element to contain bulk read minimum number of datagrams to continue consecutive receive calls.
   */
  using BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsElement =
      CfgElement<ConfigurationTypesAndDefs::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls>;

  /*!
   * \brief Element contain the Tcp Nagle enabled setting.
   */
  using TcpNagleEnabledElement = CfgElement<ConfigurationTypesAndDefs::TcpNagleEnabledType>;

  /*!
   * \brief Element contain the Tcp Keep Alive setting.
   */
  using TcpKeepAliveElement = CfgElement<ConfigurationTypesAndDefs::TcpKeepAliveType>;

  /*!
   * \brief Element containing the Quality of Service (Qos, vlanPriority) setting.
   */
  using QosElement = CfgElement<someip_protocol::internal::QoSOption>;

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
   * \param[in] network_endpoint Reference to the 'network endpoint' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit NetworkEndpointConfigObject(ConfigurationTypesAndDefs::NetworkEndpoint& network_endpoint) noexcept
      : address_{network_endpoint.address_},
        mtu_{network_endpoint.mtu_},
        service_discovery_{network_endpoint.service_discovery_},
        ports_{network_endpoint.ports_},
        elem_bulk_read_count_{network_endpoint.bulk_read_count_},
        elem_bulk_read_receive_period_us_{network_endpoint.bulk_read_receive_period_us_},
        elem_bulk_read_max_no_of_consecutive_receive_calls_{
            network_endpoint.bulk_read_max_no_of_consecutive_receive_calls},
        elem_bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls_{
            network_endpoint.bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls},
        elem_tcp_nagle_enabled_{network_endpoint.tcp_nagle_enabled_},
        elem_tcp_keep_alive_{network_endpoint.tcp_keep_alive_},
        qos_{network_endpoint.qos_},
        no_of_invalid_tcp_messages_before_disconnection_{
            network_endpoint.no_of_invalid_tcp_messages_before_disconnection_},
        no_of_consecutive_valid_tcp_messages_before_error_reset_{
            network_endpoint.no_of_consecutive_valid_tcp_messages_before_error_reset_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointConfigObject(NetworkEndpointConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointConfigObject(NetworkEndpointConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointConfigObject const&) & -> NetworkEndpointConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(NetworkEndpointConfigObject&& other) & noexcept -> NetworkEndpointConfigObject& = default;

  /*!
   * \brief Set the Address when the parser finds the Address.
   * \param[in] address Address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetAddress(someip_daemon_core::IpAddress const& address) noexcept { address_.SetElement(address); }

  /*!
   * \brief Getter for the Address element.
   * \return Reference to the Address  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  AddressElement const& GetAddress() const noexcept { return address_; }

  /*!
   * \brief Set the Mtu when the parser finds the Mtu.
   * \param[in] mtu Mtu.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMtu(someip_protocol::internal::MTU const& mtu) noexcept { mtu_.SetElement(mtu); }

  /*!
   * \brief Getter for the Mtu element.
   * \return Reference to the Mtu  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MtuElement const& GetMtu() const noexcept { return mtu_; }

  /*!
   * \brief Set the Service discovery sub-hierarchy Service discovery.
   * \param[in] service_discovery Service discovery.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServiceDiscovery(
      ConfigurationTypesAndDefs::NetworkEndpointServiceDiscovery const& service_discovery) noexcept {
    service_discovery_.SetElement(service_discovery);
  }

  /*!
   * \brief Getter for the Service discovery sub-hierarchy.
   * \return Reference to the Service discovery sub-hierarchy of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceDiscoveryElement const& GetServiceDiscovery() const noexcept { return service_discovery_; }

  /*!
   * \brief Set the Ports sub-hierarchy Ports.
   * \param[in] ports Ports.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPorts(ConfigurationTypesAndDefs::NetworkEndpointPortContainer const& ports) noexcept {
    ports_.SetElement(ports);
  }

  /*!
   * \brief Setter for the bulk read count.
   * \param[in] bulk_read_count Bulk read count.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBulkReadCount(ConfigurationTypesAndDefs::BulkReadCountType const bulk_read_count) noexcept {
    elem_bulk_read_count_.SetElement(bulk_read_count);
  }

  /*!
   * \brief Getter for the bulk read count.
   * \return Reference to the element containing bulk read count.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  BulkReadCountElement const& GetBulkReadCount() const noexcept { return elem_bulk_read_count_; }

  /*!
   * \brief Setter for the bulk read receive period.
   * \param[in] bulk_read_receive_period_us Bulk read receive period in micro seconds.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBulkReadReceivePeriodUs(
      ConfigurationTypesAndDefs::BulkReadReceivePeriodUsType const bulk_read_receive_period_us) noexcept {
    elem_bulk_read_receive_period_us_.SetElement(
        ConfigurationTypesAndDefs::BulkReadReceivePeriodUs{bulk_read_receive_period_us});
  }

  /*!
   * \brief Setter for the bulk read maximum number of consecutive receive calls.
   * \param[in] bulk_read_max_no_of_consecutive_receive_calls Bulk read maximum number of consecutive receive calls.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBulkReadMaxNoOfConsecutiveReceiveCallsElement(
      ConfigurationTypesAndDefs::BulkReadMaxNoOfConsecutiveReceiveCalls const
          bulk_read_max_no_of_consecutive_receive_calls) noexcept {
    elem_bulk_read_max_no_of_consecutive_receive_calls_.SetElement(
        ConfigurationTypesAndDefs::BulkReadMaxNoOfConsecutiveReceiveCalls{
            bulk_read_max_no_of_consecutive_receive_calls});
  }

  /*!
   * \brief Getter for the bulk read maximum number of consecutive receive calls.
   * \return Reference to the element containing bulk read maximum number of consecutive receive calls
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetBulkReadMaxNoOfConsecutiveReceiveCalls() const noexcept
      -> BulkReadMaxNoOfConsecutiveReceiveCallsElement const& {
    return elem_bulk_read_max_no_of_consecutive_receive_calls_;
  }

  /*!
   * \brief Setter for the bulk read minimum number of datagram to continue consecutive receive calls.
   * \param[in] bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls Bulk read minimum number of datagram
   * to continue consecutive receive calls.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsElement(
      ConfigurationTypesAndDefs::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls const
          bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls) noexcept {
    elem_bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls_.SetElement(
        ConfigurationTypesAndDefs::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls{
            bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls});
  }

  /*!
   * \brief Getter for the bulk read minimum number of datagram to continue consecutive receive calls.
   * \return Reference to the element containing bulk read minimum number of datagram to continue consecutive receive
   * calls.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls() const noexcept
      -> BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsElement const& {
    return elem_bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls_;
  }

  /*!
   * \brief Setter for the Tcp Nagle Option.
   * \param[in] enable_tcp_nagle Boolean value to enable or disable the parameter.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetOptionTcpNagle(ConfigurationTypesAndDefs::TcpNagleEnabledType const enable_tcp_nagle) noexcept {
    elem_tcp_nagle_enabled_.SetElement(enable_tcp_nagle);
  }

  /*!
   * \brief Getter for the Tcp Nagle Option.
   * \return true if the option is enabled, otherwise false.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TcpNagleEnabledElement const& GetOptionTcpNagle() const noexcept { return elem_tcp_nagle_enabled_; }

  /*!
   * \brief Setter for the General KeepAlive Option.
   * \param[in] keep_alive_params Reference to the KeepAlive parameters.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetOptionTcpKeepAlive(someip_protocol::internal::KeepAliveParameters const& keep_alive_params) noexcept {
    elem_tcp_keep_alive_.SetElement(ConfigurationTypesAndDefs::TcpKeepAliveType{keep_alive_params});
  }

  /*!
   * \brief Set the Qos option.
   * \param[in] qos_priority Reference to the Qos setting.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetQos(someip_protocol::internal::QoSPriority const& qos_priority) noexcept {
    qos_.SetElement(someip_protocol::internal::QoSOption(qos_priority));
  }

  /*!
   * \brief Getter for Qos element.
   * \return Reference to the Qos element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  QosElement const& GetQos() const noexcept { return qos_; }

  /*!
   * \brief Get the allowed UDP payload in bytes, derived from the MTU and address type.
   * \return MTU with maximum size of UDP+IP headers removed. If unset some error condition happened.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetUdpPayloadBytes() const noexcept -> ara::core::Optional<someip_protocol::internal::MTU> {
    using mtu_t = someip_protocol::internal::MTU;
    ara::core::Optional<mtu_t> ret{};
    mtu_t overhead{0};

    if (GetAddress().GetStatus() == ElementStatus::kSet) {
      using Udp = amsr::net::ip::Udp;
      ara::core::Result<someip_daemon_core::IpAddress> address{GetAddress().GetElement()};
      bool const is_v6{address.HasValue() && (!address.Value().IsV4())};
      constexpr mtu_t overhead_v4{mtu_t{Udp::kIPv4HeaderLengthMax} + mtu_t{Udp::kUdpHeaderLength}};
      constexpr mtu_t overhead_v6{mtu_t{Udp::kIPv6HeaderLengthMin} + mtu_t{Udp::kUdpHeaderLength}};
      overhead = is_v6 ? overhead_v6 : overhead_v4;
    }
    if ((overhead != 0) && (GetMtu().GetStatus() == ElementStatus::kSet)) {
      mtu_t const mtu{GetMtu().GetElement()};
      if (mtu >= overhead) {
        ret.emplace(mtu - overhead);
      }
    }
    return ret;
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
   * \brief The Address element.
   */
  AddressElement address_;

  /*!
   * \brief The MTU element.
   */
  MtuElement mtu_;

  /*!
   * \brief The Service discovery sub-hierarchy.
   */
  ServiceDiscoveryElement service_discovery_;

  /*!
   * \brief The Ports sub-hierarchy.
   */
  PortsElement ports_;

  /*!
   * \brief Element to contain bulk read count.
   */
  BulkReadCountElement elem_bulk_read_count_;

  /*!
   * \brief Element to contain bulk read receive period in micro seconds.
   */
  BulkReadReceivePeriodUsElement elem_bulk_read_receive_period_us_;

  /*!
   * \brief Element to contain bulk read maximum number of consecutive receive calls.
   */
  BulkReadMaxNoOfConsecutiveReceiveCallsElement elem_bulk_read_max_no_of_consecutive_receive_calls_;

  /*!
   * \brief Element to contain bulk read minimum number of datagrams to continue consecutive receive calls.
   */
  BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsElement
      elem_bulk_read_min_no_of_datagram_to_continue_consecutive_receive_calls_;

  /*!
   * \brief Element to contain Tcp Nagle enabled setting.
   */
  TcpNagleEnabledElement elem_tcp_nagle_enabled_;

  /*!
   * \brief Element to contain Tcp KeepAlive enabled setting.
   */
  TcpKeepAliveElement elem_tcp_keep_alive_;

  /*!
   * \brief Element to contain the Qos setting.
   */
  QosElement qos_;

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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_OBJECT_H_
