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
/**        \file  stop_subscribe_eventgroup_payload_builder.h
 *        \brief  Payload builder for stop subscribe eventgroup message.
 *
 *      \details  Builds a SD message payload for stop subscribe eventgroup message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_SUBSCRIBE_EVENTGROUP_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_SUBSCRIBE_EVENTGROUP_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message payload builder for StopSubscribeEventgroup entry type.
 */
class StopSubscribeEventgroupPayloadBuilder final {
 public:
  StopSubscribeEventgroupPayloadBuilder() = delete;
  ~StopSubscribeEventgroupPayloadBuilder() = delete;
  StopSubscribeEventgroupPayloadBuilder(StopSubscribeEventgroupPayloadBuilder const&) = delete;
  StopSubscribeEventgroupPayloadBuilder(StopSubscribeEventgroupPayloadBuilder&&) = delete;
  StopSubscribeEventgroupPayloadBuilder& operator=(StopSubscribeEventgroupPayloadBuilder const&) = delete;
  StopSubscribeEventgroupPayloadBuilder& operator=(StopSubscribeEventgroupPayloadBuilder&&) = delete;
  /*!
   * \brief Creates a SD Message Payload containing an entry and its options from a given StopSubscribeEventgroup Entry.
   * \param[in] entry A StopSubscribeEventgroup entry.
   * \return A SD Message Payload containing the given entry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   *
   * \trace SPEC-4981625
   * \trace SPEC-10144552
   * \trace SPEC-10144558
   * \trace SPEC-10144561
   * \trace SPEC-10144562
   * \trace SPEC-10144563
   * \trace SPEC-10144564
   * \trace SPEC-10144566
   * \trace SPEC-10144775
   * \trace SPEC-10144784
   * \trace SPEC-10144786
   * \trace SPEC-10144787
   * \trace SPEC-10144792
   * \trace SPEC-10144796
   * \trace SPEC-10144797
   */
  static ServiceDiscoveryMessagePayload CreatePayload(entries::StopSubscribeEventgroupEntry const& entry);

 private:
  /*!
   * \brief Getting the Unicast SD message type.
   *
   * \param[in] address Container of address details.
   *
   * \return An enum class value corresponding to unicast SD type.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static options::SomeIpSdEndpointOptionType GetUnicastSdMessageType(someip_daemon_core::IpAddress const& address);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_SUBSCRIBE_EVENTGROUP_PAYLOAD_BUILDER_H_
