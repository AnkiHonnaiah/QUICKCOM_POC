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
/**        \file  subscribe_eventgroup_ack_payload_builder.h
 *        \brief  Payload builder for subscribe eventgroup ACK message.
 *
 *      \details  Builds a SD message payload for subscribe eventgroup ACK message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_ACK_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_ACK_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message payload builder for SubscribeEventgroupAck entry type.
 */
class SubscribeEventgroupAckPayloadBuilder final {
 public:
  SubscribeEventgroupAckPayloadBuilder() = delete;
  ~SubscribeEventgroupAckPayloadBuilder() = delete;
  SubscribeEventgroupAckPayloadBuilder(SubscribeEventgroupAckPayloadBuilder const&) = delete;
  SubscribeEventgroupAckPayloadBuilder(SubscribeEventgroupAckPayloadBuilder&&) = delete;
  SubscribeEventgroupAckPayloadBuilder& operator=(SubscribeEventgroupAckPayloadBuilder const&) = delete;
  SubscribeEventgroupAckPayloadBuilder& operator=(SubscribeEventgroupAckPayloadBuilder&&) = delete;

  /*!
   * \brief Creates a SD Message Payload containing an entry and its options from a given SubscribeEventgroupAck Entry.
   * \param[in] entry a SubscribeEventgroupAck entry.
   * \return A SD Message Payload containing the given entry.
   * \pre -
   * \context Timer
   * \steady TRUE
   * \reentrant FALSE
   *
   * \trace SPEC-4981628, SPEC-4981629
   * \trace SPEC-10144573
   * \trace SPEC-10144574
   * \trace SPEC-10144576
   * \trace SPEC-10144577
   * \trace SPEC-10144578
   * \trace SPEC-10144580
   * \trace SPEC-10144802
   * \trace SPEC-10144811
   * \trace SPEC-10144812
   * \trace SPEC-10144813
   * \trace SPEC-10144814
   * \trace SPEC-10144817
   * \trace SPEC-10144818
   */
  static ServiceDiscoveryMessagePayload CreatePayload(entries::SubscribeEventgroupAckEntry const& entry);

 private:
  /*!
   * \brief Getting the Multicast SD message type.
   *
   * \param[in] address Container of address details.
   *
   * \return An enum class value corresponding to multicast SD type.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static options::SomeIpSdEndpointOptionType GetMulticastSdMessageType(someip_daemon_core::IpAddress const& address);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_ACK_PAYLOAD_BUILDER_H_
