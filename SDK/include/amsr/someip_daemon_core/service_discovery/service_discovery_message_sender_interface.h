/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH. Vector Informatik GmbH grants to you only those rights as
 * set out in the license conditions. All other rights remain
 * with Vector Informatik GmbH. \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_message_sender_interface.h
 *        \brief  SD Message Sender Interface
 *
 *      \details  Interface for SD message payload sending via multicast/unicast.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief This interface defines the APIs to send SD Message Payload via Unicast or Multicast.
 *        This has to be implemented by the SdEndpoint.
 */
class ServiceDiscoveryMessageSenderInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryMessageSenderInterface() noexcept = default;
  /*!
   * \brief Define default destructor.
   *
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryMessageSenderInterface() noexcept = default;

  ServiceDiscoveryMessageSenderInterface(ServiceDiscoveryMessageSenderInterface const &) = delete;
  ServiceDiscoveryMessageSenderInterface(ServiceDiscoveryMessageSenderInterface &&) = delete;
  ServiceDiscoveryMessageSenderInterface &operator=(ServiceDiscoveryMessageSenderInterface const &) & = delete;
  ServiceDiscoveryMessageSenderInterface &operator=(ServiceDiscoveryMessageSenderInterface &&) & = delete;

  /*!
   * \brief Sends a unicast SOME/IP SD message to the provided IP address and port.
   *
   * \param receiver_address Pair of IP address and port identifying the destination.
   * \param sd_message The SOME/IP SD message to be sent.
   *
   * \return true If the message was sent successfully, and false otherwise.
   *
   * \context Timer
   * \steady FALSE
   */
  virtual bool SendUnicastMessage(amsr::someip_protocol::internal::AddressPair const &receiver_address,
                                  message::ServiceDiscoveryMessage const &sd_message) = 0;

  /*!
   * \brief Sends a multicast SOME/IP SD message to the provided IP address and port.
   *
   * \param sd_message The SOME/IP SD message to be sent.
   *
   * \return true If the message was sent successfully, and false otherwise.
   *
   * \context Timer, Network, App
   * \steady FALSE
   */
  virtual bool SendMulticastMessage(message::ServiceDiscoveryMessage const &sd_message) = 0;
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_INTERFACE_H_
