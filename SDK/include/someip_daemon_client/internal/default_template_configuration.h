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
/*!        \file  someip_daemon_client/internal/default_template_configuration.h
 *        \brief  SomeIpDaemonClient default Template Configuration
 *
 *      \details  Default template parameters for the SomeIpDaemonClient classes (including ProxySomeIpDaemonClient
 *                and SkeletonSomeIpDaemonClient), combined in a struct for testability.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_DEFAULT_TEMPLATE_CONFIGURATION_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_DEFAULT_TEMPLATE_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip_daemon_client/internal/aracom_ipc_connection.h"
#include "someip_daemon_client/internal/ipc_connection_wrapper.h"
#include "someip_daemon_client/internal/message_receiver.h"
#include "someip_daemon_client/internal/message_sender.h"
#include "someip_daemon_client/internal/offer_service_controller.h"
#include "someip_daemon_client/internal/pdu_packet_sender.h"
#include "someip_daemon_client/internal/proxy_someip_daemon_client.h"
#include "someip_daemon_client/internal/receive_routing_controller.h"
#include "someip_daemon_client/internal/release_local_server_controller.h"
#include "someip_daemon_client/internal/release_service_controller.h"
#include "someip_daemon_client/internal/request_local_server_controller.h"
#include "someip_daemon_client/internal/request_service_controller.h"
#include "someip_daemon_client/internal/skeleton_someip_daemon_client.h"
#include "someip_daemon_client/internal/someip_daemon_client.h"
#include "someip_daemon_client/internal/someip_packet_sender.h"
#include "someip_daemon_client/internal/start_service_discovery_controller.h"
#include "someip_daemon_client/internal/stop_offer_service_controller.h"
#include "someip_daemon_client/internal/stop_service_discovery_controller.h"
#include "someip_daemon_client/internal/subscribe_event_controller.h"
#include "someip_daemon_client/internal/unsubscribe_event_controller.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Contains required types and constants for application.
 */
struct SomeIpDaemonClientDefaultTemplateConfiguration {
  /*!
   * \brief The type of a class responsible for handling the IPC communication.
   */
  using IpcConnectionWrapperType = IpcConnectionWrapper<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The type of a class responsible for handling the reception of IPC messages.
   */
  using MessageReceiverType = MessageReceiver<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The type of a class responsible for handling the sending of IPC messages.
   */
  using MessageSenderType = MessageSender<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The type of a class responsible for handling the sending of IPC messages.
   */
  using SomeIpPacketSenderType = SomeIpPacketSender<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The type of a class responsible for handling the sending of PDU messages.
   */
  using PduPacketSenderType = PduPacketSender<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The specific type of a class responsible for handling the BasicIpc communication.
   */
  using ConnectionConfigType = AraComIpcConnection<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for message forwarding and event notification.
   */
  using SomeIpDaemonClientType = SomeIpDaemonClient<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for Start service discovery command forwarding and receiving.
   */
  using StartServiceDiscoveryControllerType =
      StartServiceDiscoveryController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for request service command forwarding and receiving.
   */
  using RequestServiceControllerType = RequestServiceController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for requesting local server command forwarding and receiving.
   */
  using RequestLocalServerControllerType = RequestLocalServerController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for releasing local server command forwarding and receiving.
   */
  using ReleaseLocalServerControllerType = ReleaseLocalServerController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for offer service command forwarding and receiving.
   */
  using OfferServiceControllerType = OfferServiceController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for release service command forwarding and receiving.
   */
  using ReleaseServiceControllerType = ReleaseServiceController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for stop service discovery command forwarding and receiving.
   */
  using StopServiceDiscoveryControllerType =
      StopServiceDiscoveryController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for stop offer service command forwarding and receiving.
   */
  using StopOfferServiceControllerType = StopOfferServiceController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for subscribe event command forwarding and receiving.
   */
  using SubscribeEventControllerType = SubscribeEventController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for unsubscribe event command forwarding and receiving.
   */
  using UnsubscribeEventControllerType = UnsubscribeEventController<SomeIpDaemonClientDefaultTemplateConfiguration>;

  /*!
   * \brief The class type used for SOME/IP message routing.
   */
  using RoutingControllerType = ReceiveRoutingController;
  /*!
   * \brief The class type used for message forwarding and event notification for proxies.
   */
  using ProxySomeIpDaemonClientType = ProxySomeIpDaemonClient<SomeIpDaemonClientDefaultTemplateConfiguration>;
  /*!
   * \brief The class type used for message forwarding and receiving for skeletons.
   */
  using SkeletonSomeIpDaemonClientType = SkeletonSomeIpDaemonClient<SomeIpDaemonClientDefaultTemplateConfiguration>;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_DEFAULT_TEMPLATE_CONFIGURATION_H_
