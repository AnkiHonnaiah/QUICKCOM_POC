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
/**        \file  eventgroup_subscriber.h
 *        \brief  Container structure of an eventgroup subscriber, with optional TCP and UDP senders
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIBER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIBER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon_core {
namespace server {
namespace eventgroup {

/*!
 * \brief Represents an eventgroup subscriber.
 *
 * \tparam UdpSenderPtr Type of the UDP sender
 * \tparam TcpSenderPtr Type of the TCP sender
 */
template <typename UdpSenderPtr, typename TcpSenderPtr>
struct EventgroupSubscriber {
  /*!
   * \brief An optional TCP sender
   */
  TcpSenderPtr tcp_sender{nullptr};
  /*!
   * \brief An optional UDP sender
   */
  UdpSenderPtr udp_sender{nullptr};
};

}  // namespace eventgroup
}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIBER_H_
