/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *  This software is copyright protected and proprietary to Vector Informatik GmbH.
 *  Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *  All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_message_sender.h
 *        \brief  SD Message Sender.
 *
 *      \details  This class is responsible for sending unicast and multicast messages to the given sockets.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <utility>
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_socket.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief ServiceDiscoveryMessageSender.
 */
class ServiceDiscoveryMessageSender final {
 public:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryMessageSender() noexcept = default;

  ServiceDiscoveryMessageSender() = delete;
  ServiceDiscoveryMessageSender(ServiceDiscoveryMessageSender const &) = delete;
  ServiceDiscoveryMessageSender(ServiceDiscoveryMessageSender &&) = delete;
  ServiceDiscoveryMessageSender &operator=(ServiceDiscoveryMessageSender const &) & = delete;
  ServiceDiscoveryMessageSender &operator=(ServiceDiscoveryMessageSender &&) & = delete;

  /*!
   * \brief Constructor of ServiceDiscoveryMessageSender.
   * \param[in,out] ucast_socket Unicast socket.
   * \param[in,out] mcast_socket Multicast socket.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  ServiceDiscoveryMessageSender(UnicastUDPSocket &ucast_socket, MulticastUDPSocket &mcast_socket) noexcept;
  /*!
   * \brief Sends a unicast SOME/IP SD message to the provided IP address and port.
   * \param[in] to_address An IP address identifying the destination.
   * \param[in] to_port A UDP port identifying the destination.
   * \param[in] message_view The view of the message to be transmitted.
   * \return True If the message was sent successfully, and false otherwise.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  bool SendUnicastMessage(amsr::someip_protocol::internal::IpAddress const &to_address,
                          amsr::someip_protocol::internal::Port const to_port,
                          ara::core::Span<std::uint8_t> const &message_view) noexcept;
  /*!
   * \brief Sends a multicast SOME/IP SD message to the provided IP address and port.
   * \param[in] message_view The view of the message to be transmitted.
   * \return True If the message was sent successfully, and false otherwise.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   * \trace SPEC-10144659
   * \trace SPEC-10144660
   */
  bool SendMulticastMessage(ara::core::Span<std::uint8_t> const &message_view) noexcept;

 private:
  /*!
   * \brief A unicast UDP socket (wrapper).
   */
  UnicastUDPSocket &ucast_socket_;

  /*!
   * \brief A multicast UDP socket (wrapper).
   */
  MulticastUDPSocket &mcast_socket_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoveryMessageSender"}};
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_
