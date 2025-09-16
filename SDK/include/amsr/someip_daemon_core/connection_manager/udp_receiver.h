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
 *        \brief  SOME/IP UDP receiver class
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RECEIVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RECEIVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class UdpEndpoint;

/*!
 * \brief           Declaration of SOME/IP UDP receiver class, UdpReceiver
 */
class UdpReceiver final {
 public:
  /*!
   * \brief           A UDP endpoint pointer type
   */
  using UdpEndpointPtr = std::shared_ptr<UdpEndpoint>;

  /*!
   * \brief           Constructs instance of UdpReceiver
   * \param[in]       endpoint
   *                  A UDP endpoint this receiver belongs to.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  explicit UdpReceiver(UdpEndpointPtr endpoint) noexcept;

  /*!
   * \brief           Destructs instance of UdpReceiver
   * \context         Shutdown
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ~UdpReceiver() noexcept;

  /*!
   * \brief           Default copy constructor
   * \steady          TRUE
   */
  UdpReceiver(UdpReceiver const&) noexcept = default;

  /*!
   * \brief           Default move constructor
   * \steady          TRUE
   */
  UdpReceiver(UdpReceiver&&) noexcept = default;

  /*!
   * \brief           Default copy assignment operator
   * \steady          TRUE
   */
  UdpReceiver& operator=(UdpReceiver const&) & noexcept = default;

  /*!
   * \brief           Default move assignment operator
   * \steady          TRUE
   */
  UdpReceiver& operator=(UdpReceiver&&) & noexcept = default;

  /*!
   * \brief           Registers a required SOME/IP service instance.n
   *
   * \param[in]       service_instance_id
   *                  A SOME/IP service instance id containing instance id, service id and major version.
   *
   * \context         ANY
   * \pre             A user shall only once register a required service instance.
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  void RegisterRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
      amsr::net::ip::Udp::Endpoint const endpoint) noexcept;

  /*!
   * \brief           Build PDU receiver remapper
   * \param[in]       service_id
   *                  A SOME/IP service id.
   * \param[in]       required_pdus
   *                  A list of required PDUs.
   * \context         Init, Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  void BuildPduReceptionMappings(
      someip_protocol::internal::ServiceId const service_id,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus) const noexcept;

 private:
  /*!
   * \brief           Unregisters a registered required SOME/IP service instance
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  void UnregisterRequiredServiceInstance() noexcept;

  /*!
   * \brief           A UDP endpoint this receiver belongs to
   */
  UdpEndpointPtr endpoint_;

  /*!
   * \brief           This receiver's required service instance
   */
  ara::core::Optional<configuration::types::SomeIpServiceInstanceIdCommunication> required_service_instance_{
      ara::core::nullopt};

  /*!
   * \brief           This receiver's unicast sender endpoint
   */
  ara::core::Optional<amsr::net::ip::Udp::Endpoint> sender_{ara::core::nullopt};

  /*!
   * \brief           Friend test to access the private members required_service_instance_ and sender_
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpReceiver, RegisterRequiredServiceInstance);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RECEIVER_H_
