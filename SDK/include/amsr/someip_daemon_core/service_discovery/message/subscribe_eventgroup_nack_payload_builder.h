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
/**        \file  subscribe_eventgroup_nack_payload_builder.h
 *        \brief  Payload builder for subscribe eventgroup NACK message.
 *
 *      \details  Builds a SD message payload for subscribe eventgroup NACK message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_NACK_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_NACK_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message payload builder for SubscribeEventgroupNAck entry type.
 */
class SubscribeEventgroupNAckPayloadBuilder final {
 public:
  SubscribeEventgroupNAckPayloadBuilder() = delete;
  ~SubscribeEventgroupNAckPayloadBuilder() = delete;
  SubscribeEventgroupNAckPayloadBuilder(SubscribeEventgroupNAckPayloadBuilder const&) = delete;
  SubscribeEventgroupNAckPayloadBuilder(SubscribeEventgroupNAckPayloadBuilder&&) = delete;
  SubscribeEventgroupNAckPayloadBuilder& operator=(SubscribeEventgroupNAckPayloadBuilder const&) = delete;
  SubscribeEventgroupNAckPayloadBuilder& operator=(SubscribeEventgroupNAckPayloadBuilder&&) = delete;

  /*!
   * \brief Creates a SD Message Payload containing an entry and its options from a given SubscribeEventgroupNAck Entry.
   * \param[in] entry A SubscribeEventgroupNAck entry.
   * \return A SD Message Payload containing the given entry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   *
   * \trace SPEC-4981631
   * \trace SPEC-10144802
   * \trace SPEC-10144811
   * \trace SPEC-10144812
   * \trace SPEC-10144813
   * \trace SPEC-10144815
   * \trace SPEC-10144817
   * \trace SPEC-10144818
   */
  static ServiceDiscoveryMessagePayload CreatePayload(entries::SubscribeEventgroupNAckEntry const& entry);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SUBSCRIBE_EVENTGROUP_NACK_PAYLOAD_BUILDER_H_
